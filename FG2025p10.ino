/**************************************************************************

Bally Flash Gordon: Arduino Version 2025.03 by Dave's Think Tank

  Version History*

  Version: 1.0.0   by Tim Murren
  Version: 2024.04 by Dave's Think Tank
  Version: 2024.06 by Dave's Think Tank
  Version: 2024.07 by Dave's Think Tank
  Version: 2024.08 by Dave's Think Tank
  Version: 2024.09 by Dave's Think Tank
  Version: 2024.10 by Dave's Think Tank
  Version: 2024.11 by Dave's Think Tank
  Version: 2024.12 by Dave's Think Tank
  Version: 2025.01 by Dave's Think Tank (converted from BSOS to RPU)
  Version: 2025.03 by Dave's Think Tank
  Version: 2025.07 by Dave's Think Tank
  Version: 2025.08 by Dave's Think Tank
  Version: 2025.09 by Dave's Think Tank (added WAV Trigger)
  Version: 2025.10 by Dave's Think Tank

  * see detailed version notes in VersionNotes.h

**************************************************************************/

//======================================= OPERATOR GAME ADJUSTMENTS =======================================
#define VERSION_NUMBER  2025.10        // Version number appears in Display #1 / Credit display at start of game
#define ATTRACT_SPEECH_TIMER 300000    // (300000) Amount of time between attract mode speech callouts. 60000 = 1 minute
#define DEBUG_MODE 0                   // Debugging options:
                                          // 0 = no debug messages.
                                          // 1 = up to nine switch numbers scrolled through player displays 2, 3, and 4. 
                                          // 2 = display at player 4 the number of calls per second at location of function LoopCount().
                                          // 3 = up to nine sounds scrolled through player displays 2, 3, and 4.
#define MAX_TILT_WARNINGS 1            // Set to number of tilt warnings allowed before losing ball. MINIMUM = 1!
#define MAX_HEALTH 3                   // Number of times Ming must be defeated during Final Battle (maximum 4, usual value is 3).
#define STOP_THE_MUSIC 0               // 0 = ignore. Game button has normal function during game.
                                       // 1 = Music can be stopped during game play by pressing the game button.

// SELECTING SOUND BOARD!!!
// In RPU_Config.h, define exactly one of RPU_OS_USE_S_AND_T, RPU_OS_USE_WAV_TRIGGER, or RPU_OS_USE_WAV_TRIGGER_1p3.
// If you are using an original S&T board, you should define RPU_OS_USE_S_AND_T. Do not define USE_EXTENDED_SOUNDS below.
// If you are using a Geeteoh board, you should define RPU_OS_USE_S_AND_T, and USE_EXTENDED_SOUNDS below.
// If you are using a WAV Trigger, define exactly one of RPU_OS_USE_WAV_TRIGGER or RPU_OS_USE_WAV_TRIGGER_1p3. You must also define USE_EXTENDED_SOUNDS below.
// RPU_OS_USE_WAV_TRIGGER is for older WAV Triggers running an outdated WAV Trigger firmware (prior to v1.3). Any board you purchase today, you will likely 
//   want to use RPU_OS_USE_WAV_TRIGGER_1p3. The difference is a "lock" feature that ensures background music is not turned off when the queue is full.
// Note that both WAV Trigger and Geeteoh boards require you to define USE_EXTENDED_SOUNDS below. Only the original S&T does not have the extended sounds.

#define USE_EXTENDED_SOUNDS            // If defined, program expects a Geeteoh or WAV Trigger sound board to play an extended list of pre-programmed sound files.

//=========================================================================================================

#include "RPU_Config.h"
#include "RPU.h"
#include "FG2025p10.h"
#include "SelfTestAndAudit.h"
#include "VersionNotes.h"

#if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
// The following required for WAV Trigger sound routines:

#include "SendOnlyWavTrigger.h"

void SetupAudio(int sound = 0);
void StopAudio();
void StopAllButBG();
void PlaySoundEffect(int soundEffectNum, int gain = 100);
void ResetGain(int soundEffectNum, int gain);
void ResumeBackgroundSong();
void PlayBackgroundSong(int songNum, int gain = 100);
void SetTrackLoop(int soundNum, boolean enable);
void StopBackgroundSong();
inline void StopSoundEffect(int soundEffectNum);
// void PlaySound(byte soundByte, unsigned long delaySound = 0, int gain = 100);
// void PlayQueue();

// Note: Gain is specified from -70 to +10, and is used to change RELATIVE volumes of different sounds.
//       Negative values are better, as adding multiple loud sounds together with high gains will lose the high and low ends of the sounds.

int InitBgGain = -10;                  // Initial gain (~volume) to be applied to background music
int InitSfxGain = -14;                 // Initial gain to be applied to sound effects
int InitVoiceGain = -8;                // Initial gain to be applied to sounds identified as voices

int AdjBgGain = 50;                    // Gain adjustment to be applied to background music
int AdjSfxGain = 50;                   // Gain adjustment to be applied to sound effects
int AdjVoiceGain = 50;                 // Gain adjustment to be applied to sounds identified as voices

int bgGain = -10;                      // Gain to be applied to background music
int sfxGain = -14;                     // Gain to be applied to sound effects
int voiceGain = -8;                    // Gain to be applied to sounds identified as voices

int AdjustGTO = 6 * 256 + 255;         // Adjustment required to convert Geeteoh file numbering to WAV Trigger (bank 6)

// End of WAV Trigger sound setup
#endif



struct Debounce { // define Debounce structure, to be used in eliminating double hits
  unsigned long start;
  byte wait;
};

// If you're having trouble with double hits, set a "wait" time (second column) until a switch can be re-activated (max wait time 255 ms)
struct Debounce SwitchDebounce[40] = {
{0,   0}, // SW_STARS_PFIELD             0 LEFT & RIGHT R.O. BUTTONS (2 + 2)
{0,   0}, // SW_STARS_SHOOTER_LANE       1 SHOOTER LANE R.0. BUTTONS (3)
{0, 250}, // SW_DTARGET_1                2 TOP SINGLE DROP TARGET
{0, 100}, // SW_SHOOTER_LANE_ROLL        3 SHOOTER LANE ROLLOVER
{0,   0}, // SW_DTARGET_REBOUND          4 DROP TARGET 50 POINT REBOUND (2)
{0,   0}, // SW_CREDIT_BUTTON            5 CREDIT BUTTON
{0,   0}, // SW_TILT                     6 TILT (3)
{0,   0}, // SW_OUTHOLE                  7 OUTHOLE
{0,   0}, // SW_COIN_3                   8 COIN III (RIGHT)
{0,   0}, // SW_COIN_1                   9 COIN I (LEFT)
{0,   0}, // SW_COIN_2                   10 COIN II (CENTER)
{0, 250}, // SW_TARGET_LRIGHT_BOTTOM     11 LOWER RIGHT SIDE, BOTTOM TARGET
{0, 150}, // SW_INLANE_R                 12 FLIPPER FEED LANE (RIGHT)
{0, 150}, // SW_INLANE_L                 13 FLIPPER FEED LANE (LEFT)
{0, 250}, // SW_TARGET_LRIGHT_TOP        14 LOWER RIGHT SIDE, TOP TARGET
{0,   0}, // SW_SLAM                     15 SLAM (2)
{0, 250}, // SW_DTARGET_4_D              16 4 DROP TARGET "A" (BOTTOM)
{0, 250}, // SW_DTARGET_4_C              17 4 DROP TARGET "B"
{0, 250}, // SW_DTARGET_4_B              18 4 DROP TARGET "C"
{0, 250}, // SW_DTARGET_4_A              19 4 DROP TARGET "D" (TOP)
{0, 250}, // SW_DTARGET_3_A              20 #1 ARROW DROP TARGET (TOP)
{0, 250}, // SW_DTARGET_3_B              21 #2 ARROW DROP TARGET (MIDDLE)
{0, 250}, // SW_DTARGET_3_C              22 #3 ARROW DROP TARGET (BOTTOM)
{0, 150}, // SW_TARGET_TOP               23 TOP TARGET
{0, 250}, // SW_DTARGET_INLINE_1ST       24 1ST INLINE DROP TARGET
{0, 250}, // SW_DTARGET_INLINE_2ND       25 2ND INLINE DROP TARGET
{0, 250}, // SW_DTARGET_INLINE_3RD       26 3RD INLINE DROP TARGET
{0, 250}, // SW_TARGET_WOOD_BEAST        27 INLINE BACK TARGET
{0,   0}, // SW_REBOUND                  28 10 POINT REBOUND (2)
{0,   0}, // SW_SAUCER                   29 SAUCER
{0, 250}, // SW_OUTLANE_RIGHT            30 RIGHT OUTLANE
{0, 250}, // SW_OUTLANE_LEFT             31 LEFT OUTLANE
{0,   0}, // SW_SPINNER_RIGHT            32 RIGHT SPINNER
{0,   0}, // SW_SPINNER_LEFT             33 LEFT SPINNER
{0, 250}, // SW_SLING_RIGHT              34 RIGHT SLINGSHOT
{0, 250}, // SW_SLING_LEFT               35 LEFT SLINGSHOT
{0,   0}, // SW_POP_TOP                  36 TOP THUMPER BUMPER
{0,   0}, // N/A                         37 not used
{0,   0}, // SW_POP_RIGHT                38 RIGHT THUMPER BUMPER
{0,   0}  // SW_POP_LEFT                 39 LEFT THUMPER BUMPER
};

// if vibration from any solenoid is causing issues (e.g. switches firing), place "wait" time in this structure (max 255ms in 2nd column), and program response in ResetHitFix().
// Wait time can be determined from Solenoid test, which gives number of milliseconds between solenoid and switch. Use this as the minimum wait time.
struct Debounce ResetHits[17] = {
  {0, 250}, // SO_DTARGET_4_RESET        0 4 DROP TARGET                      (Solenoid test shows 133 milliseconds till Woodbeast switch activates)
  {0, 250}, // SO_DTARGET_3_RESET        1 3 DROP TARGET
  {0, 250}, // SO_DTARGET_INLINE_RESET   2 INLINE DROP TARGET
  {0,   0}, // SO_SAUCER_DOWN            3 SAUCER KICK DOWN
  {0,   0}, // N/A                       4 not used
  {0,   0}, // SO_KNOCKER                5 KNOCKER
  {0,   0}, // SO_OUTHOLE                6 OUTHOLE KICKER
  {0,   0}, // SO_SAUCER_UP              7 SAUCER KICK UP
  {0, 250}, // SO_DTARGET_1_RESET        8 SINGLE DROP TARGET RESET
  {0,   0}, // SO_POP_LEFT               9 LEFT THUMPER BUMPER
  {0,   0}, // SO_POP_RIGHT              10 RIGHT THUMPER BBUMPER
  {0,   0}, // SO_DTARGET_1_DOWN         11 SINGLE DROP TARGET PULL DOWN
  {0,   0}, // SO_POP_TOP                12 TOP THUMPER BUMPER
  {0,   0}, // SO_SLING_LEFT             13 LEFT SLINGSHOT
  {0,   0}, // SO_SLING_RIGHT            14 RIGHT SLINGSHOT
  {0,   0}, // SO_COIN_LOCKOUT           15 COIN LOCKOUT DOOR
  {0,   0}  // SO_FLIPPER_ENABLE         16 K1 RELAY (FLIPPER ENABLE)
};

int i, j, k;

int MachineState = 0;
boolean MachineStateChanged = true;
unsigned long CurrentTime = 0;
unsigned long AttractWait = 0;
unsigned long InitGameStartTime = 0;
byte HoldQuarter = 255;
unsigned long LoopTime = 0;
unsigned long LCount = 0;


unsigned long PlayerAddedTime = 0;
boolean       PlayerAdded = false;

unsigned long DisplayTimer = 0;
unsigned long DisplayValue = 0;
unsigned long DisplayCDBTime = 100;
unsigned long kidTimer = 0;
unsigned long delayTimer = 0;

byte dispSwitch[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
byte switchHit;
byte kidSwitch = 0xFF;
byte CDBSpeed = 100;

unsigned long TotalPlays = 0;
unsigned long TotalReplays = 0;
unsigned long ScoreAward1 = 0;
unsigned long ScoreAward2 = 0;
unsigned long ScoreAward3 = 0;
unsigned long PersonalGoal = 0;
unsigned long highestScore = 0;

byte ScoreAwardStates[4] = {0, 0, 0, 0};

unsigned long Credits = 0;
byte WholeCredit = 0;
unsigned long CoinsChute1 = 0;
unsigned long CoinsChute2 = 0;
unsigned long CoinsChute3 = 0;

unsigned long AttractStartAnimation = 0;
unsigned long InitGamePlayAnimation = 0;
unsigned long InitNewBallAnimation = 0;
unsigned long SkillShotScoreAnimation = 0;
unsigned long SaucerAnimation = 0;
unsigned long InitWizardModeAnimation = 0;

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
boolean FirstBall = 1;

byte NumTiltWarnings = 0;

byte SkillShotHits     =   0;
byte SkillShotState    =   0;        // 0 not started, 1 started, 2 collecting, 3 collected/fail

#define TIME_TO_WAIT_FOR_BALL 1000
unsigned long BallTimeInTrough = 0;
unsigned long BallSaveStartTime = 0;
byte ballSaves = 1;
unsigned long ballSaveScore     = 5000;
unsigned long ballSaveNumMS     = 15000;
byte BallSaveUsed[4] = {ballSaves, ballSaves, ballSaves, ballSaves};
boolean PFValidated = false;
unsigned long InitialScore = 0;

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
boolean LeftSpinnerMissed = false;
boolean RightSpinnerMissed = false;
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
unsigned long MingAttackAnimation = 0;
unsigned long MingDefeatCelebrationBPM = 0;
boolean MingAttackReady = false;
boolean MingWasDefeated = false;
byte MingMax = MAX_HEALTH;
byte MingHealth = MingMax;
byte MingAttackLamps = 1;
byte MingDefeatCelebrationIncrement = 1;
byte WizSource = 1;
boolean MingDefeated = false;

#ifdef USE_EXTENDED_SOUNDS
boolean MingDefeatCelebration = false; // original Ming defeat sound using tones is not played
#else
boolean MingDefeatCelebration = true;  // Play only if you have nothing better!
#endif

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
unsigned long TimerSoundEnd = 0;

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
byte CurrentSound = 0;

// DIP Switch Variables
byte dipBank[4]; 
byte dipChute1;
boolean dipSaucerStart10k;
boolean dipSaucerMemoryValues;
boolean dip2X3XArrowMemory;
byte dipChute3;
boolean dipOutlaneSpecialMemory;
boolean dipTopSpecialMemory;
boolean dipXBonus;
byte dipChute2;
boolean dipAttractVoice;
boolean dipSideTargetMemory;
boolean dip4DropTargetMemory;
boolean dip123ArrowsMemory;
byte dipMaxCredits;
boolean dipCreditsDisplayed;
boolean dipMatch;
boolean dipUnlimitedReplays;
boolean dipUnlimitedExtraBalls;
byte dipBallsPerGame;

byte CoinsperCredit[3]; // Number of coins required to earn one credit, by coin chute
byte CreditsperCoin[3]; // Number of credits received per coin, by coin chute
unsigned long GetValue;

byte    PlayfieldAward;              // (2) points, extra ball, or credits awarded as per Bally manual for playfield specials
byte    ThresholdAward;              // (2) points, extra ball, or credits awarded as per Bally manual for beating 3 game threshold levels
boolean backgroundOn;                // (1) background sound / music set either on or off
byte    HighScoreAward;              // (3) Number of free games awarded for beating high score
boolean FreePlay;                    // (0) Turn free play either on or off
boolean PersonalGoalOn;              // (1) Turn personal goal feature on or off
byte    kidMode = 0;                 // (0) Beginner mode settings applied, specifically for kids
boolean OriginalRules = 0;           // (0) Accept or reject certain rules from the original game
byte    WizardGoalTargets =  11;     // (11) set number of drop targets needed to light wizard mode at outlanes
byte    WizardGoalAttack  = 140;     // (140) set attack power needed to light saucer in wizard mode

unsigned long HighScore = 0;
unsigned long HighScoreBeat = 0;
unsigned long TotalSkill1 = 0;
unsigned long TotalSkill2 = 0;
unsigned long TotalSkill3 = 0;
unsigned long TotalWizardModes = 0;
unsigned long TotalWizardModesBeat = 0;

long          ExtSavedValue     = 0;
unsigned long ExtResetHold      = 0;
unsigned long ExtLastResetPress = 0;
byte          ExtDispNum        = 0;
int           ExtUpDown         = 1;
byte          skillNum          = 0;

byte           numStrobeFlashes   = 0;
int            freqStrobeFlashes  = 250;
boolean        stateStrobeFlashes = 0;
unsigned long  nextStrobeChange   = 0;

void SpiralPattern(boolean go);
void ShowMiniBonusOnLadder(byte bonus);
void ShowSuperBonusOnLadder(byte bonus);

// #################### SET UP DIP Variables using dipBank[] ####################
void SetupDIPVars() { // Set up all variables derived from the DIP switches
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
  } 
  else if (dipChute1 == 22){
    CreditsperCoin[0] = 3;
    CoinsperCredit[0] = 2;
  }
  else if (dipChute1 <= 24){
    CreditsperCoin[0] = 3;
    CoinsperCredit[0] = 4;
  }
  else if (dipChute1 == 25){
    CreditsperCoin[0] = 5;
    CoinsperCredit[0] = 4;
  }
  else if (dipChute1 <= 27){
    CreditsperCoin[0] = 7;
    CoinsperCredit[0] = 4;
  }
  else {
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
  } 
  else if (dipChute3 == 22){
    CreditsperCoin[2] = 3;
    CoinsperCredit[2] = 2;
  }
  else if (dipChute3 <= 24){
    CreditsperCoin[2] = 3;
    CoinsperCredit[2] = 4;
  }
  else if (dipChute3 == 25){
    CreditsperCoin[2] = 5;
    CoinsperCredit[2] = 4;
  }
  else if (dipChute3 <= 27){
    CreditsperCoin[2] = 7;
    CoinsperCredit[2] = 4;
  }
  else {
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
}


// #################### SETUP ####################
void setup() {
  // read analog pin for match sequence
  randomSeed(analogRead(A6));

  // Tell the OS about game-specific lights and switches
  RPU_SetupGameSwitches(NUM_SWITCHES_WITH_TRIGGERS, NUM_PRIORITY_SWITCHES_WITH_TRIGGERS, TriggeredSwitches);

  // Set up the chips and interrupts
  RPU_InitializeMPU(RPU_CMD_BOOT_ORIGINAL_IF_CREDIT_RESET | RPU_CMD_BOOT_ORIGINAL_IF_NOT_SWITCH_CLOSED | RPU_CMD_PERFORM_MPU_TEST, SW_CREDIT_BUTTON);
  RPU_DisableSolenoidStack();
  RPU_SetDisableFlippers(true);

  // Get all dip variables
  for (i = 0; i < 4; ++i) {
    dipBank[i] = RPU_GetDipSwitches(i);
    RPU_WriteByteToEEProm(RPU_DIP_BANK + i, dipBank[i]);
  }

  SetupDIPVars();

  RPU_SetLampState(LA_FLASH_STROBE, 0); // Strobe is turned off. RPU knows to invert signal if necessary.
  
  RPU_SetDisplay(0, floor(VERSION_NUMBER), true, 2);
  RPU_SetDisplayBlank(1, 0x00);
  RPU_SetDisplayBlank(2, 0x00);
  RPU_SetDisplayBlank(3, 0x00);

  RPU_SetDisplayCredits(floor(100 * (VERSION_NUMBER + 0.005 - floor(VERSION_NUMBER))));

  #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
  RPU_SetDisplayBallInPlay(3, true, false); // W for WAV Trigger
  #elif defined(USE_EXTENDED_SOUNDS)
  RPU_SetDisplayBallInPlay(6, true, false); // G for Geeteoh
  #else
  RPU_SetDisplayBallInPlay(5, true, false); // S for Squawk & Talk
  #endif

  delay(4000);

  RPU_SetDisplayBlank(0, 0);
  RPU_SetDisplayBlank(4, 0);

  CurrentSoundEffectTimer = 0;
  CurrentSound = 0;

  Credits = RPU_ReadByteFromEEProm(RPU_CREDITS_EEPROM_BYTE);
  if (Credits > dipMaxCredits) Credits = dipMaxCredits;
  RPU_SetLampState(LA_CREDIT_INDICATOR, Credits > 0);

  TotalPlays           = RPU_ReadULFromEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE, 0);
  TotalReplays         = RPU_ReadULFromEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, 0);
  ScoreAward1          = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_1_EEPROM_START_BYTE, 0); // 600,000
  ScoreAward2          = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_2_EEPROM_START_BYTE, 0); // 1,200,000
  ScoreAward3          = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_3_EEPROM_START_BYTE, 0); // 2,800,000 
  PersonalGoal         = RPU_ReadULFromEEProm(RPU_PERSONAL_GOAL_EEPROM_START_BYTE, 500000); // 500,000
  HighScore            = RPU_ReadULFromEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, 1000);
  HighScoreBeat        = RPU_ReadULFromEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE, 0);
  TotalSkill1          = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE, 0);
  TotalSkill2          = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_2_EEPROM_BYTE, 0);
  TotalSkill3          = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_3_EEPROM_BYTE, 0);
  TotalWizardModes     = RPU_ReadULFromEEProm(RPU_TOTAL_WIZ_EEPROM_BYTE, 0);
  TotalWizardModesBeat = RPU_ReadULFromEEProm(RPU_TOTAL_WIZ_BEAT_EEPROM_BYTE, 0);
  CoinsChute1          = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE, 0);
  CoinsChute2          = RPU_ReadULFromEEProm(RPU_CHUTE_2_COINS_START_BYTE, 0);
  CoinsChute3          = RPU_ReadULFromEEProm(RPU_CHUTE_3_COINS_START_BYTE, 0);
  PlayfieldAward       = RPU_ReadByteFromEEProm(RPU_PLAYFIELD_AWARD) & 3;
  ThresholdAward       = RPU_ReadByteFromEEProm(RPU_THRESHOLD_AWARD) & 3;
  HighScoreAward       = RPU_ReadByteFromEEProm(RPU_HIGHSCORE_AWARD) & 3;
  backgroundOn         = RPU_ReadByteFromEEProm(RPU_BACKGROUND_OFF) & 1;
  FreePlay             = RPU_ReadByteFromEEProm(RPU_FREEPLAY) & 1;
  PersonalGoalOn       = RPU_ReadByteFromEEProm(RPU_GOAL_OFFON) & 1;
  WizardGoalTargets    = RPU_ReadByteFromEEProm(RPU_WIZARD_GOAL_TARGETS);
  WizardGoalAttack     = RPU_ReadByteFromEEProm(RPU_WIZARD_GOAL_ATTACK);
  kidMode              = 0; // Never start in kid mode; it must be selected
  RPU_WriteByteToEEProm(RPU_TEMP_KID_MODE_OFFON, kidMode);
  OriginalRules        = RPU_ReadByteFromEEProm(RPU_ORIGINAL_RULES) & 1;
  ballSaves            = RPU_ReadByteFromEEProm(RPU_BALL_SAVES);
  ballSaveScore        = 100 * RPU_ReadByteFromEEProm(RPU_BALL_SAVE_SCORE);
  ballSaveNumMS        = 1000 * RPU_ReadByteFromEEProm(RPU_BALL_SAVE_NUM_SECS);

  #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
  AdjBgGain            = RPU_ReadByteFromEEProm(RPU_BG_GAIN);
  AdjSfxGain           = RPU_ReadByteFromEEProm(RPU_SFX_GAIN);
  AdjVoiceGain         = RPU_ReadByteFromEEProm(RPU_VOICE_GAIN);
  #endif

  if (WizardGoalTargets == 0) {
    WizardGoalTargets =  11;
    RPU_WriteByteToEEProm(RPU_WIZARD_GOAL_TARGETS, WizardGoalTargets);
  }
  if (WizardGoalAttack  == 0) {
    WizardGoalAttack  = 140;
    RPU_WriteByteToEEProm(RPU_WIZARD_GOAL_ATTACK, WizardGoalAttack);
  }
  if ((ballSaves == 0 && ballSaveScore == 0 && ballSaveNumMS == 0) 
          || ballSaves > 25 || ballSaveScore > 25000 || ballSaveNumMS > 25000) { // Likely first time running program! Set defaults.
    ballSaves = 1;
    ballSaveScore = 5000;
    ballSaveNumMS = 15000;
    RPU_WriteByteToEEProm(RPU_BALL_SAVES, ballSaves);
    RPU_WriteByteToEEProm(RPU_BALL_SAVE_SCORE, ballSaveScore / 100);
    RPU_WriteByteToEEProm(RPU_BALL_SAVE_NUM_SECS, ballSaveNumMS / 1000);
  }
  
  #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
  if ((AdjBgGain <= 0 || AdjBgGain > 100) && (AdjSfxGain <= 0 || AdjSfxGain > 100)
                                              && (AdjVoiceGain <= 0 || AdjVoiceGain > 100)) { // Likely first time running program! Set defaults.
    AdjBgGain = 50;
    AdjSfxGain = 50;
    AdjVoiceGain = 50;
    RPU_WriteByteToEEProm(RPU_BG_GAIN, AdjBgGain);
    RPU_WriteByteToEEProm(RPU_SFX_GAIN, AdjSfxGain);
    RPU_WriteByteToEEProm(RPU_VOICE_GAIN, AdjVoiceGain);
  }
  CalculateGain();
  SetupAudio(50); // Flash!
  #endif
}


