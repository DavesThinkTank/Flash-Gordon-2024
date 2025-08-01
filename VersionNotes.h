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

*/

/**************************************************************************

Version 2024.04 by Dave's Think Tank

Job #1, Documentation of the Project:
- Detailed document created covering all self tests, accounting values, game settings, and DIP switches!

Changes / Additions:
1. Added DIP-switch variables.  Read contents of MPU DIP switches into variables, and reprogrammed code to control the game as per the original Bally manual:
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

2. Many additional features were added to the Self-Test function:
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

3. Other Changes and Additions:
- Version number (2024.04) displayed at beginning of game in display #1 (2024) and credit display (04). This is the date I started working on
  the project. Future changes made by other programmers can use the same convention, with less worry about creating multiple versions with the
  same identifier.
- Removed calls to sound 5 (stop all sounds) when using a Geeteoh sound board. This sound can affect current and following sounds with Geeteoh.
- Changed all extra ball awards to award points if extra ball already awarded, or if maximum extra balls per game already won.
- Changed all free game awards to award points if maximum free games per game already won.
- All sounds checked. Sounds all working with Squawk & Talk as well as Geeteoh.
- All scoring checked. Conforms with descriptions on playfield, and original game to the extent possible.

Modified Rules:
- Was set such that Wood Beast extra ball had to be won before saucer extra ball. This seemed unnecessary and so I removed this restriction.
- After hitting the flashing drop target, then the inner flipper lane, you are given only 2 seconds to hit the opposite spinner! Changed this to five.
- After hitting the flashing drop target, you were given only 12.5 seconds to hit the matching right side target! Changed this to 15 seconds, and
  only for points. That is, if you hit it within 15 seconds you get 50,000 points. If you don't hit it in this time, you will get zero points when
  you hit it, but you do not have to start over again with the drop target.

Bug Fixes:
- Displays were handled sometimes as 6 digit, sometimes as 7, leading to strange results in display test. Fixed.
- Display test, 0000000 was only showing as 00. Fixed.
- Extended self test to include all available tests. Was stopping at award level 3.
- fixed multiple bugs leading to incorrect accounting of credits. The software appears to have been set up for 
  free play only. Fixed credits per coin and coins per credit according to DIP switches and original Bally manual (more or less).
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

/**************************************************************************

Version 2024.06 by Dave's Think Tank

Additions and changes in this version:
- Added Wizard stats to self-test (skill shots, Wizard Modes reached, Wizard Modes beaten). See self-tests 13, 14, and 15.
   (These appear to have been part of the original code, but were not included in the version I started with.)
- Added Wizard drop target goal to self-test game settings. Set to 16 if not yet entered.
- Added Wizard attack goal to self-test game settings. Set to 140 if not yet entered.
- Added more coins-per-credits options, similar to original Bally (still not completely the same). Added ascending "dings" to multiple-coin options.
- Added voice-sound queue, and delayed sounds, to handle voices better with Geeteoh. Identified voices, added sound lengths for all my files.
- Turned on strobe lights. Works incredibly well with LEDs! Probably does not work with original strobe / flash unit. Leave on, so option to use if it works, or disconnect strobe if it doesn't.
- Added scrolling displays during attract mode. 
- Set the intial score to a random value for display purposes during first attract mode.
- Added counting display, counting up to current score by 1000s. Flashing version was also required.

Modified Rules:
- If wizard mode has been earned (selected number of targets / guards down), it can be activated by entering the shooter lane, in addition to the lit outlanes.
- Increased value of spinners from 1xspins to 4xspins for Wizard attack. One point out of 140 gave no incentive to go for spinners.

Bug Fixes:
- If tilted on last ball, game would end without final sound, and background sound would continue.
- 2X and 3X saucer sounds were not working properly. Only one would play (voice, or 15-second alarm). Added 100ms delay to voice, and it works now.
- Sound #44 stopped working! No idea why. It plays nothing for four calls, then sound 254 on the fifth. Copied to #47, changed calls, and it works. Weird, but it works.
  - Geeteoh assumes attract mode until background music begins. #44 was previously the attract sound. Geeteoh reads this, and substitutes its attract sound, #254. 
    Then no sound for next four calls, to have attract sound only every five minutes.
- Ensure pinging sound is played every time during bonus collection.
- Fixed wherever flashing display might end on blank, then not be started again by non-flashing display.
- Any time ball goes through outhole, display should flash to indicate player up. Fixed this in wizard mode.

/**************************************************************************

Version 2024.07 by Dave's Think Tank

Additions and changes in this version:

(I was given access to the Geeteoh software code, and made a few minor additions to it. 
 Hopefully I can get them to incorporate these changes in future releases, as they make 
 no difference to regular use of the board, but also give great latitude for game play 
 to anyone with an Arduino!)

- Used updated Geeteoh code to add different background music at different points in the game. Different music for the Skill Shot, Normal Game Play, and the Final Battle!
- Used updated Geeteoh code to add three different ending sounds. One sound for beating Ming in the Final Battle ("You've saved your Earth!"). A different sound if you 
  beat your set personal goal ("Oh well, who wants to live forever?"), and a third sound if you do not reach your goal ("Oh dear, how pathetic").
- Added personal goal (On/Off) to Self-Test game settings.
- Adjusted timing of sounds, music, and voices ("The attack has begun") at the beginning of the final battle, from either the outhole or the shooter lane.
- Added a number of new sounds. Wood Beast roar, Princess Aura saying, "Oh, Flash".
- General review of sounds, timing of sounds, use of quotes in relation to what is happening in the game.
- Added figure-8 lights to Skill Shot.
- Added debugging tool. Define DEBUG_MODE as 1, and calls to Debugger(num) will display num in display 2, 3, or 4, consecutively. Define as 0 to ignore.
- Modified switch test: Displays number of switches set to "on" in credit display. Double-click of game button resets all drop targets. These changes should facilitate switch-matrix error testing.

Modified Rules:
- Added ball save. If score for one ball is less than BALL_SAVE_SCORE, or time is less than BALL_SAVE_NUM_MS milliseconds, 
  game (including score) is reset to beginning of ball. "Don't kill him yet, Father!"

Bug Fixes:
- Score counting display would sometimes go real fast, when switching from flashing display to non-flashing.
- The ball got trapped once when the inline targets reset! Added two seconds to the pause, to make sure ball has lots of time to get out.
- If ball got to lower level without leaving Skill Shot mode, you could collect points. Then when you entered the outhole it would kick you into the shooter lane without increasing ball number.
*/

