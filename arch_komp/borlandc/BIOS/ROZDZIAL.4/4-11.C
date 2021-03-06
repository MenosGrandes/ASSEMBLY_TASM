/*
   BIOS dla programujących w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-11.  Pisanie znaków wraz z ich interpretacją
*/ 


/* Napisz znak, przesuń kursor, interpretuj znaki sterujące*/
void writeTTYChar( unsigned char videoPage,
                   unsigned char character, 
                   unsigned char attribute )
{
union REGS regs;

regs.h.ah = 0x0e;             /* AH = 0x0E - napisz znak */
regs.h.al = character;        /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona */
int86( 0x10, &regs, &regs );  /* Wywołanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
