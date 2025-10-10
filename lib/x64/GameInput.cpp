// Copyright (c) Microsoft Corporation.  All rights reserved.

#include <windows.h>
#include <unknwn.h>
#include <stdint.h>

namespace GameInput {


//
//  Constants
//

static const IID IID_IGameInput_v0 = {0x11be2a7e, 0x4254, 0x445a, {0x9c, 0x09, 0xff, 0xc4, 0x0f, 0x00, 0x69, 0x18}};


//
//  Globals
//

static HMODULE g_gameInputDll = nullptr;
static HMODULE g_advapi32Dll = nullptr;
static HMODULE g_ntdllDll = nullptr;
static HMODULE g_versionDll = nullptr;

static uint32_t g_productType = PRODUCT_UNDEFINED;


//
//  Macros
//

#define RETURN_HR_IF(hr, condition) { bool _condition = (condition); if(_condition) { return (hr); } }
#define RETURN_HR_IF_NULL(hr, ptr) RETURN_HR_IF(hr, ((ptr) == nullptr))
#define RETURN_IF_FAILED(x) { HRESULT _hr = (x); if (FAILED(_hr)) { return _hr; } }
#define RETURN_IF_NULL_ALLOC(ptr) RETURN_HR_IF_NULL(E_OUTOFMEMORY, ptr)

#define RETURN_WIN32(error) { return HRESULT_FROM_WIN32(error); }
#define RETURN_IF_WIN32_ERROR(error) { if (error != ERROR_SUCCESS) { RETURN_WIN32(error); } }
#define RETURN_IF_WIN32_BOOL_FALSE(condition) { if (condition == FALSE) { RETURN_WIN32(GetLastError()); } }
#define RETURN_LAST_ERROR_IF(condition) { bool _condition = (condition); if(_condition) { RETURN_WIN32(GetLastError()); } }
#define RETURN_LAST_ERROR_IF_NULL(ptr) RETURN_LAST_ERROR_IF((ptr) == NULL)

#define RETURN_NTSTATUS(status) { NTSTATUS _value = (status); return NT_SUCCESS(_value) ? S_OK : (HRESULT)((_value) | FACILITY_NT_BIT); }
#define RETURN_IF_NTSTATUS_FAILED(status) { NTSTATUS _status = (status); if (!NT_SUCCESS(_status)) { RETURN_NTSTATUS(_status); } }

#define FAIL_FAST_HR(hr) { __fastfail(hr); }
#define FAIL_FAST() FAIL_FAST_HR(E_UNEXPECTED)
#define FAIL_FAST_IF(condition) { if (condition) { FAIL_FAST_HR(E_UNEXPECTED); } }
#define FAIL_FAST_IF_FAILED(x) { HRESULT _hr = (x); if (FAILED(_hr)) { FAIL_FAST_HR(_hr); } }
#define FAIL_FAST_IF_WIN32_BOOL_FALSE(condition) { if (condition == FALSE) { FAIL_FAST_HR(HRESULT_FROM_WIN32(GetLastError())); } }


//
//  Containers
//

template <typename T>
class Vector
{
public:
    ~Vector()
    {
        if (m_data != nullptr)
        {
            LocalFree(m_data);
        }
    }

    HRESULT Resize(
        _In_ size_t count) noexcept
    {
        if (count > m_capacity)
        {
            const size_t capacity = (m_capacity + count) * 2;

            T* const data = static_cast<T*>(LocalAlloc(LPTR, capacity * sizeof(T)));
            RETURN_IF_NULL_ALLOC(data);

            if (m_data != nullptr)
            {
                memcpy_s(data, count * sizeof(T), m_data, m_count * sizeof(T));
                LocalFree(m_data);
            }

            m_data = data;
            m_capacity = capacity;
        }

        m_count = count;

        return S_OK;
    }

    size_t GetCount() const noexcept
    {
        return m_count;
    }

    T& operator[](
        _In_ size_t index) noexcept
    {
        return m_data[index];
    }

    const T& operator[](
        _In_ size_t index) const noexcept
    {
        return m_data[index];
    }

    operator const T*() const noexcept
    {
        return m_data;
    }

    operator T*() noexcept
    {
        return m_data;
    }

private:
    T*     m_data = nullptr;
    size_t m_count = 0;
    size_t m_capacity = 0;
};

template <typename T>
class String
{
public:
    HRESULT Assign(
        _In_z_ const wchar_t* string)
    {
        RETURN_IF_FAILED(Resize(wcslen(string)));
        wcscpy_s(m_string, m_string.GetCount(), string);

        return S_OK;
    }

    HRESULT Append(
        _In_z_ const T* part) noexcept
    {
        const size_t stringLength = GetLength();
        const size_t partLength = wcslen(part);
        const size_t totalLength = stringLength + partLength;

        if (totalLength >= m_string.GetCount())
        {
            RETURN_IF_FAILED(Resize(totalLength));
        }

        wcscat_s(m_string, m_string.GetCount(), part);

        return S_OK;
    }

