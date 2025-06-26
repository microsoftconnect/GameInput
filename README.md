# GameInput  (https://aka.ms/gameinput)

GameInput is a next-generation input API that exposes input devices of all kinds through a single consistent interface. It's designed with a simple programming model that makes it easy to use. GameInput is built from the ground up for the best possible performance. Key features of the GameInput API are as follows.

## Overview

*   Availability:

    GameInput is available on Xbox via the GDK and PC via NuGet. Older versions of Windows going back to Windows 10 19H1 (May 2019 update) are supported.

*   Consistency:

    GameInput exposes input from keyboards, mice, gamepads, and other game controllers via a single unified input model, synchronized to a common time base. The code for handling input from these devices is nearly identical, using many of the same functions but with different filters applied. This makes it easy to add support for additional input devices, without major changes to input code.

*   Functionality:

    GameInput is a functional superset of all legacy input APIs—XInput, DirectInput, Raw Input, Human Interface Device (HID), and WinRT APIs—in addition to adding new features of its own. GameInput's functionality ranges from simple fixed-format gamepad state to detailed low-level raw device access. Input can be obtained via polling or callbacks in an event-driven way. Haptics and force feedback are fully supported, and third-party device SDKs can easily be written on top of GameInput to provide access to custom device features.

*   Performance:

    GameInput is built around an entirely new direct memory access (DMA) architecture for the lowest possible input latency and resource usage. Nearly all API functions are lock-free with strict performance guarantees, while still being 100 percent thread-safe. This makes them safe to call from time-sensitive contexts such as render threads. Advanced applications can take direct control of scheduling GameInput's internal asynchronous work queues, controlling which thread does the work and how often.

*   Usability

    GameInput was designed with ease of use as a top priority. Most common input tasks can be implemented with just a few lines of code.

## Version History

### Version 1.2

*   Fix inverted axis mapping on multiple controllers.
*   Fix two second timeout on process shutdown for games that leak the IGameInput instance.
*   GameInput will now automatically reconnect to the GameInput service when it is upgraded.
*   Increase the callback backlog to provide more time for games to process GameInput notifications.

### Version 1.1

*   Applications no longer have to call `CoInitialize`.
*   Fixed missing device disconnect notifications.
*   Fixed `FindDeviceFromId` method not returning requested device.
*   Fixed compiler compatibility issues in header.
*   Stability and performance improvements.

### Version 1.0

*   Added support for reading callbacks.
*   Added support for trackpads.
*   Added support for absolute mouse coordinates.
*   Added support for remote sessions.
*   Exposed additional device information, including PnP path and display name.
*   Removed deprecated APIs, fields, and constants.

While this version of GameInput is applicable to PC only, if you are using a previous version of GameInput on PC, or sharing code with GameInput on Xbox using the GDK, please note that many unimplemented functions and corresponding enums and constants have been removed. Additionally, the API has been placed in the `GameInput::v1` namespace to facilitate versioning. Due to these changes, you may experience compilation errors when building that code using this (and future) versions. Notable changes include:

1. `IGameInputDevice::GetDeviceInfo` previously returned the resulting `IGameInputDeviceInfo` struct as the function's return value. This struct is now returned as an out parameter on the function, and the function's return value is now an `HRESULT`.

2. `IGameInput::UnregisterCallback` previously took a timeout value as its second parameter, however this parameter has been removed. In these two cases, along with the touch API used for XCloud on console, use the `GAMEINPUT_API_VERSION` define to conditionally compile (or exclude) code between PC and console.

3. `IGameInputReading::GetSequenceNumber` has been removed. Please use `IGameInputReading::GetTimestamp` instead.

Example:

```cpp
#include <GameInput.h>
#ifndef GAMEINPUT_API_VERSION
#define GAMEINPUT_API_VERSION 0
#endif

#if GAMEINPUT_API_VERSION == 1
using namespace GameInput::v1;
#endif

// ...

#if GAMEINPUT_API_VERSION >= 1
    device->GetDeviceInfo(&deviceInfo);
#else
    deviceInfo = device->GetDeviceInfo();
#endif
```
