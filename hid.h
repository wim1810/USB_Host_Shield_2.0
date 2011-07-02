#if !defined(__HID_H__)
#define __HID_H__

#include <inttypes.h>
#include <avr/pgmspace.h>
#include "avrpins.h"
#include "max3421e.h"
#include "usbhost.h"
#include "usb_ch9.h"
#include "Usb.h"
#include <WProgram.h>

#include "printhex.h"
#include "hexdump.h"
#include "message.h"

#include "confdescparser.h"

#define DATA_SIZE_MASK           0x03
#define TYPE_MASK                0x0C
#define TAG_MASK                 0xF0

#define DATA_SIZE_0              0x00
#define DATA_SIZE_1              0x01
#define DATA_SIZE_2              0x02
#define DATA_SIZE_4              0x03

#define TYPE_MAIN                0x00
#define TYPE_GLOBAL              0x04
#define TYPE_LOCAL               0x08

#define TAG_MAIN_INPUT           0x80
#define TAG_MAIN_OUTPUT          0x90
#define TAG_MAIN_COLLECTION      0xA0
#define TAG_MAIN_FEATURE         0xB0
#define TAG_MAIN_ENDCOLLECTION   0xC0

#define TAG_GLOBAL_USAGEPAGE     0x00
#define TAG_GLOBAL_LOGICALMIN    0x10
#define TAG_GLOBAL_LOGICALMAX    0x20
#define TAG_GLOBAL_PHYSMIN       0x30
#define TAG_GLOBAL_PHYSMAX       0x40
#define TAG_GLOBAL_UNITEXP       0x50
#define TAG_GLOBAL_UNIT          0x60
#define TAG_GLOBAL_REPORTSIZE    0x70
#define TAG_GLOBAL_REPORTID      0x80
#define TAG_GLOBAL_REPORTCOUNT   0x90
#define TAG_GLOBAL_PUSH          0xA0
#define TAG_GLOBAL_POP           0xB0

#define TAG_LOCAL_USAGE          0x00
#define TAG_LOCAL_USAGEMIN       0x10
#define TAG_LOCAL_USAGEMAX       0x20

/* HID requests */
#define bmREQ_HIDOUT        USB_SETUP_HOST_TO_DEVICE|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE
#define bmREQ_HIDIN         USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_CLASS|USB_SETUP_RECIPIENT_INTERFACE 
#define bmREQ_HIDREPORT     USB_SETUP_DEVICE_TO_HOST|USB_SETUP_TYPE_STANDARD|USB_SETUP_RECIPIENT_INTERFACE

/* HID constants. Not part of chapter 9 */
/* Class-Specific Requests */
#define HID_REQUEST_GET_REPORT      0x01
#define HID_REQUEST_GET_IDLE        0x02
#define HID_REQUEST_GET_PROTOCOL    0x03
#define HID_REQUEST_SET_REPORT      0x09
#define HID_REQUEST_SET_IDLE        0x0A
#define HID_REQUEST_SET_PROTOCOL    0x0B

/* Class Descriptor Types */
#define HID_DESCRIPTOR_HID			0x21
#define HID_DESCRIPTOR_REPORT		0x22
#define HID_DESRIPTOR_PHY			0x23

/* Protocol Selection */
#define BOOT_PROTOCOL				0x00
#define RPT_PROTOCOL				0x01

/* HID Interface Class Code */
#define HID_INTF                    0x03

/* HID Interface Class SubClass Codes */
#define HID_BOOT_INTF_SUBCLASS      0x01

/* HID Interface Class Protocol Codes */
#define HID_PROTOCOL_NONE           0x00
#define HID_PROTOCOL_KEYBOARD       0x01
#define HID_PROTOCOL_MOUSE          0x02

///* HID descriptor */
//typedef struct 
//{
//    uint8_t		bLength;			
//	uint8_t		bDescriptorType;	
//	uint16_t	bcdHID;					// HID class specification release
//    uint8_t		bCountryCode;			
//	uint8_t		bNumDescriptors;		// Number of additional class specific descriptors
//	uint8_t		bDescrType;				// Type of class descriptor
//    uint16_t	wDescriptorLength;		// Total size of the Report descriptor
//} USB_HID_DESCRIPTOR;
//
//typedef struct
//{
//	uint8_t		bDescrType;				// Type of class descriptor
//    uint16_t	wDescriptorLength;		// Total size of the Report descriptor
//} HID_CLASS_DESCRIPTOR_LEN_AND_TYPE;

struct HidItemPrefix
{
	uint8_t		bSize : 2;
	uint8_t		bType : 2;
	uint8_t		bTag  : 4;
};

#define HID_ITEM_TYPE_MAIN							0
#define HID_ITEM_TYPE_GLOBAL						1
#define HID_ITEM_TYPE_LOCAL							2
#define HID_ITEM_TYPE_RESERVED						3

#define HID_LONG_ITEM_PREFIX						0xfe		// Long item prefix value

#define bmHID_MAIN_ITEM_TAG							0xfc		// Main item tag mask

#define bmHID_MAIN_ITEM_INPUT						0x80		// Main item Input tag value
#define bmHID_MAIN_ITEM_OUTPUT						0x90		// Main item Output tag value
#define bmHID_MAIN_ITEM_FEATURE						0xb0		// Main item Feature tag value
#define bmHID_MAIN_ITEM_COLLECTION					0xa0		// Main item Collection tag value
#define bmHID_MAIN_ITEM_END_COLLECTION				0xce		// Main item End Collection tag value

#define HID_MAIN_ITEM_COLLECTION_PHYSICAL			0
#define HID_MAIN_ITEM_COLLECTION_APPLICATION		1
#define HID_MAIN_ITEM_COLLECTION_LOGICAL			2
#define HID_MAIN_ITEM_COLLECTION_REPORT				3
#define HID_MAIN_ITEM_COLLECTION_NAMED_ARRAY		4
#define HID_MAIN_ITEM_COLLECTION_USAGE_SWITCH		5
#define HID_MAIN_ITEM_COLLECTION_USAGE_MODIFIER		6

struct MainItemIOFeature
{
	uint8_t		bmIsConstantOrData			: 1;
	uint8_t		bmIsArrayOrVariable			: 1;
	uint8_t		bmIsRelativeOrAbsolute		: 1;
	uint8_t		bmIsWrapOrNoWrap			: 1;
	uint8_t		bmIsNonLonearOrLinear		: 1;
	uint8_t		bmIsNoPreferedOrPrefered	: 1;
	uint8_t		bmIsNullOrNoNull			: 1;
	uint8_t		bmIsVolatileOrNonVolatile	: 1;
};

class ReportDescParser : public USBReadParser
{
public:
	typedef void (*UsagePageFunc)(uint16_t usage);

	static void PrintGenericDesktopPageUsage(uint16_t usage);
	static void PrintSimulationControlsPageUsage(uint16_t usage);
	static void PrintVRControlsPageUsage(uint16_t usage);
	static void PrintSportsControlsPageUsage(uint16_t usage);
	static void PrintGameControlsPageUsage(uint16_t usage);
	static void PrintGenericDeviceControlsPageUsage(uint16_t usage);
	static void PrintLEDPageUsage(uint16_t usage);
	static void PrintTelephonyPageUsage(uint16_t usage);
	static void PrintConsumerPageUsage(uint16_t usage);
	static void PrintDigitizerPageUsage(uint16_t usage);
	static void PrintAlphanumDisplayPageUsage(uint16_t usage);
	static void PrintMedicalInstrumentPageUsage(uint16_t usage);

	static void PrintValue(uint8_t *p, uint8_t len);
	static void PrintByteValue(uint8_t data);

private:
	static UsagePageFunc	usagePageFunctions[];

    MultiValueBuffer		theBuffer;
	MultiByteValueParser	valParser;
	ByteSkipper				theSkipper;
	uint8_t					varBuffer[sizeof(USB_CONFIGURATION_DESCRIPTOR)]; 

	uint8_t					itemParseState;		// Item parser state variable
	uint8_t					itemSize;			// Item size
	uint8_t					itemPrefix;			// Item prefix (first byte)
	uint8_t					rptSize;			// Report Size
	uint8_t					rptCount;			// Report Count

	uint8_t					totalSize;			// Report size in bytes

	uint8_t ParseItem(uint8_t **pp, uint16_t *pcntdn);

	UsagePageFunc			pfUsage;

	static void PrintUsagePage(uint16_t page);
	void SetUsagePage(uint16_t page);

public:
	ReportDescParser() :
		itemParseState(0),
		itemSize(0),		
		itemPrefix(0),
		rptSize(0),
		rptCount(0),
		pfUsage(NULL)
		{
			theBuffer.pValue = varBuffer; 
			valParser.Initialize(&theBuffer);
			theSkipper.Initialize(&theBuffer);
		};

	virtual void Parse(const uint16_t len, const uint8_t *pbuf, const uint16_t &offset);

	enum 
	{
		enErrorSuccess = 0
		, enErrorIncomplete					// value or record is partialy read in buffer
		, enErrorBufferTooSmall
	};
};


#define maxHidInterfaces		3		
#define maxEpPerInterface		2		
#define totalEndpoints			(maxHidInterfaces * maxEpPerInterface + 1)		

#define HID_MAX_HID_CLASS_DESCRIPTORS		5

class HID : public USBDeviceConfig, public UsbConfigXtracter
{
	// HID class specific descriptor type and length info obtained from HID descriptor
	HID_CLASS_DESCRIPTOR_LEN_AND_TYPE	descrInfo[HID_MAX_HID_CLASS_DESCRIPTORS];

	// Returns HID class specific descriptor length by its type and order number
	uint16_t GetHidClassDescrLen(uint8_t type, uint8_t num);

	static const uint8_t	epInterruptInIndex;		// InterruptIN  endpoint index
	static const uint8_t	epInterruptOutIndex;	// InterruptOUT endpoint index

	EpInfo		epInfo[totalEndpoints];

	struct HIDInterface
	{
		struct
		{
			uint8_t		bmInterface	: 3;
			uint8_t		bmAltSet	: 3;
			uint8_t		bmProtocol	: 2;
		};
		uint8_t			epIndex[maxEpPerInterface];
	};
	
	HIDInterface	hidInterfaces[maxHidInterfaces];

	USB			*pUsb;					// USB class instance pointer
	uint8_t		bAddress;				// address
	uint8_t		bConfNum;				// configuration number
	uint8_t		bNumIface;				// number of interfaces in the configuration
	uint8_t		bNumEP;					// total number of EP in the configuration
	uint32_t	qNextPollTime;			// next poll time
	bool		bPollEnable;			// poll enable flag

	void Initialize();
	HIDInterface* FindInterface(uint8_t iface, uint8_t alt, uint8_t proto);

	void PrintEndpointDescriptor(const USB_ENDPOINT_DESCRIPTOR* ep_ptr);
	void PrintHidDescriptor(const USB_HID_DESCRIPTOR *pDesc);

public:
	HID(USB *p);

    uint8_t setProto( uint8_t ep, uint8_t iface, uint8_t protocol );
    uint8_t getProto( uint8_t ep, uint8_t iface, uint8_t* dataptr );

    uint8_t getReportDescr( uint8_t ep, USBReadParser *parser = NULL);

