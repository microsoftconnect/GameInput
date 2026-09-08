// Copyright (c) Microsoft Corporation.  All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Microsoft.GameInput.V3
{
    public static class Interop
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct APP_LOCAL_DEVICE_ID : IEquatable<APP_LOCAL_DEVICE_ID>
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public byte[] Value;

            public static bool operator ==(APP_LOCAL_DEVICE_ID left, APP_LOCAL_DEVICE_ID right) => left.Equals(right);
            public static bool operator !=(APP_LOCAL_DEVICE_ID left, APP_LOCAL_DEVICE_ID right) => !left.Equals(right);

            public bool Equals(APP_LOCAL_DEVICE_ID other) =>
                Value.AsSpan().SequenceEqual(other.Value.AsSpan());

            public override bool Equals(object obj) =>
                obj is APP_LOCAL_DEVICE_ID other && Equals(other);

            public override int GetHashCode()
            {
                var hash = new HashCode();
                foreach (byte b in Value)
                {
                    hash.Add(b);
                }

                return hash.ToHashCode();
            }

            public override string ToString() => BitConverter.ToString(Value).Replace("-", "");
        }

        public const int GAMEINPUT_API_VERSION = 3;

        [Flags]
        public enum GameInputKind : int
        {
            GameInputKindUnknown          = 0x00000000,
            GameInputKindRawDeviceReport  = 0x00000001,
            GameInputKindControllerAxis   = 0x00000002,
            GameInputKindControllerButton = 0x00000004,
            GameInputKindControllerSwitch = 0x00000008,
            GameInputKindController       = 0x0000000E,
            GameInputKindKeyboard         = 0x00000010,
            GameInputKindMouse            = 0x00000020,
            GameInputKindSensors          = 0x00000040,
            GameInputKindArcadeStick      = 0x00010000,
            GameInputKindFlightStick      = 0x00020000,
            GameInputKindGamepad          = 0x00040000,
            GameInputKindRacingWheel      = 0x00080000,
        }

        public enum GameInputEnumerationKind : int
        {
            GameInputNoEnumeration       = 0,
            GameInputAsyncEnumeration    = 1,
            GameInputBlockingEnumeration = 2
        }

        [Flags]
        public enum GameInputFocusPolicy : int
        {
            GameInputDefaultFocusPolicy             = 0x00000000,
            GameInputExclusiveForegroundInput       = 0x00000002,
            GameInputExclusiveForegroundGuideButton = 0x00000008,
            GameInputExclusiveForegroundShareButton = 0x00000020,
            GameInputEnableBackgroundInput          = 0x00000040,
            GameInputEnableBackgroundGuideButton    = 0x00000080,
            GameInputEnableBackgroundShareButton    = 0x00000100
        }

        public enum GameInputSwitchKind : int
        {
            GameInputUnknownSwitchKind = -1,
            GameInput2WaySwitch        =  0,
            GameInput4WaySwitch        =  1,
            GameInput8WaySwitch        =  2
        }

        public enum GameInputSwitchPosition : int
        {
            GameInputSwitchCenter    = 0,
            GameInputSwitchUp        = 1,
            GameInputSwitchUpRight   = 2,
            GameInputSwitchRight     = 3,
            GameInputSwitchDownRight = 4,
            GameInputSwitchDown      = 5,
            GameInputSwitchDownLeft  = 6,
            GameInputSwitchLeft      = 7,
            GameInputSwitchUpLeft    = 8
        }

        public enum GameInputKeyboardKind : int
        {
            GameInputUnknownKerboard = -1,
            GameInputAnsiKeyboard    =  0,
            GameInputIsoKeyboard     =  1,
            GameInputKsKeyboard      =  2,
            GameInputAbntKeyboard    =  3,
            GameInputJisKeyboard     =  4
        }

        [Flags]
        public enum GameInputMouseButtons : int
        {
            GameInputMouseNone           = 0x00000000,
            GameInputMouseLeftButton     = 0x00000001,
            GameInputMouseRightButton    = 0x00000002,
            GameInputMouseMiddleButton   = 0x00000004,
            GameInputMouseButton4        = 0x00000008,
            GameInputMouseButton5        = 0x00000010,
            GameInputMouseWheelTiltLeft  = 0x00000020,
            GameInputMouseWheelTiltRight = 0x00000040
        }

        [Flags]
        public enum GameInputMousePositions : int
        {
            GameInputMouseNoPosition       = 0x00000000,
            GameInputMouseAbsolutePosition = 0x00000001,
            GameInputMouseRelativePosition = 0x00000002
        }

        [Flags]
        public enum GameInputSensorsKind : int
        {
            GameInputSensorsNone          = 0x00000000,
            GameInputSensorsAccelerometer = 0x00000001,
            GameInputSensorsGyrometer     = 0x00000002,
            GameInputSensorsCompass       = 0x00000004,
            GameInputSensorsOrientation   = 0x00000008
        }

        public enum GameInputSensorAccuracy : int
        {
            GameInputSensorsAccuracyUnknown     = 0x00000000,
            GameInputSensorsAccuracyUnreliable  = 0x00000001,
            GameInputSensorsAccuracyApproximate = 0x00000002,
            GameInputSensorsAccuracyHigh        = 0x00000003
        }

        [Flags]
        public enum GameInputArcadeStickButtons : int
        {
            GameInputArcadeStickNone     = 0x00000000,
            GameInputArcadeStickMenu     = 0x00000001,
            GameInputArcadeStickView     = 0x00000002,
            GameInputArcadeStickUp       = 0x00000004,
            GameInputArcadeStickDown     = 0x00000008,
            GameInputArcadeStickLeft     = 0x00000010,
            GameInputArcadeStickRight    = 0x00000020,
            GameInputArcadeStickAction1  = 0x00000040,
            GameInputArcadeStickAction2  = 0x00000080,
            GameInputArcadeStickAction3  = 0x00000100,
            GameInputArcadeStickAction4  = 0x00000200,
            GameInputArcadeStickAction5  = 0x00000400,
            GameInputArcadeStickAction6  = 0x00000800,
            GameInputArcadeStickSpecial1 = 0x00001000,
            GameInputArcadeStickSpecial2 = 0x00002000
        }

        [Flags]
        public enum GameInputFlightStickButtons : int
        {
            GameInputFlightStickNone           = 0x00000000,
            GameInputFlightStickMenu           = 0x00000001,
            GameInputFlightStickView           = 0x00000002,
            GameInputFlightStickFirePrimary    = 0x00000004,
            GameInputFlightStickFireSecondary  = 0x00000008,
            GameInputFlightStickHatSwitchUp    = 0x00000010,
            GameInputFlightStickHatSwitchDown  = 0x00000020,
            GameInputFlightStickHatSwitchLeft  = 0x00000040,
            GameInputFlightStickHatSwitchRight = 0x00000080,
            GameInputFlightStickA              = 0x00000100,
            GameInputFlightStickB              = 0x00000200,
            GameInputFlightStickX              = 0x00000400,
            GameInputFlightStickY              = 0x00000800,
            GameInputFlightStickLeftShoulder   = 0x00001000,
            GameInputFlightStickRightShoulder  = 0x00002000,
        }

        [Flags]
        public enum GameInputGamepadButtons : int
        {
            GameInputGamepadNone                 = 0x00000000,
            GameInputGamepadMenu                 = 0x00000001,
            GameInputGamepadView                 = 0x00000002,
            GameInputGamepadA                    = 0x00000004,
            GameInputGamepadB                    = 0x00000008,
            GameInputGamepadC                    = 0x00004000,
            GameInputGamepadX                    = 0x00000010,
            GameInputGamepadY                    = 0x00000020,
            GameInputGamepadZ                    = 0x00008000,
            GameInputGamepadDPadUp               = 0x00000040,
            GameInputGamepadDPadDown             = 0x00000080,
            GameInputGamepadDPadLeft             = 0x00000100,
            GameInputGamepadDPadRight            = 0x00000200,
            GameInputGamepadLeftShoulder         = 0x00000400,
            GameInputGamepadRightShoulder        = 0x00000800,
            GameInputGamepadLeftTriggerButton    = 0x00010000,
            GameInputGamepadRightTriggerButton   = 0x00020000,
            GameInputGamepadLeftThumbstick       = 0x00001000,
            GameInputGamepadLeftThumbstickUp     = 0x00040000,
            GameInputGamepadLeftThumbstickDown   = 0x00080000,
            GameInputGamepadLeftThumbstickLeft   = 0x00100000,
            GameInputGamepadLeftThumbstickRight  = 0x00200000,
            GameInputGamepadRightThumbstick      = 0x00002000,
            GameInputGamepadRightThumbstickUp    = 0x00400000,
            GameInputGamepadRightThumbstickDown  = 0x00800000,
            GameInputGamepadRightThumbstickLeft  = 0x01000000,
            GameInputGamepadRightThumbstickRight = 0x02000000,
            GameInputGamepadPaddleLeft1          = 0x04000000,
            GameInputGamepadPaddleLeft2          = 0x08000000,
            GameInputGamepadPaddleRight1         = 0x10000000,
            GameInputGamepadPaddleRight2         = 0x20000000,
        }

        // Gamepad modules (Groupings of gamepad elements commonly found together)
        public const GameInputGamepadButtons GameInputGamepadModuleSystemDuo =
            GameInputGamepadButtons.GameInputGamepadMenu |
            GameInputGamepadButtons.GameInputGamepadView;

        public const GameInputGamepadButtons GameInputGamepadModuleDpad =
            GameInputGamepadButtons.GameInputGamepadDPadUp |
            GameInputGamepadButtons.GameInputGamepadDPadDown |
            GameInputGamepadButtons.GameInputGamepadDPadLeft |
            GameInputGamepadButtons.GameInputGamepadDPadRight;

        public const GameInputGamepadButtons GameInputGamepadModuleShoulders =
            GameInputGamepadButtons.GameInputGamepadLeftShoulder |
            GameInputGamepadButtons.GameInputGamepadRightShoulder;

        public const GameInputGamepadButtons GameInputGamepadModuleTriggers =
            GameInputGamepadButtons.GameInputGamepadLeftTriggerButton |
            GameInputGamepadButtons.GameInputGamepadRightTriggerButton;

        public const GameInputGamepadButtons GameInputGamepadModuleThumbsticks =
            GameInputGamepadButtons.GameInputGamepadLeftThumbstickUp |
            GameInputGamepadButtons.GameInputGamepadLeftThumbstickDown |
            GameInputGamepadButtons.GameInputGamepadLeftThumbstickLeft |
            GameInputGamepadButtons.GameInputGamepadLeftThumbstickRight |
            GameInputGamepadButtons.GameInputGamepadRightThumbstickUp |
            GameInputGamepadButtons.GameInputGamepadRightThumbstickDown |
            GameInputGamepadButtons.GameInputGamepadRightThumbstickLeft |
            GameInputGamepadButtons.GameInputGamepadRightThumbstickRight;

        public const GameInputGamepadButtons GameInputGamepadModulePaddles2 =
            GameInputGamepadButtons.GameInputGamepadPaddleLeft1 |
            GameInputGamepadButtons.GameInputGamepadPaddleRight1;

        public const GameInputGamepadButtons GameInputGamepadModulePaddles4 =
            GameInputGamepadButtons.GameInputGamepadPaddleLeft1 |
            GameInputGamepadButtons.GameInputGamepadPaddleLeft2 |
            GameInputGamepadButtons.GameInputGamepadPaddleRight1 |
            GameInputGamepadButtons.GameInputGamepadPaddleRight2;

        // Commonly found gamepad layouts. Custom layouts are possible and encouraged.
        public const GameInputGamepadButtons GameInputGamepadLayoutBasic =
            GameInputGamepadModuleSystemDuo |
            GameInputGamepadModuleDpad |
            GameInputGamepadButtons.GameInputGamepadA |
            GameInputGamepadButtons.GameInputGamepadB;

        public const GameInputGamepadButtons GameInputGamepadLayoutButtons =
            GameInputGamepadLayoutBasic |
            GameInputGamepadButtons.GameInputGamepadX |
            GameInputGamepadButtons.GameInputGamepadY |
            GameInputGamepadModuleShoulders;

        public const GameInputGamepadButtons GameInputGamepadLayoutStandard =
            GameInputGamepadLayoutButtons |
            GameInputGamepadModuleTriggers |
            GameInputGamepadModuleThumbsticks |
            GameInputGamepadButtons.GameInputGamepadLeftThumbstick |
            GameInputGamepadButtons.GameInputGamepadRightThumbstick;

        public const GameInputGamepadButtons GameInputGamepadLayoutElite =
            GameInputGamepadLayoutStandard |
            GameInputGamepadModulePaddles4;


        [Flags]
        public enum GameInputRacingWheelButtons : int
        {
            GameInputRacingWheelNone            = 0x00000000,
            GameInputRacingWheelMenu            = 0x00000001,
            GameInputRacingWheelView            = 0x00000002,
            GameInputRacingWheelPreviousGear    = 0x00000004,
            GameInputRacingWheelNextGear        = 0x00000008,
            GameInputRacingWheelA               = 0x00000100,
            GameInputRacingWheelB               = 0x00000200,
            GameInputRacingWheelX               = 0x00000400,
            GameInputRacingWheelY               = 0x00000800,
            GameInputRacingWheelDpadUp          = 0x00000010,
            GameInputRacingWheelDpadDown        = 0x00000020,
            GameInputRacingWheelDpadLeft        = 0x00000040,
            GameInputRacingWheelDpadRight       = 0x00000080,
            GameInputRacingWheelLeftThumbstick  = 0x00001000,
            GameInputRacingWheelRightThumbstick = 0x00002000,
        }

        public enum GameInputRawDeviceReportKind : int
        {
            GameInputRawInputReport  = 0,
            GameInputRawOutputReport = 1,
        }

        [Flags]
        public enum GameInputSystemButtons : int
        {
            GameInputSystemButtonNone  = 0x00000000,
            GameInputSystemButtonGuide = 0x00000001,
            GameInputSystemButtonShare = 0x00000002
        }

        [Flags]
        public enum GameInputFlightStickAxes : int
        {
            GameInputFlightStickAxesNone = 0x00000000,
            GameInputFlightStickRoll     = 0x00000010,
            GameInputFlightStickPitch    = 0x00000020,
            GameInputFlightStickYaw      = 0x00000040,
            GameInputFlightStickThrottle = 0x00000080,
        }

        [Flags]
        public enum GameInputGamepadAxes : int
        {
            GameInputGamepadAxesNone         = 0x00000000,
            GameInputGamepadLeftTrigger      = 0x00000001,
            GameInputGamepadRightTrigger     = 0x00000002,
            GameInputGamepadLeftThumbstickX  = 0x00000004,
            GameInputGamepadLeftThumbstickY  = 0x00000008,
            GameInputGamepadRightThumbstickX = 0x00000010,
            GameInputGamepadRightThumbstickY = 0x00000020,
        }

        [Flags]
        public enum GameInputRacingWheelAxes : int
        {
            GameInputRacingWheelAxesNone       = 0x00000000,
            GameInputRacingWheelSteering       = 0x00000100,
            GameInputRacingWheelThrottle       = 0x00000200,
            GameInputRacingWheelBrake          = 0x00000400,
            GameInputRacingWheelClutch         = 0x00000800,
            GameInputRacingWheelHandbrake      = 0x00001000,
            GameInputRacingWheelPatternShifter = 0x00002000,
        }

        [Flags]
        public enum GameInputDeviceStatus : int
        {
            GameInputDeviceNoStatus        = 0x00000000,
            GameInputDeviceConnected       = 0x00000001,
            GameInputDeviceHapticInfoReady = 0x00200000,
            GameInputDeviceAnyStatus       = unchecked((int)0xFFFFFFFF)
        }

        public enum GameInputDeviceFamily : int
        {
            GameInputFamilyVirtual   = -1,
            GameInputFamilyUnknown   = 0,
            GameInputFamilyXboxOne   = 1,
            GameInputFamilyXbox360   = 2,
            GameInputFamilyHid       = 3,
            GameInputFamilyI8042     = 4,
            GameInputFamilyAggregate = 5,
        }

        public enum GameInputLabel : int
        {
            GameInputLabelUnknown                  = -1,
            GameInputLabelNone                     = 0,
            GameInputLabelXboxGuide                = 1,
            GameInputLabelXboxBack                 = 2,
            GameInputLabelXboxStart                = 3,
            GameInputLabelXboxMenu                 = 4,
            GameInputLabelXboxView                 = 5,
            GameInputLabelXboxA                    = 7,
            GameInputLabelXboxB                    = 8,
            GameInputLabelXboxX                    = 9,
            GameInputLabelXboxY                    = 10,
            GameInputLabelXboxDPadUp               = 11,
            GameInputLabelXboxDPadDown             = 12,
            GameInputLabelXboxDPadLeft             = 13,
            GameInputLabelXboxDPadRight            = 14,
            GameInputLabelXboxLeftShoulder         = 15,
            GameInputLabelXboxLeftTrigger          = 16,
            GameInputLabelXboxLeftStickButton      = 17,
            GameInputLabelXboxRightShoulder        = 18,
            GameInputLabelXboxRightTrigger         = 19,
            GameInputLabelXboxRightStickButton     = 20,
            GameInputLabelXboxPaddle1              = 21,
            GameInputLabelXboxPaddle2              = 22,
            GameInputLabelXboxPaddle3              = 23,
            GameInputLabelXboxPaddle4              = 24,
            GameInputLabelLetterA                  = 25,
            GameInputLabelLetterB                  = 26,
            GameInputLabelLetterC                  = 27,
            GameInputLabelLetterD                  = 28,
            GameInputLabelLetterE                  = 29,
            GameInputLabelLetterF                  = 30,
            GameInputLabelLetterG                  = 31,
            GameInputLabelLetterH                  = 32,
            GameInputLabelLetterI                  = 33,
            GameInputLabelLetterJ                  = 34,
            GameInputLabelLetterK                  = 35,
            GameInputLabelLetterL                  = 36,
            GameInputLabelLetterM                  = 37,
            GameInputLabelLetterN                  = 38,
            GameInputLabelLetterO                  = 39,
            GameInputLabelLetterP                  = 40,
            GameInputLabelLetterQ                  = 41,
            GameInputLabelLetterR                  = 42,
            GameInputLabelLetterS                  = 43,
            GameInputLabelLetterT                  = 44,
            GameInputLabelLetterU                  = 45,
            GameInputLabelLetterV                  = 46,
            GameInputLabelLetterW                  = 47,
            GameInputLabelLetterX                  = 48,
            GameInputLabelLetterY                  = 49,
            GameInputLabelLetterZ                  = 50,
            GameInputLabelNumber0                  = 51,
            GameInputLabelNumber1                  = 52,
            GameInputLabelNumber2                  = 53,
            GameInputLabelNumber3                  = 54,
            GameInputLabelNumber4                  = 55,
            GameInputLabelNumber5                  = 56,
            GameInputLabelNumber6                  = 57,
            GameInputLabelNumber7                  = 58,
            GameInputLabelNumber8                  = 59,
            GameInputLabelNumber9                  = 60,
            GameInputLabelArrowUp                  = 61,
            GameInputLabelArrowUpRight             = 62,
            GameInputLabelArrowRight               = 63,
            GameInputLabelArrowDownRight           = 64,
            GameInputLabelArrowDown                = 65,
            GameInputLabelArrowDownLLeft           = 66,
            GameInputLabelArrowLeft                = 67,
            GameInputLabelArrowUpLeft              = 68,
            GameInputLabelArrowUpDown              = 69,
            GameInputLabelArrowLeftRight           = 70,
            GameInputLabelArrowUpDownLeftRight     = 71,
            GameInputLabelArrowClockwise           = 72,
            GameInputLabelArrowCounterClockwise    = 73,
            GameInputLabelArrowReturn              = 74,
            GameInputLabelIconBranding             = 75,
            GameInputLabelIconHome                 = 76,
            GameInputLabelIconMenu                 = 77,
            GameInputLabelIconCross                = 78,
            GameInputLabelIconCircle               = 79,
            GameInputLabelIconSquare               = 80,
            GameInputLabelIconTriangle             = 81,
            GameInputLabelIconStar                 = 82,
            GameInputLabelIconDPadUp               = 83,
            GameInputLabelIconDPadDown             = 84,
            GameInputLabelIconDPadLeft             = 85,
            GameInputLabelIconDPadRight            = 86,
            GameInputLabelIconDialClockwise        = 87,
            GameInputLabelIconDialCounterClockwise = 88,
            GameInputLabelIconSliderLeftRight      = 89,
            GameInputLabelIconSliderUpDown         = 90,
            GameInputLabelIconWheelUpDown          = 91,
            GameInputLabelIconPlus                 = 92,
            GameInputLabelIconMinus                = 93,
            GameInputLabelIconSuspension           = 94,
            GameInputLabelHome                     = 95,
            GameInputLabelGuide                    = 96,
            GameInputLabelMode                     = 97,
            GameInputLabelSelect                   = 98,
            GameInputLabelMenu                     = 99,
            GameInputLabelView                     = 100,
            GameInputLabelBack                     = 101,
            GameInputLabelStart                    = 102,
            GameInputLabelOptions                  = 103,
            GameInputLabelShare                    = 104,
            GameInputLabelUp                       = 105,
            GameInputLabelDown                     = 106,
            GameInputLabelLeft                     = 107,
            GameInputLabelRight                    = 108,
            GameInputLabelLB                       = 109,
            GameInputLabelLT                       = 110,
            GameInputLabelLSB                      = 111,
            GameInputLabelL1                       = 112,
            GameInputLabelL2                       = 113,
            GameInputLabelL3                       = 114,
            GameInputLabelRB                       = 115,
            GameInputLabelRT                       = 116,
            GameInputLabelRSB                      = 117,
            GameInputLabelR1                       = 118,
            GameInputLabelR2                       = 119,
            GameInputLabelR3                       = 120,
            GameInputLabelPaddleLeft1              = 121,
            GameInputLabelPaddleLeft2              = 122,
            GameInputLabelPaddleRight1             = 123,
            GameInputLabelPaddleRight2             = 124,
        }

        [Flags]
        public enum GameInputFeedbackAxes : int
        {
            GameInputFeedbackAxisNone     = 0x00000000,
            GameInputFeedbackAxisLinearX  = 0x00000001,
            GameInputFeedbackAxisLinearY  = 0x00000002,
            GameInputFeedbackAxisLinearZ  = 0x00000004,
            GameInputFeedbackAxisAngularX = 0x00000008,
            GameInputFeedbackAxisAngularY = 0x00000010,
            GameInputFeedbackAxisNormal   = 0x00000040
        }

        public enum GameInputFeedbackEffectState : int
        {
            GameInputFeedbackStopped = 0,
            GameInputFeedbackRunning = 1,
            GameInputFeedbackPaused  = 2
        }

        public enum GameInputForceFeedbackEffectKind : int
        {
            GameInputForceFeedbackConstant         = 0,
            GameInputForceFeedbackRamp             = 1,
            GameInputForceFeedbackSineWave         = 2,
            GameInputForceFeedbackSquareWave       = 3,
            GameInputForceFeedbackTriangleWave     = 4,
            GameInputForceFeedbackSawtoothUpWave   = 5,
            GameInputForceFeedbackSawtoothDownWave = 6,
            GameInputForceFeedbackSpring           = 7,
            GameInputForceFeedbackFriction         = 8,
            GameInputForceFeedbackDamper           = 9,
            GameInputForceFeedbackInertia          = 10
        }

        [Flags]
        public enum GameInputRumbleMotors : int
        {
            GameInputRumbleNone          = 0x00000000,
            GameInputRumbleLowFrequency  = 0x00000001,
            GameInputRumbleHighFrequency = 0x00000002,
            GameInputRumbleLeftTrigger   = 0x00000004,
            GameInputRumbleRightTrigger  = 0x00000008
        }

        public enum GameInputElementKind : int
        {
            GameInputElementKindNone   = 0,
            GameInputElementKindAxis   = 1,
            GameInputElementKindButton = 2,
            GameInputElementKindSwitch = 3
        }

        public static readonly Guid GAMEINPUT_HAPTIC_LOCATION_NONE          = new Guid(0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
        public static readonly Guid GAMEINPUT_HAPTIC_LOCATION_GRIP_LEFT     = new Guid(0x08c707c2, 0x66bb, 0x406c, 0xa8, 0x4a, 0xdf, 0xe0, 0x85, 0x12, 0x0a, 0x92);
        public static readonly Guid GAMEINPUT_HAPTIC_LOCATION_GRIP_RIGHT    = new Guid(0x155a0b77, 0x8bb2, 0x40db, 0x86, 0x90, 0xb6, 0xd4, 0x11, 0x26, 0xdf, 0xc1);
        public static readonly Guid GAMEINPUT_HAPTIC_LOCATION_TRIGGER_LEFT  = new Guid(0x8de4d896, 0x5559, 0x4081, 0x86, 0xe5, 0x17, 0x24, 0xcc, 0x07, 0xc6, 0xbc);
        public static readonly Guid GAMEINPUT_HAPTIC_LOCATION_TRIGGER_RIGHT = new Guid(0xff0cb557, 0x3af5, 0x406b, 0x8b, 0x0f, 0x55, 0x5a, 0x2d, 0x92, 0xa2, 0x20);

        public const uint GAMEINPUT_HAPTIC_MAX_LOCATIONS              = 8;
        public const uint GAMEINPUT_HAPTIC_MAX_AUDIO_ENDPOINT_ID_SIZE = 256;

        public delegate void GameInputReadingCallback(
            [In] ulong callbackToken,
            [In] IntPtr context,
            [In, MarshalAs(UnmanagedType.Interface)] IGameInputReading reading);

        public delegate void GameInputDeviceCallback(
            [In] ulong callbackToken,
            [In] IntPtr context,
            [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
            [In] ulong timestamp,
            [In] GameInputDeviceStatus currentStatus,
            [In] GameInputDeviceStatus previousStatus);

        public delegate void GameInputSystemButtonCallback(
            [In] ulong callbackToken,
            [In] IntPtr context,
            [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
            [In] ulong timestamp,
            [In] GameInputSystemButtons currentButtons,
            [In] GameInputSystemButtons previousButtons);

        public delegate void GameInputKeyboardLayoutCallback(
            [In] ulong callbackToken,
            [In] IntPtr context,
            [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
            [In] ulong timestamp,
            [In] uint currentLayout,
            [In] uint previousLayout);

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputKeyState
        {
            public uint scanCode;
            public uint codePoint;
            public byte virtualKey;
            [MarshalAs(UnmanagedType.I1)]
            public bool isDeadKey;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputMouseState
        {
            public GameInputMouseButtons   buttons;
            public GameInputMousePositions positions;
            public long                    positionX;
            public long                    positionY;
            public long                    absolutePositionX;
            public long                    absolutePositionY;
            public long                    wheelX;
            public long                    wheelY;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputVersion
        {
            public ushort major;
            public ushort minor;
            public ushort build;
            public ushort revision;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputSensorsState
        {
            // GameInputSensorsAccelerometer
            public float accelerationInGX;
            public float accelerationInGY;
            public float accelerationInGZ;

            // GameInputSensorsGyrometer
            public float angularVelocityInRadPerSecX;
            public float angularVelocityInRadPerSecY;
            public float angularVelocityInRadPerSecZ;

            // GameInputSensorsCompass
            public float headingInDegreesFromMagneticNorth;
            public GameInputSensorAccuracy headingAccuracy;

            // GameInputSensorsOrientation
            public float orientationW;
            public float orientationX;
            public float orientationY;
            public float orientationZ;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputArcadeStickState
        {
            public GameInputArcadeStickButtons buttons;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputFlightStickState
        {
            public GameInputFlightStickButtons buttons;
            public GameInputSwitchPosition     hatSwitch;
            public float                       roll;
            public float                       pitch;
            public float                       yaw;
            public float                       throttle;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputGamepadState
        {
            public GameInputGamepadButtons buttons;
            public float                   leftTrigger;
            public float                   rightTrigger;
            public float                   leftThumbstickX;
            public float                   leftThumbstickY;
            public float                   rightThumbstickX;
            public float                   rightThumbstickY;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputRacingWheelState
        {
            public GameInputRacingWheelButtons buttons;
            public int                       patternShifterGear;
            public float                     wheel;
            public float                     throttle;
            public float                     brake;
            public float                     clutch;
            public float                     handbrake;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputUsage
        {
            public ushort page;
            public ushort id;
        }

        public const uint GAMEINPUT_MAX_SWITCH_STATES = 8;

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputControllerSwitchInfo
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)GAMEINPUT_MAX_SWITCH_STATES)]
            public GameInputLabel[]    labels;
            public GameInputSwitchKind kind;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputControllerInfo
        {
            public uint   controllerAxisCount;
            public IntPtr controllerAxisLabels;
            public uint   controllerButtonCount;
            public IntPtr controllerButtonLabels;
            public uint   controllerSwitchCount;
            public IntPtr controllerSwitchInfo;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputKeyboardInfo
        {
            public GameInputKeyboardKind kind;
            public uint                  layout;
            public uint                  keyCount;
            public uint                  functionKeyCount;
            public uint                  maxSimultaneousKeys;
            public uint                  platformType;
            public uint                  platformSubtype;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputMouseInfo
        {
            public GameInputMouseButtons supportedButtons;
            public uint                  sampleRate;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  hasWheelX;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  hasWheelY;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputSensorsInfo
        {
            public GameInputSensorsKind supportedSensors;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputArcadeStickInfo
        {
            public GameInputLabel menuButtonLabel;
            public GameInputLabel viewButtonLabel;
            public GameInputLabel stickUpLabel;
            public GameInputLabel stickDownLabel;
            public GameInputLabel stickLeftLabel;
            public GameInputLabel stickRightLabel;
            public GameInputLabel actionButton1Label;
            public GameInputLabel actionButton2Label;
            public GameInputLabel actionButton3Label;
            public GameInputLabel actionButton4Label;
            public GameInputLabel actionButton5Label;
            public GameInputLabel actionButton6Label;
            public GameInputLabel specialButton1Label;
            public GameInputLabel specialButton2Label;
            public uint           extraButtonCount;
            public uint           extraAxisCount;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputFlightStickInfo
        {
            public GameInputLabel menuButtonLabel;
            public GameInputLabel viewButtonLabel;
            public GameInputLabel firePrimaryButtonLabel;
            public GameInputLabel fireSecondaryButtonLabel;
            public GameInputLabel hatSwitchUpLabel;
            public GameInputLabel hatSwitchDownLabel;
            public GameInputLabel hatSwitchLeftLabel;
            public GameInputLabel hatSwitchRightLabel;
            public GameInputLabel aButtonLabel;
            public GameInputLabel bButtonLabel;
            public GameInputLabel xButtonLabel;
            public GameInputLabel yButtonLabel;
            public GameInputLabel leftShoulderButtonLabel;
            public GameInputLabel rightShoulderButtonLabel;
            public uint           extraButtonCount;
            public uint           extraAxisCount;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputGamepadInfo
        {
            public GameInputGamepadButtons supportedLayout;
            public GameInputLabel          menuButtonLabel;
            public GameInputLabel          viewButtonLabel;
            public GameInputLabel          aButtonLabel;
            public GameInputLabel          bButtonLabel;
            public GameInputLabel          cButtonLabel;
            public GameInputLabel          xButtonLabel;
            public GameInputLabel          yButtonLabel;
            public GameInputLabel          zButtonLabel;
            public GameInputLabel          dpadUpLabel;
            public GameInputLabel          dpadDownLabel;
            public GameInputLabel          dpadLeftLabel;
            public GameInputLabel          dpadRightLabel;
            public GameInputLabel          leftShoulderButtonLabel;
            public GameInputLabel          rightShoulderButtonLabel;
            public GameInputLabel          leftThumbstickButtonLabel;
            public GameInputLabel          rightThumbstickButtonLabel;
            public uint                    extraButtonCount;
            public uint                    extraAxisCount;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputRacingWheelInfo
        {
            public GameInputLabel menuButtonLabel;
            public GameInputLabel viewButtonLabel;
            public GameInputLabel previousGearButtonLabel;
            public GameInputLabel nextGearButtonLabel;
            public GameInputLabel dpadUpLabel;
            public GameInputLabel dpadDownLabel;
            public GameInputLabel dpadLeftLabel;
            public GameInputLabel dpadRightLabel;
            public GameInputLabel aButtonLabel;
            public GameInputLabel bButtonLabel;
            public GameInputLabel xButtonLabel;
            public GameInputLabel yButtonLabel;
            public GameInputLabel leftThumbstickButtonLabel;
            public GameInputLabel rightThumbstickButtonLabel;
            [MarshalAs(UnmanagedType.I1)]
            public bool           hasClutch;
            [MarshalAs(UnmanagedType.I1)]
            public bool           hasHandbrake;
            [MarshalAs(UnmanagedType.I1)]
            public bool           hasPatternShifter;
            public int            minPatternShifterGear;
            public int            maxPatternShifterGear;
            public float          maxWheelAngle;
            public uint           extraButtonCount;
            public uint           extraAxisCount;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputForceFeedbackMotorInfo
        {
            public GameInputFeedbackAxes supportedAxes;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isConstantEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isRampEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isSineWaveEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isSquareWaveEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isTriangleWaveEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isSawtoothUpWaveEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isSawtoothDownWaveEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isSpringEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isFrictionEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isDamperEffectSupported;
            [MarshalAs(UnmanagedType.I1)]
            public bool                  isInertiaEffectSupported;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputRawDeviceReportInfo
        {
            public GameInputRawDeviceReportKind kind;
            public uint                         id;
            public uint                         size;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct GameInputDeviceInfo
        {
            public ushort                 vendorId;
            public ushort                 productId;
            public ushort                 revisionNumber;
            public GameInputUsage         usage;
            public GameInputVersion       hardwareVersion;
            public GameInputVersion       firmwareVersion;
            public APP_LOCAL_DEVICE_ID    deviceId;
            public APP_LOCAL_DEVICE_ID    deviceRootId;
            public GameInputDeviceFamily  deviceFamily;
            public GameInputKind          supportedInput;
            public GameInputRumbleMotors  supportedRumbleMotors;
            public GameInputSystemButtons supportedSystemButtons;
            public Guid                   containerId;
            public IntPtr                 displayName;
            public IntPtr                 pnpPath;

            public IntPtr keyboardInfo;
            public IntPtr mouseInfo;
            public IntPtr sensorsInfo;
            public IntPtr controllerInfo;
            public IntPtr arcadeStickInfo;
            public IntPtr flightStickInfo;
            public IntPtr gamepadInfo;
            public IntPtr racingWheelInfo;

            public uint  forceFeedbackMotorCount;
            public IntPtr forceFeedbackMotorInfo;

            public uint   inputReportCount;
            public IntPtr inputReportInfo;

            public uint   outputReportCount;
            public IntPtr outputReportInfo;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct GameInputHapticInfo
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = (int)GAMEINPUT_HAPTIC_MAX_AUDIO_ENDPOINT_ID_SIZE)]
            public string audioEndpointId;
            public uint   locationCount;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)GAMEINPUT_HAPTIC_MAX_LOCATIONS)]
            public Guid[] locations;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputForceFeedbackEnvelope
        {
            public ulong attackDuration;
            public ulong sustainDuration;
            public ulong releaseDuration;
            public float attackGain;
            public float sustainGain;
            public float releaseGain;
            public uint  playCount;
            public ulong repeatDelay;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputForceFeedbackMagnitude
        {
            public float linearX;
            public float linearY;
            public float linearZ;
            public float angularX;
            public float angularY;
            public float angularZ;
            public float normal;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputForceFeedbackConditionParams
        {
            public GameInputForceFeedbackMagnitude magnitude;
            public float                           positiveCoefficient;
            public float                           negativeCoefficient;
            public float                           maxPositiveMagnitude;
            public float                           maxNegativeMagnitude;
            public float                           deadZone;
            public float                           bias;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputForceFeedbackConstantParams
        {
            public GameInputForceFeedbackEnvelope  envelope;
            public GameInputForceFeedbackMagnitude magnitude;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputForceFeedbackPeriodicParams
        {
            public GameInputForceFeedbackEnvelope  envelope;
            public GameInputForceFeedbackMagnitude magnitude;
            public float                           frequency;
            public float                           phase;
            public float                           bias;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputForceFeedbackRampParams
        {
            public GameInputForceFeedbackEnvelope  envelope;
            public GameInputForceFeedbackMagnitude startMagnitude;
            public GameInputForceFeedbackMagnitude endMagnitude;
        }

        [StructLayout(LayoutKind.Explicit)]
        public struct GameInputForceFeedbackParams
        {
            [FieldOffset(0)]
            public GameInputForceFeedbackEffectKind kind;

            [FieldOffset(4)]
            public GameInputForceFeedbackConstantParams constant;

            [FieldOffset(4)]
            public GameInputForceFeedbackRampParams ramp;

            [FieldOffset(4)]
            public GameInputForceFeedbackPeriodicParams sineWave;

            [FieldOffset(4)]
            public GameInputForceFeedbackPeriodicParams squareWave;

            [FieldOffset(4)]
            public GameInputForceFeedbackPeriodicParams triangleWave;

            [FieldOffset(4)]
            public GameInputForceFeedbackPeriodicParams sawtoothUpWave;

            [FieldOffset(4)]
            public GameInputForceFeedbackPeriodicParams sawtoothDownWave;

            [FieldOffset(4)]
            public GameInputForceFeedbackConditionParams spring;

            [FieldOffset(4)]
            public GameInputForceFeedbackConditionParams friction;

            [FieldOffset(4)]
            public GameInputForceFeedbackConditionParams damper;

            [FieldOffset(4)]
            public GameInputForceFeedbackConditionParams inertia;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputRumbleParams
        {
            public float lowFrequency;
            public float highFrequency;
            public float leftTrigger;
            public float rightTrigger;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputAxisMapping
        {
            public GameInputElementKind controllerElementKind;
            public uint controllerIndex;

            // When axis is mapped from a axis
            [MarshalAs(UnmanagedType.I1)]
            public bool isInverted;

            // When the axis is mapped from a button
            [MarshalAs(UnmanagedType.I1)]
            public bool fromTwoButtons;
            public uint buttonMinIndexValue;

            // When the axis is mapped from a switch
            public GameInputSwitchPosition referenceDirection;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GameInputButtonMapping
        {
            public GameInputElementKind controllerElementKind;
            public uint controllerIndex;

            // When the button is mapped from an axis
            [MarshalAs(UnmanagedType.I1)]
            public bool isInverted;

            // Button mapped from button only needs the index

            // When the button is mapped from a switch
            public GameInputSwitchPosition switchPosition;
        }


        [ComImport, Guid("20EFC1C7-5D9A-43BA-B26F-B807FA48609C"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        public interface IGameInput
        {
            [PreserveSig]
            ulong GetCurrentTimestamp();

            [PreserveSig]
            int GetCurrentReading(
                [In] GameInputKind inputKind,
                [In, Optional, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
                [Out, MarshalAs(UnmanagedType.Interface)] out IGameInputReading reading);

            [PreserveSig]
            int GetNextReading(
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputReading referenceReading,
                [In] GameInputKind inputKind,
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
                [Out, MarshalAs(UnmanagedType.Interface)] out IGameInputReading reading);

            [PreserveSig]
            int GetPreviousReading(
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputReading referenceReading,
                [In] GameInputKind inputKind,
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
                [Out, MarshalAs(UnmanagedType.Interface)] out IGameInputReading reading);

            [PreserveSig]
            int RegisterReadingCallback(
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
                [In] GameInputKind inputKind,
                [In] IntPtr context,
                [In, MarshalAs(UnmanagedType.FunctionPtr)] GameInputReadingCallback callbackFunc,
                [Out] out ulong callbackToken);

            [PreserveSig]
            int RegisterDeviceCallback(
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
                [In] GameInputKind inputKind,
                [In] GameInputDeviceStatus statusFilter,
                [In] GameInputEnumerationKind enumerationKind,
                [In] IntPtr context,
                [In, MarshalAs(UnmanagedType.FunctionPtr)] GameInputDeviceCallback callbackFunc,
                [Out] out ulong callbackToken);

            [PreserveSig]
            int RegisterSystemButtonCallback(
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
                [In] GameInputSystemButtons buttonFilter,
                [In] IntPtr context,
                [In, MarshalAs(UnmanagedType.FunctionPtr)] GameInputSystemButtonCallback callbackFunc,
                [Out] out ulong callbackToken);

            [PreserveSig]
            int RegisterKeyboardLayoutCallback(
                [In, MarshalAs(UnmanagedType.Interface)] IGameInputDevice device,
                [In] IntPtr context,
                [In, MarshalAs(UnmanagedType.FunctionPtr)] GameInputKeyboardLayoutCallback callbackFunc,
                [Out] out ulong callbackToken);

            [PreserveSig]
            void StopCallback([In] ulong callbackToken);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool UnregisterCallback([In] ulong callbackToken);

            [PreserveSig]
            int CreateDispatcher([Out, MarshalAs(UnmanagedType.Interface)] out IGameInputDispatcher dispatcher);

            [PreserveSig]
            int FindDeviceFromId(
                [In] ref APP_LOCAL_DEVICE_ID value,
                [Out, MarshalAs(UnmanagedType.Interface)] out IGameInputDevice device);

            [PreserveSig]
            int FindDeviceFromPlatformString(
                [In, MarshalAs(UnmanagedType.LPWStr)] string value,
                [Out, MarshalAs(UnmanagedType.Interface)] out IGameInputDevice device);

            [PreserveSig]
            void SetFocusPolicy([In] GameInputFocusPolicy policy);

            [PreserveSig]
            int CreateAggregateDevice(
                [In] GameInputKind inputKind,
                [Out] out APP_LOCAL_DEVICE_ID deviceId);

            [PreserveSig]
            int DisableAggregateDevice([In] ref APP_LOCAL_DEVICE_ID deviceId);
        }

        [ComImport, Guid("05A42D89-2CB6-45A3-874D-E635723587AB"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        public interface IGameInputRawDeviceReport
        {
            [PreserveSig]
            void GetDevice([Out, MarshalAs(UnmanagedType.Interface)] out IGameInputDevice device);

            [PreserveSig]
            void GetReportInfo([Out] out GameInputRawDeviceReportInfo reportInfo);

            [PreserveSig]
            nuint GetRawDataSize();

            [PreserveSig]
            nuint GetRawData(
                [In] nuint bufferSize,
                [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] byte[] buffer);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool SetRawData(
                [In] nuint bufferSize,
                [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] byte[] buffer);
        }

        [ComImport, Guid("C81C4CDE-ED1A-4631-A30F-C556A6241A1F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        public interface IGameInputReading
        {
            [PreserveSig]
            GameInputKind GetInputKind();

            [PreserveSig]
            ulong GetTimestamp();

            [PreserveSig]
            void GetDevice([Out, MarshalAs(UnmanagedType.Interface)] out IGameInputDevice device);

            [PreserveSig]
            uint GetControllerAxisCount();

            [PreserveSig]
            uint GetControllerAxisState(
                [In] uint stateArrayCount,
                [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] float[] stateArray);

            [PreserveSig]
            uint GetControllerButtonCount();

            [PreserveSig]
            uint GetControllerButtonState(
                [In] uint stateArrayCount,
                [Out, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1, SizeParamIndex = 0)] bool[] stateArray);

            [PreserveSig]
            uint GetControllerSwitchCount();

            [PreserveSig]
            uint GetControllerSwitchState(
                [In] uint stateArrayCount,
                [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] GameInputSwitchPosition[] stateArray);

            [PreserveSig]
            uint GetKeyCount();

            [PreserveSig]
            uint GetKeyState(
                [In] uint stateArrayCount,
                [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] GameInputKeyState[] stateArray);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetMouseState([Out] out GameInputMouseState state);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetSensorsState([Out] out GameInputSensorsState state);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetArcadeStickState([Out] out GameInputArcadeStickState state);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetFlightStickState([Out] out GameInputFlightStickState state);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetGamepadState([Out] out GameInputGamepadState state);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetRacingWheelState([Out] out GameInputRacingWheelState state);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetRawReport([Out, MarshalAs(UnmanagedType.Interface)] out IGameInputRawDeviceReport report);
        }

        [ComImport, Guid("63E2F38B-A399-4275-8AE7-D4C6E524D12A"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        public interface IGameInputDevice
        {
            [PreserveSig]
            int GetDeviceInfo([Out] out IntPtr info);

            [PreserveSig]
            int GetHapticInfo([Out] out GameInputHapticInfo info);

            [PreserveSig]
            GameInputDeviceStatus GetDeviceStatus();

            [PreserveSig]
            int CreateForceFeedbackEffect(
                [In] uint motorIndex,
                [In] ref GameInputForceFeedbackParams parameters,
                [Out, MarshalAs(UnmanagedType.Interface)] out IGameInputForceFeedbackEffect effect);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool IsForceFeedbackMotorPoweredOn([In] uint motorIndex);

            [PreserveSig]
            void SetForceFeedbackMotorGain(
                [In] uint motorIndex,
                [In] float masterGain);

            [PreserveSig]
            void SetRumbleState([In] ref GameInputRumbleParams parameters);

            [PreserveSig]
            int DirectInputEscape(
                [In] uint command,
                [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] bufferIn,
                [In] uint bufferInSize,
                [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 4)] byte[] bufferOut,
                [In] uint bufferOutSize,
                [Out] out uint bufferOutSizeWritten);

            [PreserveSig]
            int CreateInputMapper([Out, MarshalAs(UnmanagedType.Interface)] out IGameInputMapper inputMapper);

            [PreserveSig]
            int GetExtraAxisCount(
                [In] GameInputKind inputKind,
                [Out] out uint extraAxisCount);

            [PreserveSig]
            int GetExtraButtonCount(
                [In] GameInputKind inputKind,
                [Out] out uint extraButtonCount);

            [PreserveSig]
            int GetExtraAxisIndexes(
                [In] GameInputKind inputKind,
                [In] uint extraAxisCount,
                [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] byte[] extraAxisIndexes);

            [PreserveSig]
            int GetExtraButtonIndexes(
                [In] GameInputKind inputKind,
                [In] uint extraButtonCount,
                [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] byte[] extraButtonIndexes);

            [PreserveSig]
            int CreateRawDeviceReport(
                [In] uint reportId,
                [In] GameInputRawDeviceReportKind reportKind,
                [Out, MarshalAs(UnmanagedType.Interface)] out IGameInputRawDeviceReport report);

            [PreserveSig]
            int SendRawDeviceOutput([In, MarshalAs(UnmanagedType.Interface)] IGameInputRawDeviceReport report);
        }

        [ComImport, Guid("415EED2E-98CB-42C2-8F28-B94601074E31"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        public interface IGameInputDispatcher
        {
            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool Dispatch([In] ulong quotaInMicroseconds);

            [PreserveSig]
            int OpenWaitHandle([Out] out Win32.SafeHandles.SafeWaitHandle waitHandle);
        }

        [ComImport, Guid("FF61096A-3373-4093-A1DF-6D31846B3511"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        public interface IGameInputForceFeedbackEffect
        {
            [PreserveSig]
            void GetDevice([Out, MarshalAs(UnmanagedType.Interface)] out IGameInputDevice device);

            [PreserveSig]
            uint GetMotorIndex();

            [PreserveSig]
            float GetGain();

            [PreserveSig]
            void SetGain([In] float gain);

            [PreserveSig]
            void GetParams([Out] out GameInputForceFeedbackParams parameters);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool SetParams([In] ref GameInputForceFeedbackParams parameters);

            [PreserveSig]
            GameInputFeedbackEffectState GetState();

            [PreserveSig]
            void SetState([In] GameInputFeedbackEffectState state);
        }

        [ComImport, Guid("3C600700-F16C-49CE-9BE6-6A2EF752ED5E"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        public interface IGameInputMapper
        {
            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetArcadeStickButtonMappingInfo(
                [In] GameInputArcadeStickButtons buttonElement,
                [Out] out GameInputButtonMapping mapping);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetFlightStickAxisMappingInfo(
                [In] GameInputFlightStickAxes axisElement,
                [Out] out GameInputAxisMapping mapping);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetFlightStickButtonMappingInfo(
                [In] GameInputFlightStickButtons buttonElement,
                [Out] out GameInputButtonMapping mapping);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetGamepadAxisMappingInfo(
                [In] GameInputGamepadAxes axisElement,
                [Out] out GameInputAxisMapping mapping);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetGamepadButtonMappingInfo(
                [In] GameInputGamepadButtons buttonElement,
                [Out] out GameInputButtonMapping mapping);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetRacingWheelAxisMappingInfo(
                [In] GameInputRacingWheelAxes axisElement,
                [Out] out GameInputAxisMapping mapping);

            [PreserveSig]
            [return: MarshalAs(UnmanagedType.I1)]
            bool GetRacingWheelButtonMappingInfo(
                [In] GameInputRacingWheelButtons buttonElement,
                [Out] out GameInputButtonMapping mapping);
        }

        // DLL Imports
        [DllImport("GameInput.dll", ExactSpelling = true)]
        public static extern int GameInputCreate(out IGameInput gameInput);

        public const int GAMEINPUT_FACILITY = 0x38A;

        public const int GAMEINPUT_E_DEVICE_DISCONNECTED                   = unchecked((int)0x838A0001);
        public const int GAMEINPUT_E_DEVICE_NOT_FOUND                      = unchecked((int)0x838A0002);
        public const int GAMEINPUT_E_READING_NOT_FOUND                     = unchecked((int)0x838A0003);
        public const int GAMEINPUT_E_REFERENCE_READING_TOO_OLD             = unchecked((int)0x838A0004);
        public const int GAMEINPUT_E_TIMESTAMP_OUT_OF_RANGE                = unchecked((int)0x838A0005);
        public const int GAMEINPUT_E_INSUFFICIENT_FORCE_FEEDBACK_RESOURCES = unchecked((int)0x838A0006);
        public const int GAMEINPUT_E_FEEDBACK_NOT_SUPPORTED                = unchecked((int)0x838A0007);
        public const int GAMEINPUT_E_OBJECT_NO_LONGER_EXISTS               = unchecked((int)0x838A0008);
        public const int GAMEINPUT_E_CALLBACK_NOT_FOUND                    = unchecked((int)0x838A0009);
        public const int GAMEINPUT_E_HAPTIC_INFO_NOT_FOUND                 = unchecked((int)0x838A000A);
        public const int GAMEINPUT_E_AGGREGATE_OPERATION_NOT_SUPPORTED     = unchecked((int)0x838A000B);
        public const int GAMEINPUT_E_INPUT_KIND_NOT_PRESENT                = unchecked((int)0x838A000C);
    }
}
