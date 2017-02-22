/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje obsˆugi karty graficznej
*/


/* Przerwanie 10h / 00h - Ustaw tryb karty graficznej */

void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - ustaw tryb karty graficznej */
regs.h.al = newMode;          /* AL = tryb */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 01h - Ustaw rozmiary kursora */

void setCursorType( unsigned char topScanLine, 
                    unsigned char bottomScanLine )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - ustaw rozmiar kursora */
regs.h.ch = topScanLine;      /* CH = g¢rna linia kursora */
regs.h.ch &= 15;              /* Usuä nieu¾ywane bity */
regs.h.cl = bottomScanLine;   /* CL = dolna linia kursora */
regs.h.cl &= 15;              /* Usuä nieu¾ywane bity */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 02h - Ustaw pozycj© kursora */

void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - ustaw pozycj© kursora */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.dh = row;              /* DH = numer wiersza */
regs.h.dl = column;           /* DL = numer kolumny */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 03h - Odczytanie pozycji kursora */

void getCursorPosition( unsigned char videoPage,
                        unsigned char *row,
                        unsigned char *column,
                        unsigned char *topScanLine,
                        unsigned char *bottomScanLine )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - odczytanie pozycji kursora */
regs.h.bh = videoPage;        /* BH = strona */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*topScanLine = regs.h.ch;     /* CH = g¢rna linia znaku kursora */
*bottomScanLine = regs.h.cl;  /* CL = dolna linia znaku kursora */
*row = regs.h.dh;             /* DH = numer wiersza kursora */
*column = regs.h.dl;          /* DL = numer kolumny kursora */
}



/* Przerwanie 10h / 04h - Odczytaj pozycj© p¢ra ˜wietlnego */

unsigned char readLightPen( unsigned char *row,
                            unsigned char *column,
                            unsigned int *rasterLine,
                            unsigned int *pixelColumn )
{
union REGS regs;

regs.h.ah = 4;                /* AH = 4 - odczytaj pozycj© pi¢ra ˜wietlnego */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*pixelColumn = regs.x.bx;     /* BX = kolumna pikselowa */
*rasterLine = regs.x.cx;      /* CH lub CX = linia rastra */
*row = regs.h.dh;             /* DH = wiersz */
*column = regs.h.dl;          /* DL = kolumna */
return( regs.h.ah );          /* AH = 1 (OK), or 0 (brak pi¢ra ˜wietlnego) */
}



/* Przerwanie 10h / 05h - Wybierz aktywn¥ stron© graficzn¥ */

void setVideoPage( unsigned char newPage )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - wybierz aktywn¥ stron© graficzn¥ */
regs.h.al = newPage;          /* AL = numer strony */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 06h - Przewiä ekran w g¢r© */

