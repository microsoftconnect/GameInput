// Copyright (c) Microsoft Corporation.
//
// This file is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#ifndef __cplusplus
#error GameInput requires C++
#endif

#include <GameInput.h>

#pragma region Application Family or OneCore Family or Games Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)

namespace GameInput { namespace v4 {

// IGameInputAutomation is a development and validation feature. It adds synthetic devices and
// injects readings through them, so that a game's input handling can be exercised by automated
// tests without physical hardware. It is not intended for use in shipping gameplay, and games
// must not depend on it being available. Obtain it by calling QueryInterface on IGameInput.
//
// The interface is only available when the GAMEINPUT_AUTOMATION_ENABLE environment variable is set
// to a positive integer before GameInput is first created in the process, and the process is not
// running as SYSTEM. Otherwise QueryInterface returns E_NOINTERFACE.

DECLARE_INTERFACE_IID_(IGameInputAutomation, IUnknown, "AC2B5C4A-2B8D-49A1-9ECB-69FF8B7466B0")
{
    IFACEMETHOD(AddDevice)(
        GameInputKind supportedInput,
        _Out_ APP_LOCAL_DEVICE_ID* deviceId) PURE;

    IFACEMETHOD(RemoveDevice)(
        _In_ const APP_LOCAL_DEVICE_ID* deviceId) PURE;

    IFACEMETHOD(InjectGamepadReading)(
        _In_ const APP_LOCAL_DEVICE_ID* deviceId,
        _In_ const GameInputGamepadState* state) PURE;

    IFACEMETHOD(InjectKeyboardReading)(
        _In_ const APP_LOCAL_DEVICE_ID* deviceId,
        uint32_t keyCount,
        _In_reads_(keyCount) const GameInputKeyState* keys) PURE;

    IFACEMETHOD(InjectMouseReading)(
        _In_ const APP_LOCAL_DEVICE_ID* deviceId,
        _In_ const GameInputMouseState* state) PURE;
};

}} // namespace GameInput::v4

#endif // #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
#pragma endregion // Application Family or OneCore Family or Games Family
