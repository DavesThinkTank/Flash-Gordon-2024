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

    - Minor changes to ensure appropriate function in a Flash Gordon.
 */

// #define ENABLE_CPC_SETTINGS           1 // Do not use! Replaced with dipChute1 to 3, CreditsperCoin, and CoinsperCredit in main program.

#define MACHINE_STATE_TEST_LIGHTS          -1
#define MACHINE_STATE_TEST_DISPLAYS        -2
#define MACHINE_STATE_TEST_SOLENOIDS       -3
#define MACHINE_STATE_TEST_SWITCHES        -4
#define MACHINE_STATE_TEST_SOUNDS          -5
#define MACHINE_STATE_TEST_SCORE_LEVEL_1   -6
#define MACHINE_STATE_TEST_SCORE_LEVEL_2   -7
#define MACHINE_STATE_TEST_SCORE_LEVEL_3   -8
#define MACHINE_STATE_TEST_HISCR           -9
#define MACHINE_STATE_TEST_PERSONAL_GOAL  -10
#define MACHINE_STATE_TEST_CREDITS        -11
#define MACHINE_STATE_TEST_TOTAL_PLAYS    -12
#define MACHINE_STATE_TEST_TOTAL_REPLAYS  -13
#define MACHINE_STATE_TEST_HISCR_BEAT     -14
#define MACHINE_STATE_TEST_CHUTE_2_COINS  -15
#define MACHINE_STATE_TEST_CHUTE_1_COINS  -16
#define MACHINE_STATE_TEST_CHUTE_3_COINS  -17
#define MACHINE_STATE_ADJUST_CPC_CHUTE_1  -18
#define MACHINE_STATE_ADJUST_CPC_CHUTE_2  -19
#define MACHINE_STATE_ADJUST_CPC_CHUTE_3  -20

#define MACHINE_STATE_TEST_DONE           -20

unsigned long GetLastSelfTestChangedTime();
void SetLastSelfTestChangedTime(unsigned long setSelfTestChange);
int RunBaseSelfTest(int curState, boolean curStateChanged, unsigned long CurrentTime, byte resetSwitch, byte slamSwitch=0xFF);

unsigned long GetAwardScore(byte level);

#ifdef ENABLE_CPC_SETTINGS
byte GetCPCSelection(byte chuteNumber);
byte GetCPCCoins(byte cpcSelection);
byte GetCPCCredits(byte cpcSelection);
#endif 

