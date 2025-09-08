// Copyright (c) Microsoft Corporation.  All rights reserved.

#pragma once

#ifndef __cplusplus
#error GameInput requires C++
#endif

#include <stdint.h>
#include <unknwn.h>

#pragma region Application Family or OneCore Family or Games Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)

#define GAMEINPUT_API_VERSION 1

namespace GameInput { namespace v1 {

enum GameInputKind
{
    GameInputKindUnknown          = 0x00000000,
    GameInputKindControllerAxis   = 0x00000002,
    GameInputKindControllerButton = 0x00000004,
    GameInputKindControllerSwitch = 0x00000008,
    GameInputKindController       = 0x0000000E,
    GameInputKindKeyboard         = 0x00000010,
    GameInputKindMouse            = 0x00000020,
    GameInputKindArcadeStick      = 0x00010000,
    GameInputKindFlightStick      = 0x00020000,
    GameInputKindGamepad          = 0x00040000,
    GameInputKindRacingWheel      = 0x00080000,
    GameInputKindUiNavigation     = 0x01000000
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputKind);

enum GameInputEnumerationKind
{
    GameInputNoEnumeration       = 0,
    GameInputAsyncEnumeration    = 1,
    GameInputBlockingEnumeration = 2
};

enum GameInputFocusPolicy
{
    GameInputDefaultFocusPolicy             = 0x00000000,
    GameInputDisableBackgroundInput         = 0x00000001,
    GameInputExclusiveForegroundInput       = 0x00000002,
    GameInputDisableBackgroundGuideButton   = 0x00000004,
    GameInputExclusiveForegroundGuideButton = 0x00000008,
    GameInputDisableBackgroundShareButton   = 0x00000010,
    GameInputExclusiveForegroundShareButton = 0x00000020
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputFocusPolicy);

enum GameInputSwitchKind
{
    GameInputUnknownSwitchKind = -1,
    GameInput2WaySwitch        =  0,
    GameInput4WaySwitch        =  1,
    GameInput8WaySwitch        =  2
};

enum GameInputSwitchPosition
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
};

enum GameInputKeyboardKind
{
    GameInputUnknownKeyboard = -1,
    GameInputAnsiKeyboard    =  0,
    GameInputIsoKeyboard     =  1,
    GameInputKsKeyboard      =  2,
    GameInputAbntKeyboard    =  3,
    GameInputJisKeyboard     =  4
};

enum GameInputMouseButtons
{
    GameInputMouseNone           = 0x00000000,
    GameInputMouseLeftButton     = 0x00000001,
    GameInputMouseRightButton    = 0x00000002,
    GameInputMouseMiddleButton   = 0x00000004,
    GameInputMouseButton4        = 0x00000008,
    GameInputMouseButton5        = 0x00000010,
    GameInputMouseWheelTiltLeft  = 0x00000020,
    GameInputMouseWheelTiltRight = 0x00000040
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputMouseButtons)

enum GameInputMousePositions
{
    GameInputMouseNoPosition       = 0x00000000,
    GameInputMouseAbsolutePosition = 0x00000001,
    GameInputMouseRelativePosition = 0x00000002
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputMousePositions)

enum GameInputArcadeStickButtons
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
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputArcadeStickButtons)

enum GameInputFlightStickButtons
{
    GameInputFlightStickNone          = 0x00000000,
    GameInputFlightStickMenu          = 0x00000001,
    GameInputFlightStickView          = 0x00000002,
    GameInputFlightStickFirePrimary   = 0x00000004,
    GameInputFlightStickFireSecondary = 0x00000008
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputFlightStickButtons)

enum GameInputGamepadButtons
{
    GameInputGamepadNone            = 0x00000000,
    GameInputGamepadMenu            = 0x00000001,
    GameInputGamepadView            = 0x00000002,
    GameInputGamepadA               = 0x00000004,
    GameInputGamepadB               = 0x00000008,
    GameInputGamepadX               = 0x00000010,
    GameInputGamepadY               = 0x00000020,
    GameInputGamepadDPadUp          = 0x00000040,
    GameInputGamepadDPadDown        = 0x00000080,
    GameInputGamepadDPadLeft        = 0x00000100,
    GameInputGamepadDPadRight       = 0x00000200,
    GameInputGamepadLeftShoulder    = 0x00000400,
    GameInputGamepadRightShoulder   = 0x00000800,
    GameInputGamepadLeftThumbstick  = 0x00001000,
    GameInputGamepadRightThumbstick = 0x00002000
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputGamepadButtons)

