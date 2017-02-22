/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-20.  Zwraca informacj© o zainstalowanej karcie graficznej
*/


/*
   Rozpoznanie zainstalowanej karty.
   Funkcja zwraca:
   0x00 = Nie ma monitora                  0x07 = VGA / monitor monochromatyczny
   0x01 = MDA / monitor monochromatyczny   0x08 = VGA / monitor kolorowy
   0x02 = CGA / monitor kolorowy           0x0A = MCGA / monitor kolorowy
   0x04 = EGA / monitor kolorowy           0x0B = MCGA / monitor monochromatyczny
   0x05 = EGA / monitor monochromatyczny   0x0C = MCGA / monitor kolorowy
   0x06 = PGA / monitor kolorowy           0xFF = nieznana karta / monitor
*/

unsigned char getVideoAdapterType( void )
{
union REGS regs;

regs.x.ax = 0x1a00;             /* AX = funkcja 1Ah, podfunkcja 0 */
int86( 0x10, &regs, &regs );    /* Wywoˆanie funkcji BIOS */

if ( regs.h.al == 0x1a )        /* AL = 1Ah je¾eli funkcja zaimplementowana */
   return( regs.h.bl );         /* BL = rodzaj zainstalowanej karty */
                                /* Je¾eli funkcja nie jest zaimplementowana, */
return( (unsigned char)0xff );  /*    zwr¢† 0FFh (nieznana karta) */
}
