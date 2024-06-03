/**************************************************************************

  This file is part of Flash Gordon 2021 -  for the Arduino Mega 2560
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


  Version 2024.04 by Dave's Think Tank (See notes below)
*/

/**************************************************************************

  Version 2024.04 by Dave's Think Tank

  This software was received at Dave's Think Tank in 2024. It appeared to be
  the latest version available, with no other version available anywhere on
  the internet. It is not clear what happened to this project after 2021, but
  it appears to have been abandoned in this state! This section documents the 
  many bug fixes, changes, and additions made to the software:

Changes / Additions:

-  Job #1: Detailed document created covering all self tests, accounting values, game settings, and DIP switches!

- added DIP-switch variables.  Read contents of MPU DIP switches into variables, and reprogrammed code to control the game as per the original Bally manual:
- DIP switch 1-5, 9-13, 17-20: Set coins per credit, and credits per coin, for each coin chute as per the original Bally manual (more or less!)
- DIP switch  6: 10K is awarded each time up/down saucer is hit, in addition to lit values (slightly different than original, to preserve basic idea of hitting all target banks)
- DIP switch  7: Saucer lights are saved for the next ball
- DIP switch  8: 2X, 3X arrow lights are saved for the next ball
- DIP switch 14: Outlane special lights are saved for next ball (if not hit)
- DIP switch 15: Mini Bonus and special lights on top target saved for next ball
- DIP switch 16: 2X, 3X, 4X, 5X bonus multiplier saved for next ball
- DIP switch 21: Allow / disallow attract sound
- DIP switch 22: Lower right targets, flipper feed lanes, and 4 drop target lights saved for next ball
- DIP switch 23: 4 drop target lights saved for next ball
- DIP switch 24: upper level drop target arrows saved for next ball
- DIP switch 25-26: Set maximum credits
- DIP switch 27: Credits are not displayed if switch is on
- DIP switch 28: Allow / disallow Match feature at end of game
- DIP switch 29: Player restricted to only one free game per game if off (does not apply to high score or match)
- DIP switch 30: Restrict Wood Beast target to one extra ball per game if off
- DIP switch 31-32: Set number of balls per game

- Many additional features were added to the Self-Test function:

- Sound test, changed to allow cycling through all sounds. Features using game button allow skipping sounds, repeating sounds, fast forward through sounds.
- Added options in self test for all accounting values to be increased, as well as zeroed.
- Created Extended Self Test routine to handle additional tests and settings without major changes to the SelfTestAndAudit program files.
- Playfield awards were always extra ball. Added game setting to allow points, extra ball, or free game, as per original manual.
- Threshold awards were always free game. Added game setting to allow points, extra ball, or free game, as per original manual.
- Added game setting to turn background music off or on, as per original manual (minus chime options)
- High score award was always three free games. Added game setting to allow 0, 1, 2, or 3 free games, as per original manual.
- Added a game setting for free play.
- Added "Personal Goal" to self tests. This is used to provide two different end-of-game sounds, if you beat your personal goal or not.
- Added Personal Goal off/on setting to self tests, as most pinballs will not be set up with two end sounds. 
  (Note, this feature is not yet available. It relies on future coordination with Geeteoh, and so has been disabled for now.)

Other Changes and Additions:

- Version number (2024.04) displayed at beginning of game in display #1 (2024) and credit display (04). This is the date I started working on
  the project. Future changes made by other programmers can use the same convention, with less worry about creating multiple versions with the
  same identifier.
- Removed calls to sound 5 (stop all sounds) when using a Geeteoh sound board. This sound can affect current and following sounds with Geeteoh.
- Was set such that Wood Beast extra ball had to be won before saucer extra ball. This seemed unnecessary and so I removed this restriction.
- After hitting the flashing drop target, then the inner flipper lane, you are given only 2 seconds to hit the opposite spinner! Changed this to five.
- After hitting the flashing drop target, you were given only 12.5 seconds to hit the matching right side target! Changed this to 15 seconds, and
  only for points. That is, if you hit it within 15 seconds you get 50,000 points. If you don't hit it in this time, you will get zero points when
  you hit it, but you do not have to start over again with the drop target.
- Changed all extra ball awards to award points if extra ball already awarded, or if maximum extra balls per game already won.
- Changed all free game awards to award points if maximum free games per game already won.
- All sounds checked. Sounds all working with Squawk & Talk as well as Geeteoh.
- All scoring checked. Conforms with descriptions on playfield, and original game to the extent possible.

Bug Fixes:

- Displays were handled sometimes as 6 digit, sometimes as 7, leading to strange results in display test. Fixed.
- Display test, 0000000 was only showing as 00. Fixed.
- Extended self test to include all available tests. Was stopping at award level 3.
- fixed multiple bugs leading to incorrect accounting of credits. The software appears to have been set up for 
  free play only. Fixed credits per coin and coins per credit according to DIP switches and original Bally manual.
- Replaced buggy ENABLE_CPC_SETTINGS (credits per coin) code from SelfTestAndAudit with code based on DIP switch settings.
- Apron credit light fixed to be on when number of credits non-zero. Proper credit sound plays. Works even if free play selected.
- Multiple bugs prevented the three award thresholds from working properly. Thresholds were always zero, and so free play was won on first point scored, setting off knocker at start of every game.
- Self test values were updated in memory only, but the new values were not read into the game. So award levels, high score, and other values were not used or maintained properly.
- Fixed payment through chutes 1, 2, and 3 to update memory.
- Fixed all occurrences where credits won could put the number of credits over the maximum.
- Attract mode sound interval not handled properly. Fixed and set first occurrence at four seconds, five minutes thereafter.
- Background and other sounds would not play if the game was started while end quote for previous game was playing.
- Background and other sounds would not play if the game was started while attract sound was playing.
- Background and other sounds would not play if the background sound was started while tilt sound was playing.
- Fixed outhole kicker, so that if the ball does not kick out properly and falls back into the hole, it will be kicked out again. Separate fixes for Skill Shot and Wizard Mode.
- Timing of 3-arrow flashes was calculated by dividing by the number of flashing arrows, which could be zero. Not sure why it ever worked.
- Allow scoring thresholds to be zero, meaning no threshold at that level or higher.
- Single target was set to go up when any upper level target was hit, except the spinners! Added spinners to ensure target is up when playfield engaged.
- If ball got to lower level during skill shot and colored switches hit, lights would stay on. Fixed so they would just blip.
- Single drop target was not advancing mini bonus by 2.
- Solenoid stack disabled after Ming defeat, but ball can occasionally bounce back into saucer. Set override on down kicker call.
- Ball would be stuck in the outhole if machine tilted during skill shot.

*/

#include "BSOS_Config.h"
#include "BallySternOS.h"
#include "FG2024.h"
#include "SelfTestAndAudit.h"

//============================== OPERATOR GAME ADJUSTMENTS ==============================
#define WIZARD_GOAL_DTARGET 16        // (16) set number of drop targets needed to light wizard mode at outlanes
#define WIZARD_GOAL_ATTACK 140        // (140) set attack power needed to light saucer in wizard mode
#define ATTRACT_SPEECH_TIMER 300000   // (300000) Amount of time between attract mode speech callouts. 60000 = 1 minute
#define USE_STROBE 0                  // Strobe use is currently not recommended. Please message me if you would like to help! 0 = don't use backglass strobe, 1 = use backglass strobe
#define DEBUG_MESSAGES  0             // 0 = no serial monitor, 1 = serial monitor for testing
//=====================================================================================

#define VERSION_NUMBER  2024.04

boolean FirstStartup = true;
int MachineState = 0;
boolean MachineStateChanged = true;
#define MACHINE_STATE_ATTRACT         0
#define MACHINE_STATE_INIT_GAMEPLAY   1
#define MACHINE_STATE_INIT_NEW_BALL   2
#define MACHINE_STATE_SKILL_SHOT      3
#define MACHINE_STATE_NORMAL_GAMEPLAY 4
#define MACHINE_STATE_COUNTDOWN_BONUS 90
#define MACHINE_STATE_WIZARD_MODE     91
#define MACHINE_STATE_BALL_OVER       100
#define MACHINE_STATE_MATCH_MODE      110


unsigned long CurrentTime = 0;
unsigned long AttractWait = 0;
unsigned long InitGameStartTime = 0;

byte Credits = 0;
unsigned long TotalPlays = 0;
unsigned long TotalReplays = 0;
unsigned long ScoreAward1 = 0;
unsigned long ScoreAward2 = 0;
unsigned long ScoreAward3 = 0;
unsigned long PersonalGoal = 0;

byte ScoreAwardStates[4] = {0, 0, 0, 0};

unsigned long CoinsChute1 = 0;
unsigned long CoinsChute2 = 0;
unsigned long CoinsChute3 = 0;

unsigned long HighScore = 0;
unsigned long HighScoreBeat = 0;
unsigned long TotalSkill1 = 0;
unsigned long TotalSkill2 = 0;
unsigned long TotalSkill3 = 0;
unsigned long TotalWizardModes = 0;
unsigned long TotalWizardModesBeat = 0;

byte WholeCredit = 0;

unsigned long AttractStartAnimation = 0;
unsigned long InitGamePlayAnimation = 0;
unsigned long InitNewBallAnimation = 0;
unsigned long SkillShotScoreAnimation = 0;
unsigned long SaucerAnimation = 0;
unsigned long InitWizardModeAnimation = 0;
unsigned long MingAttackAnimation = 0;
unsigned long MingDefeatCelebrationBPM = 0;

byte AttractHeadMode = 255;
byte AttractPlayfieldMode = 255;
unsigned long AttractSweepTime = 0;
unsigned long AttractSirenTime = 0;
byte AttractSweepLights = 1;
byte AttractSirenLights = 1;
unsigned long AttractCalloutTimer = 0;
byte NormalGameplayBackglassMode = 255;

byte CurrentPlayer = 0;
byte CurrentBallInPlay = 1;
byte CurrentNumPlayers = 0;
unsigned long CurrentScores[4];
boolean BallDrained = false;

byte NumTiltWarnings = 0;
#define MAX_TILT_WARNINGS 1

byte SkillShotHits = 0;
byte SkillShotState = 0; // 0 not started, 1 started, 2 collecting, 3 collected/fail

#define TIME_TO_WAIT_FOR_BALL 1000
unsigned long BallTimeInTrough = 0;
unsigned long BallFirstSwitchHitTime = 0;
boolean PFValidated = false;
boolean BallSaveUsed = false; // not used
unsigned long BallSaveNumSeconds = 0; // not used

unsigned long InlaneLeftBlip = 0;
unsigned long InlaneRightBlip = 0;
unsigned long TargetUpperBlip = 0;
unsigned long TargetLowerTopBlip = 0;
unsigned long TargetLowerBottomBlip = 0;

byte DTarget3Lights[4][3] = { // [4 Players][3 Targets]: 0 = unhit, 1 = hit, 2+ undefined
                            {0, 0, 0},
                            {0, 0, 0},
                            {0, 0, 0},
                            {0, 0, 0}
                            };

byte DTarget3Flashing[4]  = {0, 0, 0, 0};


byte DTarget4Light[4][4] = { // [4 Players][4 Targets]: 0 off, 1 flashing, 2 hurry up, 3 collected
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                            }; 

#define DTARGET_4_HURRY_UP_TIMER 15000;
unsigned long WhiteHurryUpStop = 0xFFFFFFFF;
unsigned long AmberHurryUpStop = 0xFFFFFFFF;

byte    DropTargetCount = 0;
boolean DTargetInlineGoal[4] = {0, 0, 0, 0}; // Yes or No
boolean DTarget3Goal[4]      = {0, 0, 0, 0}; // Yes or No
boolean DTarget4Goal[4]      = {0, 0, 0, 0}; // Yes or No

boolean LeftSpinnerHurryUpLit = false;
boolean SpinnerMissed = false;
unsigned long LeftSpinnerHurryUpTimer = 0;
boolean RightSpinnerHurryUpLit = false;
unsigned long RightSpinnerHurryUpTimer = 0;
boolean LeftSpinnerLit = false;
boolean RightSpinnerLit = false;

byte    DTarget3Completions = 0; // Number of times upper level drop targets dropped
boolean TopPopLit = false;
boolean MiniBonusReady[4] = {0, 0, 0, 0}; // Yes or No
boolean TopSpecialLit[4]  = {0, 0, 0, 0}; // Yes or No

boolean FreeBallCollected[4] = {0, 0, 0, 0}; // Yes or No
boolean FreeGameCollected[4] = {0, 0, 0, 0}; // Yes or No
// byte  WoodBeastXBallState[4] = {0, 0, 0, 0}; // 0 off, 1 lit, 2 collected, 3 used         // Remove control on extra balls, replace with points for subsequent hits
// byte     SaucerXBallState[4] = {0, 0, 0, 0}; // 0 off, 1 lit, 2 collected, 3 undefined    // Remove control on extra balls, replace with points for subsequent hits
boolean PlayerShootsAgain = false;
boolean OutLanesLit[4] = {0, 0, 0, 0}; // Yes or No;
boolean OutLanesCollected[4] = {0, 0, 0, 0}; // Yes or No;
byte WizardState[4] = {0, 0, 0, 0}; // 0 unqualified, 1 qualified, 2 collected, 3 initball, 4 started, 5 validated, 6 ming defeated

byte MingAttackProgress = 0;
boolean MingAttackReady = false;
byte MingHealth = 3;
byte MingAttackLamps = 1;
boolean MingDefeatCelebration = false;
byte MingDefeatCelebrationIncrement = 1;

byte SaucerLamps = 1;
unsigned long SaucerHitTime = 0;
byte SaucerDirection = 0;
boolean BallInSaucer = false;

byte BonusXState[4] = {1, 1, 1, 1}; // Multiple of bonus: 1x, 2x, 3x, 4x, or 5x
#define PLAYFIELD_X_TIMER 15000;
byte Playfield2xState[4] = {0, 0, 0, 0}; // 0 off, 1 Ready (Lit), 2 Started, 3 Complete
byte Playfield3xState[4] = {0, 0, 0, 0}; // 0 off, 1 Ready (Lit), 2 Started, 3 Complete
unsigned long Playfield2XStop = 0;
unsigned long Playfield3XStop = 0;

#define MAX_MINI_BONUS 19
#define MAX_MINI_DISPLAY_BONUS 19
#define MAX_SUPER_BONUS 19
#define MAX_SUPER_DISPLAY_BONUS 19
byte MiniBonus;
byte SuperBonus;
byte CountdownMini = 0;
byte CountdownSuper = 0;
byte CountdownBonusX = 0;
boolean Super100kCollected = false;
boolean Mini50kCollected = false;
boolean SuperBonusReady = false;
unsigned long CountdownStartTime = 0;
unsigned long LastCountdownReportTime = 0;
unsigned long BonusCountDownEndTime = 0;
boolean MiniBonusCollecting = false;
unsigned long MiniBonusCollectTimer = 0;
boolean SuperBonusCollecting = false;
unsigned long SuperBonusCollectTimer = 0;

unsigned long MatchSequenceStartTime = 0;
unsigned long MatchDelay = 150;
byte MatchDigit = 0;
byte NumMatchSpins = 0;
byte ScoreMatches = 0;

unsigned long CurrentSoundEffectTimer = 0;
// byte squawkAndTalkByte = 0; // S&T

// DIP Switch Variables
byte dipBank[4];                           // Done!
byte dipChute1;                            // Done!
boolean dipSaucerStart10k;                 // Done!
boolean dipSaucerMemoryValues;             // Done!
boolean dip2X3XArrowMemory;                // Done!
byte dipChute3;                            // Done!
boolean dipOutlaneSpecialMemory;           // Done!
boolean dipTopSpecialMemory;               // Done!
boolean dipXBonus;                         // Done!
byte dipChute2;                            // Done!
boolean dipAttractVoice;                   // Done!
boolean dipSideTargetMemory;               // Done!
boolean dip4DropTargetMemory;              // Done!
boolean dip123ArrowsMemory;                // Done!
byte dipMaxCredits;                        // Done!
boolean dipCreditsDisplayed;               // Done!
boolean dipMatch;                          // Done!
boolean dipUnlimitedReplays;               // Done!
boolean dipUnlimitedExtraBalls;            // Done!
byte dipBallsPerGame;                      // Done!

byte CoinsperCredit[3]; // Number of coins required to earn one credit, by coin chute
byte CreditsperCoin[3]; // Number of credits received per coin, by coin chute
unsigned long GetValue;

// Extended Self-Test Values

#define MACHINE_STATE_TEST_PLAYFIELD_AWARD      -21
#define MACHINE_STATE_TEST_THRESHOLD_AWARD      -22
#define MACHINE_STATE_TEST_BACKGROUND_OFF       -23
#define MACHINE_STATE_TEST_HIGHSCORE_AWARD      -24
#define MACHINE_STATE_TEST_FREEPLAY             -25
#define MACHINE_STATE_TEST_GOAL_OFFON           -26

#define NUM_EXTENDED_SETTINGS      5

#define BSOS_PLAYFIELD_AWARD      74
#define BSOS_THRESHOLD_AWARD      75
#define BSOS_BACKGROUND_OFF       76
#define BSOS_HIGHSCORE_AWARD      77
#define BSOS_FREEPLAY             78
#define BSOS_GOAL_OFFON           79

byte PlayfieldAward;
byte ThresholdAward;
boolean backgroundOn;
byte HighScoreAward;
boolean FreePlay;
boolean PersonalGoalOn;

unsigned long ExtLastSelfTestChange = 0;
byte ExtSavedValue = 0;


