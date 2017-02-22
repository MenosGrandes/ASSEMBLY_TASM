/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-3.  Odczytanie zawarto˜ci konfiguracyjnej
   pami©ci CMOS RAM
*/


/*
   READCMOS.C
   Czyta i wy˜wietla zawarto˜† pami©ci CMOS RAM
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

/* Makrodefinicja zwracaj¥ca '1' lub '0' w zale¾no˜ci od stanu bitu */
#define bitOnOff( flag, bit ) ((flag) & bit) ? '1' : '0'

unsigned char readCmos( unsigned char index )
{
outp( 0x70, index );
return( inp(0x71) );
}

void main( void )
{
unsigned int cmosIndex;
unsigned char cmosValue;

printf( "\nCMOS    --  Zawarto˜†  --" );
printf( "\nBajt     Hex        Bin" );

for( cmosIndex = 0;  cmosIndex != 64;  cmosIndex++ )
{
   cmosValue = readCmos( cmosIndex );
   printf( "\n %.2Xh     %.2Xh     %c%c%c%c%c%c%c%c",
             cmosIndex,
             cmosValue,
             bitOnOff( cmosValue, 128 ),
             bitOnOff( cmosValue,  64 ),
             bitOnOff( cmosValue,  32 ),
             bitOnOff( cmosValue,  16 ),
             bitOnOff( cmosValue,   8 ),
             bitOnOff( cmosValue,   4 ),
             bitOnOff( cmosValue,   2 ),
             bitOnOff( cmosValue,   1 ) );
   }
}
