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
// DualSense helper (header-only) for controlling lightbar, LEDs,
// rumble, and adaptive triggers via GameInput raw output reports.
//
// Requires GameInput version 3 or later (IGameInputDevice::CreateRawDeviceReport,
// IGameInputDevice::SendRawDeviceOutput, IGameInputRawDeviceReport).
//
// Usage:
//   using namespace GameInput::DualSense;
//   if (IsDualSense(info->vendorId, info->productId))
//   {
//       SetDualSenseRumble(device, 255, 96);
//       SetDualSenseLightbar(device, 255, 0, 0);
//       SetDualSenseTriggerFeedback(device, Trigger::Right, 0x90, 0xFF);
//   }

#pragma once

#include <GameInput.h>

#if !defined(GAMEINPUT_API_VERSION) || GAMEINPUT_API_VERSION < 3
#error DualSenseHelper requires GameInput API version 3 or later.
#endif

#include <stdint.h>
#include <string.h>

namespace GameInput { namespace DualSense {

enum class Trigger
{
    Right,
    Left,
};

enum class PlayerLed : uint8_t
{
    Off      = 0,
    Player1  = 1,
    Player2  = 2,
    Player3  = 3,
    Player4  = 4,
};

namespace Detail {

    constexpr uint32_t ReportSize             = 64;
    constexpr uint8_t  UsbOutputReportId      = 0x02;
    constexpr uint16_t SonyVendorId           = 0x054C;
    constexpr uint16_t DualSenseProductId     = 0x0CE6;
    constexpr uint16_t DualSenseEdgeProductId = 0x0DF2;

    // flag0 (byte 1): enables rumble/haptic/trigger features
    constexpr uint32_t Flag0Offset       = 1;
    constexpr uint8_t  Flag0Vibration    = 0x01;
    constexpr uint8_t  Flag0HapticFilter = 0x02;
    constexpr uint8_t  Flag0RightTrigger = 0x04;
    constexpr uint8_t  Flag0LeftTrigger  = 0x08;

    // flag1 (byte 2): selects which features to apply
    constexpr uint32_t Flag1Offset     = 2;
    constexpr uint8_t  Flag1MicControl = 0x01;
    constexpr uint8_t  Flag1Lightbar   = 0x04;
    constexpr uint8_t  Flag1PlayerLed  = 0x10;

    // Motor bytes
    constexpr uint32_t MotorRightOffset = 3;
    constexpr uint32_t MotorLeftOffset  = 4;

    // Mute LED / power save
    constexpr uint32_t MuteLedOffset   = 9;
    constexpr uint32_t PowerSaveOffset = 10;

    // Adaptive trigger regions
    constexpr uint32_t TriggerEffectSize  = 8;
    constexpr uint32_t TriggerBlockStride = 11;
    constexpr uint32_t RightTriggerOffset = 0x0B;
    constexpr uint32_t LeftTriggerOffset  = RightTriggerOffset + TriggerBlockStride;

    // flag2: secondary feature control
    constexpr uint32_t Flag2Offset        = 40;
    constexpr uint8_t  Flag2LightbarSetup = 0x02;

    // Lightbar setup & LED
    constexpr uint32_t LightbarSetupOffset = 42;
    constexpr uint8_t  LightbarFadeOut     = 0x02;
    constexpr uint32_t PlayerLedOffset      = 44;

    // Lightbar RGB
    constexpr uint32_t LightbarRedOffset   = 45;
    constexpr uint32_t LightbarGreenOffset = 46;
    constexpr uint32_t LightbarBlueOffset  = 47;

    // Player LED bit patterns (5 LEDs across the touchpad)
    constexpr uint8_t PlayerLedPatterns[] = { 0x00, 0x04, 0x0A, 0x15, 0x1B };

    inline uint8_t GetTriggerFlag(Trigger trigger) noexcept
    {
        return (trigger == Trigger::Right) ? Flag0RightTrigger : Flag0LeftTrigger;
    }

    inline uint32_t GetTriggerOffset(Trigger trigger) noexcept
    {
        return (trigger == Trigger::Right) ? RightTriggerOffset : LeftTriggerOffset;
    }

    inline void ClearTriggerBlock(uint8_t* buf, uint32_t offset) noexcept
    {
        memset(&buf[offset], 0, TriggerEffectSize);
    }

    inline void EnableTriggerEffect(uint8_t* buf, Trigger trigger) noexcept
    {
        buf[Flag0Offset] |= GetTriggerFlag(trigger);
    }