    uint8_t getHidDescr( uint8_t ep, uint16_t nbytes, uint8_t* dataptr );
    //uint8_t getHidDescr( uint8_t ep, uint16_t nbytes, uint8_t iface, uint8_t report_type, uint8_t report_id, uint8_t* dataptr );
    uint8_t setReport( uint8_t ep, uint16_t nbytes, uint8_t iface, uint8_t report_type, uint8_t report_id, uint8_t* dataptr );
    uint8_t getReport( uint8_t ep, uint16_t nbytes, uint8_t iface, uint8_t report_type, uint8_t report_id, uint8_t* dataptr );
    uint8_t getIdle( uint8_t ep, uint8_t iface, uint8_t reportID, uint8_t* dataptr );
    uint8_t setIdle( uint8_t ep, uint8_t iface, uint8_t reportID, uint8_t duration );

	// USBDeviceConfig implementation
	virtual uint8_t Init(uint8_t parent, uint8_t port, bool lowspeed);
	virtual uint8_t Release();
	virtual uint8_t Poll();
	virtual uint8_t GetAddress() { return bAddress; };

	// UsbConfigXtracter implementation
	//virtual void ConfigXtract(const USB_CONFIGURATION_DESCRIPTOR *conf) {};
	//virtual void InterfaceXtract(uint8_t conf, const USB_INTERFACE_DESCRIPTOR *iface) {};
	virtual void EndpointXtract(uint8_t conf, uint8_t iface, uint8_t alt, uint8_t proto, const USB_ENDPOINT_DESCRIPTOR *ep);
};

const char pstrSpace						[] PROGMEM = " "; 
const char pstrCRLF							[] PROGMEM = "\r\n"; 
const char pstrSingleTab					[] PROGMEM = "\t"; 
const char pstrDoubleTab					[] PROGMEM = "\t\t"; 
const char pstrTripleTab					[] PROGMEM = "\t\t\t"; 

// Usage Page String Titles
const char pstrUsagePageUndefined				[] PROGMEM = "Undef"; 
const char pstrUsagePageGenericDesktopControls	[] PROGMEM = "Gen Desktop Ctrls"; 
const char pstrUsagePageSimulationControls		[] PROGMEM = "Simu Ctrls"; 
const char pstrUsagePageVRControls				[] PROGMEM = "VR Ctrls"; 
const char pstrUsagePageSportControls			[] PROGMEM = "Sport Ctrls"; 
const char pstrUsagePageGameControls			[] PROGMEM = "Game Ctrls"; 
const char pstrUsagePageGenericDeviceControls	[] PROGMEM = "Gen Dev Ctrls"; 
const char pstrUsagePageKeyboardKeypad			[] PROGMEM = "Kbrd/Keypad"; 
const char pstrUsagePageLEDs					[] PROGMEM = "LEDs"; 
const char pstrUsagePageButton					[] PROGMEM = "Button"; 
const char pstrUsagePageOrdinal					[] PROGMEM = "Ordinal"; 
const char pstrUsagePageTelephone				[] PROGMEM = "Tel"; 
const char pstrUsagePageConsumer				[] PROGMEM = "Consumer"; 
const char pstrUsagePageDigitizer				[] PROGMEM = "Digitizer"; 
const char pstrUsagePagePID						[] PROGMEM = "PID"; 
const char pstrUsagePageUnicode					[] PROGMEM = "Unicode"; 
const char pstrUsagePageAlphaNumericDisplay		[] PROGMEM = "Alpha Num Disp"; 
const char pstrUsagePageMedicalInstruments		[] PROGMEM = "Medical Instr"; 
const char pstrUsagePageMonitor					[] PROGMEM = "Monitor"; 
const char pstrUsagePagePower					[] PROGMEM = "Power"; 
const char pstrUsagePageBarCodeScanner			[] PROGMEM = "Bar Code Scan"; 
const char pstrUsagePageScale					[] PROGMEM = "Scale"; 
const char pstrUsagePageMSRDevices				[] PROGMEM = "Magn Stripe Read Dev"; 
const char pstrUsagePagePointOfSale				[] PROGMEM = "POS"; 
const char pstrUsagePageCameraControl			[] PROGMEM = "Cam Ctrl"; 
const char pstrUsagePageArcade					[] PROGMEM = "Arcade"; 
const char pstrUsagePageReserved				[] PROGMEM = "Reserved"; 
const char pstrUsagePageVendorDefined			[] PROGMEM = "Vendor Def"; 

// Generic Desktop Controls Page
const char pstrUsagePointer						[] PROGMEM = "Pointer"; 
const char pstrUsageMouse						[] PROGMEM = "Mouse"; 
const char pstrUsageJoystick					[] PROGMEM = "Joystick"; 
const char pstrUsageGamePad						[] PROGMEM = "Game Pad"; 
const char pstrUsageKeyboard					[] PROGMEM = "Kbrd"; 
const char pstrUsageKeypad						[] PROGMEM = "Keypad"; 
const char pstrUsageMultiAxisController			[] PROGMEM = "Multi-axis Ctrl"; 
const char pstrUsageTabletPCSystemControls		[] PROGMEM = "Tablet PC Sys Ctrls"; 
const char pstrUsageX							[] PROGMEM = "X"; 
const char pstrUsageY							[] PROGMEM = "Y"; 
const char pstrUsageZ							[] PROGMEM = "Z"; 
const char pstrUsageRx							[] PROGMEM = "Rx"; 
const char pstrUsageRy							[] PROGMEM = "Ry"; 
const char pstrUsageRz							[] PROGMEM = "Rz"; 
const char pstrUsageSlider						[] PROGMEM = "Slider"; 
const char pstrUsageDial						[] PROGMEM = "Dial"; 
const char pstrUsageWheel						[] PROGMEM = "Wheel"; 
const char pstrUsageHatSwitch					[] PROGMEM = "Hat Switch"; 
const char pstrUsageCountedBuffer				[] PROGMEM = "Counted Buf"; 
const char pstrUsageByteCount					[] PROGMEM = "Byte Count"; 
const char pstrUsageMotionWakeup				[] PROGMEM = "Motion Wakeup"; 
const char pstrUsageStart						[] PROGMEM = "Start"; 
const char pstrUsageSelect						[] PROGMEM = "Sel"; 
const char pstrUsageVx							[] PROGMEM = "Vx"; 
const char pstrUsageVy							[] PROGMEM = "Vy"; 
const char pstrUsageVz							[] PROGMEM = "Vz"; 
const char pstrUsageVbrx						[] PROGMEM = "Vbrx"; 
const char pstrUsageVbry						[] PROGMEM = "Vbry"; 
const char pstrUsageVbrz						[] PROGMEM = "Vbrz"; 
const char pstrUsageVno							[] PROGMEM = "Vno"; 
const char pstrUsageFeatureNotification			[] PROGMEM = "Feature Notif"; 
const char pstrUsageResolutionMultiplier		[] PROGMEM = "Res Mult"; 
const char pstrUsageSystemControl				[] PROGMEM = "Sys Ctrl"; 
const char pstrUsageSystemPowerDown				[] PROGMEM = "Sys Pwr Down"; 
const char pstrUsageSystemSleep					[] PROGMEM = "Sys Sleep"; 
const char pstrUsageSystemWakeup				[] PROGMEM = "Sys Wakeup"; 
const char pstrUsageSystemContextMenu			[] PROGMEM = "Sys Context Menu"; 
const char pstrUsageSystemMainMenu				[] PROGMEM = "Sys Main Menu"; 
const char pstrUsageSystemAppMenu				[] PROGMEM = "Sys App Menu"; 
const char pstrUsageSystemMenuHelp				[] PROGMEM = "Sys Menu Help"; 
const char pstrUsageSystemMenuExit				[] PROGMEM = "Sys Menu Exit"; 
const char pstrUsageSystemMenuSelect			[] PROGMEM = "Sys Menu Select"; 
const char pstrUsageSystemMenuRight				[] PROGMEM = "Sys Menu Right"; 
const char pstrUsageSystemMenuLeft				[] PROGMEM = "Sys Menu Left"; 
const char pstrUsageSystemMenuUp				[] PROGMEM = "Sys Menu Up"; 
const char pstrUsageSystemMenuDown				[] PROGMEM = "Sys Menu Down"; 
const char pstrUsageSystemColdRestart			[] PROGMEM = "Sys Cold Restart"; 
const char pstrUsageSystemWarmRestart			[] PROGMEM = "Sys Warm Restart"; 
const char pstrUsageDPadUp						[] PROGMEM = "D-pad Up"; 
const char pstrUsageDPadDown					[] PROGMEM = "D-pad Down"; 
const char pstrUsageDPadRight					[] PROGMEM = "D-pad Right"; 
const char pstrUsageDPadLeft					[] PROGMEM = "D-pad Left"; 
const char pstrUsageSystemDock					[] PROGMEM = "Sys Dock"; 
const char pstrUsageSystemUndock				[] PROGMEM = "Sys Undock"; 
const char pstrUsageSystemSetup					[] PROGMEM = "Sys Setup"; 
const char pstrUsageSystemBreak					[] PROGMEM = "Sys Break"; 
const char pstrUsageSystemDebuggerBreak			[] PROGMEM = "Sys Dbg Brk"; 
const char pstrUsageApplicationBreak			[] PROGMEM = "App Break"; 
const char pstrUsageApplicationDebuggerBreak	[] PROGMEM = "App Dbg Brk"; 
const char pstrUsageSystemSpeakerMute			[] PROGMEM = "Sys Spk Mute"; 
const char pstrUsageSystemHibernate				[] PROGMEM = "Sys Hiber"; 
const char pstrUsageSystemDisplayInvert			[] PROGMEM = "Sys Disp Inv"; 
const char pstrUsageSystemDisplayInternal		[] PROGMEM = "Sys Disp Int"; 
const char pstrUsageSystemDisplayExternal		[] PROGMEM = "Sys Disp Ext"; 
const char pstrUsageSystemDisplayBoth			[] PROGMEM = "Sys Disp Both"; 
const char pstrUsageSystemDisplayDual			[] PROGMEM = "Sys Disp Dual"; 
const char pstrUsageSystemDisplayToggleIntExt	[] PROGMEM = "Sys Disp Tgl Int/Ext"; 
const char pstrUsageSystemDisplaySwapPriSec		[] PROGMEM = "Sys Disp Swap Pri/Sec"; 
const char pstrUsageSystemDisplayLCDAutoscale	[] PROGMEM = "Sys Disp LCD Autoscale"; 

