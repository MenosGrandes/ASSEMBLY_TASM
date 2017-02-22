/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-8.  Zamiana port¢w drukarek
*/


/*
   EXCHANGE.C
   Zamiana w BDA danych o dw¢ch portach drukarki
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define firstPrinterAddress (unsigned int far *)0x00400008
#define firstPrinterTimeout (unsigned char far *)0x00400078

void main( int argc, char *argv[] )
{
static char lpt[] = "lpt";

unsigned int tempAddress;
unsigned char tempTimeout;

unsigned char *printer1,
              *printer2;

unsigned int printer1Index,
             printer2Index;

unsigned int far *printer1Address,
             far *printer2Address;

unsigned char far *printer1Timeout,
              far *printer2Timeout;

/* Musz¥ zosta† podane dwa parametry */
if ( (argc != 3) ||
     (memicmp(argv[1],lpt,strlen(lpt)) != 0) ||
     (memicmp(argv[2],lpt,strlen(lpt)) != 0) )
{
   printf( "Skˆadnia: %s <LPT1/2/3> <LPT1/2/3>\7", argv[0] );
   exit( 1 );
   }

/* Wyˆuskanie numer¢w drukarek */
printer1 = argv[1];
printer1 += strlen( lpt );

printer2 = argv[2];
printer2 += strlen( lpt );

if ( (*printer1 != '1') &&
     (*printer1 != '2') &&
     (*printer1 != '3') )
{
   printf( "Drukarka 1 musi by† oznaczona LPT1, LPT2 lub LPT3!\7" );
   exit( 1 );
   }

if ( (*printer2 != '1') &&
     (*printer2 != '2') &&
     (*printer2 != '3') )
{
   printf( "Drukarka 2 musi by† oznaczona LPT1, LPT2 lub LPT3!\7" );
   exit( 1 );
   }

if ( (*printer1) == (*printer2) )
{
   printf( "Drukarki 1 i 2 nie mog¥ by† t¥ sam¥ drukark¥!\7" );
   exit( 1 );
   }

/* Konwersja znak¢w ASCII na liczby caˆkowite */
printer1Index = ((*printer1) - '0');
printer2Index = ((*printer2) - '0');

/* Przesuni©cie do adresowania od 0 */
printer1Index -= 1;
printer2Index -= 1;

/* Wskazanie do p¢l BDA dla pierwszej drukarki */
printer1Address = firstPrinterAddress;
printer1Timeout = firstPrinterTimeout;

while ( printer1Index )
{
   ++printer1Address;
   ++printer1Timeout;
   --printer1Index;
   }

/* Wskazanie do p¢l BDA dla drugiej drukarki */
printer2Address = firstPrinterAddress;
printer2Timeout = firstPrinterTimeout;

while ( printer2Index )
{
   ++printer2Address;
   ++printer2Timeout;
   --printer2Index;
   }

/* Ostrze¾enie u¾ytkownika je¾eli port nie jest zainstalowany */
if ( *printer1Address == 0 )
   printf( "Uwaga - Drukarka 1 nie jest zainstalowana!\7" );

if ( *printer2Address == 0 )
   printf( "Uwaga - Drukarka 2 nie jest zainstalowana!\7" );

/* Zamiana danych za pomoc¥ zmiennych pomocniczych */
tempAddress = *printer1Address;       /* Drukarka 1 -> pom */
tempTimeout = *printer1Timeout;

*printer1Address = *printer2Address;  /* Drukarka 2 -> drukarka 1 */
*printer1Timeout = *printer2Timeout;

*printer2Address = tempAddress;       /* Pom ->drukarka 2 */
*printer2Timeout = tempTimeout;

printf( "\nDrukarki zostaˆy zamienione." );
exit( 0 );
}
