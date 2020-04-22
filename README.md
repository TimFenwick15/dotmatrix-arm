Driving an LED dotmatrix with an STM32F411e-Discovery.

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
Can see some non-selected rows lighting up, and can see some lights being missed.

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

