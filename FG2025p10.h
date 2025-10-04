/**************************************************************************
  This file is part of Flash Gordon 2021 - Mega

  Version: 1.0.0

  I, Tim Murren, the author of this program disclaim all copyright
  in order to make this program freely available in perpetuity to
  anyone who would like to use it. Tim Murren, 2/5/2021

  Flash Gordon 2021 is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Flash Gordon 2021 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  See <https://www.gnu.org/licenses/>.


  Version 2024.04 by Dave's Think Tank

  - Minor changes to ensure appropriate function in a Flash Gordon.

Version 2024.06 by Dave's Think Tank

  - Added GeeteohTimings variable to better control sound with a Geeteoh sound board.

Version 2025.08 by Dave's Think Tank

  - Added "Savior of the Universe" sound (#51).

Version 2025.09 by Dave's Think Tank

- GeeteohTimings renamed SoundData, to better represent its purpose in the program. SoundData extended to
   include length of original sound files used by the S&T.

Version 2025.10 by Dave's Think Tank

- Sound File Groups added at bottom following SoundData. Includes groups for saucer voices.

*/

// These are example values - fill these in with your machine's definitions
#define NUM_OF_LAMPS                      77 // 60 before aux

// Lamp Board (sort by schematics)
#define LA_BONUS_MINI_1K                  0 // 1K MINI BONUS
#define LA_BONUS_MINI_2K                  1 // 2K MINI BONUS
#define LA_BONUS_MINI_3K                  2 // 3K MINI BONUS
#define LA_BONUS_MINI_4K                  3 // 4K MINI BONUS
#define LA_BONUS_MINI_5K                  4 // 5K MINI BONUS
#define LA_BONUS_MINI_6K                  5 // 6K MINI BONUS
#define LA_BONUS_MINI_7K                  6 // 7K MINI BONUS
#define LA_BONUS_MINI_8K                  7 // 8K MINI BONUS
#define LA_BONUS_MINI_9K                  8 // 9K MINI BONUS
#define LA_BONUS_MINI_10K                 9 // 10K MINI BONUS
#define LA_SPINNER_RIGHT                  10 // RIGHT SPINNER
#define LA_SPINNER_LEFT                   11 // LEFT SPINNER
#define LA_BONUS_SUPER_1K                 12 // 1K SUPER BONUS
#define LA_BONUS_SUPER_2K                 13 // 2K SUPER BONUS
#define LA_BONUS_SUPER_3K                 14 // 3K SUPER BONUS
#define LA_BONUS_SUPER_4K                 15 // 4K SUPER BONUS
#define LA_BONUS_SUPER_5K                 16 // 5K SUPER BONUS
#define LA_BONUS_SUPER_6K                 17 // 6K SUPER BONUS
#define LA_BONUS_SUPER_7K                 18 // 7K SUPER BONUS
#define LA_BONUS_SUPER_8K                 19 // 8K SUPER BONUS
#define LA_BONUS_SUPER_9K                 20 // 9K SUPER BONUS
#define LA_BONUS_SUPER_10K                21 // 10K SUPER BONUS
#define LA_BONUS_MINI_50K                 22 // 50K MINI BONUS
#define LA_BONUS_SUPER_100K               23 // 100K SUPER BONUS
#define LA_BONUS_2X                       24 // 2X BONUS
#define LA_BONUS_3X                       25 // 3X BONUS
#define LA_BONUS_4X                       26 // 4X BONUS
#define LA_BONUS_5X                       27 // 5X BONUS
#define LA_DTARGET_ARROW_1                28 // #1 DROP TARGET ARROW
#define LA_DTARGET_ARROW_2                29 // #2 DROP TARGET ARROW
#define LA_DTARGET_ARROW_3                30 // #3 DROP TARGET ARROW
#define LA_DTARGET_BONUS_4X               31 // 4X 3 DROP TARGET
#define LA_DTARGET_4_D                    32 // 4 DROP TARGET "A" (BOTTOM)
#define LA_DTARGET_4_C                    33 // 4 DROP TARGET "B"
#define LA_DTARGET_4_B                    34 // 4 DROP TARGET "C"
#define LA_DTARGET_4_A                    35 // 4 DROP TARGET "D" (TOP)
#define LA_TARGET_LRIGHT_BOTTOM           36 // RT. SIDE LOWER TARGET
#define LA_INLANE_RIGHT                   37 // FLIPPER FEED LANE (rt.)
#define LA_INLANE_LEFT                    38 // FLIPPER FEED LANE (LEFT)
#define LA_TARGET_LRIGHT_TOP              39 // RT. SIDE UPPER TARGET
#define LA_SAME_PLAYER_SHOOTS_AGAIN       40 // SAME PLAYER SHOOTS AGAIN
#define LA_MATCH                          41 // MATCH
#define LA_SHOOT_AGAIN                    42 // SHOOT AGAIN
#define LA_CREDIT_INDICATOR               43 // CREDIT INDICATOR
#define LA_SAUCER_10K                     44 // 10K SAUCER
#define LA_SAUCER_20K                     45 // 20K SAUCER - SAUCER_20K
#define LA_SAUCER_XBALL                   46 // X-BALL SAUCCER
#define LA_DTARGET_BONUS_5X               47 // 5X 4 DROP TARGET
#define LA_BALL_IN_PLAY                   48 // BALL IN PLAY
#define LA_HIGH_SCORE_TO_DATE             49 // HIGH SCORE TO DATE
#define LA_GAME_OVER                      50 // GAME OVER
#define LA_TILT                           51 // TILT
#define LA_POP_TOP                        52 // TOP THUMPER BUMPER
#define LA_TARGET_WOOD_BEAST_XBALL        53 // INLINE LA_DROP TARGET X-BALL
#define LA_SAUCER_30K                     54 // 30K SAUCER
#define LA_TARGET_UPPER_COLLECT_BONUS     55 // TOP TAR. COLLECT BONUS
#define LA_OUTLANE_RIGHT_SPECIAL          56 // RT. OUT SPECIAL
#define LA_OUTLANE_LEFT_SPECIAL           57 // LEFT OUT SPECIAL
#define LA_STAR_PFIELD_BOTTOM             58 // LOWER TOP RT. RO. BUTTON
#define LA_TARGET_UPPER_SPECIAL           59 // TOP TAR. SPECIAL

