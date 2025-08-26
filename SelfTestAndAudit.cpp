/**************************************************************************
 *     This file is part of the Bally/Stern OS for Arduino Project.

    I, Dick Hamill, the author of this program disclaim all copyright
    in order to make this program freely available in perpetuity to
    anyone who would like to use it. Dick Hamill, 6/1/2020

    BallySternOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    BallySternOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    See <https://www.gnu.org/licenses/>.


  Version 2024.04 by Dave's Think Tank

  - Features were added to the Self-Test function:
  - Sound test, changed to allow cycling through all sounds. Features using game button allow skipping sounds, repeating sounds, fast forward through sounds.
  - Added options in self test for all accounting values to be increased, as well as zeroed.

  Version 2024.07 by Dave's Think Tank

  - Switch Test: Added count of switches set to "On", in credit display.
  - Switch Test: Double-click on credit button resets all drop targets. FLASH GORDON SPECIFIC CODE!
  - These changes to switch test allow a user to impliment a switch-matrix test, by setting multiple switches and looking for incorrect totals.

  Version 2024.08 by Dave's Think Tank

  - Self-test modification: Added use of slam switch to end self-test. Added otherSwitch as a secondary input to self-tests.
  - DIP Switch Test: Added a test for DIP switches. Dip switch banks are displayed in binary (7 digits per display, plus one in ball-in-play or credit window.) 
  - DIP switches can be temporarily changed in game, using otherSwitch (described above).

  Version 2024.09 by Dave's Think Tank

  - Added "Reset Hold" feature to tests for lights, displays, and DIP switches. Reset Hold scrolls quickly through the display / review options.
  - Reset Hold option on audit settings sped up considerably.
  - Solenoids can be made to stop firing during solenoid test by pressing otherSwitch (Coin slot 3 switch).
  - endSwitch fixed so that it will register as a switch in switch test, and NOT end self-test mode.

  Version 2024.11 by Dave's Think Tank

  - Added monitoring of switches to solenoid test, to warn if vibration from a solenoid is setting off a switch

Version 2024.12 by Dave's Think Tank

  - When the solenoid test identifies a switch set off by vibration, it will also note the time in milliseconds between the solenoid firing and the switch activating
  - Cleaned up code by removing old, unused CPC (coins per credit) code.

Version 2025.01 by Dave's Think Tank

  - New double-hit switch bounce test added, in addition to the stuck switch / switch matrix test.
  - Converted from BSOS (Bally/Stern Operating System) to RPU (Retro Pin Upgrade). RPU is an extension of BSOS. BSOS is no longer maintained.
  - Removed references to RPU_OS_USE_GEETEOH (BSOS_OS_USE_GEETEOH). RPU_OS_USE_GEETEOH is now defined in FGyyyypmm.ino as a user definition, where it belongs.
  - Solenoid / Switch test would "detect" self test switch hit on first solenoid. Fixed.

 */

#include <Arduino.h>
#include "SelfTestAndAudit.h"
#include "RPU_Config.h"
#include "RPU.h"

#define MACHINE_STATE_ATTRACT         0
//#define USE_SB100

byte dipBankVal[4];
unsigned long DisplayDIP[6];

unsigned long LastSolTestTime = 0; 
unsigned long LastSelfTestChange = 0;
unsigned long SavedValue = 0;
unsigned long SolSwitchTimer = 0;
unsigned long ResetHold = 0;
unsigned long otherHold = 0;
unsigned long NextSpeedyValueChange = 0;
unsigned long NumSpeedyChanges = 0;
unsigned long LastResetPress = 0;
unsigned long LastOtherPress = 0;

unsigned long SwitchTimer = 0;
byte HoldSwitch = SW_SELF_TEST_SWITCH;

byte CurValue = 0;
byte CurDisplay = 0;
byte CurDigit = 0;
byte xDigit = 0;
byte xDisplay = 0;
boolean SoundPlayed = false;
byte SoundPlaying = 0;
byte SoundToPlay = 0;
boolean SolenoidCycle = true;
boolean SolenoidOn = true;

byte          dispNum = 0;
int           UpDown = 1;



