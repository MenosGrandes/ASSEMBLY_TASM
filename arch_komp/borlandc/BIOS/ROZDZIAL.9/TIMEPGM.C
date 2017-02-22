/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-2.  Pomiar czasu wykonywania polecenia
*/


/*
   TIMEPGM.C
   U¾ycie licznika systemowego do obliczenia czasu
   wykonywania polecenia podanego jako argument
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define TICKS_PER_SECOND 18.2
#define TICKS_PER_DAY 1573040L

unsigned char readTickCounter( unsigned long * );

void main( int argc, char *argv[] )
{
float elapsedSeconds;
unsigned long startTick, endTick, elapsedTicks;

/* Bˆ¥d je¾eli nie podano jednego argumentu */
if ( argc != 2 )
{
   printf( "Skˆadnia: %s \"polecenie\"\7",argv[0] );
   exit( 1 );
   }

readTickCounter( &startTick );    /* Pobranie stanu licznika          */

system( argv[1] );                /* Wykonanie polecenia              */

readTickCounter( &endTick );      /* Ponowne pobranie stanu licznika  */

if ( endTick < startTick )        /* Je¾eli min©ˆa p¢ˆnoc             */
   endTick += TICKS_PER_DAY;      /* dokonaj korekty licznika         */

elapsedTicks = endTick-startTick; /* Obliczenie czasu wykonywania     */

                                  /* Przeliczenie na sekundy          */
elapsedSeconds = elapsedTicks / TICKS_PER_SECOND;

printf( "\nPolecenie wykonano w %f sek.", elapsedSeconds );
exit( 0 );
}

/*
   Zapisanie do zmiennej programu wywoˆuj¥cego stanu licznika
   systemowego i zwr¢cenie stanu znacznika przekroczenia p¢ˆnocy
*/
unsigned char readTickCounter( unsigned long *retval )
{
union REGS regs;
unsigned long result;

regs.h.ah = 0;                /* AH = 0 - odczytanie licznika */
int86( 0x1a, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

result = regs.x.cx;           /* CX = starsze 16 bit¢w */
result <<= 16;                /* przesuni©cie ich na wˆa˜ciw¥ pozycj© */
result |= regs.x.dx;          /* DX = mˆodsze 16 bit¢w */
*retval = result;            /* Zapisanie do zmiennej
                                      programu wywoˆuj¥cego */

return( regs.h.al );          /* AL = znacznik przekroczenia p¢ˆnocy */
}
