//======================================= OPERATOR GAME ADJUSTMENTS =======================================

// Go through these every time you compile and upload a new version, to make sure everything is set the way you expect!
// Select the number that represents your system from the list to the right of each variable. Type that number to the 
// right of the variable name.

// SELECTING SOUND BOARD!!! Identify your sound board (define MY_SOUND_BOARD as 0, 1, 2, or 3):
#define MY_SOUND_BOARD 3             // 0 = Original Bally Squawk & Talk
                                     // 1 = Geeteoh
                                     // 2 = older WAV Trigger
                                     // 3 = WAV Trigger, v1.3 or later (Most WAV Triggers purchased since 2016)

// SELECTING STROBE TYPE!!! Identify your strobe type (define MY_STROBE_TYPE as 0, 1, 2, 3, or 4):
#define MY_STROBE_TYPE 3             // 0 = Strobe signal always off
                                     // 1 = Strobe signal always on
                                     // 2 = Original xenon tube strobe
                                     // 3 = Incandescent / LED light(s)
                                     // 4 = Incandescent / LED light(s) with inverted signal

#define MAX_HEALTH 3                 // Number of times Ming must be defeated during Final Battle (maximum 4, usual value is 3).

// Two different skill shots are available. Identify the one you want to play:
#define ALTERNATE_SKILL_SHOT 0       // 0 = Hit one, two, or three shooter lane rollover buttons for 15, 30, or 75 thousand points. 
                                     // 1 = Hit flashing arrow drop target for an additional 30,000 points.

// Tilting the machine is allowed following an unsuccessful ball search! Select or de-select this option:
#define BALL_SEARCH_TILT 1           // 0 = Do not allow tilt as part of ball search
                                     // 1 = Tilt allowed after ball search conducted until next playfield switch hit

#define VERSION_NUMBER 2026.03       // Version number appears in Display #1 / Credit display at start of game
#define ATTRACT_SPEECH_TIMER 300000  // Amount of time between attract mode speech callouts. Usual value is 300000 = 5x60x1000 = 5 minutes.
#define MAX_TILT_WARNINGS 1          // Set to number of tilt warnings allowed before losing ball. MINIMUM = 1!

// Debug mode can be used to display switch numbers or other data during a game, in order to search for problems with your machine:
#define DEBUG_MODE 0                 // 0 = no debug messages. 
                                     // 1 = up to nine switch numbers scrolled through player displays 2, 3, and 4. 
                                     // 2 = display at player 4 the number of calls per second at location of function LoopCount(). 
                                     // 3 = up to nine sounds scrolled through player displays 2, 3, and 4.

//=========================================================================================================

//=================================== MAKE NO CHANGES BEYOND THIS POINT! ==================================

// Use sound board selection to set up sound board

#if (MY_SOUND_BOARD == 0)
  #define RPU_OS_USE_S_AND_T
#elif (MY_SOUND_BOARD == 1)
  #define RPU_OS_USE_S_AND_T
  #define USE_EXTENDED_SOUNDS
#elif (MY_SOUND_BOARD == 2)
  #define RPU_OS_USE_WAV_TRIGGER
  #define USE_EXTENDED_SOUNDS
#else
  #define RPU_OS_USE_WAV_TRIGGER_1p3
  #define USE_EXTENDED_SOUNDS
#endif
