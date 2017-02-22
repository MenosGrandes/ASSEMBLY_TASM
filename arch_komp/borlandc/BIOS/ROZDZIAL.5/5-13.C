/* 
   BIOS dla programuj•cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-13.  Sprawdzenie czy wyjmowano dyskietk©
*/


/*
   Sprawdzenie czy drzwiczki podanej stacji dyskietek
   byày otwierane (w systemach, kt¢re obsàuguj•
   sygnaà zmiany noònika)

   Zwraca:   0 = drzwiczki NIE BYùY otwierane
             1 = nieprawidàowy numer stacji dyskietek
             6 = drzwiczki BYùY otwierane
*/

unsigned char getChangeSignal( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x16;             /* AH = 16h - pobranie stanu sygnaàu
                                      zmiany noònika */
regs.h.dl = drive;            /* DL = nap©d. 0 dla A lub 1 dla B */
int86( 0x13, &regs, &regs );  /* Wywoàanie funkcji BIOS */
return ( regs.h.ah );         /* AH = 0, 1, lub 6 jak wyæej */
}
