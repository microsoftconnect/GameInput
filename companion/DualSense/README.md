# DualSense GameInput helper

`DualSenseHelper.h` is a header-only sample for sending raw output reports through GameInput. It identifies supported controllers and provides helpers for lightbar color and fade-out, player LEDs, rumble, the microphone mute LED, and adaptive trigger off, feedback, weapon, and vibration effects.

## Requirements and supported devices

* GameInput API version 3 or later is required for raw output report creation and sending.
* Sony vendor ID `0x054C`, DualSense product ID `0x0CE6`.
* Sony vendor ID `0x054C`, DualSense Edge product ID `0x0DF2`.

Device firmware and transport behavior can vary. Check every returned `HRESULT`.

## Include and use

Add this directory to your include path or copy `DualSenseHelper.h` into your project, then include it directly. The header includes `<GameInput.h>` and has no other project dependencies.

```cpp
#include "DualSenseHelper.h"

const GameInput::v3::GameInputDeviceInfo* info = nullptr;
HRESULT hr = device->GetDeviceInfo(&info);
if (SUCCEEDED(hr) &&
    GameInput::DualSense::IsDualSense(info->vendorId, info->productId))
{
    hr = GameInput::DualSense::SetDualSenseLightbar(device, 255, 0, 0);
    if (SUCCEEDED(hr))
    {
        hr = GameInput::DualSense::SetDualSenseRumble(device, 255, 96);
    }
}
```

Obtain `device` as a `GameInput::v3::IGameInputDevice*` through normal GameInput device discovery. Color, motor, force, position, frequency, resistance, and strength parameters are raw byte values from 0 through 255, not normalized floating-point values. The trigger-vibration start position uses 0 through 147 as its documented physical range.

## Raw report limitations

The helper constructs a 64-byte USB-format output report whose first byte is report ID `0x02`. It does not implement Bluetooth framing, checksums, transport selection, input parsing, or connection management, and makes no claim of Bluetooth support. Raw HID report behavior is device- and firmware-specific; this sample is not an official Sony SDK and does not imply support from Sony Interactive Entertainment.

"PlayStation" is a registered trademark or trademark of Sony Interactive Entertainment Inc.

"DualSense" is a registered trademark or trademark of Sony Interactive Entertainment Inc.
