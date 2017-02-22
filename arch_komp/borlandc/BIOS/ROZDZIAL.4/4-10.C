/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-10.  Napisanie znaku bez atrybutu
*/ 


/* Napisz tylko znak (n razy, od bie¾¥cej pozycji kursora) */
void writeChar( unsigned char videoPage, 
                unsigned char character, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 0x0a;             /* AH = 0x0A - napisz znak */
regs.h.al = character;        /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona */
regs.x.cx = times;            /* CX = liczba powt¢rzeä znaku */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
