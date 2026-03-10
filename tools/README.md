# Tools

## GameInput Device Configuration Tool

### Overview
GameInput Device Configuration Tool is a utility that allows hardware developers to test how the GameInput API recognizes and maps their devices.

HID devices can have their buttons, axis and switches mapped to different input kinds (gamepad, racing wheel, flight stick, arcade stick). XInput and GIP devices have fixed mappings but can still map haptics and sensors settings for GameInput with this tool.

This tool works best with the latest version of GameInput.

More information on this tool can be found at [GameInput Device Mapping Tool and Testing Guide](https://learn.microsoft.com/en-us/gaming/gdk/docs/features/common/input/hardware/input-hardware-mapping?view=gdk-2510).

### Submitting device mappings
The tool allows for the creation of `.reg` for a given device which can then be shared for submission.

If you are a hardware developer and would like to submit device mappings for your device, you can then manually submit the file to Microsoft for consideration (use the 'Contact Owners' link in the [GameInput NuGet page](https://www.nuget.org/packages/Microsoft.GameInput)), ship it with your driver, or include it in your application's installation package. The list of supported devices is updated in cadence with the release of new versions of GameInput, and devices that have been submitted may be added to the list of supported devices in future releases.

