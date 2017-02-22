/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-7.  Ustawianie op¢«nienia i szybko˜ci powtarzania
*/


/*
   KBDSPEED.C 
   Program ustawiania op¢«nienia i szybko˜ci powtarzania
   na podstawie parametr¢w w linii wywoˆania polecenia

   Aby uruchomi† program napisz:
      KBDSPEED <op¢«nienie> <szybko˜† powtarzania>
       Op¢«nienie - 0 (najmniejsze) do 3 (najwi©ksze)
       Szybko˜† powtarzania - 0 (najwi©ksza) do 31 (najmniejsza).
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void main( int argc, char *argv[] )
{
union REGS regs;
char delay, repeatRate;

#define MIN_DELAY   0        /* Op¢«nienie jest z zakresu 0<=n<=3 */
#define MAX_DELAY   3
#define MIN_REPEAT  0        /* Szybko˜† powtarzania - 0<=n<=31 */
#define MAX_REPEAT 31

/* Ostrze¾enie przy niewˆa˜ciwej liczbie parametr¢w */
if ( argc != 3 )
{
   printf( "Skˆadnia: %s <op¢«nienie> <szybko˜† powtarzania>\n",
                                   argv[0] );
   printf( "   Op¢«nienie = 0 (najmniejsze) do 3 (najwi©ksze)\n" );
   printf( "   Szybko˜†   = 0 (najwi©ksza) do 31 (najmniejsza)\n\7" );
   exit( 1 );
   }

/* Pobranie i sprawdzenie warto˜ci op¢«nienia */
delay = atoi( argv[1] );
if( delay > MAX_DELAY )
{
   printf( "Op¢«nienie musi by† z zakresu %u do %u!\n\7",
           MIN_DELAY, MAX_DELAY );
   exit( 1 );
   }

/* Pobranie i sprawdzenie warto˜ci szybko˜ci powtarzania */
repeatRate = atoi( argv[2] );
if( repeatRate > MAX_REPEAT )
{
   printf( "Szybko˜† powtarzania musi by† z zakresu %u do %u!\n\7",
           MIN_REPEAT, MAX_REPEAT );
   exit( 1 );
   }

/* Ustawienie op¢«nienia i szybko˜ci powtarzania */
regs.h.ah = 3;                /* AH = 3 - ustawienie op¢«nienia
                                  i szybko˜ci powtarzania */ 
regs.h.al = 5;                /* AL = 5 (tego wymaga BIOS!) */ 
regs.h.bh = delay;            /* BH = op¢«nienie */ 
regs.h.bl = repeatRate;       /* BL = szybko˜† powtarzania */ 
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */ 

printf( "Gotowe" );
exit( 0 );
}
