Driving an LED dotmatrix with an STM32F411e-Discovery.

## Board can't communicate
The current commit leaves the board in a state where is cannot communicate with the PC.

I have used GPIO PA0 - 13 for my application. Some of these pins may have alternate uses, and me pulling them down has stopped them working.

I need to find free pins to use.

If the board gets into this state, trying to flash it while holding down the reset button worked.

