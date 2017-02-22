/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-16.  Odczytanie koloru piksela
*/ 


/* Zwraca kolor piksela w podanej kolumnie i wierszu */
unsigned char readPixel( unsigned char videoPage,
                         unsigned int row, 
                         unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0d;             /* AH = 0x0D - czytaj piksel */
regs.h.bh = videoPage;        /* BH = strona video */
regs.x.cx = column;           /* CX = kolumna (x) */
regs.x.dx = row;              /* DX = wiersz (y) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.al );          /* AL = kolor piksela */
}