    HRESULT Resize(
        _In_ size_t length) noexcept
    {
        if (length >= m_string.GetCount())
        {
            RETURN_IF_FAILED(m_string.Resize(length + 1));
        }

        if (m_string.GetCount() > 0)
        {
            m_string[m_string.GetCount() - 1] = 0;
        }

        return S_OK;
    }

    size_t GetLength() const noexcept
    {
        const size_t count = m_string.GetCount();
        return count == 0 ? count : count - 1;
    }

    operator const T*() const noexcept
    {
        return m_string;
    }

    operator T*() noexcept
    {
        return m_string;
    }

    const T& GetFront() const noexcept
    {
        return m_string[0];
    }

    const T& GetBack() const noexcept
    {
        return m_string[m_string.GetCount() - 1];
    }

private:
    Vector<T> m_string;
};

using WString = String<wchar_t>;
using AString = String<char>;


//
//  Helpers
//

template <typename T>
static HRESULT GetModuleProc(
    _In_ HMODULE module,
    _In_z_ const char* name,
    _Out_ T* proc) noexcept
{
    *proc = reinterpret_cast<T>(reinterpret_cast<uintptr_t>(GetProcAddress(module, name)));
    RETURN_LAST_ERROR_IF_NULL(*proc);

    return S_OK;
}

template <typename T>
static HRESULT LoadSystemModuleProc(
    _In_ HMODULE* module,
    _In_z_ const wchar_t* path,
    _In_z_ const char* name,
    _Out_ T* proc) noexcept
{
    *proc = nullptr;

    if (*module == nullptr)
    {
        *module = LoadLibraryEx(path, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        RETURN_LAST_ERROR_IF_NULL(*module);
    }

    RETURN_IF_FAILED(GetModuleProc(*module, name, proc));

    return S_OK;
}

static HRESULT GetSystemDirectory(
    _Inout_ WString* systemDir) noexcept
{
    uint32_t length = ::GetSystemDirectory(nullptr, 0);
    RETURN_LAST_ERROR_IF(length == 0);
    RETURN_IF_FAILED(systemDir->Resize(length));

    length = ::GetSystemDirectory(*systemDir, length) + 1;
    RETURN_LAST_ERROR_IF(length == 0);
    RETURN_IF_FAILED(systemDir->Resize(length));

    return S_OK;
}

static HRESULT GetRedistDirectory(
    _Inout_ WString* redistDir) noexcept
{
    constexpr const wchar_t* RedistDirRegPath = L"SOFTWARE\\Microsoft\\GameInput";
    constexpr const wchar_t* RedistDirValueName = L"RedistDir";

    decltype(RegGetValueW)* regGetValue = nullptr;
    RETURN_IF_FAILED(LoadSystemModuleProc(
        &g_advapi32Dll,
        L"advapi32.dll",
        "RegGetValueW",
        &regGetValue));

    DWORD cbSize = 0;
    RETURN_IF_WIN32_ERROR(static_cast<DWORD>(regGetValue(
        HKEY_LOCAL_MACHINE,
        RedistDirRegPath,
        RedistDirValueName,
        RRF_RT_REG_SZ | RRF_SUBKEY_WOW6432KEY,
        nullptr,
        nullptr,
        &cbSize)));

    redistDir->Resize(cbSize / sizeof(wchar_t));

    RETURN_IF_WIN32_ERROR(static_cast<DWORD>(regGetValue(
        HKEY_LOCAL_MACHINE,
        RedistDirRegPath,
        RedistDirValueName,
        RRF_RT_REG_SZ | RRF_SUBKEY_WOW6432KEY,
        nullptr,
        *redistDir,
        &cbSize)));

    redistDir->Resize(cbSize / sizeof(wchar_t));

    return S_OK;
}

static HRESULT GetFileVersion(
    _In_z_ const wchar_t* path,
    _Out_ uint64_t* version) noexcept
{
    *version = 0;

    decltype(GetFileVersionInfoSizeW)* getFileVersionInfoSize = nullptr;
    RETURN_IF_FAILED(LoadSystemModuleProc(
        &g_versionDll,
        L"version.dll",
        "GetFileVersionInfoSizeW",
        &getFileVersionInfoSize));

    decltype(GetFileVersionInfoW)* getFileVersionInfo = nullptr;
    RETURN_IF_FAILED(LoadSystemModuleProc(
        &g_versionDll,
        L"version.dll",
        "GetFileVersionInfoW",
        &getFileVersionInfo));

    decltype(VerQueryValueW)* verQueryValue = nullptr;
    RETURN_IF_FAILED(LoadSystemModuleProc(
        &g_versionDll,
        L"version.dll",
        "VerQueryValueW",
        &verQueryValue));

    DWORD unused = 0;
    const DWORD bufferSize = getFileVersionInfoSize(path, &unused);
    RETURN_LAST_ERROR_IF(bufferSize == 0);

    Vector<uint8_t> buffer;
    RETURN_IF_FAILED(buffer.Resize(bufferSize));
    RETURN_IF_WIN32_BOOL_FALSE(getFileVersionInfo(
        path,
        0,
        static_cast<DWORD>(buffer.GetCount()),
        buffer));

    UINT verLength = 0;
    VS_FIXEDFILEINFO* verInfo = nullptr;
    RETURN_IF_WIN32_BOOL_FALSE(verQueryValue(
        buffer,
        L"\\",
        reinterpret_cast<LPVOID*>(&verInfo),
        &verLength));

    ULARGE_INTEGER li = {};
    li.HighPart = verInfo->dwFileVersionMS;
    li.LowPart = verInfo->dwFileVersionLS;

    *version = li.QuadPart;

    return S_OK;
}

static bool FileExists(
    _In_z_ const wchar_t* path) noexcept
{
    const DWORD attributes = GetFileAttributesW(path);
    if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        return false;
    }

    return true;
}

static bool GetFileInfo(
    _In_z_ const wchar_t* path,
    _Out_ uint64_t* version) noexcept
{
    *version = 0;

    if (FileExists(path))
    {
        // Best effort attempt to get DLL verison; will return zero
        // on platforms which do not support this query.

        (void)GetFileVersion(path, version);
        return true;
    }

    return false;
}

static HRESULT PathJoin(
    _In_z_ const wchar_t* path1,
    _In_z_ const wchar_t* path2,
    _Inout_ WString* result) noexcept
{
    if (*path1 == 0)
    {
        return result->Assign(path2);
    }

    if (*path2 == 0)
    {
        return result->Assign(path1);
    }

    RETURN_IF_FAILED(result->Assign(path1));

    if (result->GetBack() != L'\\' && result->GetBack() != L'/')
    {
        RETURN_IF_FAILED(result->Append(L"\\"));
    }

    if (path2[0] == L'\\' || path2[0] == L'/')
    {
        RETURN_IF_FAILED(result->Append(path2 + 1));
    }
    else
    {
        RETURN_IF_FAILED(result->Append(path2));
    }

    return S_OK;
}


//
//  Loader
//

static HRESULT LoadGameInputDll(
    _Out_ HMODULE* module) noexcept
{
    *module = nullptr;

    WString systemDir;
    RETURN_IF_FAILED(GetSystemDirectory(&systemDir));

    WString inboxPath;
    uint64_t inboxVersion = 0;
    const bool validInbox =
        SUCCEEDED(PathJoin(systemDir, L"GameInput.dll", &inboxPath)) &&
        GetFileInfo(inboxPath, &inboxVersion);

    WString redistPath;
    uint64_t redistVersion = 0;
    bool validRedist =
        SUCCEEDED(PathJoin(systemDir, L"GameInputRedist.dll", &redistPath)) &&
        GetFileInfo(redistPath, &redistVersion);

    if (!validRedist)
    {
        // GameInputRedist.dll can be found in System32 and Program Files;
        // check both locations for improved compatibility.

        WString redistDir;
        validRedist =
            SUCCEEDED(GetRedistDirectory(&redistDir)) &&
            SUCCEEDED(PathJoin(redistDir, L"GameInputRedist.dll", &redistPath)) &&
            GetFileInfo(redistPath, &redistVersion);
    }

    const wchar_t* path = nullptr;
    if (validInbox)
    {
        path = inboxPath;
    }
    if (validRedist && (!validInbox || redistVersion >= inboxVersion))
    {
        path = redistPath;
    }

    RETURN_HR_IF_NULL(HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND), path);