#define SOUND_STACK_SIZE   20
#define GTO_MIN_TIME       25

unsigned long GTOWaitingTime = 0;
unsigned long GTOLatestSound = 0;
byte soundType;
int iGTO, jGTO, kGTO;

#if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
SendOnlyWavTrigger wTrig;             // Our WAV Trigger object
#define SOUND_EFFECT_NONE               0
int CurrentBackgroundSong = SOUND_EFFECT_NONE;
#endif

struct SoundStruct { // define sound structure, to be used in storing data needed to delay and play sounds
  int sound;
  byte type;
  int  gain;
  unsigned long start;
  unsigned long length;
};

struct SoundStruct SoundQueue[SOUND_STACK_SIZE] = {
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0},
{0, 0xFF, 0, 0, 0}
};

// #################### PLAY A SOUND USING APPROPRIATE SOUND BOARD WITH NO DELAY ####################
// (for use only by PlaySound and QueueSound, to determine and use appropriate sound board)
void PlaySoundNow(int soundByte, byte soundType, int gain) {

  DebugSound(soundByte); // Set DEBUG_MODE to 3 to track sounds through displays

  #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
  if (soundByte == 90)
    StopBackgroundSong();
  else if (soundType == 1) {
    PlayBackgroundSong(AdjustGTO - soundByte, gain); // Geeteoh switches sound numbers this way. This is required to have Geeteoh and WAV Trigger use the same numbering.
    // wTrig.trackGain(AdjustGTO - soundByte, gain);
  } else {
    PlaySoundEffect(AdjustGTO - soundByte, gain);
    // wTrig.trackGain(AdjustGTO - soundByte, gain);
  }
  #else
    #ifndef USE_EXTENDED_SOUNDS
    if (soundByte == 90)
      RPU_PlaySoundSAndT(5);
    else
      RPU_PlaySoundSAndT(soundByte);
    #else
    RPU_PlaySoundSAndT(soundByte);
    #endif
  #endif
}

// #################### PLAY ANY TYPE OF SOUND, OR ADD IT TO THE QUEUE IF NECESSARY ####################
void PlaySound(int soundByte, unsigned long delaySound = 0, int gain = 100) {

// Specify number of sound to play (see SoundData list of sounds), 
//    delay until sound played (in ms, default zero), 
//    and gain (like volume but not quite. Separate defaults for background, voice, and special effects).
//
// Note: Gain is specified from -70 to +10, and is used to change RELATIVE volumes of different sounds.
//       Negative values are better, as adding multiple loud sounds together with high gains will lose the high and low ends of the sounds.

  if (CurrentTime - GTOLatestSound < GTO_MIN_TIME)                  // Give Geeteoh time to deal with each command
    delaySound = max(delaySound, GTO_MIN_TIME + GTOLatestSound - CurrentTime);

  if (SoundData[soundByte][1] == 1 || 
      SoundData[soundByte][1] == 8 || 
      SoundData[soundByte][1] == 9)      soundType = 1;      // Background / Stop All / Alarm 
  else if (SoundData[soundByte][0] == 1) soundType = 2;      // Voice
  else soundType = 3;                                        // SFX

  #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
  if (gain == 100) {
    if (soundType == 1) gain = bgGain;
    else if (soundType == 2) gain = voiceGain;
    else if (soundType == 3) gain = sfxGain;
  }
  #endif
  
  if (soundType == 1 && delaySound == 0) {     // Play background music immediately, unless delayed
    PlaySoundNow(soundByte, soundType, gain);
    GTOLatestSound = CurrentTime;
    return;
  }
  if (soundType == 2 && delaySound == 0 && GTOWaitingTime < CurrentTime) {   // Play voice immediately, unless delayed or another voice playing
    PlaySoundNow(soundByte, soundType, gain);
    GTOLatestSound = CurrentTime;
    #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
    GTOWaitingTime = CurrentTime + 100 * (unsigned long) SoundData[soundByte][2];
    #elif defined(USE_EXTENDED_SOUNDS)
    GTOWaitingTime = CurrentTime + 100 * (unsigned long) max(SoundData[soundByte][2], SoundData[soundByte][3]);
    #else
    GTOWaitingTime = CurrentTime + 100 * (unsigned long) SoundData[soundByte][4];
    #endif
    return;
  }
  if (soundType == 3 && delaySound == 0) {   // Play sound effect immediately, unless delayed
    PlaySoundNow(soundByte, soundType, gain);
    GTOLatestSound = CurrentTime;
    return;
  }
  kGTO = 99;
  for (iGTO = 0; iGTO < SOUND_STACK_SIZE; ++iGTO) {                    // Find space in queue
    if (SoundQueue[iGTO].type == 0xFF) {
      kGTO = iGTO;
      iGTO = 99;
    }
  }
  if (kGTO >= SOUND_STACK_SIZE) kGTO = 0;                           // Add new sound to queue. Write over oldest value if queue full :(

  SoundQueue[kGTO].sound  = soundByte;
  SoundQueue[kGTO].type   = soundType;
  SoundQueue[kGTO].gain   = gain;
  SoundQueue[kGTO].start  = CurrentTime + delaySound;
  #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
  SoundQueue[kGTO].length = 100 * (unsigned long) SoundData[soundByte][2];
  #elif defined(USE_EXTENDED_SOUNDS)
  SoundQueue[kGTO].length = 100 * (unsigned long) max(SoundData[soundByte][2], SoundData[soundByte][3]);
  #else
  SoundQueue[kGTO].length = 100 * (unsigned long) SoundData[soundByte][4];
  #endif
}

// #################### CHECK IF A SOUND IS QUEUED AND READY TO PLAY ####################
void PlayQueue() {

  // Run this every time through loop(). It looks for sounds set up with delays through PlaySound(), and plays them when appropriate.

  if (CurrentTime - GTOLatestSound < GTO_MIN_TIME)                  // Give Geeteoh time to deal with each command
    return;

  kGTO = 99;
  for (iGTO = 0; iGTO < SOUND_STACK_SIZE; ++iGTO){                           // Find next sound to be played
    if (SoundQueue[iGTO].type != 0xFF) {                                     // Look for valid sound type. 0xFF indicates empty row of queue.
      if (CurrentTime >= SoundQueue[iGTO].start) {                           // This sound is ready to be played
        if (SoundQueue[iGTO].type == 1) {                                    // Background sounds always play at requested start time
          PlaySoundNow(SoundQueue[iGTO].sound, SoundQueue[iGTO].type, SoundQueue[iGTO].gain);
          GTOLatestSound = CurrentTime;
          SoundQueue[iGTO].type = 0xFF;
          kGTO = iGTO;
          iGTO = 99;
        }
        else if (SoundQueue[iGTO].type == 2 && CurrentTime > GTOWaitingTime) { // Voices need to wait for other voices to finish
          PlaySoundNow(SoundQueue[iGTO].sound, SoundQueue[iGTO].type, SoundQueue[iGTO].gain);
          GTOLatestSound = CurrentTime;
          GTOWaitingTime = CurrentTime + SoundQueue[iGTO].length;
          SoundQueue[iGTO].type = 0xFF;
          kGTO = iGTO;
          iGTO = 99;
        }
        else if (SoundQueue[iGTO].type == 3) {                              // Sound effects can play at requested start time
          PlaySoundNow(SoundQueue[iGTO].sound, SoundQueue[iGTO].type, SoundQueue[iGTO].gain);
          GTOLatestSound = CurrentTime;
          SoundQueue[iGTO].type = 0xFF;
          kGTO = iGTO;
          iGTO = 99;
        }
      }
    }
  }
  if (kGTO < SOUND_STACK_SIZE - 1) {                                        // Remove played sound from stack. Compress stack.
    for (iGTO = kGTO + 1; iGTO < SOUND_STACK_SIZE; ++iGTO) {
      SoundQueue[iGTO - 1].sound    = SoundQueue[iGTO].sound;
      SoundQueue[iGTO - 1].type     = SoundQueue[iGTO].type;
      SoundQueue[iGTO - 1].gain     = SoundQueue[iGTO].gain;
      SoundQueue[iGTO - 1].start    = SoundQueue[iGTO].start;
      SoundQueue[iGTO - 1].length   = SoundQueue[iGTO].length;
    }
    SoundQueue[SOUND_STACK_SIZE - 1].type = 0xFF;
  }
}

// #################### GET RID OF ANY ITEMS SITTING IN THE QUEUE ####################

void DeleteQueue() {

  // Get rid of any items sitting in the queue

  for (iGTO = 0; iGTO < SOUND_STACK_SIZE; ++iGTO)
    SoundQueue[iGTO].type = 0xFF;
}



// #################### ADD CREDIT ####################
void AddCredit(byte Chute, byte CredperCoin, byte CoinperCred, byte creds = 0) {
  if (creds != 0) {
    Credits = Credits + creds;
  }
  else {
    WholeCredit += 1;
    if (WholeCredit < CoinperCred) {
      PlaySound(min(35, 31 + WholeCredit)); // Ascending dings
    } else 
    {
      Credits += CredperCoin;
      #ifdef USE_EXTENDED_SOUNDS
      if (kidMode)
        PlaySound(38); // Kid Ming laugh!
      else
        PlaySound(50); // Ming laugh!
      #else
      PlaySound(50); // Ming laugh!
      #endif
      WholeCredit = 0;

    }
  }
  if (Credits > 0) RPU_SetLampState(LA_CREDIT_INDICATOR, 1);
  if (Credits > dipMaxCredits) Credits = dipMaxCredits;
  RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, Credits);
  RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
  RPU_SetCoinLockout((Credits < dipMaxCredits) ? false:true);
}



// #################### ADD PLAYER ####################
boolean AddPlayer(boolean resetNumPlayers = false) {
  if (Credits<1 && !FreePlay) return false;
  if (resetNumPlayers) CurrentNumPlayers = 0;
  if (CurrentNumPlayers>=4) return false;

  CurrentNumPlayers += 1;
  RPU_SetDisplay(CurrentNumPlayers-1, 0, true, 2);
  TotalPlays++;
  RPU_WriteULToEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE, TotalPlays);

  if (!FreePlay) {
    Credits -= 1;
    #ifdef USE_EXTENDED_SOUNDS 
    PlaySound(20); // Ming's ring
    #else
    PlaySound(18); // Flash!
    #endif
    RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, Credits);
    RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
    RPU_SetLampState(LA_CREDIT_INDICATOR, Credits > 0);
    RPU_SetCoinLockout((Credits < dipMaxCredits) ? false:true);
  }
  else {
    #ifdef USE_EXTENDED_SOUNDS 
    if (kidMode)
      PlaySound(38); // Kid Ming laugh!
    else
      PlaySound(50); // Ming laugh!
    #else
      PlaySound(50); // Ming laugh!
    #endif
  }
  return true;
}


// #################### Set up strobe light to flash ####################
void SetupStrobe(byte num, int freq) {
  numStrobeFlashes = num;
  freqStrobeFlashes = freq;
  stateStrobeFlashes = 0;
  nextStrobeChange = CurrentTime;
}


// #################### Flash strobe light ####################
void FlashStrobe() {
  if (numStrobeFlashes > 0) {
    if (CurrentTime >= nextStrobeChange) {
      if (stateStrobeFlashes == 0) { // strobe is off
        RPU_SetLampState(LA_FLASH_STROBE, stateStrobeFlashes = 1);
        nextStrobeChange = CurrentTime + freqStrobeFlashes;
      }
    else { // strobe is on
        RPU_SetLampState(LA_FLASH_STROBE, stateStrobeFlashes = 0);
        numStrobeFlashes -= 1;
        nextStrobeChange = CurrentTime + freqStrobeFlashes;
      }
    }
  }
}


// #################### Scroll latest 9 switch hits through player 2, 3, and 4 displays ####################
boolean Debugger(byte hit) { 

  if (DEBUG_MODE == 1) {
    if (hit == SW_CREDIT_BUTTON) return true; // credit button does not mean to add a player! Do not execute.
    if (RPU_ReadSingleSwitchState(SW_CREDIT_BUTTON)) return false; // hold current values if game button being held.
    for (i = 0; i < 8; ++i) dispSwitch[i] = dispSwitch[i+1];
    dispSwitch[8] = hit;
    RPU_SetDisplay(1, (((unsigned long) dispSwitch[0] * 100) + dispSwitch[1]) * 100 + dispSwitch[2], true, 6);
    RPU_SetDisplay(2, (((unsigned long) dispSwitch[3] * 100) + dispSwitch[4]) * 100 + dispSwitch[5], true, 6);
    RPU_SetDisplay(3, (((unsigned long) dispSwitch[6] * 100) + dispSwitch[7]) * 100 + dispSwitch[8], true, 6);
  }
  return false;
}


// #################### Scroll latest nine sounds through player 2, 3, and 4 displays ####################
void DebugSound(byte sound) { 

  if (DEBUG_MODE == 3) {
    if (RPU_ReadSingleSwitchState(SW_CREDIT_BUTTON)) return false; // hold current values if game button being held.
    for (i = 0; i < 8; ++i) dispSwitch[i] = dispSwitch[i+1];
    dispSwitch[8] = sound;
    RPU_SetDisplay(1, (((unsigned long) dispSwitch[0] * 100) + dispSwitch[1]) * 100 + dispSwitch[2], true, 6);
    RPU_SetDisplay(2, (((unsigned long) dispSwitch[3] * 100) + dispSwitch[4]) * 100 + dispSwitch[5], true, 6);
    RPU_SetDisplay(3, (((unsigned long) dispSwitch[6] * 100) + dispSwitch[7]) * 100 + dispSwitch[8], true, 6);
  }
}



// #################### LoopCount ####################
void LoopCount() { // count number of loops performed per second

  // Results: Up to 1200 during low activity events, like match or bonus score. 
  //          Up to 250 during game play, but usually in the range 180 - 200.

  if (DEBUG_MODE != 2) return;
  LCount += 1;
  if (CurrentTime - LoopTime > 999) {
    LoopTime = CurrentTime;
    RPU_SetDisplay(3, LCount, true, 2);
    LCount = 0;
  }
}



// #################### Double Hit Fix ####################
boolean DoubleHitFix(byte switchnum) { // Eliminate double hits, when current time between start and start + wait
  if (CurrentTime - SwitchDebounce[switchnum].start < SwitchDebounce[switchnum].wait) 
    return true;
  SwitchDebounce[switchnum].start = CurrentTime;
  return false;
}



// #################### Reset Hit Fix ####################
boolean ResetHitFix(byte switchnum) { // Eliminate switch hits caused by solenoid vibrations, when current time between start and start + wait

switch (switchnum) {
  case SW_DTARGET_4_D:
  case SW_DTARGET_4_C:
  case SW_DTARGET_4_B:
  case SW_DTARGET_4_A:
  case SW_TARGET_WOOD_BEAST:
    if (CurrentTime < ResetHits[SO_DTARGET_4_RESET].start) return false;
    if (CurrentTime - ResetHits[SO_DTARGET_4_RESET].start < ResetHits[SO_DTARGET_4_RESET].wait) return true;
    return false;

  case SW_DTARGET_3_C:
  case SW_DTARGET_3_B:
  case SW_DTARGET_3_A:
    if (CurrentTime < ResetHits[SO_DTARGET_3_RESET].start) return false;
    if (CurrentTime - ResetHits[SO_DTARGET_3_RESET].start < ResetHits[SO_DTARGET_3_RESET].wait) return true;
    return false;

  case SW_DTARGET_INLINE_1ST:
  case SW_DTARGET_INLINE_2ND:
  case SW_DTARGET_INLINE_3RD:
    if (CurrentTime < ResetHits[SO_DTARGET_INLINE_RESET].start) return false;
    if (CurrentTime - ResetHits[SO_DTARGET_INLINE_RESET].start < ResetHits[SO_DTARGET_INLINE_RESET].wait) return true;
    return false;

  case SW_DTARGET_1:
    if (CurrentTime < ResetHits[SO_DTARGET_1_RESET].start) return false;
    if (CurrentTime - ResetHits[SO_DTARGET_1_RESET].start < ResetHits[SO_DTARGET_1_RESET].wait) return true;
    return false;
}

return false;
}



// #################### My_PushToTimedSolenoidStack ####################
boolean My_PushToTimedSolenoidStack(byte solenoidNumber, byte numPushes, unsigned long whenToFire, boolean disableOverride = false) {
  // Call RPU version, after saving solenoid firing time in ResetHits[].start

  ResetHits[solenoidNumber].start = whenToFire;
  return RPU_PushToTimedSolenoidStack(solenoidNumber, numPushes, whenToFire, disableOverride);
}



// #################### My_PushToSolenoidStack ####################
void My_PushToSolenoidStack(byte solenoidNumber, byte numPushes, boolean disableOverride = false) {
  // Call RPU version, after saving solenoid firing time in ResetHits[].start

  ResetHits[solenoidNumber].start = CurrentTime;
  RPU_PushToSolenoidStack(solenoidNumber, numPushes, disableOverride);
}



// #################### INIT NEW BALL ####################
int InitNewBall(bool curStateChanged, int ballNum) {

  if (curStateChanged) {
    InitNewBallAnimation = CurrentTime+2058;

    FirstBall = (ballNum == 1 && !PlayerShootsAgain);

    RPU_ClearSwitches(); // Added to clear solenoids and switches, ensure no carryover from previous game/player/ball. Added to BallySternOS.h as well! Line 103.
    RPU_TurnOffAllLamps();
    if (Credits > 0) RPU_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    RPU_SetDisableFlippers(false);
    RPU_EnableSolenoidStack();

    My_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime + 500);
    My_PushToTimedSolenoidStack(SO_DTARGET_3_RESET, 15, CurrentTime + 500);
    My_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime + 500);
    My_PushToTimedSolenoidStack(SO_DTARGET_1_DOWN, 15, CurrentTime + 500);

    BallDrained = false;

    SkillShotHits = 0;
    SkillShotState = 0;

    PFValidated = false;

    InlaneLeftBlip = 0;
    InlaneRightBlip = 0;
    TargetUpperBlip = 0;
    TargetLowerTopBlip = 0;
    TargetLowerBottomBlip = 0;
    delayTimer = 0;
    CDBSpeed = 100;

    // Upper 3 targets: Reset only if DIP switch not set, or  first ball, or all 3 hit
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

    // Saucer goals: Reset if DIP switch not set, or first ball
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
    LeftSpinnerMissed = false;
    RightSpinnerMissed = false;
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
    MingHealth = MingMax;
 
    // Top Target Special: Reset if DIP switch not set, or first ball
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
    MingDefeated = false;
    InitWizardModeAnimation = 0;

    SaucerLamps = 1;
    SaucerAnimation = 0;
    SaucerHitTime = 0;
    SaucerDirection = 0;

    // 2X/3X saucer arrows: Reset only if DIP switch not set, or first ball
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
    TimerSoundEnd = 0;
    BallInSaucer = 0;

    CountdownMini = 0;
    CountdownSuper = 0;
    Super100kCollected = false;
    Mini50kCollected = false;
    SuperBonusReady = false;

    // 2X/3X/4X/5X: Reset only if DIP switch not set, or first ball
    if (!dipXBonus || FirstBall) {
      BonusXState[CurrentPlayer] = 1;
      CountdownBonusX = 0;
    }
    else CountdownBonusX = BonusXState[CurrentPlayer];

    for (byte count=0; count<CurrentNumPlayers; count++) {
      RPU_SetDisplay(count, CurrentScores[count], true, 2);
    }
    DisplayValue = CurrentScores[CurrentPlayer];
    DisplayTimer = CurrentTime + 100;

    RPU_SetDisplayBallInPlay(ballNum);
    RPU_SetLampState(LA_BALL_IN_PLAY, 1);
    RPU_SetLampState(LA_TILT, 0);
    if (PlayerShootsAgain==true) {
      RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
      RPU_SetLampState(LA_SHOOT_AGAIN, 1);
    }
    InitialScore = CurrentScores[CurrentPlayer];
  }

  MiniBonus = 0;
  SuperBonus = 0;
  NumTiltWarnings = 0;

  if (CurrentTime >= InitNewBallAnimation) {
    My_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
    BallSaveStartTime = CurrentTime + 100;
    InitNewBallAnimation = 0xFFFFFFFF;
  }
  if (!RPU_ReadSingleSwitchState(SW_OUTHOLE)) { // Make sure ball gets out of outhole before starting Skill Shot
    PlaySound(7); // Ring shooting
    BallTimeInTrough = 0;
    return MACHINE_STATE_SKILL_SHOT;
  } else {
    return MACHINE_STATE_INIT_NEW_BALL;
  }
}



