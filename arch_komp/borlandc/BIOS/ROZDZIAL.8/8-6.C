/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-6.  Drukowanie zawarto˜ci ekranu
*/


/*
   Drukowanie bie¾¥cej zawarto˜ci ekranu na drukarce LPT1
   informacja o stanie drukowania pobierana z adresu 50:00h
*/

#define pStat (unsigned char far *)0x00500000

unsigned char printScreen( void )
{
union REGS regs;                  /* Nie potrzebne s¥ dane wej˜ciowe */

int86( 0x05, &regs, &regs );      /* Wywoˆanie funkcji BIOS */
return( *pStat );                 /* Zwr¢cenie kodu stanu */
}
