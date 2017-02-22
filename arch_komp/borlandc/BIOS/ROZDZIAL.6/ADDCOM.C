/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-2.  Dodanie portu szeregowego do BDA
*/


/*
   ADDCOM.C
   Dodanie adresu portu szeregowego do obszaru danych BIOS (BDA)
*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define com1Base (unsigned int far *)0x00400000
#define com2Base (unsigned int far *)0x00400002
#define com3Base (unsigned int far *)0x00400004
#define com4Base (unsigned int far *)0x00400006

/* Prototypy funkcji */
unsigned char addSerialPort ( unsigned int );
unsigned int  ASCIItoHex    ( char * );
unsigned char toHex         ( unsigned char );

void main( int argc, char *argv[] )
{
unsigned char portUsed;

if ( (argc != 2) || (strlen(argv[1]) != 4) )
{
   printf( "Skˆadnia: %s <Adres bazowy - czterocyfrowa liczba hex>\7", 
          argv[0] );
   printf( "\nStandardowe adresy: 03F8, 02F8, 03E8 i 02E8" );
   exit( 1 );
   }

portUsed = addSerialPort( ASCIItoHex(argv[1]) );
if ( portUsed == 0 )
{
   printf( "\nWszystkie cztery porty ju¾ zaj©te !\7" );
   exit( 2 );
   }

printf( "Port zostaˆ dodany jako COM%c", portUsed+'0' );
exit( 0 );
}

/* Zwraca 0 = brak miejsca w BDA dla kolejnego portu lub
           1,2,3,4 = dodano jako COM1, COM2, COM3 lub COM4.
*/
unsigned char addSerialPort( unsigned int ioAddress )
{
unsigned int far *comAddrP;
int portNr;

comAddrP = com1Base;

for( portNr = 1;  portNr < 5;  portNr++ )
{
   if ( *comAddrP == 0 )
   {                           /* Znaleziono wolne miejsce */
      *comAddrP = ioAddress;
      return( portNr );
      }
   ++comAddrP;
   }

/* Wszystko zaj©te ! */
return( 0 );
}

/* Zawraca binarny r¢wnowa¾nik cyfry heksadecymalnej */
unsigned char toHex( unsigned char ch )
{
unsigned char binVal;

if ( isdigit(ch) )
   binVal = ch - '0';
else
   binVal = 10 + (toupper(ch) - 'A');

binVal &= 15;
return( binVal );
}

/* Zamienia liczb© hex w tek˜cie na unsigned int */
unsigned int ASCIItoHex( char *str )
{
unsigned int val;

val  = toHex(*(str+0)) << 12;
val |= (toHex(*(str+1)) << 8);
val |= (toHex(*(str+2)) << 4);
val |=  toHex(*(str+3));

return( val );
}
