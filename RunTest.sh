#!/bin/bash
gcc -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DTEST -I"include" -I"system/include" -I"system/include/cmsis" -I"system/include/stm32f4-hal" src/Sprite.c src/UserInput.c Unity/*.c -o Unity/test.exe

./Unity/test.exe