// Simulation Controls Page
const char pstrUsageFlightSimulationDevice		[] PROGMEM = "Flight Simu Dev"; 
const char pstrUsageAutomobileSimulationDevice	[] PROGMEM = "Auto Simu Dev"; 
const char pstrUsageTankSimulationDevice		[] PROGMEM = "Tank Simu Dev"; 
const char pstrUsageSpaceshipSimulationDevice	[] PROGMEM = "Space Simu Dev"; 
const char pstrUsageSubmarineSimulationDevice	[] PROGMEM = "Subm Simu Dev"; 
const char pstrUsageSailingSimulationDevice		[] PROGMEM = "Sail Simu Dev"; 
const char pstrUsageMotocicleSimulationDevice	[] PROGMEM = "Moto Simu Dev"; 
const char pstrUsageSportsSimulationDevice		[] PROGMEM = "Sport Simu Dev"; 
const char pstrUsageAirplaneSimulationDevice	[] PROGMEM = "Airp Simu Dev"; 
const char pstrUsageHelicopterSimulationDevice	[] PROGMEM = "Heli Simu Dev"; 
const char pstrUsageMagicCarpetSimulationDevice	[] PROGMEM = "Magic Carpet Simu Dev"; 
const char pstrUsageBicycleSimulationDevice		[] PROGMEM = "Bike Simu Dev"; 
const char pstrUsageFlightControlStick			[] PROGMEM = "Flight Ctrl Stick"; 
const char pstrUsageFlightStick					[] PROGMEM = "Flight Stick"; 
const char pstrUsageCyclicControl				[] PROGMEM = "Cyclic Ctrl"; 
const char pstrUsageCyclicTrim					[] PROGMEM = "Cyclic Trim"; 
const char pstrUsageFlightYoke					[] PROGMEM = "Flight Yoke"; 
const char pstrUsageTrackControl				[] PROGMEM = "Track Ctrl"; 
const char pstrUsageAileron						[] PROGMEM = "Aileron"; 
const char pstrUsageAileronTrim					[] PROGMEM = "Aileron Trim"; 
const char pstrUsageAntiTorqueControl			[] PROGMEM = "Anti-Torque Ctrl"; 
const char pstrUsageAutopilotEnable				[] PROGMEM = "Autopilot Enable"; 
const char pstrUsageChaffRelease				[] PROGMEM = "Chaff Release"; 
const char pstrUsageCollectiveControl			[] PROGMEM = "Collective Ctrl"; 
const char pstrUsageDiveBrake					[] PROGMEM = "Dive Brake"; 
const char pstrUsageElectronicCountermeasures	[] PROGMEM = "El Countermeasures"; 
const char pstrUsageElevator					[] PROGMEM = "Elevator"; 
const char pstrUsageElevatorTrim				[] PROGMEM = "Elevator Trim"; 
const char pstrUsageRudder						[] PROGMEM = "Rudder"; 
const char pstrUsageThrottle					[] PROGMEM = "Throttle"; 
const char pstrUsageFlightCommunications		[] PROGMEM = "Flight Comm"; 
const char pstrUsageFlareRelease				[] PROGMEM = "Flare Release"; 
const char pstrUsageLandingGear					[] PROGMEM = "Landing Gear"; 
const char pstrUsageToeBrake					[] PROGMEM = "Toe Brake"; 
const char pstrUsageTrigger						[] PROGMEM = "Trigger"; 
const char pstrUsageWeaponsArm					[] PROGMEM = "Weapons Arm"; 
const char pstrUsageWeaponsSelect				[] PROGMEM = "Weapons Sel"; 
const char pstrUsageWingFlaps					[] PROGMEM = "Wing Flaps"; 
const char pstrUsageAccelerator					[] PROGMEM = "Accel"; 
const char pstrUsageBrake						[] PROGMEM = "Brake"; 
const char pstrUsageClutch						[] PROGMEM = "Clutch"; 
const char pstrUsageShifter						[] PROGMEM = "Shifter"; 
const char pstrUsageSteering					[] PROGMEM = "Steering"; 
const char pstrUsageTurretDirection				[] PROGMEM = "Turret Dir"; 
const char pstrUsageBarrelElevation				[] PROGMEM = "Barrel Ele"; 
const char pstrUsageDivePlane					[] PROGMEM = "Dive Plane"; 
const char pstrUsageBallast						[] PROGMEM = "Ballast"; 
const char pstrUsageBicycleCrank				[] PROGMEM = "Bicycle Crank"; 
const char pstrUsageHandleBars					[] PROGMEM = "Handle Bars"; 
const char pstrUsageFrontBrake					[] PROGMEM = "Front Brake"; 
const char pstrUsageRearBrake					[] PROGMEM = "Rear Brake"; 

// VR Controls Page
const char pstrUsageBelt						[] PROGMEM = "Belt"; 
const char pstrUsageBodySuit					[] PROGMEM = "Body Suit"; 
const char pstrUsageFlexor						[] PROGMEM = "Flexor"; 
const char pstrUsageGlove						[] PROGMEM = "Glove"; 
const char pstrUsageHeadTracker					[] PROGMEM = "Head Track"; 
const char pstrUsageHeadMountedDisplay			[] PROGMEM = "Head Disp"; 
const char pstrUsageHandTracker					[] PROGMEM = "Hand Track"; 
const char pstrUsageOculometer					[] PROGMEM = "Oculometer"; 
const char pstrUsageVest						[] PROGMEM = "Vest"; 
const char pstrUsageAnimatronicDevice			[] PROGMEM = "Animat Dev"; 
const char pstrUsageStereoEnable				[] PROGMEM = "Stereo Enbl"; 
const char pstrUsageDisplayEnable				[] PROGMEM = "Display Enbl"; 

// Sport Controls Page
const char pstrUsageBaseballBat					[] PROGMEM = "Baseball Bat"; 
const char pstrUsageGolfClub					[] PROGMEM = "Golf Club"; 
const char pstrUsageRowingMachine				[] PROGMEM = "Rowing Mach"; 
const char pstrUsageTreadmill					[] PROGMEM = "Treadmill"; 
const char pstrUsageOar							[] PROGMEM = "Oar"; 
const char pstrUsageSlope						[] PROGMEM = "Slope"; 
const char pstrUsageRate						[] PROGMEM = "Rate"; 
const char pstrUsageStickSpeed					[] PROGMEM = "Stick Speed"; 
const char pstrUsageStickFaceAngle				[] PROGMEM = "Stick Face Ang"; 
const char pstrUsageStickHeelToe				[] PROGMEM = "Stick Heel/Toe"; 
const char pstrUsageStickFollowThough			[] PROGMEM = "Stick Flw Thru"; 
const char pstrUsageStickTempo					[] PROGMEM = "Stick Tempo"; 
const char pstrUsageStickType					[] PROGMEM = "Stick Type"; 
const char pstrUsageStickHeight					[] PROGMEM = "Stick Hght"; 
const char pstrUsagePutter						[] PROGMEM = "Putter"; 
const char pstrUsage1Iron						[] PROGMEM = "1 Iron"; 
const char pstrUsage2Iron						[] PROGMEM = "2 Iron"; 
const char pstrUsage3Iron						[] PROGMEM = "3 Iron"; 
const char pstrUsage4Iron						[] PROGMEM = "4 Iron"; 
const char pstrUsage5Iron						[] PROGMEM = "5 Iron"; 
const char pstrUsage6Iron						[] PROGMEM = "6 Iron"; 
const char pstrUsage7Iron						[] PROGMEM = "7 Iron"; 
const char pstrUsage8Iron						[] PROGMEM = "8 Iron"; 
const char pstrUsage9Iron						[] PROGMEM = "9 Iron"; 
const char pstrUsage10Iron						[] PROGMEM = "10 Iron"; 
const char pstrUsage11Iron						[] PROGMEM = "11 Iron"; 
const char pstrUsageSandWedge					[] PROGMEM = "Sand Wedge"; 
const char pstrUsageLoftWedge					[] PROGMEM = "Loft Wedge"; 
const char pstrUsagePowerWedge					[] PROGMEM = "Pwr Wedge"; 
const char pstrUsage1Wood						[] PROGMEM = "1 Wood"; 
const char pstrUsage3Wood						[] PROGMEM = "3 Wood"; 
const char pstrUsage5Wood						[] PROGMEM = "5 Wood"; 
const char pstrUsage7Wood						[] PROGMEM = "7 Wood"; 
const char pstrUsage9Wood						[] PROGMEM = "9 Wood"; 

// Game Controls Page
const char pstrUsage3DGameController			[] PROGMEM = "3D Game Ctrl"; 
const char pstrUsagePinballDevice				[] PROGMEM = "Pinball Dev"; 
const char pstrUsageGunDevice					[] PROGMEM = "Gun Dev"; 
const char pstrUsagePointOfView					[] PROGMEM = "POV"; 
const char pstrUsageTurnRightLeft				[] PROGMEM = "Turn Right Left"; 
const char pstrUsagePitchForwardBackward		[] PROGMEM = "Pitch Fwd/Back"; 
const char pstrUsageRollRightLeft				[] PROGMEM = "Roll Right/Left"; 
const char pstrUsageMoveRightLeft				[] PROGMEM = "Move Right/Left"; 
const char pstrUsageMoveForwardBackward			[] PROGMEM = "Move Fwd/Back"; 
const char pstrUsageMoveUpDown					[] PROGMEM = "Move Up/Down"; 
const char pstrUsageLeanRightLeft				[] PROGMEM = "Lean Right/Left"; 
const char pstrUsageLeanForwardBackward			[] PROGMEM = "Lean Fwd/Back"; 
const char pstrUsageHeightOfPOV					[] PROGMEM = "Height of POV"; 
const char pstrUsageFlipper						[] PROGMEM = "Flipper"; 
const char pstrUsageSecondaryFlipper			[] PROGMEM = "Second Flipper"; 
const char pstrUsageBump						[] PROGMEM = "Bump"; 
const char pstrUsageNewGame						[] PROGMEM = "New Game"; 
const char pstrUsageShootBall					[] PROGMEM = "Shoot Ball"; 
const char pstrUsagePlayer						[] PROGMEM = "Player"; 
const char pstrUsageGunBolt						[] PROGMEM = "Gun Bolt"; 
const char pstrUsageGunClip						[] PROGMEM = "Gun Clip"; 
const char pstrUsageGunSelector					[] PROGMEM = "Gun Sel"; 
const char pstrUsageGunSingleShot				[] PROGMEM = "Gun Sngl Shot"; 
const char pstrUsageGunBurst					[] PROGMEM = "Gun Burst"; 
const char pstrUsageGunAutomatic				[] PROGMEM = "Gun Auto"; 
const char pstrUsageGunSafety					[] PROGMEM = "Gun Safety"; 
const char pstrUsageGamepadFireJump				[] PROGMEM = "Gamepad Fire/Jump"; 
const char pstrUsageGamepadTrigger				[] PROGMEM = "Gamepad Trig"; 

// Generic Device Controls Page
const char pstrUsageBatteryStrength				[] PROGMEM = "Bat Strength"; 
const char pstrUsageWirelessChannel				[] PROGMEM = "Wireless Ch"; 
const char pstrUsageWirelessID					[] PROGMEM = "Wireless ID"; 
const char pstrUsageDiscoverWirelessControl		[] PROGMEM = "Discover Wireless Ctrl"; 
const char pstrUsageSecurityCodeCharEntered		[] PROGMEM = "Sec Code Char Entrd"; 
const char pstrUsageSecurityCodeCharErased		[] PROGMEM = "Sec Code Char Erased"; 
const char pstrUsageSecurityCodeCleared			[] PROGMEM = "Sec Code Cleared"; 