enum GameInputRacingWheelButtons
{
    GameInputRacingWheelNone         = 0x00000000,
    GameInputRacingWheelMenu         = 0x00000001,
    GameInputRacingWheelView         = 0x00000002,
    GameInputRacingWheelPreviousGear = 0x00000004,
    GameInputRacingWheelNextGear     = 0x00000008,
    GameInputRacingWheelDpadUp       = 0x00000010,
    GameInputRacingWheelDpadDown     = 0x00000020,
    GameInputRacingWheelDpadLeft     = 0x00000040,
    GameInputRacingWheelDpadRight    = 0x00000080
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputRacingWheelButtons)

enum GameInputUiNavigationButtons
{
    GameInputUiNavigationNone        = 0x00000000,
    GameInputUiNavigationMenu        = 0x00000001,
    GameInputUiNavigationView        = 0x00000002,
    GameInputUiNavigationAccept      = 0x00000004,
    GameInputUiNavigationCancel      = 0x00000008,
    GameInputUiNavigationUp          = 0x00000010,
    GameInputUiNavigationDown        = 0x00000020,
    GameInputUiNavigationLeft        = 0x00000040,
    GameInputUiNavigationRight       = 0x00000080,
    GameInputUiNavigationContext1    = 0x00000100,
    GameInputUiNavigationContext2    = 0x00000200,
    GameInputUiNavigationContext3    = 0x00000400,
    GameInputUiNavigationContext4    = 0x00000800,
    GameInputUiNavigationPageUp      = 0x00001000,
    GameInputUiNavigationPageDown    = 0x00002000,
    GameInputUiNavigationPageLeft    = 0x00004000,
    GameInputUiNavigationPageRight   = 0x00008000,
    GameInputUiNavigationScrollUp    = 0x00010000,
    GameInputUiNavigationScrollDown  = 0x00020000,
    GameInputUiNavigationScrollLeft  = 0x00040000,
    GameInputUiNavigationScrollRight = 0x00080000
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputUiNavigationButtons)

enum GameInputSystemButtons
{
    GameInputSystemButtonNone  = 0x00000000,
    GameInputSystemButtonGuide = 0x00000001,
    GameInputSystemButtonShare = 0x00000002
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputSystemButtons)

enum GameInputDeviceStatus
{
    GameInputDeviceNoStatus  = 0x00000000,
    GameInputDeviceConnected = 0x00000001,
    GameInputDeviceAnyStatus = 0xFFFFFFFF
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputDeviceStatus);

enum GameInputDeviceFamily
{
    GameInputFamilyVirtual   = -1,
    GameInputFamilyAggregate =  0,
    GameInputFamilyXboxOne   =  1,
    GameInputFamilyXbox360   =  2,
    GameInputFamilyHid       =  3,
    GameInputFamilyI8042     =  4
};

