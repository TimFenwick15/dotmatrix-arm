[![Build Status](https://travis-ci.com/TimFenwick15/dotmatrix-arm.svg?branch=master)](https://travis-ci.com/TimFenwick15/dotmatrix-arm)

Driving an LED dotmatrix with an STM32F411e-Discovery.

Datasheet hosted on RS: https://docs.rs-online.com/9099/0900766b81368481.pdf

## I2C
I2C is used to comunicate with a DS3231 RTC. I'm using the breakout from Adafruit (bought from Pimoroni in UK https://shop.pimoroni.com/products/adafruit-ds3231-precision-rtc-breakout).

Examples to help with I2C setup for STM32 HAL are here: https://github.com/STMicroelectronics/STM32CubeF4/tree/master/Projects/STM32F411E-Discovery/Examples/I2C

Adafruit host a datasheet for the DS3231 RTC: https://cdn-shop.adafruit.com/product-files/3013/DS3231.pdf

This blog was helpful explaining how DS3231 works: https://controllerstech.com/ds3231-rtc-module-with-stm32/

This digikey video was helpful understanding I2C on STM32 HAL: https://www.youtube.com/watch?v=isOekyygpR8

## Unit Testing

### Running tests
To run tests (just Windows for now...):

Open a Command Prompt inside the project directory, then run:

$ RunTests

This requires you have gcc on your path. On Windows, you can get this by downloading MINGW.

I couldn't get this to work as a build target in Eclipse, but this is presumably possible. For now, you can right-click RunTest.bat and click "Show in", then "Terminal". In the terminal, type "RunTests"

### Unity
Trying unit testing with Unity because it seems to be the easiest to set up.

Their approach is:
- Write a test source file that includes a main function
- Compile this to run on the host PC
- Run the executable to run the test script

Or:
- Write a test source file without a main function
- Run an included Ruby script to generate a test runner for that file
- Compile the test runner file - this will have a main function
- Run the executable as before

So to add a test, you would write a test file, update the build command (I'm using a batch script, make is a better option), run all executables to run the test suite.

Requirements:
- Ruby
- GCC (MinGW on Windows) on your path

You could do a similar thing without using the supplied Ruby script, by making each test script expose a run function, and writing a TestRunner source file that runs each of these.

Then you can compile a single executable with a single build command, and run your whole test suite at once. For a small project like this, this seems easier to maintain. For a larger project, this could lead to longer build times.

The process for adding a test becomes: write the test script, add the run function to the TestRunner, make sure the source file under test is included in the build command, then run the executable.

A disadvantage of doing this is the results show tests as coming from the test runner file rather than the test script. The line numbers refer to the test scripts, and the function names tell you where to look.

Requirements:
- GCC (MinGW on Windows) on your path

I've chosen the second method because you don't need to maintain a make file, it doesn't depend on external tools, the batch file to run this is simple, and building everything is fast in a project of this size so I don't need to worry about running tests on a portion of my code base.

### Files that can be unit tested
- Animation.c: yes - contains several functions that can be unit tested, has a static function that it would be useful to have tests for
- GPIO.c: no - when built statically, you get SEGFAULTS when running the executable, I think because there's a macro to a memory locaiton that doens't make sense on a host PC
- Graphics.c: yes
- LedMatric.c: no - depends on GPIO.c, and contains funcitons that aren't easy to test - eg a definition of an ISR
- Sprite.c: yes - although the tests are trivial

### Unit testing static functions
Found several recomendations to #include the whole source file into the test script to get access to static data. IF you do this, you can't build the source file or you'll get redefined symbol errors. Any files that depend on this one will break too.

### Unity quirks
I think I ran into this issue: https://github.com/ThrowTheSwitch/Unity/issues/438. The solution was to not define a setUp/tearDown function. This is fine for now, but may cause problems later.

## GDB
GDB was useful while debugging the test executable.

Start GDB on the exe:
$ gdb Unity\test.exe

Run the exe:
$ run

If you compile wiht the -g3 flag, gdb can interogate line numbers when there is a fault.

## Board can't communicate
The current commit leaves the board in a state where is cannot communicate with the PC. OpenOCD gives an "Error: libusb_open() failed with LIBUSB_ERROR_NOT_SUPPORTED" error.

I have used GPIO PA0 - 13 for my application. Some of these pins may have alternate uses, and me pulling them down has stopped them working.

I need to find free pins to use.

If the board gets into this state, trying to flash it while holding down the reset button worked.

2020/04/21
The above seems to have been a coinsidence because the same happened in a project that was uploading fine before.

A forum post suggested this can happen after Windows updates. A blog post (https://www.justinmklam.com/posts/2017/10/vscode-debugger-setup/ :heart:) suggested using Zadig (https://zadig.akeo.ie/) to make sure your USB drivers are up to date.

- Download and run the executable
- Click Options > List all devices
- Connect the STM32 to your computer via the USB cable
- Select "STM32 STLink" from the drop down
- The Driver box should populate with informaiton. In my case, it said I had no drivers :woman_facepalming: :man_facepalming:
- Clicked install drvier. After this, my board could be reprogrammed again

## Finding free pins
For my application, I need 13 GPIO, including a block of 6 numbered sequentially, and a block of 4 numbered sequentially (this is so I can take a 6-bit and 4-bit number respectively and write them directly out to the hardware in one go)

The data sheet (https://docs.rs-online.com/9099/0900766b81368481.pdf) says pin PA0 is a power reset pin, so this shouldn't be used. PA8 is MCO_1 (not sure what that means, but it is initialised elsewhere in the code) and the rest in the A port are JTAG.

The D port pins appeared to have no uses, so I took PD0 through PD13 (skipping PD12 because it is an LED).

## Crashes
Calling GPIO_on or GPIO_off with 0 arguments caused the code to crash, so added guards against this.

## Display image defects

### Electrical noise
Can see some non-selected rows lighting up, and can see some lights being missed.

Some attempts at resolving are written below. The issue turned out to be electrical noise coming from my 5V mains power supply. The problem goes away when I drive the LED matrix with a 5V USB battery, or plug the display into an outlet with nothing else connected to it.

This issue was seen again when drawing using Graphics.c. This issue is mostly noise still, running the display from a battery gets rid of most of the defects. Some remain when drawing small numbers of pixels.

### Other display mistakes
When moving one red box around the screen, a defect is seen when the box moves into view from the top. It only happens on the first display row. Doing the pixel calculations by hand shows this is not a mistake in the draw code.

Adding an always on row to the left of the display corrects this defect. This suggests the display is losing synchronisation with the clock signal, and a pulse at the start of each line helps to synchronise it.

This line does not need to be the same colour as the moving box, and can be at the start or end of the row. Colouring the whole background also works. Using one [0,0] pixel does not work, at the start or the end of the line. Setting this colour before the data is being clocked out doesn't work. The "line" can be angled, it doens't need to be the full height of the display, but not much less than half. So at least one pixed per address, or slightly less works.

This "sync line" lets me draw single pixels.

It's unclear why I'd need this this time around, and not for the TI version.

## Optomised out variables
Code on a branch seemed to be having trouble with code being optomised away. I tried making variables volatile, and turning off optomisations, but this for some reason didn't help. It's possible I changed something in .cproject by mistake that was causing this because the branch commit shows some large changes in .cproject

## Display image defects (Log)

Timing
- Can't have test going to the log while running this, it slows the code down significantly
- slowing the interrupt down, and delaying GPIO switches does not eliminate these errors
- Undoing the loop unrolling in LEDMATRIX_vDrawRow doens't help
- Making the GPIO code inline doens't help

Pin setup
- changing GPIO_InitStructure.Speed from GPIO_SPEED_FAST to GPIO_SPEED_HIGH does not help (nor does GPIO_SPEED_MEDIUM)
- changing GPIO_InitStructure.Pull between GPIO_PULL_DOWN, GPIO_PULLUP, and GPIO_NOPULL did not help

Physical connection
- this is using jumper leads pushed onto the STM32 pins, these connections could be poor. The underside has longer pins to connect to - this made an improvement, with some flicker remaining

Build
- Release over debug still shows errors

Power
- Are we running too many LEDs and getting near the current limit? Running just the top half of addresses shows flicker, as does running just top quarter in red

Position
- Does this happen in all sections in all colours? Drawing half the display at a time, one colour at a time

|   | Top half | Bottom half |
|---|---|---|
| Red | Medium | High  |
| Green | High | High  |
| Blue | None | None  |

- Can we draw the whole display in blue? No, significant errors again

Noticed an LED on the board lit after the USB cable had been removed, so the display must be back feeding the board.

This is from PIN PD13 (PIN_OE) - found a 4K7 pull up resistor on the pin.

To resolve this:
- Use a IRLZ34N transistor (NPN - normally closed, 1 - 2 V gate-source voltage)
- Connect PD13 to gate, DISPLAY_OE to drain, ground to source
- Didn't add a pull down resistor between gate and source because this output is already pulled down (data sheet says 40K)

The flicker is gone, but the display is super dim now. When measuring the PD13 voltage, noticed the transistor was closed while the output was on. This makes it normally open, not normally closed.

Reversing the direction of PIN_OE (wherever I set it, clear it and vise versa) brought the brightness back, but also brings back the flickering (subjectively slightly worse than before)

|   | Top half | Bottom half |
|---|---|---|
| Red | Medium | Medium  |
| Green | High | High  |
| Blue | Medium | Low  |

