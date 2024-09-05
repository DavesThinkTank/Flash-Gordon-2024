# Flash Gordon 2024
## Version 2024.08
## for the Arduino Mega 2560 Rev3

NOTE!: Please check out the latest release. Subsequent releases have additional features.

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
* Place all files in a folder named: FG2024p08
* Download Arduino’s IDE (Integrated Development Environment). (And pay them a few bucks!)
* Find FG2024p08.ino in your FG2024p08 folder, and open it with the Arduino IDE. Compile and upload to an Arduino Mega 2560 microcontroller.
* Attach the Arduino 2560 microcontroller, as part of the daughter card from above, to the J5 connector of your Flash Gordon pinball's MPU board.
* Unzip the sound files and transfer them to the micro SD card on your Geeteoh, if you have one (if you don't have one, buy one!)

### Operator game adjustments
This section is at the top of the FG2024p08.ino file, and groups some variables that the operator may want to adjust. Note, there are very few of these left as most have been converted to self-test game settings.

### Adjustments on first startup
Score award thresholds and other game settings can be set in self-test / audit (see below). Be sure to review these as they may have defaulted to zero. See the included manual for a complete description of all settings.

### How to operate self-test / audit / game settings
- Inner coin door button: Enters self-test / audit mode and advances through sections
- Outer coin door game button: Can be used to control and direct some tests. See the included file FlashGordon2024-08manual.docx for a full explanation of the self-tests and game settings available.
- Coin 3 inner door switch: Some tests require the use of the right-most coin drop switch to modify or move between values. See the included manual for more information.
- Slam switch: The slam switch is located on the inside of the game door. It can be used to end a self-test session without going through all the tests. See the manual for more information.

### Version History
Version 2024.08 (2024/09/02, by Dave's Think Tank)

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



Version 2024.07 (2024/08/08, Dave's Think Tank):
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



V2024.06 (2024/06/25, Dave's Think Tank):
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



V2024.04 (2024/06/02, Dave's Think Tank):
- Added DIP switch functions, based on original Bally manual
- Added extensive functionality to the self-tests
- Added game settings based on original Bally settings
- Added game setting for free play
- Optimized sound for a Geeteoh sound board (although still works on an original Squawk & Talk)
- Minor modifications to game rules
- Various bug fixes



v1.0.0 (10/21/21):
- Created this alternate version of FG2021 that supports the latest BSOS version and is meant to be use with the revision 3 board (Arduino Mega 2560 Rev3)
- Added audit for how many times the high score has been beat
- Added total replays to self-test / audit
- Added score award thresholds to give credits that are adjustable in machine self-test / audit
- Added current credits that are adjustable in machine self-test / audit