enum GameInputLabel
{
    GameInputLabelUnknown                  =  -1,
    GameInputLabelNone                     =   0,
    GameInputLabelXboxGuide                =   1,
    GameInputLabelXboxBack                 =   2,
    GameInputLabelXboxStart                =   3,
    GameInputLabelXboxMenu                 =   4,
    GameInputLabelXboxView                 =   5,
    GameInputLabelXboxA                    =   7,
    GameInputLabelXboxB                    =   8,
    GameInputLabelXboxX                    =   9,
    GameInputLabelXboxY                    =  10,
    GameInputLabelXboxDPadUp               =  11,
    GameInputLabelXboxDPadDown             =  12,
    GameInputLabelXboxDPadLeft             =  13,
    GameInputLabelXboxDPadRight            =  14,
    GameInputLabelXboxLeftShoulder         =  15,
    GameInputLabelXboxLeftTrigger          =  16,
    GameInputLabelXboxLeftStickButton      =  17,
    GameInputLabelXboxRightShoulder        =  18,
    GameInputLabelXboxRightTrigger         =  19,
    GameInputLabelXboxRightStickButton     =  20,
    GameInputLabelXboxPaddle1              =  21,
    GameInputLabelXboxPaddle2              =  22,
    GameInputLabelXboxPaddle3              =  23,
    GameInputLabelXboxPaddle4              =  24,
    GameInputLabelLetterA                  =  25,
    GameInputLabelLetterB                  =  26,
    GameInputLabelLetterC                  =  27,
    GameInputLabelLetterD                  =  28,
    GameInputLabelLetterE                  =  29,
    GameInputLabelLetterF                  =  30,
    GameInputLabelLetterG                  =  31,
    GameInputLabelLetterH                  =  32,
    GameInputLabelLetterI                  =  33,
    GameInputLabelLetterJ                  =  34,
    GameInputLabelLetterK                  =  35,
    GameInputLabelLetterL                  =  36,
    GameInputLabelLetterM                  =  37,
    GameInputLabelLetterN                  =  38,
    GameInputLabelLetterO                  =  39,
    GameInputLabelLetterP                  =  40,
    GameInputLabelLetterQ                  =  41,
    GameInputLabelLetterR                  =  42,
    GameInputLabelLetterS                  =  43,
    GameInputLabelLetterT                  =  44,
    GameInputLabelLetterU                  =  45,
    GameInputLabelLetterV                  =  46,
    GameInputLabelLetterW                  =  47,
    GameInputLabelLetterX                  =  48,
    GameInputLabelLetterY                  =  49,
    GameInputLabelLetterZ                  =  50,
    GameInputLabelNumber0                  =  51,
    GameInputLabelNumber1                  =  52,
    GameInputLabelNumber2                  =  53,
    GameInputLabelNumber3                  =  54,
    GameInputLabelNumber4                  =  55,
    GameInputLabelNumber5                  =  56,
    GameInputLabelNumber6                  =  57,
    GameInputLabelNumber7                  =  58,
    GameInputLabelNumber8                  =  59,
    GameInputLabelNumber9                  =  60,
    GameInputLabelArrowUp                  =  61,
    GameInputLabelArrowUpRight             =  62,
    GameInputLabelArrowRight               =  63,
    GameInputLabelArrowDownRight           =  64,
    GameInputLabelArrowDown                =  65,
    GameInputLabelArrowDownLLeft           =  66,
    GameInputLabelArrowLeft                =  67,
    GameInputLabelArrowUpLeft              =  68,
    GameInputLabelArrowUpDown              =  69,
    GameInputLabelArrowLeftRight           =  70,
    GameInputLabelArrowUpDownLeftRight     =  71,
    GameInputLabelArrowClockwise           =  72,
    GameInputLabelArrowCounterClockwise    =  73,
    GameInputLabelArrowReturn              =  74,
    GameInputLabelIconBranding             =  75,
    GameInputLabelIconHome                 =  76,
    GameInputLabelIconMenu                 =  77,
    GameInputLabelIconCross                =  78,
    GameInputLabelIconCircle               =  79,
    GameInputLabelIconSquare               =  80,
    GameInputLabelIconTriangle             =  81,
    GameInputLabelIconStar                 =  82,
    GameInputLabelIconDPadUp               =  83,
    GameInputLabelIconDPadDown             =  84,
    GameInputLabelIconDPadLeft             =  85,
    GameInputLabelIconDPadRight            =  86,
    GameInputLabelIconDialClockwise        =  87,
    GameInputLabelIconDialCounterClockwise =  88,
    GameInputLabelIconSliderLeftRight      =  89,
    GameInputLabelIconSliderUpDown         =  90,
    GameInputLabelIconWheelUpDown          =  91,
    GameInputLabelIconPlus                 =  92,
    GameInputLabelIconMinus                =  93,
    GameInputLabelIconSuspension           =  94,
    GameInputLabelHome                     =  95,
    GameInputLabelGuide                    =  96,
    GameInputLabelMode                     =  97,
    GameInputLabelSelect                   =  98,
    GameInputLabelMenu                     =  99,
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
    GameInputLabelP1                       = 121,
    GameInputLabelP2                       = 122,
    GameInputLabelP3                       = 123,
    GameInputLabelP4                       = 124
};

enum GameInputFeedbackAxes
{
    GameInputFeedbackAxisNone     = 0x00000000,
    GameInputFeedbackAxisLinearX  = 0x00000001,
    GameInputFeedbackAxisLinearY  = 0x00000002,
    GameInputFeedbackAxisLinearZ  = 0x00000004,
    GameInputFeedbackAxisAngularX = 0x00000008,
    GameInputFeedbackAxisAngularY = 0x00000010,
    GameInputFeedbackAxisAngularZ = 0x00000020,
    GameInputFeedbackAxisNormal   = 0x00000040
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputFeedbackAxes)

enum GameInputFeedbackEffectState
{
    GameInputFeedbackStopped = 0,
    GameInputFeedbackRunning = 1,
    GameInputFeedbackPaused  = 2
};

enum GameInputForceFeedbackEffectKind
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
};

enum GameInputRumbleMotors
{
    GameInputRumbleNone          = 0x00000000,
    GameInputRumbleLowFrequency  = 0x00000001,
    GameInputRumbleHighFrequency = 0x00000002,
    GameInputRumbleLeftTrigger   = 0x00000004,
    GameInputRumbleRightTrigger  = 0x00000008
};