// #################### SETUP ####################
void setup() {
  if (DEBUG_MESSAGES) {
    Serial.begin(115200);
  }

  // read analog pin for match sequence
  randomSeed(analogRead(A6));

  // Tell the OS about game-specific lights and switches
  BSOS_SetupGameSwitches(NUM_SWITCHES_WITH_TRIGGERS, NUM_PRIORITY_SWITCHES_WITH_TRIGGERS, TriggeredSwitches);

  if (DEBUG_MESSAGES) {
    Serial.write("Attempting to initialize the MPU\n");
  }
 
  // Set up the chips and interrupts
  BSOS_InitializeMPU();
  BSOS_DisableSolenoidStack();
  BSOS_SetDisableFlippers(true);

  // Get all dip variables
  dipBank[0] = BSOS_GetDipSwitches(0);
  dipBank[1] = BSOS_GetDipSwitches(1);
  dipBank[2] = BSOS_GetDipSwitches(2);
  dipBank[3] = BSOS_GetDipSwitches(3);

  dipChute1 = dipBank[0] & 31;
  dipSaucerStart10k = (dipBank[0] & 32) == 32;
  dipSaucerMemoryValues = (dipBank[0] & 64) == 64;
  dip2X3XArrowMemory = (dipBank[0] & 128) == 128;

  dipChute3 = dipBank[1] & 31;
  dipOutlaneSpecialMemory = (dipBank[1] & 32) == 32;
  dipTopSpecialMemory = (dipBank[1] & 64) == 64;
  dipXBonus = (dipBank[1] & 128) == 128;

  dipChute2 = dipBank[2] & 15;
  dipAttractVoice = (dipBank[2] & 16) == 16;
  dipSideTargetMemory = (dipBank[2] & 32) == 32;
  dip4DropTargetMemory = (dipBank[2] & 64) == 64;
  dip123ArrowsMemory = (dipBank[2] & 128) == 128;

  dipMaxCredits = 10 + 5 * (dipBank[3] & 3);
  if (dipMaxCredits > 15) dipMaxCredits = 25 + 3 * (dipMaxCredits - 20); // Max credits either 10, 15, 25, or 40
  dipCreditsDisplayed = (dipBank[3] & 4) == 4;
  dipMatch = (dipBank[3] & 8) == 8;
  dipUnlimitedReplays = (dipBank[3] & 16) == 16;
  dipUnlimitedExtraBalls = (dipBank[3] & 32) == 32;
  dipBallsPerGame = 3 + 2 * ((dipBank[3] & 192) >> 6);
  if (dipBallsPerGame > 5) dipBallsPerGame = 11 - dipBallsPerGame; // Balls per game is 5, 4, 3, or 2 for dips = 1, 2, 0, or 3 for some reason

  // Calculate credits per coin, and coins per credit. Some of the obscure ones are ignored, or modified.

  if (dipChute1 <= 10){
    CreditsperCoin[0] = dipChute1 + 1;
    CoinsperCredit[0] = 1;
  }
  else if (dipChute1 <= 21){
    CreditsperCoin[0] = dipChute1 - 10;
    CoinsperCredit[0] = 2;
  } else{
    CreditsperCoin[0] = 1;
    CoinsperCredit[0] = 1;
  }

  if (dipChute3 <= 10){
    CreditsperCoin[2] = dipChute3 + 1;
    CoinsperCredit[2] = 1;
  }
  else if (dipChute3 <= 21){
    CreditsperCoin[2] = dipChute3 - 10;
    CoinsperCredit[2] = 2;
  } else{
    CreditsperCoin[2] = 1;
    CoinsperCredit[2] = 1;
  }
  
  if (dipChute2 == 0){
    CreditsperCoin[1] = CreditsperCoin[0];
    CoinsperCredit[1] = CoinsperCredit[0];
  }
  else 
    CreditsperCoin[1] = dipChute2;
    CoinsperCredit[1] = 1;

  BSOS_SetDisplay(0, floor(VERSION_NUMBER), true, 2);
  BSOS_SetDisplayCredits(100 * (VERSION_NUMBER - floor(VERSION_NUMBER)));
  BSOS_SetDisplayBlank(1, 0x00);
  BSOS_SetDisplayBlank(2, 0x00);
  BSOS_SetDisplayBlank(3, 0x00);

    delay(4000);

  Credits = BSOS_ReadByteFromEEProm(BSOS_CREDITS_EEPROM_BYTE);
  if (Credits > dipMaxCredits) Credits = dipMaxCredits;
  BSOS_SetLampState(LA_CREDIT_INDICATOR, Credits > 0);

  TotalPlays           = BSOS_ReadULFromEEProm(BSOS_TOTAL_PLAYS_EEPROM_START_BYTE, 0);
  TotalReplays         = BSOS_ReadULFromEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, 0);
  ScoreAward1          = BSOS_ReadULFromEEProm(BSOS_AWARD_SCORE_1_EEPROM_START_BYTE, 0); // 600,000
  ScoreAward2          = BSOS_ReadULFromEEProm(BSOS_AWARD_SCORE_2_EEPROM_START_BYTE, 0); // 1,200,000
  ScoreAward3          = BSOS_ReadULFromEEProm(BSOS_AWARD_SCORE_3_EEPROM_START_BYTE, 0); // 2,800,000 
  PersonalGoal         = BSOS_ReadULFromEEProm(BSOS_PERSONAL_GOAL_EEPROM_START_BYTE, 500000); // 500,000
  HighScore            = BSOS_ReadULFromEEProm(BSOS_HIGHSCORE_EEPROM_START_BYTE, 1000);
  HighScoreBeat        = BSOS_ReadULFromEEProm(BSOS_TOTAL_HISCORE_BEATEN_START_BYTE, 0);
  TotalSkill1          = BSOS_ReadULFromEEProm(BSOS_TOTAL_SKILL_1_EEPROM_BYTE, 0);
  TotalSkill2          = BSOS_ReadULFromEEProm(BSOS_TOTAL_SKILL_2_EEPROM_BYTE, 0);
  TotalSkill3          = BSOS_ReadULFromEEProm(BSOS_TOTAL_SKILL_3_EEPROM_BYTE, 0);
  TotalWizardModes     = BSOS_ReadULFromEEProm(BSOS_TOTAL_WIZ_EEPROM_BYTE, 0);
  TotalWizardModesBeat = BSOS_ReadULFromEEProm(BSOS_TOTAL_WIZ_BEAT_EEPROM_BYTE, 0);
  CoinsChute1          = BSOS_ReadULFromEEProm(BSOS_CHUTE_1_COINS_START_BYTE, 0);
  CoinsChute2          = BSOS_ReadULFromEEProm(BSOS_CHUTE_2_COINS_START_BYTE, 0);
  CoinsChute3          = BSOS_ReadULFromEEProm(BSOS_CHUTE_3_COINS_START_BYTE, 0);
  PlayfieldAward       = BSOS_ReadByteFromEEProm(BSOS_PLAYFIELD_AWARD) & 3;
  ThresholdAward       = BSOS_ReadByteFromEEProm(BSOS_THRESHOLD_AWARD) & 3;
  HighScoreAward       = BSOS_ReadByteFromEEProm(BSOS_HIGHSCORE_AWARD) & 3;
  backgroundOn         = BSOS_ReadByteFromEEProm(BSOS_BACKGROUND_OFF) & 1;
  FreePlay             = BSOS_ReadByteFromEEProm(BSOS_FREEPLAY) & 1;
  PersonalGoalOn       = BSOS_ReadByteFromEEProm(BSOS_GOAL_OFFON) & 1;

  if (DEBUG_MESSAGES) Serial.write("Done with setup\n");
}



// #################### PLAY SOUND ####################
void PlaySound(byte soundByte) {
  // don't interupt currently playing sound
  if (CurrentTime<CurrentSoundEffectTimer) return;
  // if (BSOS_ReadSingleSwitchState(SW_OUTHOLE) && (soundByte==8 || soundByte==9)) return;
  if (BallDrained==true && (soundByte==7 || soundByte==8 || soundByte==9)) return;
  if ((SuperBonusCollecting==true) && (soundByte!=12)) return;
  if ((MiniBonusCollecting==true) && (soundByte!=12)) return;

  // if no timed sound is being played, play triggered sound
  if (soundByte > 51)
    CurrentSoundEffectTimer = CurrentTime;
  else
    CurrentSoundEffectTimer = CurrentTime + ((unsigned long)SoundTimings[soundByte] * 250); // 250 = 0.25 seconds
//    if (SoundTimings[soundByte]) CurrentSoundEffectTimer = CurrentTime + ((unsigned long)SoundTimings[soundByte] * 250); // 250 = 0.25 seconds
  BSOS_PlaySoundSquawkAndTalk(soundByte);
}



// #################### ADD CREDIT ####################
void AddCredit(byte Chute, byte CredperCoin, byte CoinperCred, byte creds = 0) {
  if (creds != 0) {
    Credits = Credits + creds;
  }
  else {
    WholeCredit += 1;
    if (WholeCredit < CoinperCred) {
      PlaySound(max(35, 31 + WholeCredit)); // Ascending dings
    } else 
    {
      Credits += CredperCoin;
      PlaySound(50); // Ming laugh!
      WholeCredit = 0;

    }
  }
  if (Credits > 0) BSOS_SetLampState(LA_CREDIT_INDICATOR, 1);
  if (Credits > dipMaxCredits) Credits = dipMaxCredits;
  BSOS_WriteByteToEEProm(BSOS_CREDITS_EEPROM_BYTE, Credits);
  BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
  BSOS_SetCoinLockout((Credits < dipMaxCredits) ? false:true);
}



// #################### ADD PLAYER ####################
boolean AddPlayer(boolean resetNumPlayers = false) {
  if (Credits<1 && !FreePlay) return false;
  if (resetNumPlayers) CurrentNumPlayers = 0;
  if (CurrentNumPlayers>=4) return false;

  CurrentNumPlayers += 1;
  BSOS_SetDisplay(CurrentNumPlayers-1, 0);
  BSOS_SetDisplayBlank(CurrentNumPlayers-1, 0x60);
  TotalPlays++;
  BSOS_WriteULToEEProm(BSOS_TOTAL_PLAYS_EEPROM_START_BYTE, TotalPlays);
  // if (CurrentNumPlayers >= 2) {
  // PlaySound(18); // saucer 0
  // }
  if (!FreePlay) {
    Credits -= 1;
    PlaySound(18); // "Flash!" on S&T. Currently no sound for Geeteoh because of long coin drop sound.
    BSOS_WriteByteToEEProm(BSOS_CREDITS_EEPROM_BYTE, Credits);
    BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
    BSOS_SetLampState(LA_CREDIT_INDICATOR, Credits > 0);
  }
  else
    PlaySound(50); // Ming laugh! 
  return true;
}



// #################### INIT NEW BALL ####################
int InitNewBall(bool curStateChanged, int ballNum) {

  if (curStateChanged) {
    InitNewBallAnimation = CurrentTime+2058;

    boolean FirstBall = (ballNum == 1 && !PlayerShootsAgain);
    
    if (FirstBall) PlaySound(44); // Gordon's alive! Only on first ball

    BSOS_TurnOffAllLamps();
    if (Credits > 0) BSOS_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    BSOS_SetDisableFlippers(false);
    BSOS_EnableSolenoidStack();

    BSOS_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime + 500);
    BSOS_PushToTimedSolenoidStack(SO_DTARGET_3_RESET, 15, CurrentTime + 500);
    BSOS_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime + 500);
    BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_DOWN, 15, CurrentTime + 500);

    BallDrained = false;

    SkillShotHits = 0;
    SkillShotState = 0;

    PFValidated = false;

    InlaneLeftBlip = 0;
    InlaneRightBlip = 0;
    TargetUpperBlip = 0;
    TargetLowerTopBlip = 0;
    TargetLowerBottomBlip = 0;

    // Upper 3 targets: Reset only if DIP switch not set, or ball 1, or all 3 hit
    if (!dip123ArrowsMemory || FirstBall || (DTarget3Lights[CurrentPlayer][0] && DTarget3Lights[CurrentPlayer][1] && DTarget3Lights[CurrentPlayer][2])) {
      for (byte i = 0; i < 3; i++) DTarget3Lights[CurrentPlayer][i] = 0;
      DTarget3Flashing[CurrentPlayer] = 0;
      }

    // Side Targets, flipper lanes, and 4 drop targets: Reset NONE, ONLY drop targets, or BOTH drop targets and colour-matched targets.
    if ((DTarget4Light[CurrentPlayer][0] == 3 && DTarget4Light[CurrentPlayer][1] == 3 && DTarget4Light[CurrentPlayer][2] == 3 && DTarget4Light[CurrentPlayer][3] == 3)
      || FirstBall || (!dipSideTargetMemory && !dip4DropTargetMemory))
      for (byte i = 0; i < 4; i++) DTarget4Light[CurrentPlayer][i] = 0; // First ball, no memory, or all collected, set all to zero
    else {
      for (byte i = 0; i < 4; i++) {
        if (DTarget4Light[CurrentPlayer][i] <= 1) DTarget4Light[CurrentPlayer][i] = 0; // Drop target set to off if flashing
        else
        if (!dipSideTargetMemory && DTarget4Light[CurrentPlayer][i] == 3) DTarget4Light[CurrentPlayer][i] = 2; // if collected but only drop target memory, set to hurry up
      }
    }
    WhiteHurryUpStop = 0xFFFFFFFF;
    AmberHurryUpStop = 0xFFFFFFFF;
 
    DropTargetCount = 0;

    // Saucer goals: Reset if DIP switch not set, or ball 1
    if (!dipSaucerMemoryValues || FirstBall) {
      DTargetInlineGoal[CurrentPlayer] = false;
      DTarget3Goal[CurrentPlayer] = false;
      DTarget4Goal[CurrentPlayer] = false;
      }

    // One free ball per game
    if (dipUnlimitedExtraBalls || FirstBall) FreeBallCollected[CurrentPlayer] = false;

    // One free game per game
    if (dipUnlimitedReplays || FirstBall) FreeGameCollected[CurrentPlayer] = false;
   
    LeftSpinnerHurryUpLit = false;
    SpinnerMissed = false;
    LeftSpinnerHurryUpTimer = 0;
    RightSpinnerHurryUpLit = false;
    RightSpinnerHurryUpTimer = 0;
    LeftSpinnerLit = false;
    RightSpinnerLit = false;

    // Outlanes: Reset to zero only if DIP switch not set, or first ball, outlanes collected
    if (!dipOutlaneSpecialMemory || FirstBall || OutLanesCollected[CurrentPlayer]) {
      OutLanesLit[CurrentPlayer] = false;
      OutLanesCollected[CurrentPlayer] = false;
      WizardState[CurrentPlayer] = 0;
      } else {
        if (OutLanesLit[CurrentPlayer]) OutLanesCollected[CurrentPlayer] = true;
    }
    MingAttackProgress = 0;
    MingAttackReady = false;
    MingHealth = 3;
    MingDefeatCelebration = false;
 
    // Top Target Special: Reset if DIP switch not set, or ball 1
    if (!dipTopSpecialMemory || FirstBall) {
      TopSpecialLit[CurrentPlayer] = false;
      MiniBonusReady[CurrentPlayer] = false;
    }
    // The rest of these for top target are reset to zero!
    TopPopLit = false;
    DTarget3Completions = 0;

    SkillShotScoreAnimation = 0;
    SaucerAnimation = 0;
    MingAttackAnimation = 0;
    MingDefeatCelebrationBPM = 0;
    MingDefeatCelebrationIncrement = 1;
    InitWizardModeAnimation = 0;

//    if (WoodBeastXBallState[CurrentPlayer]==2) {
//      WoodBeastXBallState[CurrentPlayer] = 3;
//    } else 
//    if (WoodBeastXBallState[CurrentPlayer]==1) {
//      WoodBeastXBallState[CurrentPlayer] = 0;
//    }
//    if (SaucerXBallState[CurrentPlayer]==1) {
//      SaucerXBallState[CurrentPlayer] = 0;
//    }

    SaucerLamps = 1;
    SaucerAnimation = 0;
    SaucerHitTime = 0;
    SaucerDirection = 0;

    // 2X/3X saucer arrows: Reset only if DIP switch not set, or ball 1 
    if (!dip2X3XArrowMemory || FirstBall) {
      Playfield2xState[CurrentPlayer] = 0;
      Playfield3xState[CurrentPlayer] = 0;
    }
    else {
      if (Playfield2xState[CurrentPlayer] != 1) Playfield2xState[CurrentPlayer] = 0;
      if (Playfield3xState[CurrentPlayer] != 1) Playfield3xState[CurrentPlayer] = 0;
    }
    Playfield2XStop = 0;
    Playfield3XStop = 0;
    BallInSaucer = 0;

    CountdownMini = 0;
    CountdownSuper = 0;
    Super100kCollected = false;
    Mini50kCollected = false;
    SuperBonusReady = false;

    // 2X/3X/4X/5X: Reset only if DIP switch not set, or ball 1
    if (!dipXBonus || FirstBall) {
      BonusXState[CurrentPlayer] = 1;
      CountdownBonusX = 0;
    }
    else CountdownBonusX = BonusXState[CurrentPlayer];

    // BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);

    // if (BSOS_ReadSingleSwitchState(SW_OUTHOLE)) {
    //   BSOS_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
    // }
    // BSOS_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);

    for (byte count=0; count<CurrentNumPlayers; count++) {
      BSOS_SetDisplay(count, CurrentScores[count], true, 2);
    }

    BSOS_SetDisplayBallInPlay(ballNum);
    BSOS_SetLampState(LA_BALL_IN_PLAY, 1);
    BSOS_SetLampState(LA_TILT, 0);
    if (PlayerShootsAgain==true) {
      BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
      BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
    }

    if (BallSaveNumSeconds>0) {
      BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1, 0, 500);
    }
  }

  MiniBonus = 0;
  SuperBonus = 0;
  NumTiltWarnings = 0;

  // We should only consider the ball initialized when 
  // the ball is no longer triggering the SW_OUTHOLE
  // if (BSOS_ReadSingleSwitchState(SW_OUTHOLE)) {
  //   return MACHINE_STATE_INIT_NEW_BALL;
  // } else {
  //   //return MACHINE_STATE_NORMAL_GAMEPLAY;
  //   if (CurrentTime>=InitNewBallAnimation) {
  //     PlaySound(6);
  //     return MACHINE_STATE_SKILL_SHOT;
  //   } else {
  //     return MACHINE_STATE_INIT_NEW_BALL;
  //   }
  // }

  if (backgroundOn) PlaySound(6); // Background music (repeats until end of game. Does not restart when called again.)
  if (CurrentTime>=InitNewBallAnimation) {
    BSOS_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
    return MACHINE_STATE_SKILL_SHOT;
  } else {
    return MACHINE_STATE_INIT_NEW_BALL;
  }
}



// #################### RUN SELF TEST ####################
int RunSelfTest(int curState, boolean curStateChanged) {
  int returnState = curState;
  CurrentNumPlayers = 0;
  // Any state that's greater than CHUTE_3 is handled by the Base Self-test code
  // Any that's less, is machine specific, so we handle it here.

  if (curState >= MACHINE_STATE_TEST_DONE) { 
    returnState = RunBaseSelfTest(returnState, curStateChanged, CurrentTime, SW_CREDIT_BUTTON);
    
    // Update all local variables with information updated in self test.
    Credits = BSOS_ReadByteFromEEProm(BSOS_CREDITS_EEPROM_BYTE);
    if (Credits > dipMaxCredits) {
      Credits = dipMaxCredits;
      BSOS_WriteByteToEEProm(BSOS_CREDITS_EEPROM_BYTE, Credits & 0x000000FF);
    }
    BSOS_SetLampState(LA_CREDIT_INDICATOR, Credits > 0);
    
    TotalPlays    = BSOS_ReadULFromEEProm(BSOS_TOTAL_PLAYS_EEPROM_START_BYTE);
    TotalReplays  = BSOS_ReadULFromEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE);
    ScoreAward1   = BSOS_ReadULFromEEProm(BSOS_AWARD_SCORE_1_EEPROM_START_BYTE);
    ScoreAward2   = BSOS_ReadULFromEEProm(BSOS_AWARD_SCORE_2_EEPROM_START_BYTE);
    ScoreAward3   = BSOS_ReadULFromEEProm(BSOS_AWARD_SCORE_3_EEPROM_START_BYTE);
    HighScore     = BSOS_ReadULFromEEProm(BSOS_HIGHSCORE_EEPROM_START_BYTE);
    PersonalGoal  = BSOS_ReadULFromEEProm(BSOS_PERSONAL_GOAL_EEPROM_START_BYTE);
    HighScoreBeat = BSOS_ReadULFromEEProm(BSOS_TOTAL_HISCORE_BEATEN_START_BYTE);
    CoinsChute1   = BSOS_ReadULFromEEProm(BSOS_CHUTE_1_COINS_START_BYTE);
    CoinsChute2   = BSOS_ReadULFromEEProm(BSOS_CHUTE_2_COINS_START_BYTE);
    CoinsChute3   = BSOS_ReadULFromEEProm(BSOS_CHUTE_3_COINS_START_BYTE);

    ExtLastSelfTestChange = GetLastSelfTestChangedTime(); // Coordinate LastSelfTestChange and ExtLastSelfChange

  // Basic self tests are handled above. Self testing can be extended in the following code.
  } 
  else if (curState >= MACHINE_STATE_TEST_DONE - NUM_EXTENDED_SETTINGS) {
    returnState = RunExtendedSelfTest(returnState, curStateChanged, CurrentTime, SW_CREDIT_BUTTON);

    PlayfieldAward   = BSOS_ReadByteFromEEProm(BSOS_PLAYFIELD_AWARD) & 3;
    ThresholdAward   = BSOS_ReadByteFromEEProm(BSOS_THRESHOLD_AWARD) & 3;
    backgroundOn     = BSOS_ReadByteFromEEProm(BSOS_BACKGROUND_OFF) & 1;
    HighScoreAward   = BSOS_ReadByteFromEEProm(BSOS_HIGHSCORE_AWARD) & 3;
    FreePlay         = BSOS_ReadByteFromEEProm(BSOS_FREEPLAY) & 1;
    PersonalGoalOn   = BSOS_ReadByteFromEEProm(BSOS_GOAL_OFFON) & 1; // Feature not ready yet.

    SetLastSelfTestChangedTime(ExtLastSelfTestChange); // Coordinate LastSelfTestChange and ExtLastSelfChange
  }
  else returnState = MACHINE_STATE_ATTRACT;
  return returnState;
}



// #################### TURN OFF ATTRACT LAMPS ####################
void TurnOffAttractLamps() {
  for (int count=0; count<BSOS_MAX_LAMPS; count++) {
    if (count==40) {
      count = 42;
    } else if (count==43) {
      count = 44;
    } else if (count==48) {
      count = 52;
    }
    BSOS_SetLampState(count, 0, 0, 0);
  }
}