    *module = LoadLibrary(path);
    RETURN_LAST_ERROR_IF_NULL(*module);

    return S_OK;
}

static HRESULT GameInputCreateWithVersion(
    _In_ REFIID riid,
    _COM_Outptr_ LPVOID* ppv) noexcept
{
    *ppv = nullptr;

    if (g_gameInputDll == nullptr)
    {
        RETURN_IF_FAILED(LoadGameInputDll(&g_gameInputDll));
    }

    using GameInputInitializeFn = HRESULT (*)(
        _In_ REFIID riid,
        _COM_Outptr_ LPVOID* ppv);

    GameInputInitializeFn gameInputInitialize = nullptr;
    if (SUCCEEDED(GetModuleProc(g_gameInputDll, "GameInputInitialize", &gameInputInitialize)))
    {
        return gameInputInitialize(riid, ppv);
    }

    if (riid == IID_IGameInput_v0)
    {
        // All recent versions of GameInput support the GameInputInitialize export. As we
        // did not find it via above query, we must be running an old version of GameInput
        // which only supports the v0 API. Don't attempt to use it for newer API versions.

        using GameInputCreateFn = HRESULT (*)(
            _COM_Outptr_ LPVOID* ppv);

        GameInputCreateFn gameInputCreate = nullptr;
        if (SUCCEEDED(GetModuleProc(g_gameInputDll, "GameInputCreate", &gameInputCreate)))
        {
            return gameInputCreate(ppv);
        }

        return HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND);
    }

    return E_NOINTERFACE;
}


} // namespace GameInput


//
//  Exports
//

STDAPI GameInputInitialize(
    _In_ REFIID riid,
    _COM_Outptr_ LPVOID* ppv) noexcept
{
    return GameInput::GameInputCreateWithVersion(riid, ppv);
}