// #################### RUN SELF TEST ####################
int RunSelfTest(int curState, boolean curStateChanged) {
  int returnState = curState;
  CurrentNumPlayers = 0;
  PlayQueue();
  // Any state that's greater than CHUTE_3 is handled by the Base Self-test code.
  // Any that's less is machine specific and is handled below.
  if (curState >= MACHINE_STATE_TEST_DONE) {
    returnState = RunBaseSelfTest(returnState, curStateChanged, CurrentTime, SW_CREDIT_BUTTON, SW_COIN_3, SW_SLAM); 
    #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
    if (returnState >= 10000) { // Fudged to play sounds in the main program!
      StopAudio();
      PlaySound(returnState - 10000); // Play sound identified in sound test
      returnState = MACHINE_STATE_TEST_SOUNDS;
    }
    #endif
    // Quick jump to / out of kid mode: Press credit and coin 3 at same time
    if (RPU_ReadSingleSwitchState(SW_CREDIT_BUTTON) && RPU_ReadSingleSwitchState(SW_COIN_3) && (curState != MACHINE_STATE_TEST_STUCK_SWITCHES)) {
      kidMode = !kidMode;
      RPU_WriteByteToEEProm(RPU_TEMP_KID_MODE_OFFON, kidMode);
      #ifndef USE_EXTENDED_SOUNDS
      PlaySound(50); // Ming laugh
      #else
      PlaySound(kidMode ? 38 : 50); // kid Ming laugh : Ming Laugh
      #endif
      returnState = MACHINE_STATE_ATTRACT;
    }   
  }
  else if (curState >= MACHINE_STATE_TEST_DONE - NUM_EXTENDED_SETTINGS) { // Basic self tests are handled above. Self testing can be extended in the following code.
    returnState = RunExtendedSelfTest(returnState, curStateChanged, CurrentTime, SW_CREDIT_BUTTON, SW_COIN_3, SW_SLAM);
    // Quick jump to / out of kid mode: Press credit and coin 3 at same time
    if (RPU_ReadSingleSwitchState(SW_CREDIT_BUTTON) && RPU_ReadSingleSwitchState(SW_COIN_3)) {
      kidMode = !kidMode;
      RPU_WriteByteToEEProm(RPU_TEMP_KID_MODE_OFFON, kidMode);
      #ifndef USE_EXTENDED_SOUNDS
      PlaySound(50); // Ming laugh
      #else
      PlaySound(kidMode ? 38 : 50); // kid Ming laugh : Ming Laugh
      #endif
      returnState = MACHINE_STATE_ATTRACT;
    }
  }
  else {
    returnState = MACHINE_STATE_ATTRACT;
  }
  if (returnState == MACHINE_STATE_ATTRACT){
    // Update all local variables with information updated in self test.
    
    Credits       = RPU_ReadByteFromEEProm(RPU_CREDITS_EEPROM_BYTE);
    TotalPlays    = RPU_ReadULFromEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE);
    TotalReplays  = RPU_ReadULFromEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE);
    ScoreAward1   = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_1_EEPROM_START_BYTE);
    ScoreAward2   = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_2_EEPROM_START_BYTE);
    ScoreAward3   = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_3_EEPROM_START_BYTE);
    HighScore     = RPU_ReadULFromEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE);
    PersonalGoal  = RPU_ReadULFromEEProm(RPU_PERSONAL_GOAL_EEPROM_START_BYTE);
    HighScoreBeat = RPU_ReadULFromEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE);
    CoinsChute1   = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE);
    CoinsChute2   = RPU_ReadULFromEEProm(RPU_CHUTE_2_COINS_START_BYTE);
    CoinsChute3   = RPU_ReadULFromEEProm(RPU_CHUTE_3_COINS_START_BYTE);
    dipBank[0]    = RPU_ReadByteFromEEProm(RPU_DIP_BANK);
    dipBank[1]    = RPU_ReadByteFromEEProm(RPU_DIP_BANK + 1);
    dipBank[2]    = RPU_ReadByteFromEEProm(RPU_DIP_BANK + 2);
    dipBank[3]    = RPU_ReadByteFromEEProm(RPU_DIP_BANK + 3);
    SetupDIPVars();
    
    TotalSkill1          = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE);
    TotalSkill2          = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_2_EEPROM_BYTE);
    TotalSkill3          = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_3_EEPROM_BYTE);
    TotalWizardModes     = RPU_ReadULFromEEProm(RPU_TOTAL_WIZ_EEPROM_BYTE);
    TotalWizardModesBeat = RPU_ReadULFromEEProm(RPU_TOTAL_WIZ_BEAT_EEPROM_BYTE);
    PlayfieldAward       = RPU_ReadByteFromEEProm(RPU_PLAYFIELD_AWARD) & 3;
    ThresholdAward       = RPU_ReadByteFromEEProm(RPU_THRESHOLD_AWARD) & 3;
    backgroundOn         = RPU_ReadByteFromEEProm(RPU_BACKGROUND_OFF) & 1;
    HighScoreAward       = RPU_ReadByteFromEEProm(RPU_HIGHSCORE_AWARD) & 3;
    FreePlay             = RPU_ReadByteFromEEProm(RPU_FREEPLAY) & 1;
    PersonalGoalOn       = RPU_ReadByteFromEEProm(RPU_GOAL_OFFON) & 1;
    WizardGoalTargets    = RPU_ReadByteFromEEProm(RPU_WIZARD_GOAL_TARGETS);
    WizardGoalAttack     = RPU_ReadByteFromEEProm(RPU_WIZARD_GOAL_ATTACK);
    if (curState != -99) kidMode = RPU_ReadByteFromEEProm(RPU_TEMP_KID_MODE_OFFON);
    OriginalRules        = RPU_ReadByteFromEEProm(RPU_ORIGINAL_RULES) & 1;
    ballSaves            = RPU_ReadByteFromEEProm(RPU_BALL_SAVES);
    ballSaveScore        = 100 * RPU_ReadByteFromEEProm(RPU_BALL_SAVE_SCORE);
    ballSaveNumMS        = 1000 * RPU_ReadByteFromEEProm(RPU_BALL_SAVE_NUM_SECS);

    #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
    AdjBgGain            = RPU_ReadByteFromEEProm(RPU_BG_GAIN);
    AdjSfxGain           = RPU_ReadByteFromEEProm(RPU_SFX_GAIN);
    AdjVoiceGain         = RPU_ReadByteFromEEProm(RPU_VOICE_GAIN);

    StopAudio();
    SetTrackLoop(AdjustGTO - 20, false);
    SetTrackLoop(AdjustGTO - 43, false);
    CalculateGain();
    #endif

    // Adjustments to variables to ensure they are within max and min requirements
    if (Credits > dipMaxCredits) {
      Credits = dipMaxCredits;
      RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, Credits & 0x000000FF);
    }
    RPU_SetLampState(LA_CREDIT_INDICATOR, Credits > 0);

    if (WizardGoalTargets == 0) {
      WizardGoalTargets =  11;
      RPU_WriteByteToEEProm(RPU_WIZARD_GOAL_TARGETS, WizardGoalTargets);
    }
    if (WizardGoalAttack  == 0) {
        WizardGoalAttack  = 140;
        RPU_WriteByteToEEProm(RPU_WIZARD_GOAL_ATTACK, WizardGoalAttack);
      }
    if (kidMode) 
      KidSettings();
    else {
      MingMax = MAX_HEALTH; // Reset operator game adjustments that are changed in KidSettings()
    }
    SetLastSelfTestChangedTime(CurrentTime); // Ensure self-test switch not hit on entering attract mode from self-test!
    RPU_ClearSwitches();
  }
  return returnState;
}



// #################### TURN OFF ATTRACT LAMPS ####################
void TurnOffAttractLamps() {
  for (int count=0; count<RPU_MAX_LAMPS; count++) {
    if (count==40) {
      count = 42;
    } else if (count==43) {
      count = 44;
    } else if (count==48) {
      count = 52;
    }
    RPU_SetLampState(count, 0, 0, 0);
  }
}



// #################### RUN ATTRACT MODE ####################
int RunAttractMode(int curState, boolean curStateChanged) {

  int returnState = curState;

  // If this is the first time in the attract mode loop
  if (curStateChanged) {
    if (dipAttractVoice) AttractCalloutTimer = CurrentTime - ATTRACT_SPEECH_TIMER + 4000; // Run attract sound first time after four seconds
    RPU_TurnOffAllLamps();
    if (Credits > 0) RPU_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);
    RPU_SetCoinLockout((Credits < dipMaxCredits) ? false:true);
    
    if (!FreePlay) {
      RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
    }
    RPU_SetLampState(LA_GAME_OVER, 1);

    if (dipMatch) {
      RPU_SetLampState(LA_MATCH, 1);
      // RPU_SetDisplayBallInPlay((int)MatchDigit, 1);
    }
    else
      RPU_SetDisplayBallInPlay(0, 1);
    
    AttractStartAnimation = CurrentTime+2500;
    RPU_SetLampState(LA_MING_TOP, 1, 0, 100);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    AttractHeadMode = 255;
    AttractPlayfieldMode = 255;

    #ifndef USE_EXTENDED_SOUNDS
    PlaySound(8); // Spinner noise
    #endif

    if (CurrentScores[0] == 0) CurrentScores[0] = 500000 + 10 * random(-5000, 5000); // random initial score for display

    kidTimer = 0;
    kidSwitch = 0xFF;

    SaucerHitTime = 0;
  }

  PlayQueue();

  if (dipAttractVoice) {
    if (CurrentTime >= AttractCalloutTimer + ATTRACT_SPEECH_TIMER + AttractWait) {
      #ifdef USE_EXTENDED_SOUNDS 
      PlaySound(4); // This way Flash, come on!
      #else
      PlaySound(44); // Emperor Ming awaits
      #endif
      AttractCalloutTimer = CurrentTime;
      AttractWait = 0;
    }
  }

  if (CurrentTime >= AttractStartAnimation) {
    if (((CurrentTime - AttractStartAnimation) / 6500) % 2 == 1) { // Alternate displays between high score and latest player scores (reversed order, player scores first)
      if (AttractHeadMode!=1) { // do this once, at the beginning
        RPU_SetLampState(LA_HIGH_SCORE_TO_DATE, 1);
        if (!FreePlay) RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
        if (!dipMatch) RPU_SetDisplayBallInPlay(0, true);
      }
      AttractHeadMode = 1;
        SetDisplayAttract(HighScore, HighScore, HighScore, HighScore, CurrentTime - AttractStartAnimation);
    } else {
      if (AttractHeadMode!=2) { // do this once, at the beginning
        RPU_SetLampState(LA_HIGH_SCORE_TO_DATE, 0);
        if (!FreePlay) RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
        if (!dipMatch) RPU_SetDisplayBallInPlay(0, true);
      }
      AttractHeadMode = 2;
        if (CurrentScores[0] == 0)
          SetDisplayAttract(HighScore, HighScore, HighScore, HighScore, CurrentTime - AttractStartAnimation);
        else
          SetDisplayAttract(CurrentScores[0], CurrentScores[1], CurrentScores[2], CurrentScores[3], CurrentTime - AttractStartAnimation);
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
          RPU_SetLampState(AttractLampsDown[lightcountdown].lightNumDown, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsDown[lightcountdown].lightNumDown):0);
        }
        for (byte lightcountup=0; lightcountup<NUM_OF_ATTRACT_LAMPS_UP; lightcountup++) {
          byte dist = AttractSweepLights - AttractLampsUp[lightcountup].rowUp;
          RPU_SetLampState(AttractLampsUp[lightcountup].lightNumUp, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsUp[lightcountup].lightNumUp):0);
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
          RPU_SetLampState(AttractLampsSiren[lightcountsiren].lightNumSiren, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsSiren[lightcountsiren].lightNumSiren):0);
        }
        AttractSirenLights += 1;
        if (AttractSirenLights>33) AttractSirenLights = 0; // 49
      }
      BackglassLampsKnightRider();
      AttractPlayfieldMode = 4;
    }

    if ((CurrentTime - kidTimer > 200) && (kidSwitch != 0xFF)){                                  // Check if credit button and coin 3 button pressed at same time
      if (RPU_ReadSingleSwitchState(SW_CREDIT_BUTTON) && RPU_ReadSingleSwitchState(SW_COIN_3)) { // if yes, change kid mode, add credit, add player
        kidMode = !kidMode;
        if (!kidMode)
          RunSelfTest(-99, 0); // Get non-kid settings from memory
        else
          KidSettings();
        if (!FreePlay) {
          AddCredit(2, CreditsperCoin[2], CoinsperCredit[2]);
          RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
          CoinsChute3 += 1;
          RPU_WriteULToEEProm(RPU_CHUTE_3_COINS_START_BYTE, CoinsChute3);
        } else {
          #ifdef USE_EXTENDED_SOUNDS
          if (kidMode)
            PlaySound(38); // Kid Ming laugh!
          else
            PlaySound(50); // Ming laugh!
          #else
          PlaySound(50); // Ming laugh!
          #endif
        }
        if (AddPlayer(true)) returnState = MACHINE_STATE_INIT_GAMEPLAY;
      }
      else if (kidSwitch == SW_CREDIT_BUTTON) {                                                  // if just credit button, just add player
        if (AddPlayer(true)) returnState = MACHINE_STATE_INIT_GAMEPLAY;
      }
      else if (kidSwitch == SW_COIN_3) {                                                         // if just coin 3, just add credit
        if (!FreePlay) {
          AddCredit(2, CreditsperCoin[2], CoinsperCredit[2]);
          RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
          CoinsChute3 += 1;
          RPU_WriteULToEEProm(RPU_CHUTE_3_COINS_START_BYTE, CoinsChute3);
        }
      }
      kidTimer = 0;
      kidSwitch = 0xFF;
    }

    while ( (switchHit=RPU_PullFirstFromSwitchStack())!=SWITCH_STACK_EMPTY ) {
      if (switchHit==SW_CREDIT_BUTTON) {
        kidTimer = CurrentTime;
        kidSwitch = SW_CREDIT_BUTTON;
      }

      if (switchHit==SW_COIN_1) {
        if (!FreePlay) {
          AddCredit(0, CreditsperCoin[0], CoinsperCredit[0]);
          RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
          CoinsChute1 += 1;
          RPU_WriteULToEEProm(RPU_CHUTE_1_COINS_START_BYTE, CoinsChute1);
        }
      }
      if (switchHit==SW_COIN_2) {
        if (!FreePlay) {
          AddCredit(1, CreditsperCoin[1], CoinsperCredit[1]);
          RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
          CoinsChute2 += 1;
          RPU_WriteULToEEProm(RPU_CHUTE_2_COINS_START_BYTE, CoinsChute2);
        }
      }
      if (switchHit==SW_COIN_3) {
        kidTimer = CurrentTime;
        kidSwitch = SW_COIN_3;
      }
      if (switchHit==SW_SELF_TEST_SWITCH && (CurrentTime-GetLastSelfTestChangedTime())>5000) { // Needs l o n g delay for some reason
        returnState = MACHINE_STATE_TEST_LAMPS;
        SetLastSelfTestChangedTime(CurrentTime);
      }
    }
    if (RPU_ReadSingleSwitchState(SW_SAUCER) && CurrentTime > SaucerHitTime + 750) { // If ball sitting in saucer, kick it out
      My_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime + 500, true);
      SaucerHitTime = CurrentTime;
    }
  }

  return returnState;
}



// #################### ATTRACT DISPLAY ####################
void SetDisplayAttract(unsigned long score1, unsigned long score2, unsigned long score3, unsigned long score4, unsigned long time) {
	unsigned long scores[4] = {score1, score2, score3, score4};
	unsigned long divisor[8] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};

	byte Quarter = (time % 6500) / 125;
  if (Quarter == HoldQuarter) return; // not quarters anymore. Eighths. Oh well...
  HoldQuarter = Quarter;
	for (byte i = 0; i < 4; ++i) {
		if (scores[i] == 0) {
			RPU_SetDisplay(i, 0);
			RPU_SetDisplayBlank(i, 0);
		} else {
			if (Quarter < 8) RPU_SetDisplay(i, scores[i] / divisor[Quarter], true, 0); // scroll left
      else
      if (Quarter >=40 && Quarter < 48) RPU_SetDisplay(i, scores[i] / divisor[47 - Quarter], true, 0); // scroll right
		else
			if (Quarter == 16 || Quarter == 17 || Quarter == 20 || Quarter == 21 || Quarter == 24 || Quarter == 25 || Quarter == 28 || Quarter == 29 || Quarter >= 48) { // Flash off
        RPU_SetDisplay(i, 0);
				RPU_SetDisplayBlank(i, 0);
      }
			else
				RPU_SetDisplay(i, scores[i], true, 1);
		}
	}
	return;
}



// #################### DISPLAY COUNTS UP TO CURRENT SCORE ####################
void CountingDisplay(int displayNumber, unsigned long value, boolean blankByMagnitude, byte minDigits, boolean alwaysShow = false) {
  
  if (displayNumber != CurrentPlayer) {
    RPU_SetDisplay(displayNumber, value, blankByMagnitude, minDigits);
    return;
  }
  if (DisplayValue == value && !alwaysShow) return;
  if (CurrentTime < DisplayTimer) return;

  if (DisplayValue + 100000 < value)
    RPU_SetDisplay(displayNumber, DisplayValue += 10000, blankByMagnitude, minDigits); // Count up by 10,000s
  else
    if (DisplayValue + 1000 < value)
      RPU_SetDisplay(displayNumber, DisplayValue += 1000, blankByMagnitude, minDigits); // Count up by 1,000s
  else
      RPU_SetDisplay(displayNumber, DisplayValue = value, blankByMagnitude, minDigits); // Display is caught up
      
  DisplayTimer = CurrentTime + 100;
}



// #################### FLASHING DISPLAY COUNTS UP TO CURRENT SCORE ####################
void CountingFlashDisplay(int displayNumber, unsigned long value, unsigned long curTime, int period, byte minDigits) {
  // A period of zero toggles display every other time
  if (period) {
    if ((curTime/period)%2) {
      CountingDisplay(displayNumber, value, true, minDigits, true);
    } else {
      RPU_SetDisplayBlank(displayNumber, 0);
    }
  }
}



