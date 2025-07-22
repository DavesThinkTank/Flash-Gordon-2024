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

     Version 2024.08 by Dave's Think Tank

  - Added otherSwitch as a secondary input to self-tests. Changed slamSwitch to endSwitch.
  - DIP Switch Test: Added a test for DIP switches (-6). 

    Version 2024.12 by Dave's Think Tank

    - Cleaned up code by removing old, unused CPC (coins per credit) code, unused GetAwardScore() function definition.

Version 2025.01 by Dave's Think Tank

    - Added test for switch bounce (-5), in addition to stuck switches (-4).

 */

// Self-Test Machine States (See also FGyyyypmm.h)
#define MACHINE_STATE_TEST_LAMPS           -1
#define MACHINE_STATE_TEST_DISPLAYS        -2
#define MACHINE_STATE_TEST_SOLENOIDS       -3
#define MACHINE_STATE_TEST_STUCK_SWITCHES  -4
#define MACHINE_STATE_TEST_SWITCH_BOUNCE   -5
#define MACHINE_STATE_TEST_SOUNDS          -6
#define MACHINE_STATE_TEST_DIP_SWITCHES    -7
#define MACHINE_STATE_TEST_SCORE_LEVEL_1   -8
#define MACHINE_STATE_TEST_SCORE_LEVEL_2   -9
#define MACHINE_STATE_TEST_SCORE_LEVEL_3  -10
#define MACHINE_STATE_TEST_HISCR          -11
#define MACHINE_STATE_TEST_PERSONAL_GOAL  -12
#define MACHINE_STATE_TEST_CREDITS        -13
#define MACHINE_STATE_TEST_TOTAL_PLAYS    -14
#define MACHINE_STATE_TEST_TOTAL_REPLAYS  -15
#define MACHINE_STATE_TEST_HISCR_BEAT     -16
#define MACHINE_STATE_TEST_CHUTE_2_COINS  -17
#define MACHINE_STATE_TEST_CHUTE_1_COINS  -18
#define MACHINE_STATE_TEST_CHUTE_3_COINS  -19

#define MACHINE_STATE_TEST_DONE           -19

unsigned long GetLastSelfTestChangedTime();
void SetLastSelfTestChangedTime(unsigned long setSelfTestChange);
int RunBaseSelfTest(int curState, boolean curStateChanged, unsigned long CurrentTime, byte resetSwitch, byte otherSwitch, byte endSwitch);