// LED Page 
const char pstrUsageNumLock						[] PROGMEM = "Num Lock"; 
const char pstrUsageCapsLock					[] PROGMEM = "Caps Lock"; 
const char pstrUsageScrollLock					[] PROGMEM = "Scroll Lock"; 
const char pstrUsageCompose						[] PROGMEM = "Compose"; 
const char pstrUsageKana						[] PROGMEM = "Kana"; 
const char pstrUsagePower						[] PROGMEM = "Pwr"; 
const char pstrUsageShift						[] PROGMEM = "Shift"; 
const char pstrUsageDoNotDisturb				[] PROGMEM = "DND"; 
const char pstrUsageMute						[] PROGMEM = "Mute"; 
const char pstrUsageToneEnable					[] PROGMEM = "Tone Enbl"; 
const char pstrUsageHighCutFilter				[] PROGMEM = "High Cut Fltr"; 
const char pstrUsageLowCutFilter				[] PROGMEM = "Low Cut Fltr"; 
const char pstrUsageEqualizerEnable				[] PROGMEM = "Eq Enbl"; 
const char pstrUsageSoundFieldOn				[] PROGMEM = "Sound Field On"; 
const char pstrUsageSurroundOn					[] PROGMEM = "Surround On"; 
const char pstrUsageRepeat						[] PROGMEM = "Repeat"; 
const char pstrUsageStereo						[] PROGMEM = "Stereo"; 
const char pstrUsageSamplingRateDetect			[] PROGMEM = "Smpl Rate Detect"; 
const char pstrUsageSpinning					[] PROGMEM = "Spinning"; 
const char pstrUsageCAV							[] PROGMEM = "CAV"; 
const char pstrUsageCLV							[] PROGMEM = "CLV"; 
const char pstrUsageRecordingFormatDetect		[] PROGMEM = "Rec Format Detect"; 
const char pstrUsageOffHook						[] PROGMEM = "Off Hook"; 
const char pstrUsageRing						[] PROGMEM = "Ring"; 
const char pstrUsageMessageWaiting				[] PROGMEM = "Msg Wait"; 
const char pstrUsageDataMode					[] PROGMEM = "Data Mode"; 
const char pstrUsageBatteryOperation			[] PROGMEM = "Bat Op"; 
const char pstrUsageBatteryOK					[] PROGMEM = "Bat OK"; 
const char pstrUsageBatteryLow					[] PROGMEM = "Bat Low"; 
const char pstrUsageSpeaker						[] PROGMEM = "Speaker"; 
const char pstrUsageHeadSet						[] PROGMEM = "Head Set"; 
const char pstrUsageHold						[] PROGMEM = "Hold"; 
const char pstrUsageMicrophone					[] PROGMEM = "Mic"; 
const char pstrUsageCoverage					[] PROGMEM = "Coverage"; 
const char pstrUsageNightMode					[] PROGMEM = "Night Mode"; 
const char pstrUsageSendCalls					[] PROGMEM = "Send Calls"; 
const char pstrUsageCallPickup					[] PROGMEM = "Call Pickup"; 
const char pstrUsageConference					[] PROGMEM = "Conf"; 
const char pstrUsageStandBy						[] PROGMEM = "Stand-by"; 
const char pstrUsageCameraOn					[] PROGMEM = "Cam On"; 
const char pstrUsageCameraOff					[] PROGMEM = "Cam Off"; 
const char pstrUsageOnLine						[] PROGMEM = "On-Line"; 
const char pstrUsageOffLine						[] PROGMEM = "Off-Line"; 
const char pstrUsageBusy						[] PROGMEM = "Busy"; 
const char pstrUsageReady						[] PROGMEM = "Ready"; 
const char pstrUsagePaperOut					[] PROGMEM = "Paper Out"; 
const char pstrUsagePaperJam					[] PROGMEM = "Paper Jam"; 
const char pstrUsageRemote						[] PROGMEM = "Remote"; 
const char pstrUsageForward						[] PROGMEM = "Fwd"; 
const char pstrUsageReverse						[] PROGMEM = "Rev"; 
const char pstrUsageStop						[] PROGMEM = "Stop"; 
const char pstrUsageRewind						[] PROGMEM = "Rewind"; 
const char pstrUsageFastForward					[] PROGMEM = "Fast Fwd"; 
const char pstrUsagePlay						[] PROGMEM = "Play"; 
const char pstrUsagePause						[] PROGMEM = "Pause"; 
const char pstrUsageRecord						[] PROGMEM = "Rec"; 
const char pstrUsageError						[] PROGMEM = "Error"; 
const char pstrUsageSelectedIndicator			[] PROGMEM = "Usage Sel Ind"; 
const char pstrUsageInUseIndicator				[] PROGMEM = "Usage In Use Ind"; 
const char pstrUsageMultiModeIndicator			[] PROGMEM = "Usage Multi Mode Ind"; 
const char pstrUsageIndicatorOn					[] PROGMEM = "Ind On"; 
const char pstrUsageIndicatorFlash				[] PROGMEM = "Ind Flash"; 
const char pstrUsageIndicatorSlowBlink			[] PROGMEM = "Ind Slow Blk"; 
const char pstrUsageIndicatorFastBlink			[] PROGMEM = "Ind Fast Blk"; 
const char pstrUsageIndicatorOff				[] PROGMEM = "Ind Off"; 
const char pstrUsageFlashOnTime					[] PROGMEM = "Flash On Time"; 
const char pstrUsageSlowBlinkOnTime				[] PROGMEM = "Slow Blk On Time"; 
const char pstrUsageSlowBlinkOffTime			[] PROGMEM = "Slow Blk Off Time"; 
const char pstrUsageFastBlinkOnTime				[] PROGMEM = "Fast Blk On Time"; 
const char pstrUsageFastBlinkOffTime			[] PROGMEM = "Fast Blk Off Time"; 
const char pstrUsageIndicatorColor				[] PROGMEM = "Usage Ind Color"; 
const char pstrUsageIndicatorRed				[] PROGMEM = "Ind Red"; 
const char pstrUsageIndicatorGreen				[] PROGMEM = "Ind Green"; 
const char pstrUsageIndicatorAmber				[] PROGMEM = "Ind Amber"; 
const char pstrUsageGenericIndicator			[] PROGMEM = "Gen Ind"; 
const char pstrUsageSystemSuspend				[] PROGMEM = "Sys Suspend"; 
const char pstrUsageExternalPowerConnected		[] PROGMEM = "Ext Pwr Conn"; 

// Telephony Usage Page
const char pstrUsagePhone						[] PROGMEM = "Phone"; 
const char pstrUsageAnsweringMachine			[] PROGMEM = "Answ Mach"; 
const char pstrUsageMessageControls				[] PROGMEM = "Msg Ctrls"; 
const char pstrUsageHandset						[] PROGMEM = "Handset"; 
const char pstrUsageHeadset						[] PROGMEM = "Headset"; 
const char pstrUsageTelephonyKeyPad				[] PROGMEM = "Tel Key Pad"; 
const char pstrUsageProgrammableButton			[] PROGMEM = "Prog Button"; 
const char pstrUsageHookSwitch					[] PROGMEM = "Hook Sw"; 
const char pstrUsageFlash						[] PROGMEM = "Flash"; 
const char pstrUsageFeature						[] PROGMEM = "Feature"; 
//const char pstrUsageHold						[] PROGMEM = "Hold"; 
const char pstrUsageRedial						[] PROGMEM = "Redial"; 
const char pstrUsageTransfer					[] PROGMEM = "Transfer"; 
const char pstrUsageDrop						[] PROGMEM = "Drop"; 
const char pstrUsagePark						[] PROGMEM = "Park"; 
const char pstrUsageForwardCalls				[] PROGMEM = "Fwd Calls"; 
const char pstrUsageAlternateFunction			[] PROGMEM = "Alt Func"; 
const char pstrUsageLine						[] PROGMEM = "Line"; 
const char pstrUsageSpeakerPhone				[] PROGMEM = "Spk Phone"; 
//const char pstrUsageConference					[] PROGMEM = "Conference"; 
const char pstrUsageRingEnable					[] PROGMEM = "Ring Enbl"; 
const char pstrUsageRingSelect					[] PROGMEM = "Ring Sel"; 
const char pstrUsagePhoneMute					[] PROGMEM = "Phone Mute"; 
const char pstrUsageCallerID					[] PROGMEM = "Caller ID"; 
const char pstrUsageSend						[] PROGMEM = "Send"; 
const char pstrUsageSpeedDial					[] PROGMEM = "Speed Dial"; 
const char pstrUsageStoreNumber					[] PROGMEM = "Store Num"; 
const char pstrUsageRecallNumber				[] PROGMEM = "Recall Num"; 
const char pstrUsagePhoneDirectory				[] PROGMEM = "Phone Dir"; 
const char pstrUsageVoiceMail					[] PROGMEM = "Voice Mail"; 
const char pstrUsageScreenCalls					[] PROGMEM = "Screen Calls"; 
//const char pstrUsageDoNotDisturb				[] PROGMEM = "Do Not Disturb"; 
const char pstrUsageMessage						[] PROGMEM = "Msg"; 
const char pstrUsageAnswerOnOff					[] PROGMEM = "Answer On/Off"; 
const char pstrUsageInsideDialTone				[] PROGMEM = "Inside Dial Tone"; 
const char pstrUsageOutsideDialTone				[] PROGMEM = "Outside Dial Tone"; 
const char pstrUsageInsideRingTone				[] PROGMEM = "Inside Ring Tone"; 
const char pstrUsageOutsideRingTone				[] PROGMEM = "Outside Ring Tone"; 
const char pstrUsagePriorityRingTone			[] PROGMEM = "Prior Ring Tone"; 
const char pstrUsageInsideRingback				[] PROGMEM = "Inside Ringback"; 
const char pstrUsagePriorityRingback			[] PROGMEM = "Priority Ringback"; 
const char pstrUsageLineBusyTone				[] PROGMEM = "Ln Busy Tone"; 
const char pstrUsageReorderTone					[] PROGMEM = "Reorder Tone"; 
const char pstrUsageCallWaitingTone				[] PROGMEM = "Call Wait Tone"; 
const char pstrUsageConfirmationTone1			[] PROGMEM = "Cnfrm Tone1"; 
const char pstrUsageConfirmationTone2			[] PROGMEM = "Cnfrm Tone2"; 
const char pstrUsageTonesOff					[] PROGMEM = "Tones Off"; 
const char pstrUsageOutsideRingback				[] PROGMEM = "Outside Ringback"; 
const char pstrUsageRinger						[] PROGMEM = "Ringer"; 
const char pstrUsagePhoneKey0					[] PROGMEM = "0"; 
const char pstrUsagePhoneKey1					[] PROGMEM = "1"; 
const char pstrUsagePhoneKey2					[] PROGMEM = "2"; 
const char pstrUsagePhoneKey3					[] PROGMEM = "3"; 
const char pstrUsagePhoneKey4					[] PROGMEM = "4"; 
const char pstrUsagePhoneKey5					[] PROGMEM = "5"; 
const char pstrUsagePhoneKey6					[] PROGMEM = "6"; 
const char pstrUsagePhoneKey7					[] PROGMEM = "7"; 
const char pstrUsagePhoneKey8					[] PROGMEM = "8"; 
const char pstrUsagePhoneKey9					[] PROGMEM = "9"; 
const char pstrUsagePhoneKeyStar				[] PROGMEM = "*"; 
const char pstrUsagePhoneKeyPound				[] PROGMEM = "#"; 
const char pstrUsagePhoneKeyA					[] PROGMEM = "A"; 
const char pstrUsagePhoneKeyB					[] PROGMEM = "B"; 
const char pstrUsagePhoneKeyC					[] PROGMEM = "C"; 
const char pstrUsagePhoneKeyD					[] PROGMEM = "D"; 