// #################### NORMAL GAMEPLAY ####################
int NormalGamePlay(boolean curStateChanged) {
  int returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
  if (PFValidated) 
    RPU_SetDisplayCredits(DropTargetCount, true);
  else 
    RPU_SetDisplayFlashCredits(CurrentTime, 250);

  if (curStateChanged) {
    SuperBonusReady = false;

    // Reset rollover buttons, other lights, after skill shot
    RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
    RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
    RPU_SetLampState(LA_STAR_SHOOTER_TOP, 0);
    RPU_SetLampState(LA_STAR_PFIELD_TOP, 0);
    RPU_SetLampState(LA_STAR_PFIELD_BOTTOM, 0);
    RPU_SetLampState(LA_MING_TOP, 1);
    RPU_SetLampState(LA_MING_BOTTOM, 1);
    SpiralPattern(0); // turn off skill shot spiral pattern
    
    if (PFValidated==true) {
      My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
    }

    // Reset lights to be saved to next ball (according to DIP switches), after skill shot

    // 10k 20k 30k saucer lights
    if (DTarget4Goal[CurrentPlayer])      RPU_SetLampState(LA_SAUCER_10K, 1);
    if (DTargetInlineGoal[CurrentPlayer]) RPU_SetLampState(LA_SAUCER_20K, 1);
    if (DTarget3Goal[CurrentPlayer])      RPU_SetLampState(LA_SAUCER_30K, 1);

    // 2X/3X saucer arrows
    if (Playfield2xState[CurrentPlayer] == 1) RPU_SetLampState(LA_SAUCER_ARROW_2X, 1, 0, 150);
    if (Playfield3xState[CurrentPlayer] == 1) RPU_SetLampState(LA_SAUCER_ARROW_3X, 1, 0, 150);

    // 2X/3X/4X/5X Bonus
    if (dipXBonus) ShowExtraBonusLights();

    // Top Target Mini Bonus and Special
    if (TopSpecialLit[CurrentPlayer])  RPU_SetLampState(LA_TARGET_UPPER_SPECIAL, 1);
    if (MiniBonusReady[CurrentPlayer]) RPU_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 1);

    // Right Side and Inner Lane Flipper Targets
    if (DTarget4Light[CurrentPlayer][3] <= 1) RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 0);
    if (DTarget4Light[CurrentPlayer][2] <= 1) RPU_SetLampState(LA_INLANE_RIGHT, 0);
    if (DTarget4Light[CurrentPlayer][1] <= 1) RPU_SetLampState(LA_INLANE_LEFT, 0);
    if (DTarget4Light[CurrentPlayer][0] <= 1) RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 0);

    if (DTarget4Light[CurrentPlayer][3] == 2) RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1, 0, 250);
    if (DTarget4Light[CurrentPlayer][2] == 2) RPU_SetLampState(LA_INLANE_RIGHT, 1, 0, 250);
    if (DTarget4Light[CurrentPlayer][1] == 2) RPU_SetLampState(LA_INLANE_LEFT, 1, 0, 250);
    if (DTarget4Light[CurrentPlayer][0] == 2) RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 1, 0, 250);

    if (DTarget4Light[CurrentPlayer][3] == 3) RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1);
    if (DTarget4Light[CurrentPlayer][2] == 3) RPU_SetLampState(LA_INLANE_RIGHT, 1);
    if (DTarget4Light[CurrentPlayer][1] == 3) RPU_SetLampState(LA_INLANE_LEFT, 1);
    if (DTarget4Light[CurrentPlayer][0] == 3) RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 1);

    // Outlanes
    if (OutLanesLit[CurrentPlayer]) RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 1);
    if (OutLanesLit[CurrentPlayer]) RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 1);

    DeleteQueue();
    PlaySound(90); // stop all sounds
    if (backgroundOn) PlaySound(6, 100); // Background music

    ShowMiniBonusOnLadder(MiniBonus);
    ShowSuperBonusOnLadder(SuperBonus);
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
          RPU_SetLampState(DTarget4ToHandle, 1, 0, 250);
          DTarget4Light[CurrentPlayer][i] = 1;
        } else {
          if (DTarget4Light[CurrentPlayer][0]>=2 && DTarget4Light[CurrentPlayer][2]>=2 && (DTarget4Light[CurrentPlayer][1]<=1 || DTarget4Light[CurrentPlayer][3]<=1)) {
            if (DTarget4Light[CurrentPlayer][1]<=1 && DTarget4Light[CurrentPlayer][3]<=1) {
              RPU_SetLampState(LA_DTARGET_4_B, 1, 0, 250);
              RPU_SetLampState(LA_DTARGET_4_D, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][1]<=1 && DTarget4Light[CurrentPlayer][3]>=2) {
              RPU_SetLampState(LA_DTARGET_4_B, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][1]>=2 && DTarget4Light[CurrentPlayer][3]<=1) {
              RPU_SetLampState(LA_DTARGET_4_D, 1, 0, 250);
            }
          } else {
            RPU_SetLampState(DTarget4ToHandle, 0);
            DTarget4Light[CurrentPlayer][i] = 0;
          }
        }
      } else {
        if (i==1 || i==3) {
          RPU_SetLampState(DTarget4ToHandle, 1, 0, 250);
          DTarget4Light[CurrentPlayer][i] = 1;
        } else {
          if (DTarget4Light[CurrentPlayer][1]>=2 && DTarget4Light[CurrentPlayer][3]>=2 && (DTarget4Light[CurrentPlayer][0]<=1 || DTarget4Light[CurrentPlayer][2]<=1)) {
            if (DTarget4Light[CurrentPlayer][0]<=1 && DTarget4Light[CurrentPlayer][2]<=1) {
              RPU_SetLampState(LA_DTARGET_4_A, 1, 0, 250);
              RPU_SetLampState(LA_DTARGET_4_C, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][0]<=1 && DTarget4Light[CurrentPlayer][2]>=2) {
              RPU_SetLampState(LA_DTARGET_4_A, 1, 0, 250);
            } else if (DTarget4Light[CurrentPlayer][0]>=2 && DTarget4Light[CurrentPlayer][2]<=1) {
              RPU_SetLampState(LA_DTARGET_4_C, 1, 0, 250);
            }
          } else {
            RPU_SetLampState(DTarget4ToHandle, 0);
            DTarget4Light[CurrentPlayer][i] = 0;
          }
        }
      }
    } else if (DTarget4Light[CurrentPlayer][i]>=2) RPU_SetLampState(DTarget4ToHandle, 1);
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
        RPU_SetLampState(DTarget3ToLight, 1);
      } else {
        RPU_SetLampState(DTarget3ToLight, lamp3Phase==lamp3PhaseIncrement, 0, 200);
        if (lamp3Phase==lamp3PhaseIncrement) {
          DTarget3Flashing[CurrentPlayer] = DTarget3ToLight;
        }
        lamp3PhaseIncrement++;
      }
    }
  }


  // handle super bonus collect lit animation
  if (SuperBonusReady!=true) {
    RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
    RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
    RPU_SetLampState(LA_STAR_SHOOTER_TOP, 0);
    RPU_SetLampState(LA_STAR_PFIELD_TOP, 0);
    RPU_SetLampState(LA_STAR_PFIELD_BOTTOM, 0);
  } else {
    byte lamp6Phase = (CurrentTime/125)%6;
    RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, lamp6Phase==4);
    RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, lamp6Phase==3||lamp6Phase==4, lamp6Phase==4);
    RPU_SetLampState(LA_STAR_SHOOTER_TOP, lamp6Phase==2||lamp6Phase==3, lamp6Phase==3);
    RPU_SetLampState(LA_STAR_PFIELD_TOP, lamp6Phase==1||lamp6Phase==2, lamp6Phase==2);
    RPU_SetLampState(LA_STAR_PFIELD_BOTTOM, lamp6Phase<2, lamp6Phase==1);
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
      if (!OriginalRules) { // Original rules zeroed mini bonus after collecting from upper-level target
        MiniBonus = CountdownMini;
        ShowMiniBonusOnLadder(MiniBonus);
      }
    }
    LastCountdownReportTime = CurrentTime;
  }

  // handle super bonus ring collect animation
  if (((CurrentTime-LastCountdownReportTime)>25) && (SuperBonusCollecting==true)) { // adjust speed 300
    if (SuperBonus>0) {
      CurrentScores[CurrentPlayer] += 1000;
      SuperBonus -= 1;
      ShowSuperBonusOnLadder(SuperBonus);
      PlaySound(12); // Other ascending sound
      if (BonusXState[CurrentPlayer]>1 && SuperBonus==0) {
        SuperBonus = CountdownSuper;
        BonusXState[CurrentPlayer] -= 1;
      }
    } else {
      SuperBonusCollecting = false;
      SuperBonusReady = false;
      BonusXState[CurrentPlayer] = CountdownBonusX;
      if (!OriginalRules) { // Original rules zeroed super bonus after collecting in shooter lane
        SuperBonus = CountdownSuper;
        ShowSuperBonusOnLadder(SuperBonus);
      }
      PFValidated = false;
    }
    LastCountdownReportTime = CurrentTime;
  }

  // Delay for "Savior of the Universe" sound before going into countdown
  if (delayTimer != 0 && CurrentTime >= delayTimer) {
    delayTimer = 0;
    returnState = MACHINE_STATE_COUNTDOWN_BONUS;
  }

  // LAMP BLIP
  if (CurrentTime>=InlaneLeftBlip && (DTarget4Light[CurrentPlayer][1]==0 || DTarget4Light[CurrentPlayer][1]==1)) {
    RPU_SetLampState(LA_INLANE_LEFT, 0);
  }
  if (CurrentTime>=InlaneRightBlip && (DTarget4Light[CurrentPlayer][2]==0 || DTarget4Light[CurrentPlayer][2]==1)) {
    RPU_SetLampState(LA_INLANE_RIGHT, 0);
  }
  if (CurrentTime>=TargetUpperBlip && TopSpecialLit[CurrentPlayer]==false && MiniBonusReady[CurrentPlayer]==false) {
    RPU_SetLampState(LA_TARGET_UPPER_SPECIAL, 0);
    RPU_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 0);
  }
  if (CurrentTime>=TargetLowerTopBlip && (DTarget4Light[CurrentPlayer][0]==0 || DTarget4Light[CurrentPlayer][0]==1)) {
    RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 0);
  }
  if (CurrentTime>=TargetLowerBottomBlip && (DTarget4Light[CurrentPlayer][3]==0 || DTarget4Light[CurrentPlayer][3]==1)) {
    RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 0);
  }

  // PLAYFIELD X STOP
  if ((Playfield2xState[CurrentPlayer]==2 && CurrentTime>=Playfield2XStop) || (Playfield3xState[CurrentPlayer]==2 && CurrentTime>=Playfield3XStop)) {
    if (Playfield2xState[CurrentPlayer]==2 && CurrentTime>=Playfield2XStop) {
      if (OriginalRules) // X2 bonus only allowed once per ball
        Playfield2xState[CurrentPlayer]  = 3; // Done for current ball
      else {
        Playfield2xState[CurrentPlayer]  = 0; // Set up to go again
        DTarget3Lights[CurrentPlayer][0] = 0;
        DTarget3Lights[CurrentPlayer][1] = 0;
        DTarget3Lights[CurrentPlayer][2] = 0;
      }
      RPU_SetLampState(LA_CLOCK_15_SECONDS_2X, 0);
    }
    if (Playfield3xState[CurrentPlayer]==2 && CurrentTime>=Playfield3XStop) {
      if (OriginalRules) // X3 bonus only allowed once per ball
        Playfield3xState[CurrentPlayer] = 3; // Done for current ball
      else {
        Playfield3xState[CurrentPlayer] = 0; // Set up to go again
        DTarget4Light[CurrentPlayer][0] = 0;
        DTarget4Light[CurrentPlayer][1] = 0;
        DTarget4Light[CurrentPlayer][2] = 0;
        DTarget4Light[CurrentPlayer][3] = 0;
      }
      RPU_SetLampState(LA_CLOCK_15_SECONDS_3X, 0);
    }
    #ifndef USE_EXTENDED_SOUNDS
    PlaySound(11); // background sound 2 (restart)
    #else
    // DeleteQueue(); // moved to "End Alarm Sound" below, where it runs 260 ms earlier, BEFORE football hit on head!
    PlaySound(90); // Football alarm ending
    if (backgroundOn) PlaySound(6, 10); // restart background music
    #endif
  }

  // End Alarm Sound
  if (CurrentTime >= TimerSoundEnd && TimerSoundEnd != 0) {
    TimerSoundEnd = 0;
    DeleteQueue();
    #ifdef USE_EXTENDED_SOUNDS
    PlaySound(88); // Football hit on head
    #endif
  }

  // TARGET HURRY UPS, NOT HIT IN TIME
  if (DTarget4Light[CurrentPlayer][0]==2 && CurrentTime>=WhiteHurryUpStop) {
    PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
    RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 1, 0, 250);
    WhiteHurryUpStop = 0xFFFFFFFF;
  }
  if (DTarget4Light[CurrentPlayer][3]==2 && CurrentTime>=AmberHurryUpStop) {
    PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
    RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1, 0, 250);
    AmberHurryUpStop = 0xFFFFFFFF;
  }


  // SPINNER HURRY UPS
  if (LeftSpinnerHurryUpLit==true) {
    if (CurrentTime>=LeftSpinnerHurryUpTimer) {
      if (LeftSpinnerMissed) PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
      if (LeftSpinnerLit==true) {
        RPU_SetLampState(LA_SPINNER_LEFT, 1);
      } else {
        RPU_SetLampState(LA_SPINNER_LEFT, 0);
      }
      LeftSpinnerHurryUpLit = false;
      LeftSpinnerMissed = false;
    }
  }

  if (RightSpinnerHurryUpLit==true) {
    if (CurrentTime>=RightSpinnerHurryUpTimer) {
      if (RightSpinnerMissed) PlaySound(48); // S&T: Pathetic Earthling. Geeteoh: Oh, dear
      if (RightSpinnerLit==true) {
        RPU_SetLampState(LA_SPINNER_RIGHT, 1);
      } else {
        RPU_SetLampState(LA_SPINNER_RIGHT, 0);
      }
      RightSpinnerHurryUpLit = false;
      RightSpinnerMissed = false;
    }
  }


  // HANDLE SAUCER
  if (BallInSaucer==true) {
    if (CurrentTime<=SaucerAnimation) {
      backglassLampsCenterOut();
      if ((CurrentTime-AttractSweepTime)>25) { // 50 (15 count right now)
        AttractSweepTime = CurrentTime;
        if (SaucerDirection == 0) {
          for (byte lightcountdown=0; lightcountdown<NUM_OF_SAUCER_LAMPS_DOWN; lightcountdown++) {
            byte dist = SaucerLamps - SaucerLampsDown[lightcountdown].rowSaucerDown;
            RPU_SetLampState(SaucerLampsDown[lightcountdown].lightNumSaucerDown, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+SaucerLampsDown[lightcountdown].lightNumSaucerDown):0);
          }
        } else {
          for (byte lightcountup=0; lightcountup<NUM_OF_SAUCER_LAMPS_UP; lightcountup++) {
            byte dist = SaucerLamps - SaucerLampsUp[lightcountup].rowSaucerUp;
            RPU_SetLampState(SaucerLampsUp[lightcountup].lightNumSaucerUp, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+SaucerLampsUp[lightcountup].lightNumSaucerUp):0);
          }
        }
        SaucerLamps += 1;
        if (SaucerLamps>30) SaucerLamps = 0;
      }
    } else {
      if (Playfield2xState[CurrentPlayer] == 1 || Playfield3xState[CurrentPlayer] == 1) {
        #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
        PlaySound(90); // stop background
        #endif
        PlaySound(10); // 15 second football alarm
        // TimerSoundEnd = CurrentTime + PLAYFIELD_X_TIMER - 260; // sound ends slightly early, so there is no gap when alarm ends
        // NOTE: above doesn't work for some reason. Replaced PLAYFIELD_X_TIMER - 260 with 14740 below.
        TimerSoundEnd = CurrentTime + 14740; // sound ends slightly early, so there is no gap when alarm ends
        
        if (Playfield2xState[CurrentPlayer]==1) {
          Playfield2xState[CurrentPlayer] = 2;
          Playfield2XStop = CurrentTime + PLAYFIELD_X_TIMER;
          RPU_SetLampState(LA_SAUCER_ARROW_2X, 0);
          RPU_SetLampState(LA_CLOCK_15_SECONDS_2X, 1, 0, 100);
        }
        if (Playfield3xState[CurrentPlayer]==1) {
          Playfield3xState[CurrentPlayer] = 2;
          Playfield3XStop = CurrentTime + PLAYFIELD_X_TIMER;
          RPU_SetLampState(LA_SAUCER_ARROW_3X, 0);
          RPU_SetLampState(LA_CLOCK_15_SECONDS_3X, 1, 0, 100);
        }
      }
      if (SaucerDirection == 0) {
        My_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime);
        SaucerDirection = 1;
      } else {
        My_PushToTimedSolenoidStack(SO_SAUCER_UP, 5, CurrentTime);
        SaucerDirection = 0;
      }
      SaucerLamps = 1;
      BallInSaucer = false;
      for (byte off=LA_BONUS_MINI_1K; off<=LA_BONUS_MINI_10K; off++) RPU_SetLampState(off, 0);
      for (byte off=LA_BONUS_SUPER_1K; off<=LA_BONUS_5X; off++) RPU_SetLampState(off, 0);
      for (byte off=LA_SAUCER_10K; off<=LA_SAUCER_XBALL; off++) RPU_SetLampState(off, 0);
      RPU_SetLampState(LA_SAUCER_30K, 0);
      ShowMiniBonusOnLadder(MiniBonus);
      ShowSuperBonusOnLadder(SuperBonus);
      ShowExtraBonusLights();
    }
  }

  CountingDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true, 2);

  // Check to see if ball is in the outhole
  if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
    if (BallTimeInTrough==0) {
      BallTimeInTrough = CurrentTime;
      DeleteQueue();
      PlaySound(90); // Stop sound
    } else {
      // Make sure the ball stays on the sensor for at least 
      // 0.5 seconds to be sure that it's not bouncing
      if ((CurrentTime-BallTimeInTrough)>500) {
        BallDrained = true;
        if (NumTiltWarnings>=MAX_TILT_WARNINGS) {
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
    RPU_SetCoinLockout((Credits < dipMaxCredits) ? false:true);
    RPU_SetDisableFlippers(true);
    RPU_DisableSolenoidStack();
    RPU_TurnOffAllLamps();
    if (Credits > 0) RPU_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    RPU_SetDisplayBallInPlay(0);
    InitGamePlayAnimation = CurrentTime+3240;
    RPU_EnableSolenoidStack();
    My_PushToSolenoidStack(SO_SAUCER_DOWN, 5, true);

    // Set up general game variables
    CurrentNumPlayers = 1;
    CurrentPlayer = 0;
    CurrentBallInPlay = 1;
    PlayerShootsAgain = false;

    CurrentSoundEffectTimer = 0;
    CurrentSound = 0;
    AttractWait = 0;

    MingWasDefeated = false;

    for (byte i = 0; i < 4; i++) {
         ScoreAwardStates[i] = 0;
        FreeGameCollected[i] = 0;
        FreeBallCollected[i] = 0;
              OutLanesLit[i] = 0;
        OutLanesCollected[i] = 0;
              WizardState[i] = 0;
            CurrentScores[i] = 0;
             BallSaveUsed[i] = ballSaves;
            RPU_SetDisplay(i, 0);
       RPU_SetDisplayBlank(i, 0);
    }
  }
  RPU_SetDisplayBlank(0, 0x60); // First player display on

  BackglassLampsLeft2Right();

  if (CurrentTime>=InitGamePlayAnimation) {
    RPU_EnableSolenoidStack();
    RPU_SetDisableFlippers(false);
    returnState = MACHINE_STATE_INIT_NEW_BALL;
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
      DeleteQueue(); // Clear out any remaining queued sounds before next ball

      // if we have remaining ball saves, and we're under the time limit, then save the ball
      if ((BallSaveUsed[CurrentPlayer] > 0) && (CurrentTime < (BallSaveStartTime + ballSaveNumMS))) {
        BallSaveUsed[CurrentPlayer]--;
        RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 0);
        PlayerShootsAgain = true;
        CurrentScores[CurrentPlayer] = InitialScore;

        #ifndef USE_EXTENDED_SOUNDS
        PlaySound(46, 1000); // Try again, Earthling
        #else
        PlaySound(77, 1000); // Don't kill him yet, father
        #endif
      }
        // if we have remaining ball saves, and we're under the score limit, then save the ball
      else if ((BallSaveUsed[CurrentPlayer] > 0) && (CurrentScores[CurrentPlayer] < InitialScore + ballSaveScore)) {
        BallSaveUsed[CurrentPlayer]--;
        RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 0);
        PlayerShootsAgain = true;
        CurrentScores[CurrentPlayer] = InitialScore;
        
        #ifndef USE_EXTENDED_SOUNDS
        PlaySound(46, 1000); // Try again, Earthling
        #else
        PlaySound(77, 1000); // Don't kill him yet, father
        #endif
      }
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
            RPU_SetDisplay(count, CurrentScores[count], true, 2);
          }
          DisplayValue = CurrentScores[CurrentPlayer];
          if (NumTiltWarnings >= MAX_TILT_WARNINGS) {
          DeleteQueue();
          PlaySound(90); // Stop all sounds
          #ifndef USE_EXTENDED_SOUNDS
          PlaySound(41); // Ming laugh five times
          #else
          PlaySound(95, 10); // Oh dear, how pathetic. Only used if tilt on last ball.
          #endif
          }
          returnState = MACHINE_STATE_MATCH_MODE;
        }
        else returnState = MACHINE_STATE_INIT_NEW_BALL;
      }
  } else if (curState==MACHINE_STATE_MATCH_MODE) {
      
      if (curStateChanged) {
        DeleteQueue(); // Clear out any queued sounds
        highestScore = 0; // Take care of high score award
        for (byte count=0; count<CurrentNumPlayers; count++) {
          if (CurrentScores[count]>highestScore) highestScore = CurrentScores[count];
        }
      if (highestScore > HighScore){
        HighScore = highestScore;
        if (HighScoreAward > 0) {
          AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], HighScoreAward);
          for (byte i = 0; i < HighScoreAward; i++) {
            My_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime + 250 * i, true);
          }
        }
        if (!kidMode){ // High score info not saved in kid mode
          TotalReplays += HighScoreAward;
          RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
          HighScoreBeat = HighScoreBeat + 1;
          RPU_WriteByteToEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE, HighScoreBeat);
          RPU_WriteULToEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, HighScore);
        }
        for (byte count=0; count<CurrentNumPlayers; count++) {
          RPU_SetDisplay(count, CurrentScores[count], true, 2);
          }
          DisplayValue = CurrentScores[CurrentPlayer];
        }
      }
    
    if (dipMatch) {
      returnState = ShowMatchSequence(curStateChanged); 
    } else {
      #ifdef USE_EXTENDED_SOUNDS 
      if (PersonalGoalOn) {
        if (MingWasDefeated)
          PlaySound(46); // You've saved your Earth. Have a nice day!
        else
          if (highestScore < PersonalGoal)
            PlaySound(kidMode ? 39 : 95); // Yeah! : Oh dear, how pathetic
          else
            PlaySound(kidMode ? 39 : 94); // Yeah! : Oh well, who wants to live forever?
      }
      else
        PlaySound(kidMode ? 39 : 46); // No personal goal. Use regular end sound.
      #else
        PlaySound(5);  // sound off
        PlaySound(46); // Try again Earthling
      #endif
      // Wait 10 seconds to avoid final sound clobbering first attract sound
      AttractWait = 10000;
      returnState = MACHINE_STATE_ATTRACT;
    }
  }
  
  PlayQueue();   // Handle delayed sounds
  LoopCount();   // Display count of times per second that this code is run
  FlashStrobe(); // Turn strobe light off and on a set number of times

  if (NumTiltWarnings<MAX_TILT_WARNINGS) {
    while ((switchHit=RPU_PullFirstFromSwitchStack()) != SWITCH_STACK_EMPTY) {

      if (DoubleHitFix(switchHit)) break; // ignore if a double hit
      if (ResetHitFix(switchHit)) break; // ignore if switch set off by resetting solenoid
      if (Debugger(switchHit)) break;     // display most recent switches hit, scrolling through displays 2, 3, and 4. Break if game button held.

      switch (switchHit) {
        case SW_SELF_TEST_SWITCH:
          returnState = MACHINE_STATE_TEST_LAMPS;
          SetLastSelfTestChangedTime(CurrentTime);
          break;

        case SW_STARS_PFIELD:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            PlaySound(13); // Ascending sound
            AddToScore(1000);
            AddToMiniBonus(1);
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
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
                RPU_SetLampState(LA_STAR_SHOOTER_TOP, 0);
                RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
                RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
                if (PlayerShootsAgain==true) {
                  PlayerShootsAgain = false;
                  RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 0);
                  RPU_SetLampState(LA_SHOOT_AGAIN, 0);
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

              #ifndef USE_EXTENDED_SOUNDS
              PlaySound(29); // Crash
              #else
              PlaySound(75); // Oh, Flash!
              #endif
              DropTargetHit();
            }
          } else  {
            PlaySound(24); // wizard sound bong bounce down
            if (WizardState[CurrentPlayer]==5) {
              // My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;

        case SW_SHOOTER_LANE_ROLL:
          if (PFValidated==true) {
            My_PushToSolenoidStack(SO_DTARGET_1_DOWN, 5, true);
            PFValidated = false;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (curState==MACHINE_STATE_NORMAL_GAMEPLAY) {
              if (SuperBonusReady!=true) {
                #ifdef USE_EXTENDED_SOUNDS 
                PlaySound(7); // Shooting noise on Geeteoh
                #else
                PlaySound(18); // "Flash!" on S&T. Not used for Geeteoh, as it interferes with "Flash!" in background music.
                #endif
              } else if (WizardState[CurrentPlayer] == 1) { // add shooter lane as a pathway to wizard mode
                  WizardState[CurrentPlayer] = 2;
                  WizSource = 0;
                  DeleteQueue();
                  PlaySound(90); // Stop sounds
                  #ifndef USE_EXTENDED_SOUNDS
                  PlaySound(18); // "Flash!" on S&T. Not used for Geeteoh, as it interferes with "Flash!" in background music.
                  #else
                  PlaySound(51, 10); // Savior of the Universe!
                  #endif
                  delayTimer = max(GTOWaitingTime, CurrentTime) + 100 * SoundData[51][2]; // will delay for sound to play before going into countdown
                  CDBSpeed = 25;
                } else {
                  SuperBonusCollecting = true;
                  CountdownSuper = SuperBonus;
                  CountdownBonusX = BonusXState[CurrentPlayer];
                  LastCountdownReportTime = CurrentTime;
                  #ifdef USE_EXTENDED_SOUNDS 
                  PlaySound(7); // Shooting noise on Geeteoh
                  #else
                  PlaySound(18); // "Flash!" on S&T. Not used for Geeteoh, as it interferes with "Flash!" in background music.
                  #endif
              }
            } else if (curState==MACHINE_STATE_SKILL_SHOT) {
              if (SkillShotState==0) PlaySound(7); // Shooting noise. Was 12, ascending sound.
              if (SkillShotState==1) {
                #ifdef USE_EXTENDED_SOUNDS
                PlaySound(4); // This way Flash, come on! 42 is only meant for 2X bonus, and has been changed.
                #else
                PlaySound(42); // wizard sound: Lucky shot earthling
                #endif
                SkillShotScoreAnimation = CurrentTime+500;
                SkillShotState = 2;
                if (SkillShotHits==1 || SkillShotHits==2) {
                  CurrentScores[CurrentPlayer] += 15000;
                  TotalSkill1++;
                  RPU_WriteULToEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE, TotalSkill1);
                } else if (SkillShotHits==3 || SkillShotHits==4) {
                  CurrentScores[CurrentPlayer] += 30000;
                  TotalSkill2++;
                  RPU_WriteULToEEProm(RPU_TOTAL_SKILL_2_EEPROM_BYTE, TotalSkill2);
                } else if (SkillShotHits==5) {
                  CurrentScores[CurrentPlayer] += 75000;
                  TotalSkill3++;
                  RPU_WriteULToEEProm(RPU_TOTAL_SKILL_3_EEPROM_BYTE, TotalSkill3);
                }
              }
            }
          } else {
            PlaySound(26); // wizard sound, crash bounce up. Electric zap on Geeteoh.
          }
          break;

        case SW_DTARGET_REBOUND:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            PlaySound(7); // Quick sound.
            AddToScore(50);
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;

        case SW_CREDIT_BUTTON:
          #ifdef USE_EXTENDED_SOUNDS
          if (CurrentScores[CurrentPlayer] != 0 && STOP_THE_MUSIC) {
            PlaySound(90);
            backgroundOn = !backgroundOn;
            if (backgroundOn) PlaySound(6, 10);
            break;
          }
          #endif
          if (CurrentBallInPlay < 2) { // Can add player during ball 1, but not if double tap
              PlayerAdded = AddPlayer();
              PlayerAddedTime = CurrentTime;
          }
          break;

        case SW_TILT:
        case SW_SLAM:
          TiltHit();
          break;

        case SW_COIN_1:
          if (!FreePlay) {
            AddCredit(0, CreditsperCoin[0], CoinsperCredit[0]);
            RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
            GetValue = RPU_ReadULFromEEProm(RPU_CHUTE_1_COINS_START_BYTE);
            RPU_WriteULToEEProm(RPU_CHUTE_1_COINS_START_BYTE, (GetValue + 1));
          }
          break;

        case SW_COIN_2:
          if (!FreePlay) {
            AddCredit(1, CreditsperCoin[1], CoinsperCredit[1]);
            RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
            GetValue = RPU_ReadULFromEEProm(RPU_CHUTE_2_COINS_START_BYTE);
            RPU_WriteULToEEProm(RPU_CHUTE_2_COINS_START_BYTE, (GetValue + 1));
          }
          break;

        case SW_COIN_3:
          if (!FreePlay) {
            AddCredit(2, CreditsperCoin[2], CoinsperCredit[2]);
            RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
            GetValue = RPU_ReadULFromEEProm(RPU_CHUTE_3_COINS_START_BYTE);
            RPU_WriteULToEEProm(RPU_CHUTE_3_COINS_START_BYTE, (GetValue + 1));
          }
          break;

        case SW_TARGET_LRIGHT_BOTTOM:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][3]==0 || DTarget4Light[CurrentPlayer][3]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              TargetLowerBottomBlip = CurrentTime + 100;
              RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][3]==2) { // Made hurry-up in time!
              DTarget4Light[CurrentPlayer][3] = 3;
              if (!Bonus3XComplete()) PlaySound(45); // Flash!
              if (AmberHurryUpStop != 0xFFFFFFFF) AddToScore(50000); 
                else AddToScore(10000);
              AmberHurryUpStop = 0xFFFFFFFF;
              RPU_SetLampState(LA_DTARGET_4_D, 1);
              RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1);
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
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][2]==0 || DTarget4Light[CurrentPlayer][2]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              InlaneRightBlip = CurrentTime + 100;
              RPU_SetLampState(LA_INLANE_RIGHT, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][2]==2) {
              DTarget4Light[CurrentPlayer][2] = 3;
              if (!Bonus3XComplete()) PlaySound(27); // Look out, Flash!
              AddToScore(25000);
              RPU_SetLampState(LA_DTARGET_4_C, 1);
              RPU_SetLampState(LA_INLANE_RIGHT, 1);
              LeftSpinnerHurryUpLit = true;
              LeftSpinnerMissed = true;
              LeftSpinnerHurryUpTimer = CurrentTime + 5000;
              RPU_SetLampState(LA_SPINNER_LEFT, 1, 0, 100);
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
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][1]==0 || DTarget4Light[CurrentPlayer][1]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              InlaneLeftBlip = CurrentTime + 100;
              RPU_SetLampState(LA_INLANE_LEFT, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][1]==2) {
              DTarget4Light[CurrentPlayer][1] = 3;
              if (!Bonus3XComplete()) PlaySound(27); // Look out, Flash!
              AddToScore(25000);
              RPU_SetLampState(LA_DTARGET_4_B, 1);
              RPU_SetLampState(LA_INLANE_LEFT, 1);
              RightSpinnerHurryUpLit = true;
              RightSpinnerMissed = true;
              RightSpinnerHurryUpTimer = CurrentTime + 5000;
              RPU_SetLampState(LA_SPINNER_RIGHT, 1, 0, 100);
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
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            // HURRY UP
            if (DTarget4Light[CurrentPlayer][0]==0 || DTarget4Light[CurrentPlayer][0]==1) {
              PlaySound(26); // Electric Zap
              AddToScore(5000);
              TargetLowerTopBlip = CurrentTime + 100;
              RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 1);
            } 
            else if (DTarget4Light[CurrentPlayer][0]==2) {
              DTarget4Light[CurrentPlayer][0] = 3;
              if (!Bonus3XComplete()) PlaySound(45); // Flash!
              if (WhiteHurryUpStop != 0xFFFFFFFF) AddToScore(50000);
                else AddToScore(10000);
              WhiteHurryUpStop = 0xFFFFFFFF;
              RPU_SetLampState(LA_DTARGET_4_A, 1);
              RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 1);
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
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            
            if (switchHit==SW_DTARGET_4_A) {
              if (DTarget4Light[CurrentPlayer][0]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
                AddToSuperBonus(1);
               DropTargetHit();
              } else if (DTarget4Light[CurrentPlayer][0]==1) {
                WhiteHurryUpStop = CurrentTime + DTARGET_4_HURRY_UP_TIMER;
                RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 1, 0, 100);
                DTarget4Light[CurrentPlayer][0] = 2;
                #ifndef USE_EXTENDED_SOUNDS
                PlaySound(47); // 15 seconds
                #else
                PlaySound(17); // Switches over there, start hitting them!
                #endif
                AddToScore(10000);
                AddToSuperBonus(1);
                DropTargetHit();
              } else if ((DTarget4Light[CurrentPlayer][0]==2 || DTarget4Light[CurrentPlayer][0]==3)) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
                AddToSuperBonus(1);
               DropTargetHit();
              }
            }
            if (switchHit==SW_DTARGET_4_B) {
              if (DTarget4Light[CurrentPlayer][1]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
                AddToSuperBonus(1);
               DropTargetHit();
              } else if (DTarget4Light[CurrentPlayer][1]==1) {
                RPU_SetLampState(LA_INLANE_LEFT, 1, 0, 250);
                DTarget4Light[CurrentPlayer][1] = 2;
                #ifndef USE_EXTENDED_SOUNDS
                PlaySound(25); // Bong bounce up
                #else
                PlaySound(27); // Look out, Flash!
                #endif
                AddToScore(10000);
                AddToSuperBonus(1);
              DropTargetHit();
              } else if ((DTarget4Light[CurrentPlayer][1]==2 || DTarget4Light[CurrentPlayer][1]==3)) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
                AddToSuperBonus(1);
                DropTargetHit();
              }
            }
            if (switchHit==SW_DTARGET_4_C) {
              if (DTarget4Light[CurrentPlayer][2]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
                AddToSuperBonus(1);
                DropTargetHit();
              } else if (DTarget4Light[CurrentPlayer][2]==1) {
                RPU_SetLampState(LA_INLANE_RIGHT, 1, 0, 250);
                DTarget4Light[CurrentPlayer][2] = 2;
                #ifndef USE_EXTENDED_SOUNDS
                PlaySound(25); // Bong bounce up
                #else
                PlaySound(27); // Look out, Flash!
                #endif
                AddToScore(10000);
                AddToSuperBonus(1);
                DropTargetHit();
              } else if ((DTarget4Light[CurrentPlayer][2]==2 || DTarget4Light[CurrentPlayer][2]==3)) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
                AddToSuperBonus(1);
                DropTargetHit();
              }
            }
            if (switchHit==SW_DTARGET_4_D) {
              if (DTarget4Light[CurrentPlayer][3]==0) {
                PlaySound(24); // Bong Bounce Down
                AddToScore(5000);
                AddToSuperBonus(1);
                DropTargetHit();
              } else if (DTarget4Light[CurrentPlayer][3]==1) {
                AmberHurryUpStop = CurrentTime + DTARGET_4_HURRY_UP_TIMER;
                RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 1, 0, 100);
                DTarget4Light[CurrentPlayer][3] = 2;
                #ifndef USE_EXTENDED_SOUNDS
                PlaySound(47); // 15 seconds
                #else
                PlaySound(17); // Switches over there, start hitting them!
                #endif
                AddToScore(10000);
                AddToSuperBonus(1);
                DropTargetHit();
              } else if ((DTarget4Light[CurrentPlayer][3]==2 || DTarget4Light[CurrentPlayer][3]==3)) {
                PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
                AddToSuperBonus(1);
                DropTargetHit();
              }
            }
            if (CheckIfDTargets4Down()) {
              AddToScore(15000);
              if (LeftSpinnerLit==false) {
                LeftSpinnerLit = true;
                RPU_SetLampState(LA_SPINNER_LEFT, 1);
              } else {
                RightSpinnerLit = true;
                RPU_SetLampState(LA_SPINNER_RIGHT, 1);
              }
              DTarget4Goal[CurrentPlayer] = true;
              RPU_SetLampState(LA_SAUCER_10K, 1);
              CheckSaucerDTargetGoal();
              if (BonusXState[CurrentPlayer]==4) {
                BonusXState[CurrentPlayer] = 5;
                ShowExtraBonusLights();
              }
              My_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime + 500);
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            // My_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime);
          }
          break;

        case SW_DTARGET_3_A:
        case SW_DTARGET_3_B:
        case SW_DTARGET_3_C:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (switchHit==SW_DTARGET_3_A) {
              if (DTarget3Lights[CurrentPlayer][0]==0 && DTarget3Flashing[CurrentPlayer]==LA_DTARGET_ARROW_1) {
                RPU_SetLampState(LA_DTARGET_ARROW_1, 1);
                DTarget3Lights[CurrentPlayer][0] = 1;
                if (!Bonus2XComplete()) PlaySound(24); // Bong Bounce Down
                AddToScore(10000);
                AddToMiniBonus(1);
                DropTargetHit();
              } else if (DTarget3Lights[CurrentPlayer][0]==1) {
                  PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
                AddToMiniBonus(1);
                DropTargetHit();
              } else {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(5000);
                AddToMiniBonus(1);
                DropTargetHit();
              }
            }
            if (switchHit==SW_DTARGET_3_B) {
              if (DTarget3Lights[CurrentPlayer][1]==0 && DTarget3Flashing[CurrentPlayer]==LA_DTARGET_ARROW_2) {
                RPU_SetLampState(LA_DTARGET_ARROW_2, 1);
                DTarget3Lights[CurrentPlayer][1] = 1;
                if (!Bonus2XComplete()) PlaySound(24); // Bong Bounce Down
                AddToScore(10000);
                AddToMiniBonus(1);
                DropTargetHit();
              } else if (DTarget3Lights[CurrentPlayer][1]==1) {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
                AddToMiniBonus(1);
                DropTargetHit();
              } else {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(5000);
                AddToMiniBonus(1);
                DropTargetHit();
              }
            }
            if (switchHit==SW_DTARGET_3_C) {
              if (DTarget3Lights[CurrentPlayer][2]==0 && DTarget3Flashing[CurrentPlayer]==LA_DTARGET_ARROW_3) {
                RPU_SetLampState(LA_DTARGET_ARROW_3, 1);
                DTarget3Lights[CurrentPlayer][2] = 1;
                if (!Bonus2XComplete()) PlaySound(24); // Bong Bounce Down
                AddToScore(10000);
                AddToMiniBonus(1);
                DropTargetHit();
              } else if (DTarget3Lights[CurrentPlayer][2]==1) {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(10000);
                AddToMiniBonus(1);
                DropTargetHit();
              } else {
                 PlaySound(25); // Bong Bounce Up
                AddToScore(5000);
                AddToMiniBonus(1);
                DropTargetHit();
              }
            }
            if (CheckIfDTargets3Down()) {
              My_PushToTimedSolenoidStack(SO_DTARGET_3_RESET, 15, CurrentTime + 500);
              DTarget3Completions++;
              if ((!OriginalRules) && (DTarget3Completions > 3)) DTarget3Completions = 2; // Done by old rules, or set up to go again
              if (DTarget3Completions==1) {
                RPU_SetLampState(LA_POP_TOP, 1);
                TopPopLit = true;
              } else if (DTarget3Completions==2) {
                RPU_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 1);
                MiniBonusReady[CurrentPlayer] = true;
              } else if (DTarget3Completions==3) {
                RPU_SetLampState(LA_TARGET_UPPER_SPECIAL, 1);
                TopSpecialLit[CurrentPlayer] = true;
              }
              AddToScore(15000);
              RPU_SetLampState(LA_SAUCER_30K, 1);
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
              PlaySound(42); // S&T: Lucky shot Earthling. Geeteoh: Very roughly, 14 seconds!
              RPU_SetLampState(LA_SAUCER_ARROW_2X, 1, 0, 150);
              RPU_SetLampState(LA_BONUS_MINI_50K, 1);
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
            }
          } else  {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;

        case SW_TARGET_TOP:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(5000);
            AddToMiniBonus(1);
            if (MiniBonusReady[CurrentPlayer]==false && TopSpecialLit[CurrentPlayer]==false) {
              TargetUpperBlip = CurrentTime + 100;
              RPU_SetLampState(LA_TARGET_UPPER_SPECIAL, 1);
              RPU_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 1);
            }
            if (MiniBonusReady[CurrentPlayer]==true) {
              RPU_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, 0);
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
                RPU_SetLampState(LA_SHOOT_AGAIN, 1);
                RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
              }
              else if (PlayfieldAward == 3) {
              AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
              if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
              TotalReplays += 1;
              RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
              My_PushToSolenoidStack(SO_KNOCKER, 5, true);
              }
              RPU_SetLampState(LA_TARGET_UPPER_SPECIAL, 0);
              TopSpecialLit[CurrentPlayer] = false;
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
            } else {
              PlaySound(27); // Look out, Flash!
            }
          } else  {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;

        case SW_DTARGET_INLINE_1ST:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(10000);
            AddToSuperBonus(2);
            DropTargetHit();
            PlaySound(25); // Bong Bounce Up
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            // My_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime);
          }
          break;

        case SW_DTARGET_INLINE_2ND:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
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
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(10000);
            DTargetInlineGoal[CurrentPlayer] = true;
            // RightSpinnerLit = true;
            // RPU_SetLampState(LA_SPINNER_RIGHT, 1);
            RPU_SetLampState(LA_SAUCER_20K, 1);
            CheckSaucerDTargetGoal();
            #ifndef USE_EXTENDED_SOUNDS
            PlaySound(25); // Bong Bounce Up
            #else
            PlaySound(85); // Try the Wood Beast, or die!
            #endif
            RPU_SetLampState(LA_TARGET_WOOD_BEAST_XBALL, 1);
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
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            RPU_SetLampState(LA_TARGET_WOOD_BEAST_XBALL, 0);
            if (PlayfieldAward == 1 || (PlayfieldAward >= 2 && (PlayerShootsAgain || FreeBallCollected[CurrentPlayer])))
              AddToScore(0); // No extra points awarded for no extra ball
            else if (PlayfieldAward == 2 ||PlayfieldAward == 3) {
              PlayerShootsAgain = true;
              if (!dipUnlimitedExtraBalls) FreeBallCollected[CurrentPlayer] = true;
              RPU_SetLampState(LA_SHOOT_AGAIN, 1);
              RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
            }
            My_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime + 2000); // increased from 750, to prevent ball from getting trapped!
            #ifndef USE_EXTENDED_SOUNDS
            PlaySound(27); // Crash bounce up
            #else
            PlaySound(74); // Wood Beast roar
            #endif
            AddToScore(25000);
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;

        case SW_REBOUND:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            PlaySound(7); // Rebound
            AddToScore(10);
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
              PlaySound(13); // Ascending sound
            }
          } else {
            PlaySound(24); // wizard sound: bong bounce down
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;

        case SW_SAUCER:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (SaucerHitTime==0 || (CurrentTime-SaucerHitTime)>500) { // Run when ball enters saucer. If called again within 1/2 second, ignore. Ball is jostling.
              if (RPU_STROBE_TYPE == 2) SetupStrobe(5, 250);
              SaucerHitTime = CurrentTime;
              if (Playfield2xState[CurrentPlayer]==1 || Playfield3xState[CurrentPlayer]==1) {
                SaucerAnimation = CurrentTime+3770;
                DeleteQueue();
                PlaySound(90); // Stop sound
                #ifndef USE_EXTENDED_SOUNDS
                PlaySound(40); // Ignite death ray, 15 seconds
                #else
                PlaySound(86, 10); // Remove the Earth woman. Forget it Ming! Dale's with me!
                #endif
              } else {
                #ifndef USE_EXTENDED_SOUNDS
                PlaySound(22); // Escape is impossible!
                SaucerAnimation = CurrentTime + 1650;
                #else
                if (SaucerDirection)
                  PlaySound(k = VoiceSaucerUp[1 + random(0, VoiceSaucerUp[0])]);     // Choose from random selection of saucer up voices 
                else
                  PlaySound(k = VoiceSaucerDown[1 + random(0, VoiceSaucerDown[0])]); // Choose from random selection of saucer down voices
                SaucerAnimation = CurrentTime + max(1650, 100 * SoundData[k][2]);
                #endif
              }
              if (dipSaucerStart10k) AddToScore(10000);
              if (DTarget4Goal[CurrentPlayer]) AddToScore(10000);
              if (DTargetInlineGoal[CurrentPlayer]) AddToScore(20000);
              if (DTarget3Goal[CurrentPlayer]) AddToScore(30000);
                  RPU_SetLampState(LA_SAUCER_XBALL, 0);
                if (DTarget4Goal[CurrentPlayer] && DTarget3Goal[CurrentPlayer] && DTargetInlineGoal[CurrentPlayer]) {
                  if (PlayfieldAward == 1 || (PlayfieldAward == 2 || PlayfieldAward == 3) && PlayerShootsAgain)
                    AddToScore(25000);
                  else if (PlayfieldAward == 2 || PlayfieldAward == 3) {
                    PlayerShootsAgain = true;
                    RPU_SetLampState(LA_SHOOT_AGAIN, 1);
                    RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
                  }
                }
              DTarget4Goal[CurrentPlayer] = false;
              DTargetInlineGoal[CurrentPlayer] = false;
              DTarget3Goal[CurrentPlayer] = false;

              RPU_SetLampState(LA_SAUCER_10K, 0);
              RPU_SetLampState(LA_SAUCER_20K, 0);
              RPU_SetLampState(LA_SAUCER_30K, 0);
              BallInSaucer = true;
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
            }
          } else {
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
            if (SaucerHitTime==0 || (CurrentTime-SaucerHitTime)>500) {
              if (RPU_STROBE_TYPE == 2) SetupStrobe(5, 250);
              SaucerHitTime = CurrentTime;
              if (MingAttackReady==false) {
                AddToScore(3500);
                MingAttackProgress += 35;
                if (MingAttackProgress>=WizardGoalAttack && MingAttackReady==false) {
                  MingAttackReady = true;
                } else {
                  PlaySound(25); // wizard sound: bong bounce up
                }
              }
              if (MingAttackReady==true) {
                if (MingHealth != 0) MingHealth--;
                MingHealthLights(MingHealth); // set drop target lights to indicate Ming's health
                if (MingHealth==0) {
                  MingWasDefeated = true;
                  DeleteQueue();
                  PlaySound(90); // stop all sounds
                  #ifndef USE_EXTENDED_SOUNDS 
                  PlaySound(48); // wizard sound: defeat miserable earthling
                  #else
                  PlaySound(46, 10); // You've saved your Earth. Have a nice day!
                  #endif
                  My_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime+3100, true);
                  My_PushToTimedSolenoidStack(SO_KNOCKER, 5, CurrentTime+3100);
                  MingAttackAnimation = CurrentTime+2475;
                } else {
                  #ifndef USE_EXTENDED_SOUNDS
                  PlaySound(42); // wizard sound: Lucky shot earthling
                  #else
                  PlaySound(31); // Your power's fading, Ming
                  #endif
                  MingAttackAnimation = CurrentTime+1650;
                }
              }
              else
                if (MingHealth == 0) My_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime+1000, true); // added in case ball pops back into saucer after Ming Defeat
              BallInSaucer = true;
            }
          }
          break;

        case SW_OUTLANE_RIGHT:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (OutLanesLit[CurrentPlayer]) {
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              if (PlayfieldAward == 1 || (PlayfieldAward == 2 && PlayerShootsAgain) || (PlayfieldAward == 3 && FreeGameCollected[CurrentPlayer]))
                  AddToScore(50000);
                else if (PlayfieldAward == 2) {
                  PlayerShootsAgain = true;
                  RPU_SetLampState(LA_SHOOT_AGAIN, 1);
                  RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
                }
                else if (PlayfieldAward == 3) {
                  AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
                  if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
                  TotalReplays += 1;
                  RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
                  My_PushToSolenoidStack(SO_KNOCKER, 5, true);
                  }
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(2000);
            AddToSuperBonus(2);
            if (WizardState[CurrentPlayer]==0) PlaySound(28); // What a damn nuisance!
            if (WizardState[CurrentPlayer]==1) {
              WizardState[CurrentPlayer] = 2;
              WizSource = 1;
              DeleteQueue();
              PlaySound(90); // Stop sounds
              #ifndef USE_EXTENDED_SOUNDS
              PlaySound(50); // Ming laugh
              #else
              PlaySound(51, 10); // Savior of the Universe!
              #endif
              // delayTimer = max(GTOWaitingTime, CurrentTime) + 100 * SoundData[51][2]; // will delay for sound to play before going into countdown
              CDBSpeed = 25;
            }
          } else {
            if (WizardState[CurrentPlayer]==5) PlaySound(50); // wizard sound: Ming laugh!
          }
          BallDrained = true;
          break;

        case SW_OUTLANE_LEFT:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (OutLanesLit[CurrentPlayer]) {
            RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
            RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
            if (PlayfieldAward == 1 || (PlayfieldAward == 2 && PlayerShootsAgain) || (PlayfieldAward == 3 && FreeGameCollected[CurrentPlayer]))
                  AddToScore(50000);
                else if (PlayfieldAward == 2) {
                  PlayerShootsAgain = true;
                  RPU_SetLampState(LA_SHOOT_AGAIN, 1);
                  RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
                }
                else if (PlayfieldAward == 3) {
                  AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
                  if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
                  TotalReplays += 1;
                  RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
                  My_PushToSolenoidStack(SO_KNOCKER, 5, true);
                  }
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(2000);
            AddToSuperBonus(2);
            if (WizardState[CurrentPlayer]==0) PlaySound(28); // What a damn nuisance!
            if (WizardState[CurrentPlayer]==1) {
              WizardState[CurrentPlayer] = 2;
              WizSource = 1;
              DeleteQueue();
              PlaySound(90); // Stop sounds
              #ifndef USE_EXTENDED_SOUNDS
              PlaySound(50); // Ming laugh
              #else
              PlaySound(51, 10); // Savior of the Universe!
              #endif
              // delayTimer = max(GTOWaitingTime, CurrentTime) + 100 * SoundData[51][2]; // will delay for sound to play before going into countdown
              CDBSpeed = 25;
            }
          } else {
            if (WizardState[CurrentPlayer]==5) PlaySound(50); // wizard sound: Ming laugh!
          }
          BallDrained = true;
          break;

        case SW_SPINNER_RIGHT:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (RightSpinnerLit==false) {
              PlaySound(8); // Spinner noise
              AddToScore(100);
            } else {
              PlaySound(9); // Low hum
              AddToScore(1000);
            }
            if (RightSpinnerHurryUpLit==true) {
              if (RightSpinnerMissed) AddToScore(15000);
              RightSpinnerMissed = false;
              RightSpinnerHurryUpTimer = CurrentTime + 500;
              AddToScore(2000);
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
            }
          } else {
            AddToScore(1000);
            MingAttackProgress += 4;
            if (MingAttackProgress>=WizardGoalAttack && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(7); // wizard sound: Rebound hit
            }
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
          
        case SW_SPINNER_LEFT:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            if (LeftSpinnerLit==false) {
              PlaySound(8); // Spinner noise
              AddToScore(100);
            } else {
              PlaySound(9); // Low hum
              AddToScore(1000);
            }
            if (LeftSpinnerHurryUpLit==true) {
              if (LeftSpinnerMissed) AddToScore(15000);
              LeftSpinnerMissed = false;
              LeftSpinnerHurryUpTimer = CurrentTime + 500;
              AddToScore(2000);
            }
            if (curState==MACHINE_STATE_SKILL_SHOT) {
              SkillShotState = 3;
            }
          } else {
            AddToScore(1000);
            MingAttackProgress += 4;
            if (MingAttackProgress>=WizardGoalAttack && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(7); // wizard sound: Rebound hit
            }
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;

        case SW_SLING_RIGHT:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(50);
            PlaySound(21); // Ray gun
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;

        case SW_SLING_LEFT:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            AddToScore(50);
            PlaySound(21); // Ray gun
          } else {
            PlaySound(24); // wizard sound: bong bounce down
          }
          break;

        case SW_POP_TOP:
          if (PFValidated==false) {
            My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
            PFValidated = true;
          }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
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
            }
          } else {
            AddToScore(2000);
            MingAttackProgress += 20;
            if (MingAttackProgress>=WizardGoalAttack && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(25); // wizard sound: bong bounce up. Was 27, which was repurposed.
            }
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;

        case SW_POP_RIGHT:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            PlaySound(20); // Ming's ring
            AddToScore(1000);
          } else  {
            AddToScore(2000);
            MingAttackProgress += 20;
            if (MingAttackProgress>=WizardGoalAttack && MingAttackReady==false) {
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(25); // wizard sound: bong bounce up. Was 27, which was repurposed.
            }
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
          
        case SW_POP_LEFT:
          if (PFValidated==false) {
              My_PushToSolenoidStack(SO_DTARGET_1_RESET, 5, true);
              PFValidated = true;
            }
          if (curState!=MACHINE_STATE_WIZARD_MODE) {
            PlaySound(20); // Ming's ring
            AddToScore(1000);
          } else  {
            AddToScore(2000);
            MingAttackProgress += 20;
            if (MingAttackProgress>=WizardGoalAttack && MingAttackReady==false) {
              MingAttackReady = true;
              PlaySound(45); // wizard sound: Flash!
              MingAttackReady = true;
            } else {
              PlaySound(25); // wizard sound: bong bounce up. Was 27, which was repurposed.
            }
            if (WizardState[CurrentPlayer]==4) {
              RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 0);
              RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 0);
              OutLanesLit[CurrentPlayer] = false;
              WizardState[CurrentPlayer] = 5;
              My_PushToTimedSolenoidStack(SO_DTARGET_1_RESET, 15, CurrentTime);
            }
          }
          break;
      }
    }
  } else {
    // We're tilted, so just wait for outhole. Pull all switches, no points awarded. React to bare minimum of switches!
    while ( (switchHit = RPU_PullFirstFromSwitchStack()) != SWITCH_STACK_EMPTY && switchHit != SW_OUTHOLE) {
      switch (switchHit) {
        case SW_SELF_TEST_SWITCH:
          returnState = MACHINE_STATE_TEST_LAMPS;
          SetLastSelfTestChangedTime(CurrentTime);
          break; 
        case SW_COIN_1:
          if (!FreePlay) {
            AddCredit(0, CreditsperCoin[0], CoinsperCredit[0]);
            RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
          }
          break;
        case SW_COIN_2:
          if (!FreePlay) {
            AddCredit(1, CreditsperCoin[1], CoinsperCredit[1]);
            RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
          }
          break;
        case SW_COIN_3:
          if (!FreePlay) {
            AddCredit(2, CreditsperCoin[2], CoinsperCredit[2]);
            RPU_SetDisplayCredits(Credits, dipCreditsDisplayed);
          }
          break;
        case SW_SAUCER:
          My_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime + 500, true);
         break;
      }
    }
  }
  if (switchHit == SW_OUTHOLE && NumTiltWarnings >= MAX_TILT_WARNINGS) 
    returnState = MACHINE_STATE_BALL_OVER; // Should take care of tilt during skill shot!

  if (miniBonusAtTop!=MiniBonus) { // display mini bonus if changed
    ShowMiniBonusOnLadder(MiniBonus);
  }

  if (superBonusAtTop!=SuperBonus) { // display super bonus if changed
    ShowSuperBonusOnLadder(SuperBonus);
  }

  if (scoreAtTop!=CurrentScores[CurrentPlayer]) {
  }


  // handle score award thresholds
  if (CurrentScores[CurrentPlayer]>=ScoreAward1 && ScoreAward1 > 0 && ScoreAwardStates[CurrentPlayer]==0) {
    ScoreAwardStates[CurrentPlayer] = 1;
    if (ThresholdAward == 1 || (ThresholdAward == 2 && PlayerShootsAgain) || (ThresholdAward == 3 && FreeGameCollected[CurrentPlayer]))
      AddToScore(25000);
    else if (ThresholdAward == 2) { // Extra Ball
      PlayerShootsAgain = true;
      RPU_SetLampState(LA_SHOOT_AGAIN, 1);
      RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
    }
    else if (ThresholdAward == 3) { // One Credit
    My_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
    AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
    if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
    TotalReplays += 1;
    RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
    }
  }
  if (CurrentScores[CurrentPlayer]>=ScoreAward2 && ScoreAward2 > 0 && ScoreAwardStates[CurrentPlayer]==1) {
    ScoreAwardStates[CurrentPlayer] = 2;
    if (ThresholdAward == 1 || (ThresholdAward == 2 && PlayerShootsAgain) || (ThresholdAward == 3 && FreeGameCollected[CurrentPlayer]))
      AddToScore(25000);
    else if (ThresholdAward == 2) { // Extra Ball
      PlayerShootsAgain = true;
      RPU_SetLampState(LA_SHOOT_AGAIN, 1);
      RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
    }
    else if (ThresholdAward == 3) { // One Credit
    My_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
    AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
    if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
    TotalReplays += 1;
    RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
    }
  }
  if (CurrentScores[CurrentPlayer]>=ScoreAward3 && ScoreAward3 > 0 && ScoreAwardStates[CurrentPlayer]==2) {
    ScoreAwardStates[CurrentPlayer] = 3;
    if (ThresholdAward == 1 || (ThresholdAward == 2 && PlayerShootsAgain) || (ThresholdAward == 3 && FreeGameCollected[CurrentPlayer]))
      AddToScore(25000);
    else if (ThresholdAward == 2) { // Extra Ball
      PlayerShootsAgain = true;
      RPU_SetLampState(LA_SHOOT_AGAIN, 1);
      RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
    }
    else if (ThresholdAward == 3) { // One Credit
    My_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
    AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
    if (!dipUnlimitedReplays) FreeGameCollected[CurrentPlayer] = true;
    TotalReplays += 1;
    RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
    }
  }

  return returnState;
}



