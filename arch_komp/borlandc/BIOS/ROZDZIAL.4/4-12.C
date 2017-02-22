/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-12.  Wy˜wietlenie tekstu ASCIIZ
*/ 


/* Wy˜wietlenie tekstu zakoäczonego zerem */
void displayString( unsigned char videoPage, 
                    unsigned char row, 
                    unsigned char column, 
                    unsigned char *string )
{
union REGS regs;

regs.h.ah = 2;                   /* AH = 2 - ustaw pozycj© kursora */ 
regs.h.bh = videoPage;           /* BH = strona */ 
regs.h.dh = row;                 /* DH = wiersz */ 
regs.h.dl = column;              /* DL = kolumna */ 
int86( 0x10, &regs, &regs );     /* Wywoˆanie funkcji BIOS */ 

while( *string )                 /* Wy˜wietl kolejno wszystkie znaki */
{
   regs.h.ah = 0x0e;             /* AH = 0x0E - napisz znak */
   regs.h.al = *string;          /* AL = znak */
   regs.h.bh = videoPage;        /* BH = strona */
   int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

   ++string;                     /* Przejd« do nast©pnego znaku */
   }
}