// Consumer Usage Page
const char pstrUsageConsumerControl				[] PROGMEM = "Consumer Ctrl"; 
const char pstrUsageNumericKeyPad				[] PROGMEM = "Num Key Pad"; 
//const char pstrUsageProgrammableButton			[] PROGMEM = "Prog Btn"; 
//const char pstrUsageMicrophone					[] PROGMEM = "Mic"; 
const char pstrUsageHeadphone					[] PROGMEM = "Headphone"; 
const char pstrUsageGraphicEqualizer			[] PROGMEM = "Graph Eq"; 
const char pstrUsagePlus10						[] PROGMEM = "+10"; 
const char pstrUsagePlus100						[] PROGMEM = "+100"; 
const char pstrUsageAMPM						[] PROGMEM = "AM/PM"; 
//const char pstrUsagePower						[] PROGMEM = "Pwr"; 
const char pstrUsageReset						[] PROGMEM = "Reset"; 
const char pstrUsageSleep						[] PROGMEM = "Sleep"; 
const char pstrUsageSleepAfter					[] PROGMEM = "Sleep After"; 
const char pstrUsageSleepMode					[] PROGMEM = "Sleep Mode"; 
const char pstrUsageIllumination				[] PROGMEM = "Illumin"; 
const char pstrUsageFunctionButtons				[] PROGMEM = "Func Btns"; 
const char pstrUsageMenu						[] PROGMEM = "Menu"; 
const char pstrUsageMenuPick					[] PROGMEM = "Menu Pick"; 
const char pstrUsageMenuUp						[] PROGMEM = "Menu Up"; 
const char pstrUsageMenuDown					[] PROGMEM = "Menu Down"; 
const char pstrUsageMenuLeft					[] PROGMEM = "Menu Left"; 
const char pstrUsageMenuRight					[] PROGMEM = "Menu Right"; 
const char pstrUsageMenuEscape					[] PROGMEM = "Menu Esc"; 
const char pstrUsageMenuValueIncrease			[] PROGMEM = "Menu Val Inc"; 
const char pstrUsageMenuValueDecrease			[] PROGMEM = "Menu Val Dec"; 
const char pstrUsageDataOnScreen				[] PROGMEM = "Data On Scr"; 
const char pstrUsageClosedCaption				[] PROGMEM = "Closed Cptn"; 
const char pstrUsageClosedCaptionSelect			[] PROGMEM = "Closed Cptn Sel"; 
const char pstrUsageVCRTV						[] PROGMEM = "VCR/TV"; 
const char pstrUsageBroadcastMode				[] PROGMEM = "Brdcast Mode"; 
const char pstrUsageSnapshot					[] PROGMEM = "Snapshot"; 
const char pstrUsageStill						[] PROGMEM = "Still"; 
const char pstrUsageSelection					[] PROGMEM = "Sel"; 
const char pstrUsageAssignSelection				[] PROGMEM = "Assign Sel"; 
const char pstrUsageModeStep					[] PROGMEM = "Mode Step"; 
const char pstrUsageRecallLast					[] PROGMEM = "Recall Last"; 
const char pstrUsageEnterChannel				[] PROGMEM = "Entr Channel"; 
const char pstrUsageOrderMovie					[] PROGMEM = "Ord Movie"; 
const char pstrUsageChannel						[] PROGMEM = "Channel"; 
const char pstrUsageMediaSelection				[] PROGMEM = "Med Sel"; 
const char pstrUsageMediaSelectComputer			[] PROGMEM = "Med Sel Comp"; 
const char pstrUsageMediaSelectTV				[] PROGMEM = "Med Sel TV"; 
const char pstrUsageMediaSelectWWW				[] PROGMEM = "Med Sel WWW"; 
const char pstrUsageMediaSelectDVD				[] PROGMEM = "Med Sel DVD"; 
const char pstrUsageMediaSelectTelephone		[] PROGMEM = "Med Sel Tel"; 
const char pstrUsageMediaSelectProgramGuide		[] PROGMEM = "Med Sel PG"; 
const char pstrUsageMediaSelectVideoPhone		[] PROGMEM = "Med Sel Vid"; 
const char pstrUsageMediaSelectGames			[] PROGMEM = "Med Sel Games"; 
const char pstrUsageMediaSelectMessages			[] PROGMEM = "Med Sel Msg"; 
const char pstrUsageMediaSelectCD				[] PROGMEM = "Med Sel CD"; 
const char pstrUsageMediaSelectVCR				[] PROGMEM = "Med Sel VCR"; 
const char pstrUsageMediaSelectTuner			[] PROGMEM = "Med Sel Tuner"; 
const char pstrUsageQuit						[] PROGMEM = "Quit"; 
const char pstrUsageHelp						[] PROGMEM = "Help"; 
const char pstrUsageMediaSelectTape				[] PROGMEM = "Med Sel Tape"; 
const char pstrUsageMediaSelectCable			[] PROGMEM = "Med Sel Cbl"; 
const char pstrUsageMediaSelectSatellite		[] PROGMEM = "Med Sel Sat"; 
const char pstrUsageMediaSelectSecurity			[] PROGMEM = "Med Sel Secur"; 
const char pstrUsageMediaSelectHome				[] PROGMEM = "Med Sel Home"; 
const char pstrUsageMediaSelectCall				[] PROGMEM = "Med Sel Call"; 
const char pstrUsageChannelIncrement			[] PROGMEM = "Ch Inc"; 
const char pstrUsageChannelDecrement			[] PROGMEM = "Ch Dec"; 
const char pstrUsageMediaSelectSAP				[] PROGMEM = "Med Sel SAP"; 
const char pstrUsageVCRPlus						[] PROGMEM = "VCR+"; 
const char pstrUsageOnce						[] PROGMEM = "Once"; 
const char pstrUsageDaily						[] PROGMEM = "Daily"; 
const char pstrUsageWeekly						[] PROGMEM = "Weekly"; 
const char pstrUsageMonthly						[] PROGMEM = "Monthly"; 
//const char pstrUsagePlay						[] PROGMEM = "Play"; 
//const char pstrUsagePause						[] PROGMEM = "Pause"; 
//const char pstrUsageRecord						[] PROGMEM = "Rec"; 
//const char pstrUsageFastForward					[] PROGMEM = "FF"; 
//const char pstrUsageRewind						[] PROGMEM = "Rewind"; 
const char pstrUsageScanNextTrack				[] PROGMEM = "Next Track"; 
const char pstrUsageScanPreviousTrack			[] PROGMEM = "Prev Track"; 
//const char pstrUsageStop						[] PROGMEM = "Stop"; 
const char pstrUsageEject						[] PROGMEM = "Eject"; 
const char pstrUsageRandomPlay					[] PROGMEM = "Random"; 
const char pstrUsageSelectDisk					[] PROGMEM = "Sel Disk"; 
const char pstrUsageEnterDisk					[] PROGMEM = "Ent Disk"; 
//const char pstrUsageRepeat						[] PROGMEM = "Repeat"; 
const char pstrUsageTracking					[] PROGMEM = "Tracking"; 
const char pstrUsageTrackNormal					[] PROGMEM = "Trk Norm"; 
const char pstrUsageSlowTracking				[] PROGMEM = "Slow Trk"; 
const char pstrUsageFrameForward				[] PROGMEM = "Frm Fwd"; 
const char pstrUsageFrameBackwards				[] PROGMEM = "Frm Back"; 
const char pstrUsageMark						[] PROGMEM = "Mark"; 
const char pstrUsageClearMark					[] PROGMEM = "Clr Mark"; 
const char pstrUsageRepeatFromMark				[] PROGMEM = "Rpt Mark"; 
const char pstrUsageReturnToMark				[] PROGMEM = "Ret to Mark"; 
const char pstrUsageSearchMarkForward			[] PROGMEM = "Search Mark Fwd"; 
const char pstrUsageSearchMarkBackwards			[] PROGMEM = "Search Mark Back"; 
const char pstrUsageCounterReset				[] PROGMEM = "Counter Reset"; 
const char pstrUsageShowCounter					[] PROGMEM = "Show Counter"; 
const char pstrUsageTrackingIncrement			[] PROGMEM = "Track Inc"; 
const char pstrUsageTrackingDecrement			[] PROGMEM = "Track Dec"; 
const char pstrUsageStopEject					[] PROGMEM = "Stop/Eject"; 
const char pstrUsagePlayPause					[] PROGMEM = "Play/Pause"; 
const char pstrUsagePlaySkip					[] PROGMEM = "Play/Skip"; 
const char pstrUsageVolume						[] PROGMEM = "Vol"; 
const char pstrUsageBalance						[] PROGMEM = "Balance"; 
//const char pstrUsageMute						[] PROGMEM = "Mute"; 
const char pstrUsageBass						[] PROGMEM = "Bass"; 
const char pstrUsageTreble						[] PROGMEM = "Treble"; 
const char pstrUsageBassBoost					[] PROGMEM = "Bass Boost"; 
const char pstrUsageSurroundMode				[] PROGMEM = "Surround"; 
const char pstrUsageLoudness					[] PROGMEM = "Loud"; 
const char pstrUsageMPX							[] PROGMEM = "MPX"; 
const char pstrUsageVolumeIncrement				[] PROGMEM = "Vol Inc"; 
const char pstrUsageVolumeDecrement				[] PROGMEM = "Vol Dec"; 
const char pstrUsageSpeedSelect					[] PROGMEM = "Speed"; 
const char pstrUsagePlaybackSpeed				[] PROGMEM = "Play Speed"; 
const char pstrUsageStandardPlay				[] PROGMEM = "Std Play"; 
const char pstrUsageLongPlay					[] PROGMEM = "Long Play"; 
const char pstrUsageExtendedPlay				[] PROGMEM = "Ext Play"; 
const char pstrUsageSlow						[] PROGMEM = "Slow"; 
const char pstrUsageFanEnable					[] PROGMEM = "Fan Enbl"; 
const char pstrUsageFanSpeed					[] PROGMEM = "Fan Speed"; 
const char pstrUsageLightEnable					[] PROGMEM = "Light Enbl"; 
const char pstrUsageLightIlluminationLevel		[] PROGMEM = "Light Illum Lev"; 
const char pstrUsageClimateControlEnable		[] PROGMEM = "Climate Enbl"; 
const char pstrUsageRoomTemperature				[] PROGMEM = "Room Temp"; 
const char pstrUsageSecurityEnable				[] PROGMEM = "Secur Enbl"; 
const char pstrUsageFireAlarm					[] PROGMEM = "Fire Alm"; 
const char pstrUsagePoliceAlarm					[] PROGMEM = "Police Alm"; 
const char pstrUsageProximity					[] PROGMEM = "Prox"; 
const char pstrUsageMotion						[] PROGMEM = "Motion"; 
const char pstrUsageDuresAlarm					[] PROGMEM = "Dures Alm"; 
const char pstrUsageHoldupAlarm					[] PROGMEM = "Holdup Alm"; 
const char pstrUsageMedicalAlarm				[] PROGMEM = "Med Alm"; 
const char pstrUsageBalanceRight				[] PROGMEM = "Balance Right"; 
const char pstrUsageBalanceLeft					[] PROGMEM = "Balance Left"; 
const char pstrUsageBassIncrement				[] PROGMEM = "Bass Inc"; 
const char pstrUsageBassDecrement				[] PROGMEM = "Bass Dec"; 
const char pstrUsageTrebleIncrement				[] PROGMEM = "Treble Inc"; 
const char pstrUsageTrebleDecrement				[] PROGMEM = "Treble Dec"; 
const char pstrUsageSpeakerSystem				[] PROGMEM = "Spk Sys"; 
const char pstrUsageChannelLeft					[] PROGMEM = "Ch Left"; 
const char pstrUsageChannelRight				[] PROGMEM = "Ch Right"; 
const char pstrUsageChannelCenter				[] PROGMEM = "Ch Center"; 
const char pstrUsageChannelFront				[] PROGMEM = "Ch Front"; 
const char pstrUsageChannelCenterFront			[] PROGMEM = "Ch Cntr Front"; 
const char pstrUsageChannelSide					[] PROGMEM = "Ch Side"; 
const char pstrUsageChannelSurround				[] PROGMEM = "Ch Surround"; 
const char pstrUsageChannelLowFreqEnhancement	[] PROGMEM = "Ch Low Freq Enh"; 
const char pstrUsageChannelTop					[] PROGMEM = "Ch Top"; 
const char pstrUsageChannelUnknown				[] PROGMEM = "Ch Unk"; 
const char pstrUsageSubChannel					[] PROGMEM = "Sub-ch"; 
const char pstrUsageSubChannelIncrement			[] PROGMEM = "Sub-ch Inc"; 
const char pstrUsageSubChannelDecrement			[] PROGMEM = "Sub-ch Dec"; 
const char pstrUsageAlternateAudioIncrement		[] PROGMEM = "Alt Aud Inc"; 
const char pstrUsageAlternateAudioDecrement		[] PROGMEM = "Alt Aud Dec"; 
const char pstrUsageApplicationLaunchButtons	[] PROGMEM = "App Launch Btns"; 
const char pstrUsageALLaunchButtonConfigTool	[] PROGMEM = "AL Launch Conf Tl"; 
const char pstrUsageALProgrammableButton		[] PROGMEM = "AL Pgm Btn"; 
const char pstrUsageALConsumerControlConfig		[] PROGMEM = "AL Cons Ctrl Cfg"; 
const char pstrUsageALWordProcessor				[] PROGMEM = "AL Word Proc"; 
const char pstrUsageALTextEditor				[] PROGMEM = "AL Txt Edtr"; 
const char pstrUsageALSpreadsheet				[] PROGMEM = "AL Sprdsheet"; 
const char pstrUsageALGraphicsEditor			[] PROGMEM = "AL Graph Edtr"; 
const char pstrUsageALPresentationApp			[] PROGMEM = "AL Present App"; 
const char pstrUsageALDatabaseApp				[] PROGMEM = "AL DB App"; 
const char pstrUsageALEmailReader				[] PROGMEM = "AL E-mail Rdr"; 
const char pstrUsageALNewsreader				[] PROGMEM = "AL Newsrdr"; 
const char pstrUsageALVoicemail					[] PROGMEM = "AL Voicemail"; 
const char pstrUsageALContactsAddressBook		[] PROGMEM = "AL Addr Book"; 
const char pstrUsageALCalendarSchedule			[] PROGMEM = "AL Clndr/Schdlr"; 
const char pstrUsageALTaskProjectManager		[] PROGMEM = "AL Task/Prj Mgr"; 
const char pstrUsageALLogJournalTimecard		[] PROGMEM = "AL Log/Jrnl/Tmcrd"; 
const char pstrUsageALCheckbookFinance			[] PROGMEM = "AL Chckbook/Fin"; 
const char pstrUsageALCalculator				[] PROGMEM = "AL Calc"; 
const char pstrUsageALAVCapturePlayback			[] PROGMEM = "AL A/V Capt/Play"; 
const char pstrUsageALLocalMachineBrowser		[] PROGMEM = "AL Loc Mach Brow"; 
const char pstrUsageALLANWANBrow				[] PROGMEM = "AL LAN/WAN Brow"; 
const char pstrUsageALInternetBrowser			[] PROGMEM = "AL I-net Brow"; 
const char pstrUsageALRemoteNetISPConnect		[] PROGMEM = "AL Rem Net Con"; 
const char pstrUsageALNetworkConference			[] PROGMEM = "AL Net Conf"; 
const char pstrUsageALNetworkChat				[] PROGMEM = "AL Net Chat"; 
const char pstrUsageALTelephonyDialer			[] PROGMEM = "AL Tel/Dial"; 
const char pstrUsageALLogon						[] PROGMEM = "AL Logon"; 
const char pstrUsageALLogoff					[] PROGMEM = "AL Logoff"; 
const char pstrUsageALLogonLogoff				[] PROGMEM = "AL Logon/Logoff"; 
const char pstrUsageALTermLockScrSav			[] PROGMEM = "AL Term Lock/Scr Sav"; 
const char pstrUsageALControlPannel				[] PROGMEM = "AL Ctrl Pan"; 
const char pstrUsageALCommandLineProcessorRun	[] PROGMEM = "AL Cmd/Run"; 
const char pstrUsageALProcessTaskManager		[] PROGMEM = "AL Task Mgr"; 
const char pstrUsageALSelectTaskApplication		[] PROGMEM = "AL Sel App"; 
const char pstrUsageALNextTaskApplication		[] PROGMEM = "AL Next App"; 
const char pstrUsageALPreviousTaskApplication	[] PROGMEM = "AL Prev App"; 
const char pstrUsageALPreemptiveHaltTaskApp		[] PROGMEM = "AL Prmpt Halt App"; 
const char pstrUsageALIntegratedHelpCenter		[] PROGMEM = "AL Hlp Cntr"; 
const char pstrUsageALDocuments					[] PROGMEM = "AL Docs"; 
const char pstrUsageALThesaurus					[] PROGMEM = "AL Thsrs"; 
const char pstrUsageALDictionary				[] PROGMEM = "AL Dict"; 
const char pstrUsageALDesktop					[] PROGMEM = "AL Desktop"; 
const char pstrUsageALSpellCheck				[] PROGMEM = "AL Spell Chk"; 
const char pstrUsageALGrammarCheck				[] PROGMEM = "AL Gram Chk"; 
const char pstrUsageALWirelessStatus			[] PROGMEM = "AL Wireless Sts"; 
const char pstrUsageALKeyboardLayout			[] PROGMEM = "AL Kbd Layout"; 
const char pstrUsageALVirusProtection			[] PROGMEM = "AL Vir Protect"; 
const char pstrUsageALEncryption				[] PROGMEM = "AL Encrypt"; 
const char pstrUsageALScreenSaver				[] PROGMEM = "AL Scr Sav"; 
const char pstrUsageALAlarms					[] PROGMEM = "AL Alarms"; 
const char pstrUsageALClock						[] PROGMEM = "AL Clock"; 
const char pstrUsageALFileBrowser				[] PROGMEM = "AL File Brow"; 
const char pstrUsageALPowerStatus				[] PROGMEM = "AL Pwr Sts"; 
const char pstrUsageALImageBrowser				[] PROGMEM = "AL Img Brow"; 
const char pstrUsageALAudioBrowser				[] PROGMEM = "AL Aud Brow"; 
const char pstrUsageALMovieBrowser				[] PROGMEM = "AL Mov Brow"; 
const char pstrUsageALDigitalRightsManager		[] PROGMEM = "AL Dig Rights Mgr"; 
const char pstrUsageALDigitalWallet				[] PROGMEM = "AL Dig Wallet"; 
const char pstrUsageALInstantMessaging			[] PROGMEM = "AL Inst Msg"; 
const char pstrUsageALOEMFeaturesBrowser		[] PROGMEM = "AL OEM Tips Brow"; 
const char pstrUsageALOEMHelp					[] PROGMEM = "AL OEM Hlp"; 
const char pstrUsageALOnlineCommunity			[] PROGMEM = "AL Online Com"; 
const char pstrUsageALEntertainmentContentBrow	[] PROGMEM = "AL Ent Cont Brow"; 
const char pstrUsageALOnlineShoppingBrowser		[] PROGMEM = "AL Online Shop Brow"; 
const char pstrUsageALSmartCardInfoHelp			[] PROGMEM = "AL SmartCard Inf"; 
const char pstrUsageALMarketMonitorFinBrowser	[] PROGMEM = "AL Market Brow"; 
const char pstrUsageALCustomCorpNewsBrowser		[] PROGMEM = "AL Cust Corp News Brow"; 
const char pstrUsageALOnlineActivityBrowser		[] PROGMEM = "AL Online Act Brow"; 
const char pstrUsageALResearchSearchBrowser		[] PROGMEM = "AL Search Brow"; 
const char pstrUsageALAudioPlayer				[] PROGMEM = "AL Aud Player"; 
const char pstrUsageGenericGUIAppControls		[] PROGMEM = "Gen GUI App Ctrl"; 
const char pstrUsageACNew						[] PROGMEM = "AC New"; 
const char pstrUsageACOpen						[] PROGMEM = "AC Open"; 
const char pstrUsageACClose						[] PROGMEM = "AC Close"; 
const char pstrUsageACExit						[] PROGMEM = "AC Exit"; 
const char pstrUsageACMaximize					[] PROGMEM = "AC Max"; 
const char pstrUsageACMinimize					[] PROGMEM = "AC Min"; 
const char pstrUsageACSave						[] PROGMEM = "AC Save"; 
const char pstrUsageACPrint						[] PROGMEM = "AC Print"; 
const char pstrUsageACProperties				[] PROGMEM = "AC Prop"; 
const char pstrUsageACUndo						[] PROGMEM = "AC Undo"; 
const char pstrUsageACCopy						[] PROGMEM = "AC Copy"; 
const char pstrUsageACCut						[] PROGMEM = "AC Cut"; 
const char pstrUsageACPaste						[] PROGMEM = "AC Paste"; 
const char pstrUsageACSelectAll					[] PROGMEM = "AC Sel All"; 
const char pstrUsageACFind						[] PROGMEM = "AC Find"; 
const char pstrUsageACFindAndReplace			[] PROGMEM = "AC Find/Replace"; 
const char pstrUsageACSearch					[] PROGMEM = "AC Search"; 
const char pstrUsageACGoto						[] PROGMEM = "AC Goto"; 
const char pstrUsageACHome						[] PROGMEM = "AC Home"; 
const char pstrUsageACBack						[] PROGMEM = "AC Back"; 
const char pstrUsageACForward					[] PROGMEM = "AC Fwd"; 
const char pstrUsageACStop						[] PROGMEM = "AC Stop"; 
const char pstrUsageACRefresh					[] PROGMEM = "AC Refresh"; 
const char pstrUsageACPreviousLink				[] PROGMEM = "AC Prev Link"; 
const char pstrUsageACNextLink					[] PROGMEM = "AC Next Link"; 
const char pstrUsageACBookmarks					[] PROGMEM = "AC Bkmarks"; 
const char pstrUsageACHistory					[] PROGMEM = "AC Hist"; 
const char pstrUsageACSubscriptions				[] PROGMEM = "AC Subscr"; 
const char pstrUsageACZoomIn					[] PROGMEM = "AC Zoom In"; 
const char pstrUsageACZoomOut					[] PROGMEM = "AC Zoom Out"; 
const char pstrUsageACZoom						[] PROGMEM = "AC Zoom"; 
const char pstrUsageACFullScreenView			[] PROGMEM = "AC Full Scr"; 
const char pstrUsageACNormalView				[] PROGMEM = "AC Norm View"; 
const char pstrUsageACViewToggle				[] PROGMEM = "AC View Tgl"; 
const char pstrUsageACScrollUp					[] PROGMEM = "AC Scroll Up"; 
const char pstrUsageACScrollDown				[] PROGMEM = "AC Scroll Down"; 
const char pstrUsageACScroll					[] PROGMEM = "AC Scroll"; 
const char pstrUsageACPanLeft					[] PROGMEM = "AC Pan Left"; 
const char pstrUsageACPanRight					[] PROGMEM = "AC Pan Right"; 
const char pstrUsageACPan						[] PROGMEM = "AC Pan"; 
const char pstrUsageACNewWindow					[] PROGMEM = "AC New Wnd"; 
const char pstrUsageACTileHoriz					[] PROGMEM = "AC Tile Horiz"; 
const char pstrUsageACTileVert					[] PROGMEM = "AC Tile Vert"; 
const char pstrUsageACFormat					[] PROGMEM = "AC Frmt"; 
const char pstrUsageACEdit						[] PROGMEM = "AC Edit"; 
const char pstrUsageACBold						[] PROGMEM = "AC Bold"; 
const char pstrUsageACItalics					[] PROGMEM = "AC Ital"; 
const char pstrUsageACUnderline					[] PROGMEM = "AC Under"; 
const char pstrUsageACStrikethrough				[] PROGMEM = "AC Strike"; 
const char pstrUsageACSubscript					[] PROGMEM = "AC Sub"; 
const char pstrUsageACSuperscript				[] PROGMEM = "AC Super"; 
const char pstrUsageACAllCaps					[] PROGMEM = "AC All Caps"; 
const char pstrUsageACRotate					[] PROGMEM = "AC Rotate"; 
const char pstrUsageACResize					[] PROGMEM = "AC Resize"; 
const char pstrUsageACFlipHorizontal			[] PROGMEM = "AC Flp H"; 
const char pstrUsageACFlipVertical				[] PROGMEM = "AC Flp V"; 
const char pstrUsageACMirrorHorizontal			[] PROGMEM = "AC Mir H"; 
const char pstrUsageACMirrorVertical			[] PROGMEM = "AC Mir V"; 
const char pstrUsageACFontSelect				[] PROGMEM = "AC Fnt Sel"; 
const char pstrUsageACFontColor					[] PROGMEM = "AC Fnt Clr"; 
const char pstrUsageACFontSize					[] PROGMEM = "AC Fnt Size"; 
const char pstrUsageACJustifyLeft				[] PROGMEM = "AC Just Left"; 
const char pstrUsageACJustifyCenterH			[] PROGMEM = "AC Just Cent H"; 
const char pstrUsageACJustifyRight				[] PROGMEM = "AC Just Right"; 
const char pstrUsageACJustifyBlockH				[] PROGMEM = "AC Just Block H"; 
const char pstrUsageACJustifyTop				[] PROGMEM = "AC Just Top"; 
const char pstrUsageACJustifyCenterV			[] PROGMEM = "AC Just Cent V"; 
const char pstrUsageACJustifyBottom				[] PROGMEM = "AC Just Bot"; 
const char pstrUsageACJustifyBlockV				[] PROGMEM = "AC Just Block V"; 
const char pstrUsageACIndentDecrease			[] PROGMEM = "AC Indent Dec"; 
const char pstrUsageACIndentIncrease			[] PROGMEM = "AC Indent Inc"; 
const char pstrUsageACNumberedList				[] PROGMEM = "AC Num List"; 
const char pstrUsageACRestartNumbering			[] PROGMEM = "AC Res Num"; 
const char pstrUsageACBulletedList				[] PROGMEM = "AC Blt List"; 
const char pstrUsageACPromote					[] PROGMEM = "AC Promote"; 
const char pstrUsageACDemote					[] PROGMEM = "AC Demote"; 
const char pstrUsageACYes						[] PROGMEM = "AC Yes"; 
const char pstrUsageACNo						[] PROGMEM = "AC No"; 
const char pstrUsageACCancel					[] PROGMEM = "AC Cancel"; 
const char pstrUsageACCatalog					[] PROGMEM = "AC Ctlg"; 
const char pstrUsageACBuyChkout					[] PROGMEM = "AC Buy"; 
const char pstrUsageACAddToCart					[] PROGMEM = "AC Add2Cart"; 
const char pstrUsageACExpand					[] PROGMEM = "AC Xpnd"; 
const char pstrUsageACExpandAll					[] PROGMEM = "AC Xpand All"; 
const char pstrUsageACCollapse					[] PROGMEM = "AC Collapse"; 
const char pstrUsageACCollapseAll				[] PROGMEM = "AC Collapse All"; 
const char pstrUsageACPrintPreview				[] PROGMEM = "AC Prn Prevw"; 
const char pstrUsageACPasteSpecial				[] PROGMEM = "AC Paste Spec"; 
const char pstrUsageACInsertMode				[] PROGMEM = "AC Ins Mode"; 
const char pstrUsageACDelete					[] PROGMEM = "AC Del"; 
const char pstrUsageACLock						[] PROGMEM = "AC Lock"; 
const char pstrUsageACUnlock					[] PROGMEM = "AC Unlock"; 
const char pstrUsageACProtect					[] PROGMEM = "AC Prot"; 
const char pstrUsageACUnprotect					[] PROGMEM = "AC Unprot"; 
const char pstrUsageACAttachComment				[] PROGMEM = "AC Attach Cmnt"; 
const char pstrUsageACDeleteComment				[] PROGMEM = "AC Del Cmnt"; 
const char pstrUsageACViewComment				[] PROGMEM = "AC View Cmnt"; 
const char pstrUsageACSelectWord				[] PROGMEM = "AC Sel Word"; 
const char pstrUsageACSelectSentence			[] PROGMEM = "AC Sel Sntc"; 
const char pstrUsageACSelectParagraph			[] PROGMEM = "AC Sel Para"; 
const char pstrUsageACSelectColumn				[] PROGMEM = "AC Sel Col"; 
const char pstrUsageACSelectRow					[] PROGMEM = "AC Sel Row"; 
const char pstrUsageACSelectTable				[] PROGMEM = "AC Sel Tbl"; 
const char pstrUsageACSelectObject				[] PROGMEM = "AC Sel Obj"; 
const char pstrUsageACRedoRepeat				[] PROGMEM = "AC Redo"; 
const char pstrUsageACSort						[] PROGMEM = "AC Sort"; 
const char pstrUsageACSortAscending				[] PROGMEM = "AC Sort Asc"; 
const char pstrUsageACSortDescending			[] PROGMEM = "AC Sort Desc"; 
const char pstrUsageACFilter					[] PROGMEM = "AC Filt"; 
const char pstrUsageACSetClock					[] PROGMEM = "AC Set Clk"; 
const char pstrUsageACViewClock					[] PROGMEM = "AC View Clk"; 
const char pstrUsageACSelectTimeZone			[] PROGMEM = "AC Sel Time Z"; 
const char pstrUsageACEditTimeZone				[] PROGMEM = "AC Edt Time Z"; 
const char pstrUsageACSetAlarm					[] PROGMEM = "AC Set Alm"; 
const char pstrUsageACClearAlarm				[] PROGMEM = "AC Clr Alm"; 
const char pstrUsageACSnoozeAlarm				[] PROGMEM = "AC Snz Alm"; 
const char pstrUsageACResetAlarm				[] PROGMEM = "AC Rst Alm"; 
const char pstrUsageACSyncronize				[] PROGMEM = "AC Sync"; 
const char pstrUsageACSendReceive				[] PROGMEM = "AC Snd/Rcv"; 
const char pstrUsageACSendTo					[] PROGMEM = "AC Snd To"; 
const char pstrUsageACReply						[] PROGMEM = "AC Reply"; 
const char pstrUsageACReplyAll					[] PROGMEM = "AC Reply All"; 
const char pstrUsageACForwardMessage			[] PROGMEM = "AC Fwd Msg"; 
const char pstrUsageACSend						[] PROGMEM = "AC Snd"; 
const char pstrUsageACAttachFile				[] PROGMEM = "AC Att File"; 
const char pstrUsageACUpload					[] PROGMEM = "AC Upld"; 
const char pstrUsageACDownload					[] PROGMEM = "AC Dnld"; 
const char pstrUsageACSetBorders				[] PROGMEM = "AC Set Brd"; 
const char pstrUsageACInsertRow					[] PROGMEM = "AC Ins Row"; 
const char pstrUsageACInsertColumn				[] PROGMEM = "AC Ins Col"; 
const char pstrUsageACInsertFile				[] PROGMEM = "AC Ins File"; 
const char pstrUsageACInsertPicture				[] PROGMEM = "AC Ins Pic"; 
const char pstrUsageACInsertObject				[] PROGMEM = "AC Ins Obj"; 
const char pstrUsageACInsertSymbol				[] PROGMEM = "AC Ins Sym"; 
const char pstrUsageACSaveAndClose				[] PROGMEM = "AC Sav&Cls"; 
const char pstrUsageACRename					[] PROGMEM = "AC Rename"; 
const char pstrUsageACMerge						[] PROGMEM = "AC Merge"; 
const char pstrUsageACSplit						[] PROGMEM = "AC Split"; 
const char pstrUsageACDistributeHorizontaly		[] PROGMEM = "AC Dist Hor"; 
const char pstrUsageACDistributeVerticaly		[] PROGMEM = "AC Dist Ver"; 

