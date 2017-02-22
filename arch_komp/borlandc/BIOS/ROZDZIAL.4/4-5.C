/*
   BIOS dla programuj¥cach w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-5.  Odczytanie i ustawienie pozycji kursora
*/


/*
   Funkcja zwraca g¢rn¥ i doln¥ lini© kursora
   dla bie¾¥cego trybu tekstowego
*/
void getCursorSize( unsigned char *topScanLine, 
                    unsigned char *bottomScanLine )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - odczytanie pozycji kursora */
regs.h.bh = 0;                /* BH = strona (wszystko jedno kt¢ra) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*topScanLine = regs.h.ch;     /* CH = g¢rna linia znaku kursora */
*bottomScanLine = regs.h.cl;  /* CL = dolna linia znaku kursora */
}


/* Ustawienie rozmiaru kursora dla trybu tekstowego */
void setCursorSize( unsigned char topScanLine, 
                    unsigned char bottomScanLine )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - ustaw rozmiar kursora */
regs.h.ch = topScanLine;      /* CH = g¢rna linia kursora */
regs.h.ch &= 31;              /* Usuä nieu¾ywane bity */
regs.h.cl = bottomScanLine;   /* CL = dolna linia kursora */
regs.h.cl &= 31;              /* Usuä nieu¾ywane bity */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
