/*
   BIOS dla programuj¥cach w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-2.  Okre˜lenie liczby wierszy i kolumn ekranu
*/


/*
   Funkcja zwraca:
        - bie¾¥cy tryb pracy karty graficznej
        - aktywn¥ stron©
        - liczb© wierszy i kolumn
*/
void getScreenParameters( unsigned char *videoMode,
                          unsigned char *activePage,
                          unsigned char *textRows,
                          unsigned char *textColumns )
{
#define BDA_rows (unsigned int far *)0x00400084

union REGS regs;

regs.h.ah = 0x0f;               /* AH = 0x0F - odczytaj parametry trybu pracy */
int86( 0x10, &regs, &regs );    /* Wywoˆanie funkcji BIOS */
*videoMode = regs.h.al;         /* AL = bie¾¥cy tryb */
*activePage = regs.h.bh;        /* BH = wy˜wietlana strona */
*textColumns = regs.h.ah;       /* AH = liczba wy˜wietlanych kolumn */
*textRows = *BDA_rows + 1;      /* Liczba wierszy z BDA */
}
