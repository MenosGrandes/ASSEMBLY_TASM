/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-9.  Wy˜wietlenie wszystkich kombinacji atrybut¢w
*/


/*
   SHOWATTR.C
   Pokazuje wszystkie mo¾liwe warto˜ci atrybutu w trybie tekstowym
*/

#include <dos.h>
#include <stdio.h>

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
int attribute;
unsigned char row, column;
char scratch[5];

printf( "\n\n  Wy˜wietlenie atrybut¢w" );
printf( "\n--------------------------" );

for ( attribute = 0;  attribute != 256;  attribute++ )
{
   if ( (attribute % 16) == 0 )
   {  /* Przej˜cie do nowej linii co 16 atrybut¢w */
      printf( "\n" );
      getCursorPosition( 0, &row, &column );
      }

   sprintf( scratch, " %.3u", attribute );
   writeCell( 0, scratch[0], (unsigned char)attribute, 1 );

   ++column;
   setCursorPosition( 0, row, column );
   writeCell( 0, scratch[1], (unsigned char)attribute, 1 );

   ++column;
   setCursorPosition( 0, row, column );
   writeCell( 0, scratch[2], (unsigned char)attribute, 1 );

   ++column;
   setCursorPosition( 0, row, column );
   writeCell( 0, scratch[3], (unsigned char)attribute, 1 );

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
