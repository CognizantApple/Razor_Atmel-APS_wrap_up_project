/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"
#include "buttons.h"
#include "lcd_bitmaps.h"
#include "lcd_NHD-C12864LZ.h"
#include "ant_api.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  
  LcdClearScreen();
  PixelAddressType UserApp_sStringLocation = {LCD_SMALL_FONT_LINE3, LCD_LEFT_MOST_COLUMN};
  u8 au8TestString[22] = {'U','p','d','a','t','e', ' ', 'p','e','r','i','o','d',':',' ', '1','0','0',' ','m','s',0};
  LcdLoadString(au8TestString, LCD_FONT_SMALL, &UserApp_sStringLocation); 
LedOn(RED0);
LedOn(RED1);
LedOn(RED2);
LedOn(RED3);
LedOff(GREEN0);
LedOff(GREEN1);
LedOff(GREEN2);
LedOff(GREEN3);
LedOff(BLUE0);
LedOff(BLUE1);
LedOff(BLUE2);
LedOff(BLUE3);

  
  /* If good initialization, set state to Idle */
  if( 1 /* Add condition for good init */)
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


void UserAppRunActiveState(void)
{
 UserAppSM_Idle();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  
  if (IsButtonPressed(PA_17_BUTTON0)){
    PWMAudioSetFrequency(BUZZER1, 262);
    PWMAudioOn(BUZZER1);
  }
  
  if (!(IsButtonPressed(PA_17_BUTTON0))){
    PWMAudioOff(BUZZER1);
    ButtonAcknowledge(PA_17_BUTTON0);
    
  }
  if (IsButtonPressed(PB_00_BUTTON1)){
    PWMAudioSetFrequency(BUZZER1, 294);
    PWMAudioOn(BUZZER1);
  }
  if (!(IsButtonPressed(PB_00_BUTTON1))){
    PWMAudioOff(BUZZER1);
    ButtonAcknowledge(PB_00_BUTTON1);
    
  }
  
  // ~~~~~ COLOUR RAINBOW AND BUTTON PROGRAM ENSEMBLE
  
  static PixelAddressType sTestStringLocation = {LCD_SMALL_FONT_LINE3, LCD_LEFT_MOST_COLUMN};
  static u8 au8TestString[22] = {'U','p','d','a','t','e', ' ', 'p','e','r','i','o','d',':',' ', '1','0','0',' ','m','s',0}; 
    
  static LedNumberType aeCurrentLed[] = {GREEN0, RED0, BLUE0, GREEN1, RED1, BLUE1, GREEN2, RED2, BLUE2, GREEN3, RED3, BLUE3};
  static bool abLedRateIncreasing[] = {TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, FALSE};
  
  static u16 TimeFraction = 100;
  static u16 u16CyclePeriod = 0;
  static u8 u8CurrentState = 0;
  static u8 u8PowerLevel = 0;
  
  if(WasButtonPressed(PA_17_BUTTON0)){
    ButtonAcknowledge(PA_17_BUTTON0);
    if(TimeFraction > 20){
      TimeFraction -= 20;
      au8TestString[15] = ((int)TimeFraction / 100) + '0';
      au8TestString[16] = (((int)TimeFraction % 100) / 10) + '0';
      au8TestString[17] = ((int)TimeFraction % 10) + '0';
      LcdLoadString(au8TestString, LCD_FONT_SMALL, &sTestStringLocation);
    }
  }
  
  
  if(WasButtonPressed(PB_00_BUTTON1)){
    ButtonAcknowledge(PB_00_BUTTON1);
    if(TimeFraction < 300){
      TimeFraction += 20;
      au8TestString[15] = ((int)TimeFraction / 100) + '0';
      au8TestString[16] = (((int)TimeFraction % 100) / 10) + '0';
      au8TestString[17] = ((int)TimeFraction % 10) + '0';
      LcdLoadString(au8TestString, LCD_FONT_SMALL, &sTestStringLocation);
    } 
  }
  
  u16CyclePeriod++;
  if(u16CyclePeriod >= TimeFraction)
  {
    //0 Red to yellow: red 100%, ramp green from 0 to 100%          0, 3, 6, 9
    if(u8CurrentState == 0){
      u8PowerLevel++;
      for(u8 i = 0; i < 12 ; i++){
        if(abLedRateIncreasing[i])
            LedPWM(aeCurrentLed[i], (LedRateType)(u8PowerLevel));
      }
      if(u8PowerLevel == 20){
        u8CurrentState = 1;
        for(u8 i = 0; i < 12 ; i++){
          if((i-1) % 3 == 0)
            abLedRateIncreasing[i] = TRUE;
          else
            abLedRateIncreasing[i] = FALSE;
        }
      }
              
      u16CyclePeriod = 0;         
    }
    //1 Yellow to green: green 100%, ramp red from 100% to 0        1, 4, 7, 10
    else if(u8CurrentState == 1){
      u8PowerLevel--;
      for(u8 i = 0; i < 12 ; i++){
        if(abLedRateIncreasing[i])
            LedPWM(aeCurrentLed[i], (LedRateType)(u8PowerLevel));
      }
      if(u8PowerLevel == 0){
        u8CurrentState = 2;
        for(u8 i = 0; i < 12 ; i++){
          if((i-2) % 3 == 0)
            abLedRateIncreasing[i] = TRUE;
          else
            abLedRateIncreasing[i] = FALSE;
        }
      }
              
      u16CyclePeriod = 0;         
    }
    //2 Green to cyan: green 100%, ramp blue from 0 to 100% 
    else if(u8CurrentState == 2){
      u8PowerLevel++;
      for(u8 i = 0; i < 12 ; i++){
        if(abLedRateIncreasing[i])
            LedPWM(aeCurrentLed[i], (LedRateType)(u8PowerLevel));
      }
      if(u8PowerLevel == 20){
        u8CurrentState = 3;
        for(u8 i = 0; i < 12 ; i++){
          if((i) % 3 == 0)
            abLedRateIncreasing[i] = TRUE;
          else
            abLedRateIncreasing[i] = FALSE;
        }
      }
              
      u16CyclePeriod = 0;         
    }
    //3 Cyan to blue: blue 100%, ramp green from 100% to 0 
    else if(u8CurrentState == 3){
      u8PowerLevel--;
      for(u8 i = 0; i < 12 ; i++){
        if(abLedRateIncreasing[i])
            LedPWM(aeCurrentLed[i], (LedRateType)(u8PowerLevel));
      }
      if(u8PowerLevel == 0){
        u8CurrentState = 4;
        for(u8 i = 0; i < 12 ; i++){
          if((i-1) % 3 == 0)
            abLedRateIncreasing[i] = TRUE;
          else
            abLedRateIncreasing[i] = FALSE;
        }
      }
              
      u16CyclePeriod = 0;         
    }
    //4 Blue to purple: blue 100%, ramp red from 0 to 100%
    else if(u8CurrentState == 4){
      u8PowerLevel++;
      for(u8 i = 0; i < 12 ; i++){
        if(abLedRateIncreasing[i])
            LedPWM(aeCurrentLed[i], (LedRateType)(u8PowerLevel));
      }
      if(u8PowerLevel == 20){
        u8CurrentState = 5;
        for(u8 i = 0; i < 12 ; i++){
          if((i-2) % 3 == 0)
            abLedRateIncreasing[i] = TRUE;
          else
            abLedRateIncreasing[i] = FALSE;
        }
      }
              
      u16CyclePeriod = 0;         
    }
    //5  Purple to red: red 100%, ramp blue from 100% to 0
    else if(u8CurrentState == 5){
      u8PowerLevel--;
      for(u8 i = 0; i < 12 ; i++){
        if(abLedRateIncreasing[i])
            LedPWM(aeCurrentLed[i], (LedRateType)(u8PowerLevel));
      }
      if(u8PowerLevel == 0){
        u8CurrentState = 0;
        for(u8 i = 0; i < 12 ; i++){
          if((i) % 3 == 0)
            abLedRateIncreasing[i] = TRUE;
          else
            abLedRateIncreasing[i] = FALSE;
        }
      }
              
      u16CyclePeriod = 0;         
    }
  }
  
  
  
   //0 Red to yellow: red 100%, ramp green from 0 to 100%          0, 3, 6, 9
   //1 Yellow to green: green 100%, ramp red from 100% to 0        1, 4, 7, 10
   //2 Green to cyan: green 100%, ramp blue from 0 to 100%         2, 5, 8, 11
   //3 Cyan to blue: blue 100%, ramp green from 100% to 0          0, 3, 6, 9
   //4 Blue to purple: blue 100%, ramp red from 0 to 100%          1, 4, 7, 10
   //5  Purple to red: red 100%, ramp blue from 100% to 0          2, 5, 8, 11
  
            
  
} /* end UserAppSM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  UserApp_StateMachine = UserAppSM_Idle;
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
