/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-1.  Wy˜wietlenie informacji o systemowej pami©ci ROM
*/


/*
   ROM-INFO.C
   Wy˜wietla pola informacyjne ROMu
   zaczynaj¥cego si© w segmencie F000
*/

#include <stdio.h>
#include <string.h>

#define eisaSig      (unsigned char far *) 0xf000ffd9
#define romFamily    (unsigned char far *) 0xf000ffe4
#define romRelease   (unsigned char far *) 0xf000ffe5
#define romRev       (unsigned char far *) 0xf000ffe6
#define romType      (unsigned char far *) 0xf000ffe8
#define manufacturer (unsigned char far *) 0xf000ffea
#define romDate      (unsigned char far *) 0xf000fff5
#define pcType       (unsigned char far *) 0xf000fffe

char eisa[]    = "EISA",
     nonEisa[] = "ISA lub PS/2",
    *architecture;

void main( void )
{
static unsigned char far *EISA_SIG = eisaSig;
int i;

/* Sprawdzenie czy to komputer z magistral¥ EISA czy ISA */
for( i = 0; i < strlen( eisa ); i++ )
{
   if ( EISA_SIG[i] != eisa[i] )
      break;
   }

if ( i != strlen( eisa ) )
   architecture = nonEisa;
else
   architecture = eisa;

printf( "\nInformacja o systemowej pami©ci ROM" );
printf( "\nWytw¢rca        : %.6Fs",   manufacturer );
printf( "\nKod rodziny     : %Fc",     *romFamily );
printf( "\nWersja          : %Fc.%Fc", *romRev, *romRelease );
printf( "\nTyp ROMu        : %.2Fs",   romType );
printf( "\nData utworzenia : %.8Fs",   romDate );
printf( "\nRodzaj systemu  : %.2FX",   *pcType );
printf( "\nArchitektura    : %s",      architecture );
}