int RunBaseSelfTest(int curState, boolean curStateChanged, unsigned long CurrentTime, byte resetSwitch, byte otherSwitch, byte endSwitch) {
  // Set resetSwitch to the game / credit button on the front of your pinball.
  // Set otherSwitch to any other switch easily accessible from the door of your pinball. This is used in some tests, where more than one switch is required to perform all necessary functions.
  //   I like to use SW_COIN_3, as I have it wired to a handy button for free game purposes!
  // Set endSwitch to the slam switch. This is used to end self test and return to attract mode.

  byte curSwitch = RPU_PullFirstFromSwitchStack();
  int returnState = curState;
  boolean resetDoubleClick = false;
  boolean otherDoubleClick = false;
  unsigned short savedScoreStartByte = 0;
  unsigned short auditNumStartByte = 0;

  if (curSwitch==resetSwitch) {
    ResetHold = CurrentTime;
    if ((CurrentTime-LastResetPress)<400) {
      resetDoubleClick = true;
      curSwitch = SWITCH_STACK_EMPTY;
    }
    LastResetPress = CurrentTime;
  }

  if (curSwitch==otherSwitch) {
    otherHold = CurrentTime;
    if ((CurrentTime-LastOtherPress)<400) {
      otherDoubleClick = true;
      curSwitch = SWITCH_STACK_EMPTY;
    }
    LastOtherPress = CurrentTime;
  }

  if (ResetHold!=0 && !RPU_ReadSingleSwitchState(resetSwitch)) {
    ResetHold = 0;
    NextSpeedyValueChange = 0;
  }

  boolean resetBeingHeld = false;
  if (ResetHold!=0 && (CurrentTime-ResetHold)>1300) {
    resetBeingHeld = true;
    if (NextSpeedyValueChange==0) {
      NextSpeedyValueChange = CurrentTime;
      NumSpeedyChanges = 0;
    }
  }

  if ((curSwitch==endSwitch) && (curState != MACHINE_STATE_TEST_STUCK_SWITCHES)) {
    return MACHINE_STATE_ATTRACT;
  }
  
  if (curSwitch==SW_SELF_TEST_SWITCH && (CurrentTime-LastSelfTestChange)>250) {
    returnState -= 1;
    LastSelfTestChange = CurrentTime;
  }

  if (curStateChanged) {
    
    for (int count=0; count<4; count++) {
      RPU_SetDisplay(count, 0);
      RPU_SetDisplayBlank(count, 0x00);        
    }

    if (curState<=MACHINE_STATE_TEST_SCORE_LEVEL_1) {
      RPU_SetDisplayCredits(MACHINE_STATE_TEST_DIP_SWITCHES-curState);
      RPU_SetDisplayBallInPlay(0, false);
    }
  }

  if (curState==MACHINE_STATE_TEST_LAMPS) { //                                                            *** Test Lamps ***
    if (curStateChanged) {
      RPU_DisableSolenoidStack();        
      RPU_SetDisableFlippers(true);
      RPU_SetDisplayCredits(0);
      RPU_SetDisplayBallInPlay(1);
      RPU_TurnOffAllLamps();
      for (int count=0; count<RPU_MAX_LAMPS; count++) {
        RPU_SetLampState(count, 1, 0, 500);
      }
      CurValue = 99;
      RPU_SetDisplay(0, CurValue, true);
      LastSolTestTime = CurrentTime;
    }
    if (curSwitch==resetSwitch || resetDoubleClick || (ResetHold && CurrentTime > LastSolTestTime + 250)) {
      LastSolTestTime = CurrentTime;
      CurValue += 1;
      if (CurValue>99) CurValue = 0;
      if (CurValue==RPU_MAX_LAMPS) {
        CurValue = 99;
        for (int count=0; count<RPU_MAX_LAMPS; count++) {
          RPU_SetLampState(count, 1, 0, 500);
        }
      } else {
        RPU_TurnOffAllLamps();
        RPU_SetLampState(CurValue, 1, 0, 0);
      }      
      RPU_SetDisplay(0, CurValue, true);  
    }    
  } else if (curState==MACHINE_STATE_TEST_DISPLAYS) { //                                                  *** Test Displays ***
    if (curStateChanged) {
      RPU_TurnOffAllLamps();
      RPU_SetDisplayCredits(0);
      RPU_SetDisplayBallInPlay(2);
      for (int count=0; count<4; count++) {
        RPU_SetDisplayBlank(count, 0x3F);        
      }
      CurValue = 0;
      LastSolTestTime = CurrentTime;
    }
    if (curSwitch==resetSwitch || resetDoubleClick || (ResetHold && CurrentTime > LastSolTestTime + 250)) {
      CurValue += 1;
      LastSolTestTime = CurrentTime;
      #ifdef RPU_OS_USE_7_DIGIT_DISPLAYS
        if (CurValue>34) CurValue = 0;
      #else
        if (CurValue>30) CurValue = 0;
      #endif
    }    
    RPU_CycleAllDisplays(CurrentTime, CurValue);
  } else if (curState==MACHINE_STATE_TEST_SOLENOIDS) { //                                                 *** Test Solenoids ***
    if (curStateChanged) {
      RPU_TurnOffAllLamps();
      LastSolTestTime = CurrentTime;
      SolSwitchTimer = CurrentTime;
      RPU_EnableSolenoidStack(); 
      RPU_SetDisableFlippers(false);
      RPU_SetDisplayBlank(4, 0);
      RPU_SetDisplayBallInPlay(3);
      SolenoidCycle = true;
      SolenoidOn = true;
      SavedValue = 0;
      RPU_PushToSolenoidStack(SavedValue, 5);
    } 
    if (curSwitch==resetSwitch || resetDoubleClick) SolenoidCycle = !SolenoidCycle;
    if (curSwitch == otherSwitch) SolenoidOn = !SolenoidOn;
    if (curSwitch!=resetSwitch && curSwitch != otherSwitch && curSwitch != endSwitch && curSwitch != SWITCH_STACK_EMPTY && curSwitch != SW_SELF_TEST_SWITCH) {
      RPU_SetDisplayCredits(curSwitch);
      RPU_SetDisplay(3, CurrentTime - SolSwitchTimer, true, 3);
    }
    if (!SolenoidOn) {
      RPU_SetDisplayCredits(99, false); // Blank display when solenoids turned off
      RPU_SetDisplayBlank(3, 0);
    }

    if ((CurrentTime-LastSolTestTime)>1000) {
      if (SolenoidCycle) {
        SavedValue += 1;
        if (SavedValue>14) SavedValue = 0;
      }
      if (SolenoidOn) {
        RPU_PushToSolenoidStack(SavedValue, 5);
        SolSwitchTimer = CurrentTime;
      }
      RPU_SetDisplay(0, SavedValue, true);
      LastSolTestTime = CurrentTime;
    }
    
  } else if (curState==MACHINE_STATE_TEST_STUCK_SWITCHES) { //                                                  *** Test Stuck Switches ***
    if (curStateChanged) {
      RPU_TurnOffAllLamps();
      RPU_DisableSolenoidStack(); // switches will not activate solenoids!
      RPU_SetDisableFlippers(true);
      RPU_SetDisplayCredits(0);
      RPU_SetDisplayBallInPlay(4);
    }

    byte displayOutput = 0;
    for (byte switchCount=0; switchCount<40; switchCount++) {
      if (RPU_ReadSingleSwitchState(switchCount)) {
        if (displayOutput < 4) RPU_SetDisplay(displayOutput, switchCount, true);
        displayOutput += 1;
      }
    }

    if (displayOutput<4) {
      for (int count=displayOutput; count<4; count++) {
        RPU_SetDisplayBlank(count, 0x00);
      }
    }
    RPU_SetDisplayCredits(displayOutput); // Let user know how many switches are on, since max four displayed
    
    if (resetDoubleClick) { // Double-click to reset all drop targets. FLASH GORDON GAME SPECIFIC CODE!
      RPU_PushToSolenoidStack(8, 5, true); // SO_DTARGET_1_RESET
      RPU_PushToTimedSolenoidStack(0, 15, CurrentTime + 250, true); //SO_DTARGET_4_RESET
      RPU_PushToTimedSolenoidStack(1, 15, CurrentTime + 500, true); // SO_DTARGET_3_RESET
      RPU_PushToTimedSolenoidStack(2, 15, CurrentTime + 750, true); // SO_DTARGET_INLINE_RESET
    }
  } else if (curState==MACHINE_STATE_TEST_SWITCH_BOUNCE) { //                                                *** Test for Switch Bounce ***
    if (curStateChanged) {
      RPU_TurnOffAllLamps();
      RPU_DisableSolenoidStack(); // switches will not activate solenoids!
      RPU_SetDisableFlippers(true);
      RPU_SetDisplayCredits(0, false);
      RPU_SetDisplayBallInPlay(5);

      for (int count=0; count<4; count++)
          RPU_SetDisplayBlank(count, 0x00);

      SwitchTimer = 0;
      HoldSwitch = SW_SELF_TEST_SWITCH;
    }

    
    if (curSwitch == HoldSwitch && curSwitch != SWITCH_STACK_EMPTY && curSwitch != SW_SELF_TEST_SWITCH && CurrentTime - SwitchTimer < 500) { // double-hit detected on a single switch
      RPU_SetDisplay(0, curSwitch, true);
      RPU_SetDisplay(1, CurrentTime - SwitchTimer, true);
      SwitchTimer = CurrentTime;
    }
    else {
      if (curSwitch != SWITCH_STACK_EMPTY && curSwitch != SW_SELF_TEST_SWITCH) { // single switch hit once
        RPU_SetDisplay(0, curSwitch, true);
        RPU_SetDisplayBlank(1, 0x00);
        HoldSwitch = curSwitch;
        SwitchTimer = CurrentTime;
      }
    }

    if (resetDoubleClick) { // Double-click to reset all drop targets. FLASH GORDON GAME SPECIFIC CODE!
      RPU_PushToSolenoidStack(8, 5, true); // SO_DTARGET_1_RESET
      RPU_PushToTimedSolenoidStack(0, 15, CurrentTime + 250, true); //SO_DTARGET_4_RESET
      RPU_PushToTimedSolenoidStack(1, 15, CurrentTime + 500, true); // SO_DTARGET_3_RESET
      RPU_PushToTimedSolenoidStack(2, 15, CurrentTime + 750, true); // SO_DTARGET_INLINE_RESET
    }

  } else if (curState==MACHINE_STATE_TEST_SOUNDS) { //                                                    *** Test Sounds ***
    if (curStateChanged) {
      // RPU_TurnOffAllLamps();
      RPU_SetDisplayCredits(0);
      RPU_SetDisplayBallInPlay(6);
      SolenoidCycle = true;
      SoundToPlay = 0; 
      // RPU_PlaySoundSquawkAndTalk(SoundToPlay);
      SoundPlaying = SoundToPlay;
      SoundPlayed = true;
      // RPU_SetDisplay(0, (unsigned long)SoundToPlay, true);
      LastSolTestTime = CurrentTime - 5000; // Time the sound started to play (5 seconds ago)
    } 

    if (resetBeingHeld && (CurrentTime - LastSolTestTime > 250)) {
      SoundToPlay += 1;
      SoundPlayed = false;
      RPU_SetDisplay(0,(unsigned long) SoundToPlay, true);
      LastSolTestTime = CurrentTime;
      SolenoidCycle = true;
    }
    else {
      if (curSwitch==resetSwitch || resetDoubleClick) {
        if (CurrentTime - LastSolTestTime <= 500) { // Allow 0.5 seconds to click and move forward without playing sound
          SoundToPlay +=1;
          if (SoundToPlay > 255) SoundToPlay = 0;
          RPU_SetDisplay(0, (unsigned long)SoundToPlay, true);
          LastSolTestTime = CurrentTime - 500;
          }
        else {
          SolenoidCycle = !SolenoidCycle;
          }
        }
      if ((CurrentTime - LastSolTestTime) >= 500 && !SoundPlayed) {
        #if   defined (RPU_OS_USE_SB100)
          RPU_PlaySB100(soundToPlay);
        #elif defined (RPU_OS_USE_S_AND_T)
          RPU_PlaySoundSAndT(SoundToPlay);
        #elif defined (RPU_OS_USE_DASH51)
          RPU_PlaySoundDash51(soundToPlay);
        #elif defined (RPU_OS_USE_WTYPE_1_SOUND)
          RPU_PushToSoundStack(soundToPlay*256, 8);
        #elif defined (RPU_OS_USE_WTYPE_2_SOUND)
          RPU_PushToSoundStack(SoundToPlay, 8);
        #elif defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
          returnState = 10000 + SoundToPlay;          // Main program has all the info to play sounds using WAV Trigger!
        #endif
        
        SoundPlaying = SoundToPlay;
        SoundPlayed = true;
        }
      if ((CurrentTime - LastSolTestTime) >= 5000) {
        if (SolenoidCycle) {
          SoundToPlay += 1;
          if (SoundToPlay > 255) SoundToPlay = 0;
          }
        LastSolTestTime = CurrentTime;
        SoundPlayed = false;
        RPU_SetDisplay(0, (unsigned long)SoundToPlay, true);
      }
    }
  } else if (curState==MACHINE_STATE_TEST_DIP_SWITCHES) { //                                              *** Test DIP Switches ***
    
    if (curStateChanged) {
      #ifdef USE_SB100    
      RPU_PlaySB100(0);
      #endif
      #ifdef RPU_OS_USE_S_AND_T
      RPU_PlaySoundSAndT(90); // Sound off for Geeteoh
      #endif

      RPU_TurnOffAllLamps();

      for (int i=0; i<4; i++) { // Get four DIP banks from memory, convert to binary display
        dipBankVal[i] = RPU_ReadByteFromEEProm(RPU_DIP_BANK + i);
        DisplayDIP[i] = 0;
        int k = 64;
        for (int j=0; j<7; ++j) {
          DisplayDIP[i] = 10 * DisplayDIP[i] + ((dipBankVal[i] & k) != 0);
          k = k >> 1;
        }
        RPU_SetDisplayBlank(i, 127);
        RPU_SetDisplay(i, DisplayDIP[i], false);
      }
      DisplayDIP[4] = 10 * (dipBankVal[1] >= 128) + (dipBankVal[0] >= 128);
      DisplayDIP[5] = 10 * (dipBankVal[3] >= 128) + (dipBankVal[2] >= 128);
      RPU_SetDisplayBallInPlay(DisplayDIP[4]);
      RPU_SetDisplayCredits(DisplayDIP[5]);

      CurValue = 0;
      xDisplay = CurDisplay = 0;
      LastSolTestTime = CurrentTime;
    }
   
    if (curSwitch==resetSwitch || resetDoubleClick || (ResetHold && CurrentTime > LastSolTestTime + 250)) {
      if (xDisplay < 4) RPU_SetDisplayBlank(CurDisplay, 127); // Reset previous digit to not flash
      else RPU_SetDisplayBlank(4, 108);
      
      CurValue += 1;
      if (CurValue>=32) CurValue = 0;
      LastSolTestTime = CurrentTime;
    }    
    xDisplay = CurDisplay = CurValue / 8;
    xDigit = CurDigit = CurValue % 8;

    if (CurDigit == 7) { // Final digit must be displayed in ball-in-play or credit window
      xDigit = CurDisplay & 1; // Digit 0 or 1, depending on which display being completed
      xDisplay = 4 + CurValue / 16; // Ball in play or credit window
    }

    if (curSwitch == otherSwitch) { // Flip current digit in current display
      dipBankVal[CurDisplay] = dipBankVal[CurDisplay] ^ (1 << CurDigit); // exclusive or function, reverses current digit
      RPU_WriteByteToEEProm(RPU_DIP_BANK + CurDisplay, dipBankVal[CurDisplay]);

      if (xDisplay < 4) { // display value as binary
        DisplayDIP[CurDisplay] = 0;
        int k = 64;
        for (int j=0; j<7; ++j) {
          DisplayDIP[CurDisplay] = 10 * DisplayDIP[CurDisplay] + ((dipBankVal[CurDisplay] & k) != 0);
          k = k >> 1;
        }
        RPU_SetDisplay(CurDisplay, DisplayDIP[CurDisplay], false);
      }
      else if (xDisplay == 4) {
        DisplayDIP[4] = 10 * (dipBankVal[1] >= 128) + (dipBankVal[0] >= 128);
        RPU_SetDisplayBallInPlay(DisplayDIP[4]);
      }
      else {
        DisplayDIP[5] = 10 * (dipBankVal[3] >= 128) + (dipBankVal[2] >= 128);
        RPU_SetDisplayCredits(DisplayDIP[5]);
      }
    }

    if (xDisplay < 4) // set mask for flashing digit
      RPU_SetDigitFlash(CurDisplay, CurDigit, DisplayDIP[CurDisplay], CurrentTime, 250);
    else if (xDisplay == 4) 
      RPU_SetDigitFlashBallInPlay(xDigit, CurrentTime, 250);
    else
      RPU_SetDigitFlashCredits(xDigit, CurrentTime, 250);

  } else if (curState==MACHINE_STATE_TEST_SCORE_LEVEL_1) { //                                             *** Set Score Level 1 ***
    #ifdef RPU_OS_USE_S_AND_T
      RPU_PlaySoundSAndT(5); // Sound off
      #endif
    savedScoreStartByte = RPU_AWARD_SCORE_1_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_SCORE_LEVEL_2) { //                                             *** Set Score Level 2 ***
    savedScoreStartByte = RPU_AWARD_SCORE_2_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_SCORE_LEVEL_3) { //                                             *** Set Score Level 3 ***
    savedScoreStartByte = RPU_AWARD_SCORE_3_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_HISCR) { //                                                     *** Set High Score ***
    savedScoreStartByte = RPU_HIGHSCORE_EEPROM_START_BYTE;
    } else if (curState==MACHINE_STATE_TEST_PERSONAL_GOAL) {
    savedScoreStartByte = RPU_PERSONAL_GOAL_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_CREDITS) { //                                                   *** Set Credits ***
    if (curStateChanged) {
      SavedValue = RPU_ReadByteFromEEProm(RPU_CREDITS_EEPROM_BYTE);
      RPU_SetDisplay(0, SavedValue, true);
      LastSolTestTime = CurrentTime;
    }
    if (curSwitch==resetSwitch || resetDoubleClick || (ResetHold && CurrentTime > LastSolTestTime + 250)) {
      SavedValue += 1;
      LastSolTestTime = CurrentTime;
      // if (SavedValue>40) SavedValue = 0;
      if (resetDoubleClick) SavedValue = 0;
      RPU_SetDisplay(0, SavedValue, true);
      RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, SavedValue & 0x000000FF);
    }
  } else if (curState==MACHINE_STATE_TEST_TOTAL_PLAYS) { //                                               *** Set Total Plays ***
    auditNumStartByte = RPU_TOTAL_PLAYS_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_TOTAL_REPLAYS) { //                                             *** Set Total Replays ***
    auditNumStartByte = RPU_TOTAL_REPLAYS_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_HISCR_BEAT) { //                                                *** Set High Scores Won ***
    auditNumStartByte = RPU_TOTAL_HISCORE_BEATEN_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_COIN_CHUTES) { //                                             *** Set Chute 2 ***
    EnterCoinChuteData(curSwitch, resetDoubleClick, resetBeingHeld, curStateChanged, CurrentTime, resetSwitch, otherSwitch, endSwitch);
  } 

  if (savedScoreStartByte) {
    if (curStateChanged) {
      SavedValue = RPU_ReadULFromEEProm(savedScoreStartByte);
      RPU_SetDisplay(0, SavedValue, true);  
    }

    if (curSwitch==resetSwitch) {
      SavedValue += 1000;
      RPU_SetDisplay(0, SavedValue, true);  
      RPU_WriteULToEEProm(savedScoreStartByte, SavedValue);
    }

    if (resetBeingHeld && (CurrentTime>=NextSpeedyValueChange)) {
      SavedValue += 1000;
      RPU_SetDisplay(0, SavedValue, true);  
      if (NumSpeedyChanges<6) NextSpeedyValueChange = CurrentTime + 400;
      else if (NumSpeedyChanges<50) NextSpeedyValueChange = CurrentTime + 50;
      else NextSpeedyValueChange = CurrentTime + 10;
      NumSpeedyChanges += 1;
    }

    if (!resetBeingHeld && NumSpeedyChanges>0) {
      RPU_WriteULToEEProm(savedScoreStartByte, SavedValue);
      NumSpeedyChanges = 0;
    }
    
    if (resetDoubleClick) {
      SavedValue = 0;
      RPU_SetDisplay(0, SavedValue, true);  
      RPU_WriteULToEEProm(savedScoreStartByte, SavedValue);
    }
  }

  if (auditNumStartByte) {
    if (curStateChanged) {
      SavedValue = RPU_ReadULFromEEProm(auditNumStartByte);
      RPU_SetDisplay(0, SavedValue, true);
      LastSolTestTime = CurrentTime;
    }
    if (curSwitch == resetSwitch || (ResetHold && CurrentTime > LastSolTestTime + 250)) {
      SavedValue +=1;
      LastSolTestTime = CurrentTime;
      RPU_SetDisplay(0, SavedValue, true);  
      RPU_WriteULToEEProm(auditNumStartByte, SavedValue);
    }
    if (resetDoubleClick) {
      SavedValue = 0;
      RPU_SetDisplay(0, SavedValue, true);  
      RPU_WriteULToEEProm(auditNumStartByte, SavedValue);
    }
  }
  
  return returnState;
}

