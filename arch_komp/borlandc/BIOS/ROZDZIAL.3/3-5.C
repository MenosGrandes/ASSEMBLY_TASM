/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-5.  Okre˜lenie rozmiaru pami©ci podstawowej
*/ 


/* Zwraca w KB ilo˜† zainstalowanej pami©ci podstawowej */
unsigned int getBaseMemorySize( void )
{
union REGS regs;

   int86( 0x12, &regs, &regs );  /* Wywoˆanie funkcji BIOS
                                    - bez ustawiania zawarto˜ci rejestr¢w */
   return( regs.x.ax );          /* AX = pami©† podstawowa w KB */
   }
