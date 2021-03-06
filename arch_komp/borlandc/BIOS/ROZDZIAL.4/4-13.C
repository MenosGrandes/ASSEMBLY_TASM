/*
   BIOS dla programujących w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-13.  Odczytanie z ekranu znaku i atrybutu
*/ 


/* Odczytaj znak i atrybut z bieżącej pozycji kursora*/
void readCell( unsigned char videoPage, 
               unsigned char *pChar, 
               unsigned char *pAttribute )
{
union REGS regs;

regs.h.ah = 8;                /* AH = 8 - odczytaj komórkę */
regs.h.bh = videoPage;        /* BH = strona */
int86( 0x10, &regs, &regs );  /* Wywołanie funkcji BIOS */

*pChar = regs.h.al;           /* AL = znak */
*pAttribute = regs.h.ah;      /* AH = atrybut */
}