DEFINE_ENUM_FLAG_OPERATORS(GameInputRumbleMotors)

interface IGameInput;
interface IGameInputReading;
interface IGameInputDevice;
interface IGameInputDispatcher;
interface IGameInputForceFeedbackEffect;

typedef uint64_t GameInputCallbackToken;

typedef void (CALLBACK* GameInputReadingCallback)(
    _In_ GameInputCallbackToken callbackToken,
    _In_ void* context,
    _In_ IGameInputReading* reading);

typedef void (CALLBACK* GameInputDeviceCallback)(
    _In_ GameInputCallbackToken callbackToken,
    _In_ void* context,
    _In_ IGameInputDevice* device,
    _In_ uint64_t timestamp,
    _In_ GameInputDeviceStatus currentStatus,
    _In_ GameInputDeviceStatus previousStatus);

typedef void (CALLBACK* GameInputSystemButtonCallback)(
    _In_ GameInputCallbackToken callbackToken,
    _In_ void* context,
    _In_ IGameInputDevice* device,
    _In_ uint64_t timestamp,
    _In_ GameInputSystemButtons currentButtons,
    _In_ GameInputSystemButtons previousButtons);

typedef void (CALLBACK* GameInputKeyboardLayoutCallback)(
    _In_ GameInputCallbackToken callbackToken,
    _In_ void* context,
    _In_ IGameInputDevice* device,
    _In_ uint64_t timestamp,
    _In_ uint32_t currentLayout,
    _In_ uint32_t previousLayout);

struct GameInputKeyState
{
    uint32_t scanCode;
    uint32_t codePoint;
    uint8_t  virtualKey;
    bool     isDeadKey;
};

struct GameInputMouseState
{
    GameInputMouseButtons   buttons;
    GameInputMousePositions positions;
    int64_t                 positionX;
    int64_t                 positionY;
    int64_t                 absolutePositionX;
    int64_t                 absolutePositionY;
    int64_t                 wheelX;
    int64_t                 wheelY;
};

struct GameInputArcadeStickState
{
    GameInputArcadeStickButtons buttons;
};

struct GameInputFlightStickState
{
    GameInputFlightStickButtons buttons;
    GameInputSwitchPosition     hatSwitch;
    float                       roll;
    float                       pitch;
    float                       yaw;
    float                       throttle;
};

struct GameInputGamepadState
{
    GameInputGamepadButtons buttons;
    float                   leftTrigger;
    float                   rightTrigger;
    float                   leftThumbstickX;
    float                   leftThumbstickY;
    float                   rightThumbstickX;
    float                   rightThumbstickY;
};

struct GameInputRacingWheelState
{
    GameInputRacingWheelButtons buttons;
    int32_t                     patternShifterGear;
    float                       wheel;
    float                       throttle;
    float                       brake;
    float                       clutch;
    float                       handbrake;
};

struct GameInputUiNavigationState
{
    GameInputUiNavigationButtons buttons;
};

struct GameInputUsage
{
    uint16_t page;
    uint16_t id;
};

struct GameInputControllerAxisInfo
{
    GameInputKind  mappedInputKinds;
    GameInputLabel label;
};

struct GameInputControllerButtonInfo
{
    GameInputKind  mappedInputKinds;
    GameInputLabel label;
};

struct GameInputControllerSwitchInfo
{
    GameInputKind       mappedInputKinds;
    GameInputLabel      label;
    GameInputSwitchKind kind;
};

struct GameInputKeyboardInfo
{
    GameInputKeyboardKind kind;
    uint32_t              layout;
    uint32_t              keyCount;
    uint32_t              functionKeyCount;
    uint32_t              maxSimultaneousKeys;
    uint32_t              platformType;
    uint32_t              platformSubtype;
};

struct GameInputMouseInfo
{
    GameInputMouseButtons supportedButtons;
    uint32_t              sampleRate;
    bool                  hasWheelX;
    bool                  hasWheelY;
};

struct GameInputArcadeStickInfo
{
    GameInputLabel menuButtonLabel;
    GameInputLabel viewButtonLabel;
    GameInputLabel stickUpLabel;
    GameInputLabel stickDownLabel;
    GameInputLabel stickLeftLabel;
    GameInputLabel stickRightLabel;
    GameInputLabel actionButton1Label;
    GameInputLabel actionButton2Label;
    GameInputLabel actionButton3Label;
    GameInputLabel actionButton4Label;
    GameInputLabel actionButton5Label;
    GameInputLabel actionButton6Label;
    GameInputLabel specialButton1Label;
    GameInputLabel specialButton2Label;
};

