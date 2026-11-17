# DualSense GameInput helper

`DualSenseHelper.h` is a header-only sample for sending adaptive-trigger output reports through GameInput. It identifies supported controllers and provides Off, Feedback, Weapon, and Vibration effects. Use GameInput's lighting and rumble APIs for lighting and ordinary motor vibration, according to the device's advertised capabilities.

## Requirements and supported devices

* GameInput API version 3 or later is required for raw output report creation and sending.
* Sony vendor ID `0x054C`, DualSense product ID `0x0CE6`.
* Sony vendor ID `0x054C`, DualSense Edge product ID `0x0DF2`.

The helper uses the selected header's v3 or v4 interfaces. Device firmware and transport behavior can vary. The device pointer is required and must remain valid for the call. Check every returned `HRESULT`.

## Include and use

Add this directory to your include path or copy `DualSenseHelper.h` into your project, then include it directly. The header includes `<GameInput.h>` and has no other project dependencies.

```cpp
#include "DualSenseHelper.h"

// device is a selected GameInput::v4::IGameInputDevice*.
if (GameInput::DualSense::IsDualSense(device->GetVendorId(), device->GetProductId()))
{
    const HRESULT hr = GameInput::DualSense::SetDualSenseTriggerVibration(
        device, GameInput::DualSense::Trigger::Right, 5, 1, 8);
    if (FAILED(hr))
    {
        // Report the failure to the application.
    }
}
```

Obtain `device` through normal GameInput device discovery and check that its advertised output reports support the helper's report format. With the v3 header, use a `GameInput::v3::IGameInputDevice*` and obtain the VID/PID through `GetDeviceInfo` instead.

## Adaptive-trigger contract

| Helper | Parameters | Effect mode |
| --- | --- | --- |
| `SetDualSenseTriggerOff` | Device and `Trigger::Right` or `Trigger::Left` | `0x05` |
| `SetDualSenseTriggerFeedback` | Position 0-9; strength 0-8 | `0x21` |
| `SetDualSenseTriggerWeapon` | Start 2-7; end greater than start and at most 8; strength 0-8 | `0x25` |
| `SetDualSenseTriggerVibration` | Position 0-9; amplitude 0-8; frequency 0-255 in protocol units | `0x26` |

Positions are protocol zone indices, not measured distances or guaranteed equal percentages of trigger travel. Feedback and Vibration apply from the selected position through zone 9. Strength and amplitude are relative levels, not calibrated force.

Numeric effect parameters are `uint32_t`. Invalid triggers or out-of-range values return `E_INVALIDARG` before report creation or sending, including invalid values combined with zero parameters. Valid zero strength/amplitude or zero vibration frequency sends Off. Each call initializes the complete 11-byte effect block, including reserved bytes. Applying an effect replaces that trigger's previous effect; effects are not layered.

Start with low levels. Call `SetDualSenseTriggerOff` for each trigger your application controls when stopping an effect or relinquishing output ownership. Hardware behavior and physical-force equivalence require device validation.

## Migrating from the previous helper

This is a breaking sample-helper change, not a GameInput runtime API/ABI change.

* The existing trigger helper names now take the discrete parameters above rather than raw-byte positions and forces. Scaling legacy values by 255/9 or 255/8 is not a force-equivalence conversion.
* Vibration's argument order changes from `(frequency, resistance, startPosition)` to `(position, amplitude, frequency)`.
* `PlayerLed`, `SetDualSenseLightbar`, `TurnOffDualSenseLightbar`, `SetDualSensePlayerLed`, `SetDualSenseMuteLed`, and `SetDualSenseRumble` are removed. Use GameInput v4 lighting and standard rumble APIs where supported. No raw-lighting fallback is provided for older API versions.
* The header selects the GameInput interface namespace matching the included API header instead of hard-coding v3.

## Raw report limitations

The helper constructs a 63-byte USB-format output report whose first byte is report ID `0x02`. Report creation and sending errors are propagated; `SetRawData` rejection returns `HRESULT_FROM_WIN32(ERROR_INVALID_DATA)` without sending the report. It does not implement Bluetooth framing, checksums, transport selection, input parsing, or connection management, and makes no claim of Bluetooth support. Raw HID report behavior is device- and firmware-specific; this sample is not an official Sony SDK and does not imply support from Sony Interactive Entertainment.

"PlayStation" is a registered trademark or trademark of Sony Interactive Entertainment Inc.

"DualSense" is a registered trademark or trademark of Sony Interactive Entertainment Inc.