// Aux Lamp Board (sorted by schematics)
#define LA_STAR_PFIELD_TOP                60 // UPPER TOP RT. RO. BUTTON
#define LA_STAR_SHOOTER_TOP               61 // TOP SHOOTER ALLEY RO. BUTTON
#define LA_STAR_SHOOTER_MIDDLE            62 // MIDDLE SHOOTER ALLEY RO. BUTTON
#define LA_STAR_SHOOTER_BOTTOM            63 // LOWER SHOOTER ALLEY RO. BUTTON
#define LA_FLASH_GORDON_1                 64 // 1 FLASH GORDON
#define LA_FLASH_GORDON_2                 65 // 2 FLASH GORDON
#define LA_FLASH_GORDON_3                 66 // 3 FLASH GORDON
// #define N/A                            67 // not used
#define LA_FLASH_GORDON_4                 68 // 4 FLASH GORDON
#define LA_FLASH_GORDON_5                 69 // 5 FLASH GORDON
#define LA_FLASH_GORDON_6                 70 // 6 FLASH GORDON
// #define N/A                            71 // not used
#define LA_MING_BOTTOM                    72 // FACE OF MING
#define LA_MING_TOP                       73 // FACE OF MING
// #define N/A                            74 // not used
#define LA_FLASH_STROBE                   75 // BACK BOX STROBE
#define LA_CLOCK_15_SECONDS_3X            76 // 3X 15 SECOND CLOCK
#define LA_CLOCK_15_SECONDS_2X            77 // 2X 15 SECOND CLOCK
#define LA_SAUCER_ARROW_3X                78 // 3X SAUCER ARROW
#define LA_SAUCER_ARROW_2X                79 // 2X SAUCER ARROW
// #define N/A                            80 // not used
// #define N/A                            81 // not used
// #define N/A                            82 // not used
// #define N/A                            83 // not used
// #define N/A                            84 // not used
// #define N/A                            85 // not used
// #define N/A                            86 // not used
// #define N/A                            87 // not used

#define NUM_OF_SWITCHES             39 // 40 with one not used

#define SW_STARS_PFIELD             0 // LEFT & RIGHT R.O. BUTTONS (2 + 2)
#define SW_STARS_SHOOTER_LANE       1 // SHOOTER LANE R.0. BUTTONS (3)
#define SW_DTARGET_1                2 // TOP SINGLE DROP TARGET
#define SW_SHOOTER_LANE_ROLL        3 // SHOOTER LANE ROLLOVER
#define SW_DTARGET_REBOUND          4 // DROP TARGET 50 POINT REBOUND (2)
#define SW_CREDIT_BUTTON            5 // CREDIT BUTTON
#define SW_TILT                     6 // TILT (3)
#define SW_OUTHOLE                  7 // OUTHOLE
#define SW_COIN_3                   8 // COIN III (RIGHT)
#define SW_COIN_1                   9 // COIN I (LEFT)
#define SW_COIN_2                   10 // COIN II (CENTER)
#define SW_TARGET_LRIGHT_BOTTOM     11 // LOWER RIGHT SIDE, BOTTOM TARGET
#define SW_INLANE_R                 12 // FLIPPER FEED LANE (RIGHT)
#define SW_INLANE_L                 13 // FLIPPER FEED LANE (LEFT)
#define SW_TARGET_LRIGHT_TOP        14 // LOWER RIGHT SIDE, TOP TARGET
#define SW_SLAM                     15 // SLAM (2)
#define SW_DTARGET_4_D              16 // 4 DROP TARGET "A" (BOTTOM)
#define SW_DTARGET_4_C              17 // 4 DROP TARGET "B"
#define SW_DTARGET_4_B              18 // 4 DROP TARGET "C"
#define SW_DTARGET_4_A              19 // 4 DROP TARGET "D" (TOP)
#define SW_DTARGET_3_A              20 // #1 ARROW DROP TARGET (TOP)
#define SW_DTARGET_3_B              21 // #2 ARROW DROP TARGET (MIDDLE)
#define SW_DTARGET_3_C              22 // #3 ARROW DROP TARGET (BOTTOM)
#define SW_TARGET_TOP               23 // TOP TARGET
#define SW_DTARGET_INLINE_1ST       24 // 1ST INLINE DROP TARGET
#define SW_DTARGET_INLINE_2ND       25 // 2ND INLINE DROP TARGET
#define SW_DTARGET_INLINE_3RD       26 // 3RD INLINE DROP TARGET
#define SW_TARGET_WOOD_BEAST        27 // INLINE BACK TARGET
#define SW_REBOUND                  28 // 10 POINT REBOUND (2)
#define SW_SAUCER                   29 // SAUCER
#define SW_OUTLANE_RIGHT            30 // RIGHT OUTLANE
#define SW_OUTLANE_LEFT             31 // LEFT OUTLANE
#define SW_SPINNER_RIGHT            32 // RIGHT SPINNER
#define SW_SPINNER_LEFT             33 // LEFT SPINNER
#define SW_SLING_RIGHT              34 // RIGHT SLINGSHOT
#define SW_SLING_LEFT               35 // LEFT SLINGSHOT
#define SW_POP_TOP                  36 // TOP THUMPER BUMPER
// #define N/A                      37 // not used
#define SW_POP_RIGHT                38 // RIGHT THUMPER BUMPER
#define SW_POP_LEFT                 39 // LEFT THUMPER BUMPER


