/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-1.  Wykrywanie zainstalowanych port¢w szeregowych
*/


/*
   SHOW-COM.C
   Informuje, kt¢re porty szeregowe s¥ zainstalowane
*/

#include <stdio.h>

#define boolean unsigned char

#define com1Base (unsigned int far *)0x00400000
#define com2Base (unsigned int far *)0x00400002
#define com3Base (unsigned int far *)0x00400004
#define com4Base (unsigned int far *)0x00400006

boolean testSerialPortInstalled( char *, unsigned int far * );

void main( void )
{
testSerialPortInstalled( "COM1", com1Base );
testSerialPortInstalled( "COM2", com2Base );
testSerialPortInstalled( "COM3", com3Base );
testSerialPortInstalled( "COM4", com4Base );
}

/* 
   Zwraca 1 - port szeregowy jest zainstalowany
          0 - port szeregowy nie jest zainstalowany
   Pierwszy parametr jest nazw¥ portu (tekst ASCII).
   Drugi jest wska«nikiem do pola adresu bazowego w BDA.
*/
boolean testSerialPortInstalled( char *portName,
                                 unsigned int far *ioAddress )
{
if( *ioAddress == 0 )
   printf( "\n%s : nie jest zainstalowany.", portName );
else
   printf( "\n%s : jest zainstalowany pod adresem %.4X.",
           portName, *ioAddress );

return( ( *ioAddress != 0 ) );
}