    inline HRESULT SendReport(GameInput::v3::IGameInputDevice* device, const uint8_t* buf) noexcept
    {
        if (device == nullptr)
        {
            return E_POINTER;
        }

        GameInput::v3::IGameInputRawDeviceReport* rawReport = nullptr;
        HRESULT hr = device->CreateRawDeviceReport(
            buf[0],
            GameInput::v3::GameInputRawOutputReport,
            &rawReport);
        if (FAILED(hr))
        {
            return hr;
        }

        if (!rawReport->SetRawData(ReportSize, buf))
        {
            rawReport->Release();
            return E_INVALIDARG;
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

// Lightbar / LEDs / Rumble

// Set lightbar color (RGB, each 0-255).
inline HRESULT SetDualSenseLightbar(
    GameInput::v3::IGameInputDevice* device,
    uint8_t r,
    uint8_t g,
    uint8_t b) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    buf[Detail::Flag1Offset]        |= Detail::Flag1Lightbar;
    buf[Detail::LightbarRedOffset]   = r;
    buf[Detail::LightbarGreenOffset] = g;
    buf[Detail::LightbarBlueOffset]  = b;
    return Detail::SendReport(device, buf);
}

// Turn off the lightbar (fade out).
inline HRESULT TurnOffDualSenseLightbar(GameInput::v3::IGameInputDevice* device) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    buf[Detail::Flag1Offset]         |= Detail::Flag1Lightbar;
    buf[Detail::Flag2Offset]         |= Detail::Flag2LightbarSetup;
    buf[Detail::LightbarSetupOffset]  = Detail::LightbarFadeOut;
    return Detail::SendReport(device, buf);
}

// Set player LED indicator.
inline HRESULT SetDualSensePlayerLed(
    GameInput::v3::IGameInputDevice* device,
    PlayerLed player) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    buf[Detail::Flag1Offset] |= Detail::Flag1PlayerLed;
    const uint8_t index = static_cast<uint8_t>(player);
    buf[Detail::PlayerLedOffset] = (index <= 4) ? Detail::PlayerLedPatterns[index] : 0;
    return Detail::SendReport(device, buf);
}

// Set rumble motor intensities (each 0-255).
// leftMotor = strong/low-frequency, rightMotor = weak/high-frequency.
inline HRESULT SetDualSenseRumble(
    GameInput::v3::IGameInputDevice* device,
    uint8_t leftMotor,
    uint8_t rightMotor) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    buf[Detail::Flag0Offset]     |= Detail::Flag0Vibration | Detail::Flag0HapticFilter;
    buf[Detail::MotorLeftOffset]  = leftMotor;
    buf[Detail::MotorRightOffset] = rightMotor;
    return Detail::SendReport(device, buf);
}

// Set the microphone mute LED.
inline HRESULT SetDualSenseMuteLed(GameInput::v3::IGameInputDevice* device, bool on) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    buf[Detail::Flag1Offset]    |= Detail::Flag1MicControl;
    buf[Detail::MuteLedOffset]   = on ? 0x01 : 0x00;
    buf[Detail::PowerSaveOffset] = on ? 0x00 : 0x10;
    return Detail::SendReport(device, buf);
}

// Adaptive Trigger Effects
//
// Only three mode flags are known to work on real hardware.
// All trigger params are raw uint8_t bytes matching the HID report layout.

// Off: clears any trigger effect.
inline HRESULT SetDualSenseTriggerOff(
    GameInput::v3::IGameInputDevice* device,
    Trigger trigger) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    Detail::EnableTriggerEffect(buf, trigger);
    Detail::ClearTriggerBlock(buf, Detail::GetTriggerOffset(trigger));
    return Detail::SendReport(device, buf);
}

// Feedback: no resistance until a starting point, then constant force.
// startPosition: where resistance begins (0 = top, 255 = bottom).
// force: amount of feedback after the start point.
inline HRESULT SetDualSenseTriggerFeedback(
    GameInput::v3::IGameInputDevice* device,
    Trigger trigger,
    uint8_t startPosition,
    uint8_t force) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    const uint32_t offset = Detail::GetTriggerOffset(trigger);
    Detail::EnableTriggerEffect(buf, trigger);
    Detail::ClearTriggerBlock(buf, offset);
    buf[offset + 0] = 0x01;
    buf[offset + 1] = startPosition;
    buf[offset + 2] = force;
    return Detail::SendReport(device, buf);
}

// Weapon: resistance region with a breakout force.
// startPosition: where resistance begins (0 = top, 255 = bottom).
// endPosition: where resistance ends (0 = top, 255 = bottom).
// strength: how strong the resistance is to break through.
inline HRESULT SetDualSenseTriggerWeapon(
    GameInput::v3::IGameInputDevice* device,
    Trigger trigger,
    uint8_t startPosition,
    uint8_t endPosition,
    uint8_t strength) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    const uint32_t offset = Detail::GetTriggerOffset(trigger);
    Detail::EnableTriggerEffect(buf, trigger);
    Detail::ClearTriggerBlock(buf, offset);
    buf[offset + 0] = 0x02;
    buf[offset + 1] = startPosition;
    buf[offset + 2] = endPosition;
    buf[offset + 3] = strength;
    return Detail::SendReport(device, buf);
}

// Vibration: vibrates the trigger with configurable frequency and resistance.
// frequency: vibration speed (0 = low, 255 = high).
// resistance: resistance level (0 = high, 255 = low / inverted).
// startPosition: where vibration + resistance begin (0 = top, 147 = bottom).
inline HRESULT SetDualSenseTriggerVibration(
    GameInput::v3::IGameInputDevice* device,
    Trigger trigger,
    uint8_t frequency,
    uint8_t resistance,
    uint8_t startPosition) noexcept
{
    uint8_t buf[Detail::ReportSize] = {Detail::UsbOutputReportId};
    const uint32_t offset = Detail::GetTriggerOffset(trigger);
    Detail::EnableTriggerEffect(buf, trigger);
    Detail::ClearTriggerBlock(buf, offset);
    buf[offset + 0] = 0x06;
    buf[offset + 1] = frequency;
    buf[offset + 2] = resistance;
    buf[offset + 3] = startPosition;
    return Detail::SendReport(device, buf);
}

}} // namespace GameInput::DualSense