// #################### RUN ATTRACT MODE ####################
int RunAttractMode(int curState, boolean curStateChanged) {

  int returnState = curState;

  // If this is the first time in the attract mode loop
  if (curStateChanged) {
    // reset eeproms
    // BSOS_WriteByteToEEProm(BSOS_CREDITS_EEPROM_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_PLAYS_EEPROM_START_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_AWARD_SCORE_1_EEPROM_START_BYTE, 600000);
    // BSOS_WriteULToEEProm(BSOS_AWARD_SCORE_2_EEPROM_START_BYTE, 1000000);
    // BSOS_WriteULToEEProm(BSOS_AWARD_SCORE_3_EEPROM_START_BYTE, 1500000);
    // BSOS_WriteULToEEProm(BSOS_HIGHSCORE_EEPROM_START_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_HISCORE_BEATEN_START_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_SKILL_1_EEPROM_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_SKILL_2_EEPROM_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_SKILL_3_EEPROM_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_WIZ_EEPROM_BYTE, 0);
    // BSOS_WriteULToEEProm(BSOS_TOTAL_WIZ_BEAT_EEPROM_BYTE, 0);

    // Run first time after four seconds
    if (dipAttractVoice) AttractCalloutTimer = CurrentTime - ATTRACT_SPEECH_TIMER + 4000;
    BSOS_TurnOffAllLamps();
    if (Credits > 0) BSOS_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    BSOS_DisableSolenoidStack();
    BSOS_SetDisableFlippers(true);
    if (DEBUG_MESSAGES) {
      Serial.println(F("Entering Attract Mode"));
    }
    for (byte count=0; count<5; count++) {
      BSOS_SetDisplayBlank(count, 0x00);
    }
    // if (FirstStartup==true) {
    //   BSOS_SetDisplay(0, VERSION_NUMBER, true, 2);
    //   FirstStartup = false;
    // }
    if (!FreePlay) {
      BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
    }
    BSOS_SetLampState(LA_GAME_OVER, 1);
    BSOS_SetDisplayBallInPlay(0);
    AttractStartAnimation = CurrentTime+2500;
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 100);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    AttractHeadMode = 255;
    AttractPlayfieldMode = 255;
    PlaySound(8); // Spinner noise
  }

  if (dipAttractVoice) {
    if (CurrentTime >= AttractCalloutTimer + ATTRACT_SPEECH_TIMER + AttractWait) {
      #ifdef BALLY_STERN_OS_USE_GEETEOH 
      PlaySound(254); // This way Flash, come on!
      #else
      PlaySound(44); // Emperor Ming awaits
      #endif
      AttractCalloutTimer = CurrentTime;
      AttractWait = 0;
    }
  }

  if (CurrentTime>=AttractStartAnimation) {
    // Alternate displays between high score and blank
    if ((CurrentTime/6000)%2==0) {

      if (AttractHeadMode!=1) {
        BSOS_SetLampState(LA_HIGH_SCORE_TO_DATE, 1);

        for (byte count=0; count<4; count++) {
          BSOS_SetDisplay(count, HighScore, true, 2);
        }
        if (!FreePlay) {
          BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
        }
        BSOS_SetDisplayBallInPlay(0, true);
      }
      AttractHeadMode = 1;

    } else {
      if (AttractHeadMode!=2) {
        BSOS_SetLampState(LA_HIGH_SCORE_TO_DATE, 0);
        if (!FreePlay) {
          BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
        }
        BSOS_SetDisplayBallInPlay(0, true);
        for (byte count=0; count<4; count++) {
          if (CurrentNumPlayers>0) {
            if (count<CurrentNumPlayers) {
              BSOS_SetDisplay(count, CurrentScores[count], true, 2); 
            } else {
              BSOS_SetDisplay(count, 0);
              BSOS_SetDisplayBlank(count, 0xE0);
            }
          } else {
            BSOS_SetDisplay(count, 0);
            BSOS_SetDisplayBlank(count, 0xE0);
          }
        }
      }
      AttractHeadMode = 2;
    }

    // MAIN ATTRACT MODE - 4 MODES AT 7 SECONDS
    if ((CurrentTime/7000)%4==0) {
      if (AttractPlayfieldMode!=1) {
        TurnOffAttractLamps();
      }
      AttractRetro();
      AttractPlayfieldMode = 1;
    } else if ((CurrentTime/7000)%4==1) {
      if (AttractPlayfieldMode!=2) {
        TurnOffAttractLamps();
      }
      if ((CurrentTime-AttractSweepTime)>25) { // 50 (15 count right now)
        AttractSweepTime = CurrentTime;
        for (byte lightcountdown=0; lightcountdown<NUM_OF_ATTRACT_LAMPS_DOWN; lightcountdown++) {
          byte dist = AttractSweepLights - AttractLampsDown[lightcountdown].rowDown;
          BSOS_SetLampState(AttractLampsDown[lightcountdown].lightNumDown, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsDown[lightcountdown].lightNumDown):0);
          if (lightcountdown==(NUM_OF_ATTRACT_LAMPS_DOWN/2)) BSOS_DataRead(0);
        }
        BSOS_DataRead(0);
        for (byte lightcountup=0; lightcountup<NUM_OF_ATTRACT_LAMPS_UP; lightcountup++) {
          byte dist = AttractSweepLights - AttractLampsUp[lightcountup].rowUp;
          BSOS_SetLampState(AttractLampsUp[lightcountup].lightNumUp, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsUp[lightcountup].lightNumUp):0);
          if (lightcountup==(NUM_OF_ATTRACT_LAMPS_UP/2)) BSOS_DataRead(0);
        }
        AttractSweepLights += 1;
        if (AttractSweepLights>24) AttractSweepLights = 0; // 49
      }
      backglassLampsCenterOut();
      AttractPlayfieldMode = 2;
    } else if ((CurrentTime/7000)%4==2) {
      if (AttractPlayfieldMode!=3) {
        TurnOffAttractLamps();
      }
      AttractRetro();
      AttractPlayfieldMode = 3;
    } else {
      if (AttractPlayfieldMode!=4) {
        TurnOffAttractLamps();
      }
      if ((CurrentTime-AttractSirenTime)>33) { // 50 (15 count right now)
        AttractSirenTime = CurrentTime;
        for (byte lightcountsiren=0; lightcountsiren<NUM_OF_ATTRACT_LAMPS_SIREN; lightcountsiren++) {
          byte dist = AttractSirenLights - AttractLampsSiren[lightcountsiren].rowSiren;
          BSOS_SetLampState(AttractLampsSiren[lightcountsiren].lightNumSiren, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsSiren[lightcountsiren].lightNumSiren):0);
          if (lightcountsiren==(NUM_OF_ATTRACT_LAMPS_SIREN/2)) BSOS_DataRead(0);
        }
        BSOS_DataRead(0);
        AttractSirenLights += 1;
        if (AttractSirenLights>33) AttractSirenLights = 0; // 49
      }
      BackglassLampsKnightRider();
      AttractPlayfieldMode = 4;
    }

    byte switchHit;
    while ( (switchHit=BSOS_PullFirstFromSwitchStack())!=SWITCH_STACK_EMPTY ) {
      if (switchHit==SW_CREDIT_BUTTON) {
        if (AddPlayer(true)) returnState = MACHINE_STATE_INIT_GAMEPLAY;
      }
      if (switchHit==SW_COIN_1) {
        if (!FreePlay) {
          AddCredit(0, CreditsperCoin[0], CoinsperCredit[0]);
          BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
          CoinsChute1 += 1;
          BSOS_WriteByteToEEProm(BSOS_CHUTE_1_COINS_START_BYTE, CoinsChute1);
        }
      }
      if (switchHit==SW_COIN_2) {
        if (!FreePlay) {
          AddCredit(1, CreditsperCoin[1], CoinsperCredit[1]);
          BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
          CoinsChute3 += 1;
          BSOS_WriteByteToEEProm(BSOS_CHUTE_2_COINS_START_BYTE, CoinsChute2);
        }
      }
      if (switchHit==SW_COIN_3) {
        if (!FreePlay) {
          AddCredit(2, CreditsperCoin[2], CoinsperCredit[2]);
          BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
          CoinsChute3 += 1;
          BSOS_WriteByteToEEProm(BSOS_CHUTE_3_COINS_START_BYTE, CoinsChute3);
        }
      }
      if (switchHit==SW_SELF_TEST_SWITCH && (CurrentTime-GetLastSelfTestChangedTime())>500) {
        returnState = MACHINE_STATE_TEST_LIGHTS;
        SetLastSelfTestChangedTime(CurrentTime);
      }
      #ifndef BALLY_STERN_OS_USE_GEETEOH 
      if (switchHit==SW_REBOUND) PlaySound(5); // sound off
      #endif
      // if (switchHit==SW_TARGET_LRIGHT_BOTTOM) { // S&T
      //   PlaySound(5); // sound off
      //   BSOS_PlaySoundSquawkAndTalk(squawkAndTalkByte);
      //   BSOS_SetDisplayCredits(squawkAndTalkByte, true);
      //   squawkAndTalkByte++;
      // }
      // if (DEBUG_MESSAGES) {
      //   char buf[128];
      //   sprintf(buf, "Switch = 0x%02X\n", switchHit);
      //   Serial.write(buf);
      // }
    }
  }

  return returnState;
}



// #################### NORMAL GAMEPLAY ####################
int NormalGamePlay(boolean curStateChanged) {
  int returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
  BSOS_SetDisplayCredits(DropTargetCount, true);

  if (curStateChanged) {
    SuperBonusReady = false;

    // Reset rollover buttons, other lights, after skill shot
    BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
    BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
    BSOS_SetLampState(LA_STAR_SHOOTER_TOP, 0);
    BSOS_SetLampState(LA_STAR_PFIELD_TOP, 0);
    BSOS_SetLampState(LA_STAR_PFIELD_BOTTOM, 0);
    BSOS_SetLampState(LA_MING_TOP, 1);
    BSOS_SetLampState(LA_MING_BOTTOM, 1);
    
    if (PFValidated==true) BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);

  // Reset lights to be saved to next ball (according to DIP switches), after skill shot

  // 10k 20k 30k saucer lights
  if (DTarget4Goal[CurrentPlayer])      BSOS_SetLampState(LA_SAUCER_10K, 1);
  if (DTargetInlineGoal[CurrentPlayer]) BSOS_SetLampState(LA_SAUCER_20K, 1);
  if (DTarget3Goal[CurrentPlayer])      BSOS_SetLampState(LA_SAUCER_30K, 1);

  // 2X/3X saucer arrows
  if (Playfield2xState[CurrentPlayer] == 1) BSOS_SetLampState(LA_SAUCER_ARROW_2X, 1, 0, 150);
  if (Playfield3xState[CurrentPlayer] == 1) BSOS_SetLampState(LA_SAUCER_ARROW_3X, 1, 0, 150);

  // 2X/3X/4X/5X Bonus
  if (dipXBonus) ShowExtraBonusLights();

  // Top Target Mini Bonus and Special
  if (TopSpecialLit[CurrentPlayer])  BSOS_SetLampState(LA_TARGET_UPPER_SPECIAL, 1);
  if (MiniBonusReady[CurrentPlayer]) BSOS_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 1);

  // Right Side and Inner Lane Flipper Targets
  if (DTarget4Light[CurrentPlayer][3] <= 1) BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 0);
  if (DTarget4Light[CurrentPlayer][2] <= 1) BSOS_SetLampState(LA_INLANE_RIGHT, 0);
  if (DTarget4Light[CurrentPlayer][1] <= 1) BSOS_SetLampState(LA_INLANE_LEFT, 0);
  if (DTarget4Light[CurrentPlayer][0] <= 1) BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 0);

  if (DTarget4Light[CurrentPlayer][3] == 2) BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1, 0, 250);
  if (DTarget4Light[CurrentPlayer][2] == 2) BSOS_SetLampState(LA_INLANE_RIGHT, 1, 0, 250);
  if (DTarget4Light[CurrentPlayer][1] == 2) BSOS_SetLampState(LA_INLANE_LEFT, 1, 0, 250);
  if (DTarget4Light[CurrentPlayer][0] == 2) BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 1, 0, 250);

  if (DTarget4Light[CurrentPlayer][3] == 3) BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1);
  if (DTarget4Light[CurrentPlayer][2] == 3) BSOS_SetLampState(LA_INLANE_RIGHT, 1);
  if (DTarget4Light[CurrentPlayer][1] == 3) BSOS_SetLampState(LA_INLANE_LEFT, 1);
  if (DTarget4Light[CurrentPlayer][0] == 3) BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 1);

  // Outlanes
  if (OutLanesLit[CurrentPlayer]) BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 1);
  if (OutLanesLit[CurrentPlayer]) BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 1);
  }


  // handle backglass animations
  if (PFValidated==false) {
    BackglassLampsLeft2Right();
  } else if (PFValidated==true && BallInSaucer==false) {
    BackglassLampsClear();
    if ((CurrentTime/7000)%3==0) {
      if (NormalGameplayBackglassMode!=1) {
        BackglassLampsClear();
      }
      BackglassLampsKnightRider();
      NormalGameplayBackglassMode = 1;
    } else if ((CurrentTime/7000)%3==1) {
      if (NormalGameplayBackglassMode!=2) {
        BackglassLampsClear();
      }
      backglassLampsCenterOut();
      NormalGameplayBackglassMode = 2;
    } else {
      if (NormalGameplayBackglassMode!=3) {
        BackglassLampsClear();
      }
      BackglassLampsLeft2Right();
      NormalGameplayBackglassMode = 3;
    }
  }


  // handle dtarget 4 lights
  // 0 off, 1 flashing, 2 hurry up, 3 collected
  int DTarget4ToHandle = 0;
  byte lamp4Phase = (CurrentTime/2000)%2;
  for (int i = 3; i > -1; i--) {
    if      (i==0) DTarget4ToHandle = LA_DTARGET_4_A;
    else if (i==1) DTarget4ToHandle = LA_DTARGET_4_B;
    else if (i==2) DTarget4ToHandle = LA_DTARGET_4_C;
    else if (i==3) DTarget4ToHandle = LA_DTARGET_4_D;
    
    if (DTarget4Light[CurrentPlayer][i]<=1) {
      if (lamp4Phase==0) {
        if (i==0 || i==2) {
          BSOS_SetLampState(DTarget4ToHandle, 1, 0, 250);
          DTarget4Light[CurrentPlayer][i] = 1;
        } else {
          if (DTarget4Light[CurrentPlayer][0]>=2 && DTarget4Light[CurrentPlayer][2]>=2 && (DTarget4Light[CurrentPlayer][1]<=1 || DTarget4Light[CurrentPlayer][3]<=1)) {
            if (DTarget4Light[CurrentPlayer][1]<=1 && DTarget4Light[CurrentPlayer][3]<=1) {
              BSOS_SetLampState(LA_DTARGET_4_B, 1, 0, 250);
              BSOS_SetLampState(LA_DTARGET_4_D, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][1]<=1 && DTarget4Light[CurrentPlayer][3]>=2) {
              BSOS_SetLampState(LA_DTARGET_4_B, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][1]>=2 && DTarget4Light[CurrentPlayer][3]<=1) {
              BSOS_SetLampState(LA_DTARGET_4_D, 1, 0, 250);
            }
          } else {
            BSOS_SetLampState(DTarget4ToHandle, 0);
            DTarget4Light[CurrentPlayer][i] = 0;
          }
        }
      } else {
        if (i==1 || i==3) {
          BSOS_SetLampState(DTarget4ToHandle, 1, 0, 250);
          DTarget4Light[CurrentPlayer][i] = 1;
        } else {
          if (DTarget4Light[CurrentPlayer][1]>=2 && DTarget4Light[CurrentPlayer][3]>=2 && (DTarget4Light[CurrentPlayer][0]<=1 || DTarget4Light[CurrentPlayer][2]<=1)) {
            if (DTarget4Light[CurrentPlayer][0]<=1 && DTarget4Light[CurrentPlayer][2]<=1) {
              BSOS_SetLampState(LA_DTARGET_4_A, 1, 0, 250);
              BSOS_SetLampState(LA_DTARGET_4_C, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][0]<=1 && DTarget4Light[CurrentPlayer][2]>=2) {
              BSOS_SetLampState(LA_DTARGET_4_A, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][0]>=2 && DTarget4Light[CurrentPlayer][2]<=1) {
              BSOS_SetLampState(LA_DTARGET_4_C, 1, 0, 250);
            }
          } else {
            BSOS_SetLampState(DTarget4ToHandle, 0);
            DTarget4Light[CurrentPlayer][i] = 0;
          }
        }
      }
    } else if (DTarget4Light[CurrentPlayer][i]>=2) BSOS_SetLampState(DTarget4ToHandle, 1);
  }


  // handle dtarget 3 lights
  unsigned long NumberOf3LampsToStrobe = 3;
  int DTarget3ToLight = 0;
  unsigned long lamp3PhaseIncrement = 0;
  if (DTarget3Lights[CurrentPlayer][0]==1 || DTarget3Lights[CurrentPlayer][0]==2) NumberOf3LampsToStrobe--;
  if (DTarget3Lights[CurrentPlayer][1]==1 || DTarget3Lights[CurrentPlayer][1]==2) NumberOf3LampsToStrobe--;
  if (DTarget3Lights[CurrentPlayer][2]==1 || DTarget3Lights[CurrentPlayer][2]==2) NumberOf3LampsToStrobe--;
  if (NumberOf3LampsToStrobe != 0) {
    byte lamp3Phase = (CurrentTime/800)%NumberOf3LampsToStrobe;
    for (byte i = 0; i < 3; i++) {
      if (i==0) {
        DTarget3ToLight = LA_DTARGET_ARROW_1;
      } else if (i==1) {
        DTarget3ToLight = LA_DTARGET_ARROW_2;
      } else if (i==2) {
        DTarget3ToLight = LA_DTARGET_ARROW_3;
      }
      if (DTarget3Lights[CurrentPlayer][i]==1 || DTarget3Lights[CurrentPlayer][i]==2) {
        BSOS_SetLampState(DTarget3ToLight, 1);
      } else {
        BSOS_SetLampState(DTarget3ToLight, lamp3Phase==lamp3PhaseIncrement, 0, 200);
        if (lamp3Phase==lamp3PhaseIncrement) {
          DTarget3Flashing[CurrentPlayer] = DTarget3ToLight;
        }
        lamp3PhaseIncrement++;
      }
    }
  }


  // handle super bonus collect lit animation
  if (SuperBonusReady!=true) {
    BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
    BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
    BSOS_SetLampState(LA_STAR_SHOOTER_TOP, 0);
    BSOS_SetLampState(LA_STAR_PFIELD_TOP, 0);
    BSOS_SetLampState(LA_STAR_PFIELD_BOTTOM, 0);
  } else {
    byte lamp6Phase = (CurrentTime/125)%6;
    BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, lamp6Phase==4);
    BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, lamp6Phase==3||lamp6Phase==4, lamp6Phase==4);
    BSOS_SetLampState(LA_STAR_SHOOTER_TOP, lamp6Phase==2||lamp6Phase==3, lamp6Phase==3);
    BSOS_SetLampState(LA_STAR_PFIELD_TOP, lamp6Phase==1||lamp6Phase==2, lamp6Phase==2);
    BSOS_SetLampState(LA_STAR_PFIELD_BOTTOM, lamp6Phase<2, lamp6Phase==1);
  }

  // handle mini bonus ring collect animation
  if (((CurrentTime-LastCountdownReportTime)>25) && (MiniBonusCollecting==true)) { // adjust speed 300
    if (MiniBonus>0) {
      CurrentScores[CurrentPlayer] += 1000;
      MiniBonus -= 1;
      ShowMiniBonusOnLadder(MiniBonus);
      PlaySound(12); // Ascending Sound
      if (BonusXState[CurrentPlayer]>1 && MiniBonus==0) {
        MiniBonus = CountdownMini;
        BonusXState[CurrentPlayer] -= 1;
      }
    } else {
      MiniBonusCollecting = false;
      MiniBonusReady[CurrentPlayer] = false;
      BonusXState[CurrentPlayer] = CountdownBonusX;
    }
    LastCountdownReportTime = CurrentTime;
  }

  // handle super bonus ring collect animation
  if (((CurrentTime-LastCountdownReportTime)>25) && (SuperBonusCollecting==true)) { // adjust speed 300
    if (SuperBonus>0) {
      CurrentScores[CurrentPlayer] += 1000;
      SuperBonus -= 1;
      ShowSuperBonusOnLadder(SuperBonus);
      PlaySound(12); // Ascending sound
      if (BonusXState[CurrentPlayer]>1 && SuperBonus==0) {
        SuperBonus = CountdownSuper;
        BonusXState[CurrentPlayer] -= 1;
      }
    } else {
      SuperBonusCollecting = false;
      SuperBonusReady = false;
      BonusXState[CurrentPlayer] = CountdownBonusX;
      PFValidated = false;
    }
    LastCountdownReportTime = CurrentTime;
  }

  // LAMP BLIP
  if (CurrentTime>=InlaneLeftBlip && (DTarget4Light[CurrentPlayer][1]==0 || DTarget4Light[CurrentPlayer][1]==1)) {
    BSOS_SetLampState(LA_INLANE_LEFT, 0);
  }
  if (CurrentTime>=InlaneRightBlip && (DTarget4Light[CurrentPlayer][2]==0 || DTarget4Light[CurrentPlayer][2]==1)) {
    BSOS_SetLampState(LA_INLANE_RIGHT, 0);
  }
  if (CurrentTime>=TargetUpperBlip && TopSpecialLit[CurrentPlayer]==false && MiniBonusReady[CurrentPlayer]==false) {
    BSOS_SetLampState(LA_TARGET_UPPER_SPECIAL, 0);
    BSOS_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 0);
  }
  if (CurrentTime>=TargetLowerTopBlip && (DTarget4Light[CurrentPlayer][0]==0 || DTarget4Light[CurrentPlayer][0]==1)) {
    BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 0);
  }
  if (CurrentTime>=TargetLowerBottomBlip && (DTarget4Light[CurrentPlayer][3]==0 || DTarget4Light[CurrentPlayer][3]==1)) {
    BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 0);
  }

  // PLAYFIELD X STOP
  if ((Playfield2xState[CurrentPlayer]==2 && CurrentTime>=Playfield2XStop) || (Playfield3xState[CurrentPlayer]==2 && CurrentTime>=Playfield3XStop)) {
    PlaySound(11); // background sound 2 (restart). Currently no assigned sound for S&T or Geeteoh.
    if (Playfield2xState[CurrentPlayer]==2 && CurrentTime>=Playfield2XStop) {
      Playfield2xState[CurrentPlayer] = 3;
      BSOS_SetLampState(LA_CLOCK_15_SECONDS_2X, 0);
    }
    if (Playfield3xState[CurrentPlayer]==2 && CurrentTime>=Playfield3XStop) {
      Playfield3xState[CurrentPlayer] = 3;
      BSOS_SetLampState(LA_CLOCK_15_SECONDS_3X, 0);
    }
  }

  // TARGET HURRY UPS, NOT HIT IN TIME
  if (DTarget4Light[CurrentPlayer][0]==2 && CurrentTime>=WhiteHurryUpStop) {
    PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
    BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 1, 0, 250);
    WhiteHurryUpStop = 0xFFFFFFFF;
  }
  if (DTarget4Light[CurrentPlayer][3]==2 && CurrentTime>=AmberHurryUpStop) {
    PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
    BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1, 0, 250);
    AmberHurryUpStop = 0xFFFFFFFF;
  }


  // SPINNER HURRY UPS
  if (LeftSpinnerHurryUpLit==true) {
    if (CurrentTime>=LeftSpinnerHurryUpTimer) {
      if (SpinnerMissed) PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
      if (LeftSpinnerLit==true) {
        BSOS_SetLampState(LA_SPINNER_LEFT, 1);
      } else {
        BSOS_SetLampState(LA_SPINNER_LEFT, 0);
      }
      LeftSpinnerHurryUpLit = false;
      SpinnerMissed = false;
    }
  }

  if (RightSpinnerHurryUpLit==true) {
    if (CurrentTime>=RightSpinnerHurryUpTimer) {
      if (SpinnerMissed) PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
      if (RightSpinnerLit==true) {
        BSOS_SetLampState(LA_SPINNER_RIGHT, 1);
      } else {
        BSOS_SetLampState(LA_SPINNER_RIGHT, 0);
      }
      RightSpinnerHurryUpLit = false;
      SpinnerMissed = false;
    }
  }


  // HANDLE SAUCER
  if (BallInSaucer==true) {
    // if (Playfield2xState[CurrentPlayer]==1 || Playfield3xState[CurrentPlayer]==1) PlaySound(40);
    if (CurrentTime<=SaucerAnimation) {
      backglassLampsCenterOut();
      if ((CurrentTime-AttractSweepTime)>25) { // 50 (15 count right now)
        AttractSweepTime = CurrentTime;
        if (SaucerDirection == 0) {
          for (byte lightcountdown=0; lightcountdown<NUM_OF_SAUCER_LAMPS_DOWN; lightcountdown++) {
            byte dist = SaucerLamps - SaucerLampsDown[lightcountdown].rowSaucerDown;
            BSOS_SetLampState(SaucerLampsDown[lightcountdown].lightNumSaucerDown, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+SaucerLampsDown[lightcountdown].lightNumSaucerDown):0);
            if (lightcountdown==(NUM_OF_SAUCER_LAMPS_DOWN/2)) BSOS_DataRead(0);
          }
        } else {
          for (byte lightcountup=0; lightcountup<NUM_OF_SAUCER_LAMPS_UP; lightcountup++) {
            byte dist = SaucerLamps - SaucerLampsUp[lightcountup].rowSaucerUp;
            BSOS_SetLampState(SaucerLampsUp[lightcountup].lightNumSaucerUp, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+SaucerLampsUp[lightcountup].lightNumSaucerUp):0);
            if (lightcountup==(NUM_OF_SAUCER_LAMPS_UP/2)) BSOS_DataRead(0);
          }
        }
        SaucerLamps += 1;
        if (SaucerLamps>30) SaucerLamps = 0;
      }
    } else {
      if (Playfield2xState[CurrentPlayer]==1) {
        Playfield2xState[CurrentPlayer] = 2;
        Playfield2XStop = CurrentTime + PLAYFIELD_X_TIMER;
        BSOS_SetLampState(LA_SAUCER_ARROW_2X, 0);
        BSOS_SetLampState(LA_CLOCK_15_SECONDS_2X, 1, 0, 100);
        PlaySound(10); // 15 second alarm
      }
      if (Playfield3xState[CurrentPlayer]==1) {
        Playfield3xState[CurrentPlayer] = 2;
        Playfield3XStop = CurrentTime + PLAYFIELD_X_TIMER;
        BSOS_SetLampState(LA_SAUCER_ARROW_3X, 0);
        BSOS_SetLampState(LA_CLOCK_15_SECONDS_3X, 1, 0, 100);
        PlaySound(10); // 15 second alarm
      }
      if (SaucerDirection == 0) {
        BSOS_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime);
        SaucerDirection = 1;
      } else {
        BSOS_PushToTimedSolenoidStack(SO_SAUCER_UP, 5, CurrentTime);
        SaucerDirection = 0;
      }
      SaucerLamps = 1;
      BallInSaucer = false;
      for (byte off=LA_BONUS_MINI_1K; off<=LA_BONUS_MINI_10K; off++) BSOS_SetLampState(off, 0);
      for (byte off=LA_BONUS_SUPER_1K; off<=LA_BONUS_5X; off++) BSOS_SetLampState(off, 0);
      for (byte off=LA_SAUCER_10K; off<=LA_SAUCER_XBALL; off++) BSOS_SetLampState(off, 0);
      BSOS_SetLampState(LA_SAUCER_30K, 0);
      ShowMiniBonusOnLadder(MiniBonus);
      ShowSuperBonusOnLadder(SuperBonus);
      ShowExtraBonusLights();
    }
  }


  // If the playfield hasn't been validated yet, flash score and player up num
  BSOS_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true, 2);
  // if (BallFirstSwitchHitTime==0) {
  //   // BSOS_SetDisplayFlash(CurrentPlayer, CurrentScores[CurrentPlayer], CurrentTime, 500, 2);
  //   BSOS_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true, 2);
  //   // if (!PlayerUpLightBlinking) {
  //   //   PlayerUpLightBlinking = true;
  //   // }
  // } else {
  //   // if (PlayerUpLightBlinking) {
  //   //   PlayerUpLightBlinking = false;
  //   // }
  // }

  // Check to see if ball is in the outhole
  if (BSOS_ReadSingleSwitchState(SW_OUTHOLE)) {
    if (BallTimeInTrough==0) {
      BallTimeInTrough = CurrentTime;
    } else {
      // Make sure the ball stays on the sensor for at least 
      // 0.5 seconds to be sure that it's not bouncing
      if ((CurrentTime-BallTimeInTrough)>500) {

        if (BallFirstSwitchHitTime==0) BallFirstSwitchHitTime = CurrentTime;
        BallDrained = true;
        #ifndef BALLY_STERN_OS_USE_GEETEOH 
        PlaySound(5); // sound off
        #endif
        // if we haven't used the ball save, and we're under the time limit, then save the ball
        if (  !BallSaveUsed && 
              ((CurrentTime-BallFirstSwitchHitTime)/1000)<((unsigned long)BallSaveNumSeconds) ) {
        
          BSOS_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
          if (BallFirstSwitchHitTime>0) {
            BallSaveUsed = true;
            BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 0);
//            BSOS_SetLampState(HEAD_SAME_PLAYER, 0);
          }
          BallTimeInTrough = CurrentTime;

          returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
        } else if (NumTiltWarnings>=MAX_TILT_WARNINGS) {
          returnState = MACHINE_STATE_BALL_OVER;
        } else if (NumTiltWarnings<MAX_TILT_WARNINGS) {
          returnState = MACHINE_STATE_COUNTDOWN_BONUS;
        }
      }
    }
  } else {
    BallTimeInTrough = 0;
  }

  return returnState;
}



