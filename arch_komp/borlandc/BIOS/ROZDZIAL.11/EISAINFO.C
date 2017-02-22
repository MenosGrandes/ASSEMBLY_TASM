/* **********************************************************
   EISAINFO.C 
   Copyright (c) 1992 - Billy P. Taylor
   **********************************************************
   Wyòwietlenie zawartoòci wszystkich gniazd EISA na podstawie
   informacji zwracanych przez funkcj© D800h przerwania 15h
   oraz dodatkowych informacji zawartych w plikach
   konfiguracyjnych kart.

   Aby uruchomiÜ program napisz:
     EISAINFO <òcieækaDoPlik¢wKonfiguracyjnych>
   ******************************************************* */

/* ******** STAùE, MAKRODEFINICJE i INNE NIEZB®DNE RZECZY ********* */
#include <ctype.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define boolean unsigned char
#define YES 1
#define NO  0

#define EISA_BIOS 0x15
#define MAX_SLOT    64        /* Maksymalna liczba gniazd */

/* Kody bà©d¢w funkcji D8xxh przerwania 15h */
#define INFO_READ_OK  0x00
#define NO_MORE_SLOTS 0x80
#define NVRAM_CORRUPT 0x82
#define SLOT_EMPTY    0x83
#define SLOT_MISMATCH 0x87

#define BIT0   1  /* 00000001 */
#define BIT1   2  /* 00000010 */
#define BIT2   4  /* 00000100 */
#define BIT3   8  /* 00001000 */
#define BIT4  16  /* 00010000 */
#define BIT5  32  /* 00100000 */
#define BIT6  64  /* 01000000 */
#define BIT7 128  /* 10000000 */

/* Wielokrotne wyst•pienie ID i numer wyst•pienia
   zwracane przez funkcj© D800h przerwania 15h */
#define DUP_NR   (BIT0 + BIT1 + BIT2 + BIT3)
#define DUP_FLAG (BIT4 + BIT5)

/* *** ZMIENNE GLOBALNE *** */
boolean searchConfigFiles = NO,
        moreSlotsToCheck = YES;

char *configFileDirectory;

/* Struktura definiuj•ca skompresowane ID karty jako pola bitowe */
union boardIdStruct
{
unsigned int compressed[2];
struct bitFields
{
   unsigned int letter2a : 2;
   unsigned int letter1  : 5;
   unsigned int unused   : 1;
   unsigned int letter3  : 5;
   unsigned int letter2b : 3;
   unsigned int number2  : 4;
   unsigned int number1  : 4;
   unsigned int number4  : 4;
   unsigned int number3  : 4;
	} bits;
} boardId;

union  REGS  regs;
struct SREGS sregs;

char boardName[128],
     vendor[128];

/* *** PROTOTYPY FUNKCJI *** */
void          describeSlot            ( int );
void          expandEISAId            ( unsigned int, 
                                        unsigned int, 
                                        char * );
unsigned char hexToAscii              ( char );
void          openAndSearchConfigFile ( char *, 
                                        char *, 
                                        char * );

/* ***  MAIN  *** */
void main( int argc, char *argv[] )
{
static unsigned char eisaString[] = "EISA";
static unsigned char far *EISA_SIG = 0xf000ffd9;

int i, slot;

/* Sprawdzenie, czy to jest komputer z magistral• EISA */
for( i = 0; i < strlen( eisaString ); i++ )
{
   if ( EISA_SIG[i] != eisaString[i] )
      break;
   }
if ( i != strlen( eisaString ) )
{
   printf( "Ten program wymaga komputera z magistral• EISA.\7" );
   exit( 1 );
   }

/* argv[1] jest òcieæk• do plik¢w konfiguracyjnych (jeæeli j• podano) */
if ( argc > 1 )               /* Jeæeli podano òcieæk© */
{
   searchConfigFiles = YES;    /* przejrzyj pliki konfiguracyjne */
   configFileDirectory = argv[1];
   }

/* Opisz kaæde gniazdo */
for( slot = 0; ((slot <= MAX_SLOT) && moreSlotsToCheck ); slot++ )
   describeSlot( slot );

exit( 0 );
}

