/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-15.  Narysowanie piksela
*/ 


/* Narysowanie piksela we wskazanej kolumnie i wierszu */
void writePixel( unsigned char color, 
                 unsigned char videoPage, 
                 unsigned int row, 
                 unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0c;             /* AH = 0x0C - rysuj piksel */
regs.h.al = color;            /* AL = kolor */
regs.h.bh = videoPage;        /* BH = strona */
regs.x.cx = column;           /* CX = kolumna (x) */
regs.x.dx = row;              /* DX = wiersz (y) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