// #################### INIT GAMEPLAY ####################
int InitGamePlay(boolean curStateChanged) {
  int returnState = MACHINE_STATE_INIT_GAMEPLAY;

  if (curStateChanged) {
    InitGameStartTime = CurrentTime;
    BSOS_SetCoinLockout((Credits >= dipMaxCredits) ? true:false);
    BSOS_SetDisableFlippers(true);
    BSOS_DisableSolenoidStack();
    BSOS_TurnOffAllLamps();
    if (Credits > 0) BSOS_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    BSOS_SetDisplayBallInPlay(1);
    InitGamePlayAnimation = CurrentTime+3240;
//    PlaySound(45); // Flash! (was originally 18, but this works for both S&T and Geeteoh)
    BSOS_EnableSolenoidStack();
    BSOS_PushToSolenoidStack(SO_SAUCER_DOWN, 5, true);

    // Set up general game variables
    CurrentNumPlayers = 1;
    CurrentPlayer = 0;
    CurrentBallInPlay = 1;
    PlayerShootsAgain = false;

    for (byte i = 0; i < 4; i++) {
         ScoreAwardStates[i] = 0;
//    WoodBeastXBallState[i] = 0;
//       SaucerXBallState[i] = 0;
        FreeGameCollected[i] = 0;
        FreeBallCollected[i] = 0;
              OutLanesLit[i] = 0;
        OutLanesCollected[i] = 0;
              WizardState[i] = 0;
            CurrentScores[i] = 0;
            BSOS_SetDisplay(i, 0);
       BSOS_SetDisplayBlank(i, 0);
    }
  }

  BackglassLampsLeft2Right();

  if (CurrentTime>=InitGamePlayAnimation) {
    BSOS_EnableSolenoidStack();
    BSOS_SetDisableFlippers(false);
    returnState = MACHINE_STATE_INIT_NEW_BALL;
    // if the ball is in the outhole, then we can move on
    // if (BSOS_ReadSingleSwitchState(SW_OUTHOLE)) {
    //   if (DEBUG_MESSAGES) {
    //     Serial.println(F("Ball is in trough - starting new ball"));
    //   }
    //   BSOS_EnableSolenoidStack();
    //   BSOS_SetDisableFlippers(false);
    //   returnState = MACHINE_STATE_INIT_NEW_BALL;
    // } else {

    //   if (DEBUG_MESSAGES) {
    //     Serial.println(F("Ball is not in trough - firing stuff and giving it a chance to come back"));
    //   }

    //   // And then set a base time for when we can continue
    //   InitGameStartTime = CurrentTime;
    // }

    // // Wait for TIME_TO_WAIT_FOR_BALL seconds, or until the ball appears
    // // The reason to bail out after TIME_TO_WAIT_FOR_BALL is just
    // // in case the ball is already in the shooter lane.
    // if ((CurrentTime-InitGameStartTime)>TIME_TO_WAIT_FOR_BALL || BSOS_ReadSingleSwitchState(SW_OUTHOLE)) {
    //   BSOS_EnableSolenoidStack();
    //   BSOS_SetDisableFlippers(false);
    //   returnState = MACHINE_STATE_INIT_NEW_BALL;
    // }
  } else {
    AttractRetro();
  }

  return returnState;
}