/* Opisz podane gniazdo.  Ustaw zmienn• 
   moreSlotsToCheck na faàsz (FALSE) kiedy
   dojdziesz do ostatniego gniazda
*/
void describeSlot( int slotNumber )
{
static char nullString[] = {0};
unsigned char boardID[10];

regs.x.ax = 0xd800;                /* AX = D800h - podaj info gniazda */
regs.h.cl = slotNumber;            /* CL = numer gniazda */
int86( EISA_BIOS, &regs, &regs );  /* Wywoàaj funkcj© BIOS */

printf( "\n" );

if ( regs.h.ah == NO_MORE_SLOTS )
{
   printf( "* Nie ma wi©cej gniazd *" );
   moreSlotsToCheck = NO;
   return;
   }

/* Jeæeli dane s• uszkodzone, lub gniazdo zawiera
   inn• kart© niæ zostaàa skonfigurowana,
   wyòwietl ostrzeæenie.
*/
if ( (regs.h.ah == NVRAM_CORRUPT) || 
     (regs.h.ah == SLOT_MISMATCH) )
{
   printf( "*** NIEPRAWIDùOWE DANE W PAMI®CI KONFIGURACYJNEJ EISA! ***\7" );
   moreSlotsToCheck = NO;
   return;
   }

/* Kontynuuj jeæeli dane zostaày odczytane prawidàowo
   lub gniazdo jest puste 
*/
if ( (regs.h.ah == INFO_READ_OK) || 
     (regs.h.ah == SLOT_EMPTY) )
{
   printf( "Numer gniazda................[%u]", slotNumber);

   if ( regs.h.ah == SLOT_EMPTY )
      printf( " puste\n", slotNumber);
   else
   {
      expandEISAId( regs.x.di, regs.x.si, boardID );

      printf( "\n" );
      printf( "Identyfikator karty..........[%s]\n", boardID);

      if ( searchConfigFiles )
      {
         openAndSearchConfigFile( boardID, boardName, vendor );

/* Wyr¢wnywanie do lewej
   pola mog• byÜ dàugie i nie zmieòciÜ si© w jednej linii */
         printf( "Nazwa karty..[%s]\n", boardName);
         printf( "Wytw¢rca.....[%s]\n", vendor);
         }

      printf( "Wersja pr. konfiguracyjnego..[%u.%u]\n", 
                regs.h.bh, regs.h.bl );

      printf( "Funkcje w tym gnie´dzie......[%u]\n", regs.h.dh );

      printf( "Karta jest..................." );

      if ( (regs.h.al & BIT7) != 0 )
         printf( "[Pojedyncza]\n" );
      else
         printf( "[%u egzemplarzem]\n", (1 + (regs.h.al & DUP_NR)) );

      printf( "Identyfikator gniazda........[%czytelny]\n",
               ( (regs.h.al & BIT6) != 0 ) ? "Niec" : "C" );

      printf( "Rodzaj gniazda...............[" );
      switch ( (regs.h.al & DUP_FLAG) )
      {
         case 0x00   :  printf( "Rozszerzaj•ce]\n" );
                        break;
         case BIT4   :  printf( "Wbudowane]\n" );
                        break;
         case BIT5   :  printf( "Wirtualne]\n" );
                        break;
         default     :  printf( "Nieznane]\n" );
                        break;
         }
      }
   }
}

/* Rozwini©cie skompresowanego ID karty EISA
   do tekstu zako‰czonego zerem
*/
void expandEISAId( unsigned int high16Bits,    /* DI z D800h */
                   unsigned int low16Bits,     /* SI z D800h */
                   char *stringP )             /* bufor wyjòciowy */
{
boardId.compressed[0] = high16Bits;
boardId.compressed[1] = low16Bits;

*(stringP+0) = boardId.bits.letter1 + '@';          /* Bajt 1 z 7 */
*(stringP+1) = (boardId.bits.letter2a * 8) +        /* Bajt 2 z 7 */
					 boardId.bits.letter2b + '@';
*(stringP+2) = boardId.bits.letter3 + '@';          /* Bajt 3 z 7 */
*(stringP+3) = hexToAscii( boardId.bits.number1 );  /* Bajt 4 z 7 */
*(stringP+4) = hexToAscii( boardId.bits.number2 );  /* Bajt 5 z 7 */
*(stringP+5) = hexToAscii( boardId.bits.number3 );  /* Bajt 6 z 7 */
*(stringP+6) = hexToAscii( boardId.bits.number4 );  /* Bajt 7 z 7 */
*(stringP+7) = 0;                                   /* Ko‰cz•ce zero */
}

/* Konwersja szesnastkowej cyfry z postaci binarnej do ASCII */
unsigned char hexToAscii( char character )
{
if ( (character >= 0) && (character <= 9) )
   return (character + '0');

if ( (character >= 0x0a) && (character <= 0x0f) )
   return (character - 0x0a + 'A');

return( 0 );
}