// Defines for solenoids
// NOTE: Coin lockout and flipper enable are accessed through calls to RPU routines, never through the solenoid stack
#define SO_DTARGET_4_RESET          0 // 4 DROP TARGET
#define SO_DTARGET_3_RESET          1 // 3 DROP TARGET
#define SO_DTARGET_INLINE_RESET     2 // INLINE DROP TARGET
#define SO_SAUCER_DOWN              3 // SAUCER KICK DOWN
// #define N/A                      4 // not used
#define SO_KNOCKER                  5 // KNOCKER
#define SO_OUTHOLE                  6 // OUTHOLE KICKER
#define SO_SAUCER_UP                7 // SAUCER KICK UP
#define SO_DTARGET_1_RESET          8 // SINGLE DROP TARGET RESET
#define SO_POP_LEFT                 9 // LEFT THUMPER BUMPER
#define SO_POP_RIGHT                10 // RIGHT THUMPER BBUMPER
#define SO_DTARGET_1_DOWN           11 // SINGLE DROP TARGET PULL DOWN
#define SO_POP_TOP                  12 // TOP THUMPER BUMPER
#define SO_SLING_LEFT               13 // LEFT SLINGSHOT
#define SO_SLING_RIGHT              14 // RIGHT SLINGSHOT
#define SO_COIN_LOCKOUT             15 // COIN LOCKOUT DOOR
#define SO_FLIPPER_ENABLE           16 // K1 RELAY (FLIPPER ENABLE)
// #define N/A                      ?? // not used
// #define N/A                      ?? // not used


// Game Machine States
#define MACHINE_STATE_ATTRACT         0
#define MACHINE_STATE_INIT_GAMEPLAY   1
#define MACHINE_STATE_INIT_NEW_BALL   2
#define MACHINE_STATE_SKILL_SHOT      3
#define MACHINE_STATE_NORMAL_GAMEPLAY 4
#define MACHINE_STATE_COUNTDOWN_BONUS 90
#define MACHINE_STATE_WIZARD_MODE     91
#define MACHINE_STATE_BALL_OVER       100
#define MACHINE_STATE_MATCH_MODE      110


// Extended Self-Test Machine States (See also SelfTestAndAudit.h)
#define MACHINE_STATE_TEST_BALL_SAVE            -18
#define MACHINE_STATE_TEST_GAIN                 -19
#define MACHINE_STATE_TEST_TOTAL_SKILL          -20
#define MACHINE_STATE_TEST_TOTAL_WIZ            -21
#define MACHINE_STATE_TEST_TOTAL_WIZ_BEAT       -22
#define MACHINE_STATE_TEST_PLAYFIELD_AWARD      -23
#define MACHINE_STATE_TEST_THRESHOLD_AWARD      -24
#define MACHINE_STATE_TEST_BACKGROUND_OFF       -25
#define MACHINE_STATE_TEST_HIGHSCORE_AWARD      -26
#define MACHINE_STATE_TEST_FREEPLAY             -27
#define MACHINE_STATE_TEST_GOAL_OFFON           -28
#define MACHINE_STATE_TEST_WIZARD_GOAL_TARGETS  -29
#define MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK   -30
#define MACHINE_STATE_TEST_KID_MODE             -31
#define MACHINE_STATE_TEST_ORIGINAL_RULES       -32

#define NUM_EXTENDED_SETTINGS      15

