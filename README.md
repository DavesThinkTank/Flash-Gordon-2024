# Flash Gordon 2024
## Version 2024.04
## for the Arduino Mega 2560 Rev3

NOTE!: Please check out the latest release. Subsequent releases have additional features.

Re-imagined rules for Bally's 1981 Flash Gordon pinball machine. Based on the Bally/Stern Operating System (BSOS), and implemented by adding a daughter card to the MPU's J5 connector. The card can be built yourself using instructions available online. I did not build mine myself and so cannot really advise you. The link to the original instructions is gone, but copies occasionally pop up online.

A safer option is to purchase a kit, or even a pre-built card. Both are available at:
https://pinside.com/pinball/market/shops/1304-roygbev-pinball/by-game/185-flash-gordon

Note: if this link is out of date, you will have to find a current link yourself. I had to!

### To use this code
* Download this zip file (Code > Download ZIP) or clone the repository to your hard drive.
* Unzip the FG2024 repository and make sure the parent folder is named: FG2024
* Download Arduino’s IDE (Integrated Development Environment). And pay them a few bucks!
* Find FG2024.ino in your FG2024 folder, and open it with the Arduino IDE. Compile and upload to an Arduino Mega 2560 microcontroller.
* Attach the Arduino 2560 microcontroller, as part of the daughter card from above, to the J5 connector of your Flash Gordon pinball's MPU board.

### Operator game adjustments
This section at the top of the FG2024.ino file groups some variables that the operator may want to adjust, including difficulty settings like wizard mode drop target goal, and wizard mode attack power goal.

### Adjustments on first startup
Score award thresholds and other game settings can be set in self-test / audit. Be sure to review these as they may have defaulted to zero.

### How to operate self-test / audit / game settings
- Inner coin door button: Enters self-test / audit mode and advances through sections
- Outer coin door game button: Can be used to control and direct some tests. See the Flash Gordon 2024 manual for a full explanation of the self-tests and game settings available.

### Version History
V2024.04 (2024/06/02, Dave's Think Tank):
- Added DIP switch functions, based on original Bally manual
- Added extensive functionality to the self-tests
- Added game settings based on original Bally settings
- Added game setting for free play
- Optimized sound for a Geeteoh sound board (although should be fine on an original Squawk & Talk)
- Minor modifications to game rules
- Various bug fixes

v1.0.0 (10/21/21):
- Created this alternate version of FG2021 that supports the latest BSOS version and is meant to be use with the revision 3 board (Arduino Mega 2560 Rev3)
- Added audit for how many times the high score has been beat
- Added total replays to self-test / audit
- Added score award thresholds to give credits that are adjustable in machine self-test / audit
- Added current credits that are adjustable in machine self-test / audit
