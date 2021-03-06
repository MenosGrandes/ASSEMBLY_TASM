/*
   BIOS dla programujących w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-7.  Odczytanie informacji o zainstalowanym wyposażeniu
*/ 


/* Odczytaj z BDA informację o zainstalowanym wyposażeniu */
unsigned int getInstalledEquipment( void )
{
union REGS regs;

   int86( 0x11, &regs, &regs);  /* Bez ustawiania zawartości rejestrów */
   return( regs.x.ax );         /* AX = informacja o wyposażeniu */
   }