// Storage for Extended Self-Test Game settings
#define RPU_PLAYFIELD_AWARD      74
#define RPU_THRESHOLD_AWARD      75
#define RPU_BACKGROUND_OFF       76
#define RPU_HIGHSCORE_AWARD      77
#define RPU_FREEPLAY             78
#define RPU_GOAL_OFFON           79
#define RPU_WIZARD_GOAL_TARGETS  80
#define RPU_WIZARD_GOAL_ATTACK   81
#define RPU_TEMP_KID_MODE_OFFON  86
#define RPU_ORIGINAL_RULES       87
#define RPU_BALL_SAVES           88
#define RPU_BALL_SAVE_SCORE      89
#define RPU_BALL_SAVE_NUM_SECS   90
#define RPU_BG_GAIN              91
#define RPU_SFX_GAIN             92
#define RPU_VOICE_GAIN           93
// see also RPU_Config.h #define RPU_testname, currently up to 82 + 3 = 85



// SWITCHES_WITH_TRIGGERS are for switches that will automatically
// activate a solenoid (like in the case of a chime that rings on a rollover)
// but SWITCHES_WITH_TRIGGERS are fully debounced before being activated
#define NUM_SWITCHES_WITH_TRIGGERS 5

// PRIORITY_SWITCHES_WITH_TRIGGERS are switches that trigger immediately
// (like for pop bumpers or slings) - they are not debounced completely
#define NUM_PRIORITY_SWITCHES_WITH_TRIGGERS 5


#define NUM_OF_SAUCER_LAMPS_UP 30
struct SaucerLampsUp {
   byte lightNumSaucerUp;
   byte rowSaucerUp;
};
struct SaucerLampsUp SaucerLampsUp[] = {
   {LA_BONUS_3X, 1},
   {LA_BONUS_4X, 1},
   {LA_BONUS_2X, 2},
   {LA_BONUS_5X, 2},
   {LA_BONUS_SUPER_6K, 3},
   {LA_BONUS_SUPER_5K, 4},
   {LA_BONUS_SUPER_7K, 4},
   {LA_BONUS_SUPER_4K, 5},
   {LA_BONUS_SUPER_8K, 5},
   {LA_BONUS_SUPER_100K, 6},
   {LA_BONUS_SUPER_3K, 7},
   {LA_BONUS_SUPER_9K, 7},
   {LA_BONUS_SUPER_2K, 8},
   {LA_BONUS_SUPER_10K, 8},
   {LA_BONUS_SUPER_1K, 9},
   {LA_BONUS_MINI_6K, 10},
   {LA_BONUS_MINI_5K, 11},
   {LA_BONUS_MINI_7K, 11},
   {LA_BONUS_MINI_4K, 12},
   {LA_BONUS_MINI_8K, 12},
   {LA_BONUS_MINI_50K, 13},
   {LA_BONUS_MINI_3K, 14},
   {LA_BONUS_MINI_9K, 14},
   {LA_BONUS_MINI_2K, 15},
   {LA_BONUS_MINI_10K, 15},
   {LA_BONUS_MINI_1K, 16},
   {LA_SAUCER_10K,17},
   {LA_SAUCER_20K, 18},
   {LA_SAUCER_30K, 19},
   {LA_SAUCER_XBALL, 20}
};

#define NUM_OF_SAUCER_LAMPS_DOWN 30
struct SaucerLampsDown {
   byte lightNumSaucerDown;
   byte rowSaucerDown;
};
struct SaucerLampsDown SaucerLampsDown[] = {
   {LA_SAUCER_XBALL, 1},
   {LA_SAUCER_30K, 2},
   {LA_SAUCER_20K, 3},
   {LA_SAUCER_10K,4},
   {LA_BONUS_MINI_1K, 5},
   {LA_BONUS_MINI_10K, 6},
   {LA_BONUS_MINI_2K, 6},
   {LA_BONUS_MINI_9K, 7},
   {LA_BONUS_MINI_3K, 7},
   {LA_BONUS_MINI_50K, 8},
   {LA_BONUS_MINI_8K, 9},
   {LA_BONUS_MINI_4K, 9},
   {LA_BONUS_MINI_7K, 10},
   {LA_BONUS_MINI_5K, 10},
   {LA_BONUS_MINI_6K, 11},
   {LA_BONUS_SUPER_1K, 12},
   {LA_BONUS_SUPER_10K, 13},
   {LA_BONUS_SUPER_2K, 13},
   {LA_BONUS_SUPER_9K, 14},
   {LA_BONUS_SUPER_3K, 14},
   {LA_BONUS_SUPER_100K, 15},
   {LA_BONUS_SUPER_8K, 16},
   {LA_BONUS_SUPER_4K, 16},
   {LA_BONUS_SUPER_7K, 17},
   {LA_BONUS_SUPER_5K, 17},
   {LA_BONUS_SUPER_6K, 18},
   {LA_BONUS_5X, 19},
   {LA_BONUS_2X, 19},
   {LA_BONUS_4X, 20},
   {LA_BONUS_3X, 20}
};

