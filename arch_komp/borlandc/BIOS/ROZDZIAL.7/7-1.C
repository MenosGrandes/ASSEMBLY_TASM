/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-1.  Odczytanie znaku z klawiatury
*/


/*
   Zwraca nast©pn¥ par© kod klawisza/kod znaku 
   z bufora klawiatury
*/

void getKbdChar( unsigned char *charCode,
                 unsigned char *scanCode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 lub 10h - odczytanie znaku  */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

*scanCode = regs.h.ah;        /* AH = kod klawisza */
*charCode = regs.h.al;        /* AL = kod znaku */
}                             /* Niczego nie zwracamy */
