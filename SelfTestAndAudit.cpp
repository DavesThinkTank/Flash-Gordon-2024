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
 */

#include <Arduino.h>
#include "SelfTestAndAudit.h"
#include "BSOS_Config.h"
#include "BallySternOS.h"

#define MACHINE_STATE_ATTRACT         0
//#define USE_SB100

unsigned long LastSolTestTime = 0; 
unsigned long LastSelfTestChange = 0;
unsigned long SavedValue = 0;
unsigned long ResetHold = 0;
unsigned long NextSpeedyValueChange = 0;
unsigned long NumSpeedyChanges = 0;
unsigned long LastResetPress = 0;
byte CurValue = 0;
boolean SoundPlayed = false;
byte SoundPlaying = 0;
byte SoundToPlay = 0;
boolean SolenoidCycle = true;

#ifdef ENABLE_CPC_SETTINGS
boolean CPCSelectionsHaveBeenRead = false;
#define NUM_CPC_PAIRS 9
byte CPCPairs[NUM_CPC_PAIRS][2] = { // Credits per Coin, e.g. 5 credits per 1 coins on first line
  {1, 5},
  {1, 4},
  {1, 3},
  {1, 2},
  {1, 1},
  {2, 3},
  {2, 1},
  {3, 1},
  {4, 1}
};
byte CPCSelection[3];

byte GetCPCSelection(byte chuteNumber) {
  if (chuteNumber>2) return 0xFF;

  if (CPCSelectionsHaveBeenRead==false) {
    CPCSelection[0] = BSOS_ReadByteFromEEProm(BSOS_CPC_CHUTE_1_SELECTION_BYTE);
    if (CPCSelection[0]>=NUM_CPC_PAIRS) {
      CPCSelection[0] = 4;
      BSOS_WriteByteToEEProm(BSOS_CPC_CHUTE_1_SELECTION_BYTE, 4);
    }
    CPCSelection[1] = BSOS_ReadByteFromEEProm(BSOS_CPC_CHUTE_2_SELECTION_BYTE);  
    if (CPCSelection[1]>=NUM_CPC_PAIRS) {
      CPCSelection[1] = 4;
      BSOS_WriteByteToEEProm(BSOS_CPC_CHUTE_2_SELECTION_BYTE, 4);
    }
    CPCSelection[2] = BSOS_ReadByteFromEEProm(BSOS_CPC_CHUTE_3_SELECTION_BYTE);  
    if (CPCSelection[2]>=NUM_CPC_PAIRS) {
      CPCSelection[2] = 4;
      BSOS_WriteByteToEEProm(BSOS_CPC_CHUTE_3_SELECTION_BYTE, 4);
    }
    CPCSelectionsHaveBeenRead = true;
  }
  
  return CPCSelection[chuteNumber];
}


byte GetCPCCoins(byte cpcSelection) {
  if (cpcSelection>=NUM_CPC_PAIRS) return 1;
  return CPCPairs[cpcSelection][0];
}


byte GetCPCCredits(byte cpcSelection) {
  if (cpcSelection>=NUM_CPC_PAIRS) return 1;
  return CPCPairs[cpcSelection][1];
}

#endif


