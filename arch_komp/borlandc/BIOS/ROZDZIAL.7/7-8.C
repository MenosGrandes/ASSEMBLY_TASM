/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-8.  Wpisywanie znaku do bufora klawiatury
*/


/* 
   Wpisanie kodu klawisza i kodu znaku do bufora klawiatury.  
   Zwraca 0 po udanej operacji lub 1 je¾eli bufor jest zapeˆniony
*/
unsigned char writeKbdData( unsigned char scanCode, 
                            unsigned char charCode )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - zapisz do bufora klawiatury */
regs.h.ch = scanCode;         /* CH = kod klawisza */
regs.h.cl = charCode;         /* CL = kod znaku */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

return( regs.h.al );          /* AL = 0 (sukces) lub 1 (bufor peˆen) */
}
