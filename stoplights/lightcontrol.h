#include <LiquidCrystal.h>

/******************************************************
			DEFINITIONS
******************************************************/

//Standard task stack size
#define TASK_STK_SIZE   1024

//Light bits
//These are the bits designating the individual light status
//0 = RED
//1 = GREEN

#define LIGHT_NORTH 	8
#define LIGHT_SOUTH 	4
#define LIGHT_EAST 	2
#define LIGHT_WEST	1

#define TURN_NORTH	128
#define TURN_SOUTH	64
#define TURN_EAST	32
#define TURN_WEST	16

#define WALK_NS	1
#define WALK_EW	2

//Exact States
//These are states defined by the sum of the status of several lights

//All Reds
#define ALL_STOP	0

//Green in both directions
#define NS_GO		LIGHT_NORTH + LIGHT_SOUTH
#define EW_GO		LIGHT_EAST + LIGHT_WEST

//Green for both directions to turn
#define NS_TURN		TURN_NORTH + TURN_SOUTH
#define EW_TURN	TURN_EAST + TURN_WEST

//Green to turn and go straight for one direction
#define N_TURN		LIGHT_NORTH + TURN_NORTH
#define S_TURN		LIGHT_SOUTH + TURN_SOUTH
#define E_TURN		LIGHT_EAST + TURN_EAST
#define W_TURN		LIGHT_WEST + TURN_WEST

//FLAGS
//These flags correspond to the input pins for the sensors

//Cars waiting to turn states (ie. which bits need to be set)
#define NORTH_TURN_FLAG 1			//Pin 1
#define SOUTH_TURN_FLAG 2		//Pin 2
#define EAST_TURN_FLAG 4			//Pin 3
#define WEST_TURN_FLAG 8			//Pin 4

#define NORTH_AMBULANCE_FLAG 16	//Pin 5
#define SOUTH_AMBULANCE_FLAG 32	//Pin 6
#define EAST_AMBULANCE_FLAG 64	//Pin 7
#define WEST_AMBULANCE_FLAG 128	//Pin 8

//Input PINs
#define NORTH_TURN_PIN 48			
#define SOUTH_TURN_PIN 49		
#define EAST_TURN_PIN 50			
#define WEST_TURN_PIN 51			

//#define NORTH_AMBULANCE_PIN 52	
//#define SOUTH_AMBULANCE_PIN 53	
//#define EAST_AMBULANCE_PIN 54	
//#define WEST_AMBULANCE_PIN 55	

//LEDs
//These correspond to the pins attached to the ports with the specific lights

//Red lights
#define LED_NORTH_RED	22
#define LED_SOUTH_RED	23
#define LED_EAST_RED    24
#define LED_WEST_RED	25

//Yellow lights
#define LED_NORTH_YELLOW	26
#define LED_SOUTH_YELLOW	27
#define LED_EAST_YELLOW		28
#define LED_WEST_YELLOW		29

//Green lights
#define LED_NORTH_GREEN		30
#define LED_SOUTH_GREEN		31
#define LED_EAST_GREEN		32
#define LED_WEST_GREEN		33

//Turn signal yellow lights
#define LED_NORTH_TURN_YELLOW	34
#define LED_SOUTH_TURN_YELLOW	35
#define LED_EAST_TURN_YELLOW	36
#define LED_WEST_TURN_YELLOW	37

//Turn signal green lights
#define LED_NORTH_TURN_GREEN	38
#define LED_SOUTH_TURN_GREEN	39
#define LED_EAST_TURN_GREEN		40
#define LED_WEST_TURN_GREEN		41

//Walk signal LEDs
#define LED_WALK_NS_WHITE		42
#define LED_WALK_EW_WHITE		44

//Unused red walk LEDs
#define LED_WALK_NS_RED                 46
#define LED_WALK_EW_RED                 47

/******************************************************
			TYPE DEFINITIONS
******************************************************/

typedef int lightFlags;

//lightState type
//Contains state defining what lights are on and off
typedef struct{
	int lstate;	//Light State
	int astate;	//Ambulance State
} lightState;

static lightState cState;
static lightFlags cflags;

/******************************************************
			FUNCTION PROTOTYPES
******************************************************/

//determineNextState:  Receives a state and determines the next appropriate one
lightState determineNextState(lightState currState);

//changeLighgts:  Changes the lights from their current state into the passed state
void changeLights(lightState nextState, lightState cState);

//initializeLights:  Initializes the LEDs to all red and sets up the ports
void initializeLights();
void setLightsOff(int*, int);
void setLightsOn(int*, int);
void setLightOn(int);
void setLightOff(int);
void initializeInputsAndLCD();

//checkSensors:  Sets the cflags variable to match the current input from the sensors
void checkSensors();

//printStatus:  Outputs the current status over the serial port in ASCII
void printStatus(lightState currState, LiquidCrystal lcd);  //TESTING FUNCTION
