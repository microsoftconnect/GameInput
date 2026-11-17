// Copyright (c) Microsoft Corporation.  All rights reserved.
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
//
// DualSense adaptive-trigger helper (header-only) using GameInput raw output reports.
// Use GameInput's lighting and rumble APIs for lighting and ordinary motor vibration.
//
// Requires GameInput version 3 or later (IGameInputDevice::CreateRawDeviceReport,
// IGameInputDevice::SendRawDeviceOutput, IGameInputRawDeviceReport).
//
// Usage:
//   using namespace GameInput::DualSense;
//   if (IsDualSense(info->vendorId, info->productId))
//   {
//       SetDualSenseTriggerVibration(device, Trigger::Right, 5, 1, 8);
//   }

#pragma once

#include <GameInput.h>

#if !defined(GAMEINPUT_API_VERSION) || GAMEINPUT_API_VERSION < 3
#error DualSenseHelper requires GameInput API version 3 or later.
#endif

#include <stdint.h>

namespace GameInput { namespace DualSense {

#if GAMEINPUT_API_VERSION == 3
using namespace ::GameInput::v3;
#elif GAMEINPUT_API_VERSION >= 4
using namespace ::GameInput::v4;
#endif

enum class Trigger
{
    Right,
    Left,
};

namespace Detail {

    constexpr uint32_t ReportSize             = 63;
    constexpr uint8_t  UsbOutputReportId      = 0x02;
    constexpr uint16_t SonyVendorId           = 0x054C;
    constexpr uint16_t DualSenseProductId     = 0x0CE6;
    constexpr uint16_t DualSenseEdgeProductId = 0x0DF2;

    // flag0 (byte 1): selects the adaptive trigger to update
    constexpr uint32_t Flag0Offset       = 1;
    constexpr uint8_t  Flag0RightTrigger = 0x04;
    constexpr uint8_t  Flag0LeftTrigger  = 0x08;

    // Adaptive trigger regions
    constexpr uint32_t TriggerBlockStride = 11;
    constexpr uint32_t RightTriggerOffset = 0x0B;
    constexpr uint32_t LeftTriggerOffset  = RightTriggerOffset + TriggerBlockStride;

    inline uint8_t GetTriggerFlag(Trigger trigger) noexcept
    {
        return (trigger == Trigger::Right) ? Flag0RightTrigger : Flag0LeftTrigger;
    }

    inline uint32_t GetTriggerOffset(Trigger trigger) noexcept
    {
        return (trigger == Trigger::Right) ? RightTriggerOffset : LeftTriggerOffset;
    }

    inline void EnableTriggerEffect(uint8_t* buf, Trigger trigger) noexcept
    {
        buf[Flag0Offset] |= GetTriggerFlag(trigger);
    }

    inline bool IsValidTrigger(Trigger trigger) noexcept
    {
        return trigger == Trigger::Right || trigger == Trigger::Left;
    }

    inline void SetTriggerZones(uint8_t* effect, uint32_t position, uint32_t level) noexcept
    {
        uint32_t mask = 0;
        uint32_t levels = 0;
        for (uint32_t zone = position; zone < 10; ++zone)
        {
            mask |= 1u << zone;
            levels |= (level - 1u) << (3u * zone);
        }

        effect[1] = static_cast<uint8_t>(mask);
        effect[2] = static_cast<uint8_t>(mask >> 8);
        for (uint32_t byte = 0; byte < 4; ++byte)
        {
            effect[3 + byte] = static_cast<uint8_t>(levels >> (8u * byte));
        }
    }

