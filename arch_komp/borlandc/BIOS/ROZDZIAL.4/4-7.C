/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-7.  Napisanie znaku wraz z atrybutem
*/


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
