/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-1.  Znalezienie zainstalowanych port¢w drukarki
*/


/* SHOW-LPT.C
   Program sprawdza, kt¢re porty r¢wnolegˆe s¥ zainstalowane 
   poprzez sprawdzenie informacji o ich adresach bazowych w BDA
*/

#include <stdio.h>

#define boolean unsigned char

#define lpt1Base (unsigned int far *)0x00400008
#define lpt2Base (unsigned int far *)0x0040000a
#define lpt3Base (unsigned int far *)0x0040000c

boolean checkPrinter( char *, unsigned int far * );

void main( void )
{
checkPrinter( "LPT1", lpt1Base );
checkPrinter( "LPT2", lpt2Base );
checkPrinter( "LPT3", lpt3Base );
}

/* Zwraca 1 = port drukarki jest zainstalowany 
          0 = port drukarki nie jest zainstalowany
*/
boolean checkPrinter( char *printerName,
                      unsigned int far *ioAddress )
{
if( *ioAddress == 0 )
   printf( "\n%s : nie jest zainstalowany.", printerName );
else
   printf( "\n%s : jest zainstalowany pod adresem %.4X.",
           printerName, *ioAddress );

return( ( *ioAddress != 0 ) );
}