struct GameInputFlightStickInfo
{
    GameInputLabel      menuButtonLabel;
    GameInputLabel      viewButtonLabel;
    GameInputLabel      firePrimaryButtonLabel;
    GameInputLabel      fireSecondaryButtonLabel;
    GameInputSwitchKind hatSwitchKind;
};

struct GameInputGamepadInfo
{
    GameInputLabel menuButtonLabel;
    GameInputLabel viewButtonLabel;
    GameInputLabel aButtonLabel;
    GameInputLabel bButtonLabel;
    GameInputLabel xButtonLabel;
    GameInputLabel yButtonLabel;
    GameInputLabel dpadUpLabel;
    GameInputLabel dpadDownLabel;
    GameInputLabel dpadLeftLabel;
    GameInputLabel dpadRightLabel;
    GameInputLabel leftShoulderButtonLabel;
    GameInputLabel rightShoulderButtonLabel;
    GameInputLabel leftThumbstickButtonLabel;
    GameInputLabel rightThumbstickButtonLabel;
};

struct GameInputRacingWheelInfo
{
    GameInputLabel menuButtonLabel;
    GameInputLabel viewButtonLabel;
    GameInputLabel previousGearButtonLabel;
    GameInputLabel nextGearButtonLabel;
    GameInputLabel dpadUpLabel;
    GameInputLabel dpadDownLabel;
    GameInputLabel dpadLeftLabel;
    GameInputLabel dpadRightLabel;
    bool           hasClutch;
    bool           hasHandbrake;
    bool           hasPatternShifter;
    int32_t        minPatternShifterGear;
    int32_t        maxPatternShifterGear;
    float          maxWheelAngle;
};

struct GameInputUiNavigationInfo
{
    GameInputLabel menuButtonLabel;
    GameInputLabel viewButtonLabel;
    GameInputLabel acceptButtonLabel;
    GameInputLabel cancelButtonLabel;
    GameInputLabel upButtonLabel;
    GameInputLabel downButtonLabel;
    GameInputLabel leftButtonLabel;
    GameInputLabel rightButtonLabel;
    GameInputLabel contextButton1Label;
    GameInputLabel contextButton2Label;
    GameInputLabel contextButton3Label;
    GameInputLabel contextButton4Label;
    GameInputLabel pageUpButtonLabel;
    GameInputLabel pageDownButtonLabel;
    GameInputLabel pageLeftButtonLabel;
    GameInputLabel pageRightButtonLabel;
    GameInputLabel scrollUpButtonLabel;
    GameInputLabel scrollDownButtonLabel;
    GameInputLabel scrollLeftButtonLabel;
    GameInputLabel scrollRightButtonLabel;
    GameInputLabel guideButtonLabel;
};

struct GameInputForceFeedbackMotorInfo
{
    GameInputFeedbackAxes supportedAxes;
    bool                  isConstantEffectSupported;
    bool                  isRampEffectSupported;
    bool                  isSineWaveEffectSupported;
    bool                  isSquareWaveEffectSupported;
    bool                  isTriangleWaveEffectSupported;
    bool                  isSawtoothUpWaveEffectSupported;
    bool                  isSawtoothDownWaveEffectSupported;
    bool                  isSpringEffectSupported;
    bool                  isFrictionEffectSupported;
    bool                  isDamperEffectSupported;
    bool                  isInertiaEffectSupported;
};

struct GameInputDeviceInfo
{
    uint16_t               vendorId;
    uint16_t               productId;
    GameInputUsage         usage;
    APP_LOCAL_DEVICE_ID    deviceId;
    APP_LOCAL_DEVICE_ID    deviceRootId;
    GameInputDeviceFamily  deviceFamily;
    GameInputKind          supportedInput;
    GameInputRumbleMotors  supportedRumbleMotors;
    GameInputSystemButtons supportedSystemButtons;
    const char*            displayName;
    const char*            pnpPath;

    _Field_size_full_opt_(1) const GameInputKeyboardInfo*     keyboardInfo;
    _Field_size_full_opt_(1) const GameInputMouseInfo*        mouseInfo;
    _Field_size_full_opt_(1) const GameInputArcadeStickInfo*  arcadeStickInfo;
    _Field_size_full_opt_(1) const GameInputFlightStickInfo*  flightStickInfo;
    _Field_size_full_opt_(1) const GameInputGamepadInfo*      gamepadInfo;
    _Field_size_full_opt_(1) const GameInputRacingWheelInfo*  racingWheelInfo;
    _Field_size_full_opt_(1) const GameInputUiNavigationInfo* uiNavigationInfo;

