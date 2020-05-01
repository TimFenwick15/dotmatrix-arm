:: Compile
:: C:\\MinGW\\bin\\gcc -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"include" -I"system/include" -I"system/include/cmsis" -I"system/include/stm32f4-hal" src/Animation.c Unity/Unity.c Unity/AnimationTest.c -o Unity/test.exe

:: gcc -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DTEST -I"include" -I"system/include" -I"system/include/cmsis" -I"system/include/stm32f4-hal" src/Animation.c Unity/Unity.c Unity/AnimationTest.c -o Unity/test.exe

:: gcc -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DTEST -I"include" -I"system/include" -I"system/include/cmsis" -I"system/include/stm32f4-hal" Unity/Unity.c Unity/AnimationTest.c -o Unity/test.exe

gcc -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DTEST -I"include" -I"system/include" -I"system/include/cmsis" -I"system/include/stm32f4-hal" Unity/*.c -o Unity/test.exe

:: Run the tests
Unity\\test.exe

