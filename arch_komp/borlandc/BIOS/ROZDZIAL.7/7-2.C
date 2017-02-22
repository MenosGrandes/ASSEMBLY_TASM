/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-2.  Odczytanie tekstu z klawiatury
*/


/* 
   Funkcja og¢lnego przeznaczenia czytaj¥ca wprowadzany
   tekst za pomoc¥ funkcji przerwania 16h
*/

#include <stdio.h>
#include <ctype.h>
#include <dos.h>

/* Prototypy funkcji */
void getCursorPosition ( unsigned char, 
                         unsigned char *, 
                         unsigned char * );
void getKbdChar        ( unsigned char *,
                         unsigned char * );
void setCursorPosition ( unsigned char, 
                         unsigned char, 
                         unsigned char );
void writeChar         ( unsigned char, 
                         unsigned char, 
                         unsigned int );

/* *** Tutaj zaczynaj¥ si© funkcje *** */
void readKbdString( unsigned char videoPage,   /* Strona */
                    unsigned char row,         /* Pocz¥tkowy wiersz */
                    unsigned char column,      /* Pocz¥tkowa kolumna */
                    unsigned char maxLength,   /* Maksymalna dˆugo˜† */
                    unsigned char *bufPtr )    /* Wska«nik do bufora */
{
#define BACKSPACE  8    /* Kod znaku Backspace */
#define ENTER_KEY 13    /* Kod znaku Enter */
#define WRITE_ONCE 1

union REGS regs;

unsigned char scanCode,
              charCode,
              currentLength;

for( currentLength = 0, *bufPtr = 0; ; )
{
/* Ustawienie pozycji kursora dla bie¾¥cego znaku */
   setCursorPosition( videoPage, row, column );

/* Odczytanie nast©pnego znaku */
   getKbdChar( &charCode, &scanCode );

   switch ( charCode )
   {
/* Obsˆuga klawisza Backspace */
      case BACKSPACE:
         if ( currentLength != 0 )   /* i dˆugo˜† tekstu != 0 */
         {
/* Przesuni©cie kursora do poprzedniej kolumny */
            --column;
            setCursorPosition( videoPage, row, column );
/* Wymazanie znaku na tej pozycji */
            writeChar( videoPage, ' ', WRITE_ONCE );
/* Zmniejszenie o 1 wska«nika bufora i zakoäczenie tekstu zerem */
            --bufPtr;
            *bufPtr = 0;
/* Zmniejszenie o 1 bie¾¥cej dˆugo˜ci tekstu */
            --currentLength;
            }
         break;
/* Je¾eli naci˜ni©to Enter, skoäcz */
      case ENTER_KEY:
         return;
/* Obsˆuga pozostaˆych klawiszy */
      default:
/* Je¾eli jest wolne miejsce w buforze wczytaj nast©pne znaki */
         if ( ( currentLength != maxLength ) && 
/* oczywi˜cie je¾eli s¥ one drukowalne */
              ( isprint(charCode) ) )
         {
/* Dodanie znaku do bufora */
            *bufPtr = charCode;
/* Zwi©kszenie wska«nika bufora i zakoäczenie tekstu zerem */
            ++bufPtr;
            *bufPtr = 0;
/* Wy˜wietlenie znaku na ekranie */
            writeChar( videoPage, charCode, WRITE_ONCE );
/* Zmiana kolumny i dodanie 1 do dˆugo˜ci tekstu */
            ++column;
            ++currentLength;
            }
         break;
      }
   }
}

/*
   Zwraca nast©pn¥ par© kod klawisza/kod znaku 
   z bufora klawiatury
*/

void getKbdChar( unsigned char *charCode,
                 unsigned char *scanCode )
{
union REGS regs;
regs.h.ah = 0;                /* AH = 0 lub 10h - odczytanie znaku  */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*scanCode = regs.h.ah;        /* AH = kod klawisza */
*charCode = regs.h.al;        /* AL = kod znaku */
}                             /* Niczego nie zwracamy */

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

/* Zwraca pozycj© kursora */
void getCursorPosition( unsigned char videoPage, 
                        unsigned char *row, 
                        unsigned char *column )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - pobranie pozycji kursora */
regs.h.bh = videoPage;        /* BH = strona */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

*row = regs.h.dh;             /* DH = bie¾¥cy wiersz */
*column = regs.h.dl;          /* DL = bie¾¥ca kolumna */
}

/*
   Wypisanie znaku n razy 
   poczynaj¥c od bie¾¥cej pozycji kursora
*/
void writeChar( unsigned char videoPage, 
                unsigned char character, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 0x0a;             /* AH = 0x0A - wy˜wietl znak */
regs.h.al = character;        /* AL = znak */
regs.h.bh = videoPage;        /* BH = strona video */
regs.x.cx = times;            /* CX = krotno˜† wypisania znaku */
int86( 0x10, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}                             /* Niczego nie zwracamy */
