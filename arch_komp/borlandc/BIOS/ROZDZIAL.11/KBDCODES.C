/* ***************************************************
   KBDCODES.C 
   Copyright (c) 1992 - Billy P. Taylor
   ***************************************************
   Program demonstruje u¾ycie funkcji 0, 1, 10h i 11h
   do odczytywania i wy˜wietlania informacji otrzymywanych
   z klawiatury. Umo¾liwia sprawdzenie stanu klawiatury
   i kod¢w klawiszy i znak¢w generowanych przy naciskaniu
   poszczeg¢lnych klawiszy.

   Uwaga: Program wymaga u¾ycia kompilatora umo¾liwiaj¥cego
   u¾ycie instrukcji asemblera w tek˜cie programu.

   Uruchomienie: KBDCODES<Enter>.
   *************************************************** */

#include <ctype.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

char *mainScreen[] = { 
"Przerw.   16h Funkcja  02h   Funkcja  12h   Funkcja  12h ",
"              Rejestr   AL   Rejestr   AL   Rejestr   AH ",
"     Insert :   .              .              .          : SysReq               ",
"  Caps Lock :   .              .              .          : Caps Lock            ",
"   Num Lock :   .              .              .          : Num Lock             ",
"Scroll Lock :   .              .              .          : Scroll Lock          ",
"        Alt :   .              .              .          : Prawy Alt            ",
"       Ctrl :   .              .              .          : Prawy Ctrl           ",
" Lewy Shift :   .              .              .          : Lewy Alt             ",
"Prawy Shift :   .              .              .          : Lewy Ctrl            ",
"                                                                                ",
"Przerw.   16h  Funkcja  11h                                                     ",
"              Kod klaw./znaku                                                   ",
0 };

/* 
   Okre˜lenie wierszy i kolumn, w kt¢rych wy˜wietlane
   b©d¥ informacje o stanie klawiatury.
   Zakˆadany jest ekran 80x25 znak¢w. Tryb taki
   ustawiany jest w funkcji main() (tryb 3).
*/
#define TEXT_MODE           3  /* Tryb pracy ekranu */

#define ROW_DYNAMIC_STATUS  2  /* Wiersz wy˜wietlania stanu klawiatury */
#define COL_FUNC02h_AL     16  /* Kolumna dla wyniku funkcji 2 przerwania 16h */
#define COL_FUNC12h_AL     31  /* Kolumna dla wyniku funkcji 12 przerw. 16h (AL) */
#define COL_FUNC12h_AH     46  /* Kolumna dla wyniku funkcji 12 przerw. 16h (AH) */

#define ROW_TOP_SCAN_CODES 13  /* G¢rny wiersz okna danych z klawiatury */
#define ROW_BOT_SCAN_CODES 24  /* Dolny wiersz okna danych z klawiatury */
#define COL_FUNC11h_LEFT   15  /* Lewa kolumna wy˜wietlania wyniku funkcji 11h */
#define COL_FUNC11h_RIGHT  25  /* Prawa kolumna wy˜wietlania wyniku funkcji 11h */

#define ONCE   1
#define NORMAL 7

char pressed[]  = "Wci˜ni©ty",
     released[] = "    .    ";

/* PROTOTYPY FUNKCJI */
void          displayBits              ( unsigned char, 
                                         unsigned char, 
                                         unsigned char );
unsigned char getCurrentPage           ( void );
unsigned int  getEnhancedKbdChar       ( void );
unsigned int  getKbdChar               ( void );
unsigned char getVideoMode             ( void );
unsigned int  readExtendedShiftStatus  ( void );
unsigned char readShiftStatus          ( void );
void          scrollUp                 ( unsigned char, 
                                         unsigned char, 
                                         unsigned char,
                                         unsigned char, 
                                         unsigned char, 
                                         unsigned char );
void          setCursorPosition        ( unsigned char, 
                                         unsigned char, 
                                         unsigned char );
void          setVideoMode             ( unsigned char );
void          showCodes                ( unsigned char, 
                                         unsigned char, 
                                         unsigned char, 
                                         unsigned char );
