/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-14.  Przewini©cie ekranu w g¢r© lub w d¢ˆ
*/ 


/* Przewini©cie cz©˜ci ekranu w g¢r© lub w d¢ˆ */

#define scrollUp(nLines,color,tr,lc,br,rc)   \
        scrollScreen(6,nLines,color,tr,lc,br,rc)  /* funkcja 6 */

#define scrollDown(nLines,color,tr,lc,br,rc) \
        scrollScreen(7,nLines,color,tr,lc,br,rc)  /* funkcja 7 */

void scrollScreen( unsigned char BIOSFunc6or7,
                   unsigned char nLines, 
                   unsigned char attribute, 
                   unsigned char topRow, 
                   unsigned char leftColumn, 
                   unsigned char bottomRow, 
                   unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = BIOSFunc6or7;     /* AH = 6 (przewiä w g¢r©), 7 (lub w d¢ˆ) */
regs.h.al = nLines;           /* AL = liczba linii do przewini©cia */
                              /*      lub 0 - caˆy obszar */
regs.h.bh = attribute;        /* BH = atrybut dla nowych linii */
regs.h.ch = topRow;           /* CH = g¢rny wiersz obszaru przewijanego */
regs.h.cl = leftColumn;       /* CL = lewa kolumna obszaru przewijanego */
regs.h.dh = bottomRow;        /* DH = dolny wiersz obszaru przewijanego */
regs.h.dl = rightColumn;      /* DL = prawa kolumna obszaru przewijanego */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
