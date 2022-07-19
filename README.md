# MCU_DLT_LOGS_TRIVIAL_LIB
A trivial library to send DLT (debug) logs from MCU like STM32 or any other MCU, via serial communication interface like USART/UART. 

**_Demo video:_**

https://www.youtube.com/watch?v=oruxyXiptlI

To view these logs i used the DLT-Viewer:
 * https://github.com/COVESA/dlt-viewer

![DltViewerScreen](https://github.com/trteodor/MCU_DLT_LOGS_TRIVIAL_LIB/blob/master/Dlt_Viewer_ScreenShot.jpg)

ECU Config:

![DltEcuConfig](https://github.com/trteodor/MCU_DLT_LOGS_TRIVIAL_LIB/blob/master/Ecu_Config.jpg)

This program is very useful to filtering, marking and we can do much more things with received data. Anyway i planning to use this library in my future hobbyst projects to show debug logs :) 

If such arise and I will need debug logs from the microcontroller. Doubtful but possible.

This library is very simple, and support only small part of dlt logs possibilities but in my opinion it's usefull solution. Neverless Many optimizations can be done, 
if you want change something please feel free to create change pull request.

**_Detailed instruction is in file: dlt_logs_mcu_lib_files/dlt_logs_mcu.h_**

Provided examples was tested on NUCLEO STM32F103RB board and DLT_Viewer Release Tag  2.18 :
**_Actually only example called "example_makefile" is maintance because this approach is very convience for me and should be platform independent_**

![NucleoScreen](https://github.com/trteodor/MCU_DLT_LOGS_TRIVIAL_LIB/blob/master/NucleoBoard.jpg)

You are ready to start adventure with this example when you have this tools in your environment: (environment variables)!

    * arm-none-eabi-gcc 9.3.1

    * OpenOCD 0.11.0

    * make 3.81 or newer

Then just call make in directory of this repository or project


# References

https://github.com/COVESA/dlt-viewer

https://github.com/COVESA/dlt-daemon

https://www.autosar.org/fileadmin/user_upload/standards/foundation/1-0/AUTOSAR_PRS_DiagnosticLogAndTraceProtocol.pdf

https://qiita.com/takeoverjp/items/d7a9ad4e5f0b778308be

https://medium.com/inatech/about-your-next-automotive-friendly-prototype-53ce5f2923e7


#stm32 #dlt #viewer #debug #log #logs #mcu #microcontroller #data #log #diagnostic #and #trace
