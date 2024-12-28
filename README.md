# Flash Gordon 2024
## Version 2024.11
## for the Arduino Mega 2560 Rev3

https://github.com/DavesThinkTank/Flash-Gordon-2024/releases/tag/v2024.11

NOTE!: Please check out the latest release at the link above. Subsequent releases have additional features. The program will display the version number for four seconds when the pinball is turned on, in the Player 1 display (2024) and the Credit window (11). If it doesn't, you have the wrong software or the wrong version!

Re-imagined rules for Bally's 1981 Flash Gordon pinball machine. Based on the Bally/Stern Operating System (BSOS), and implemented by adding a daughter card to the MPU's J5 connector. The card can be built yourself using instructions available online. I did not build mine myself and so cannot really advise you. The link to the original instructions is gone, but copies occasionally pop up online.

A safer option is to purchase a kit, or even a pre-built card. Both are available at:
https://pinside.com/pinball/market/shops/1304-roygbev-pinball/by-game/185-flash-gordon   (Be sure to ask for a MEGA 2560 REV3!!!)

Note: if this link is out of date, you will have to find a current link yourself. I had to!

### Important Notes!
* This version is meant to be used with a Geeteoh sound board. If you are using an original Bally Squawk & Talk, or some board compatible with it, the sounds should work for the most part, but there may be some minor issues with sounds not playing under some circumstances. The best way to handle this is to comment out line 35 (add // at the beginning) in file BSOS_Config.h as follows:

// #define BALLY_STERN_OS_USE_GEETEOH 

Then re-compile the code to your Arduino. The very best way to handle this is to just buy a Geeteoh board already!

Even if you have a Geeteoh board, you may need the latest software update. This can be obtained at a reasonable price by contacting Geeteoh.com.

* The sound files included are meant to be used in Geeteoh sound bank 6, but can be renamed for any other bank if needed. See your Geeteoh manual.
* The sound files do not include any of the music from the movie, for copyright reasons. I used "In the Space Capsule" during the Skill Shot, "Flash's Theme" for regular game play, and "Battle Theme" during the final battle. All three are available from any music streaming service. I use instrumental versions, or edited out much of the dialog and sound effects, as the voices in the full songs tend to disrupt voices in the game. Just go to Google and search for "Flash Gordon Theme Instrumental" for an instrumental version of "Flash's Theme". There are several online, including several on YouTube. Create a wav file of sound only (no video) and name it 1785_0249_FlashsTheme.wav. You will also want to create two music files, 1707_0171_SpaceCapsule.wav and 1708_0172_BattleTheme.wav.

* This version has the strobe light turned on. If you have no strobe it will make no difference to you. If you have an LED strobe you will be pleasantly surprised. If you have an original strobe, or an exact replacement, I don't know what will happen but it probably won't work well. You can recompile with USE_STROBE defined as 0 (see "Operator game adjustments" below), or just unplug the strobe. Or watch my video on how to build an LED strobe out of spare parts you probably have lying around the house:

https://youtu.be/VCkcB5TzOqM?si=1H3Oql_PQe3d2cmk . Or write some amazing code to handle an original strobe and let me know!

### To use this code

* Under "Releases", click on "Latest".
* Click on each file to download. 
* Place all files in a folder named: FG2024p11
* Download Arduino’s IDE (Integrated Development Environment). (And pay them a few bucks!)
* Find FG2024p11.ino in your FG2024p11 folder, and open it with the Arduino IDE. Compile and upload to an Arduino Mega 2560 microcontroller.
* Attach the Arduino 2560 microcontroller, as part of the daughter card from above, to the J5 connector of your Flash Gordon pinball's MPU board.
* Unzip the sound files and transfer them to the micro SD card on your Geeteoh, if you have one (if you don't have one, buy one!)

### Operator game adjustments
This section is at the top of the FG2024p11.ino file, and groups some variables that the operator may want to adjust. Note, there are very few of these left as most have been converted to self-test game settings.

### Adjustments on first startup
Score award thresholds and other game settings can be set in self-test / audit (see below). Be sure to review these as they may have defaulted to zero. See the included manual for a complete description of all settings.

### How to operate self-test / audit / game settings
- Inner coin door button: Enters self-test / audit mode and advances through sections
- Outer coin door game button: Can be used to control and direct some tests. See the included file FlashGordon2024-11manual.docx for a full explanation of the self-tests and game settings available.
- Coin 3 inner door switch: Some tests require the use of the right-most coin drop switch to modify or move between values. See the included manual for more information.
- Slam switch: The slam switch is located on the inside of the game door. It can be used to end a self-test session without going through all the tests. See the manual for more information.

### Version History
### Version 2024.11 by Dave's Think Tank

Additions and changes in this version:

- Solenoid test modified to show any switches activated by vibration from firing solenoids.

Modified Rules:

- The original rule was that the X2 and X3 bonuses could only be won once per ball. By setting self-test entry #25 (OriginalRules) to 1, this rule is
    maintained. Or, by setting it to 0, the X2 and X3 bonuses will be reset after the 15-second timer is complete, to be won all over again.
- Self-test #25 (Original Rules) also affects the collection of mini-bonus at the upper-level target, and super-bonus in the shooter lane. Under original rules,
    after these bonuses were collected, they were set to zero and had to be earned again. Under new rules these bonuses can be collected repeatedly, until
    finally collected and reset to zero in the outhole.
- In the 2021 version, if you hit the left-side targets but missed the 15-seconds to hit the lower-level, right-side targets for 50,000 points, you had to 
    hit the left-side targets all over again. I thought this was overly punitive, but instead of just removing it, I compromised by saying you would get
    zero points when you ultimately did hit the right targets. But then I thought, "Why am I compromising? It's my game now!". So under these circumstances,
    if you hit the right side target after 15 seconds, missing the 50,000 points, you will still get 10,000 points.
- Standard number of wizard goal targets changed from 16 to 11.

Bug Fixes:

- Up/down kicker delayed 500ms when activated during attract mode, to give ball time to settle.
- Match has a four-second delay at the end to allow matching digits to flash. But if there is no match, it was still waiting four seconds to enter attract mode. Delay
    removed if there is no match.
- Skill Shot mode ends when you hit something on the upper playfield. Why not the lower playfield? I always wondered if this was a bug or a feature, as it
    seemed deliberate. However, I've now decided, it is a bug. Skill Shot mode will now end when you hit any switch on the playfield, upper or lower level.
- if you add a player and the number of credits decreases below the maximum credits, the coin lockout mechanism is now turned off to allow you to spend more money.
- Two BSOS solenoid functions are used in the program, but only one was included in the debounce changes for the previous version. both are included now.
- The new solenoid / switch test indicated the 4-drop-target bank was setting off the wood beast switch. Used Debounce structures and ResetHitFix() to resolve.
- A longstanding issue, where leaving self-test using the self-test button would sometimes take you straight back into self-test, has been resolved.
- Player 1 would be blank for a couple seconds at beginning of game. fixed.


### Version 2024.10 (2024/11/06, by Dave's Think Tank)

Additions and changes in this version:

- SwitchDebounce[] structure created, where operator can set timing of switch hits to eliminate double hits (see "Bug Fixes" below).
- ResetHits[] structure created, where operator can set timing to avoid solenoid resets from activating switches (see "Bug Fixes" below).
- Debugger() modified to scroll most recent nine switches hit through player 2, 3, and 4 displays. Hold credit button to stop.
- LoopCount() written to determine number of times per second the switches are monitored (results: ~180-200 during game play).
- DEBUG_MESSAGES changed to DEBUG_MODE, to allow more debug options in the future.
- DEBUG_MODE = 2 added to display at player display 4 the number of times per second the switches are monitored (LoopCount() above).

Bug Fixes:

I'm not sure if this counts as a bug! But I was having a fair amount of difficulty in my machine with switches activating due to switch "bounce". Switches would sometimes activate twice, or activate due to vibration from solenoids firing on the playfield. Cleaning and re-gapping couldn't solve all the issues. I have since found out that other Arduino projects make extensive use of timers to watch for and eliminate this type of switch activity. My solution was to write the data structures mentioned above, SwitchDebounce[] and ResetHits[], as well as the functions DoubleHitFix(), ResetHitFix(), and My_PushToTimedSolenoidStack(). These five items record the timing of every switch hit and solenoid firing, and ensure the problem switches are not allowed to activate again within a user-adjustable time period. 

To use these solutions you need to identify a problem switch or solenoid / switch combination. To fix a switch that is double hitting, simply set a "wait" time in the second column of SwitchDebounce[] on the row identifying the switch to be fixed. Any time up to 255ms can be entered. The time selected must be long enough to cover the expected gap between hits, and short enough to allow for any possible way that the switch could legitimately be hit twice. I have already added wait times for all drop target switches, as well as two switches which were giving me trouble; the shooter lane wire rollover and the right outlane. I've also done the left outlane and both slingshots. I stopped there, as there is no reason to add this fix to switches that are not malfunctioning.

To fix a solenoid that is activating a switch, set a "wait" time in the second column of ResetHits[] on the row identifying the solenoid that is causing a problem.Any time up to 255ms can be entered. Now you need to identify the switch that is being incorrectly activated. This is done by adding code to the function ResetHitFix().There are already several examples included to copy from.Simply set up a case statement for the switch, and "return true" if the current time less the start time of the solenoid is less that the wait time for the solenoid. I have already added code to cover all drop target switches being activated by their own drop target reset solenoid.


### Version 2024.09 (2024/10/01, by Dave's Think Tank)

Additions and changes in this version:

- Added code to kick ball out of saucer, should it be there during attract mode, rather than wait until a game begins.
- Changed bonus circle display for values over ten: all values lit except for units digit of value. (Looks better during countdown, seriously).
- Added a kid mode, where all settings are changed to reduce difficulty of the game. See KidSettings() function to modify.
- Kid sounds added at start and end of games in kid mode.
- Added quick way to start and end kid mode. In self test, press game button and coin 3 button at same time. Does not work during switch test!
- Added "Reset Hold" feature to tests for lights, displays, and DIP switches. Reset Hold scrolls quickly through the display / review options.
- Reset Hold option on audit settings sped up considerably.
- Solenoids can be made to stop firing during solenoid test by pressing otherSwitch (Coin slot 3 switch)

Modified Rules:

- If spinner hit within 5 seconds of hitting the lit flipper feed lane you will receive 15000 points, in addition to 2000 per spin. Adds incentive to quickly hit spinners.

Bug Fixes:

- Pressing game button during game and after ball 1 would restart game. It looks like this was intentional, although I can't see why, so I removed it.
- Bug in Geeteoh that would restart background music at the end of 15 seconds, even if the background music had changed, has been fixed. 
    So, temporary fix in this software has been removed.
- Slam switch will no longer end self-test during the switch test. It will simply register as a switch.


### Version 2024.08 (2024/09/02, by Dave's Think Tank)

Additions and changes in this version:

- Background music stops during tilt
- 15-second timer uses music from the movie (Football Fight). The 3770 ms "blank" before the timer, during the light show, is filled with voices from the film.
- New voices: "Try the Wood Beast or die" added to inline drop target #3. "This is a lunatic asylum" added when 3x bonus earned. 
- Background music ends when the ball enters the outhole, rather than when skill shot background begins. Helps to distinguish the modes.
- DIP Switch Test: Added a test (following sound test) for DIP switches. Dip switch banks are displayed in binary (7 digits per display, plus one in ball-in-play or credit window.) 
  - DIP switches can be temporarily changed in game, allowing testing of alternative settings. Values return to the hard-coded values from the MPU, the next time the machine is turned on.
- Self-test modification: Added use of slam switch to end self-test.
- On defeating Ming, both the quote "You have saved your Earth...", and the original tune (using tones), were played. Only one is now played, based on definition of BALLY_STERN_OS_USE_GEETEOH.

Modified Rules:
- During the Final Battle, Ming's remaining lives are now displayed in the lower drop-target lights.
- Ming's maximum lives can be set to any value from 1 to 4 using the Operator Game Adjustments. Recommended value is 3.

Bug Fixes:

- Self-test would return to attract mode, and would sometimes go straight back into self-test.
- Princess Aura would sometimes say "Don't kill him yet, father" inappropriately at the start of the Skill Shot. Now, only when ball save is used.
- Background sound is ended following sound test.
- Match feature, echo sound was awful. Eliminated echo sound, fixed timing and selection of random numbers, fixed match light to be on when appropriate.



### Version 2024.07 (2024/08/08, by Dave's Think Tank):
Additions and changes in this version:

(I was given access to the Geeteoh software code, and made a few minor additions to it. These changes make no difference to regular use of a Geeteoh board, but extend its capabilities for use with an Arduino. Geeteoh plans to include the changes in future releases. If you already have a Geeteoh, contact geeteoh.com. He will likely be willing to sell you a replacement chip which will make your current board completely compatible with this software.)

- Used updated Geeteoh code to add different background music at different points in the game. Different music for the Skill Shot, Normal Game Play, and the Final Battle!
- Used updated Geeteoh code to add three different ending sounds. One sound for beating Ming in the Final Battle ("You've saved your Earth!"). A different sound if you beat your set personal goal ("Oh well, who wants to live forever?"), and a third sound if you do not reach your goal ("Oh dear, how pathetic").
- Added personal goal (On/Off) to Self-Test game settings.
- Adjusted timing of sounds, music, and voices ("The attack has begun") at the beginning of the final battle, from either the outhole or the shooter lane.
- Added a number of new sounds. Wood Beast roar, Princess Aura saying, "Oh, Flash".
- General review of sounds, timing of sounds, use of quotes in relation to what is happening in the game.
- Added figure-8 lights to Skill Shot.
- Added debugging tool. Define DEBUG_MESSAGES as 1, and calls to Debugger(num) will display num in display 2, 3, or 4, consecutively. Define as 0 to ignore.
- Modified switch test: Displays number of switches set to "on" in credit display. Double-click of game button resets all drop targets. These changes should facilitate switch-matrix error testing.

Modified Rules:
- Added ball save. If score for one ball is less than 1500, or time is less than 15000 milliseconds, game (including score) is reset to beginning of ball. "Don't kill him yet, Father!"
- Ball save amount and time, and number of ball saves per game, can be set in Operator Game Adjustments (see above).

Bug Fixes:
- Score counting display would sometimes go real fast, when switching from flashing display to non-flashing.
- The ball got trapped once when the inline targets reset! Added two seconds to the pause, to make sure ball has lots of time to get out.
- If ball got to lower level without leaving Skill Shot mode, you could collect points. Then when you entered the outhole it would kick you into the shooter lane without increasing ball number.



### V2024.06 (2024/06/25, by Dave's Think Tank):
Additions and changes in this version:

- Added Final Battle stats to self-test (skill shots, Final Battles reached, Final Battles beaten). See self-tests 13, 14, and 15.(These appear to have been part of the original code, but were not included in the version I started with.)
- Added Final Battle drop target goal to self-test game settings. Automatically set to 16 if not yet entered.
- Added Final Battle attack goal to self-test game settings. Automatically set to 140 if not yet entered.
- Added more coins-per-credits options, similar to original Bally (still not completely the same).
- Added voice-sound queue, and delayed sounds, to handle voices better with Geeteoh. 
- Turned on strobe lights. Works incredibly well with LEDs! Probably does not work with original strobe / flash unit.
- Added scrolling displays during attract mode. 
- Added counting display, counting up to current score by 1000s.

Modified Rules:
- If Final Battle has been earned (selected number of targets / guards down), it can be activated by entering the shooter lane, in addition to the lit outlanes.
- Increased value of spinners from 1 x spins to 4 x spins for Final Battle attack. One point out of 140 gave no incentive to go for spinners.

Bug Fixes:

- If tilted on last ball, game would end without final sound, and background sound would continue.
- 2X and 3X saucer sounds were not working properly. Only one would play (voice, or 15-second alarm). Added 100ms delay to voice, and it works now.
- Any time ball goes through outhole, display should flash to indicate player up. Fixed this in Final Battle mode.



### V2024.04 (2024/06/02, by Dave's Think Tank):
Additions and changes in this version:

- Added DIP switch functions, based on original Bally manual
- Added extensive functionality to the self-tests
- Added game settings based on original Bally settings
- Added game setting for free play
- Optimized sound for a Geeteoh sound board (although still works on an original Squawk & Talk)
- Minor modifications to game rules
- Various bug fixes



### v1.0.0 (10/21/21, by Tim Murren):
- Created this alternate version of FG2021 that supports the latest BSOS version and is meant to be use with the revision 3 board (Arduino Mega 2560 Rev3)
- Added audit for how many times the high score has been beat
- Added total replays to self-test / audit
- Added score award thresholds to give credits that are adjustable in machine self-test / audit
- Added current credits that are adjustable in machine self-test / audit