    uint32_t controllerAxisCount;
    _Field_size_full_(controllerAxisCount) const GameInputControllerAxisInfo* controllerAxisInfo;

    uint32_t controllerButtonCount;
    _Field_size_full_(controllerButtonCount) const GameInputControllerButtonInfo* controllerButtonInfo;

    uint32_t controllerSwitchCount;
    _Field_size_full_(controllerSwitchCount) const GameInputControllerSwitchInfo* controllerSwitchInfo;

    uint32_t forceFeedbackMotorCount;
    _Field_size_full_(forceFeedbackMotorCount) const GameInputForceFeedbackMotorInfo* forceFeedbackMotorInfo;
};

struct GameInputForceFeedbackEnvelope
{
    uint64_t attackDuration;
    uint64_t sustainDuration;
    uint64_t releaseDuration;
    float    attackGain;
    float    sustainGain;
    float    releaseGain;
    uint32_t playCount;
    uint64_t repeatDelay;
};

struct GameInputForceFeedbackMagnitude
{
    float linearX;
    float linearY;
    float linearZ;
    float angularX;
    float angularY;
    float angularZ;
    float normal;
};

struct GameInputForceFeedbackConditionParams
{
    GameInputForceFeedbackMagnitude magnitude;
    float                           positiveCoefficient;
    float                           negativeCoefficient;
    float                           maxPositiveMagnitude;
    float                           maxNegativeMagnitude;
    float                           deadZone;
    float                           bias;
};

struct GameInputForceFeedbackConstantParams
{
    GameInputForceFeedbackEnvelope  envelope;
    GameInputForceFeedbackMagnitude magnitude;
};

struct GameInputForceFeedbackPeriodicParams
{
    GameInputForceFeedbackEnvelope  envelope;
    GameInputForceFeedbackMagnitude magnitude;
    float                           frequency;
    float                           phase;
    float                           bias;
};

struct GameInputForceFeedbackRampParams
{
    GameInputForceFeedbackEnvelope  envelope;
    GameInputForceFeedbackMagnitude startMagnitude;
    GameInputForceFeedbackMagnitude endMagnitude;
};

struct GameInputForceFeedbackParams
{
    GameInputForceFeedbackEffectKind kind;
    union
    {
        GameInputForceFeedbackConstantParams  constant;
        GameInputForceFeedbackRampParams      ramp;
        GameInputForceFeedbackPeriodicParams  sineWave;
        GameInputForceFeedbackPeriodicParams  squareWave;
        GameInputForceFeedbackPeriodicParams  triangleWave;
        GameInputForceFeedbackPeriodicParams  sawtoothUpWave;
        GameInputForceFeedbackPeriodicParams  sawtoothDownWave;
        GameInputForceFeedbackConditionParams spring;
        GameInputForceFeedbackConditionParams friction;
        GameInputForceFeedbackConditionParams damper;
        GameInputForceFeedbackConditionParams inertia;
    } data;
};

struct GameInputRumbleParams
{
    float lowFrequency;
    float highFrequency;
    float leftTrigger;
    float rightTrigger;
};

const IID IID_IGameInput = {0x40ffb7e4, 0x6150, 0x407a, 0xb4, 0x39, 0x13, 0x2b, 0xad, 0xc0, 0x8d, 0x2d};