// #################### RUN GAMEPLAY MODE ####################
int RunGamePlayMode(int curState, boolean curStateChanged) {
  int returnState = curState;
  byte miniBonusAtTop = MiniBonus;
  byte superBonusAtTop = SuperBonus;
  unsigned long scoreAtTop = CurrentScores[CurrentPlayer];

  // Very first time into gameplay loop
  if (curState==MACHINE_STATE_INIT_GAMEPLAY) {
    returnState = InitGamePlay(curStateChanged);
  } else if (curState==MACHINE_STATE_INIT_NEW_BALL) {
    returnState = InitNewBall(curStateChanged, CurrentBallInPlay);
  } else if (curState==MACHINE_STATE_SKILL_SHOT) {
    returnState = SkillShot(curStateChanged);
  } else if (curState==MACHINE_STATE_NORMAL_GAMEPLAY) {
    returnState = NormalGamePlay(curStateChanged);
  } else if (curState==MACHINE_STATE_COUNTDOWN_BONUS) {
    returnState = CountdownBonus(curStateChanged);
  } else if (curState==MACHINE_STATE_WIZARD_MODE) {
    returnState = WizardMode(curStateChanged);
  } else if (curState==MACHINE_STATE_BALL_OVER) {    
    if (PlayerShootsAgain) {
      returnState = MACHINE_STATE_INIT_NEW_BALL;
    } else {
      CurrentPlayer+=1;
      if (CurrentPlayer>=CurrentNumPlayers) {
        CurrentPlayer = 0;
        CurrentBallInPlay+=1;
      }
        
      if (CurrentBallInPlay > dipBallsPerGame) {
        // CheckHighScores();
        for (byte count=0; count<CurrentNumPlayers; count++) {
          BSOS_SetDisplay(count, CurrentScores[count], true, 2);
        }

        returnState = MACHINE_STATE_MATCH_MODE;
      }
      else returnState = MACHINE_STATE_INIT_NEW_BALL;
    }
  } else if (curState==MACHINE_STATE_MATCH_MODE) {
      unsigned long highestScore = 0;
      if (curStateChanged) { // Take care of high score            
        // int highScorePlayerNum = 0;
        for (byte count=0; count<CurrentNumPlayers; count++) {
          if (CurrentScores[count]>highestScore) highestScore = CurrentScores[count];
          // highScorePlayerNum = count;
        }
      if (highestScore > HighScore){
        HighScore = highestScore;
        if (HighScoreAward > 0) {
          AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], HighScoreAward);
          for (byte i = 0; i < HighScoreAward; i++) {
            BSOS_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime + 250 * i, true);
            }
        }
        TotalReplays += HighScoreAward;
        BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
        HighScoreBeat = HighScoreBeat + 1;
        BSOS_WriteByteToEEProm(BSOS_TOTAL_HISCORE_BEATEN_START_BYTE, HighScoreBeat);
        BSOS_WriteULToEEProm(BSOS_HIGHSCORE_EEPROM_START_BYTE, HighScore);
        for (byte count=0; count<CurrentNumPlayers; count++) {
          // if (count==highScorePlayerNum) {
          //   BSOS_SetDisplay(count, CurrentScores[count], true, 2);
          // } else {
          //   BSOS_SetDisplayBlank(count, 0x00);
          // }
          BSOS_SetDisplay(count, CurrentScores[count], true, 2);
          }
        }
      }
    if (dipMatch) {
      returnState = ShowMatchSequence(curStateChanged); 
    } else {
      #ifdef BALLY_STERN_OS_USE_GEETEOH 
      if (PersonalGoalOn) { // Feature not ready yet.
        if (highestScore < PersonalGoal)
          PlaySound(46); // Oh dear, how pathetic
        else
          PlaySound(46); // You've saved your Earth. Habe a nice day!
      }
      else
        PlaySound(46); // No goal. Use regular end sound.
      #else
        PlaySound(5);  // sound off
        PlaySound(46);
      #endif
      // Wait 10 seconds to avoid final sound clobbering first attract sound
      AttractWait = 10000;
      returnState = MACHINE_STATE_ATTRACT;
    }
  }

  byte switchHit;

  if (NumTiltWarnings<MAX_TILT_WARNINGS) {
    while ( (switchHit=BSOS_PullFirstFromSwitchStack())!=SWITCH_STACK_EMPTY ) {

      switch (switchHit) {
        case SW_SELF_TEST_SWITCH:
          returnState = MACHINE_STATE_TEST_LIGHTS;
          SetLastSelfTestChangedTime(CurrentTime);
          break;
        case SW_STARS_PFIELD:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            PlaySound(13); // Ascending sound
            AddToScore(1000);
            AddToMiniBonus(1);
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_STARS_SHOOTER_LANE:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (curState==MACHINE_STATE_NORMAL_GAMEPLAY) {
              if (SuperBonusReady==true) {
                PlaySound(13); // Ascending sound
                AddToScore(1000);
              }
            } else if (curState==MACHINE_STATE_SKILL_SHOT) {
              if (SkillShotState==0) {
                SkillShotState = 1;
                BSOS_SetLampState(LA_STAR_SHOOTER_TOP, 0);
                BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
                BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
                if (PlayerShootsAgain==true) {
                  PlayerShootsAgain = false;
                  BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 0);
                  BSOS_SetLampState(LA_SHOOT_AGAIN, 0);
                }
              }
              PlaySound(13); // Ascending sound
              SkillShotHits++;
              if (SkillShotHits>=5) SkillShotHits = 5;
            }
          }
          break;
        case SW_DTARGET_1:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==true) {
              SuperBonusReady = true;
              AddToScore(10000);
              AddToMiniBonus(2);
              PlaySound(29); // S&T Explosion, Geeteoh Zap 2 Quick
              DropTargetHit();
            }
          } else  {
            PlaySound(24); // wizard sound bong bounce down
            if (WizardState[CurrentPlayer]==5) {
              // BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_SHOOTER_LANE_ROLL:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (curState==MACHINE_STATE_NORMAL_GAMEPLAY) {
              #ifdef BALLY_STERN_OS_USE_GEETEOH 
              PlaySound(7); // Shooting noise on Geeteoh
              #else
              PlaySound(18); // "Flash!" on S&T. Not used for Geeteoh, as it interferes with "Flash!" in background music.
              #endif
              if (SuperBonusReady!=true) {
                if (PFValidated==true) {
                  BSOS_PushToSolenoidStack(SO_DTARGET_1_DOWN, 5, true);
                  PFValidated = false;
                }
              } else {
                SuperBonusCollecting = true;
                CountdownSuper = SuperBonus;
                CountdownBonusX = BonusXState[CurrentPlayer];
                LastCountdownReportTime = CurrentTime;
              }
            } else if (curState==MACHINE_STATE_SKILL_SHOT) {
              if (SkillShotState==0) PlaySound(7); // Shooting noise. Was 12, ascending sound.
              if (SkillShotState==1) {
                #ifdef BALLY_STERN_OS_USE_GEETEOH
                PlaySound(254); // This way Flash, come on! 42 is only meant for 2X bonus, and has been changed.
                #else
                PlaySound(42); // wizard sound: Lucky shot earthling
                #endif
                SkillShotScoreAnimation = CurrentTime+500;
                SkillShotState = 2;
                if (SkillShotHits==1 || SkillShotHits==2) {
                  CurrentScores[CurrentPlayer] += 15000; // 10000
                  TotalSkill1++;
                  BSOS_WriteULToEEProm(BSOS_TOTAL_SKILL_1_EEPROM_BYTE, TotalSkill1);
                } else if (SkillShotHits==3 || SkillShotHits==4) {
                  CurrentScores[CurrentPlayer] += 30000; // 25000
                  TotalSkill2++;
                  BSOS_WriteULToEEProm(BSOS_TOTAL_SKILL_2_EEPROM_BYTE, TotalSkill2);
                } else if (SkillShotHits==5) {
                  CurrentScores[CurrentPlayer] += 75000; // 50000
                  TotalSkill3++;
                  BSOS_WriteULToEEProm(BSOS_TOTAL_SKILL_3_EEPROM_BYTE, TotalSkill3);
                }
              }
            }
          } else {
            PlaySound(26); // wizard sound, crash bounce up. Electric zap on Geeteoh.
          }
          break;
        case SW_DTARGET_REBOUND:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            PlaySound(7); // Quick sound. 
            AddToScore(50);
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
              // PFValidated = true;
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_CREDIT_BUTTON:
          if (DEBUG_MESSAGES) {
            Serial.println(F("Start game button pressed"));
          }
          if (CurrentBallInPlay<2) { // single player
            // If we haven't finished the first ball, we can add players
            AddPlayer();
          } else {
            // If the first ball is over, pressing start again resets the game
            returnState = MACHINE_STATE_INIT_GAMEPLAY;
          }
          break;
        case SW_TILT:
        case SW_SLAM:
          TiltHit();
          break;
        case SW_COIN_1:
          if (!FreePlay) {
            AddCredit(0, CreditsperCoin[0], CoinsperCredit[0]);
            BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
            GetValue = BSOS_ReadByteFromEEProm(BSOS_CHUTE_1_COINS_START_BYTE);
            BSOS_WriteByteToEEProm(BSOS_CHUTE_1_COINS_START_BYTE, (GetValue + 1) & 0x000000FF);
          }
          break;
        case SW_COIN_2:
          if (!FreePlay) {
            AddCredit(1, CreditsperCoin[1], CoinsperCredit[1]);
            BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
            GetValue = BSOS_ReadByteFromEEProm(BSOS_CHUTE_2_COINS_START_BYTE);
            BSOS_WriteByteToEEProm(BSOS_CHUTE_2_COINS_START_BYTE, (GetValue + 1) & 0x000000FF);
          }
          break;
        case SW_COIN_3:
          if (!FreePlay) {
            AddCredit(2, CreditsperCoin[2], CoinsperCredit[2]);
            BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
            GetValue = BSOS_ReadByteFromEEProm(BSOS_CHUTE_3_COINS_START_BYTE);
            BSOS_WriteByteToEEProm(BSOS_CHUTE_3_COINS_START_BYTE, (GetValue + 1) & 0x000000FF);
          }
          break;
        case SW_TARGET_LRIGHT_BOTTOM:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][3]==0 || DTarget4Light[CurrentPlayer][3]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              TargetLowerBottomBlip = CurrentTime + 100;
              BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][3]==2) {
              DTarget4Light[CurrentPlayer][3] = 3;
              if (!Bonus3XComplete()) PlaySound(45); // Flash!
              if (AmberHurryUpStop != 0xFFFFFFFF) AddToScore(50000); 
              AmberHurryUpStop = 0xFFFFFFFF;
              BSOS_SetLampState(LA_DTARGET_4_D, 1);
              BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1);
              CheckHurryUpCompletion();
            } 
            else if (DTarget4Light[CurrentPlayer][3]==3) {
              PlaySound(27); // Look out, Flash!
              AddToScore(10000);
            }
            AddToSuperBonus(1);
          } 
          else  {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_INLANE_R:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][2]==0 || DTarget4Light[CurrentPlayer][2]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              InlaneRightBlip = CurrentTime + 100;
              BSOS_SetLampState(LA_INLANE_RIGHT, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][2]==2) {
              DTarget4Light[CurrentPlayer][2] = 3;
              if (!Bonus3XComplete()) PlaySound(27); // Look out, Flash!
              AddToScore(25000);
              BSOS_SetLampState(LA_DTARGET_4_C, 1);
              BSOS_SetLampState(LA_INLANE_RIGHT, 1);
              LeftSpinnerHurryUpLit = true;
              SpinnerMissed = true;
              LeftSpinnerHurryUpTimer = CurrentTime + 5000;
              BSOS_SetLampState(LA_SPINNER_LEFT, 1, 0, 100);
              CheckHurryUpCompletion();
            } 
            else if (DTarget4Light[CurrentPlayer][2]==3) {
              PlaySound(27); // Look out, Flash!
              AddToScore(10000);
            }
          } 
          else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_INLANE_L:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][1]==0 || DTarget4Light[CurrentPlayer][1]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              InlaneLeftBlip = CurrentTime + 100;
              BSOS_SetLampState(LA_INLANE_LEFT, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][1]==2) {
              DTarget4Light[CurrentPlayer][1] = 3;
              if (!Bonus3XComplete()) PlaySound(27); // Look out, Flash!
              AddToScore(25000);
              BSOS_SetLampState(LA_DTARGET_4_B, 1);
              BSOS_SetLampState(LA_INLANE_LEFT, 1);
              RightSpinnerHurryUpLit = true;
              SpinnerMissed = true;
              RightSpinnerHurryUpTimer = CurrentTime + 5000;
              BSOS_SetLampState(LA_SPINNER_RIGHT, 1, 0, 100);
              CheckHurryUpCompletion();
            } 
            else if (DTarget4Light[CurrentPlayer][1]==3) {
              PlaySound(27); // Look out, Flash!
              AddToScore(10000);
            }
          } 
          else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_TARGET_LRIGHT_TOP:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][0]==0 || DTarget4Light[CurrentPlayer][0]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              TargetLowerTopBlip = CurrentTime + 100;
              BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][0]==2) {
              DTarget4Light[CurrentPlayer][0] = 3;
              if (!Bonus3XComplete()) PlaySound(45); // Flash!
              if (WhiteHurryUpStop != 0xFFFFFFFF) AddToScore(50000);
              WhiteHurryUpStop = 0xFFFFFFFF;
              BSOS_SetLampState(LA_DTARGET_4_A, 1);
              BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 1);
              CheckHurryUpCompletion();
            } 
            else if (DTarget4Light[CurrentPlayer][0]==3) {
              PlaySound(27); // Look out, Flash!
              AddToScore(10000);
            }
            AddToSuperBonus(1);
          } 
          else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
          
        case SW_DTARGET_4_A:
        case SW_DTARGET_4_B:
        case SW_DTARGET_4_C:
        case SW_DTARGET_4_D:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToSuperBonus(1);
            DropTargetHit();
            if (switchHit==SW_DTARGET_4_A) {
              if (DTarget4Light[CurrentPlayer][0]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
              } else if (DTarget4Light[CurrentPlayer][0]==1) {
                WhiteHurryUpStop = CurrentTime + DTARGET_4_HURRY_UP_TIMER;
                BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 1, 0, 100);
                DTarget4Light[CurrentPlayer][0] = 2;
                PlaySound(17); // Switches over there, start hitting them! Was 47, 15 second warning.
                AddToScore(10000);
              } else if (DTarget4Light[CurrentPlayer][0]==2 || DTarget4Light[CurrentPlayer][0]==3) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
              }
            }
            if (switchHit==SW_DTARGET_4_B) {
              if (DTarget4Light[CurrentPlayer][1]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
              } else if (DTarget4Light[CurrentPlayer][1]==1) {
                BSOS_SetLampState(LA_INLANE_LEFT, 1, 0, 250);
                DTarget4Light[CurrentPlayer][1] = 2;
                PlaySound(17); // Switches over there, start hitting them! Was 47, 15 second warning.
                AddToScore(10000);
              } else if (DTarget4Light[CurrentPlayer][1]==2 || DTarget4Light[CurrentPlayer][1]==3) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
              }
            }
            if (switchHit==SW_DTARGET_4_C) {
              if (DTarget4Light[CurrentPlayer][2]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
              } else if (DTarget4Light[CurrentPlayer][2]==1) {
                BSOS_SetLampState(LA_INLANE_RIGHT, 1, 0, 250);
                DTarget4Light[CurrentPlayer][2] = 2;
                PlaySound(17); // Switches over there, start hitting them! Was 47, 15 second warning.
                AddToScore(10000);
              } else if (DTarget4Light[CurrentPlayer][2]==2 || DTarget4Light[CurrentPlayer][2]==3) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
              }
            }
            if (switchHit==SW_DTARGET_4_D) {
              if (DTarget4Light[CurrentPlayer][3]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
              } else if (DTarget4Light[CurrentPlayer][3]==1) {
                AmberHurryUpStop = CurrentTime + DTARGET_4_HURRY_UP_TIMER;
                BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1, 0, 100);
                DTarget4Light[CurrentPlayer][3] = 2;
                PlaySound(17); // Switches over there, start hitting them! Was 47, 15 second warning.
                AddToScore(10000);
              } else if (DTarget4Light[CurrentPlayer][3]==2 || DTarget4Light[CurrentPlayer][3]==3) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
              }
            }
            if (CheckIfDTargets4Down()) {
              AddToScore(15000);
              if (LeftSpinnerLit==false) {
                LeftSpinnerLit = true;
                BSOS_SetLampState(LA_SPINNER_LEFT, 1);
              } else {
                RightSpinnerLit = true;
                BSOS_SetLampState(LA_SPINNER_RIGHT, 1);
              }
              DTarget4Goal[CurrentPlayer] = true;
              BSOS_SetLampState(LA_SAUCER_10K, 1);
              CheckSaucerDTargetGoal();
              if (BonusXState[CurrentPlayer]==4) {
                BonusXState[CurrentPlayer] = 5;
                ShowExtraBonusLights();
              }
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime + 500);
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            // BSOS_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime);
          }
          break;

        case SW_DTARGET_3_A:
        case SW_DTARGET_3_B:
        case SW_DTARGET_3_C:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            AddToMiniBonus(1);
            DropTargetHit();
            if (switchHit==SW_DTARGET_3_A) {
              if (DTarget3Lights[CurrentPlayer][0]==0 && DTarget3Flashing[CurrentPlayer]==LA_DTARGET_ARROW_1) {
                BSOS_SetLampState(LA_DTARGET_ARROW_1, 1);
                DTarget3Lights[CurrentPlayer][0] = 1;
                if (!Bonus2XComplete()) PlaySound(24); // Bong Bounce Down
                AddToScore(10000);
              } else if (DTarget3Lights[CurrentPlayer][0]==1) {
                  PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
              } else {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(5000);
              }
            }
            if (switchHit==SW_DTARGET_3_B) {
              if (DTarget3Lights[CurrentPlayer][1]==0 && DTarget3Flashing[CurrentPlayer]==LA_DTARGET_ARROW_2) {
                BSOS_SetLampState(LA_DTARGET_ARROW_2, 1);
                DTarget3Lights[CurrentPlayer][1] = 1;
                if (!Bonus2XComplete()) PlaySound(24); // Bong Bounce Down
                AddToScore(10000);
              } else if (DTarget3Lights[CurrentPlayer][1]==1) {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
              } else {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(5000);
              }
            }
            if (switchHit==SW_DTARGET_3_C) {
              if (DTarget3Lights[CurrentPlayer][2]==0 && DTarget3Flashing[CurrentPlayer]==LA_DTARGET_ARROW_3) {
                BSOS_SetLampState(LA_DTARGET_ARROW_3, 1);
                DTarget3Lights[CurrentPlayer][2] = 1;
                if (!Bonus2XComplete()) PlaySound(24); // Bong Bounce Down
                AddToScore(10000);
              } else if (DTarget3Lights[CurrentPlayer][2]==1) {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
              } else {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(5000);
              }
            }
            if (CheckIfDTargets3Down()) {
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_3_RESET, 15, CurrentTime + 500);
              DTarget3Completions++;
              if (DTarget3Completions==1) {
                BSOS_SetLampState(LA_POP_TOP, 1);
                TopPopLit = true;
              } else if (DTarget3Completions==2) {
                BSOS_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 1);
                MiniBonusReady[CurrentPlayer] = true;
              } else if (DTarget3Completions==3) {
                BSOS_SetLampState(LA_TARGET_UPPER_SPECIAL, 1);
                TopSpecialLit[CurrentPlayer] = true;
              }
              AddToScore(15000);
              BSOS_SetLampState(LA_SAUCER_30K, 1);
              DTarget3Goal[CurrentPlayer] = true;
              CheckSaucerDTargetGoal();
              if (BonusXState[CurrentPlayer]==3) {
                BonusXState[CurrentPlayer] = 4;
                ShowExtraBonusLights();
              }
            }
            if (Bonus2XComplete()) {
              Mini50kCollected = true;
              Playfield2xState[CurrentPlayer] = 1;
              CurrentSoundEffectTimer = 0;
              PlaySound(42); // S&T: Lucky shot Earthling. Geeteoh: Very roughly, 14 seconds!
              BSOS_SetLampState(LA_SAUCER_ARROW_2X, 1, 0, 150);
              BSOS_SetLampState(LA_BONUS_MINI_50K, 1);
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
              // PFValidated = true;
            }
          } else  {
            PlaySound(24); // wizard sound: bong bounce down
            // BSOS_PushToTimedSolenoidStack(SO_DTARGET_3_RESET, 15, CurrentTime);
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_TARGET_TOP:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            AddToScore(5000);
            AddToMiniBonus(1);
            if (MiniBonusReady[CurrentPlayer]==false && TopSpecialLit[CurrentPlayer]==false) {
              TargetUpperBlip = CurrentTime + 100;
              BSOS_SetLampState(LA_TARGET_UPPER_SPECIAL, 1);
              BSOS_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 1);
            }
            if (MiniBonusReady[CurrentPlayer]==true) {
              BSOS_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 0);
              MiniBonusCollecting = true;
              CountdownMini = MiniBonus;
              CountdownBonusX = BonusXState[CurrentPlayer];
              LastCountdownReportTime = CurrentTime;
            }
            if (TopSpecialLit[CurrentPlayer]==true) {
              if (PlayfieldAward == 1 || (PlayfieldAward == 2 && PlayerShootsAgain) || (PlayfieldAward == 3 && FreeGameCollected[CurrentPlayer]))
                AddToScore(50000);
              else if (PlayfieldAward == 2) {
                PlayerShootsAgain = true;
                BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
                BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
              }
              else if (PlayfieldAward == 3) {
              AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
              if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
              TotalReplays += 1;
              BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
              BSOS_PushToSolenoidStack(SO_KNOCKER, 5, true);
              }
              BSOS_SetLampState(LA_TARGET_UPPER_SPECIAL, 0);
              TopSpecialLit[CurrentPlayer] = false;
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
              // PFValidated = true;
            } else {
              PlaySound(27); // Look out, Flash!
            }
          } else  {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_DTARGET_INLINE_1ST:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(10000);
            AddToSuperBonus(2);
            DropTargetHit();
            PlaySound(25); // Bong Bounce Up
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            // BSOS_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime);
          }
          break;
        case SW_DTARGET_INLINE_2ND:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(10000);
            DropTargetHit();
            PlaySound(25); // Bong Bounce Up
            if (BonusXState[CurrentPlayer]==1) {
              BonusXState[CurrentPlayer] = 2;
              ShowExtraBonusLights();
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_DTARGET_INLINE_3RD:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(10000);
            DTargetInlineGoal[CurrentPlayer] = true;
            // RightSpinnerLit = true;
            // BSOS_SetLampState(LA_SPINNER_RIGHT, 1);
            BSOS_SetLampState(LA_SAUCER_20K, 1);
            CheckSaucerDTargetGoal();
            PlaySound(25); // Bong Bounce Up
//            if (WoodBeastXBallState[CurrentPlayer]==0) {
//              WoodBeastXBallState[CurrentPlayer] = 1;
              BSOS_SetLampState(LA_TARGET_WOOD_BEAST_XBALL, 1);
//            }
            DropTargetHit();
            if (BonusXState[CurrentPlayer]==2) {
              BonusXState[CurrentPlayer] = 3;
              ShowExtraBonusLights();
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_TARGET_WOOD_BEAST:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            BSOS_SetLampState(LA_TARGET_WOOD_BEAST_XBALL, 0);
            if (PlayfieldAward == 1 || (PlayfieldAward >= 2 && (PlayerShootsAgain || FreeBallCollected[CurrentPlayer])))
              AddToScore(0); // No extra points awarded for no extra ball
            else if (PlayfieldAward == 2 ||PlayfieldAward == 3) {
              PlayerShootsAgain = true;
              if (!dipUnlimitedExtraBalls) FreeBallCollected[CurrentPlayer] = true;
              BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
              BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
            }
            BSOS_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime + 750);
            PlaySound(27); // Look out, Flash!
            AddToScore(25000);
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_REBOUND:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            PlaySound(7); // Rebound
            AddToScore(10);
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
              // PFValidated = true;
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_SAUCER:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            if (SaucerHitTime==0 || (CurrentTime-SaucerHitTime)>500) {
              SaucerHitTime = CurrentTime;
              if (Playfield2xState[CurrentPlayer]==1 || Playfield3xState[CurrentPlayer]==1) {
                SaucerAnimation = CurrentTime+3770;
                PlaySound(40); // Open fire, all weapons!
              } else {
                SaucerAnimation = CurrentTime+1650;
                PlaySound(22); // Escape is impossible!
              }
              if (dipSaucerStart10k) AddToScore(10000);
              if (DTarget4Goal[CurrentPlayer]) AddToScore(10000);
              if (DTargetInlineGoal[CurrentPlayer]) AddToScore(20000);
              if (DTarget3Goal[CurrentPlayer]) AddToScore(30000);
                  BSOS_SetLampState(LA_SAUCER_XBALL, 0);
                if (DTarget4Goal[CurrentPlayer] && DTarget3Goal[CurrentPlayer] && DTargetInlineGoal[CurrentPlayer]) {
                  if (PlayfieldAward == 1 || (PlayfieldAward == 2 || PlayfieldAward == 3) && PlayerShootsAgain)
                    AddToScore(25000);
                  else if (PlayfieldAward == 2 || PlayfieldAward == 3) {
                    PlayerShootsAgain = true;
                    BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
                    BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
                  }
                }
              DTarget4Goal[CurrentPlayer] = false;
              DTargetInlineGoal[CurrentPlayer] = false;
              DTarget3Goal[CurrentPlayer] = false;

              BSOS_SetLampState(LA_SAUCER_10K, 0);
              BSOS_SetLampState(LA_SAUCER_20K, 0);
              BSOS_SetLampState(LA_SAUCER_30K, 0);
              BallInSaucer = true;
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              // PFValidated = true;
            }
          } else {
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
            if (SaucerHitTime==0 || (CurrentTime-SaucerHitTime)>500) {
              SaucerHitTime = CurrentTime;
              if (MingAttackReady==false) {
                AddToScore(3500);
                MingAttackProgress += 35;
                if (MingAttackProgress>=WIZARD_GOAL_ATTACK && MingAttackReady==false) {
                  MingAttackReady = true;
                } else {
                  PlaySound(25); // wizard sound: bong bounce up
                }
              }
              if (MingAttackReady==true) {
                if (MingHealth != 0) MingHealth--;
                if (MingHealth==0) {
                  #ifndef BALLY_STERN_OS_USE_GEETEOH 
                  PlaySound(5); // Stop sounds
                  PlaySound(48); // wizard sound: defeat miserable earthling
                  #else
                  PlaySound(46); // You've saved your Earth. Have a nice day!
                  #endif
                  BSOS_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime+3100, true);
                  BSOS_PushToTimedSolenoidStack(SO_KNOCKER, 5, CurrentTime+3100);
                  MingAttackAnimation = CurrentTime+2475;
                } else {
                  #ifndef BALLY_STERN_OS_USE_GEETEOH
                  PlaySound(5); // Stop sounds
                  PlaySound(42); // wizard sound: Lucky shot earthling
                  #else
                  PlaySound(45); // Flash!
                  #endif
                  MingAttackAnimation = CurrentTime+1650;
                }
              }
              else
                if (MingHealth == 0) BSOS_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime+1000, true); // added in case ball pops back into saucer after Ming Defeat
              BallInSaucer = true;
            }
          }
          break;
        case SW_OUTLANE_RIGHT:
          if (OutLanesLit[CurrentPlayer]) {
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              if (PlayfieldAward == 1 || (PlayfieldAward == 2 && PlayerShootsAgain) || (PlayfieldAward == 3 && FreeGameCollected[CurrentPlayer]))
                  AddToScore(50000);
                else if (PlayfieldAward == 2) {
                  PlayerShootsAgain = true;
                  BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
                  BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
                }
                else if (PlayfieldAward == 3) {
                  AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
                  if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
                  TotalReplays += 1;
                  BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
                  BSOS_PushToSolenoidStack(SO_KNOCKER, 5, true);
                  }
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(2000);
            AddToSuperBonus(2);
            if (WizardState[CurrentPlayer]==0) PlaySound(28); // What a damn nuisance!
            if (WizardState[CurrentPlayer]==1) {
              WizardState[CurrentPlayer] = 2;

              #ifndef BALLY_STERN_OS_USE_GEETEOH
                PlaySound(5); // Stop sounds
              #endif
              PlaySound(50); // wizard sound: Ming laugh!
            }
          } else {
            if (WizardState[CurrentPlayer]==5) PlaySound(50); // wizard sound: Ming laugh!
          }
          BallDrained = true;
          break;
        case SW_OUTLANE_LEFT:
          if (OutLanesLit[CurrentPlayer]) {
            BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
            BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
            if (PlayfieldAward == 1 || (PlayfieldAward == 2 && PlayerShootsAgain) || (PlayfieldAward == 3 && FreeGameCollected[CurrentPlayer]))
                  AddToScore(50000);
                else if (PlayfieldAward == 2) {
                  PlayerShootsAgain = true;
                  BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
                  BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
                }
                else if (PlayfieldAward == 3) {
                  AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
                  if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
                  TotalReplays += 1;
                  BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
                  BSOS_PushToSolenoidStack(SO_KNOCKER, 5, true);
                  }
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(2000);
            AddToSuperBonus(2);
            if (WizardState[CurrentPlayer]==0) PlaySound(28); // What a damn nuisance!
            if (WizardState[CurrentPlayer]==1) {
              WizardState[CurrentPlayer] = 2;
              #ifndef BALLY_STERN_OS_USE_GEETEOH
                PlaySound(5); // Stop sounds
              #endif
              PlaySound(50); // wizard sound: Ming laugh!
            }
          } else {
            if (WizardState[CurrentPlayer]==5) PlaySound(50); // wizard sound: Ming laugh!
          }
          BallDrained = true;
          break;
        case SW_SPINNER_RIGHT:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            if (RightSpinnerLit==false) {
              PlaySound(8); // Spinner noise
              AddToScore(100);
            } else {
              PlaySound(9); // Low hum
              AddToScore(1000);
            }
            if (RightSpinnerHurryUpLit==true) {
              SpinnerMissed = false;
              RightSpinnerHurryUpTimer = CurrentTime + 500;
              AddToScore(2000);
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              // PFValidated = true;
              // PlaySound(13);
            }
          } else {
            AddToScore(1000);
            MingAttackProgress++;
            if (MingAttackProgress>=WIZARD_GOAL_ATTACK && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(7); // wizard sound: Rebound hit
            }
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_SPINNER_LEFT:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            if (LeftSpinnerLit==false) {
              PlaySound(8); // Spinner noise
              AddToScore(100);
            } else {
              PlaySound(9); // Low hum
              AddToScore(1000);
            }
            if (LeftSpinnerHurryUpLit==true) {
              LeftSpinnerHurryUpTimer = CurrentTime + 500;
              SpinnerMissed = false;
              AddToScore(2000);
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              // PlaySound(13);
              // PFValidated = true;
            }
          } else {
            AddToScore(1000);
            MingAttackProgress++;
            if (MingAttackProgress>=WIZARD_GOAL_ATTACK && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(7); // wizard sound: Rebound hit
            }
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_SLING_RIGHT:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(50);
            PlaySound(21); // Ray gun
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_SLING_LEFT:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(50);
            PlaySound(21); // Ray gun
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;
        case SW_POP_TOP:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (PFValidated==false) {
              BSOS_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
            if (TopPopLit==false) {
              PlaySound(20); // Ming's Ring
              AddToScore(1000);
            } else {
              PlaySound(20); // Ming's ring. Was 13.
              AddToScore(5000);
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(20); // Ming's ring. Was 13.
              // PFValidated = true;
            }
          } else {
            AddToScore(2000);
            MingAttackProgress += 20;
            if (MingAttackProgress>=WIZARD_GOAL_ATTACK && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(25); // wizard sound: bong bounce up. Was 27, which was repurposed.
            }
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_POP_RIGHT:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            PlaySound(20); // Ming's ring
            AddToScore(1000);
          } else  {
            AddToScore(2000);
            MingAttackProgress += 20;
            if (MingAttackProgress>=WIZARD_GOAL_ATTACK && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(25); // wizard sound: bong bounce up. Was 27, which was repurposed.
            }
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
        case SW_POP_LEFT:
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            PlaySound(20); // Ming's ring
            AddToScore(1000);
          } else  {
            AddToScore(2000);
            MingAttackProgress += 20;
            if (MingAttackProgress>=WIZARD_GOAL_ATTACK && MingAttackReady==false) {
              MingAttackReady = true;
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(25); // wizard sound: bong bounce up. Was 27, which was repurposed.
            }
            if (WizardState[CurrentPlayer]==4) {
              BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
      }
      // if (DEBUG_MESSAGES) {
      //   char buf[128];
      //   sprintf(buf, "Switch = 0x%02X\n", switchHit);
      //   Serial.write(buf);
      // }
    }
  } else {
    // We're tilted, so just wait for outhole. Pull all switches, no points awarded. React to bare minimum of switches!
    while ( (switchHit = BSOS_PullFirstFromSwitchStack()) != SWITCH_STACK_EMPTY && switchHit != SW_OUTHOLE) { // xxx
      switch (switchHit) {
        case SW_SELF_TEST_SWITCH:
          returnState = MACHINE_STATE_TEST_LIGHTS;
          SetLastSelfTestChangedTime(CurrentTime);
          break; 
        case SW_COIN_1:
          if (!FreePlay) {
            AddCredit(0, CreditsperCoin[0], CoinsperCredit[0]);
            BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
          }
          break;
        case SW_COIN_2:
          if (!FreePlay) {
            AddCredit(1, CreditsperCoin[1], CoinsperCredit[1]);
            BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
          }
          break;
        case SW_COIN_3:
          if (!FreePlay) {
            AddCredit(2, CreditsperCoin[2], CoinsperCredit[2]);
            BSOS_SetDisplayCredits(Credits, dipCreditsDisplayed);
          }
          break;
        case SW_SAUCER:
          BSOS_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime + 500, true);
         break;
      }
    }
  }
  if (switchHit == SW_OUTHOLE && NumTiltWarnings >= MAX_TILT_WARNINGS) 
    returnState = MACHINE_STATE_BALL_OVER; // xxx Should take care of tilt during skill shot!

  if (miniBonusAtTop!=MiniBonus) {
    ShowMiniBonusOnLadder(MiniBonus);
  }

  if (superBonusAtTop!=SuperBonus) {
    ShowSuperBonusOnLadder(SuperBonus);
  }

  if (scoreAtTop!=CurrentScores[CurrentPlayer]) {
    // if (DEBUG_MESSAGES) {
    //   Serial.println(F("Score changed"));
    // }
  }


  // handle score award thresholds
  if (CurrentScores[CurrentPlayer]>=ScoreAward1 && ScoreAward1 > 0 && ScoreAwardStates[CurrentPlayer]==0) {
    ScoreAwardStates[CurrentPlayer] = 1;
    if (ThresholdAward == 1 || (ThresholdAward == 2 && PlayerShootsAgain) || (ThresholdAward == 3 && FreeGameCollected[CurrentPlayer]))
      AddToScore(25000);
    else if (ThresholdAward == 2) { // Extra Ball
      PlayerShootsAgain = true;
      BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
      BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
    }
    else if (ThresholdAward == 3) { // One Credit
    BSOS_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
    AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
    if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
    TotalReplays += 1;
    BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
    }
    if (DEBUG_MESSAGES) Serial.println(F("Score award 1"));
  }
  if (CurrentScores[CurrentPlayer]>=ScoreAward2 && ScoreAward2 > 0 && ScoreAwardStates[CurrentPlayer]==1) {
    ScoreAwardStates[CurrentPlayer] = 2;
    if (ThresholdAward == 1 || (ThresholdAward == 2 && PlayerShootsAgain) || (ThresholdAward == 3 && FreeGameCollected[CurrentPlayer]))
      AddToScore(25000);
    else if (ThresholdAward == 2) { // Extra Ball
      PlayerShootsAgain = true;
      BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
      BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
    }
    else if (ThresholdAward == 3) { // One Credit
    BSOS_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
    AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
    if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
    TotalReplays += 1;
    BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
    }
    if (DEBUG_MESSAGES) Serial.println(F("Score award 2"));
  }
  if (CurrentScores[CurrentPlayer]>=ScoreAward3 && ScoreAward3 > 0 && ScoreAwardStates[CurrentPlayer]==2) {
    ScoreAwardStates[CurrentPlayer] = 3;
    if (ThresholdAward == 1 || (ThresholdAward == 2 && PlayerShootsAgain) || (ThresholdAward == 3 && FreeGameCollected[CurrentPlayer]))
      AddToScore(25000);
    else if (ThresholdAward == 2) { // Extra Ball
      PlayerShootsAgain = true;
      BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
      BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
    }
    else if (ThresholdAward == 3) { // One Credit
    BSOS_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
    AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
    if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
    TotalReplays += 1;
    BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
    }
    if (DEBUG_MESSAGES) Serial.println(F("Score award 3"));
  }

  return returnState;
}



