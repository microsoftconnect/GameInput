# DualSense GameInput helper
 
`DualSenseHelper.h` is a header-only sample for sending raw output reports through GameInput.
It identifies supported controllers and provides helpers for lightbar color and fade-out,
player LEDs, rumble, the microphone mute LED, microphone mute, and adaptive trigger off,
feedback, weapon, and vibration effects.
Every feature is available as a free function that sends one report and as a method of the
`OutputReport` builder, which combines several features into a single report.
 
## Requirements and supported devices
 
* GameInput API version 3 or later is required for raw output report creation and sending.
* C++11 or later.
* Sony vendor ID `0x054C`, DualSense product ID `0x0CE6`.
* Sony vendor ID `0x054C`, DualSense Edge product ID `0x0DF2`.
Device firmware and transport behavior can vary. Check every returned `HRESULT`.
 
## Include and use
 
Add this directory to your include path or copy `DualSenseHelper.h` into your project,
then include it directly. The header includes `<GameInput.h>` and has no other project dependencies.
 
```cpp
#include "DualSenseHelper.h"
 
namespace ds = GameInput::DualSense;
 
const GameInput::v3::GameInputDeviceInfo* info = nullptr;
HRESULT hr = device->GetDeviceInfo(&info);
if (SUCCEEDED(hr) && ds::IsDualSense(info->vendorId, info->productId))
{
    // One report per call.
    hr = ds::SetDualSenseLightbar(device, 255, 0, 0);
    if (SUCCEEDED(hr))
    {
        hr = ds::SetDualSenseRumble(device, 255, 96);
    }
 
    // Several features in one report.
    if (SUCCEEDED(hr))
    {
        ds::OutputReport report;
        report.SetLightbar(0, 0, 255)
              .SetPlayerLed(ds::PlayerLed::Player1)
              .SetTriggerFeedback(ds::Trigger::Right, 0x90, 0xFF);
        hr = report.Send(device);
    }
}
```
 
Obtain `device` as a `GameInput::v3::IGameInputDevice*` through normal GameInput device discovery.
Color, motor, force, position, frequency, resistance, and strength parameters are raw byte values
from 0 through 255, not normalized floating-point values.
The trigger-vibration start position uses 0 through 147 as its documented physical range.
 
## Functions
 
| Free function | `OutputReport` method | Effect |
| --- | --- | --- |
| `SetDualSenseLightbar` | `SetLightbar` | Lightbar color. |
| `TurnOffDualSenseLightbar` | `TurnOffLightbar` | Black lightbar plus firmware fade-out. |
| `SetDualSensePlayerLed` | `SetPlayerLed` | Player indicator, `Off` or `Player1` to `Player5`. |
| `SetDualSenseRumble` | `SetRumble` | Strong (left) and weak (right) rumble motor. |
| `SetDualSenseMuteLed` | `SetMuteLed` | Mute LED (`Off`, `On`, `Pulse`), mic unchanged. |
| `SetDualSenseMicMuted` | `SetMicMuted` | Mutes the mic unless `MuteLed::Off`, LED shows mode. |
| `SetDualSenseTriggerOff` | `SetTriggerOff` | Removes the effect, trigger back to neutral. |
| `SetDualSenseTriggerFeedback` | `SetTriggerFeedback` | Constant resistance from a start point. |
| `SetDualSenseTriggerWeapon` | `SetTriggerWeapon` | Resistance zone with a breakout force. |
| `SetDualSenseTriggerVibration` | `SetTriggerVibration` | Vibration, frequency and resistance. |
 
Every report only marks the features that were set as valid.
The controller keeps the current state of all other features.
Setting the same feature twice on one `OutputReport` overwrites the earlier value.
 
### Microphone mute
 
The controller does not mute its microphone when the mute button is pressed.
The application has to react to the button and call `SetDualSenseMicMuted`.
The firmware applies the power save byte as a whole, so this call also clears power save
and mute bits that other software may have set.
Use `SetDualSenseMuteLed` to drive the LED without changing the microphone.
 
### Adaptive triggers
 
Off uses Sony's official trigger mode `0x05`, which also returns the trigger to its neutral position.
Feedback, weapon, and vibration use the legacy simple modes `0x01`, `0x02`, and `0x06`.
They take raw 8-bit parameters, but the firmware does not validate them.
The official effects `0x21`, `0x25`, and `0x26`, which encode zones and strengths as bit fields,
are not implemented.
 
## Raw report limitations
 
The helper builds a 48-byte USB-format output report:
report ID `0x02` followed by the 47-byte common block.
`SetRawData` requires a buffer of exactly `GetRawDataSize()` bytes,
so the report is zero-padded to the size GameInput reports.
If that size is below 48 or above 64 bytes, the helpers return
`HRESULT_FROM_WIN32(ERROR_BAD_LENGTH)`.
 
The helper does not implement Bluetooth framing, checksums, transport selection, input parsing,
or connection management, and makes no claim of Bluetooth support.
Raw HID report behavior is device- and firmware-specific.
This sample is not an official Sony SDK and does not imply support from Sony Interactive
Entertainment.
 
"PlayStation" is a registered trademark or trademark of Sony Interactive Entertainment Inc.
 
"DualSense" is a registered trademark or trademark of Sony Interactive Entertainment Inc.
