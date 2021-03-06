/*
   BIOS dla programuj�cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-17.  Zmiana koloru t�a/obrze�a dla karty CGA
*/ 


/*
   Ustawia dla kart CGA:
      - kolor obrze�a ekranu dla tryb�w tekstowych
      - kolor t�a dla tryb�w graficznych 320x200
      - kolor pi�ra dla trybu graficznego 640x200
*/

void setCgaColor( unsigned char newColorValue )
{
union REGS regs;

regs.h.ah = 0x0b;             /* AH = 0x0B - ustaw palet� barw */
regs.h.bh = 0;                /* BH = podfunkcja 0 */
regs.h.bl = newColorValue;    /* BL = nowy kolor lub paleta */
int86( 0x10, &regs, &regs );  /* Wywo�anie funkcji BIOS */
}                             /* Niczego nie zwracamy */