#define NUM_OF_ATTRACT_LAMPS_UP 35
struct AttractLampsUp {
   byte lightNumUp;
   byte rowUp;
};
struct AttractLampsUp AttractLampsUp[] = {
   {LA_SHOOT_AGAIN, 1},
   {LA_BONUS_3X, 2},
   {LA_BONUS_4X, 2},
   {LA_BONUS_2X, 3},
   {LA_BONUS_5X, 3},
   {LA_BONUS_SUPER_6K, 4},
   {LA_BONUS_SUPER_5K, 5},
   {LA_BONUS_SUPER_7K, 5},
   {LA_BONUS_SUPER_4K, 6},
   {LA_BONUS_SUPER_8K, 6},
   {LA_BONUS_SUPER_100K, 7},
   {LA_BONUS_SUPER_3K, 8},
   {LA_BONUS_SUPER_9K, 8},
   {LA_BONUS_SUPER_2K, 9},
   {LA_BONUS_SUPER_10K, 9},
   {LA_CLOCK_15_SECONDS_2X, 10},
   {LA_CLOCK_15_SECONDS_3X, 10},
   {LA_BONUS_SUPER_1K, 11},
   {LA_BONUS_MINI_6K, 12},
   {LA_BONUS_MINI_5K, 13},
   {LA_BONUS_MINI_7K, 13},
   {LA_BONUS_MINI_4K, 14},
   {LA_BONUS_MINI_8K, 14},
   {LA_BONUS_MINI_50K, 15},
   {LA_BONUS_MINI_3K, 16},
   {LA_BONUS_MINI_9K, 16},
   {LA_BONUS_MINI_2K, 17},
   {LA_BONUS_MINI_10K, 17},
   {LA_BONUS_MINI_1K, 18},
   {LA_SAUCER_ARROW_2X, 19},
   {LA_SAUCER_ARROW_3X, 19},
   {LA_SAUCER_10K, 20},
   {LA_SAUCER_20K, 21},
   {LA_SAUCER_30K, 22},
   {LA_SAUCER_XBALL, 23}
};

#define NUM_OF_ATTRACT_LAMPS_DOWN 28
struct AttractLampsDown {
   byte lightNumDown;
   byte rowDown;
};
struct AttractLampsDown AttractLampsDown[] = {
   {LA_POP_TOP, 1},
   {LA_STAR_SHOOTER_TOP, 1},
   {LA_TARGET_UPPER_SPECIAL, 2},
   {LA_TARGET_UPPER_COLLECT_BONUS, 3},
   {LA_STAR_SHOOTER_MIDDLE, 3},
   {LA_DTARGET_ARROW_1, 4},
   {LA_DTARGET_ARROW_2, 5},
   {LA_STAR_PFIELD_TOP, 5},
   {LA_STAR_SHOOTER_BOTTOM, 5},
   {LA_DTARGET_ARROW_3, 6},
   {LA_DTARGET_BONUS_4X, 6},
   {LA_STAR_PFIELD_BOTTOM, 7},
   {LA_MING_TOP, 8},
   {LA_MING_BOTTOM, 9},
   {LA_SPINNER_LEFT, 10},
   {LA_SPINNER_RIGHT, 11},
   {LA_DTARGET_4_A, 12},
   {LA_DTARGET_4_B, 13},
   {LA_TARGET_WOOD_BEAST_XBALL, 14},
   {LA_DTARGET_BONUS_5X, 14},
   {LA_DTARGET_4_C, 15},
   {LA_TARGET_LRIGHT_TOP, 16},
   {LA_DTARGET_4_D, 17},
   {LA_TARGET_LRIGHT_BOTTOM, 18},
   {LA_INLANE_LEFT, 19},
   {LA_INLANE_RIGHT, 19},
   {LA_OUTLANE_LEFT_SPECIAL, 20},
   {LA_OUTLANE_RIGHT_SPECIAL, 20}
};

#define NUM_OF_ATTRACT_LAMPS_SIREN 63
struct AttractLampsSiren {
   byte lightNumSiren;
   byte rowSiren;
};
struct AttractLampsSiren AttractLampsSiren[] = {
   {LA_POP_TOP, 1},
   {LA_DTARGET_BONUS_4X, 2},
   {LA_TARGET_UPPER_COLLECT_BONUS, 3},
   {LA_TARGET_UPPER_SPECIAL, 4},
   {LA_DTARGET_ARROW_1, 5},
   {LA_DTARGET_ARROW_2, 6},
   {LA_SAUCER_10K, 6},
   {LA_DTARGET_ARROW_3, 7},
   {LA_SAUCER_20K, 7},
   {LA_SAUCER_30K, 8},
   {LA_SAUCER_XBALL, 9},
   {LA_MING_TOP, 10},
   {LA_MING_BOTTOM, 11},
   {LA_SPINNER_LEFT, 11},
   {LA_DTARGET_4_A, 12},
   {LA_DTARGET_4_B, 13},
   {LA_DTARGET_4_C, 14},
   {LA_DTARGET_BONUS_5X, 14},
   {LA_DTARGET_4_D, 15},
   {LA_SAUCER_ARROW_2X, 15},
   {LA_OUTLANE_LEFT_SPECIAL, 16},
   {LA_INLANE_LEFT, 16},
   {LA_BONUS_MINI_8K, 17},
   {LA_BONUS_MINI_9K, 17},
   {LA_BONUS_MINI_10K, 17},
   {LA_CLOCK_15_SECONDS_2X, 17},
   {LA_BONUS_MINI_7K, 18},
   {LA_BONUS_SUPER_8K, 18},
   {LA_BONUS_SUPER_9K, 18},
   {LA_BONUS_2X, 18},
   {LA_BONUS_SUPER_7K, 19},
   {LA_BONUS_SUPER_10K, 19},
   {LA_BONUS_3X, 19},
   {LA_BONUS_MINI_1K, 20},
   {LA_BONUS_MINI_50K, 20},
   {LA_BONUS_MINI_6K, 20},
   {LA_BONUS_SUPER_1K, 20},
   {LA_BONUS_SUPER_100K, 20},
   {LA_BONUS_SUPER_6K, 20},
   {LA_SHOOT_AGAIN, 20},
   {LA_BONUS_SUPER_2K, 21},
   {LA_BONUS_SUPER_5K, 21},
   {LA_BONUS_4X, 21},
   {LA_BONUS_MINI_5K, 22},
   {LA_BONUS_SUPER_3K, 22},
   {LA_BONUS_SUPER_4K, 22},
   {LA_BONUS_5X, 22},
   {LA_BONUS_MINI_2K, 23},
   {LA_BONUS_MINI_3K, 23},
   {LA_BONUS_MINI_4K, 23},
   {LA_CLOCK_15_SECONDS_3X, 23},
   {LA_INLANE_RIGHT, 24},
   {LA_OUTLANE_RIGHT_SPECIAL, 24},
   {LA_TARGET_LRIGHT_BOTTOM, 25},
   {LA_TARGET_LRIGHT_TOP, 26},
   {LA_TARGET_WOOD_BEAST_XBALL, 27},
   {LA_SAUCER_ARROW_3X, 28},
   {LA_SPINNER_RIGHT, 29},
   {LA_STAR_PFIELD_BOTTOM, 30},
   {LA_STAR_SHOOTER_BOTTOM, 31},
   {LA_STAR_PFIELD_TOP, 32},
   {LA_STAR_SHOOTER_MIDDLE, 32},
   {LA_STAR_SHOOTER_TOP, 33}
};