unsigned int  testEnhancedKbdCharReady ( void );
unsigned int  testKbdCharReady         ( void );

void main( void )
{
unsigned char status,
              lastStatus,
              flagsRegister;

unsigned int  extendedStatus,
              lastExtendedStatus,
              scanAndCharCode,
              row;

/* Ustawienie trybu 80x25 i przygotowanie ekranu */
setVideoMode( TEXT_MODE );
for( row=0; mainScreen[row] != 0; row++)
{
   setCursorPosition( getCurrentPage(), row, 0 );
   printf( "%s", mainScreen[row] );
   }

/* Program pracuje w p©tli dop¢ki nie naci˜niesz Ctrl-Break */
for(;;)
{
/* Odczytanie stanu klawiszy klawiatury 83/84 */
   status = readShiftStatus();

/* Je¾eli si© zmieniˆ, wy˜wietlenie */
   if ( status != lastStatus )
   {
      displayBits( ROW_DYNAMIC_STATUS, 
                   COL_FUNC02h_AL, 
                   status );
      lastStatus = status;
      }

/* Odczytanie stanu klawiatury rozszerzonej */
   extendedStatus = readExtendedShiftStatus();

/* Je¾eli si© zmieniˆ, wy˜wietlenie */
   if ( extendedStatus != lastExtendedStatus )
   {
      displayBits( ROW_DYNAMIC_STATUS, 
                   COL_FUNC12h_AL, 
                   extendedStatus );
      displayBits( ROW_DYNAMIC_STATUS, 
                   COL_FUNC12h_AH, 
                   (extendedStatus >> 8) );
      lastExtendedStatus = extendedStatus;
      }

/* Sprawdzenie czy naci˜ni©to klawisz */
   scanAndCharCode = testEnhancedKbdCharReady();
   if ( scanAndCharCode != 0 )
   {
      scrollUp( ONCE, 
                NORMAL, 
                ROW_TOP_SCAN_CODES, 
                COL_FUNC11h_LEFT, 
                ROW_BOT_SCAN_CODES, 
                COL_FUNC11h_RIGHT );

      scanAndCharCode = getEnhancedKbdChar();

      showCodes( scanAndCharCode >> 8, 
                 scanAndCharCode, 
                 ROW_BOT_SCAN_CODES, 
                 COL_FUNC11h_LEFT );
      }
   }
}

/* Wy˜wietlenie bajtu jako kolumny bit¢w */
void displayBits( unsigned char row, 
                  unsigned char col, 
                  unsigned char bitMask )
{
unsigned char bitToTest;
bitToTest = 128;
do
{
   setCursorPosition( getCurrentPage(), row, col );

   if ( (bitMask & bitToTest) )
      printf( "%s", pressed );
   else
      printf( "%s", released );

   ++row;
   bitToTest >>= 1;
   } while ( bitToTest );
}

/* Wy˜wietlenie kodu klawisza i znaku */
void showCodes( unsigned char scanCode, 
                unsigned char charCode, 
                unsigned char row, 
                unsigned char col )
{
setCursorPosition( getCurrentPage(), row, col );
printf( "%.2Xh - %.2Xh %c", 
         scanCode, 
         charCode,
         ( isprint(charCode) ) ? charCode : ' ' );
}

/* Ustawienie trybu karty graficznej */
void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - ustaw tryb karty grafiki */
regs.h.al = newMode;          /* AL = tryb */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}

/* Odczytanie bie¾¥cego trybu karty graficznej */
unsigned char getVideoMode( void )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0Fh - podaj bie¾¥cy tryb karty grafiki */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.al );          /* AL = bie¾¥cy tryb */
}

/* Odczytanie aktywnej strony karty graficznej */
unsigned char getCurrentPage( void )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0Fh - podaj bie¾¥c¥ stron© */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.bh );          /* BH = bie¾¥ca strona */
}

/* Przemie˜† kursor */
void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - ustawienie pozycji kursora */
regs.h.bh = videoPage;        /* BH = strona */
regs.h.dh = row;              /* DH = numer wiersza */
regs.h.dl = column;           /* DL = numer kolumny */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}