// #################### LOOP ####################
void loop() {
  // This line has to be in the main loop
  RPU_DataRead(0);

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

  if (newMachineState!=MachineState) {
    MachineState = newMachineState;
    MachineStateChanged = true;
  } else {
    MachineStateChanged = false;
  }

  RPU_ApplyFlashToLamps(CurrentTime);
  RPU_UpdateTimedSolenoidStack(CurrentTime);
}



// #################### LAMP ANIMATIONS ####################
// ########## BACKGLASS LAMPS CLEAR ##########
void BackglassLampsClear() {
  RPU_SetLampState(LA_FLASH_GORDON_6, 0);
  RPU_SetLampState(LA_FLASH_GORDON_1, 0);
  RPU_SetLampState(LA_FLASH_GORDON_5, 0);
  RPU_SetLampState(LA_FLASH_GORDON_2, 0);
  RPU_SetLampState(LA_FLASH_GORDON_4, 0);
  RPU_SetLampState(LA_FLASH_GORDON_3, 0);
  RPU_SetLampState(LA_FLASH_STROBE, 0);
}


// ########## BACKGLASS LAMPS CENTER OUT ##########
void backglassLampsCenterOut() {
  if (RPU_STROBE_TYPE >= 3) {
    byte lampPhase = (CurrentTime/115)%4;
    RPU_SetLampState(LA_FLASH_GORDON_6, lampPhase==3||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_1, lampPhase==3||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_5, lampPhase==2||lampPhase==3, lampPhase==3);
    RPU_SetLampState(LA_FLASH_GORDON_2, lampPhase==2||lampPhase==3, lampPhase==3);
    RPU_SetLampState(LA_FLASH_GORDON_4, lampPhase==1||lampPhase==2, lampPhase==2);
    RPU_SetLampState(LA_FLASH_GORDON_3, lampPhase==1||lampPhase==2, lampPhase==2);
    // if (MachineState!=MACHINE_STATE_NORMAL_GAMEPLAY || (MachineState==MACHINE_STATE_NORMAL_GAMEPLAY && BallInSaucer==true)) 
      RPU_SetLampState(LA_FLASH_STROBE, lampPhase<2, lampPhase==1);
  } else {
    byte lampPhase = (CurrentTime/115)%3;
    RPU_SetLampState(LA_FLASH_GORDON_6, lampPhase==2||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_1, lampPhase==2||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_5, lampPhase==1||lampPhase==2, lampPhase==2);
    RPU_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==2, lampPhase==2);
    RPU_SetLampState(LA_FLASH_GORDON_4, lampPhase<2, lampPhase==1);
    RPU_SetLampState(LA_FLASH_GORDON_3, lampPhase<2, lampPhase==1);
  }
}