// #################### LOOP ####################
void loop() {
  // This line has to be in the main loop
  BSOS_DataRead(0);

  CurrentTime = millis();
  int newMachineState = MachineState;

  // Machine state is self-test/attract/game play
  if (MachineState<0) {
    newMachineState = RunSelfTest(MachineState, MachineStateChanged);
  } else if (MachineState==MACHINE_STATE_ATTRACT) {
    newMachineState = RunAttractMode(MachineState, MachineStateChanged);
  } else {
    newMachineState = RunGamePlayMode(MachineState, MachineStateChanged);
  }
  // if (MachineState==MACHINE_STATE_ATTRACT) {
  //   newMachineState = RunAttractMode(MachineState, MachineStateChanged);
  // } else {
  //   newMachineState = RunGamePlayMode(MachineState, MachineStateChanged);
  // }

  if (newMachineState!=MachineState) {
    MachineState = newMachineState;
    MachineStateChanged = true;
  } else {
    MachineStateChanged = false;
  }

  BSOS_ApplyFlashToLamps(CurrentTime);
  BSOS_UpdateTimedSolenoidStack(CurrentTime);
}



// #################### LAMP ANIMATIONS ####################
// ########## BACKGLASS LAMPS CLEAR ##########
void BackglassLampsClear() {
  BSOS_SetLampState(LA_FLASH_GORDON_6, 0);
  BSOS_SetLampState(LA_FLASH_GORDON_1, 0);
  BSOS_SetLampState(LA_FLASH_GORDON_5, 0);
  BSOS_SetLampState(LA_FLASH_GORDON_2, 0);
  BSOS_SetLampState(LA_FLASH_GORDON_4, 0);
  BSOS_SetLampState(LA_FLASH_GORDON_3, 0);
  BSOS_SetLampState(LA_FLASH_STROBE, 0);
}


// ########## BACKGLASS LAMPS CENTER OUT ##########
void backglassLampsCenterOut() {
  if (USE_STROBE) {
    byte lampPhase = (CurrentTime/115)%4;
    BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhase==3||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhase==3||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhase==2||lampPhase==3, lampPhase==3);
    BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhase==2||lampPhase==3, lampPhase==3);
    BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhase==1||lampPhase==2, lampPhase==2);
    BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhase==1||lampPhase==2, lampPhase==2);
    if (MachineState!=MACHINE_STATE_NORMAL_GAMEPLAY || (MachineState==MACHINE_STATE_NORMAL_GAMEPLAY && BallInSaucer==true)) BSOS_SetLampState(LA_FLASH_STROBE, lampPhase<2, lampPhase==1);
  } else {
    byte lampPhase = (CurrentTime/115)%3;
    BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhase==2||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhase==2||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhase==1||lampPhase==2, lampPhase==2);
    BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==2, lampPhase==2);
    BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhase<2, lampPhase==1);
    BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhase<2, lampPhase==1);
  }
}


// ########## BACKGLASS LAMPS KNIGHT RIDER ##########
void BackglassLampsKnightRider() {
  if (USE_STROBE) {
    byte lampPhase = (CurrentTime/80)%14;
    BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhase==6||lampPhase==7, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhase==5||lampPhase==8, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhase==4||lampPhase==9, 0, 0);
    if (MachineState!=MACHINE_STATE_NORMAL_GAMEPLAY) BSOS_SetLampState(LA_FLASH_STROBE, lampPhase==3||lampPhase==10, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhase==2||lampPhase==11, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==12, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhase==0||lampPhase==13, 0, 0);
  } else {
    byte lampPhase = (CurrentTime/80)%12;
    BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhase==5||lampPhase==6, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhase==4||lampPhase==7, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhase==3||lampPhase==8, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhase==2||lampPhase==9, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==10, 0, 0);
    BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhase==0||lampPhase==11, 0, 0);
  }
}


// ########## BACKGLASS LAMPS LEFT TO RIGHT ##########
void BackglassLampsLeft2Right() {
  if (USE_STROBE) {
    byte lampPhase = (CurrentTime/85)%4;
    BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhase==3||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhase==3||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhase==2||lampPhase==3, lampPhase==3);
    BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhase==2||lampPhase==3, lampPhase==3);
    BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhase==1||lampPhase==2, lampPhase==2);
    BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhase==1||lampPhase==2, lampPhase==2);
    if (MachineState!=MACHINE_STATE_NORMAL_GAMEPLAY) BSOS_SetLampState(LA_FLASH_STROBE, lampPhase<2, lampPhase==1);
  } else {
    byte lampPhase = (CurrentTime/95)%3;
    BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhase==2||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhase==2||lampPhase==0, lampPhase==0);
    BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhase==1||lampPhase==2, lampPhase==2);
    BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==2, lampPhase==2);
    BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhase<2, lampPhase==1);
    BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhase<2, lampPhase==1);
  }
}


// ########## ATTRACT RETRO ##########
void AttractRetro() {
  // bonus
  byte attractBonus;
  for (attractBonus=0; attractBonus<10; attractBonus++){
    if ( attractBonus % 2 == 0) { // even but odd on pf
      BSOS_SetLampState(LA_BONUS_MINI_1K+attractBonus, 1, 0, 100);
      BSOS_SetLampState(LA_BONUS_SUPER_1K+attractBonus, 1, 0, 100);
    } else { // odd even on pf
      BSOS_SetLampState(LA_BONUS_MINI_1K+attractBonus, 1, 0, 200);
      BSOS_SetLampState(LA_BONUS_SUPER_1K+attractBonus, 1, 0, 200);
    }
  }

  // sweep 4
  byte lampPhase1 = (CurrentTime/115)%4; // 125
  BSOS_SetLampState(LA_SAUCER_XBALL, lampPhase1==3||lampPhase1==0, lampPhase1==0); // SAUCER
  BSOS_SetLampState(LA_SAUCER_30K, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  BSOS_SetLampState(LA_SAUCER_20K, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  BSOS_SetLampState(LA_SAUCER_10K, lampPhase1<2, lampPhase1==1);
  BSOS_SetLampState(LA_DTARGET_4_A, lampPhase1==3||lampPhase1==0, lampPhase1==0); // DTARGET 4
  BSOS_SetLampState(LA_DTARGET_4_B, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  BSOS_SetLampState(LA_DTARGET_4_C, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  BSOS_SetLampState(LA_DTARGET_4_D, lampPhase1<2, lampPhase1==1);
  BSOS_SetLampState(LA_BONUS_5X, lampPhase1==3||lampPhase1==0, lampPhase1==0); // BONUS X
  BSOS_SetLampState(LA_BONUS_4X, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  BSOS_SetLampState(LA_BONUS_3X, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  BSOS_SetLampState(LA_BONUS_2X, lampPhase1<2, lampPhase1==1);
  BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, lampPhase1==3||lampPhase1==0, lampPhase1==0); // OUTLANES & PFX
  BSOS_SetLampState(LA_SAUCER_ARROW_3X, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  BSOS_SetLampState(LA_CLOCK_15_SECONDS_2X, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  BSOS_SetLampState(LA_SAUCER_ARROW_2X, lampPhase1<2, lampPhase1==1);
  BSOS_SetLampState(LA_CLOCK_15_SECONDS_3X, lampPhase1<2, lampPhase1==1);
  BSOS_SetLampState(LA_INLANE_LEFT, lampPhase1==3||lampPhase1==0, lampPhase1==0); // INLANES & SPINNERS
  BSOS_SetLampState(LA_SPINNER_RIGHT, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  BSOS_SetLampState(LA_INLANE_RIGHT, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  BSOS_SetLampState(LA_SPINNER_LEFT, lampPhase1<2, lampPhase1==1);
  BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, lampPhase1==3||lampPhase1==0, lampPhase1==0); // REMAINDER
  BSOS_SetLampState(LA_TARGET_WOOD_BEAST_XBALL, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  BSOS_SetLampState(LA_DTARGET_BONUS_5X, lampPhase1<2, lampPhase1==1);

  // sweep 5
  byte lampPhase2 = (CurrentTime/95)%5;
  BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, lampPhase2==4||lampPhase2==0, lampPhase2==0);
  BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, lampPhase2==3||lampPhase2==4, lampPhase2==4);
  BSOS_SetLampState(LA_STAR_SHOOTER_TOP, lampPhase2==2||lampPhase2==3, lampPhase2==3);
  BSOS_SetLampState(LA_STAR_PFIELD_TOP, lampPhase2==1||lampPhase2==2, lampPhase2==2);
  BSOS_SetLampState(LA_STAR_PFIELD_BOTTOM, lampPhase2<2, lampPhase2==1);

  // sweep 4
  byte lampPhase3 = (CurrentTime/75)%6;
  BSOS_SetLampState(LA_DTARGET_ARROW_3, lampPhase3==5||lampPhase3==0, lampPhase3==0);
  BSOS_SetLampState(LA_DTARGET_ARROW_2, lampPhase3==4||lampPhase3==5, lampPhase3==5);
  BSOS_SetLampState(LA_DTARGET_ARROW_1, lampPhase3==3, lampPhase3==4, lampPhase3==4);
  BSOS_SetLampState(LA_TARGET_UPPER_SPECIAL, lampPhase3==2||lampPhase3==3, lampPhase3==3);
  BSOS_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, lampPhase3==1||lampPhase3==2, lampPhase3==2);
  BSOS_SetLampState(LA_DTARGET_BONUS_4X, lampPhase3<2, lampPhase3==1);

  // remainder
  BSOS_DataRead(0);
  BSOS_SetLampState(LA_POP_TOP, 1, 0, 250);
  BSOS_SetLampState(LA_BONUS_MINI_50K, 1, 0, 250);
  BSOS_SetLampState(LA_BONUS_SUPER_100K, 1, 0, 250);
  BSOS_SetLampState(LA_SHOOT_AGAIN, 1, 0, 250);
  BSOS_SetLampState(LA_MING_TOP, 1, 0, 250);
  BSOS_SetLampState(LA_MING_BOTTOM, 1);

  BackglassLampsLeft2Right();
}



// #################### ADD TO SCORE ####################
void AddToScore(unsigned long scoreAmount) {
  if (Playfield3xState[CurrentPlayer]==2 && Playfield2xState[CurrentPlayer]==2) {
    scoreAmount = scoreAmount * 5;
  } else if (Playfield2xState[CurrentPlayer]==2) {
    scoreAmount = scoreAmount * 2;
  } else if (Playfield3xState[CurrentPlayer]==2) {
    scoreAmount = scoreAmount * 3;
  }
  CurrentScores[CurrentPlayer] += scoreAmount;
}



// #################### TILT HIT ####################
void TiltHit() {
  NumTiltWarnings += 1;
  if (NumTiltWarnings>=MAX_TILT_WARNINGS) {
    #ifndef BALLY_STERN_OS_USE_GEETEOH 
    PlaySound(5); // Stop sound
    #endif
    BSOS_DisableSolenoidStack();
    BSOS_SetDisableFlippers(true);
    BSOS_TurnOffAllLamps();
    if (Credits > 0) BSOS_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    BSOS_SetLampState(LA_TILT, 1);
    PlaySound(41); // Gently, Darling! It's extremely sensitive, like me.
  }
}



// #################### SKILL SHOT ####################
int SkillShot(boolean curStateChanged) {
  int returnState = MACHINE_STATE_SKILL_SHOT;

  if (curStateChanged) {
    BSOS_SetDisplayCredits(0, true);
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 100);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    // Serial.println(F("Skill Shot Started"));
  }

  // HANDLE LAMPS
  if (SkillShotState==0) {
    backglassLampsCenterOut();
    byte lampPhase = (CurrentTime/600)%3;
    BSOS_SetLampState(LA_STAR_SHOOTER_TOP, lampPhase==2, 0, 100);
    BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, lampPhase==1||lampPhase==2, 0, 100);
    BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, lampPhase==0||lampPhase==1||lampPhase==2, 0, 100);
  } else if (SkillShotState==1) {
    BackglassLampsClear();
    if (SkillShotHits==1 || SkillShotHits==2) {
      BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1);
    } else if (SkillShotHits==3 || SkillShotHits==4) {
      BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1);
      BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1);
    } else if (SkillShotHits>=5) {
      BSOS_SetLampState(LA_STAR_SHOOTER_TOP, 1);
      BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1);
      BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1);
    }
  } else if (SkillShotState==2) {
    if (CurrentTime<=SkillShotScoreAnimation) {
      if (SkillShotHits==1 || SkillShotHits==2) {
        BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1, 0, 125);
      } else if (SkillShotHits==3 || SkillShotHits==4) {
        BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1, 0, 125);
        BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1, 0, 125);
      } else if (SkillShotHits>=5) {
        BSOS_SetLampState(LA_STAR_SHOOTER_TOP, 1, 0, 125);
        BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1, 0, 125);
        BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1, 0, 125);
      }
      if (USE_STROBE) BSOS_SetLampState(LA_FLASH_STROBE, 1, 0, 125);
    } else {
      SkillShotState = 3;
    }
  }

  // If the skillshot hasn't been collected yet, flash score
  if (SkillShotState<=2) {
    BSOS_SetDisplayFlash(CurrentPlayer, CurrentScores[CurrentPlayer], CurrentTime, 250, 2);
  } else {
    // Serial.println(F("Skill Shot Ended"));
    BSOS_SetLampState(LA_STAR_SHOOTER_TOP, 0);
    BSOS_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
    BSOS_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
    if (USE_STROBE) BSOS_SetLampState(LA_FLASH_STROBE, 0);
    returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
    for (byte count=0; count<CurrentNumPlayers; count++) {
      BSOS_SetDisplay(count, CurrentScores[count], true, 2);
    }
  }
  // Stop lamp blips for lower level hits during skill shot
  if (CurrentTime >= TargetLowerTopBlip   ) BSOS_SetLampState(LA_TARGET_LRIGHT_TOP, 0);
  if (CurrentTime >= InlaneLeftBlip       ) BSOS_SetLampState(LA_INLANE_LEFT, 0);
  if (CurrentTime >= InlaneRightBlip      ) BSOS_SetLampState(LA_INLANE_RIGHT, 0);
  if (CurrentTime >= TargetLowerBottomBlip) BSOS_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 0);

  // Check to see if ball is in the outhole. Added to fix skillshot straight to outhole, or simply fell back in.
  if (BSOS_ReadSingleSwitchState(SW_OUTHOLE)) {
    if (BallTimeInTrough == 0) {
      BallTimeInTrough = CurrentTime;
    } else {
      // Make sure the ball stays on the sensor for at least 
      // 0.5 seconds to be sure that it's not bouncing
      if ((CurrentTime - BallTimeInTrough) > 500) {
          BSOS_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
          BallTimeInTrough = 0;
          SkillShotState = 0;
          SkillShotHits = 0;
        } 
      }
    }
    else
    BallTimeInTrough = 0;
  
  return returnState;
}



