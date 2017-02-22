/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-8.  Wy˜wietlenie zestawu znak¢w komputera
*/


/*
   SHOWCSET.C
   Wy˜wietla peˆny zestaw znak¢w komputera PC 
   w 16 wierszach po 16 znak¢w
*/

#include <dos.h>
#include <stdio.h>

#define NORMAL 7           /* Normalny tekst: biaˆe litery, czarne tˆo */

void getCursorPosition( unsigned char, 
                        unsigned char *, 
                        unsigned char * );
void setCursorPosition( unsigned char, 
                        unsigned char, 
                        unsigned char );
void writeCell( unsigned char, 
                unsigned char, 
                unsigned char, 
                unsigned int );

void main( void )
{
int charValue;
unsigned char row, 
              column;

printf( "\n\n  Wy˜wietlenie zestawu znak¢w" );
printf( "\n-------------------------------" );

for ( charValue = 0;  
      charValue != 256;  
      charValue++ )
{
   if ( (charValue % 16) == 0 )
   {  /* Drukuj opis co 16 znak¢w */
      printf( "\n%.3u-%.3u  ", charValue, charValue+15 );
      getCursorPosition( 0, &row, &column );
      }

   writeCell( 0, (unsigned char)charValue, NORMAL, 1 );
   ++column;
   setCursorPosition( 0, row, column );
   }
}

/* Odczytanie pozycji kursora */
void getCursorPosition( unsigned char videoPage, 
                        unsigned char *row, 
                        unsigned char *column )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - odczytanie pozycji kursora */
regs.h.bh = videoPage;        /* BH = strona */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*row = regs.h.dh;             /* DH = numer wiersza kursora */
*column = regs.h.dl;          /* DL = numer kolumny kursora */
}

/* Ustaw pozycj© kursora */
void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - ustaw pozycj© kursora */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.dh = row;              /* DH = numer wiersza */
regs.h.dl = column;           /* DL = numer kolumny */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */

/* Napisanie znaku i atrybutu na wybranej stronie */
void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - zapisz kom¢rk© */
regs.h.al = character;        /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.bl = attribute;        /* BL = atrybut */
regs.x.cx = times;            /* CX = liczba powt¢rzeä znaku */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