#define NUM_OF_ATTRACT_LAMPS_MING_ATTACK 60
struct AttractLampsMingAttack {
   byte lightNumMingAttack;
   byte rowMingAttack;
};
struct AttractLampsMingAttack AttractLampsMingAttack[] = {
   {LA_DTARGET_ARROW_3, 1},
   {LA_DTARGET_ARROW_2, 1},
   {LA_DTARGET_BONUS_4X, 2},
   {LA_DTARGET_ARROW_1, 2},
   {LA_SAUCER_XBALL, 2},
   {LA_SPINNER_LEFT, 2},
   {LA_SAUCER_30K, 3},
   {LA_TARGET_UPPER_COLLECT_BONUS, 3},
   {LA_TARGET_UPPER_SPECIAL, 4},
   {LA_SAUCER_20K, 4},
   {LA_SAUCER_10K, 5},
   {LA_DTARGET_4_A, 5},
   {LA_POP_TOP, 6},
   {LA_STAR_PFIELD_TOP, 6},
   {LA_STAR_PFIELD_BOTTOM, 6},
   {LA_SAUCER_ARROW_2X, 6},
   {LA_SAUCER_ARROW_3X, 6},
   {LA_DTARGET_4_B, 6},
   {LA_STAR_SHOOTER_TOP, 7},
   {LA_SPINNER_RIGHT, 7},
   {LA_DTARGET_BONUS_5X, 7},
   {LA_DTARGET_4_C, 7},
   {LA_STAR_SHOOTER_MIDDLE, 8},
   {LA_DTARGET_4_D, 8},
   {LA_BONUS_MINI_1K, 8},
   {LA_BONUS_MINI_10K, 8},
   {LA_STAR_SHOOTER_BOTTOM, 9},
   {LA_TARGET_WOOD_BEAST_XBALL, 9},
   {LA_BONUS_MINI_2K, 9},
   {LA_BONUS_MINI_9K, 9},
   {LA_TARGET_LRIGHT_TOP, 10},
   {LA_BONUS_MINI_3K, 10},
   {LA_BONUS_MINI_8K, 10},
   {LA_BONUS_MINI_50K, 10},
   {LA_TARGET_LRIGHT_BOTTOM, 11},
   {LA_BONUS_MINI_4K, 11},
   {LA_BONUS_MINI_5K, 11},
   {LA_BONUS_MINI_6K, 11},
   {LA_BONUS_MINI_7K, 11},
   {LA_INLANE_LEFT, 11},
   {LA_CLOCK_15_SECONDS_2X, 12},
   {LA_CLOCK_15_SECONDS_3X, 13},
   {LA_OUTLANE_LEFT_SPECIAL, 13},
   {LA_BONUS_SUPER_1K, 13},
   {LA_BONUS_SUPER_2K, 13},
   {LA_BONUS_SUPER_10K, 13},
   {LA_INLANE_RIGHT, 14},
   {LA_BONUS_SUPER_3K, 14},
   {LA_BONUS_SUPER_8K, 14},
   {LA_BONUS_SUPER_9K, 14},
   {LA_BONUS_SUPER_100K, 14},
   {LA_OUTLANE_RIGHT_SPECIAL, 15},
   {LA_BONUS_SUPER_4K, 15},
   {LA_BONUS_SUPER_7K, 15},
   {LA_BONUS_SUPER_5K, 16},
   {LA_BONUS_SUPER_6K, 16},
   {LA_BONUS_2X, 16},
   {LA_BONUS_4X, 17},
   {LA_BONUS_3X, 18},
   {LA_BONUS_5X, 19}
};


