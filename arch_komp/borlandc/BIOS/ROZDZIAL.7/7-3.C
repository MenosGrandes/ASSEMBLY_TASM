/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-3.  Sprawdzenie czy naci˜ni©to klawisz
*/


/*
   Zwraca 0 je¾eli bufor jest pusty 
   lub warto˜† niezerow¥ je¾eli w buforze s¥ dane
*/

unsigned int testKbdCharReady( unsigned char *charCode,
                               unsigned char *scanCode )
{
unsigned char localAH, 
              localAL;
unsigned int returnValue;

/* Przej˜cie do asemblera aby wywoˆa† przerwanie */
_asm  mov AH, 1             /* AH = 1 lub 11h - sprawdzenie klawiatury */
_asm  int 16h               /* Wywoˆanie funkcji BIOS */
_asm  jnz endFunc           /* Zero = 0 oznacza znak w buforze */
_asm  xor AX,AX             /* W przeciwnym wypadku bufor jest pusty
                                - zwracamy 0 */
endFunc:                    /* Przeniesienie AH i AL do zmiennych */
_asm  mov localAH, AH       /* AH = kod klawisza */
_asm  mov localAL, AL       /* AL = kod znaku */
_asm  mov returnValue, AX   /* Zachowanie zˆo¾onej warto˜ci 16-bitowej
                                jako warto˜ci zwracanej przez funkcj© */

/* Powr¢t do C i zakoäczenie funkcji */
*scanCode = localAH;        /* Skopiowanie kodu klawisza */
*charCode = localAL;        /* Skopiowanie kodu znaku */

return( returnValue );      /* Je¾eli s¥ dane, to returnValue != 0 */
}