void scrollUp( unsigned char lines, 
               unsigned char attribute, 
               unsigned char topRow, 
               unsigned char leftColumn, 
               unsigned char bottomRow, 
               unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = 6;                /* AH = 6 - przewiä ekran w g¢r© */
regs.h.al = lines;            /* AL = liczba linii do przewini©cia */
regs.h.bh = attribute;        /* BH = atrybut dla nyowych linii */
regs.h.ch = topRow;           /* CH = g¢rny wiersz obszaru przewijanego */
regs.h.cl = leftColumn;       /* CL = lewa kolumna obszaru przewijanego */
regs.h.dh = bottomRow;        /* DH = dolny wiersz obszaru przewijanego */
regs.h.dl = rightColumn;      /* DL = prawa kolumna obszaru przewijanego */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 07h - Przewiä stron© w d¢ˆ */

void scrollDown( unsigned char lines, 
                 unsigned char attribute, 
                 unsigned char topRow, 
                 unsigned char leftColumn, 
                 unsigned char bottomRow, 
                 unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = 7;                /* AH = 7 - przewiä w d¢ˆ */
regs.h.al = lines;            /* AL = liczby linii do przewini©cia */
regs.h.bh = attribute;        /* BH = atrybut dla nowych linii */
regs.h.ch = topRow;           /* CH = g¢rny wiersz przewijanego obszaru */
regs.h.cl = leftColumn;       /* CL = lewa kolumna obszaru przewijanego */
regs.h.dh = bottomRow;        /* DH = dolny wiersz przewijanego obszaru */
regs.h.dl = rightColumn;      /* DL = prawa kolumna obszaru przewijanego */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 08h - Odczytaj znak i atrybut */

void readCell( unsigned char videoPage, 
               unsigned char *pChar, 
               unsigned char *pAttribute )
{
union REGS regs;

regs.h.ah = 8;                /* AH = 8 - odczytaj kom¢rk© */
regs.h.bh = videoPage;        /* BH = strona */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*pAttribute = regs.h.ah;      /* AH = atrybut */
*pChar = regs.h.al;           /* AL = znak */
}



/* Przerwanie 10h / 09h - Napisanie znaku i atrybutu */

void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - zapisz kom¢rk© */
regs.h.al = character;        /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.bl = attribute;        /* BL = atrybut */
regs.x.cx = times;            /* CX = liczba powt¢rzeä znaku */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 0Ah - Napisz tylko znak */

void writeChar( unsigned char videoPage, 
                unsigned char ch, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 0x0a;             /* AH = 0x0A - napisz znak */
regs.h.al = ch;               /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona */
regs.x.cx = times;            /* CX = liczba powt¢rzeä znaku */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 0Bh - Ustawienie palety barw */

void setPalette( unsigned char function,
                 unsigned char color )
{
union REGS regs;

regs.h.ah = 0x0b;             /* AH = 0x0B - ustaw palet© barw */
regs.h.bh = function;         /* BH = numer funkcji (0 lub 1) */
regs.h.bl = color;            /* BL = nowy kolor lub paleta */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 0Ch - Rysuj piksel */

void writePixel( unsigned char color, 
                 unsigned char videoPage, 
                 unsigned int row, 
                 unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0c;             /* AH = 0x0C - rysuj piksel */
regs.h.al = color;            /* AL = kolor */
regs.h.bh = videoPage;        /* BH = strona */
regs.x.cx = column;           /* CX = kolumna (x) */
regs.x.dx = row;              /* DX = wiersz (y) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 0Dh - Odczytaj piksel */

unsigned char readPixel( unsigned char videoPage, 
                         unsigned int row, 
                         unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0d;             /* AH = 0x0D - czytaj piksel */
regs.h.bh = videoPage;        /* BH = strona video */
regs.x.cx = column;           /* CX = kolumna (x) */
regs.x.dx = row;              /* DX = wiersz (y) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.al );          /* AL = kolor piksela */
}



/* Przerwanie 10h / 0Eh - Napisz znak, przesuä kursor */

void writeTTYChar( unsigned char videoPage, 
                   unsigned char ch, 
                   unsigned char attribute )
{
union REGS regs;

regs.h.ah = 0x0e;             /* AH = 0x0E - napisz znak */
regs.h.al = ch;               /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.bl = attribute;        /* BL = atrybut (w trybach graficznych) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 10h / 0Fh - Odczytaj bie¾¥cy stan karty graficznej */

void readVideoState( unsigned char *columns, 
                     unsigned char *currentMode, 
                     unsigned char *currentPage )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0x0F - odczytaj stan */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*columns = regs.h.ah;         /* AH = liczba wy˜wietlanych kolumn */
*currentMode = regs.h.al;     /* AL = bie¾¥cy tryb */
*currentPage = regs.h.bh;     /* BH = wy˜wietlana strona */
}



/* Przerwanie 10h / 1A00h - Odczytaj kod typu karty */
void getVideoType( unsigned char *primary,
                   unsigned char *secondary )
{
union REGS regs;

regs.h.ah = 0x1a;             /* AH = 0x1A - odczytaj kody typu karty */
regs.h.al = 0;                /* AL = 0 */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
if ( regs.h.al == 0x1a )      /* AL = 0x1A je¾eli funkcja jest obsˆugiwana */
{
   *primary = regs.h.bl;      /* BL = kod pierwszej karty graficznej */
   *secondary = regs.h.bh;    /* BH = kod drugiej karty graficznej */
   }
else                          /* Je¾eli funkcja nie jest obsˆugiwana */
{
   *primary = 0xff;           /* Zwr¢† informacj©, ¾e obydwie karty */
   *secondary = 0xff;         /* s¥ nieznanego typu */
   }
}

