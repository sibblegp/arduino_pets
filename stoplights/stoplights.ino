#include <LiquidCrystal.h>
#include "Arduino.h"

#include "lightcontrol.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
 
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
 return btnNONE;  // when all others fail, return this...
}

void printStandby(){
  lcd.setCursor(0, 1);
  lcd.print("Standby");
}

void printInfo(){
    lcd.setCursor(0, 0);
    lcd.print("MCSLC v0.2");
}

volatile int ambulance_count = 0;
volatile lightState allStopState;

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  printInfo();
  //printStandby();
  
  //Serial.println("Setting up lights...");
  lcd.setCursor(0, 1);
 // lcd.print("Setting up...");
  
  initializeLights();
  initializeInputsAndLCD();
  //attachInterrupt(2, handleAmbulance, FALLING);
  allStopState.lstate = ALL_STOP;
  allStopState.astate = 0;
  
  //Setup out put for 5V power
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  
}


void handleAmbulance(){
        //Serial.println("----AMBULANCE----");
        //lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("AMBULANCE");
        //ambulance_count += 1;
        //lcd.print(ambulance_count);
        //initializeLights();
        //cState.lstate = allStopState.lstate;
        //cState.astate = allStopState.astate;
}

void loop(){
	//Debug output code
	Serial.println("\nENTERING MAIN LIGHT CYCLE TASK\n");
	
        //Attach Ambulance Interrupt
        

	lightState 	nextState,	//Holder for the next state
			stopState;	//Blank all red state


	//Setup all red state
	stopState.lstate = ALL_STOP;
	stopState.astate = 0;
	
	//Clear the sensors
	//lightFlags cflags = 0;
        cState = stopState;
    
	//Main cycle
	while(1)
	{
                printInfo();
		//Change all of the lights to red
		//NOTE:  cState is NOT changed here
		//This is purely an INTERMEDIATE state
		changeLights(stopState, cState);

		//Determine the next state after the current state
		nextState = determineNextState(cState);

		//Set cState to stopState so that LEDs change appropriately
		//Already determined next state so cState doesn't need to reflect actual state
		cState = stopState;

                printStatus(cState, lcd);

		//Wait for a period
		delay(3000);

		//Change the lights to the next state
		changeLights(nextState, cState);
     
           
                //******ARDUINO*******
                //delay(1000);
		
		//When done changing lights, change current state to reflect
		cState = nextState;
		
		//DEBUG:  Print the current state
		printStatus(cState, lcd);

		//Turn signal handling
		//Since the waiting time is different for go states and turn states
		
		//If there is a turning state active (IE. not a GO state)
		if(cState.lstate != EW_GO && cState.lstate != NS_GO)
		{
			//Wait a period with the turning light green
			delay(4000);
			
			//Determine the next state
			nextState = determineNextState(cState);
			
			//DEBUG CODE
			//puts("\nTURN STATE\n");
			
			//Change the lights
			changeLights(nextState, cState);
			
			//Set the current state to reflect change
			cState = nextState;
			
			//DEBUG:  Print current state
		        printStatus(cState, lcd);
			
			//Wait a period with the go light green
			delay(7000);
		}
		else
			//If not a turning state, wait a period with lights green
			delay(10000);
	}
}