/**************************************************************************

Version 2024.08 by Dave's Think Tank

Additions and changes in this version:

- Background music stops during tilt
- 15-second timer uses music from the movie (Football Fight). The 3770 ms "blank" before the timer, during the light show, is filled with voices from the film.
- New voices: "Try the Wood Beast or die" added to inline drop target #3. "This is a lunatic asylum" added when 3x bonus earned. 
- Background music ends when the ball enters the outhole, rather than when skill shot background begins. Helps to distinguish the modes.
- DIP Switch Test: Added a test (following sound test) for DIP switches. Dip switch banks are displayed in binary (7 digits per display, plus one in ball-in-play or credit window.) 
  - DIP switches can be temporarily changed in game, allowing testing of alternative settings. Values return to the hard-coded values from the MPU, the next time the machine is turned on.
- Self-test modification: Added use of slam switch to end self-test. Added otherSwitch as a secondary input to self-tests.
- On defeating Ming, both the quote "You have saved your Earth...", and the original tune (using tones), were played. Only one is now played, based on definition of BALLY_STERN_OS_USE_GEETEOH.

Modified Rules:
- During the Final Battle, Ming's remaining lives are now displayed in the lower drop-target lights.
- Ming's maximum lives can be set to any value from 1 to 4 using the Operator Game Adjustments. Recommended value is 3.

Bug Fixes:

- Self-test would return to attract mode, and would sometimes go straight back into self-test.
- Princess Aura would sometimes say "Don't kill him yet, father" inappropriately at the start of the Skill Shot. Now, only when ball save is used.
- Background sound is ended following sound test.
- Match feature, echo sound was awful. Eliminated echo sound, fixed timing and selection of random numbers, fixed match light to be on when appropriate.

*/

/**************************************************************************

Version 2024.09 by Dave's Think Tank

Additions and changes in this version:

- Added code to kick ball out of saucer, should it be there during attract mode, rather than wait until a game begins.
- Changed bonus circle display for values over ten: all values lit except for units digit of value.
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

*/

/**************************************************************************

Version 2024.10 by Dave's Think Tank

Additions and changes in this version:

- SwitchDebounce[] structure created, where operator can set timing of switch hits to eliminate double hits (see "Bug Fixes" below).
- ResetHits[] structure created, where operator can set timing to avoid solenoid resets from activating switches (see "Bug Fixes" below).
- Debugger() modified to scroll most recent nine switches hit through player 2, 3, and 4 displays. Hold credit button to stop.
- LoopCount() written to determine number of times per second the switches are monitored (results: ~180-200 during game play).
- DEBUG_MESSAGES changed to DEBUG_MODE, to allow more debug options in the future.
- DEBUG_MODE = 2 added to display at player display 4 the number of times per second the switches are monitored (LoopCount() above).

Bug Fixes:

I'm not sure if this counts as a bug! But I was having a fair amount of difficulty in my machine with switches activating due to switch "bounce".  Switches would sometimes  activate 
twice, or activate due to vibration from solenoids firing on the playfield. Cleaning and regapping couldn't solve all the issues. I have since found out that other Arduino  projects
make extensive use of timers to watch for and eliminate this type of switch activity. My solution was to write the data structures mentioned above, SwitchDebounce[] and ResetHits[],
as well as the functions DoubleHitFix(), ResetHitFix(), and My_PushToTimedSolenoidStack(). These five items record the timing of every switch hit and solenoid firing, and ensure the
problem switches are not allowed to activate again within a user-adjustable time period. 

To use these solutions you need to identify a problem switch or solenoid / switch combination. To fix a switch that is double hitting, simply set a "wait" time in the second  column
of SwitchDebounce[] on the row identifying the switch to be fixed. Any time up to 255ms can be entered. The time selected must be long enough to cover the expected gap between hits,
and short enough to allow for any possible way that the switch could legitimately be hit twice. I have already added wait times for all drop target switches, as well as two switches
which were giving me trouble; the shooter lane wire rollover and the right outlane.  I've also done the left  outlane and both slingshots. I stopped there,  as there is no reason to
add this fix to switches that are not malfunctioning.

To fix a solenoid that is activating a switch,  set a "wait" time in the  second column of ResetHits[] on the row identifying the solenoid that is causing a problem.  Any time up to
255ms can be entered. Now you need to identify the switch that is being incorrectly activated. This is done by  adding code to the function ResetHitFix().  There are already several
examples included to copy from.  Simply set up a case statement for the switch, and "return true" if the current time less the start time of the solenoid  is less that the wait time
for the solenoid. I have already added code to cover all drop target switches being activated by their own drop target reset solenoid.

*/

