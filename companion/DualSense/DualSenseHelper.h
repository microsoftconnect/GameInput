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
// DualSense helper (header-only) for the lightbar, LEDs, rumble, microphone mute
// and adaptive triggers via GameInput raw output reports.
// Requires GameInput API version 3 or later.
//
// Only USB is supported (output report 0x02, report ID at byte 0).
// Over Bluetooth the controller expects output report 0x31 with a sequence/tag
// header and a trailing CRC32, which this helper does not build.
//
// Every free function sends one report that only marks its own feature as valid.
// The controller keeps the state of all features a report does not mark.
// OutputReport combines several features into a single report.
//
// Usage:
//   using namespace GameInput::DualSense;
//   if (IsDualSense(info->vendorId, info->productId))
//   {
//       SetDualSenseRumble(device, 255, 96);
//       SetDualSenseLightbar(device, 255, 0, 0);
//       SetDualSenseTriggerFeedback(device, Trigger::Right, 0x90, 0xFF);
//
//       OutputReport report;
//       report.SetLightbar(0, 0, 255).SetPlayerLed(PlayerLed::Player1);
//       report.SetMicMuted(MuteLed::On);
//       HRESULT hr = report.Send(device);
//   }

#pragma once

#include <GameInput.h>

#if !defined(GAMEINPUT_API_VERSION) || GAMEINPUT_API_VERSION < 3
#error DualSenseHelper requires GameInput API version 3 or later.
#endif

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace GameInput { namespace DualSense {

using Device = ::GameInput::v3::IGameInputDevice;

enum class Trigger
{
    Right,
    Left,
};

// Player indicator, the five white LEDs below the touchpad.
enum class PlayerLed : uint8_t
{
    Off     = 0,
    Player1 = 1,
    Player2 = 2,
    Player3 = 3,
    Player4 = 4,
    Player5 = 5,
};

// Mode of the LED inside the microphone mute button.
enum class MuteLed : uint8_t
{
    Off   = 0x00,
    On    = 0x01,
    Pulse = 0x02,
};

namespace Detail {

    namespace Api = ::GameInput::v3;

    constexpr uint16_t SonyVendorId           = 0x054C;
    constexpr uint16_t DualSenseProductId     = 0x0CE6;
    constexpr uint16_t DualSenseEdgeProductId = 0x0DF2;

    // USB output report 0x02 is the report ID followed by the 47 byte common block.
    // All offsets below include the report ID at byte 0.
    constexpr uint8_t UsbOutputReportId = 0x02;
    constexpr size_t  ReportLength      = 48;

    // Upper bound for the buffer size GameInput may request for this report.
    constexpr size_t MaxRawDataSize = 64;

    // valid_flag0 (byte 1): rumble and trigger effects.
    constexpr size_t  Flag0Offset        = 1;
    constexpr uint8_t Flag0Vibration     = 0x01;
    constexpr uint8_t Flag0HapticsSelect = 0x02;
    constexpr uint8_t Flag0Rumble        = Flag0Vibration | Flag0HapticsSelect;
    constexpr uint8_t Flag0RightTrigger  = 0x04;
    constexpr uint8_t Flag0LeftTrigger   = 0x08;

    // valid_flag1 (byte 2): mute LED, power save, lightbar and player LEDs.
    constexpr size_t  Flag1Offset    = 2;
    constexpr uint8_t Flag1MuteLed   = 0x01;
    constexpr uint8_t Flag1PowerSave = 0x02;
    constexpr uint8_t Flag1Lightbar  = 0x04;
    constexpr uint8_t Flag1PlayerLed = 0x10;

    // Rumble motors.
    constexpr size_t MotorRightOffset = 3;
    constexpr size_t MotorLeftOffset  = 4;

    // Mute button LED and power save / mute control.
    // The firmware applies the power save byte as a whole when Flag1PowerSave is set.
    constexpr size_t  MuteLedOffset    = 9;
    constexpr size_t  PowerSaveOffset  = 10;
    constexpr uint8_t PowerSaveMicMute = 0x10;

    // Adaptive trigger effect blocks, each a mode byte followed by 10 parameter bytes.
    constexpr size_t TriggerBlockSize   = 11;
    constexpr size_t RightTriggerOffset = 11;
    constexpr size_t LeftTriggerOffset  = RightTriggerOffset + TriggerBlockSize;

    // Trigger effect modes.
    // Off is Sony's official mode, the other three are the legacy simple modes.
    constexpr uint8_t TriggerModeOff             = 0x05;
    constexpr uint8_t TriggerModeSimpleFeedback  = 0x01;
    constexpr uint8_t TriggerModeSimpleWeapon    = 0x02;
    constexpr uint8_t TriggerModeSimpleVibration = 0x06;

    // valid_flag2 (byte 39): lightbar setup.
    constexpr size_t  Flag2Offset        = 39;
    constexpr uint8_t Flag2LightbarSetup = 0x02;

    // Lightbar setup, player LEDs and lightbar color.
    constexpr size_t  LightbarSetupOffset = 42;
    constexpr uint8_t LightbarFadeOut     = 0x02;
    constexpr size_t  PlayerLedOffset     = 44;
    constexpr size_t  LightbarRedOffset   = 45;
    constexpr size_t  LightbarGreenOffset = 46;
    constexpr size_t  LightbarBlueOffset  = 47;

    static_assert(LeftTriggerOffset + TriggerBlockSize <= Flag2Offset,
                  "Trigger blocks must end before valid_flag2.");
    static_assert(LightbarBlueOffset < ReportLength,
                  "All fields must lie inside the report.");
    static_assert(ReportLength <= MaxRawDataSize,
                  "The report must fit into the raw data buffer.");

    inline void SetBits(uint8_t& value, uint8_t mask) noexcept
    {
        value = static_cast<uint8_t>(value | mask);
    }

    inline void ClearBits(uint8_t& value, uint8_t mask) noexcept
    {
        value = static_cast<uint8_t>(value & ~mask);
    }

    // Players 1 to 4 match the PS5, player 5 follows the Linux driver.
    inline uint8_t PlayerLedPattern(PlayerLed player) noexcept
    {
        switch (player)
        {
        case PlayerLed::Player1: return 0x04;
        case PlayerLed::Player2: return 0x0A;
        case PlayerLed::Player3: return 0x15;
        case PlayerLed::Player4: return 0x1B;
        case PlayerLed::Player5: return 0x1F;
        case PlayerLed::Off:
        default:                 return 0x00;
        }
    }

    inline uint8_t TriggerFlag(Trigger trigger) noexcept
    {
        return (trigger == Trigger::Right) ? Flag0RightTrigger : Flag0LeftTrigger;
    }

    inline size_t TriggerOffset(Trigger trigger) noexcept
    {
        return (trigger == Trigger::Right) ? RightTriggerOffset : LeftTriggerOffset;
    }

    // Owns one reference to a raw device report.
    class RawReport
    {
    public:
        RawReport() noexcept = default;

        RawReport(const RawReport&) = delete;
        RawReport& operator=(const RawReport&) = delete;

        RawReport(RawReport&& other) noexcept
            : m_report(other.m_report)
        {
            other.m_report = nullptr;
        }

        RawReport& operator=(RawReport&& other) noexcept
        {
            if (this != &other)
            {
                Reset();
                m_report = other.m_report;
                other.m_report = nullptr;
            }
            return *this;
        }

        ~RawReport() noexcept
        {
            Reset();
        }

        Api::IGameInputRawDeviceReport* Get() const noexcept
        {
            return m_report;
        }

        // Releases any held reference first, then returns the slot for an out parameter.
        Api::IGameInputRawDeviceReport** Put() noexcept
        {
            Reset();
            return &m_report;
        }

    private:
        void Reset() noexcept
        {
            if (m_report != nullptr)
            {
                m_report->Release();
                m_report = nullptr;
            }
        }

        Api::IGameInputRawDeviceReport* m_report = nullptr;
    };

    static_assert(!std::is_copy_constructible<RawReport>::value &&
                  !std::is_copy_assignable<RawReport>::value,
                  "RawReport must not be copyable.");
    static_assert(std::is_nothrow_move_constructible<RawReport>::value &&
                  std::is_nothrow_move_assignable<RawReport>::value,
                  "RawReport must be nothrow movable.");

    inline HRESULT SendReport(
        Api::IGameInputDevice* device,
        const uint8_t (&data)[ReportLength]) noexcept
    {
        if (device == nullptr)
        {
            return E_POINTER;
        }

        RawReport report;
        HRESULT hr = device->CreateRawDeviceReport(
            data[0],
            Api::GameInputRawOutputReport,
            report.Put());
        if (FAILED(hr))
        {
            return hr;
        }
        if (report.Get() == nullptr)
        {
            return E_UNEXPECTED;
        }

        // SetRawData only accepts a buffer of exactly GetRawDataSize() bytes.
        // That size is defined by GameInput, so the report is zero padded to it.
        const size_t size = report.Get()->GetRawDataSize();
        if (size < ReportLength || size > MaxRawDataSize)
        {
            return HRESULT_FROM_WIN32(ERROR_BAD_LENGTH);
        }

        uint8_t raw[MaxRawDataSize] = {};
        std::memcpy(raw, data, ReportLength);
        if (!report.Get()->SetRawData(size, raw))
        {
            return E_INVALIDARG;
        }

        return device->SendRawDeviceOutput(report.Get());
    }

} // namespace Detail

// Controller identification

inline bool IsDualSense(uint16_t vendorId, uint16_t productId) noexcept
{
    return vendorId == Detail::SonyVendorId &&
           (productId == Detail::DualSenseProductId ||
            productId == Detail::DualSenseEdgeProductId);
}

// Builds one USB output report.
// Each setter only marks its own feature as valid, so every feature that is not set
// keeps its current state on the controller.
// Setting the same feature twice overwrites the earlier value.
class OutputReport
{
public:
    using Bytes = uint8_t[Detail::ReportLength];

    OutputReport() noexcept
        : m_data{Detail::UsbOutputReportId}
    {
    }

    OutputReport(const OutputReport&) noexcept = default;
    OutputReport& operator=(const OutputReport&) noexcept = default;
    OutputReport(OutputReport&&) noexcept = default;
    OutputReport& operator=(OutputReport&&) noexcept = default;
    ~OutputReport() noexcept = default;

    // Lightbar / LEDs / Rumble

    // Set the lightbar color (RGB, each 0-255).
    OutputReport& SetLightbar(uint8_t r, uint8_t g, uint8_t b) noexcept
    {
        Detail::SetBits(m_data[Detail::Flag1Offset], Detail::Flag1Lightbar);
        m_data[Detail::LightbarRedOffset]   = r;
        m_data[Detail::LightbarGreenOffset] = g;
        m_data[Detail::LightbarBlueOffset]  = b;
        return *this;
    }

    // Turn the lightbar off.
    // Sets the color to black and requests the firmware fade out.
    OutputReport& TurnOffLightbar() noexcept
    {
        SetLightbar(0, 0, 0);
        Detail::SetBits(m_data[Detail::Flag2Offset], Detail::Flag2LightbarSetup);
        m_data[Detail::LightbarSetupOffset] = Detail::LightbarFadeOut;
        return *this;
    }

    // Set the player indicator.
    OutputReport& SetPlayerLed(PlayerLed player) noexcept
    {
        Detail::SetBits(m_data[Detail::Flag1Offset], Detail::Flag1PlayerLed);
        m_data[Detail::PlayerLedOffset] = Detail::PlayerLedPattern(player);
        return *this;
    }

    // Set the rumble motor intensities (each 0-255).
    // leftMotor is the strong low frequency motor, rightMotor the weak high frequency one.
    OutputReport& SetRumble(uint8_t leftMotor, uint8_t rightMotor) noexcept
    {
        Detail::SetBits(m_data[Detail::Flag0Offset], Detail::Flag0Rumble);
        m_data[Detail::MotorLeftOffset]  = leftMotor;
        m_data[Detail::MotorRightOffset] = rightMotor;
        return *this;
    }

    // Microphone

    // Set the LED inside the mute button without touching the microphone.
    OutputReport& SetMuteLed(MuteLed mode) noexcept
    {
        Detail::SetBits(m_data[Detail::Flag1Offset], Detail::Flag1MuteLed);
        m_data[Detail::MuteLedOffset] = static_cast<uint8_t>(mode);
        return *this;
    }

    // Mute or unmute the built-in microphone and show the state on the mute LED.
    // MuteLed::Off unmutes the microphone, On and Pulse mute it with that LED mode.
    // The mute button does not do this on its own, the host has to react to it.
    // The firmware applies the whole power save byte, so this also resets power save
    // and mute bits that other software may have set.
    OutputReport& SetMicMuted(MuteLed mode) noexcept
    {
        SetMuteLed(mode);
        Detail::SetBits(m_data[Detail::Flag1Offset], Detail::Flag1PowerSave);
        if (mode != MuteLed::Off)
        {
            Detail::SetBits(m_data[Detail::PowerSaveOffset], Detail::PowerSaveMicMute);
        }
        else
        {
            Detail::ClearBits(m_data[Detail::PowerSaveOffset], Detail::PowerSaveMicMute);
        }
        return *this;
    }

    // Adaptive trigger effects.
    //
    // Off uses Sony's official mode 0x05, which also returns the trigger to neutral.
    // Feedback, Weapon and Vibration use the legacy simple modes 0x01, 0x02 and 0x06.
    // They take raw 8 bit parameters, but the firmware does not validate them.
    // Sony's official effects 0x21, 0x25 and 0x26 encode zones and strengths as bit fields.

    // Off: removes any trigger effect and returns the trigger to its neutral position.
    OutputReport& SetTriggerOff(Trigger trigger) noexcept
    {
        BeginTriggerEffect(trigger, Detail::TriggerModeOff);
        return *this;
    }

    // Feedback: no resistance until a starting point, then constant force.
    // startPosition: where resistance begins (0 = top, 255 = bottom).
    // force: amount of feedback after the start point.
    OutputReport& SetTriggerFeedback(
        Trigger trigger,
        uint8_t startPosition,
        uint8_t force) noexcept
    {
        uint8_t* const block =
            BeginTriggerEffect(trigger, Detail::TriggerModeSimpleFeedback);
        block[1] = startPosition;
        block[2] = force;
        return *this;
    }

    // Weapon: resistance region with a breakout force.
    // startPosition: where resistance begins (0 = top, 255 = bottom).
    // endPosition: where resistance ends (0 = top, 255 = bottom).
    // strength: how strong the resistance is to break through.
    OutputReport& SetTriggerWeapon(
        Trigger trigger,
        uint8_t startPosition,
        uint8_t endPosition,
        uint8_t strength) noexcept
    {
        uint8_t* const block =
            BeginTriggerEffect(trigger, Detail::TriggerModeSimpleWeapon);
        block[1] = startPosition;
        block[2] = endPosition;
        block[3] = strength;
        return *this;
    }

    // Vibration: vibrates the trigger with configurable frequency and resistance.
    // frequency: vibration speed (0 = low, 255 = high).
    // resistance: resistance level (0 = high, 255 = low / inverted).
    // startPosition: where vibration and resistance begin (0 = top, 147 = bottom).
    OutputReport& SetTriggerVibration(
        Trigger trigger,
        uint8_t frequency,
        uint8_t resistance,
        uint8_t startPosition) noexcept
    {
        uint8_t* const block =
            BeginTriggerEffect(trigger, Detail::TriggerModeSimpleVibration);
        block[1] = frequency;
        block[2] = resistance;
        block[3] = startPosition;
        return *this;
    }

    // Sending

    // Send the report to the controller.
    HRESULT Send(Device* device) const noexcept
    {
        return Detail::SendReport(device, m_data);
    }

    // Raw report bytes including the report ID.
    const Bytes& Data() const noexcept
    {
        return m_data;
    }

private:
    // Marks the trigger effect as valid, clears its whole block and writes the mode byte.
    uint8_t* BeginTriggerEffect(Trigger trigger, uint8_t mode) noexcept
    {
        Detail::SetBits(m_data[Detail::Flag0Offset], Detail::TriggerFlag(trigger));
        uint8_t* const block = &m_data[Detail::TriggerOffset(trigger)];
        std::memset(block, 0, Detail::TriggerBlockSize);
        block[0] = mode;
        return block;
    }

    Bytes m_data;
};

static_assert(std::is_trivially_copyable<OutputReport>::value,
              "OutputReport must stay a trivially copyable value type.");

// Free functions, each sends a single report for a single feature.

// Set lightbar color (RGB, each 0-255).
inline HRESULT SetDualSenseLightbar(
    Device* device,
    uint8_t r,
    uint8_t g,
    uint8_t b) noexcept
{
    return OutputReport().SetLightbar(r, g, b).Send(device);
}

// Turn off the lightbar (black plus firmware fade out).
inline HRESULT TurnOffDualSenseLightbar(Device* device) noexcept
{
    return OutputReport().TurnOffLightbar().Send(device);
}

// Set player LED indicator.
inline HRESULT SetDualSensePlayerLed(Device* device, PlayerLed player) noexcept
{
    return OutputReport().SetPlayerLed(player).Send(device);
}

// Set rumble motor intensities (each 0-255).
// leftMotor = strong/low-frequency, rightMotor = weak/high-frequency.
inline HRESULT SetDualSenseRumble(
    Device* device,
    uint8_t leftMotor,
    uint8_t rightMotor) noexcept
{
    return OutputReport().SetRumble(leftMotor, rightMotor).Send(device);
}

// Set the mute button LED only, the microphone state is not changed.
inline HRESULT SetDualSenseMuteLed(Device* device, MuteLed mode) noexcept
{
    return OutputReport().SetMuteLed(mode).Send(device);
}

// Mute or unmute the microphone and show the state on the mute LED.
// MuteLed::Off unmutes, On and Pulse mute with that LED mode.
inline HRESULT SetDualSenseMicMuted(Device* device, MuteLed mode) noexcept
{
    return OutputReport().SetMicMuted(mode).Send(device);
}

// See OutputReport::SetTriggerOff.
inline HRESULT SetDualSenseTriggerOff(Device* device, Trigger trigger) noexcept
{
    return OutputReport().SetTriggerOff(trigger).Send(device);
}

// See OutputReport::SetTriggerFeedback.
inline HRESULT SetDualSenseTriggerFeedback(
    Device* device,
    Trigger trigger,
    uint8_t startPosition,
    uint8_t force) noexcept
{
    return OutputReport().SetTriggerFeedback(trigger, startPosition, force).Send(device);
}

// See OutputReport::SetTriggerWeapon.
inline HRESULT SetDualSenseTriggerWeapon(
    Device* device,
    Trigger trigger,
    uint8_t startPosition,
    uint8_t endPosition,
    uint8_t strength) noexcept
{
    return OutputReport()
        .SetTriggerWeapon(trigger, startPosition, endPosition, strength)
        .Send(device);
}

// See OutputReport::SetTriggerVibration.
inline HRESULT SetDualSenseTriggerVibration(
    Device* device,
    Trigger trigger,
    uint8_t frequency,
    uint8_t resistance,
    uint8_t startPosition) noexcept
{
    return OutputReport()
        .SetTriggerVibration(trigger, frequency, resistance, startPosition)
        .Send(device);
}

}} // namespace GameInput::DualSense