// #################### ENTER BALL SAVE DATA USING DISPLAYS ####################
void EnterCoinChuteData(byte curSwitch,boolean resetDoubleClick, boolean resetBeingHeld, boolean curStateChanged, 
                                          unsigned long CurrentTime, byte resetSwitch, byte otherSwitch, byte endSwitch) {

  if (curStateChanged) {
    dispNum = 0;
    LastSolTestTime = CurrentTime;
    SavedValue = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE);
    RPU_SetDisplayFlash(0, SavedValue, CurrentTime, 250, 2);
    SavedValue = RPU_ReadULFromEEProm(RPU_CHUTE_2_COINS_START_BYTE);
    RPU_SetDisplay(1, SavedValue, true, 2);
    SavedValue = RPU_ReadULFromEEProm(RPU_CHUTE_3_COINS_START_BYTE);
    RPU_SetDisplay(2, SavedValue, true, 2);
    }
  SavedValue = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE + dispNum * 4);
  RPU_SetDisplayFlash(dispNum, SavedValue, CurrentTime, 250, 2);
  if (curSwitch == otherSwitch) {
    RPU_SetDisplay(dispNum, SavedValue, true, 2);
    dispNum = (dispNum + 1) % 3;
    SavedValue = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE + dispNum * 4);
    RPU_SetDisplayFlash(dispNum, SavedValue, CurrentTime, 250, 2);
    } 
    if (resetDoubleClick) {
    SavedValue = 0;
    RPU_SetDisplayFlash(dispNum, SavedValue, CurrentTime, 250, 2); 
    RPU_WriteULToEEProm(RPU_CHUTE_1_COINS_START_BYTE + dispNum * 4, SavedValue);
    }
    if (curSwitch == resetSwitch || (ResetHold && CurrentTime > LastSolTestTime + 250)) {
      SavedValue += 1;
      LastSolTestTime = CurrentTime;
      RPU_SetDisplayFlash(dispNum, SavedValue, CurrentTime, 250, 2); 
      RPU_WriteULToEEProm(RPU_CHUTE_1_COINS_START_BYTE + dispNum * 4, SavedValue);
    }
}



unsigned long GetLastSelfTestChangedTime() {
  return LastSelfTestChange;
}


void SetLastSelfTestChangedTime(unsigned long setSelfTestChange) {
  LastSelfTestChange = setSelfTestChange;
}
