#include <18F4685.h>
#device ADC=16

/*
TCP/IP Stack enabled.
Many TCP/IP configuration settings (servers enabled, ports used,
etc) are defined in TCPIPConfig.h.
Many hardware configuration settings (SPI port and GPIO pins used)
are defined in HardwareProfile.h.
*/

#include "tcpip/p18cxxx.h"

#fuses HS,WDT4096,NOPROTECT,NOLVP,NODEBUG,NOSTVREN,PUT,NOCPD,NOWRT,NOIESO,NOFCMEN,NOPBADEN,NOWRTC,NOWRTB,NOEBTR,NOEBTRB,NOCPB,NOXINST,NOLPT1OSC,NOMCLR,NOBROWNOUT,BORV43

#use delay(crystal=20000000)


#if defined(__PCH__)
   #define GetSystemClock()       getenv("CLOCK")
   #define GetInstructionClock()  (GetSystemClock()/4)
   #define GetPeripheralClock()   (unsigned int32)GetInstructionClock()
#else
   #define GetSystemClock()       getenv("CLOCK")
   #define GetInstructionClock()  (GetSystemClock()/2)
   #define GetPeripheralClock()   (GetSystemClock()/2)
#endif

#define MIN(a,b)  ((a > b) ? b : a)

#include <stdint.h>
#include "tcpip/StackTsk2.h"
#include "tcpip/TCPIP Stack/TCPIPConfig.h"
#include "tcpip/TCPIP Stack/HardwareProfile.h"
#include "tcpip/StackTsk2.c"