// Define automatic solenoid triggers (switch, solenoid, number of 1/120ths of a second to fire)
struct PlayfieldAndCabinetSwitch TriggeredSwitches[] = {
   { SW_SLING_RIGHT, SO_SLING_RIGHT, 4},
   { SW_SLING_LEFT,  SO_SLING_LEFT,  4},
   { SW_POP_RIGHT,   SO_POP_RIGHT,   3},
   { SW_POP_LEFT,    SO_POP_LEFT,    3},
   { SW_POP_TOP,     SO_POP_TOP,     3}
};



/* Squawk & Talk Timings (No longer used)
byte SoundTimings[52] = {
   0,  // 0         off/strange
   0,  // 1         off/strange
   0,  // 2         off/strange
   0,  // 3         off/strange
   0,  // 4  ****** 2.060s humm low (same as 9 ???)
   0,  // 5         sound off
   1,  // 6         background sound 1
   0,  // 7  ****** 0.173s rebound hit
   0,  // 8  ****** 1.550s spinner humm medium
   0,  // 9  ****** 1.971s spinner humm low (same as 4 ???)
   1,  // 10 0.500s or 11.820s (multiply) timer sound 
   0,  // 11        background sound 2
   0,  // 12 ****** 0.411s boink up high
   0,  // 13 ****** 0.467s boink up low
   0,  // 14        background sound 3
   0,  // 15        background sound 4
   2,  // 16 0.400s (multiply) alarm
   6,  // 17 1.360s low grumble (new)
   0,  // 18 3.240s saucer (repeat of 22 & 23)
   0,  // 19        background sound 5
   0,  // 20 ****** 0.844s drop target hit
   0,  // 21 ****** 0.889s beep beep hit
   13, // 22 3.240s saucer (repeat of 18 & 23)
   13, // 23 3.240s saucer OR sound off (repeat of 18 & 22)
   0,  // 24 ****** 0.643s bong bounce down
   0,  // 25 ****** 1.260s bong bounce up
   0,  // 26 ****** 1.760s crash bounce down
   0,  // 27 ****** 1.760s crash bounce up
   0,  // 28 3.638s outlanes
   0,  // 29 ****** 1.497s crash
   0,  // 30        background sound 6 OR sound off
   0,  // 31        background sound 7
   0,  // 32 ****** 0.463s ding 1
   0,  // 33 ****** 0.463s ding 2
   0,  // 34 ****** 0.463s ding 3
   0,  // 35 ****** 0.463s ding 4
   0,  // 36        background sound 8
   0,  // 37        background sound 9
   0,  // 38        background sound 10 OR sound off
   0,  // 39        background sound 11
   15, // 40 3.721s "ignite deathray, 15 seconds" ming
   52,  // 41 12.940s ming laugh five times
   10, // 42 2.321s "lucky shot earthling" ming
   8,  // 43 1.830s "miserable earthling" ming OR sound off
   8,  // 44 2.058s "emperor ming awaits" ming
   6,  // 45 1.500s "flash" ming
   14, // 46 2.471s "try again earthling" ming *Lengthened for Geeteoh end sound
   8,  // 47 1.856s "15 seconds" ming
   7,  // 48 1.770s "miserable earthling" ming
   6,  // 49 1.500s "flash" ming
   10,  // 50 2.527s ming laugh single OR ding ?
   8   // 51 1.803s "15 seconds" ming
};
*/

