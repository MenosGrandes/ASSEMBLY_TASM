/* ***************************************************
   POPUP.C
   Copyright (c) 1992 - Billy P. Taylor
   ***************************************************
   U¾ycie stron grafiki do utworzenia okienka
   z komunikatem w trybie tekstowym
   ************************************************** */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>


/* Prototypy funkcji */
void copyPage            ( unsigned char, 
                           unsigned char,
                           unsigned char,
                           unsigned char );
void getScreenParameters ( unsigned char *,
                           unsigned char *,
                           unsigned char *,
                           unsigned char * );
void makeVideoPageActive ( unsigned char );
void popup               ( unsigned char * );
void readCell            ( unsigned char,  
                           unsigned char *,  
                           unsigned char * );
void setCursorPosition   ( unsigned char, 
                           unsigned char, 
                           unsigned char );
void writeCell           ( unsigned char, 
                           unsigned char, 
                           unsigned char, 
                           unsigned int );

#define FG_WHITE    7         /* Atrybuty znaku */
#define BG_RED     64
#define INTENSE     8
#define POPUP_ATTRIBUTE FG_WHITE + BG_RED + INTENSE

void main( int argc, char *argv[] )
{
if ( argc > 1 )
   popup( argv[1] );
else
   popup( " Naci˜nij dowolny klawisz " );
}

/* Utworzenie okienka */
void popup( unsigned char *string )
{
unsigned char activePage,    popupPage,
              currentMode,   maxRows,
              maxColumns,    msgRow,
              msgColumn;
/* 
   Musimy zna† aktywn¥ stron©
   oraz liczb© wierszy i kolumn
*/
getScreenParameters( &currentMode, 
                     &activePage, 
                     &maxRows, 
                     &maxColumns );

/* Wybranie nieu¾ywanej strony */
popupPage = activePage + 1;

if ( popupPage >= 8 )
   popupPage = 0;

/*
   Skopiowanie aktywnej strony na inn¥ woln¥ stron©
*/
copyPage( activePage, popupPage, maxRows, maxColumns );

/* Wy˜rodkuj komunikat */
msgRow = maxRows / 2;
msgColumn = (maxColumns - strlen( string )) / 2;

/* Napisanie komunikatu na dodatkowej stronie */
while( *string != 0 )
{
   setCursorPosition( popupPage, msgRow, msgColumn );
   writeCell( popupPage, *string, POPUP_ATTRIBUTE, 1 );
   ++string;
   ++msgColumn;
   }

/* Wy˜wietlenie dodatkowej strony z komunikatem */
makeVideoPageActive( popupPage );

/* Zaczekaj na naci˜ni©cie klawisza */
getch();

/* Odtw¢rz oryginaln¥ stron© - komunikat znika */
makeVideoPageActive( activePage );
}

/*
   Funkcja zwraca bie¾¥cy tryb graficzny, stron© graficzn¥
   oraz liczb© kolumn i wierszy
*/
void getScreenParameters( unsigned char *videoMode,
                          unsigned char *activePage,
                          unsigned char *textRows,
                          unsigned char *textColumns )
{
#define BDA_rows (unsigned int far *)0x00400084
union REGS regs;

regs.h.ah = 0x0f;               /* AH = 0Fh - podaj parametry trybu graficznego */
int86( 0x10, &regs, &regs );    /* Wywoˆanie funkcji BIOS */

*videoMode = regs.h.al;         /* AL = tryb */
*activePage = regs.h.bh;        /* BH = numer aktywnej strony */
*textColumns = regs.h.ah;       /* AH = liczba kolumn */
*textRows = *BDA_rows + 1;      /* liczba wierszy z BDA */
}

/* Wy˜wietlenie wybranej strony */
void makeVideoPageActive( unsigned char whichPage )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - Ustaw aktywn¥ stron© */
regs.h.al = whichPage;        /* AL = numer strony (0..7) */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}

/* Ustawienie pozycji kursora */
void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - ustawienie pozycji kursora */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.dh = row;              /* DH = wiersz */
regs.h.dl = column;           /* DL = kolumna */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */

/* Napisanie znaku i atrybutu na wybranej stronie */
void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - napisanie znaku i atrybutu */
regs.h.al = character;        /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.bl = attribute;        /* BL = atrybut */
regs.x.cx = times;            /* CX = liczba powt¢rzeä */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */

/* Odczytanie znaku i atrybutu z pozycji kursora */
void readCell( unsigned char videoPage,  
               unsigned char *pChar,  
               unsigned char *pAttribute ) 
{ 
union REGS regs; 

regs.h.ah = 8;                /* AH = 8 - odczytaj kom¢rk© znaku */ 
regs.h.bh = videoPage;        /* BH = strona */ 
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */ 
*pChar = regs.h.al;           /* AL = znak */ 
*pAttribute = regs.h.ah;      /* AH = atrybut */ 
}

/* Kopiowanie strony */
void copyPage( unsigned char source, 
               unsigned char dest,
               unsigned char maxRows,
               unsigned char maxColumns )
{
unsigned char currentRow,
              currentColumn;
unsigned char character,
              attribute;

for( currentRow = 0; currentRow != maxRows; currentRow++ )
{
   for( currentColumn = 0; currentColumn != maxColumns; currentColumn++ )
   {
      setCursorPosition( source, currentRow, currentColumn );
      readCell( source, &character, &attribute );
      setCursorPosition( dest, currentRow, currentColumn );
      writeCell( dest, character, attribute, 1 );
      }
   }
}