// Digitaizers
const char pstrUsageDigitizer					[] PROGMEM = "Digitizer"; 
const char pstrUsagePen							[] PROGMEM = "Pen"; 
const char pstrUsageLightPen					[] PROGMEM = "Light Pen"; 
const char pstrUsageTouchScreen					[] PROGMEM = "Touch Scr"; 
const char pstrUsageTouchPad					[] PROGMEM = "Touch Pad"; 
const char pstrUsageWhiteBoard					[] PROGMEM = "White Brd"; 
const char pstrUsageCoordinateMeasuringMachine	[] PROGMEM = "Coord Meas Mach"; 
const char pstrUsage3DDigitizer					[] PROGMEM = "3D Dgtz"; 
const char pstrUsageStereoPlotter				[] PROGMEM = "Stereo Plot"; 
const char pstrUsageArticulatedArm				[] PROGMEM = "Art Arm"; 
const char pstrUsageArmature					[] PROGMEM = "Armature"; 
const char pstrUsageMultiplePointDigitizer		[] PROGMEM = "Multi Point Dgtz"; 
const char pstrUsageFreeSpaceWand				[] PROGMEM = "Free Space Wand"; 
const char pstrUsageStylus						[] PROGMEM = "Stylus"; 
const char pstrUsagePuck						[] PROGMEM = "Puck"; 
const char pstrUsageFinger						[] PROGMEM = "Finger"; 
const char pstrUsageTipPressure					[] PROGMEM = "Tip Press"; 
const char pstrUsageBarrelPressure				[] PROGMEM = "Brl Press"; 
const char pstrUsageInRange						[] PROGMEM = "In Range"; 
const char pstrUsageTouch						[] PROGMEM = "Touch"; 
const char pstrUsageUntouch						[] PROGMEM = "Untouch"; 
const char pstrUsageTap							[] PROGMEM = "Tap"; 
const char pstrUsageQuality						[] PROGMEM = "Qlty"; 
const char pstrUsageDataValid					[] PROGMEM = "Data Valid"; 
const char pstrUsageTransducerIndex				[] PROGMEM = "Transducer Ind"; 
const char pstrUsageTabletFunctionKeys			[] PROGMEM = "Tabl Func Keys"; 
const char pstrUsageProgramChangeKeys			[] PROGMEM = "Pgm Chng Keys"; 
//const char pstrUsageBatteryStrength				[] PROGMEM = "Bat Strength"; 
const char pstrUsageInvert						[] PROGMEM = "Invert"; 
const char pstrUsageXTilt						[] PROGMEM = "X Tilt"; 
const char pstrUsageYTilt						[] PROGMEM = "Y Tilt"; 
const char pstrUsageAzimuth						[] PROGMEM = "Azimuth"; 
const char pstrUsageAltitude					[] PROGMEM = "Altitude"; 
const char pstrUsageTwist						[] PROGMEM = "Twist"; 
const char pstrUsageTipSwitch					[] PROGMEM = "Tip Sw"; 
const char pstrUsageSecondaryTipSwitch			[] PROGMEM = "Scnd Tip Sw"; 
const char pstrUsageBarrelSwitch				[] PROGMEM = "Brl Sw"; 
const char pstrUsageEraser						[] PROGMEM = "Eraser"; 
const char pstrUsageTabletPick					[] PROGMEM = "Tbl Pick"; 