/* Przewiä ekran */
void scrollUp( unsigned char times, 
               unsigned char attribute, 
               unsigned char topRow, 
               unsigned char leftColumn, 
               unsigned char bottomRow, 
               unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = 6;                /* AH = 6 - przewini©cie ekranu */
regs.h.al = times;            /* AL = liczba wierszy do przewini©cia */
regs.h.bh = attribute;        /* BH = atrybut dla nowych linii */
regs.h.ch = topRow;           /* CH = numer g¢rnego wiersza okna */
regs.h.cl = leftColumn;       /* CL = lewa kolumna okna */
regs.h.dh = bottomRow;        /* DH = dolny wiersz okna */
regs.h.dl = rightColumn;      /* DL = prawa kolumna okna */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}

/* Odczytanie nast©pnego kodu klawisza (starsze 8 bit¢w) 
   i kodu znaku (mˆodsze 8 bit¢w) 
*/
unsigned int getKbdChar( void )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - Odczytaj znak z klawiatury */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AH = kod klawisza, AL = kod znaku */
}

/* Sprawdzenie, czy naci˜ni©to jaki˜ klawisz.
   Funkcja zwraca kod klawisza (starsze 8 bit¢w)
   i kod znaku (mˆodsze 8 bit¢w). Operacja musi
   by† wykonana w asemblerze, poniewa¾ do sygnalizacji
   u¾ywany jest znacznik Zero niedost©pny w standardowej
   strukturze REGS.
*/
unsigned int testKbdCharReady( void )
{
unsigned int returnValue;

_asm  mov   AH, 1               /* AH = 1 - sprawdzenie czy naci˜ni©to klawisz */
_asm  int   16h                 /* Wywoˆanie funkcji BIOS */
_asm  jnz   endFunc1            /* Zero = 0 - naci˜ni©to klawisz */
_asm  xor   AX,AX               /* Nie ma nowych danych - zwracamy 0 */
endFunc1:
_asm  mov   returnValue, AX     /* AH = kod klawisza, AL = kod znaku */
return( returnValue );          /* Zwracamy kody */
}

/* Odczytanie tradycyjnego (o˜miobitowego) sˆowa stanu klawiatury */
unsigned char readShiftStatus( void )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - podaj stan klawiatury */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.al );          /* AL = kody stanu klawiszy */
}

/* Odczytanie rozszerzonego kodu klawisza (starsze 8 bit¢w) 
   oraz kodu znaku (mˆodsze 8 bit¢w) 
*/
unsigned int getEnhancedKbdChar( void )
{
union REGS regs;

regs.h.ah = 0x10;             /* AH = 10h - odczytanie znaku z klawiatury */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AH = kod klawisza, AL = kod znaku */
}

/* Sprawdzenie, czy naci˜ni©to jaki˜ klawisz
   (wersja dla klawiatury rozszerzonej).
   Funkcja zwraca kod klawisza (starsze 8 bit¢w)
   i kod znaku (mˆodsze 8 bit¢w). Operacja musi
   by† wykonana w asemblerze poniewa¾, do sygnalizacji
   u¾ywany jest znacznik Zero niedost©pny w standardowej
   strukturze REGS.
*/
unsigned int testEnhancedKbdCharReady( void )
{
unsigned int  returnValue;

_asm  mov   AH, 11h             /* AH = 0x11 - sprawdzenie czy naci˜ni©to klawisz */
_asm  int   16h                 /* Wywoˆanie funkcji BIOS */
_asm  jnz   endFunc11           /* Zero = 0 - naci˜ni©to klawisz */
_asm  xor   AX,AX               /* Nie ma nowych danych - zwracamy 0 */
endFunc11:
_asm  mov   returnValue, AX     /* AH = kod klawisza, AL = kod znaku */
return( returnValue );          /* Zwracamy kody */
}

/* Odczytanie stanu klawiatury rozszerzonej */
unsigned int readExtendedShiftStatus( void )
{
union REGS regs;

regs.h.ah = 0x12;             /* AH = 12h - odczytanie stanu klaw. rozszerzonej */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AH = rozszerzona inf. o stanie */
                              /* AL = konwencjonalna informacja o stanie */
}

