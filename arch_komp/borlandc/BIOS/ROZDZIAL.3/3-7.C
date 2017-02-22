/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-7.  Odczytanie informacji o zainstalowanym wyposa¾eniu
*/ 


/* Odczytaj z BDA informacj© o zainstalowanym wyposa¾eniu */
unsigned int getInstalledEquipment( void )
{
union REGS regs;

   int86( 0x11, &regs, &regs);  /* Bez ustawiania zawarto˜ci rejestr¢w */
   return( regs.x.ax );         /* AX = informacja o wyposa¾eniu */
   }
