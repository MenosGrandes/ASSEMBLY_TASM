/*
   BIOS dla programuj¥cach w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-3.  Pisanie na wszystkich stronach w trybie tekstowym
*/


/*
   SEE8PGS.C
   Pisze na wszystkich 8 stronach,
   a nast©pnie wy˜wietla je kolejno
*/

#include <dos.h>
#include <stdio.h>
#include <conio.h>

#define ESCAPE 27                /* Kod klawisza Esc */

void makeVideoPageActive( unsigned char );

void main( void )
{
int page;

/* Napisz komunikat na ka¾dej stronie */
for( page=0; page<8; page++ )
{
   makeVideoPageActive( page );
   printf( "\n\nTo jest strona %u", page );
   printf( "  \nNaci˜nij Enter, aby kontynuowa† lub Esc, aby skoäczy†" );
   }

/* P©tla wy˜wietlaj¥ca strony, dop¢ki u¾ytkownik nie przerwie */
for( page=0; ; page++ )          /* Start od strony 0 */
{
   if ( page == 8 )              /* Kiedy strona == 8 */
      page = 0;                  /* zacznij od pocz¥tku */

   makeVideoPageActive( page );  /* Wy˜wietlenie strony */

/* Strona jest widoczna i mo¾esz przeczyta† komunikat */
   if ( getch() == ESCAPE )
      break;                     /* Skoäcz je¾eli u¾ytkownik nacisn¥ˆ Esc */
   }                             /* lub kontynuuj prac© */

makeVideoPageActive( 0 );        /* Odtw¢rz domy˜ln¥ stron© (0) */
}

/* Wybierz aktywn¥ stron© graficzn¥*/
void makeVideoPageActive( unsigned char whichPage )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - wybierz aktywn¥ stron© graficzn¥ */
regs.h.al = whichPage;        /* AL = numer strony (0...7) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}