// Geeteoh Sound Data [256 sounds][Voice (Y/N), Geeteoh Type, Length / No Interrupt for Voices (10ths of a second), Geeteoh expected length, S&T length]
byte SoundData[256][5] = { 
{0, 0,  0,  0,  0}, // 000 255 Blank
{0, 0,  0,  0,  0}, // 001 254 Blank
{0, 0,  0,  0,  0}, // 002 253 Blank
{0, 0,  0,  0,  0}, // 003 252 Blank
{1, 2, 23, 17, 17}, // 004 251 This way Flash, come on!
{0, 0,  0,  0,  0}, // 005 250 Stop all sounds. Not used by Geeteoh.
{0, 1,  0,  0,  0}, // 006 249 Flash by Queen, instrumental
{0, 2,  8,  4,  2}, // 007 248 Blaster
{0, 2, 16, 17, 16}, // 008 247 Ray Gun
{0, 2, 16, 18, 15}, // 009 246 Low hum
{0, 9,  0,  0, 57}, // 010 245 15 second football alarm
{0, 0,  0,  0,  0}, // 011 244 Blank
{0, 2,  4,  6, 39}, // 012 243 Ascending, low
{0, 2,  5,  7, 46}, // 013 242 Ascending, high
{0, 0,  0,  0,  0}, // 014 241 Blank
{0, 0,  0,  0,  0}, // 015 240 Blank
{0, 2, 33, 80, 50}, // 016 239 Old Alarm
{1, 2, 20, 16, 14}, // 017 238 Switches over there, start hitting them!
{0, 7,  0, 35, 34}, // 018 237 Blank
{0, 0,  0,  0,  0}, // 019 236 Blank
{0, 2, 25, 10,  9}, // 020 235 Ming’s Ring
{0, 2, 21,  9,  9}, // 021 234 Ray Gun
{1, 7, 20, 35, 34}, // 022 233 Escape is impossible!
{0, 7,  0, 35, 34}, // 023 232 blank
{0, 2, 11, 12, 11}, // 024 231 Bounce (Low)
{0, 2, 11, 13, 11}, // 025 230 Bounce (High)
{0, 2, 12, 21, 22}, // 026 229 Zap (Electric)
{1, 2, 13, 23, 22}, // 027 228 Look out Flash!
{1, 2, 17,  0, 36}, // 028 227 What a Damn Nuisance! (Geeteoh says 3.8 seconds. Setting to zero to hopefully avoid loss of bonus collection sounds.)
{0, 2, 10, 27, 18}, // 029 226 Zap 2 Quick
{0, 0, 37,  0,  0}, // 030 225 Match
{1, 0, 16,  0,  0}, // 031 224 Your power's fading, Ming
{0, 2,  5,  5,  5}, // 032 223 Tone
{0, 2,  5,  5,  5}, // 033 222 Tone+
{0, 2,  5,  5,  5}, // 034 221 Tone++
{0, 2,  5,  5,  5}, // 035 220 Tone+++
{1, 0, 28,  0,  0}, // 036 219 Oh Dear, How Pathetic
{1, 0, 33,  0,  0}, // 037 218 You’ve saved your Earth. Have a nice day. Yeah!
{1, 0, 25,  0,  0}, // 038 217 Kid Ming laugh
{1, 0, 40,  0,  0}, // 039 216 Kids cheering
{1, 4, 24, 35, 34}, // 040 215 Open fire, all weapons!
{1, 4, 41,  0,  0}, // 041 214 Gently Darling, it’s extremely sensitive… like me! (Geeteoh says 12.1 seconds. Setting to zero to hopefully avoid loss of subsequent sounds.)
{1, 4, 24, 23, 21}, // 042 213 Very roughly, 14 seconds! (should be length 20. Changed to 24 for timing issue with 86 below)
{1, 4, 23, 18, 17}, // 043 212 The attack has begun!
{1, 4, 20, 20, 19}, // 044 211 Gordon’s alive!
{1, 4, 10, 15, 14}, // 045 210 Flash!
{1, 4, 33, 24, 23}, // 046 209 You’ve saved your Earth. Have a nice day. Yeah!
{1, 4, 20, 19, 17}, // 047 208 Gordon's alive! // Copied from 44 when it stopped working for unknown reasons!
{1, 4, 10, 18, 17}, // 048 207 Oh dear…
{1, 4,  8, 15, 14}, // 049 206 Flash!
{1,10, 38, 25, 24}, // 050 205 Ming laugh!
{1, 4, 36, 18, 17}, // 051 204 Saviour of the Universe!
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{1, 0, 25,  0,  0}, // 70 185 Kid Ming laugh
{1, 0, 40,  0,  0}, // 71 184 Kids cheering
{0, 0,  0,  0,  0},
{1, 0, 10,  0,  0}, // 73 182 Oh dear
{1, 0, 20,  0,  0}, // 74 181 Wood Beast roar 
{1, 0, 10,  0,  0}, // 75 180 Oh Flash
{1, 0, 28,  0,  0}, // 76 179 How? By magic, of course
{1, 0, 20,  0,  0}, // 77 178 Don't kill him yet, father
{0, 0,150,  0,  0}, // 78 177 15 second hurry-up timer
{0, 0, 50,  0,  0}, // 79 176 5 second hurry-up timer
{0, 1,  0,  0,  0}, // 80 175
{0, 1,  0,  0,  0}, // 81 174
{0, 1,  0,  0,  0}, // 82 173 
{0, 1,  0,  0,  0}, // 83 172 Background music for Final Battle
{0, 1,  0,  0,  0}, // 84 171 Background music for Skill Shot
{1, 0, 30,  0,  0}, // 85 170 Try the Wood Beast, or die!
{1, 0, 38,  0,  0}, // 86 169 Remove the Earth woman. Forget it Ming, Dale's with me!
{1, 0, 18,  0,  0}, // 87 168 This place is a lunatic asylum
{0, 0, 30,  0,  0}, // 88 167 Football Alarm Ending
{0, 0,  0,  0,  0}, // 89 166 
{0, 8,  0,  0,  0}, // 90 165 Stop all background sounds
{0, 0,  0,  0,  0}, // 91 164
{0, 0,  0,  0,  0}, // 92 163
{0, 0,  0,  0,  0}, // 93 162
{1, 4, 70,  0,  0}, // 94 161 Oh well. Who wants to live forever?
{1, 4, 28,  0,  0}, // 95 160 Oh dear, how pathetic
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{0, 0,  0,  0,  0},
{1, 0, 23,  0,  0}, // 254 001 This way Flash, come on!
{0, 0,  0,  0,  0}
};

// *** Sound File Groups. First value is number of files in group:

byte VoiceSaucerDown[3] = {2, 22, 40};
byte VoiceSaucerUp  [3] = {2, 76, 87};