// ########## BACKGLASS LAMPS KNIGHT RIDER ##########
void BackglassLampsKnightRider() {
  if (RPU_STROBE_TYPE >= 3) {
    byte lampPhase = (CurrentTime/80)%14;
    RPU_SetLampState(LA_FLASH_GORDON_6, lampPhase==6||lampPhase==7, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_5, lampPhase==5||lampPhase==8, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_4, lampPhase==4||lampPhase==9, 0, 0);
    // if (MachineState!=MACHINE_STATE_NORMAL_GAMEPLAY) 
      RPU_SetLampState(LA_FLASH_STROBE, lampPhase==3||lampPhase==10, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_3, lampPhase==2||lampPhase==11, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==12, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_1, lampPhase==0||lampPhase==13, 0, 0);
  } else {
    byte lampPhase = (CurrentTime/80)%12;
    RPU_SetLampState(LA_FLASH_GORDON_6, lampPhase==5||lampPhase==6, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_5, lampPhase==4||lampPhase==7, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_4, lampPhase==3||lampPhase==8, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_3, lampPhase==2||lampPhase==9, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==10, 0, 0);
    RPU_SetLampState(LA_FLASH_GORDON_1, lampPhase==0||lampPhase==11, 0, 0);
  }
}


// ########## BACKGLASS LAMPS LEFT TO RIGHT ##########
void BackglassLampsLeft2Right() {
  if (RPU_STROBE_TYPE >= 3) {
    byte lampPhase = (CurrentTime/85)%4;
    RPU_SetLampState(LA_FLASH_GORDON_6, lampPhase==3||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_3, lampPhase==3||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_5, lampPhase==2||lampPhase==3, lampPhase==3);
    RPU_SetLampState(LA_FLASH_GORDON_2, lampPhase==2||lampPhase==3, lampPhase==3);
    RPU_SetLampState(LA_FLASH_GORDON_4, lampPhase==1||lampPhase==2, lampPhase==2);
    RPU_SetLampState(LA_FLASH_GORDON_1, lampPhase==1||lampPhase==2, lampPhase==2);
    // if (MachineState!=MACHINE_STATE_NORMAL_GAMEPLAY) 
      RPU_SetLampState(LA_FLASH_STROBE, lampPhase<2, lampPhase==1);
  } else {
    byte lampPhase = (CurrentTime/95)%3;
    RPU_SetLampState(LA_FLASH_GORDON_6, lampPhase==2||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_3, lampPhase==2||lampPhase==0, lampPhase==0);
    RPU_SetLampState(LA_FLASH_GORDON_5, lampPhase==1||lampPhase==2, lampPhase==2);
    RPU_SetLampState(LA_FLASH_GORDON_2, lampPhase==1||lampPhase==2, lampPhase==2);
    RPU_SetLampState(LA_FLASH_GORDON_4, lampPhase<2, lampPhase==1);
    RPU_SetLampState(LA_FLASH_GORDON_1, lampPhase<2, lampPhase==1);
  }
}


// ########## SPIRAL PATTERN ##########
// Run spiral pattern (1) or turn it off (0)
void SpiralPattern(boolean go) { 
  byte spiral[23]= {1, 0, 9, 8, 7, 6, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 5, 4, 3, 2, 1, 0, 9};

  if (go) {
    byte lampOff = (CurrentTime / 115) % 20; // lamp to go off; next 3 go on. 115 ms increments, 20 phases.
    RPU_SetLampState(spiral[lampOff], 0);
    RPU_SetLampState(spiral[lampOff+1], 1);
    RPU_SetLampState(spiral[lampOff+2], 1);
    RPU_SetLampState(spiral[lampOff+3], 1);
  }
  else 
    for (byte i = 0; i < 20; ++i)
      RPU_SetLampState(spiral[i], 0);
}


// ########## Ming Health Lights ##########
// Light a number of drop target lights to indicate Ming's health
void MingHealthLights(byte health) { 
  RPU_SetLampState(LA_DTARGET_4_A, health >= 4);
  RPU_SetLampState(LA_DTARGET_4_B, health >= 3);
  RPU_SetLampState(LA_DTARGET_4_C, health >= 2);
  RPU_SetLampState(LA_DTARGET_4_D, health >= 1);
}


// ########## ATTRACT RETRO ##########
void AttractRetro() {
  // bonus
  byte attractBonus;
  for (attractBonus=0; attractBonus<10; attractBonus++){
    if ( attractBonus % 2 == 0) { // even but odd on pf
      RPU_SetLampState(LA_BONUS_MINI_1K+attractBonus, 1, 0, 100);
      RPU_SetLampState(LA_BONUS_SUPER_1K+attractBonus, 1, 0, 100);
    } else { // odd even on pf
      RPU_SetLampState(LA_BONUS_MINI_1K+attractBonus, 1, 0, 200);
      RPU_SetLampState(LA_BONUS_SUPER_1K+attractBonus, 1, 0, 200);
    }
  }

  // sweep 4
  byte lampPhase1 = (CurrentTime/115)%4; // 125
  RPU_SetLampState(LA_SAUCER_XBALL, lampPhase1==3||lampPhase1==0, lampPhase1==0); // SAUCER
  RPU_SetLampState(LA_SAUCER_30K, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  RPU_SetLampState(LA_SAUCER_20K, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  RPU_SetLampState(LA_SAUCER_10K, lampPhase1<2, lampPhase1==1);
  RPU_SetLampState(LA_DTARGET_4_A, lampPhase1==3||lampPhase1==0, lampPhase1==0); // DTARGET 4
  RPU_SetLampState(LA_DTARGET_4_B, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  RPU_SetLampState(LA_DTARGET_4_C, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  RPU_SetLampState(LA_DTARGET_4_D, lampPhase1<2, lampPhase1==1);
  RPU_SetLampState(LA_BONUS_5X, lampPhase1==3||lampPhase1==0, lampPhase1==0); // BONUS X
  RPU_SetLampState(LA_BONUS_4X, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  RPU_SetLampState(LA_BONUS_3X, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  RPU_SetLampState(LA_BONUS_2X, lampPhase1<2, lampPhase1==1);
  RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, lampPhase1==3||lampPhase1==0, lampPhase1==0); // OUTLANES & PFX
  RPU_SetLampState(LA_SAUCER_ARROW_3X, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  RPU_SetLampState(LA_CLOCK_15_SECONDS_2X, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  RPU_SetLampState(LA_SAUCER_ARROW_2X, lampPhase1<2, lampPhase1==1);
  RPU_SetLampState(LA_CLOCK_15_SECONDS_3X, lampPhase1<2, lampPhase1==1);
  RPU_SetLampState(LA_INLANE_LEFT, lampPhase1==3||lampPhase1==0, lampPhase1==0); // INLANES & SPINNERS
  RPU_SetLampState(LA_SPINNER_RIGHT, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  RPU_SetLampState(LA_INLANE_RIGHT, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  RPU_SetLampState(LA_SPINNER_LEFT, lampPhase1<2, lampPhase1==1);
  RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, lampPhase1==3||lampPhase1==0, lampPhase1==0); // REMAINDER
  RPU_SetLampState(LA_TARGET_WOOD_BEAST_XBALL, lampPhase1==2||lampPhase1==3, lampPhase1==3);
  RPU_SetLampState(LA_TARGET_LRIGHT_TOP, lampPhase1==1||lampPhase1==2, lampPhase1==2);
  RPU_SetLampState(LA_DTARGET_BONUS_5X, lampPhase1<2, lampPhase1==1);

  // sweep 5
  byte lampPhase2 = (CurrentTime/95)%5;
  RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, lampPhase2==4||lampPhase2==0, lampPhase2==0);
  RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, lampPhase2==3||lampPhase2==4, lampPhase2==4);
  RPU_SetLampState(LA_STAR_SHOOTER_TOP, lampPhase2==2||lampPhase2==3, lampPhase2==3);
  RPU_SetLampState(LA_STAR_PFIELD_TOP, lampPhase2==1||lampPhase2==2, lampPhase2==2);
  RPU_SetLampState(LA_STAR_PFIELD_BOTTOM, lampPhase2<2, lampPhase2==1);

  // sweep 4
  byte lampPhase3 = (CurrentTime/75)%6;
  RPU_SetLampState(LA_DTARGET_ARROW_3, lampPhase3==5||lampPhase3==0, lampPhase3==0);
  RPU_SetLampState(LA_DTARGET_ARROW_2, lampPhase3==4||lampPhase3==5, lampPhase3==5);
  RPU_SetLampState(LA_DTARGET_ARROW_1, lampPhase3==3, lampPhase3==4, lampPhase3==4);
  RPU_SetLampState(LA_TARGET_UPPER_SPECIAL, lampPhase3==2||lampPhase3==3, lampPhase3==3);
  RPU_SetLampState(LA_TARGET_UPPER_COLLECT_BONUS, lampPhase3==1||lampPhase3==2, lampPhase3==2);
  RPU_SetLampState(LA_DTARGET_BONUS_4X, lampPhase3<2, lampPhase3==1);

  // remainder
  RPU_SetLampState(LA_POP_TOP, 1, 0, 250);
  RPU_SetLampState(LA_BONUS_MINI_50K, 1, 0, 250);
  RPU_SetLampState(LA_BONUS_SUPER_100K, 1, 0, 250);
  RPU_SetLampState(LA_SHOOT_AGAIN, 1, 0, 250);
  RPU_SetLampState(LA_MING_TOP, 1, 0, 250);
  RPU_SetLampState(LA_MING_BOTTOM, 1);

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
    RPU_DisableSolenoidStack();
    RPU_SetDisableFlippers(true);
    RPU_TurnOffAllLamps();
    if (Credits > 0) RPU_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    RPU_SetLampState(LA_TILT, 1);
    DeleteQueue();
    PlaySound(90); // stop all sounds
    PlaySound(41, 10); // Gently, Darling! It's extremely sensitive, like me.
  }
}



// #################### SKILL SHOT ####################
int SkillShot(boolean curStateChanged) {
  int returnState = MACHINE_STATE_SKILL_SHOT;

  if (curStateChanged) {
    RPU_SetDisplayCredits(0, true);
    RPU_SetLampState(LA_MING_TOP, 1, 0, 100);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    if (FirstBall && RPU_STROBE_TYPE == 2) SetupStrobe(4, 250);

    DeleteQueue();
    PlaySound(90); // stop all sounds
    #ifdef USE_EXTENDED_SOUNDS
    if (backgroundOn) PlaySound(84, 100); // Background music
    if (FirstBall) PlaySound(47, 1250); // Gordon's alive! Only on first ball. Cannot use 44, since Geeteoh assumes it's in attract mode if I do and plays 254!
    #else
    if (backgroundOn) PlaySound(6); // Background Sound 1
    PlaySound(44); // Emperor Ming Awaits
    #endif
  }

  // HANDLE LAMPS
  if (SkillShotState==0) {
    backglassLampsCenterOut();
    byte lampPhase = (CurrentTime/600)%3;
    RPU_SetLampState(LA_STAR_SHOOTER_TOP, lampPhase==2, 0, 100);
    RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, lampPhase==1||lampPhase==2, 0, 100);
    RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, lampPhase==0||lampPhase==1||lampPhase==2, 0, 100);
  } else if (SkillShotState==1) {
    BackglassLampsClear();
    if (SkillShotHits==1 || SkillShotHits==2) {
      RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1);
      RPU_SetDisplayCredits(15);
    } else if (SkillShotHits==3 || SkillShotHits==4) {
      RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1);
      RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1);
      RPU_SetDisplayCredits(30);
    } else if (SkillShotHits>=5) {
      RPU_SetLampState(LA_STAR_SHOOTER_TOP, 1);
      RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1);
      RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1);
      RPU_SetDisplayCredits(75);
    }
  } else if (SkillShotState==2) {
    if (CurrentTime<=SkillShotScoreAnimation) {
      if (SkillShotHits==1 || SkillShotHits==2) {
        RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1, 0, 125);
      } else if (SkillShotHits==3 || SkillShotHits==4) {
        RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1, 0, 125);
        RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1, 0, 125);
      } else if (SkillShotHits>=5) {
        RPU_SetLampState(LA_STAR_SHOOTER_TOP,    1, 0, 125);
        RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 1, 0, 125);
        RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 1, 0, 125);
      }
      if (RPU_STROBE_TYPE >= 3) RPU_SetLampState(LA_FLASH_STROBE, 1, 0, 125);
    } else {
      SkillShotState = 3;
    }
  }
  SpiralPattern(1);

  // If the skillshot hasn't been collected yet, flash score
  if (SkillShotState<=2 && !PFValidated) { // Skill shot over if state is 3 or ball on playfield
    CountingFlashDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], CurrentTime, 250, 2);
  } else {
    RPU_SetLampState(LA_STAR_SHOOTER_TOP, 0);
    RPU_SetLampState(LA_STAR_SHOOTER_MIDDLE, 0);
    RPU_SetLampState(LA_STAR_SHOOTER_BOTTOM, 0);
    if (RPU_STROBE_TYPE >= 3) RPU_SetLampState(LA_FLASH_STROBE, 0);
    returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
    PlaySound(90); // stop skill shot music
    for (byte count=0; count<CurrentNumPlayers; count++) {
      CountingDisplay(count, CurrentScores[count], true, 2, true);
    }
  }
  // Stop lamp blips for lower level hits during skill shot
  if (CurrentTime >= TargetLowerTopBlip   ) RPU_SetLampState(LA_TARGET_LRIGHT_TOP, 0);
  if (CurrentTime >= InlaneLeftBlip       ) RPU_SetLampState(LA_INLANE_LEFT, 0);
  if (CurrentTime >= InlaneRightBlip      ) RPU_SetLampState(LA_INLANE_RIGHT, 0);
  if (CurrentTime >= TargetLowerBottomBlip) RPU_SetLampState(LA_TARGET_LRIGHT_BOTTOM, 0);

  // Check to see if ball is in the outhole. Added to fix skillshot straight to outhole, or simply fell back in.
  if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
    RPU_SetDisplayCredits(DropTargetCount, true); 
    if (BallTimeInTrough == 0) {
      BallTimeInTrough = CurrentTime;
    } else {
      // Make sure the ball stays on the sensor for at least 
      // 0.5 seconds to be sure that it's not bouncing
      if ((CurrentTime - BallTimeInTrough) > 500) {
          BallTimeInTrough = 0;
          SkillShotState = 0;
          SkillShotHits = 0;
          if (CurrentScores[CurrentPlayer] == InitialScore) 
            {
            My_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
            #ifndef USE_EXTENDED_SOUNDS
            PlaySound(46); // Try again Earthling
            #else
            PlaySound(77, 1000); // Don't kill him yet, father
            #endif
            }
          else
            returnState = MACHINE_STATE_BALL_OVER; // points scored in lower playfield with Skill Shot mode. Ball over will handle.
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
  if (DropTargetCount==WizardGoalTargets) {
    RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 1);
    RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 1);
    OutLanesLit[CurrentPlayer] = true;
    OutLanesCollected[CurrentPlayer] = false;
    RPU_SetLampState(LA_MING_TOP, 1, 0, 100);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    WizardState[CurrentPlayer] = 1;

    #ifndef USE_EXTENDED_SOUNDS
    PlaySound(44); // Emperor Ming awaits
    #else
    PlaySound(4); // wizard sound: This way Flash, come on! Was 44, Emperor Ming awaits.
    #endif
  }
}


// ########## CHECK IF DROP TARGETS FOUR DOWN ##########
boolean CheckIfDTargets4Down() {
  return (  RPU_ReadSingleSwitchState(SW_DTARGET_4_A) &&
            RPU_ReadSingleSwitchState(SW_DTARGET_4_B) &&
            RPU_ReadSingleSwitchState(SW_DTARGET_4_C) &&
            RPU_ReadSingleSwitchState(SW_DTARGET_4_D) );
}


// ########## CHECK IF DROP TARGETS 3 DOWN ##########
boolean CheckIfDTargets3Down() {
  return (  RPU_ReadSingleSwitchState(SW_DTARGET_3_A) &&
            RPU_ReadSingleSwitchState(SW_DTARGET_3_B) &&
            RPU_ReadSingleSwitchState(SW_DTARGET_3_C) );
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

    #ifndef USE_EXTENDED_SOUNDS
    PlaySound(48); // Miserable Earthling
    #else
    // PlaySound(87); // This place is a lunatic asylum
    PlaySound(42); // Very roughly, 14 seconds
    #endif
    RPU_SetLampState(LA_SAUCER_ARROW_3X, 1, 0, 150);
    RPU_SetLampState(LA_BONUS_SUPER_100K, 1);
  }
}