// Alphanumeric Display Page
const char pstrUsageAlphanumericDisplay			[] PROGMEM = "Alphanum Disp"; 
const char pstrUsageBitmappedDisplay			[] PROGMEM = "Bmp Disp"; 
const char pstrUsageDisplayAttributesReport		[] PROGMEM = "Disp Attr Rpt"; 
const char pstrUsageASCIICharacterSet			[] PROGMEM = "ASCII chset"; 
const char pstrUsageDataReadBack				[] PROGMEM = "Data Rd Back"; 
const char pstrUsageFontReadBack				[] PROGMEM = "Fnt Rd Back"; 
const char pstrUsageDisplayControlReport		[] PROGMEM = "Disp Ctrl Rpt"; 
const char pstrUsageClearDisplay				[] PROGMEM = "Clr Disp"; 
//const char pstrUsageDisplayEnable				[] PROGMEM = "Disp Enbl"; 
const char pstrUsageScreenSaverDelay			[] PROGMEM = "Scr Sav Delay"; 
const char pstrUsageScreenSaverEnable			[] PROGMEM = "Scr Sav Enbl"; 
const char pstrUsageVerticalScroll				[] PROGMEM = "V Scroll"; 
const char pstrUsageHorizontalScroll			[] PROGMEM = "H Scroll"; 
const char pstrUsageCharacterReport				[] PROGMEM = "Char Rpt"; 
const char pstrUsageDisplayData					[] PROGMEM = "Disp Data"; 
const char pstrUsageDisplayStatus				[] PROGMEM = "Disp Stat"; 
const char pstrUsageStatusNotReady				[] PROGMEM = "Stat !Ready"; 
const char pstrUsageStatusReady					[] PROGMEM = "Stat Ready"; 
const char pstrUsageErrorNotALoadableCharacter	[] PROGMEM = "Err Not Ld Char"; 
const char pstrUsageErrorFotDataCanNotBeRead	[] PROGMEM = "Fnt Data Rd Err"; 
const char pstrUsageCursorPositionReport		[] PROGMEM = "Cur Pos Rpt"; 
const char pstrUsageRow							[] PROGMEM = "Row"; 
const char pstrUsageColumn						[] PROGMEM = "Col"; 
const char pstrUsageRows						[] PROGMEM = "Rows"; 
const char pstrUsageColumns						[] PROGMEM = "Cols"; 
const char pstrUsageCursorPixelPosition			[] PROGMEM = "Cur Pix Pos"; 
const char pstrUsageCursorMode					[] PROGMEM = "Cur Mode"; 
const char pstrUsageCursorEnable				[] PROGMEM = "Cur Enbl"; 
const char pstrUsageCursorBlink					[] PROGMEM = "Cur Blnk"; 
const char pstrUsageFontReport					[] PROGMEM = "Fnt Rpt"; 
const char pstrUsageFontData					[] PROGMEM = "Fnt Data"; 
const char pstrUsageCharacterWidth				[] PROGMEM = "Char Wdth"; 
const char pstrUsageCharacterHeight				[] PROGMEM = "Char Hght"; 
const char pstrUsageCharacterSpacingHorizontal	[] PROGMEM = "Char Space H"; 
const char pstrUsageCharacterSpacingVertical	[] PROGMEM = "Char Space V"; 
const char pstrUsageUnicodeCharset				[] PROGMEM = "Unicode Char"; 
const char pstrUsageFont7Segment				[] PROGMEM = "Fnt 7-seg"; 
const char pstrUsage7SegmentDirectMap			[] PROGMEM = "7-seg map"; 
const char pstrUsageFont14Segment				[] PROGMEM = "Fnt 14-seg"; 
const char pstrUsage14SegmentDirectMap			[] PROGMEM = "14-seg map"; 
const char pstrUsageDisplayBrightness			[] PROGMEM = "Disp Bright"; 
const char pstrUsageDisplayContrast				[] PROGMEM = "Disp Cntrst"; 
const char pstrUsageCharacterAttribute			[] PROGMEM = "Char Attr"; 
const char pstrUsageAttributeReadback			[] PROGMEM = "Attr Readbk"; 
const char pstrUsageAttributeData				[] PROGMEM = "Attr Data"; 
const char pstrUsageCharAttributeEnhance		[] PROGMEM = "Char Attr Enh"; 
const char pstrUsageCharAttributeUnderline		[] PROGMEM = "Char Attr Undl"; 
const char pstrUsageCharAttributeBlink			[] PROGMEM = "Char Attr Blnk"; 
const char pstrUsageBitmapSizeX					[] PROGMEM = "Bmp Size X"; 
const char pstrUsageBitmapSizeY					[] PROGMEM = "Bmp Size Y"; 
const char pstrUsageBitDepthFormat				[] PROGMEM = "Bit Dpth Fmt"; 
const char pstrUsageDisplayOrientation			[] PROGMEM = "Disp Ornt"; 
const char pstrUsagePaletteReport				[] PROGMEM = "Pal Rpt"; 
const char pstrUsagePaletteDataSize				[] PROGMEM = "Pal Data Size"; 
const char pstrUsagePaletteDataOffset			[] PROGMEM = "Pal Data Off"; 
const char pstrUsagePaletteData					[] PROGMEM = "Pal Data"; 
const char pstrUsageBlitReport					[] PROGMEM = "Blit Rpt"; 
const char pstrUsageBlitRectangleX1				[] PROGMEM = "Blit Rect X1"; 
const char pstrUsageBlitRectangleY1				[] PROGMEM = "Blit Rect Y1"; 
const char pstrUsageBlitRectangleX2				[] PROGMEM = "Blit Rect X2"; 
const char pstrUsageBlitRectangleY2				[] PROGMEM = "Blit Rect Y2"; 
const char pstrUsageBlitData					[] PROGMEM = "Blit Data"; 
const char pstrUsageSoftButton					[] PROGMEM = "Soft Btn"; 
const char pstrUsageSoftButtonID				[] PROGMEM = "Soft Btn ID"; 
const char pstrUsageSoftButtonSide				[] PROGMEM = "Soft Btn Side"; 
const char pstrUsageSoftButtonOffset1			[] PROGMEM = "Soft Btn Off1"; 
const char pstrUsageSoftButtonOffset2			[] PROGMEM = "Soft Btn Off2"; 
const char pstrUsageSoftButtonReport			[] PROGMEM = "Soft Btn Rpt"; 