// #################### DROP TARGETS ####################
// ########## DROP TARGET HIT ##########
void DropTargetHit() {
  DropTargetCount++;
  if (DropTargetCount==WIZARD_GOAL_DTARGET) {
    BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 1);
    BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 1);
    OutLanesLit[CurrentPlayer] = true;
    OutLanesCollected[CurrentPlayer] = false;
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 100);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    WizardState[CurrentPlayer] = 1;
    PlaySound(4); // wizard sound: This way Flash, come on! Was 44, Emperor Ming awaits.
  }
}


// ########## CHECK IF DROP TARGETS FOUR DOWN ##########
boolean CheckIfDTargets4Down() {
  return (  BSOS_ReadSingleSwitchState(SW_DTARGET_4_A) &&
            BSOS_ReadSingleSwitchState(SW_DTARGET_4_B) &&
            BSOS_ReadSingleSwitchState(SW_DTARGET_4_C) &&
            BSOS_ReadSingleSwitchState(SW_DTARGET_4_D) );
}


// ########## CHECK IF DROP TARGETS 3 DOWN ##########
boolean CheckIfDTargets3Down() {
  return (  BSOS_ReadSingleSwitchState(SW_DTARGET_3_A) &&
            BSOS_ReadSingleSwitchState(SW_DTARGET_3_B) &&
            BSOS_ReadSingleSwitchState(SW_DTARGET_3_C) );
}


// ########## CHECK 2X BONUS REQUIREMENTS MET ##########
boolean Bonus2XComplete() { 
  return (DTarget3Lights[CurrentPlayer][0]==1 && 
          DTarget3Lights[CurrentPlayer][1]==1 && 
          DTarget3Lights[CurrentPlayer][2]==1 && 
          Playfield2xState[CurrentPlayer]==0);
}


// ########## CHECK IF 3X BONUS REQUIREMENTS MET ##########
boolean Bonus3XComplete() {
  return (DTarget4Light[CurrentPlayer][0]==3 && 
          DTarget4Light[CurrentPlayer][1]==3 && 
          DTarget4Light[CurrentPlayer][2]==3 && 
          DTarget4Light[CurrentPlayer][3]==3);
}


// ########## CHECK HURRY UP COMPLETION ##########
void CheckHurryUpCompletion() {
  // HURRY UP
  if (Bonus3XComplete()) {
    Super100kCollected = true;
    Playfield3xState[CurrentPlayer] = 1;
    CurrentSoundEffectTimer = 0;
    PlaySound(43); // The attack has begun! Was 48.
    BSOS_SetLampState(LA_SAUCER_ARROW_3X, 1, 0, 150);
    BSOS_SetLampState(LA_BONUS_SUPER_100K, 1);
  }
}


// ########## CHECK SAUCER DROP TARGET GOAL ##########
void CheckSaucerDTargetGoal() {
//  if (DTarget4Goal[CurrentPlayer]==true && DTarget3Goal[CurrentPlayer]==true && DTargetInlineGoal[CurrentPlayer]==true && WoodBeastXBallState[CurrentPlayer]==3 && SaucerXBallState[CurrentPlayer]==0) {
  if (DTarget4Goal[CurrentPlayer]==true && DTarget3Goal[CurrentPlayer]==true && DTargetInlineGoal[CurrentPlayer]==true) {
    BSOS_SetLampState(LA_SAUCER_XBALL, 1);
//    SaucerXBallState[CurrentPlayer] = 1;
  }
}



// #################### BONUS ####################
// ########## ADD TO MINI BONUS ##########
void AddToMiniBonus(byte bonusAddition) {
  MiniBonus += bonusAddition;
  if (MiniBonus>MAX_MINI_BONUS) MiniBonus = MAX_MINI_BONUS;
}


// ########## ADD TO SUPER BONUS ##########
void AddToSuperBonus(byte bonusAddition) {
  SuperBonus += bonusAddition;
  if (SuperBonus>MAX_SUPER_BONUS) SuperBonus = MAX_SUPER_BONUS;
}


// ########## SET MINI BONUS INDICATOR ##########
void SetMiniBonusIndicator(byte number, byte state, byte dim, int flashPeriod=0) {
  if (number==0 || number>19) return;
  BSOS_SetLampState(LA_BONUS_MINI_1K+(number-1), state, dim, flashPeriod);
}


// ########## SET SUPER BONUS INDICATOR ##########
void SetSuperBonusIndicator(byte number, byte state, byte dim, int flashPeriod=0) {
  if (number==0 || number>19) return;
  BSOS_SetLampState(LA_BONUS_SUPER_1K+(number-1), state, dim, flashPeriod);
}


// ########## SHOW MINI BONUS ON LADDER ##########
void ShowMiniBonusOnLadder(byte bonus) {
  if (bonus>MAX_MINI_DISPLAY_BONUS) bonus = MAX_MINI_DISPLAY_BONUS;
  byte cap = 10;

  for (byte count=1; count<11; count++) SetMiniBonusIndicator(count, 0, 0);
  
  if (bonus==0) {
    return;
  } else if (bonus<cap) {
    SetMiniBonusIndicator(bonus, 1, 0);
    byte bottom;
    for (bottom=1; bottom<bonus; bottom++){
      SetMiniBonusIndicator(bottom, 1, 0, 0);
    }
  } else if (bonus==cap) {
    SetMiniBonusIndicator(cap, 1, 0, 0);
  } else if (bonus>cap) {
    SetMiniBonusIndicator(cap, 1, 0, 0);
    SetMiniBonusIndicator(bonus-10, 1, 0);
    byte bottom;
    for (bottom=1; bottom<bonus-10; bottom++){
      SetMiniBonusIndicator(bottom, 1, 0, 0);
    }
  }
}


// ########## SHOW SUPER BONUS ON LADDER ##########
void ShowSuperBonusOnLadder(byte bonus) {
  if (bonus>MAX_SUPER_DISPLAY_BONUS) bonus = MAX_SUPER_DISPLAY_BONUS;
  byte cap = 10;

  for (byte count=1; count<11; count++) SetSuperBonusIndicator(count, 0, 0);
  
  if (bonus==0) {
    return;
  } else if (bonus<cap) {
    SetSuperBonusIndicator(bonus, 1, 0);
    byte bottom;
    for (bottom=1; bottom<bonus; bottom++){
      SetSuperBonusIndicator(bottom, 1, 0, 0);
    }
  } else if (bonus==cap) {
    SetSuperBonusIndicator(cap, 1, 0, 0);
  } else if (bonus>cap) {
    SetSuperBonusIndicator(cap, 1, 0, 0);
    SetSuperBonusIndicator(bonus-10, 1, 0);
    byte bottom;
    for (bottom=1; bottom<bonus-10; bottom++){
      SetSuperBonusIndicator(bottom, 1, 0, 0);
    }
  }
  
//  if (bonus>MAX_SUPER_DISPLAY_BONUS) bonus = MAX_SUPER_DISPLAY_BONUS;
//  byte cap = 10;
//
//  for (byte count=1; count<11; count++) SetSuperBonusIndicator(count, 0, 0);
//  if (bonus==0) return;
//
//  while (bonus>cap) {
//    SetSuperBonusIndicator(cap, 1, 0, 100);
//    bonus -= cap;
//    cap -= 1;
//    if (cap==0) bonus = 0;
//  }
//
//  byte bottom;
//  for (bottom=1; bottom<bonus; bottom++){
//    SetSuperBonusIndicator(bottom, 1, 0);
//    // SetSuperBonusIndicator(bottom, 0, 1);
//  }
//
//  if (bottom<=cap) {
//    SetSuperBonusIndicator(bottom, 1, 0);
//  }
}


// ########## SHOW EXTRA BONUS LIGHTS ##########
void ShowExtraBonusLights() {
  if (Mini50kCollected==true) {
    BSOS_SetLampState(LA_BONUS_MINI_50K, 1);
  }
  if (Super100kCollected==true) {
    BSOS_SetLampState(LA_BONUS_SUPER_100K, 1);
  }
  if (BonusXState[CurrentPlayer]==2) {
    BSOS_SetLampState(LA_BONUS_2X, 1);
  } else if (BonusXState[CurrentPlayer]==3) {
    BSOS_SetLampState(LA_BONUS_2X, 0);
    BSOS_SetLampState(LA_BONUS_3X, 1);
    BSOS_SetLampState(LA_DTARGET_BONUS_4X, 1, 0, 200);
  } else if (BonusXState[CurrentPlayer]==4) {
    BSOS_SetLampState(LA_DTARGET_BONUS_4X, 0);
    BSOS_SetLampState(LA_BONUS_3X, 0);
    BSOS_SetLampState(LA_BONUS_4X, 1);
    BSOS_SetLampState(LA_DTARGET_BONUS_5X, 1, 0, 200);
  } else if (BonusXState[CurrentPlayer]==5) {
    BSOS_SetLampState(LA_DTARGET_BONUS_5X, 0);
    BSOS_SetLampState(LA_BONUS_4X, 0);
    BSOS_SetLampState(LA_BONUS_5X, 1);
  }
}


// ########## COUNTDOWN BONUS ##########
int CountdownBonus(boolean curStateChanged) {

  // If this is the first time through the countdown loop
  if (curStateChanged) {
    BSOS_SetLampState(LA_BALL_IN_PLAY, 1, 0, 250);

    CountdownMini = MiniBonus;
    CountdownSuper = SuperBonus;
    CountdownBonusX = BonusXState[CurrentPlayer];

    CountdownStartTime = CurrentTime;
    ShowMiniBonusOnLadder(MiniBonus);
    ShowSuperBonusOnLadder(SuperBonus);

    if (Super100kCollected==true) {
      CurrentScores[CurrentPlayer] += 100000;
      BSOS_SetLampState(LA_BONUS_SUPER_100K, 0);
    }
    if (Mini50kCollected==true) {
      CurrentScores[CurrentPlayer] += 50000;
      BSOS_SetLampState(LA_BONUS_MINI_50K, 0);
    }
    
    LastCountdownReportTime = CountdownStartTime;
    BonusCountDownEndTime = 0xFFFFFFFF;
  }

  BackglassLampsLeft2Right();

  if ((CurrentTime-LastCountdownReportTime)>100) { // adjust speed 300

    if (MiniBonus>0) {
      CurrentScores[CurrentPlayer] += 1000;
      MiniBonus -= 1;
      ShowMiniBonusOnLadder(MiniBonus);
      PlaySound(12); // Ascending sound
      if (BonusXState[CurrentPlayer]>1 && MiniBonus==0) {
        MiniBonus = CountdownMini;
        BonusXState[CurrentPlayer] -= 1;
      }
      if ((BonusXState[CurrentPlayer]==1) && (MiniBonus==0)) BonusXState[CurrentPlayer] = CountdownBonusX;
    } else if (SuperBonus>0) {
      CurrentScores[CurrentPlayer] += 1000;
      SuperBonus -= 1;
      ShowSuperBonusOnLadder(SuperBonus);
      PlaySound(12); // Ascending sound
      if (BonusXState[CurrentPlayer]>1 && SuperBonus==0) {
        SuperBonus = CountdownSuper;
        BonusXState[CurrentPlayer] -= 1;
      }
      if ((BonusXState[CurrentPlayer]==1) && (SuperBonus==0)) BonusXState[CurrentPlayer] = CountdownBonusX;
    } else if (BonusCountDownEndTime==0xFFFFFFFF) {
      BonusCountDownEndTime = CurrentTime+1000;
      BSOS_SetLampState(LA_BONUS_MINI_1K, 0);
      BSOS_SetLampState(LA_BONUS_SUPER_1K, 0);
    }
    LastCountdownReportTime = CurrentTime;
    BSOS_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true, 2);
  }

  if (CurrentTime>BonusCountDownEndTime) { 
    BonusCountDownEndTime = 0xFFFFFFFF;
    PlaySound(8); // Spinner noise
    if (WizardState[CurrentPlayer]==2) {
      WizardState[CurrentPlayer] = 3;
      return MACHINE_STATE_WIZARD_MODE;
    } else {
      return MACHINE_STATE_BALL_OVER;
    }
  }

  return MACHINE_STATE_COUNTDOWN_BONUS;
}



