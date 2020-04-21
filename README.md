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

