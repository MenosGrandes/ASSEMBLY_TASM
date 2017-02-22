/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-6.  Okre˜lenie rozmiaru pami©ci rozszerzonej
                 (Extended Memory)
*/ 


/* Zwraca w KB rozmiar pami©ci rozszerzonej */
unsigned int getExtendedMemorySize( void )
{
union REGS regs;

   regs.h.ah = 0x88;             /* AH = 88h - podaj rozmiar
                                               pami©ci rozszerzonej */
   int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
   return ( regs.x.ax );         /* AX = dost©pna pami©† rozszerzona w */
   }