// #################### SHOW MATCH SEQUENCE ####################
int ShowMatchSequence(boolean curStateChanged) {  
  if (curStateChanged) {
    MatchSequenceStartTime = CurrentTime;
    MatchDelay = 1500;
    MatchDigit = random(0,10);
    NumMatchSpins = 0;
    BSOS_SetLampState(LA_MATCH, 1, 0);
    BSOS_SetDisableFlippers();
    ScoreMatches = 0;
    BSOS_SetLampState(LA_BALL_IN_PLAY, 0);
  }

  if (NumMatchSpins<10) {
    if (CurrentTime > (MatchSequenceStartTime + MatchDelay)) {
      MatchDigit += 1;
      if (MatchDigit>9) MatchDigit = 0;
      CurrentSoundEffectTimer = 0;
      PlaySound(30); // Spinner noise. Was 28. Original game used this for both match and outlane. Outlane is now a voice, and unsuitiable for this echo effect.
      BSOS_SetDisplayBallInPlay((int)MatchDigit*10);
      MatchDelay += 200 + 4*NumMatchSpins;
      NumMatchSpins += 1;
      BSOS_SetLampState(LA_MATCH, NumMatchSpins%2, 0);

      if (NumMatchSpins==10) {
        BSOS_SetLampState(LA_MATCH, 0);
        MatchDelay = CurrentTime - MatchSequenceStartTime;
      }
    }
  }

  if (NumMatchSpins>=10 && NumMatchSpins<=13) {
    if (CurrentTime>(MatchSequenceStartTime + MatchDelay)) {
      if ( (CurrentNumPlayers>(NumMatchSpins-10)) && ((CurrentScores[NumMatchSpins-10]/10)%10)==MatchDigit) {
        ScoreMatches |= (1<<(NumMatchSpins-10));
        AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
        TotalReplays += 1;
        BSOS_WriteULToEEProm(BSOS_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
        BSOS_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
        MatchDelay += 1000;
        NumMatchSpins += 1;
        BSOS_SetLampState(LA_MATCH, 1);
      } else {
        NumMatchSpins += 1;
      }
      if (NumMatchSpins==14) {
        MatchDelay += 5000;
      }
    }      
  }

  if (NumMatchSpins>13) {
    if (CurrentTime>(MatchSequenceStartTime + MatchDelay)) {    
      #ifdef BALLY_STERN_OS_USE_GEETEOH 
        if (PersonalGoalOn) { // Feature not ready yet.
        unsigned long highestScore = 0;
        for (byte count=0; count<CurrentNumPlayers; count++) {
            if (CurrentScores[count]>highestScore) highestScore = CurrentScores[count];
            }
        if (highestScore < PersonalGoal)
          PlaySound(46); // Oh dear, how pathetic
        else
          PlaySound(46); // You've saved your Earth. Habe a nice day!
        } else
        PlaySound(46); // No personal goal. Use regular end sound.
      #else
        PlaySound(5);  // sound off
        PlaySound(46); // Try again, Earthling!
      #endif
      // Wait 10 seconds to avoid final sound clobbering first attract sound
      AttractWait = 10000;
      return MACHINE_STATE_ATTRACT;
    }    
  }

  for (byte count=0; count<4; count++) {
    if ((ScoreMatches>>count)&0x01) {
      // If this score matches, we're going to flash the last two digits
      if ( (CurrentTime/200)%2 ) {
        BSOS_SetDisplayBlank(count, BSOS_GetDisplayBlank(count) & 0x1F); // 0x0F 1111 // 0x1F 11111 // 0x7f 1111111
      } else {
        BSOS_SetDisplayBlank(count, BSOS_GetDisplayBlank(count) | 0x60); // 0x30 110000 // 0x60 1100000
      }
    }
  }

  return MACHINE_STATE_MATCH_MODE;
}



// #################### WIZARD MODE ####################
int WizardMode(boolean curStateChanged) {
  int returnState = MACHINE_STATE_WIZARD_MODE;
  BSOS_SetDisplayCredits(MingAttackProgress, true);

  if (curStateChanged) {
    InitWizardModeAnimation = CurrentTime+1770;
    TotalWizardModes++;
    BSOS_WriteULToEEProm(BSOS_TOTAL_WIZ_EEPROM_BYTE, TotalWizardModes);
    BallDrained = false;
    #ifndef BALLY_STERN_OS_USE_GEETEOH 
    PlaySound(48); // wizard sound: miserable earthling
    #else
    PlaySound(43); // The attack has begun!
    #endif
    WizardState[CurrentPlayer] = 3;
    AddToScore(50000);
    BSOS_TurnOffAllLamps();
    if (Credits > 0) BSOS_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    BSOS_PushToTimedSolenoidStack(SO_DTARGET_1_DOWN, 15, CurrentTime);
    BSOS_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 1);
    BSOS_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 1);
    OutLanesLit[CurrentPlayer] = true;
    OutLanesCollected[CurrentPlayer] = false;
    BSOS_SetLampState(LA_POP_TOP, 1);
    BSOS_SetLampState(LA_SPINNER_LEFT, 1);
    BSOS_SetLampState(LA_SPINNER_RIGHT, 1);
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 100);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    if (PlayerShootsAgain==true) {
      BSOS_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
      BSOS_SetLampState(LA_SHOOT_AGAIN, 1);
    }
    BSOS_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime);
    BSOS_PushToTimedSolenoidStack(SO_DTARGET_3_RESET, 15, CurrentTime + 250);
    BSOS_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime + 500);
    // BSOS_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
  }

  // validate wizard mode
  if (WizardState[CurrentPlayer]<4) {
    BSOS_SetDisplayFlash(CurrentPlayer, CurrentScores[CurrentPlayer], CurrentTime, 250, 2);
    BackglassLampsLeft2Right();
  } else {  
    BSOS_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true, 2);
  }

  // handle lamps // commented numbers based on 140 ming attack
  if (WizardState[CurrentPlayer]==5) {
    if (MingAttackProgress==0 || MingAttackProgress<=WIZARD_GOAL_ATTACK/4-1) { // 0-34
      BSOS_SetLampState(LA_FLASH_GORDON_5, 0);
      BSOS_SetLampState(LA_FLASH_GORDON_2, 0);
      BSOS_SetLampState(LA_FLASH_GORDON_4, 0);
      BSOS_SetLampState(LA_FLASH_GORDON_3, 0);
      if (USE_STROBE) BSOS_SetLampState(LA_FLASH_STROBE, 0);

      if (MingAttackProgress==0 || MingAttackProgress<=(WIZARD_GOAL_ATTACK/4)/3-1) { // 0-10 : 11
        if (MingHealth!=0) {
          BSOS_SetLampState(LA_SAUCER_10K, 1, 0, 250);
          BSOS_SetLampState(LA_FLASH_GORDON_6, 1, 0, 250);
          BSOS_SetLampState(LA_FLASH_GORDON_1, 1, 0, 250);
        }
      } else if (MingAttackProgress>=(WIZARD_GOAL_ATTACK/4)/3 && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)/3)*2-1) { // 11-22 : 12
        BSOS_SetLampState(LA_SAUCER_10K, 1, 0, 180);
        BSOS_SetLampState(LA_FLASH_GORDON_6, 1, 0, 180);
        BSOS_SetLampState(LA_FLASH_GORDON_1, 1, 0, 180);
      } else if (MingAttackProgress>=((WIZARD_GOAL_ATTACK/4)/3)*2 && MingAttackProgress<=WIZARD_GOAL_ATTACK/4-1) { // 23-34 : 12
        BSOS_SetLampState(LA_SAUCER_10K, 1, 0, 110);
        BSOS_SetLampState(LA_FLASH_GORDON_6, 1, 0, 110);
        BSOS_SetLampState(LA_FLASH_GORDON_1, 1, 0, 110);
      }

    } else if (MingAttackProgress>=WIZARD_GOAL_ATTACK/4 && MingAttackProgress<=(WIZARD_GOAL_ATTACK/4)*2-1) { // 35-69
      BSOS_SetLampState(LA_SAUCER_10K, 1);
      BSOS_SetLampState(LA_FLASH_GORDON_6, 1);
      BSOS_SetLampState(LA_FLASH_GORDON_1, 1);

      if (MingAttackProgress>=WIZARD_GOAL_ATTACK/4 && MingAttackProgress<=(WIZARD_GOAL_ATTACK/4)+(((WIZARD_GOAL_ATTACK/4)/3)-1)) { // 35-45
        BSOS_SetLampState(LA_SAUCER_20K, 1, 0, 250);
        BSOS_SetLampState(LA_FLASH_GORDON_5, 1, 0, 250);
        BSOS_SetLampState(LA_FLASH_GORDON_2, 1, 0, 250);
      } else if (MingAttackProgress>=(WIZARD_GOAL_ATTACK/4)+((WIZARD_GOAL_ATTACK/4)/3) && MingAttackProgress<=(WIZARD_GOAL_ATTACK/4)+((((WIZARD_GOAL_ATTACK/4)/3)*2)-1)) { // 46-57
        BSOS_SetLampState(LA_SAUCER_20K, 1, 0, 180);
        BSOS_SetLampState(LA_FLASH_GORDON_5, 1, 0, 180);
        BSOS_SetLampState(LA_FLASH_GORDON_2, 1, 0, 180);
      } else if (MingAttackProgress>=(WIZARD_GOAL_ATTACK/4)+(((WIZARD_GOAL_ATTACK/4)/3)*2) && MingAttackProgress<=(((WIZARD_GOAL_ATTACK/4)*2)-1)) { // 58-69
        BSOS_SetLampState(LA_SAUCER_20K, 1, 0, 110);
        BSOS_SetLampState(LA_FLASH_GORDON_5, 1, 0, 110);
        BSOS_SetLampState(LA_FLASH_GORDON_2, 1, 0, 110);
      }

    } else if (MingAttackProgress>=(WIZARD_GOAL_ATTACK/4)*2 && MingAttackProgress<=(WIZARD_GOAL_ATTACK/4)*3-1) { // 70-104
      BSOS_SetLampState(LA_SAUCER_10K, 1);
      BSOS_SetLampState(LA_SAUCER_20K, 1);
      BSOS_SetLampState(LA_FLASH_GORDON_5, 1);
      BSOS_SetLampState(LA_FLASH_GORDON_2, 1);

      if (MingAttackProgress>=(WIZARD_GOAL_ATTACK/4)*2 && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*2)+((WIZARD_GOAL_ATTACK/4)/3-1)) { // 70-80
        BSOS_SetLampState(LA_SAUCER_30K, 1, 0, 250);
        BSOS_SetLampState(LA_FLASH_GORDON_4, 1, 0, 250);
        BSOS_SetLampState(LA_FLASH_GORDON_3, 1, 0, 250);
      } else if (MingAttackProgress>=((WIZARD_GOAL_ATTACK/4)*2)+((WIZARD_GOAL_ATTACK/4)/3) && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*2)+(((WIZARD_GOAL_ATTACK/4)/3)*2-1)) { // 81-92
        BSOS_SetLampState(LA_SAUCER_30K, 1, 0, 180);
        BSOS_SetLampState(LA_FLASH_GORDON_4, 1, 0, 180);
        BSOS_SetLampState(LA_FLASH_GORDON_3, 1, 0, 180);
      } else if (MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*2)+(((WIZARD_GOAL_ATTACK/4)/3)*2) && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*3-1)) { // 91-104
        BSOS_SetLampState(LA_SAUCER_30K, 1, 0, 110);
        BSOS_SetLampState(LA_FLASH_GORDON_4, 1, 0, 110);
        BSOS_SetLampState(LA_FLASH_GORDON_3, 1, 0, 110);
      }

    } else if (MingAttackProgress>=(WIZARD_GOAL_ATTACK/4*3) && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*4-1)) { // 105-139
      BSOS_SetLampState(LA_SAUCER_10K, 1);
      BSOS_SetLampState(LA_SAUCER_20K, 1);
      BSOS_SetLampState(LA_SAUCER_30K, 1);
      BSOS_SetLampState(LA_FLASH_GORDON_4, 1);
      BSOS_SetLampState(LA_FLASH_GORDON_3, 1);

      if (MingAttackProgress>=(WIZARD_GOAL_ATTACK/4)*3 && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*3)+((WIZARD_GOAL_ATTACK/4)/3-1)) { // 105-115
        BSOS_SetLampState(LA_SAUCER_XBALL, 1, 0, 250);
        if (USE_STROBE) BSOS_SetLampState(LA_FLASH_STROBE, 1, 0, 250);
      } else if (MingAttackProgress>=((WIZARD_GOAL_ATTACK/4)*3)+((WIZARD_GOAL_ATTACK/4)/3) && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*3)+(((WIZARD_GOAL_ATTACK/4)/3)*2-1)) { // 116-127
        BSOS_SetLampState(LA_SAUCER_XBALL, 1, 0, 180);
        if (USE_STROBE) BSOS_SetLampState(LA_FLASH_STROBE, 1, 0, 180);
      } else if (MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*3)+(((WIZARD_GOAL_ATTACK/4)/3)*2) && MingAttackProgress<=((WIZARD_GOAL_ATTACK/4)*3)+((WIZARD_GOAL_ATTACK/4)-1)) { // 128-139
        BSOS_SetLampState(LA_SAUCER_XBALL, 1, 0, 110);
        if (USE_STROBE) BSOS_SetLampState(LA_FLASH_STROBE, 1, 0, 110);
      }

    } else if (MingAttackProgress>=WIZARD_GOAL_ATTACK && BallInSaucer==false) { // 140
      MingAttackProgress = WIZARD_GOAL_ATTACK;
      // MingAttackReady = true;
      // BSOS_SetLampState(LA_SAUCER_XBALL, 1);

      byte lampPhaseSaucer = (CurrentTime/150)%4; // 250
      BSOS_SetLampState(LA_SAUCER_XBALL, lampPhaseSaucer==3, lampPhaseSaucer==0, lampPhaseSaucer==0);
      BSOS_SetLampState(LA_SAUCER_30K, lampPhaseSaucer==2||lampPhaseSaucer==3, lampPhaseSaucer==3);
      BSOS_SetLampState(LA_SAUCER_20K, lampPhaseSaucer==1||lampPhaseSaucer==2, lampPhaseSaucer==2);
      BSOS_SetLampState(LA_SAUCER_10K, lampPhaseSaucer<2, lampPhaseSaucer==1);
      if (USE_STROBE) {
        byte lampPhaseFlash = (CurrentTime/150)%4; // 250
        BSOS_SetLampState(LA_FLASH_STROBE, lampPhaseFlash==3, lampPhaseFlash==0, lampPhaseFlash==0);
        BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhaseFlash==2||lampPhaseFlash==3, lampPhaseFlash==3);
        BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhaseFlash==2||lampPhaseFlash==3, lampPhaseFlash==3);
        BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhaseFlash<2, lampPhaseFlash==1);
        BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhaseFlash<2, lampPhaseFlash==1);
      } else {
        byte lampPhaseFlash = (CurrentTime/150)%3; // 250
        BSOS_SetLampState(LA_FLASH_GORDON_4, lampPhaseFlash==2||lampPhaseFlash==0, lampPhaseFlash==0);
        BSOS_SetLampState(LA_FLASH_GORDON_3, lampPhaseFlash==2||lampPhaseFlash==3, lampPhaseFlash==3);
        BSOS_SetLampState(LA_FLASH_GORDON_5, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        BSOS_SetLampState(LA_FLASH_GORDON_2, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        BSOS_SetLampState(LA_FLASH_GORDON_6, lampPhaseFlash<2, lampPhaseFlash==1);
        BSOS_SetLampState(LA_FLASH_GORDON_1, lampPhaseFlash<2, lampPhaseFlash==1);
      }
    }
  }

  // handle ming attack animation
  if (BallInSaucer==true && MingAttackReady==true) {
    if (CurrentTime<=MingAttackAnimation) {
      backglassLampsCenterOut();
      if ((CurrentTime-AttractSweepTime)>25) {
        AttractSweepTime = CurrentTime;
        for (byte lightcountdown=0; lightcountdown<NUM_OF_ATTRACT_LAMPS_MING_ATTACK; lightcountdown++) {
          byte dist = MingAttackLamps - AttractLampsMingAttack[lightcountdown].rowMingAttack;
          BSOS_SetLampState(AttractLampsMingAttack[lightcountdown].lightNumMingAttack, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsMingAttack[lightcountdown].lightNumMingAttack):0);
          if (lightcountdown==(NUM_OF_ATTRACT_LAMPS_MING_ATTACK/2)) BSOS_DataRead(0);
        }
        MingAttackLamps += 1;
        if (MingAttackLamps>30) MingAttackLamps = 0;
      }
      if (MingHealth==0 && CurrentTime>=(MingAttackAnimation-825)) {
        BSOS_DisableSolenoidStack();
      }
    } else {
      for (byte off=LA_BONUS_MINI_1K; off<=LA_TARGET_LRIGHT_TOP; off++) BSOS_SetLampState(off, 0);
      for (byte off=LA_SAUCER_10K; off<=LA_DTARGET_BONUS_5X; off++) BSOS_SetLampState(off, 0);
      for (byte off=LA_POP_TOP; off<=LA_STAR_SHOOTER_BOTTOM; off++) BSOS_SetLampState(off, 0);
      for (byte off=LA_CLOCK_15_SECONDS_3X; off<=LA_SAUCER_ARROW_2X; off++) BSOS_SetLampState(off, 0);
      BSOS_SetLampState(LA_SPINNER_RIGHT, 1);
      BSOS_SetLampState(LA_SPINNER_LEFT, 1);
      BSOS_SetLampState(LA_POP_TOP, 1);
      if (MingHealth!=0) {
        AddToScore(50000);
        BSOS_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime);
      } else {
        // ming defeated
        AddToScore(150000);
        BSOS_SetDisableFlippers(true);
        byte bonusFireworks;
        for (bonusFireworks=0; bonusFireworks<10; bonusFireworks++){
          if ( bonusFireworks % 2 == 0) { // even but odd on pf
            BSOS_SetLampState(LA_BONUS_MINI_1K+bonusFireworks, 1, 0, 100);
            BSOS_SetLampState(LA_BONUS_SUPER_1K+bonusFireworks, 1, 0, 100);
          } else { // odd even on pf
            BSOS_SetLampState(LA_BONUS_MINI_1K+bonusFireworks, 1, 0, 200);
            BSOS_SetLampState(LA_BONUS_SUPER_1K+bonusFireworks, 1, 0, 200);
          }
        }
        BSOS_SetLampState(LA_BONUS_MINI_50K, 1, 0, 500);
        BSOS_SetLampState(LA_BONUS_SUPER_100K, 1, 0, 500);
        BSOS_SetLampState(LA_FLASH_GORDON_1, 1, 0, 200);
        BSOS_SetLampState(LA_FLASH_GORDON_2, 1, 0, 100);
        BSOS_SetLampState(LA_FLASH_GORDON_3, 1, 0, 200);
        BSOS_SetLampState(LA_FLASH_GORDON_4, 1, 0, 200);
        BSOS_SetLampState(LA_FLASH_GORDON_5, 1, 0, 100);
        BSOS_SetLampState(LA_FLASH_GORDON_6, 1, 0, 200);
        if (USE_STROBE) BSOS_SetLampState(LA_FLASH_STROBE, 1, 0, 500);
        TotalWizardModesBeat++;
        BSOS_WriteULToEEProm(BSOS_TOTAL_WIZ_BEAT_EEPROM_BYTE, TotalWizardModesBeat);
        MingDefeatCelebration = true;
      }
      MingAttackLamps = 1;
      MingAttackReady = false;
      MingAttackProgress = 0;
      BallInSaucer = false;
    }
  } else if (BallInSaucer==true && MingAttackReady==false) {
    BSOS_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime + 1000, true); // override used, in case where solenoid stack is disabled and the ball falls back into the saucer
    BallInSaucer = false;
  }

  // handle ming health animation
  if (MingHealth==3) {
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 100);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
  } else if (MingHealth==2) {
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 200);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 300);
  } else if (MingHealth==1) {
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 300);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 400);
  } else if (MingHealth==0) {
    BSOS_SetLampState(LA_MING_TOP, 1, 0, 400);
    BSOS_SetLampState(LA_MING_BOTTOM, 1, 0, 500);
    BSOS_SetLampState(LA_SPINNER_LEFT, 0);
    BSOS_SetLampState(LA_SPINNER_RIGHT, 0);
    BSOS_SetLampState(LA_POP_TOP, 0);
    WizardState[CurrentPlayer] = 6;
  }

  // handle ming defeat celebration sound
  if (MingDefeatCelebration==true) {
    if (CurrentTime>=MingDefeatCelebrationBPM) {
      MingDefeatCelebrationBPM = CurrentTime + 182; // 329 from 182 bpm
      if (MingDefeatCelebrationIncrement==1) {
        PlaySound(32); // wizard sound: ding 1
      } else if (MingDefeatCelebrationIncrement==2) {
        PlaySound(33); // wizard sound: ding 2
      } else if (MingDefeatCelebrationIncrement==3) {
        PlaySound(34); // wizard sound: ding 3
      } else if (MingDefeatCelebrationIncrement==4) {
        PlaySound(33); // wizard sound: ding 2
      } else if (MingDefeatCelebrationIncrement==5) {
        PlaySound(35); // wizard sound: ding 4
      }
      MingDefeatCelebrationIncrement += 1;
      if (MingDefeatCelebrationIncrement > 5) MingDefeatCelebrationIncrement = 1;
    }
  }

  // switch hits
  if (BSOS_ReadSingleSwitchState(SW_OUTHOLE)) {
    if (BallTimeInTrough==0) {
      BallTimeInTrough = CurrentTime;
    } else {
      BSOS_SetLampState(LA_MING_TOP, 0);
      BSOS_SetLampState(LA_MING_BOTTOM, 0);
      // Make sure the ball stays on the sensor for at least 
      // 0.5 seconds to be sure that it's not bouncing
      if ((CurrentTime-BallTimeInTrough)>500) { // was 3000

        if (BallFirstSwitchHitTime==0) BallFirstSwitchHitTime = CurrentTime;
        
        if (NumTiltWarnings>=MAX_TILT_WARNINGS) {
          returnState = MACHINE_STATE_BALL_OVER;
        } else if (NumTiltWarnings<MAX_TILT_WARNINGS) {
          if (WizardState[CurrentPlayer]==3 || WizardState[CurrentPlayer] == 4) { // 4 added, in case it's fallen back into the outhole
            if (CurrentTime>=InitWizardModeAnimation) {
              if (backgroundOn) PlaySound(6); // Background music
              BSOS_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
              WizardState[CurrentPlayer] = 4;
              BallTimeInTrough = 0;
            }
            returnState = MACHINE_STATE_WIZARD_MODE;
          } else if (WizardState[CurrentPlayer]>=5) {
            #ifndef BALLY_STERN_OS_USE_GEETEOH 
            PlaySound(5); // Stop sound
            #endif
            BallDrained = true;
            returnState = MACHINE_STATE_BALL_OVER;
          }
        }
      }
    }
  } else {
    BallTimeInTrough = 0;
  }

  return returnState;
}

// #################### GAME-SPECIFIC SELF TESTS ####################
int RunExtendedSelfTest(int curState, boolean curStateChanged, unsigned long CurrentTime, byte resetSwitch) {
  byte curSwitch = BSOS_PullFirstFromSwitchStack();
  int returnState = curState;
  boolean resetDoubleClick = false;
  unsigned short auditNumStartByte = 0;
  byte highNum =1;
  
if (curState==MACHINE_STATE_TEST_PLAYFIELD_AWARD|| curState==MACHINE_STATE_TEST_THRESHOLD_AWARD || curState==MACHINE_STATE_TEST_HIGHSCORE_AWARD) 
  highNum = 3;
else 
  highNum = 1;

if (curSwitch==SW_SELF_TEST_SWITCH && (CurrentTime - ExtLastSelfTestChange)>500) {         
  returnState -= 1;
  ExtLastSelfTestChange = CurrentTime;
  }

if (curStateChanged) {
  for (int count=0; count<4; count++) {
    BSOS_SetDisplay(count, 0);
    BSOS_SetDisplayBlank(count, 0x00);        
    }
  BSOS_SetDisplayCredits(MACHINE_STATE_TEST_SOUNDS - curState);
  BSOS_SetDisplayBallInPlay(0, false);
  }

if (curState==MACHINE_STATE_TEST_PLAYFIELD_AWARD) {
    auditNumStartByte = BSOS_PLAYFIELD_AWARD;
  } else if (curState==MACHINE_STATE_TEST_THRESHOLD_AWARD) {
    auditNumStartByte = BSOS_THRESHOLD_AWARD;
  } else if (curState==MACHINE_STATE_TEST_BACKGROUND_OFF) {
    auditNumStartByte = BSOS_BACKGROUND_OFF;
  } else if (curState==MACHINE_STATE_TEST_HIGHSCORE_AWARD) {
    auditNumStartByte = BSOS_HIGHSCORE_AWARD;
  } else if (curState==MACHINE_STATE_TEST_FREEPLAY) {
    auditNumStartByte = BSOS_FREEPLAY;
  } else if (curState==MACHINE_STATE_TEST_GOAL_OFFON) {
    auditNumStartByte = BSOS_GOAL_OFFON;
  } 

  if (auditNumStartByte != 0) {
    if (curStateChanged) {
      ExtSavedValue = BSOS_ReadByteFromEEProm(auditNumStartByte) & highNum;
      BSOS_SetDisplay(0, ExtSavedValue, true);
    }
    if (curSwitch == resetSwitch) {
      ExtSavedValue +=1;
      if (ExtSavedValue > highNum) ExtSavedValue = 0;
      BSOS_SetDisplay(0, ExtSavedValue, true);  
      BSOS_WriteByteToEEProm(auditNumStartByte, ExtSavedValue);
    }
  }
  return returnState;
}