int RunBaseSelfTest(int curState, boolean curStateChanged, unsigned long CurrentTime, byte resetSwitch, byte slamSwitch) {

  byte curSwitch = BSOS_PullFirstFromSwitchStack();
  int returnState = curState;
  boolean resetDoubleClick = false;
  unsigned short savedScoreStartByte = 0;
  unsigned short auditNumStartByte = 0;

#ifdef ENABLE_CPC_SETTINGS
  unsigned short cpcSelectorStartByte = 0;
#endif

  if (curSwitch==resetSwitch) {
    ResetHold = CurrentTime;
    if ((CurrentTime-LastResetPress)<400) {
      resetDoubleClick = true;
      curSwitch = SWITCH_STACK_EMPTY;
    }
    LastResetPress = CurrentTime;
  }

  if (ResetHold!=0 && !BSOS_ReadSingleSwitchState(resetSwitch)) {
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

  if (slamSwitch!=0xFF && curSwitch==slamSwitch) {
    returnState = MACHINE_STATE_ATTRACT;
  }
  
  if (curSwitch==SW_SELF_TEST_SWITCH && (CurrentTime-LastSelfTestChange)>250) {
    returnState -= 1;
//    if (returnState==MACHINE_STATE_TEST_DONE) returnState = MACHINE_STATE_ATTRACT;
    LastSelfTestChange = CurrentTime;
  }

  if (curStateChanged) {
//    BSOS_SetCoinLockout(false);
    
    for (int count=0; count<4; count++) {
      BSOS_SetDisplay(count, 0);
      BSOS_SetDisplayBlank(count, 0x00);        
    }

    if (curState<=MACHINE_STATE_TEST_SCORE_LEVEL_1) {
      BSOS_SetDisplayCredits(MACHINE_STATE_TEST_SOUNDS-curState);
      BSOS_SetDisplayBallInPlay(0, false);
    }
  }

  if (curState==MACHINE_STATE_TEST_LIGHTS) {
    if (curStateChanged) {
      BSOS_DisableSolenoidStack();        
      BSOS_SetDisableFlippers(true);
      BSOS_SetDisplayCredits(0);
      BSOS_SetDisplayBallInPlay(1);
      BSOS_TurnOffAllLamps();
      for (int count=0; count<BSOS_MAX_LAMPS; count++) {
        BSOS_SetLampState(count, 1, 0, 500);
      }
      CurValue = 99;
      BSOS_SetDisplay(0, CurValue, true);  
    }
    if (curSwitch==resetSwitch || resetDoubleClick) {
      CurValue += 1;
      if (CurValue>99) CurValue = 0;
      if (CurValue==BSOS_MAX_LAMPS) {
        CurValue = 99;
        for (int count=0; count<BSOS_MAX_LAMPS; count++) {
          BSOS_SetLampState(count, 1, 0, 500);
        }
      } else {
        BSOS_TurnOffAllLamps();
        BSOS_SetLampState(CurValue, 1, 0, 0);
      }      
      BSOS_SetDisplay(0, CurValue, true);  
    }    
  } else if (curState==MACHINE_STATE_TEST_DISPLAYS) {
    if (curStateChanged) {
      BSOS_TurnOffAllLamps();
      BSOS_SetDisplayCredits(0);
      BSOS_SetDisplayBallInPlay(2);
      for (int count=0; count<4; count++) {
        BSOS_SetDisplayBlank(count, 0x3F);        
      }
      CurValue = 0;
    }
    if (curSwitch==resetSwitch || resetDoubleClick) {
      CurValue += 1;
      #ifdef BALLY_STERN_OS_USE_7_DIGIT_DISPLAYS
        if (CurValue>34) CurValue = 0;
      #else
        if (CurValue>30) CurValue = 0;
      #endif
    }    
    BSOS_CycleAllDisplays(CurrentTime, CurValue);
  } else if (curState==MACHINE_STATE_TEST_SOLENOIDS) {
    if (curStateChanged) {
      BSOS_TurnOffAllLamps();
      LastSolTestTime = CurrentTime;
      BSOS_EnableSolenoidStack(); 
      BSOS_SetDisableFlippers(false);
      BSOS_SetDisplayBlank(4, 0);
      BSOS_SetDisplayCredits(0);
      BSOS_SetDisplayBallInPlay(3);
      SolenoidCycle = true;
      SavedValue = 0;
      BSOS_PushToSolenoidStack(SavedValue, 5);
    } 
    if (curSwitch==resetSwitch || resetDoubleClick) {
      SolenoidCycle = (SolenoidCycle) ? false : true;
    }

    if ((CurrentTime-LastSolTestTime)>1000) {
      if (SolenoidCycle) {
        SavedValue += 1;
        if (SavedValue>14) SavedValue = 0;
      }
      BSOS_PushToSolenoidStack(SavedValue, 3);
      BSOS_SetDisplay(0, SavedValue, true);
      LastSolTestTime = CurrentTime;
    }
    
  } else if (curState==MACHINE_STATE_TEST_SWITCHES) {
    if (curStateChanged) {
      BSOS_TurnOffAllLamps();
      BSOS_DisableSolenoidStack(); 
      BSOS_SetDisableFlippers(true);
      BSOS_SetDisplayCredits(0);
      BSOS_SetDisplayBallInPlay(4);
    }

    byte displayOutput = 0;
    for (byte switchCount=0; switchCount<40 && displayOutput<4; switchCount++) {
      if (BSOS_ReadSingleSwitchState(switchCount)) {
        BSOS_SetDisplay(displayOutput, switchCount, true);
        displayOutput += 1;
      }
    }

    if (displayOutput<4) {
      for (int count=displayOutput; count<4; count++) {
        BSOS_SetDisplayBlank(count, 0x00);
      }
    }

  } else if (curState==MACHINE_STATE_TEST_SOUNDS) {
    if (curStateChanged) {
      // BSOS_TurnOffAllLamps();
      BSOS_SetDisplayCredits(0);
      BSOS_SetDisplayBallInPlay(5);
      SolenoidCycle = true;
      SoundToPlay = 6; // Start with sound 6. This jumps immediately to sound 7, avoiding the continuous background, then continues up.
      // BSOS_PlaySoundSquawkAndTalk(SoundToPlay);
      SoundPlaying = SoundToPlay;
      SoundPlayed = true;
      // BSOS_SetDisplay(0, (unsigned long)SoundToPlay, true);
      LastSolTestTime = CurrentTime - 5000; // Time the sound started to play (5 seconds ago)
    } 

#ifdef USE_SB100
    SoundToPlay = 0x01 << (((CurrentTime-LastSelfTestChange)/750)%8);
    if (SoundPlaying!=SoundToPlay) {
      BSOS_PlaySB100(SoundToPlay);
      SoundPlaying = SoundToPlay;
      BSOS_SetDisplay(0, (unsigned long)SoundToPlay, true);
      LastSolTestTime = CurrentTime; // Time the sound started to play
    }
    // If the sound play call was more than 300ms ago, turn it off
//    if ((CurrentTime-LastSolTestTime)>300) BSOS_PlaySB100(128);

#elif defined (BALLY_STERN_OS_USE_SQUAWK_AND_TALK)
    if (resetBeingHeld && (CurrentTime - LastSolTestTime > 250)) {
      SoundToPlay += 1;
      SoundPlayed = false;
      BSOS_SetDisplay(0,(unsigned long) SoundToPlay, true);
      LastSolTestTime = CurrentTime;
      SolenoidCycle = true;
    }
    else {
      if (curSwitch==resetSwitch || resetDoubleClick) {
//      SolenoidCycle = (SolenoidCycle) ? false : true;
        if (CurrentTime - LastSolTestTime <= 500) { // Allow 0.5 seconds to click and move forward without playing sound
          SoundToPlay +=1;
          if (SoundToPlay > 255) SoundToPlay = 0;
          BSOS_SetDisplay(0, (unsigned long)SoundToPlay, true);
          LastSolTestTime = CurrentTime - 500;
          }
        else {
          SolenoidCycle = !SolenoidCycle;
          }
        }
    if ((CurrentTime - LastSolTestTime) >= 500 && !SoundPlayed) {
#ifndef BALLY_STERN_OS_USE_GEETEOH 
        BSOS_PlaySoundSquawkAndTalk(5); // Sound off
        BSOS_PlaySoundSquawkAndTalk(SoundToPlay);
#else
        if (SoundToPlay != 5) BSOS_PlaySoundSquawkAndTalk(SoundToPlay); // Geeteoh has eliminated sound 5, but sometimes has trouble because of it
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
        BSOS_SetDisplay(0, (unsigned long)SoundToPlay, true);
      }
    }

#elif defined (BALLY_STERN_OS_USE_DASH51) 
    soundToPlay = ((CurrentTime-LastSelfTestChange)/2000)%32;
    if (SoundPlaying!=soundToPlay) {
      if (soundToPlay==17) SoundToPlay = 0;
      BSOS_PlaySoundDash51(SoundToPlay);
      SoundPlaying = SoundToPlay;
      BSOS_SetDisplay(0, (unsigned long)soundToPlay, true);
      LastSolTestTime = CurrentTime; // Time the sound started to play
    }
#endif

  } else if (curState==MACHINE_STATE_TEST_SCORE_LEVEL_1) {
#ifdef USE_SB100    
    if (curStateChanged) BSOS_PlaySB100(0);
#endif
    savedScoreStartByte = BSOS_AWARD_SCORE_1_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_SCORE_LEVEL_2) {
    savedScoreStartByte = BSOS_AWARD_SCORE_2_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_SCORE_LEVEL_3) {
    savedScoreStartByte = BSOS_AWARD_SCORE_3_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_HISCR) {
    savedScoreStartByte = BSOS_HIGHSCORE_EEPROM_START_BYTE;
    } else if (curState==MACHINE_STATE_TEST_PERSONAL_GOAL) {
    savedScoreStartByte = BSOS_PERSONAL_GOAL_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_CREDITS) {
    if (curStateChanged) {
      SavedValue = BSOS_ReadByteFromEEProm(BSOS_CREDITS_EEPROM_BYTE);
      BSOS_SetDisplay(0, SavedValue, true);
    }
    if (curSwitch==resetSwitch || resetDoubleClick || resetBeingHeld) {
      SavedValue += 1;
      ResetHold = CurrentTime;
      // if (SavedValue>40) SavedValue = 0;
      if (resetDoubleClick) SavedValue = 0;
      BSOS_SetDisplay(0, SavedValue, true);
      BSOS_WriteByteToEEProm(BSOS_CREDITS_EEPROM_BYTE, SavedValue & 0x000000FF);
    }
  } else if (curState==MACHINE_STATE_TEST_TOTAL_PLAYS) {
    auditNumStartByte = BSOS_TOTAL_PLAYS_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_TOTAL_REPLAYS) {
    auditNumStartByte = BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_HISCR_BEAT) {
    auditNumStartByte = BSOS_TOTAL_HISCORE_BEATEN_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_CHUTE_2_COINS) {
    auditNumStartByte = BSOS_CHUTE_2_COINS_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_CHUTE_1_COINS) {
    auditNumStartByte = BSOS_CHUTE_1_COINS_START_BYTE;
  } else if (curState==MACHINE_STATE_TEST_CHUTE_3_COINS) {
    auditNumStartByte = BSOS_CHUTE_3_COINS_START_BYTE;    
  } 
#ifdef ENABLE_CPC_SETTINGS  
    else if (curState==MACHINE_STATE_ADJUST_CPC_CHUTE_1) {
    cpcSelectorStartByte = BSOS_CPC_CHUTE_1_SELECTION_BYTE;
  } else if (curState==MACHINE_STATE_ADJUST_CPC_CHUTE_2) {
    cpcSelectorStartByte = BSOS_CPC_CHUTE_2_SELECTION_BYTE;
  } else if (curState==MACHINE_STATE_ADJUST_CPC_CHUTE_3) {
    cpcSelectorStartByte = BSOS_CPC_CHUTE_3_SELECTION_BYTE;
  }
#else
  else returnState = curState - 1;
#endif  

  if (savedScoreStartByte) {
    if (curStateChanged) {
      SavedValue = BSOS_ReadULFromEEProm(savedScoreStartByte);
      BSOS_SetDisplay(0, SavedValue, true);  
    }

    if (curSwitch==resetSwitch) {
      SavedValue += 1000;
      BSOS_SetDisplay(0, SavedValue, true);  
      BSOS_WriteULToEEProm(savedScoreStartByte, SavedValue);
    }

    if (resetBeingHeld && (CurrentTime>=NextSpeedyValueChange)) {
      SavedValue += 1000;
      BSOS_SetDisplay(0, SavedValue, true);  
      if (NumSpeedyChanges<6) NextSpeedyValueChange = CurrentTime + 400;
      else if (NumSpeedyChanges<50) NextSpeedyValueChange = CurrentTime + 50;
      else NextSpeedyValueChange = CurrentTime + 10;
      NumSpeedyChanges += 1;
    }

    if (!resetBeingHeld && NumSpeedyChanges>0) {
      BSOS_WriteULToEEProm(savedScoreStartByte, SavedValue);
      NumSpeedyChanges = 0;
    }
    
    if (resetDoubleClick) {
      SavedValue = 0;
      BSOS_SetDisplay(0, SavedValue, true);  
      BSOS_WriteULToEEProm(savedScoreStartByte, SavedValue);
    }
  }

  if (auditNumStartByte) {
    if (curStateChanged) {
      SavedValue = BSOS_ReadULFromEEProm(auditNumStartByte);
      BSOS_SetDisplay(0, SavedValue, true);
    }
    if (curSwitch == resetSwitch || resetBeingHeld) {
      SavedValue +=1;
      ResetHold = CurrentTime;
      BSOS_SetDisplay(0, SavedValue, true);  
      BSOS_WriteULToEEProm(auditNumStartByte, SavedValue);
    }
    if (resetDoubleClick) {
      SavedValue = 0;
      BSOS_SetDisplay(0, SavedValue, true);  
      BSOS_WriteULToEEProm(auditNumStartByte, SavedValue);
    }
    
  }

#ifdef ENABLE_CPC_SETTINGS
  if (cpcSelectorStartByte) {
    if (curStateChanged) {
      SavedValue = BSOS_ReadByteFromEEProm(cpcSelectorStartByte);
      if (SavedValue>NUM_CPC_PAIRS) SavedValue = 4;
      BSOS_SetDisplay(0, CPCPairs[SavedValue][0], true);
      BSOS_SetDisplay(1, CPCPairs[SavedValue][1], true);
    }

    if (curSwitch==resetSwitch) {
      byte lastValue = (byte)SavedValue;
//      if (BSOS_GetUpDownSwitchState()) { 
        SavedValue += 1;
        if (SavedValue>=NUM_CPC_PAIRS) SavedValue = (NUM_CPC_PAIRS-1);
//      } else {
//        if (SavedValue>0) SavedValue -= 1;
//      }
      BSOS_SetDisplay(0, CPCPairs[SavedValue][0], true);
      BSOS_SetDisplay(1, CPCPairs[SavedValue][1], true);
      if (lastValue!=SavedValue) {
        BSOS_WriteByteToEEProm(cpcSelectorStartByte, (byte)SavedValue);
        if (cpcSelectorStartByte==BSOS_CPC_CHUTE_1_SELECTION_BYTE) CPCSelection[0] = (byte)SavedValue;
        else if (cpcSelectorStartByte==BSOS_CPC_CHUTE_2_SELECTION_BYTE) CPCSelection[1] = (byte)SavedValue;
        else if (cpcSelectorStartByte==BSOS_CPC_CHUTE_3_SELECTION_BYTE) CPCSelection[2] = (byte)SavedValue;
      }
    }
  }
#endif
  
  return returnState;
}

unsigned long GetLastSelfTestChangedTime() {
  return LastSelfTestChange;
}


void SetLastSelfTestChangedTime(unsigned long setSelfTestChange) {
  LastSelfTestChange = setSelfTestChange;
}