// Medical Instrument Page
const char pstrUsageMedicalUltrasound			[] PROGMEM = "Med Ultrasnd"; 
const char pstrUsageVCRAcquisition				[] PROGMEM = "VCR/Acq"; 
const char pstrUsageFreezeThaw					[] PROGMEM = "Freeze"; 
const char pstrUsageClipStore					[] PROGMEM = "Clip Store"; 
const char pstrUsageUpdate						[] PROGMEM = "Update"; 
const char pstrUsageNext						[] PROGMEM = "Next"; 
const char pstrUsageSave						[] PROGMEM = "Save"; 
const char pstrUsagePrint						[] PROGMEM = "Print"; 
const char pstrUsageMicrophoneEnable			[] PROGMEM = "Mic Enbl"; 
const char pstrUsageCine						[] PROGMEM = "Cine"; 
const char pstrUsageTransmitPower				[] PROGMEM = "Trans Pwr"; 
//const char pstrUsageVolume						[] PROGMEM = "Vol"; 
const char pstrUsageFocus						[] PROGMEM = "Focus"; 
const char pstrUsageDepth						[] PROGMEM = "Depth"; 
const char pstrUsageSoftStepPrimary				[] PROGMEM = "Soft Stp-Pri"; 
const char pstrUsageSoftStepSecondary			[] PROGMEM = "Soft Stp-Sec"; 
const char pstrUsageDepthGainCompensation		[] PROGMEM = "Dpth Gain Comp"; 
const char pstrUsageZoomSelect					[] PROGMEM = "Zoom Sel"; 
const char pstrUsageZoomAdjust					[] PROGMEM = "Zoom Adj"; 
const char pstrUsageSpectralDopplerModeSelect	[] PROGMEM = "Spec Dop Mode Sel"; 
const char pstrUsageSpectralDopplerModeAdjust	[] PROGMEM = "Spec Dop Mode Adj"; 
const char pstrUsageColorDopplerModeSelect		[] PROGMEM = "Color Dop Mode Sel"; 
const char pstrUsageColorDopplerModeAdjust		[] PROGMEM = "Color Dop Mode Adj"; 
const char pstrUsageMotionModeSelect			[] PROGMEM = "Motion Mode Sel"; 
const char pstrUsageMotionModeAdjust			[] PROGMEM = "Motion Mode Adj"; 
const char pstrUsage2DModeSelect				[] PROGMEM = "2D Mode Sel"; 
const char pstrUsage2DModeAdjust				[] PROGMEM = "2D Mode Adj"; 
const char pstrUsageSoftControlSelect			[] PROGMEM = "Soft Ctrl Sel"; 
const char pstrUsageSoftControlAdjust			[] PROGMEM = "Soft Ctrl Adj"; 


#endif // __HID_H__