DECLARE_INTERFACE_IID_(IGameInput, IUnknown, "40FFB7E4-6150-407A-B439-132BADC08D2D")
{
    IFACEMETHOD_(uint64_t, GetCurrentTimestamp)() PURE;

    _Must_inspect_result_ IFACEMETHOD(GetCurrentReading)(
        _In_ GameInputKind inputKind,
        _In_opt_ IGameInputDevice* device,
        _COM_Outptr_ IGameInputReading** reading) PURE;

    _Must_inspect_result_ IFACEMETHOD(GetNextReading)(
        _In_ IGameInputReading* referenceReading,
        _In_ GameInputKind inputKind,
        _In_opt_ IGameInputDevice* device,
        _COM_Outptr_ IGameInputReading** reading) PURE;

    _Must_inspect_result_ IFACEMETHOD(GetPreviousReading)(
        _In_ IGameInputReading* referenceReading,
        _In_ GameInputKind inputKind,
        _In_opt_ IGameInputDevice* device,
        _COM_Outptr_ IGameInputReading** reading) PURE;

    IFACEMETHOD(RegisterReadingCallback)(
        _In_opt_ IGameInputDevice* device,
        _In_ GameInputKind inputKind,
        _In_opt_ void* context,
        _In_ GameInputReadingCallback callbackFunc,
        _Out_opt_ _Result_zeroonfailure_ GameInputCallbackToken* callbackToken) PURE;

    IFACEMETHOD(RegisterDeviceCallback)(
        _In_opt_ IGameInputDevice* device,
        _In_ GameInputKind inputKind,
        _In_ GameInputDeviceStatus statusFilter,
        _In_ GameInputEnumerationKind enumerationKind,
        _In_opt_ void* context,
        _In_ GameInputDeviceCallback callbackFunc,
        _Out_opt_ _Result_zeroonfailure_ GameInputCallbackToken* callbackToken) PURE;

    IFACEMETHOD(RegisterSystemButtonCallback)(
        _In_opt_ IGameInputDevice* device,
        _In_ GameInputSystemButtons buttonFilter,
        _In_opt_ void* context,
        _In_ GameInputSystemButtonCallback callbackFunc,
        _Out_opt_ _Result_zeroonfailure_ GameInputCallbackToken* callbackToken) PURE;

    IFACEMETHOD(RegisterKeyboardLayoutCallback)(
        _In_opt_ IGameInputDevice* device,
        _In_opt_ void* context,
        _In_ GameInputKeyboardLayoutCallback callbackFunc,
        _Out_opt_ _Result_zeroonfailure_ GameInputCallbackToken* callbackToken) PURE;

    IFACEMETHOD_(void, StopCallback)(
        _In_ GameInputCallbackToken callbackToken) PURE;

    IFACEMETHOD_(bool, UnregisterCallback)(
        _In_ GameInputCallbackToken callbackToken) PURE;

    _Must_inspect_result_ IFACEMETHOD(CreateDispatcher)(
        _COM_Outptr_ IGameInputDispatcher** dispatcher) PURE;

    _Must_inspect_result_ IFACEMETHOD(FindDeviceFromId)(
        _In_ const APP_LOCAL_DEVICE_ID* value,
        _COM_Outptr_ IGameInputDevice** device) PURE;

    _Must_inspect_result_ IFACEMETHOD(FindDeviceFromPlatformString)(
        _In_ LPCWSTR value,
        _COM_Outptr_ IGameInputDevice** device) PURE;

    IFACEMETHOD_(void, SetFocusPolicy)(
        _In_ GameInputFocusPolicy policy) PURE;
};

DECLARE_INTERFACE_IID_(IGameInputReading, IUnknown, "86318E60-0B3C-40D6-BEFA-C62F2D952724")
{
    IFACEMETHOD_(GameInputKind, GetInputKind)() PURE;

    IFACEMETHOD_(uint64_t, GetTimestamp)() PURE;

    IFACEMETHOD_(void, GetDevice)(
        _Outptr_ IGameInputDevice** device) PURE;

    IFACEMETHOD_(uint32_t, GetControllerAxisCount)() PURE;

    IFACEMETHOD_(uint32_t, GetControllerAxisState)(
        _In_ uint32_t stateArrayCount,
        _Out_writes_(stateArrayCount) float* stateArray) PURE;

    IFACEMETHOD_(uint32_t, GetControllerButtonCount)() PURE;

    IFACEMETHOD_(uint32_t, GetControllerButtonState)(
        _In_ uint32_t stateArrayCount,
        _Out_writes_(stateArrayCount) bool* stateArray) PURE;

    IFACEMETHOD_(uint32_t, GetControllerSwitchCount)() PURE;

    IFACEMETHOD_(uint32_t, GetControllerSwitchState)(
        _In_ uint32_t stateArrayCount,
        _Out_writes_(stateArrayCount) GameInputSwitchPosition* stateArray) PURE;

    IFACEMETHOD_(uint32_t, GetKeyCount)() PURE;

    IFACEMETHOD_(uint32_t, GetKeyState)(
        _In_ uint32_t stateArrayCount,
        _Out_writes_(stateArrayCount) GameInputKeyState* stateArray) PURE;

    IFACEMETHOD_(bool, GetMouseState)(
        _Out_ GameInputMouseState* state) PURE;

    IFACEMETHOD_(bool, GetArcadeStickState)(
        _Out_ GameInputArcadeStickState* state) PURE;

    IFACEMETHOD_(bool, GetFlightStickState)(
        _Out_ GameInputFlightStickState* state) PURE;

    IFACEMETHOD_(bool, GetGamepadState)(
        _Out_ GameInputGamepadState* state) PURE;

    IFACEMETHOD_(bool, GetRacingWheelState)(
        _Out_ GameInputRacingWheelState* state) PURE;

    IFACEMETHOD_(bool, GetUiNavigationState)(
        _Out_ GameInputUiNavigationState* state) PURE;
};

