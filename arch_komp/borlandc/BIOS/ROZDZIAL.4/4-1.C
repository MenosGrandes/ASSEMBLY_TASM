/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-1.  Funkcje podaj¥ca i ustawiaj¥ca tryb pracy
                 karty graficznej
*/


/* Ustaw tryb karty graficznej */
void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                   /* AH = 0 - ustaw tryb karty graficznej */
regs.h.al = newMode;             /* AL = tryb */
int86( 0x10, &regs, &regs );     /* Wywoˆanie funkcji BIOS */
}                                /* Niczego nie zwracamy */


/* Odczytaj bie¾¥cy tryb karty graficznej */
unsigned char getVideoMode( void )
{
union REGS regs;

regs.h.ah = 0x0f;               /* AH = 0x0F - odczytaj stan karty */
int86( 0x10, &regs, &regs );    /* Wywoˆanie funkcji BIOS */
return( regs.h.al );            /* AL = bie¾¥cy tryb */
}