// ########## CHECK SAUCER DROP TARGET GOAL ##########
void CheckSaucerDTargetGoal() {
//  if (DTarget4Goal[CurrentPlayer]==true && DTarget3Goal[CurrentPlayer]==true && DTargetInlineGoal[CurrentPlayer]==true && WoodBeastXBallState[CurrentPlayer]==3 && SaucerXBallState[CurrentPlayer]==0) {
  if (DTarget4Goal[CurrentPlayer]==true && DTarget3Goal[CurrentPlayer]==true && DTargetInlineGoal[CurrentPlayer]==true) {
    RPU_SetLampState(LA_SAUCER_XBALL, 1);
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
  RPU_SetLampState(LA_BONUS_MINI_1K+(number-1), state, dim, flashPeriod);
}


// ########## SET SUPER BONUS INDICATOR ##########
void SetSuperBonusIndicator(byte number, byte state, byte dim, int flashPeriod=0) {
  if (number==0 || number>19) return;
  RPU_SetLampState(LA_BONUS_SUPER_1K+(number-1), state, dim, flashPeriod);
}


// ########## SHOW MINI BONUS ON LADDER ##########
void ShowMiniBonusOnLadder(byte bonus) {
  if (bonus > MAX_MINI_DISPLAY_BONUS) bonus = MAX_MINI_DISPLAY_BONUS;

  for (byte count = 0; count < 10; ++count)
    RPU_SetLampState(LA_BONUS_MINI_1K + count, count < bonus, 0, 0);

  if (bonus > 10) RPU_SetLampState(LA_BONUS_MINI_1K + (bonus - 11), 0, 0, 0);
}


// ########## SHOW SUPER BONUS ON LADDER ##########
void ShowSuperBonusOnLadder(byte bonus) {
  if (bonus > MAX_SUPER_DISPLAY_BONUS) bonus = MAX_SUPER_DISPLAY_BONUS;
  
  for (byte count = 0; count < 10; ++count)
    RPU_SetLampState(LA_BONUS_SUPER_1K + count, count < bonus, 0, 0);

  if (bonus > 10) RPU_SetLampState(LA_BONUS_SUPER_1K + (bonus - 11), 0, 0, 0);
}


// ########## SHOW EXTRA BONUS LIGHTS ##########
void ShowExtraBonusLights() {
  if (Mini50kCollected==true) {
    RPU_SetLampState(LA_BONUS_MINI_50K, 1);
  }
  if (Super100kCollected==true) {
    RPU_SetLampState(LA_BONUS_SUPER_100K, 1);
  }
  if (BonusXState[CurrentPlayer]==2) {
    RPU_SetLampState(LA_BONUS_2X, 1);
  } else if (BonusXState[CurrentPlayer]==3) {
    RPU_SetLampState(LA_BONUS_2X, 0);
    RPU_SetLampState(LA_BONUS_3X, 1);
    RPU_SetLampState(LA_DTARGET_BONUS_4X, 1, 0, 200);
  } else if (BonusXState[CurrentPlayer]==4) {
    RPU_SetLampState(LA_DTARGET_BONUS_4X, 0);
    RPU_SetLampState(LA_BONUS_3X, 0);
    RPU_SetLampState(LA_BONUS_4X, 1);
    RPU_SetLampState(LA_DTARGET_BONUS_5X, 1, 0, 200);
  } else if (BonusXState[CurrentPlayer]==5) {
    RPU_SetLampState(LA_DTARGET_BONUS_5X, 0);
    RPU_SetLampState(LA_BONUS_4X, 0);
    RPU_SetLampState(LA_BONUS_5X, 1);
  }
}


// ########## COUNTDOWN BONUS ##########
int CountdownBonus(boolean curStateChanged) {

  // If this is the first time through the countdown loop
  if (curStateChanged) {
    DeleteQueue(); // Clear out any remaining sounds before countdown bonus
    RPU_SetLampState(LA_BALL_IN_PLAY, 1, 0, 250);

    CountdownMini = MiniBonus;
    CountdownSuper = SuperBonus;
    CountdownBonusX = BonusXState[CurrentPlayer];

    CountdownStartTime = CurrentTime;
    ShowMiniBonusOnLadder(MiniBonus);
    ShowSuperBonusOnLadder(SuperBonus);

    if (Super100kCollected==true) {
      CurrentScores[CurrentPlayer] += 100000;
      RPU_SetLampState(LA_BONUS_SUPER_100K, 0);
    }
    if (Mini50kCollected==true) {
      CurrentScores[CurrentPlayer] += 50000;
      RPU_SetLampState(LA_BONUS_MINI_50K, 0);
    }
    DisplayCDBTime = CDBSpeed;
    LastCountdownReportTime = CountdownStartTime;
    BonusCountDownEndTime = 0xFFFFFFFF;
  }

  BackglassLampsLeft2Right();

  if ((CurrentTime - LastCountdownReportTime) > DisplayCDBTime) {
    DisplayCDBTime = CDBSpeed;

    if (MiniBonus>0) {
      CurrentScores[CurrentPlayer] += 1000;
      MiniBonus -= 1;
      ShowMiniBonusOnLadder(MiniBonus);
      PlaySound(12); // Ascending sound
      if (MiniBonus == 0) DisplayCDBTime = 2 * CDBSpeed;
      if (BonusXState[CurrentPlayer]>1 && MiniBonus==0) {
        ShowMiniBonusOnLadder(MiniBonus = CountdownMini);
        BonusXState[CurrentPlayer] -= 1;
      }
      if ((BonusXState[CurrentPlayer]==1) && (MiniBonus==0)) BonusXState[CurrentPlayer] = CountdownBonusX;
    } else if (SuperBonus>0) {
      CurrentScores[CurrentPlayer] += 1000;
      SuperBonus -= 1;
      ShowSuperBonusOnLadder(SuperBonus);
      PlaySound(12); // Other ascending sound
      if (SuperBonus == 0) DisplayCDBTime = 2 * CDBSpeed;
      if (BonusXState[CurrentPlayer]>1 && SuperBonus==0) {
        ShowSuperBonusOnLadder(SuperBonus = CountdownSuper);
        BonusXState[CurrentPlayer] -= 1;
      }
      if ((BonusXState[CurrentPlayer]==1) && (SuperBonus==0)) BonusXState[CurrentPlayer] = CountdownBonusX;
    } else if (BonusCountDownEndTime==0xFFFFFFFF) {
      BonusCountDownEndTime = CurrentTime+1000;
      RPU_SetLampState(LA_BONUS_MINI_1K, 0);
      RPU_SetLampState(LA_BONUS_SUPER_1K, 0);
    }
    LastCountdownReportTime = CurrentTime;
    RPU_SetDisplay(CurrentPlayer, DisplayValue = CurrentScores[CurrentPlayer], true, 2);
  }

  if (CurrentTime>BonusCountDownEndTime) { 
    BonusCountDownEndTime = 0xFFFFFFFF;
    CDBSpeed = 100;
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
    MatchDelay = 150;
    NumMatchSpins = 0;
    RPU_SetLampState(LA_MATCH, 1, 0);
    RPU_SetDisableFlippers();
    ScoreMatches = 0;
    RPU_SetLampState(LA_BALL_IN_PLAY, 0);

    #ifndef USE_EXTENDED_SOUNDS
    PlaySound(28, MatchDelay); // Match sound
    #else
    PlaySound(30, MatchDelay); // Match sound
    #endif
  }
  
  // Simulate spin, select match digits
  if (NumMatchSpins < 21) {
    if (CurrentTime > (MatchSequenceStartTime + MatchDelay)) {
      NumMatchSpins += 1;
      if (NumMatchSpins == 21) {
        MatchDelay += 1000;
        MatchDigit = 10 * random(0, 10);
      }
      else {
        MatchDigit = random(0,100);
        MatchDelay += 78 + 39 * (1 + 3 * max(0, NumMatchSpins - 16)); // should end after 21 spins, at end of 3.6 second sound file
      }
      RPU_SetDisplayBallInPlay((int)MatchDigit);
      RPU_SetLampState(LA_MATCH, NumMatchSpins % 2, 0);
    }
  }

  // Determine which scores match
  if (NumMatchSpins>=21 && NumMatchSpins<=24) {
    if (CurrentTime>(MatchSequenceStartTime + MatchDelay)) {
      NumMatchSpins += 1;
      if ((CurrentNumPlayers > (NumMatchSpins - 22)) && (CurrentScores[NumMatchSpins-22] % 100) == MatchDigit) {
        ScoreMatches |= (1<<(NumMatchSpins-22));
        AddCredit(0, CreditsperCoin[0], CoinsperCredit[0], 1);
        TotalReplays += 1;
        RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, TotalReplays);
        My_PushToTimedSolenoidStack(SO_KNOCKER, 3, CurrentTime, true);
        MatchDelay += 1000;
        RPU_SetLampState(LA_MATCH, 1);
      }
    }      
  }

  // Play end sound
  if (NumMatchSpins==25) {
    if (CurrentTime>(MatchSequenceStartTime + MatchDelay)) {    
      NumMatchSpins += 1;
      MatchDelay += (ScoreMatches == 0 ? 0 : 4000);
      #ifdef USE_EXTENDED_SOUNDS 
        if (PersonalGoalOn) {
          if (MingWasDefeated)
            PlaySound(46); // You've saved your Earth. Have a nice day!
          else {
            unsigned long highestScore = 0;
            for (byte count=0; count<CurrentNumPlayers; count++) {
                if (CurrentScores[count]>highestScore) highestScore = CurrentScores[count];
                }
            if (highestScore < PersonalGoal)
              PlaySound(kidMode ? 39 : 95); // Oh dear, how pathetic
            else
              PlaySound(kidMode ? 39 : 94); // Oh well, who wants to live forever?
            }
          } else
          PlaySound(kidMode ? 39 : 46); // No personal goal. Use regular end sound
      #else
        PlaySound(90);  // sound off
        PlaySound(46); // Try again, Earthling!
      #endif
    }
  }

  // End of game, return to attract mode
  if (NumMatchSpins == 26) {
    if (CurrentTime>(MatchSequenceStartTime + MatchDelay)) {
      for (byte count=0; count<4; count++) {
        if ((ScoreMatches>>count)&0x01) // If this score matches, put back the last two digits
          RPU_SetDisplayBlank(count, RPU_GetDisplayBlank(count) | 0x60); // 0x30 110000 // 0x60 1100000
      }
      // Delay attract sound 10 seconds to avoid having it clobbered by end sound
      AttractWait = 10000;
      return MACHINE_STATE_ATTRACT;
    }
  }    

  // Flash last two digits of all scores as they match the selected value
  for (byte count=0; count<4; count++) {
    if ((ScoreMatches>>count)&0x01) {
      // If this score matches, we're going to flash the last two digits
      if ( (CurrentTime/200)%2 ) {
        RPU_SetDisplayBlank(count, RPU_GetDisplayBlank(count) & 0x1F); // 0x0F 1111 // 0x1F 11111 // 0x7f 1111111
      } else {
        RPU_SetDisplayBlank(count, RPU_GetDisplayBlank(count) | 0x60); // 0x30 110000 // 0x60 1100000
      }
    }
  }

  return MACHINE_STATE_MATCH_MODE;
}



// #################### WIZARD MODE ####################
int WizardMode(boolean curStateChanged) {
  int returnState = MACHINE_STATE_WIZARD_MODE;
  RPU_SetDisplayCredits(MingAttackProgress, true);

  if (curStateChanged) {
    InitWizardModeAnimation = CurrentTime+1770;
    TotalWizardModes++;
    RPU_WriteULToEEProm(RPU_TOTAL_WIZ_EEPROM_BYTE, TotalWizardModes);
    BallDrained = false;
    WizardState[CurrentPlayer] = 3;
    PFValidated = false;
    AddToScore(50000);
    RPU_TurnOffAllLamps();
    if (Credits > 0) RPU_SetLampState(LA_CREDIT_INDICATOR, 1); // Credit light stays on
    MingHealthLights(MingHealth = MingMax); // set drop target lights to indicate Ming's health
    My_PushToTimedSolenoidStack(SO_DTARGET_1_DOWN, 15, CurrentTime);
    RPU_SetLampState(LA_OUTLANE_RIGHT_SPECIAL, 1);
    RPU_SetLampState(LA_OUTLANE_LEFT_SPECIAL, 1);
    OutLanesLit[CurrentPlayer] = true;
    OutLanesCollected[CurrentPlayer] = false;
    RPU_SetLampState(LA_POP_TOP, 1);
    RPU_SetLampState(LA_SPINNER_LEFT, 1);
    RPU_SetLampState(LA_SPINNER_RIGHT, 1);
    RPU_SetLampState(LA_MING_TOP, 1, 0, 100);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
    if (PlayerShootsAgain==true) {
      RPU_SetLampState(LA_SAME_PLAYER_SHOOTS_AGAIN, 1);
      RPU_SetLampState(LA_SHOOT_AGAIN, 1);
    }
    My_PushToTimedSolenoidStack(SO_DTARGET_4_RESET, 15, CurrentTime);
    My_PushToTimedSolenoidStack(SO_DTARGET_3_RESET, 15, CurrentTime + 250);
    My_PushToTimedSolenoidStack(SO_DTARGET_INLINE_RESET, 15, CurrentTime + 500);

    // My_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
    DeleteQueue();
    PlaySound(90); // stop all sounds
    #ifndef USE_EXTENDED_SOUNDS 
    if (backgroundOn) PlaySound(6, 100); // background sound 1
    PlaySound(48, 150 + (WizSource ? 2500 : 1000)); // wizard sound: miserable earthling
    #else
    if (backgroundOn) PlaySound(83, 100); // Final Battle background music
    PlaySound(43, 150 + (WizSource ? 2500 : 1000)); // The attack has begun! Delayed longer if outhole kicker used.
    #endif
  }

  // validate wizard mode
  if (PFValidated && WizardState[CurrentPlayer] < 4) {
    WizardState[CurrentPlayer] = 4;
    CountingDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true, 2, true);
  }
  if (WizardState[CurrentPlayer]<4) {
    CountingFlashDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], CurrentTime, 250, 2);
    BackglassLampsLeft2Right();
  } else {  
    CountingDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true, 2);
  }

  // handle lamps // commented numbers based on 140 ming attack
  if (WizardState[CurrentPlayer]==5) {
    if (MingAttackProgress==0 || MingAttackProgress<=WizardGoalAttack/4-1) { // 0-34
      RPU_SetLampState(LA_FLASH_GORDON_5, 0);
      RPU_SetLampState(LA_FLASH_GORDON_2, 0);
      RPU_SetLampState(LA_FLASH_GORDON_4, 0);
      RPU_SetLampState(LA_FLASH_GORDON_3, 0);
      if (RPU_STROBE_TYPE >= 3) RPU_SetLampState(LA_FLASH_STROBE, 0);

      if (MingAttackProgress==0 || MingAttackProgress<=(WizardGoalAttack/4)/3-1) { // 0-10 : 11
        if (MingHealth!=0) {
          RPU_SetLampState(LA_SAUCER_10K, 1, 0, 250);
          RPU_SetLampState(LA_FLASH_GORDON_6, 1, 0, 250);
          RPU_SetLampState(LA_FLASH_GORDON_1, 1, 0, 250);
        }
      } else if (MingAttackProgress>=(WizardGoalAttack/4)/3 && MingAttackProgress<=((WizardGoalAttack/4)/3)*2-1) { // 11-22 : 12
        RPU_SetLampState(LA_SAUCER_10K, 1, 0, 180);
        RPU_SetLampState(LA_FLASH_GORDON_6, 1, 0, 180);
        RPU_SetLampState(LA_FLASH_GORDON_1, 1, 0, 180);
      } else if (MingAttackProgress>=((WizardGoalAttack/4)/3)*2 && MingAttackProgress<=WizardGoalAttack/4-1) { // 23-34 : 12
        RPU_SetLampState(LA_SAUCER_10K, 1, 0, 110);
        RPU_SetLampState(LA_FLASH_GORDON_6, 1, 0, 110);
        RPU_SetLampState(LA_FLASH_GORDON_1, 1, 0, 110);
      }

    } else if (MingAttackProgress>=WizardGoalAttack/4 && MingAttackProgress<=(WizardGoalAttack/4)*2-1) { // 35-69
      RPU_SetLampState(LA_SAUCER_10K, 1);
      RPU_SetLampState(LA_FLASH_GORDON_6, 1);
      RPU_SetLampState(LA_FLASH_GORDON_1, 1);

      if (MingAttackProgress>=WizardGoalAttack/4 && MingAttackProgress<=(WizardGoalAttack/4)+(((WizardGoalAttack/4)/3)-1)) { // 35-45
        RPU_SetLampState(LA_SAUCER_20K, 1, 0, 250);
        RPU_SetLampState(LA_FLASH_GORDON_5, 1, 0, 250);
        RPU_SetLampState(LA_FLASH_GORDON_2, 1, 0, 250);
      } else if (MingAttackProgress>=(WizardGoalAttack/4)+((WizardGoalAttack/4)/3) && MingAttackProgress<=(WizardGoalAttack/4)+((((WizardGoalAttack/4)/3)*2)-1)) { // 46-57
        RPU_SetLampState(LA_SAUCER_20K, 1, 0, 180);
        RPU_SetLampState(LA_FLASH_GORDON_5, 1, 0, 180);
        RPU_SetLampState(LA_FLASH_GORDON_2, 1, 0, 180);
      } else if (MingAttackProgress>=(WizardGoalAttack/4)+(((WizardGoalAttack/4)/3)*2) && MingAttackProgress<=(((WizardGoalAttack/4)*2)-1)) { // 58-69
        RPU_SetLampState(LA_SAUCER_20K, 1, 0, 110);
        RPU_SetLampState(LA_FLASH_GORDON_5, 1, 0, 110);
        RPU_SetLampState(LA_FLASH_GORDON_2, 1, 0, 110);
      }

    } else if (MingAttackProgress>=(WizardGoalAttack/4)*2 && MingAttackProgress<=(WizardGoalAttack/4)*3-1) { // 70-104
      RPU_SetLampState(LA_SAUCER_10K, 1);
      RPU_SetLampState(LA_SAUCER_20K, 1);
      RPU_SetLampState(LA_FLASH_GORDON_5, 1);
      RPU_SetLampState(LA_FLASH_GORDON_2, 1);

      if (MingAttackProgress>=(WizardGoalAttack/4)*2 && MingAttackProgress<=((WizardGoalAttack/4)*2)+((WizardGoalAttack/4)/3-1)) { // 70-80
        RPU_SetLampState(LA_SAUCER_30K, 1, 0, 250);
        RPU_SetLampState(LA_FLASH_GORDON_4, 1, 0, 250);
        RPU_SetLampState(LA_FLASH_GORDON_3, 1, 0, 250);
      } else if (MingAttackProgress>=((WizardGoalAttack/4)*2)+((WizardGoalAttack/4)/3) && MingAttackProgress<=((WizardGoalAttack/4)*2)+(((WizardGoalAttack/4)/3)*2-1)) { // 81-92
        RPU_SetLampState(LA_SAUCER_30K, 1, 0, 180);
        RPU_SetLampState(LA_FLASH_GORDON_4, 1, 0, 180);
        RPU_SetLampState(LA_FLASH_GORDON_3, 1, 0, 180);
      } else if (MingAttackProgress<=((WizardGoalAttack/4)*2)+(((WizardGoalAttack/4)/3)*2) && MingAttackProgress<=((WizardGoalAttack/4)*3-1)) { // 91-104
        RPU_SetLampState(LA_SAUCER_30K, 1, 0, 110);
        RPU_SetLampState(LA_FLASH_GORDON_4, 1, 0, 110);
        RPU_SetLampState(LA_FLASH_GORDON_3, 1, 0, 110);
      }

    } else if (MingAttackProgress>=(WizardGoalAttack/4*3) && MingAttackProgress<=((WizardGoalAttack/4)*4-1)) { // 105-139
      RPU_SetLampState(LA_SAUCER_10K, 1);
      RPU_SetLampState(LA_SAUCER_20K, 1);
      RPU_SetLampState(LA_SAUCER_30K, 1);
      RPU_SetLampState(LA_FLASH_GORDON_4, 1);
      RPU_SetLampState(LA_FLASH_GORDON_3, 1);

      if (MingAttackProgress>=(WizardGoalAttack/4)*3 && MingAttackProgress<=((WizardGoalAttack/4)*3)+((WizardGoalAttack/4)/3-1)) { // 105-115
        RPU_SetLampState(LA_SAUCER_XBALL, 1, 0, 250);
        if (RPU_STROBE_TYPE >= 3) RPU_SetLampState(LA_FLASH_STROBE, 1, 0, 250);
      } else if (MingAttackProgress>=((WizardGoalAttack/4)*3)+((WizardGoalAttack/4)/3) && MingAttackProgress<=((WizardGoalAttack/4)*3)+(((WizardGoalAttack/4)/3)*2-1)) { // 116-127
        RPU_SetLampState(LA_SAUCER_XBALL, 1, 0, 180);
        if (RPU_STROBE_TYPE >= 3) RPU_SetLampState(LA_FLASH_STROBE, 1, 0, 180);
      } else if (MingAttackProgress<=((WizardGoalAttack/4)*3)+(((WizardGoalAttack/4)/3)*2) && MingAttackProgress<=((WizardGoalAttack/4)*3)+((WizardGoalAttack/4)-1)) { // 128-139
        RPU_SetLampState(LA_SAUCER_XBALL, 1, 0, 110);
        if (RPU_STROBE_TYPE >= 3) RPU_SetLampState(LA_FLASH_STROBE, 1, 0, 110);
      }

    } else if (MingAttackProgress>=WizardGoalAttack && BallInSaucer==false) { // 140
      MingAttackProgress = WizardGoalAttack;
      // MingAttackReady = true;
      // RPU_SetLampState(LA_SAUCER_XBALL, 1);

      byte lampPhaseSaucer = (CurrentTime/150)%4; // 250
      RPU_SetLampState(LA_SAUCER_XBALL, lampPhaseSaucer==3, lampPhaseSaucer==0, lampPhaseSaucer==0);
      RPU_SetLampState(LA_SAUCER_30K, lampPhaseSaucer==2||lampPhaseSaucer==3, lampPhaseSaucer==3);
      RPU_SetLampState(LA_SAUCER_20K, lampPhaseSaucer==1||lampPhaseSaucer==2, lampPhaseSaucer==2);
      RPU_SetLampState(LA_SAUCER_10K, lampPhaseSaucer<2, lampPhaseSaucer==1);
      if (RPU_STROBE_TYPE >= 3) {
        byte lampPhaseFlash = (CurrentTime/150)%4; // 250
        RPU_SetLampState(LA_FLASH_STROBE, lampPhaseFlash==3, lampPhaseFlash==0, lampPhaseFlash==0);
        RPU_SetLampState(LA_FLASH_GORDON_4, lampPhaseFlash==2||lampPhaseFlash==3, lampPhaseFlash==3);
        RPU_SetLampState(LA_FLASH_GORDON_3, lampPhaseFlash==2||lampPhaseFlash==3, lampPhaseFlash==3);
        RPU_SetLampState(LA_FLASH_GORDON_5, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        RPU_SetLampState(LA_FLASH_GORDON_2, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        RPU_SetLampState(LA_FLASH_GORDON_6, lampPhaseFlash<2, lampPhaseFlash==1);
        RPU_SetLampState(LA_FLASH_GORDON_1, lampPhaseFlash<2, lampPhaseFlash==1);
      } else {
        byte lampPhaseFlash = (CurrentTime/150)%3; // 250
        RPU_SetLampState(LA_FLASH_GORDON_4, lampPhaseFlash==2||lampPhaseFlash==0, lampPhaseFlash==0);
        RPU_SetLampState(LA_FLASH_GORDON_3, lampPhaseFlash==2||lampPhaseFlash==3, lampPhaseFlash==3);
        RPU_SetLampState(LA_FLASH_GORDON_5, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        RPU_SetLampState(LA_FLASH_GORDON_2, lampPhaseFlash==1||lampPhaseFlash==2, lampPhaseFlash==2);
        RPU_SetLampState(LA_FLASH_GORDON_6, lampPhaseFlash<2, lampPhaseFlash==1);
        RPU_SetLampState(LA_FLASH_GORDON_1, lampPhaseFlash<2, lampPhaseFlash==1);
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
          RPU_SetLampState(AttractLampsMingAttack[lightcountdown].lightNumMingAttack, (dist<8), (dist==0/*||dist>5*/)?0:dist/3, (dist>5)?(100+AttractLampsMingAttack[lightcountdown].lightNumMingAttack):0);
        }
        MingAttackLamps += 1;
        if (MingAttackLamps>30) MingAttackLamps = 0;
      }
      if (MingHealth==0 && CurrentTime>=(MingAttackAnimation-825)) {
        RPU_DisableSolenoidStack();
      }
    } else {
      for (byte off=LA_BONUS_MINI_1K; off<=LA_TARGET_LRIGHT_TOP; off++) RPU_SetLampState(off, 0);
      for (byte off=LA_SAUCER_10K; off<=LA_DTARGET_BONUS_5X; off++) RPU_SetLampState(off, 0);
      for (byte off=LA_POP_TOP; off<=LA_STAR_SHOOTER_BOTTOM; off++) RPU_SetLampState(off, 0);
      for (byte off=LA_CLOCK_15_SECONDS_3X; off<=LA_SAUCER_ARROW_2X; off++) RPU_SetLampState(off, 0);
      MingHealthLights(MingHealth);
      RPU_SetLampState(LA_SPINNER_RIGHT, 1);
      RPU_SetLampState(LA_SPINNER_LEFT, 1);
      RPU_SetLampState(LA_POP_TOP, 1);
      if (MingHealth!=0) {
        AddToScore(50000);
        My_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime);
      } else {
        // ming defeated
        AddToScore(150000);
        RPU_SetDisableFlippers(true);
        byte bonusFireworks;
        for (bonusFireworks=0; bonusFireworks<10; bonusFireworks++){
          if ( bonusFireworks % 2 == 0) { // even but odd on pf
            RPU_SetLampState(LA_BONUS_MINI_1K+bonusFireworks, 1, 0, 100);
            RPU_SetLampState(LA_BONUS_SUPER_1K+bonusFireworks, 1, 0, 100);
          } else { // odd even on pf
            RPU_SetLampState(LA_BONUS_MINI_1K+bonusFireworks, 1, 0, 200);
            RPU_SetLampState(LA_BONUS_SUPER_1K+bonusFireworks, 1, 0, 200);
          }
        }
        RPU_SetLampState(LA_BONUS_MINI_50K, 1, 0, 500);
        RPU_SetLampState(LA_BONUS_SUPER_100K, 1, 0, 500);
        RPU_SetLampState(LA_FLASH_GORDON_1, 1, 0, 200);
        RPU_SetLampState(LA_FLASH_GORDON_2, 1, 0, 100);
        RPU_SetLampState(LA_FLASH_GORDON_3, 1, 0, 200);
        RPU_SetLampState(LA_FLASH_GORDON_4, 1, 0, 200);
        RPU_SetLampState(LA_FLASH_GORDON_5, 1, 0, 100);
        RPU_SetLampState(LA_FLASH_GORDON_6, 1, 0, 200);
        if (RPU_STROBE_TYPE >= 3) RPU_SetLampState(LA_FLASH_STROBE, 1, 0, 500);
        TotalWizardModesBeat++;
        RPU_WriteULToEEProm(RPU_TOTAL_WIZ_BEAT_EEPROM_BYTE, TotalWizardModesBeat);
        MingDefeated = true;
      }
      MingAttackLamps = 1;
      MingAttackReady = false;
      MingAttackProgress = 0;
      BallInSaucer = false;
    }
  } else if (BallInSaucer==true && MingAttackReady==false) {
    My_PushToTimedSolenoidStack(SO_SAUCER_DOWN, 5, CurrentTime + 1000, true); // override used, in case where solenoid stack is disabled and the ball falls back into the saucer
    BallInSaucer = false;
  }

  // handle ming health animation
  if (MingHealth>=3) {
    RPU_SetLampState(LA_MING_TOP, 1, 0, 100);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 200);
  } else if (MingHealth==2) {
    RPU_SetLampState(LA_MING_TOP, 1, 0, 200);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 300);
  } else if (MingHealth==1) {
    RPU_SetLampState(LA_MING_TOP, 1, 0, 300);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 400);
  } else if (MingHealth==0) {
    RPU_SetLampState(LA_MING_TOP, 1, 0, 400);
    RPU_SetLampState(LA_MING_BOTTOM, 1, 0, 500);
    RPU_SetLampState(LA_SPINNER_LEFT, 0);
    RPU_SetLampState(LA_SPINNER_RIGHT, 0);
    RPU_SetLampState(LA_POP_TOP, 0);
    WizardState[CurrentPlayer] = 6;
  }

  // handle ming defeat celebration sound
  if (MingDefeated && MingDefeatCelebration) {
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
  if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
    if (BallTimeInTrough==0) {
      BallTimeInTrough = CurrentTime;
    } else {
      RPU_SetLampState(LA_MING_TOP, 0);
      RPU_SetLampState(LA_MING_BOTTOM, 0);
      // Make sure the ball stays on the sensor for at least 
      // 0.5 seconds to be sure that it's not bouncing
      if ((CurrentTime-BallTimeInTrough)>500) { // was 3000
        if (NumTiltWarnings>=MAX_TILT_WARNINGS) {
          returnState = MACHINE_STATE_BALL_OVER;
        } else if (NumTiltWarnings<MAX_TILT_WARNINGS) {
          if (WizardState[CurrentPlayer] == 3 || WizardState[CurrentPlayer] == 4) { // 4 added, in case it's fallen back into the outhole
            if (CurrentTime>=InitWizardModeAnimation) {
              My_PushToTimedSolenoidStack(SO_OUTHOLE, 4, CurrentTime + 100);
              // WizardState[CurrentPlayer] = 4;
              WizardState[CurrentPlayer] = 3; // Changed from 4 to 3, to allow flashing display to indicate player up
              BallTimeInTrough = 0;
            }
            returnState = MACHINE_STATE_WIZARD_MODE;
          } else if (WizardState[CurrentPlayer]>=5) {
            PlaySound(90); // Stop sound
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
int RunExtendedSelfTest(int curState, boolean curStateChanged, unsigned long CurrentTime, byte resetSwitch, byte otherSwitch, byte endSwitch) {
  byte curSwitch = RPU_PullFirstFromSwitchStack();
  int returnState = curState;
  boolean resetDoubleClick = false;
  unsigned short auditNumStartByte = 0;
  unsigned short auditNumStartUL = 0;
  long highNum = 1;
  boolean resetBeingHeld = false;
  
  if (curSwitch==resetSwitch) {
      ExtResetHold = CurrentTime;
      if ((CurrentTime-ExtLastResetPress)<400) {
        resetDoubleClick = true;
        curSwitch = SWITCH_STACK_EMPTY;
      }
      ExtLastResetPress = CurrentTime;
    }

  if (ExtResetHold!=0 && !RPU_ReadSingleSwitchState(resetSwitch)) ExtResetHold = 0;
  if (ExtResetHold!=0 && (CurrentTime-ExtResetHold)>500) resetBeingHeld = true;

  if (curSwitch == endSwitch) return MACHINE_STATE_ATTRACT;

  if (curSwitch==SW_SELF_TEST_SWITCH && (CurrentTime - GetLastSelfTestChangedTime())>500) {         
    returnState -= 1;
    SetLastSelfTestChangedTime(CurrentTime);
    }

  if (curStateChanged) {
    for (int count=0; count<4; count++) {
      RPU_SetDisplay(count, 0);
      RPU_SetDisplayBlank(count, 0x00);        
      }
    RPU_SetDisplayCredits(MACHINE_STATE_TEST_DIP_SWITCHES - curState);
    RPU_SetDisplayBallInPlay(0, false);
    }
    
  if (curState==MACHINE_STATE_TEST_PLAYFIELD_AWARD || curState==MACHINE_STATE_TEST_THRESHOLD_AWARD || curState==MACHINE_STATE_TEST_HIGHSCORE_AWARD) 
    highNum = 3;
  else if (curState==MACHINE_STATE_TEST_BACKGROUND_OFF|| curState==MACHINE_STATE_TEST_FREEPLAY || curState==MACHINE_STATE_TEST_GOAL_OFFON 
                || curState==MACHINE_STATE_TEST_KID_MODE || curState == MACHINE_STATE_TEST_ORIGINAL_RULES) 
    highNum = 1;
  else if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_TARGETS)
    highNum = 25;
  else if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK)
    highNum = 250;
  else
    highNum = 9999999;

  if (curState == MACHINE_STATE_TEST_BALL_SAVE)
    EnterBallSaveData(curSwitch, resetDoubleClick, resetBeingHeld, curStateChanged, CurrentTime, resetSwitch, otherSwitch, endSwitch);
  else if (curState == MACHINE_STATE_TEST_GAIN)
    #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
    EnterGainData(curSwitch, resetDoubleClick, resetBeingHeld, curStateChanged, CurrentTime, resetSwitch, otherSwitch, endSwitch);  
    #else
    returnState = MACHINE_STATE_TEST_TOTAL_SKILL;
    #endif;
  else if (curState==MACHINE_STATE_TEST_TOTAL_SKILL) {
    #if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
    StopAudio();
    #endif
    auditNumStartByte = 0;
    auditNumStartUL = 0;
    if (curStateChanged) {
      skillNum = 0;
      ExtSavedValue = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE);
      RPU_SetDisplayFlash(0, ExtSavedValue, CurrentTime, 250, 2);
      ExtSavedValue = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_2_EEPROM_BYTE);
      RPU_SetDisplay(1, ExtSavedValue, true, 2);
      ExtSavedValue = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_3_EEPROM_BYTE);
      RPU_SetDisplay(2, ExtSavedValue, true, 2);
      }
    ExtSavedValue = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE + 4 * skillNum);
    RPU_SetDisplayFlash(skillNum, ExtSavedValue, CurrentTime, 250, 2);
    if (curSwitch == otherSwitch) {
      RPU_SetDisplay(skillNum, ExtSavedValue, true, 2);
      skillNum = (skillNum + 1) % 3;
      ExtSavedValue = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE + 4 * skillNum);
      RPU_SetDisplayFlash(skillNum, ExtSavedValue, CurrentTime, 250, 2);
      } 
    if (resetDoubleClick) {
      ExtSavedValue = 0;
      RPU_SetDisplayFlash(skillNum, ExtSavedValue, CurrentTime, 250, 2); 
      RPU_WriteULToEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE + 4 * skillNum, ExtSavedValue);
    }
    if (curSwitch == resetSwitch) {
      ExtSavedValue += 1;
      RPU_SetDisplayFlash(skillNum, ExtSavedValue, CurrentTime, 250, 2); 
      RPU_WriteULToEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE + 4 * skillNum, ExtSavedValue);
    }
    if (resetBeingHeld) {
      ExtSavedValue = RPU_ReadULFromEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE + 4 * skillNum);
      ExtSavedValue +=1;
      ExtResetHold = CurrentTime;
      RPU_SetDisplayFlash(skillNum, ExtSavedValue, CurrentTime, 250, 2);  
      RPU_WriteULToEEProm(RPU_TOTAL_SKILL_1_EEPROM_BYTE + 4 * skillNum, ExtSavedValue);
    }
  } else if (curState==MACHINE_STATE_TEST_TOTAL_WIZ) {
    auditNumStartUL = RPU_TOTAL_WIZ_EEPROM_BYTE;
  } else if (curState==MACHINE_STATE_TEST_TOTAL_WIZ_BEAT) {
    auditNumStartUL = RPU_TOTAL_WIZ_BEAT_EEPROM_BYTE;
  } else if (curState==MACHINE_STATE_TEST_PLAYFIELD_AWARD) {
    auditNumStartByte = RPU_PLAYFIELD_AWARD;
  } else if (curState==MACHINE_STATE_TEST_THRESHOLD_AWARD) {
    auditNumStartByte = RPU_THRESHOLD_AWARD;
  } else if (curState==MACHINE_STATE_TEST_BACKGROUND_OFF) {
    auditNumStartByte = RPU_BACKGROUND_OFF;
  } else if (curState==MACHINE_STATE_TEST_HIGHSCORE_AWARD) {
    auditNumStartByte = RPU_HIGHSCORE_AWARD;
  } else if (curState==MACHINE_STATE_TEST_FREEPLAY) {
    auditNumStartByte = RPU_FREEPLAY;
  } else if (curState==MACHINE_STATE_TEST_GOAL_OFFON) {
    auditNumStartByte = RPU_GOAL_OFFON;
  } else if (curState==MACHINE_STATE_TEST_WIZARD_GOAL_TARGETS) {
    auditNumStartByte = RPU_WIZARD_GOAL_TARGETS;
  } else if (curState==MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK) {
    auditNumStartByte = RPU_WIZARD_GOAL_ATTACK;
  } else if (curState==MACHINE_STATE_TEST_KID_MODE) {
    auditNumStartByte = RPU_TEMP_KID_MODE_OFFON;
  } else if (curState==MACHINE_STATE_TEST_ORIGINAL_RULES) {
    auditNumStartByte = RPU_ORIGINAL_RULES;
  }
  else return MACHINE_STATE_ATTRACT;

  if (auditNumStartByte != 0) {
    if (curStateChanged) {
      ExtSavedValue = RPU_ReadByteFromEEProm(auditNumStartByte);
      if (ExtSavedValue > highNum) ExtSavedValue = highNum;
      RPU_SetDisplay(0, ExtSavedValue, true, 2);
    }
    if (curSwitch == resetSwitch) {
      ExtSavedValue += (curState == MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK ? 10 : 1);
      if (ExtSavedValue > highNum) 
        if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_TARGETS)
          ExtSavedValue = 1;
        else if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK)
          ExtSavedValue = 10;
        else
          ExtSavedValue = 0;
      RPU_SetDisplay(0, ExtSavedValue, true, 2);  
      RPU_WriteByteToEEProm(auditNumStartByte, ExtSavedValue);
    }
    if (resetBeingHeld) {
      ExtSavedValue += (curState == MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK ? 10 : 1);
      if (ExtSavedValue > highNum) 
        if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_TARGETS)
          ExtSavedValue = 1;
        else if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK)
          ExtSavedValue = 10;
        else
          ExtSavedValue = 0;
      ExtResetHold = CurrentTime;
      RPU_SetDisplay(0, ExtSavedValue, true, 2);  
      RPU_WriteByteToEEProm(auditNumStartByte, ExtSavedValue);
    }
    if (resetDoubleClick) {
      ExtSavedValue = 0;
      if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_TARGETS) ExtSavedValue =  1;
      if (curState == MACHINE_STATE_TEST_WIZARD_GOAL_ATTACK ) ExtSavedValue = 10;
      RPU_SetDisplay(0, ExtSavedValue, true, 2); 
      RPU_WriteByteToEEProm(auditNumStartByte, ExtSavedValue);
    }
  }

  if (auditNumStartUL != 0) {
    if (curStateChanged) {
      if ((ExtSavedValue = RPU_ReadULFromEEProm(auditNumStartUL)) > highNum) ExtSavedValue = highNum;
      RPU_SetDisplay(0, ExtSavedValue, true, 2);
    }
    if (curSwitch == resetSwitch) {
      ExtSavedValue +=1;
      if (ExtSavedValue > highNum) ExtSavedValue = 0;
      RPU_SetDisplay(0, ExtSavedValue, true, 2);  
      RPU_WriteULToEEProm(auditNumStartUL, ExtSavedValue);
    }
    if (resetBeingHeld) {
      ExtSavedValue +=1;
      if (ExtSavedValue > highNum) ExtSavedValue = 0;
      ExtResetHold = CurrentTime;
      RPU_SetDisplay(0, ExtSavedValue, true, 2);  
      RPU_WriteULToEEProm(auditNumStartUL, ExtSavedValue);
    }
    if (resetDoubleClick) {
      ExtSavedValue = 0;
      RPU_SetDisplay(0, ExtSavedValue, true, 2); 
      RPU_WriteULToEEProm(auditNumStartUL, ExtSavedValue);
    }
  }
  return returnState;
}