/* Znalezienie pliku konfiguracyjnego karty EISA
   na podstawie jej identyfikatora. W przypadku
   powodzenia funkcja zwraca 0, a bufor zawiera
   peàn• nazw© pliku konfiguracyjnego wraz ze
   òcieæk• (gotow• do uæycia przez open()).
   W przypadku bà©du funkcja zwraca DOSowski kod
   bà©du i jego wyjaònienie w buforze.
*/
unsigned int findConfigFile( char *boardName,
                             char *pathToSearch,
                             char *outputBuffer )
{
char scratch[128];
char far *scratchP;

struct DTA
{  char reserved[21];
   unsigned char attributes;
   unsigned int  creationTime;
   unsigned int  creationDate;
   unsigned long fileSizeInBytes;
   char fileName[13];
   } far *DTA_Ptr;

/* Pobranie bieæ•cego adresu BDA */
regs.h.ah = 0x2f;                     /* AH = 2Fh - Pobranie adresu DTA */
int86x( 0x21, &regs, &regs, &sregs );   /* Wywoàanie funkcji DOS */

( unsigned long )DTA_Ptr = sregs.es;    /* Segment wska´nika */
( unsigned long )DTA_Ptr <<= 16;     /* Przesuni©cie na wàaòciwe miejsce */
( unsigned long )DTA_Ptr |= regs.x.bx;  /* Dodanie przesuni©cia (offset) */

/* Utworzenie wzorca do poszukiwania */
strcpy( scratch, pathToSearch );        /* Skopiowanie òcieæki */

if ( scratch[strlen(scratch) - 1] != '\\' )
   strcat( scratch,"\\" );              /* Dodanie w razie potrzeby
                                           ko‰cz•cego '\' */

strcpy( outputBuffer, scratch );        /* Zachowanie òcieæki na p¢´niej */

strcat( scratch,"?" );                  /* Pierwszy znak to '?' */
strcat( scratch, boardName );           /* Dodanie ID karty */
strcat( scratch, ".cfg" );              /* Dodanie rozszerzenia (CFG) */

regs.h.ah = 0x4e;                       /* AH = 4Eh - Znajd´ nazw© pliku */
scratchP = scratch;
sregs.ds = FP_SEG( scratchP );          /* DS = segment tekstu wzorca */
regs.x.dx = FP_OFF( scratchP );      /* DX = przesuni©cie tekstu wzorca */
regs.x.cx = 0;                       /* CX = 0 - szukamy zwykàych plik¢w */
int86x( 0x21, &regs, &regs, &sregs );   /* Wywoàanie funkcji DOS */

/* Jeæeli nie znaleziono pliku podaj przyczyn© */
if ( regs.x.cflag )
{
   strcpy( outputBuffer, _strerror(NULL) );
   return( regs.x.ax );
   }

/* Znaleziono plik */
strcat( outputBuffer, DTA_Ptr->fileName );  /* Dodaj nazw© do òcieæki */
return( 0 );                                /* Zwr¢Ü 0 = powodzenie */
}

/* Przegl•danie pliku konfiguracyjnego i skopiowanie
   jego p¢l NAME= i MFR= do bufor¢w
*/
void searchConfigFile( FILE *stream, 
                       char *name, 
                       char *vendor )
{
#define BUFFERSIZE 128

char buffer[BUFFERSIZE];
char *pValue;

*name = 0;
*vendor = 0;

while( fgets(buffer, BUFFERSIZE, stream) )
{
   while( isspace(buffer[0]) )           /* Pomi‰ wiod•ce biaàe znaki */
      strcpy( &buffer[0], &buffer[1] );

   pValue = strchr( buffer, '=' );       /* Znajd´ '=' w tekòcie */

   if ( pValue )                      /* Jeæeli znalazàeò to kontynuuj */
   {
      do                                 /* Znajd´ wartoòÜ */
      {
         ++pValue; 
         }  while ( isspace(*pValue) );

      if ( *pValue == '"' )              /* Jeæeli jest w cudzysàowach */
      {
         ++pValue;                       /* Przejd´ do nast©pnego znaku */
         *strchr( pValue, '"' ) = 0;     /* i pomi‰ zamykaj•cy cudzysà¢w */
         }

      if ( memicmp( buffer, "NAME", 4) == 0 )
         strcpy( name, pValue );         /* Kopiuj pole NAME= */

      if ( memicmp( buffer, "MFR", 3) == 0 )
         strcpy( vendor, pValue );       /* Kopiuj pole MFR= */

      if ( (*name != 0) && (*vendor != 0) )
         break;                          /* Zako‰cz przegl•danie jeæeli
                                            znalazàeò obydwa pola */
      }
   }
}

/* Znajdowanie pliku konfiguracyjnego na podstawie ID,
   otwarcie go i znalezienie nazwy karty i wytw¢rcy
*/
void openAndSearchConfigFile( char *boardID, 
                              char *boardName, 
                              char *vendor )
{
char scratch[128];
FILE *configFile;
boolean okSoFar;
static char notAvailable[] = "Dane niedost©pne";

okSoFar = ( findConfigFile( boardID, 
                            configFileDirectory, 
                            scratch ) == 0 );

if ( okSoFar )
{
   configFile = fopen( scratch, "rt" );
   okSoFar = ( configFile != NULL );
   }

if ( !okSoFar )
{
   strcpy( boardName, notAvailable );
   strcpy( vendor, notAvailable );
   }
else
{
   searchConfigFile( configFile, boardName, vendor );
   fclose( configFile );
   }
}