    inline HRESULT SendReport(IGameInputDevice* device, const uint8_t* buf) noexcept
    {
        IGameInputRawDeviceReport* rawReport = nullptr;
        HRESULT hr = device->CreateRawDeviceReport(
            buf[0],
            GameInputRawOutputReport,
            &rawReport);
        if (FAILED(hr))
        {
            return hr;
        }

        if (!rawReport->SetRawData(ReportSize, buf))
        {
            rawReport->Release();
            return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
        }

        hr = device->SendRawDeviceOutput(rawReport);
        rawReport->Release();
        return hr;
    }

} // namespace Detail

// Controller identification

inline bool IsDualSense(uint16_t vendorId, uint16_t productId) noexcept
{
    return vendorId == Detail::SonyVendorId &&
           (productId == Detail::DualSenseProductId || productId == Detail::DualSenseEdgeProductId);
}

// Adaptive trigger effects.
//
// A position is a zone number along the trigger's travel, not a distance.
// Strength and amplitude are relative levels, not units of force. Each
// function below lists its valid ranges. A value outside them returns
// E_INVALIDARG and nothing is sent to the controller. A strength, amplitude,
// or frequency of 0 turns the effect off.

// Off: clears any trigger effect.
inline HRESULT SetDualSenseTriggerOff(
    IGameInputDevice* device,
    Trigger trigger) noexcept
{
    if (!Detail::IsValidTrigger(trigger))
    {
        return E_INVALIDARG;
    }

    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    Detail::EnableTriggerEffect(buf, trigger);
    buf[Detail::GetTriggerOffset(trigger)] = 0x05;
    return Detail::SendReport(device, buf);
}

// Feedback: position 0-9; strength 0-8. Applies from position through zone 9.
inline HRESULT SetDualSenseTriggerFeedback(
    IGameInputDevice* device,
    Trigger trigger,
    uint32_t position,
    uint32_t strength) noexcept
{
    if (!Detail::IsValidTrigger(trigger) || position > 9 || strength > 8)
    {
        return E_INVALIDARG;
    }

    if (strength == 0)
    {
        return SetDualSenseTriggerOff(device, trigger);
    }

    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    const uint32_t offset = Detail::GetTriggerOffset(trigger);
    Detail::EnableTriggerEffect(buf, trigger);
    buf[offset] = 0x21;
    Detail::SetTriggerZones(&buf[offset], position, strength);
    return Detail::SendReport(device, buf);
}

// Weapon: startPosition 2-7; endPosition startPosition+1 through 8; strength 0-8.
inline HRESULT SetDualSenseTriggerWeapon(
    IGameInputDevice* device,
    Trigger trigger,
    uint32_t startPosition,
    uint32_t endPosition,
    uint32_t strength) noexcept
{
    if (!Detail::IsValidTrigger(trigger) || startPosition < 2 || startPosition > 7 ||
        endPosition <= startPosition || endPosition > 8 || strength > 8)
    {
        return E_INVALIDARG;
    }

    if (strength == 0)
    {
        return SetDualSenseTriggerOff(device, trigger);
    }

    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    const uint32_t offset = Detail::GetTriggerOffset(trigger);
    const uint32_t mask = (1u << startPosition) | (1u << endPosition);
    Detail::EnableTriggerEffect(buf, trigger);
    buf[offset] = 0x25;
    buf[offset + 1] = static_cast<uint8_t>(mask);
    buf[offset + 2] = static_cast<uint8_t>(mask >> 8);
    buf[offset + 3] = static_cast<uint8_t>(strength - 1u);
    return Detail::SendReport(device, buf);
}

// Vibration: position 0-9; amplitude 0-8; frequency 0-255 in protocol units.
// Amplitude applies from position through zone 9, independently of frequency.
inline HRESULT SetDualSenseTriggerVibration(
    IGameInputDevice* device,
    Trigger trigger,
    uint32_t position,
    uint32_t amplitude,
    uint32_t frequency) noexcept
{
    if (!Detail::IsValidTrigger(trigger) || position > 9 || amplitude > 8 || frequency > 255)
    {
        return E_INVALIDARG;
    }

    if (amplitude == 0 || frequency == 0)
    {
        return SetDualSenseTriggerOff(device, trigger);
    }

    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    const uint32_t offset = Detail::GetTriggerOffset(trigger);
    Detail::EnableTriggerEffect(buf, trigger);
    buf[offset] = 0x26;
    Detail::SetTriggerZones(&buf[offset], position, amplitude);
    buf[offset + 9] = static_cast<uint8_t>(frequency);
    return Detail::SendReport(device, buf);
}

}} // namespace GameInput::DualSense