// #################### ENTER BALL SAVE DATA USING DISPLAYS ####################
void EnterBallSaveData(byte curSwitch,boolean resetDoubleClick, boolean resetBeingHeld, boolean curStateChanged, 
                                          unsigned long CurrentTime, byte resetSwitch, byte otherSwitch, byte endSwitch) {
  unsigned long highNum[3] = {5, 25000, 25}; // Maximum values

  if (curStateChanged) {
    ExtDispNum = 0;
    ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BALL_SAVES);
    RPU_SetDisplayFlash(0, ExtSavedValue, CurrentTime, 250, 2);
    ExtSavedValue = 100 * RPU_ReadByteFromEEProm(RPU_BALL_SAVE_SCORE);
    RPU_SetDisplay(1, ExtSavedValue, true, 2);
    ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BALL_SAVE_NUM_SECS);
    RPU_SetDisplay(2, ExtSavedValue, true, 2);
    }
  ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BALL_SAVES + ExtDispNum);
  if (ExtDispNum == 1) ExtSavedValue = 100 * ExtSavedValue;
  RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2);
  if (curSwitch == otherSwitch) {
    RPU_SetDisplay(ExtDispNum, ExtSavedValue, true, 2);
    ExtDispNum = (ExtDispNum + 1) % 3;
    ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BALL_SAVES + ExtDispNum);
    if (ExtDispNum == 1) ExtSavedValue = 100 * ExtSavedValue;
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2);
    } 
    if (resetDoubleClick) {
    ExtSavedValue = 0;
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2); 
    RPU_WriteByteToEEProm(RPU_BALL_SAVES + ExtDispNum, ExtSavedValue);
    }
    if (curSwitch == resetSwitch) {
    ExtSavedValue += (ExtDispNum == 1) ? 100 : 1;
    if (ExtSavedValue > highNum[ExtDispNum]) ExtSavedValue = highNum[ExtDispNum];
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2); 
    RPU_WriteByteToEEProm(RPU_BALL_SAVES + ExtDispNum, ExtSavedValue / ((ExtDispNum == 1) ? 100 : 1));
    }
  if (resetBeingHeld) {
    // ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BALL_SAVES + ExtDispNum);
    // if (ExtDispNum == 1) ExtSavedValue = 100 * ExtSavedValue;
    ExtSavedValue += (ExtDispNum == 1) ? 100 : 1;
    if (ExtSavedValue > highNum[ExtDispNum]) ExtSavedValue = highNum[ExtDispNum];
    ExtResetHold = CurrentTime;
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2);  
    RPU_WriteByteToEEProm(RPU_BALL_SAVES + ExtDispNum, ExtSavedValue / ((ExtDispNum == 1) ? 100 : 1));
    }
}

#if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
// #################### ENTER GAIN DATA USING DISPLAYS ####################
void EnterGainData(byte curSwitch, boolean resetDoubleClick, boolean resetBeingHeld, boolean curStateChanged, 
                            unsigned long CurrentTime, byte resetSwitch, byte otherSwitch, byte endSwitch) {
  unsigned long highNum[3] = {100, 100, 100}; // Maximum values

  if (curStateChanged) {
    ExtUpDown = 1;
    ExtDispNum = 0;
    ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BG_GAIN);
    RPU_SetDisplayFlash(0, ExtSavedValue, CurrentTime, 250, 2);
    ExtSavedValue = RPU_ReadByteFromEEProm(RPU_SFX_GAIN);
    RPU_SetDisplay(1, ExtSavedValue, true, 2);
    ExtSavedValue = RPU_ReadByteFromEEProm(RPU_VOICE_GAIN);
    RPU_SetDisplay(2, ExtSavedValue, true, 2);

    StopAudio();
    PlayBackgroundSong(AdjustGTO - 6, bgGain);
  }
  ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BG_GAIN + ExtDispNum);
  RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2);
  if (curSwitch == otherSwitch) {
    ExtUpDown = 1;
    RPU_SetDisplay(ExtDispNum, ExtSavedValue, true, 2);
    ExtDispNum = (ExtDispNum + 1) % 3;
    ExtSavedValue = RPU_ReadByteFromEEProm(RPU_BG_GAIN + ExtDispNum);
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2);

    StopSoundEffect(AdjustGTO - 20);
    StopSoundEffect(AdjustGTO - 43);
    if (ExtDispNum == 1) {
      PlaySoundEffect(AdjustGTO - 20, sfxGain);
      SetTrackLoop(AdjustGTO - 20, true);
    }
    if (ExtDispNum == 2) {
      PlaySoundEffect(AdjustGTO - 43, voiceGain);
      SetTrackLoop(AdjustGTO - 43, true);
    }
  }
  if (resetDoubleClick) {
    ExtUpDown = -ExtUpDown;
    ExtSavedValue += ExtUpDown;
    if (ExtSavedValue < 0) ExtSavedValue = highNum[ExtDispNum];
    if (ExtSavedValue > highNum[ExtDispNum]) ExtSavedValue = 0;
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2); 
    RPU_WriteByteToEEProm(RPU_BG_GAIN + ExtDispNum, ExtSavedValue);
  }
  if (curSwitch == resetSwitch) {
    ExtSavedValue += ExtUpDown;
    if (ExtSavedValue < 0) ExtSavedValue = highNum[ExtDispNum];
    if (ExtSavedValue > highNum[ExtDispNum]) ExtSavedValue = 0;
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2); 
    RPU_WriteByteToEEProm(RPU_BG_GAIN + ExtDispNum, ExtSavedValue);
  }
  if (resetBeingHeld) {
    ExtSavedValue += ExtUpDown;
    if (ExtSavedValue < 0) ExtSavedValue = highNum[ExtDispNum];
    if (ExtSavedValue > highNum[ExtDispNum]) ExtSavedValue = 0;
    ExtResetHold = CurrentTime;
    RPU_SetDisplayFlash(ExtDispNum, ExtSavedValue, CurrentTime, 250, 2);  
    RPU_WriteByteToEEProm(RPU_BG_GAIN + ExtDispNum, ExtSavedValue);
  }
  if (ExtDispNum == 0) {
    AdjBgGain = ExtSavedValue;
    CalculateGain();
    ResetGain(AdjustGTO - 6, bgGain);
  }
  else if (ExtDispNum == 1) {
    AdjSfxGain = ExtSavedValue;
    CalculateGain();
    ResetGain(AdjustGTO - 20, sfxGain);
  }
  else if (ExtDispNum == 2) {
    AdjVoiceGain = ExtSavedValue;
    CalculateGain();
    ResetGain(AdjustGTO - 43, voiceGain);
  }
}

// #################### CALCULATE GAIN FOR BACKGROUND, SFX, AND VOICE ####################
void CalculateGain() {
// Initial gain values set as reasonable values for gain, and do not change
// Adustments are values from 0 - 100 entered by user, with distance from 50 indicating lower or higher gain
// bgGain, sfxGain, and voiceGain are calculated from Initial and Adjustment values. Calculated gains are in the range of -70 to +10.

  if (AdjBgGain < 0) AdjBgGain = 0;
  if (AdjBgGain > 100) AdjBgGain = 100;
  if (AdjSfxGain < 0) AdjSfxGain = 0;
  if (AdjSfxGain > 100) AdjSfxGain = 100;
  if (AdjVoiceGain < 0) AdjVoiceGain = 0;
  if (AdjVoiceGain > 100) AdjVoiceGain = 100;

  bgGain = InitBgGain;
  if (AdjBgGain > 50) bgGain = InitBgGain + (10 - InitBgGain) * (AdjBgGain - 50) / 50.0;
  if (AdjBgGain < 50) bgGain = InitBgGain - (70 + InitBgGain) * (50 - AdjBgGain) / 50.0;

  sfxGain = InitSfxGain;
  if (AdjSfxGain > 50) sfxGain = InitSfxGain + (10 - InitSfxGain) * (AdjSfxGain - 50) / 50.0;
  if (AdjSfxGain < 50) sfxGain = InitSfxGain - (70 + InitSfxGain) * (50 - AdjSfxGain) / 50.0;

  voiceGain = InitVoiceGain;
  if (AdjVoiceGain > 50) voiceGain = InitVoiceGain + (10 - InitVoiceGain) * (AdjVoiceGain - 50) / 50.0;
  if (AdjVoiceGain < 50) voiceGain = InitVoiceGain - (70 + InitVoiceGain) * (50 - AdjVoiceGain) / 50.0;
}
#endif

// #################### RESET ALL VARIABLES FOR A BEGINNER GAME ####################
void KidSettings() { // Set all DIP switches and game settings for beginner mode

  // DIP switches set to easy
  dipSaucerStart10k = true;
  dipSaucerMemoryValues = true;
  dip2X3XArrowMemory = true;

  dipOutlaneSpecialMemory = true;
  dipTopSpecialMemory = true;
  dipXBonus = true;

  dipSideTargetMemory = true;
  dip4DropTargetMemory = true;
  dip123ArrowsMemory = true;

  dipUnlimitedReplays = true;
  dipUnlimitedExtraBalls = true;
  dipBallsPerGame = 5;

  // Game settings in memory set to easy
  HighScore            = 100000;
  ScoreAward1          = 100000;
  ScoreAward2          = 200000;
  ScoreAward3          = 300000; 
  PersonalGoal         = 150000;
  PlayfieldAward       = 2;
  ThresholdAward       = 2;
  WizardGoalTargets    = 7;
  WizardGoalAttack     = 100;
  OriginalRules        = 0;
  ballSaves            = 5;
  ballSaveScore        = 7000;
  ballSaveNumMS        = 20000;

  // Operator game adjustments set to easy
  MingMax = 1;
}

#if defined(RPU_OS_USE_WAV_TRIGGER) || defined(RPU_OS_USE_WAV_TRIGGER_1p3)
////////////////////////////////////////////////////////////////////////////
//
//  WAV Trigger Audio Output functions
//
////////////////////////////////////////////////////////////////////////////

void SetupAudio(int sound = 0) { // Must be run at beginning of program. Will play specified test sound at startup.
  wTrig.start();
  wTrig.stopAllTracks();
  delayMicroseconds(10000);

  StopAudio();
  PlaySound(sound); // Play startup test sound
}

void StopAudio() {
  wTrig.stopAllTracks();
  CurrentBackgroundSong = SOUND_EFFECT_NONE;
  GTOWaitingTime = 0;
}

void StopAllButBG() {
  int curSong = CurrentBackgroundSong;
  StopAudio();
  PlayBackgroundSong(curSong);
}

void PlayBackgroundSong(int songNum, int gain = 100) {

  if (gain == 100) gain = bgGain;

  if (songNum != CurrentBackgroundSong) {
    if (CurrentBackgroundSong != SOUND_EFFECT_NONE) 
      wTrig.trackStop(CurrentBackgroundSong);
    if (songNum != SOUND_EFFECT_NONE) {
      #ifdef RPU_OS_USE_WAV_TRIGGER_1p3
      wTrig.trackPlayPoly(songNum, true); // Ability to lock background sounds started with version 1.3
      #else
      wTrig.trackPlayPoly(songNum);
      #endif
      wTrig.trackLoop(songNum, true);
      wTrig.trackGain(songNum, gain);
    }
    CurrentBackgroundSong = songNum;
  }
}

void SetTrackLoop(int soundNum, boolean enable) {
      wTrig.trackLoop(soundNum, enable);
}

void SetBackgroundSong(int soundNum) {
  CurrentBackgroundSong = soundNum;
}

void StopBackgroundSong() {
  PlayBackgroundSong(SOUND_EFFECT_NONE);
}

void ResumeBackgroundSong() {
  byte curSong = CurrentBackgroundSong;
  CurrentBackgroundSong = SOUND_EFFECT_NONE;
  PlayBackgroundSong(curSong);
}

inline void StopSoundEffect(int soundEffectNum) {
  wTrig.trackStop(soundEffectNum);
}

void PlaySoundEffect(int soundEffectNum, int gain = 100) {
  if (gain == 100) {
    if (SoundData[soundEffectNum % 256][0] == 1) gain = voiceGain;
    else gain = sfxGain;
   }
  wTrig.trackPlayPoly(soundEffectNum);
  wTrig.trackGain(soundEffectNum, gain);
}

void ResetGain(int soundEffectNum, int gain) {
  wTrig.trackGain(soundEffectNum, gain);
}
#endif