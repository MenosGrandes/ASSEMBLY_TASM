/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-2.  Pobranie bajtu stanu klawiatury
*/


/*
   READBDA.C
   Ci¥gˆe wy˜wietlanie bajtu stanu klawiatury (BDA, pozycja 40:17)
*/

#include <stdio.h>

#define kbdStatByte (unsigned char far *)0x00400017

/* Definicja bit¢w u¾ywanych w kbdStatByte */
#define K_INSERT    128
#define K_CAPSLOCK   64
#define K_NUMLOCK    32
#define K_SCROLLLOCK 16
#define K_ALT         8
#define K_CTRL        4
#define K_LSHIFT      2
#define K_RSHIFT      1

/* Makrodefinicja zwracaj¥ca '1' je¾eli bit jest r¢wny 1
   lub '0' je¾eli bit jest r¢wny 0 */
#define bitOnOff( flag, bit ) ((flag) & bit) ? '1' : '0'

void main( void )
{
printf( "\nBity 76543210 w pozycji 40:17 BDA\n" );

for(;;)
{
   printf( "\015     %c%c%c%c%c%c%c%c",
           bitOnOff( *kbdStatByte, K_INSERT ),
           bitOnOff( *kbdStatByte, K_CAPSLOCK ),
           bitOnOff( *kbdStatByte, K_NUMLOCK ),
           bitOnOff( *kbdStatByte, K_SCROLLLOCK ),
           bitOnOff( *kbdStatByte, K_ALT ),
           bitOnOff( *kbdStatByte, K_CTRL ),
           bitOnOff( *kbdStatByte, K_LSHIFT ),
           bitOnOff( *kbdStatByte, K_RSHIFT ) );
   }
}
