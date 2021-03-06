/*
   BIOS dla programujących w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-7.  Ustawianie opóźnienia i szybkości powtarzania
*/


/*
   KBDSPEED.C 
   Program ustawiania opóźnienia i szybkości powtarzania
   na podstawie parametrów w linii wywołania polecenia

   Aby uruchomić program napisz:
      KBDSPEED <opóźnienie> <szybkość powtarzania>
       Opóźnienie - 0 (najmniejsze) do 3 (największe)
       Szybkość powtarzania - 0 (największa) do 31 (najmniejsza).
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void main( int argc, char *argv[] )
{
union REGS regs;
char delay, repeatRate;

#define MIN_DELAY   0        /* Opóźnienie jest z zakresu 0<=n<=3 */
#define MAX_DELAY   3
#define MIN_REPEAT  0        /* Szybkość powtarzania - 0<=n<=31 */
#define MAX_REPEAT 31

/* Ostrzeżenie przy niewłaściwej liczbie parametrów */
if ( argc != 3 )
{
   printf( "Składnia: %s <opóźnienie> <szybkość powtarzania>\n",
                                   argv[0] );
   printf( "   Opóźnienie = 0 (najmniejsze) do 3 (największe)\n" );
   printf( "   Szybkość   = 0 (największa) do 31 (najmniejsza)\n\7" );
   exit( 1 );
   }

/* Pobranie i sprawdzenie wartości opóźnienia */
delay = atoi( argv[1] );
if( delay > MAX_DELAY )
{
   printf( "Opóźnienie musi być z zakresu %u do %u!\n\7",
           MIN_DELAY, MAX_DELAY );
   exit( 1 );
   }

/* Pobranie i sprawdzenie wartości szybkości powtarzania */
repeatRate = atoi( argv[2] );
if( repeatRate > MAX_REPEAT )
{
   printf( "Szybkość powtarzania musi być z zakresu %u do %u!\n\7",
           MIN_REPEAT, MAX_REPEAT );
   exit( 1 );
   }

/* Ustawienie opóźnienia i szybkości powtarzania */
regs.h.ah = 3;                /* AH = 3 - ustawienie opóźnienia
                                  i szybkości powtarzania */ 
regs.h.al = 5;                /* AL = 5 (tego wymaga BIOS!) */ 
regs.h.bh = delay;            /* BH = opóźnienie */ 
regs.h.bl = repeatRate;       /* BL = szybkość powtarzania */ 
int86( 0x16, &regs, &regs );  /* Wywołanie funkcji BIOS */ 

printf( "Gotowe" );
exit( 0 );
}