/**************************************************************************

Version 2024.11 by Dave's Think Tank

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

*/

/**************************************************************************

Version 2024.12 by Dave's Think Tank

Additions and changes in this version:

- When the solenoid test identifies a switch set off by vibration, it will also note the time in milliseconds between the solenoid firing and the switch activating.
- When you defeat Ming and are informed, "You've saved your Earth", you will also receive this same message again when the game ends, no matter what your 
    score (PersonalGoalOn must be set to true)
- Placed a break between multiple bonus countdowns, and between mini and super bonus countdowns, at end of ball.

Bug Fixes:

- Fixed Debugger() to continue game after hitting the game credit button, but discontinue displaying switches for as long as the button is held.
- If skill shot ended by hitting something on playfield, bonus points would not show until another bonus target was hit. Fixed.
- "You've saved your Earth" was not always playing when Ming was defeated. Added a sound stop code, and 10 ms delay. Seems to work now.
- Cleaned up old unused code. Removed old CPC (coins per credit) code from SelfTestAndAudit, replaced long ago with DIP switch controls.

*/

/**************************************************************************

Version 2025.01 by Dave's Think Tank

Additions and changes in this version:

- New double-hit switch bounce test added, in addition to the stuck switch / switch matrix test.
- I thought, "Hey, shouldn't kids be able to put the machine into Kid's Mode?" So you can now access Kid's Mode by pressing the game button and coin 3
    switch at the same time (hold until sound plays). Requires an extension of the coin 3 switch on the outside of the machine. I have a switch added, 
    unobtrusively inside the coin return.
- Created separate file for version notes. Notes were getting too long to scroll through all the time!
- Converted from BSOS (Bally/Stern Operating System) to RPU (Retro Pin Upgrade). RPU is an extension of BSOS. BSOS is no longer maintained.
- Moved definition of RPU_OS_USE_GEETEOH to Operator Game Adjustments in primary file, rather than RPU configuration file. Made required software changes.

Bug Fixes:

- Sound does not always stop when ball in outhole. Moved the stop-sound code so it is not clobbered immediately by the countdown bonus sounds.
- RPU_ReadByteFromEEProm sets value to zero if it equals 255! Removed, allowing byte = 255.
- Coin counts were limited to 255 per slot, even though saved in unsigned long variables. Increased to 4 billion.
- Solenoid / Switch test would "detect" self test switch hit on first solenoid. Fixed.

*/
/**************************************************************************

Version 2025.03 by Dave's Think Tank

Additions and changes in this version:

- Sometimes the Geeteoh seems to allow two background sounds to play at the same time. I'm not sure if this is just my machine! But I've added an option to define a variable STOP_THE_MUSIC equal to 1, which
    causes the game button to turn off the background music temporarily during a game. Note that, once a game starts, you cannot add another player with this option. Setting it to zero results in normal play.
- Switch bounce Test: clear time in player 2 display if hit time exceeds 500 ms.

Bug Fixes:

- High score light stayed on when player score displayed during attract mode (before first game only)
- Saucer-down solenoid stayed on for 1/2 second when kicking ball out of saucer during attract mode.
- The random(A, B) function selects a random number between A and B+1, NOT B! Only affects match. Fixed!

*/
/**************************************************************************

Version 2025.07 by Dave's Think Tank

Additions and changes in this version:

Bug Fixes:

- Replaced PlaySound() and PlayQueue() functions with functions written for the Bally Star Trek. These allow up to 20 voices to be stored in the queue, rather than just 1.
    The programming is tighter as well, hopefully leading to a solution for the double-background sounds mentioned in 2025.03 above. 
    Added code to ensure a minimum time allowed for Geeteoh to deal with each sound - currently 25 ms. Hopefully this ensures all sounds and other commands played.
    Will continue to watch - so far no issue.

*/