/*
   BIOS dla programuj¥cach w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-4.  Odczytanie i ustawienie pozycji kursora
*/


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