DECLARE_INTERFACE_IID_(IGameInputDevice, IUnknown, "B169652A-4A32-40D7-9FA9-905997952516")
{
    IFACEMETHOD(GetDeviceInfo)(
        _Outptr_ const GameInputDeviceInfo** info) PURE;

    IFACEMETHOD_(GameInputDeviceStatus, GetDeviceStatus)() PURE;

    _Must_inspect_result_ IFACEMETHOD(CreateForceFeedbackEffect)(
        _In_ uint32_t motorIndex,
        _In_ const GameInputForceFeedbackParams* params,
        _COM_Outptr_ IGameInputForceFeedbackEffect** effect) PURE;

    IFACEMETHOD_(bool, IsForceFeedbackMotorPoweredOn)(
        _In_ uint32_t motorIndex) PURE;

    IFACEMETHOD_(void, SetForceFeedbackMotorGain)(
        _In_ uint32_t motorIndex,
        _In_ float masterGain) PURE;

    IFACEMETHOD_(void, SetRumbleState)(
        _In_opt_ const GameInputRumbleParams* params) PURE;
};

DECLARE_INTERFACE_IID_(IGameInputDispatcher, IUnknown, "415EED2E-98CB-42C2-8F28-B94601074E31")
{
    IFACEMETHOD_(bool, Dispatch)(
        _In_ uint64_t quotaInMicroseconds) PURE;

    IFACEMETHOD(OpenWaitHandle)(
        _Outptr_result_nullonfailure_ HANDLE* waitHandle) PURE;
};

DECLARE_INTERFACE_IID_(IGameInputForceFeedbackEffect, IUnknown, "51BDA05E-F742-45D9-B085-9444AE48381D")
{
    IFACEMETHOD_(void, GetDevice)(
        _Outptr_ IGameInputDevice** device) PURE;

    IFACEMETHOD_(uint32_t, GetMotorIndex)() PURE;

    IFACEMETHOD_(float, GetGain)() PURE;

    IFACEMETHOD_(void, SetGain)(
        _In_ float gain) PURE;

    IFACEMETHOD_(void, GetParams)(
        _Out_ GameInputForceFeedbackParams* params) PURE;

    IFACEMETHOD_(bool, SetParams)(
        _In_ const GameInputForceFeedbackParams* params) PURE;

    IFACEMETHOD_(GameInputFeedbackEffectState, GetState)() PURE;

    IFACEMETHOD_(void, SetState)(
        _In_ GameInputFeedbackEffectState state) PURE;
};

// Legacy compatibility, not intended for direct use.
_Must_inspect_result_ STDAPI GameInputCreate(
    _COM_Outptr_ class GameInputLegacy** legacy);

inline _Must_inspect_result_ HRESULT GameInputCreate(
    _COM_Outptr_ IGameInput** gameInput)
{
    *gameInput = nullptr;

    HRESULT hr = S_OK;
    IUnknown* unknown = nullptr;

    hr = GameInputCreate(reinterpret_cast<GameInputLegacy**>(&unknown));
    if (SUCCEEDED(hr))
    {
        hr = unknown->QueryInterface(IID_IGameInput, reinterpret_cast<void**>(gameInput));
        unknown->Release();
    }

    return hr;
}

const LONG GAMEINPUT_FACILITY = 0x38A;

const HRESULT GAMEINPUT_E_DEVICE_DISCONNECTED       = _HRESULT_TYPEDEF_(0x838A0001L);
const HRESULT GAMEINPUT_E_DEVICE_NOT_FOUND          = _HRESULT_TYPEDEF_(0x838A0002L);
const HRESULT GAMEINPUT_E_READING_NOT_FOUND         = _HRESULT_TYPEDEF_(0x838A0003L);
const HRESULT GAMEINPUT_E_REFERENCE_READING_TOO_OLD = _HRESULT_TYPEDEF_(0x838A0004L);
const HRESULT GAMEINPUT_E_FEEDBACK_NOT_SUPPORTED    = _HRESULT_TYPEDEF_(0x838A0007L);
const HRESULT GAMEINPUT_E_OBJECT_NO_LONGER_EXISTS   = _HRESULT_TYPEDEF_(0x838A0008L);
const HRESULT GAMEINPUT_E_CALLBACK_NOT_FOUND        = _HRESULT_TYPEDEF_(0x838A0009L);

}} // namespace GameInput::v1

#endif // #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
#pragma endregion // Application Family or OneCore Family or Games Family
