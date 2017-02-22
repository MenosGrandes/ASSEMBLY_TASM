/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-3.  Odszukanie pliku konfiguracyjnego
*/


/*
   Znalezienie pliku konfiguracyjnego karty EISA na podstawie ID.
   Je¾eli operacja powiedzie si©, funkcja zwraca 0, a bufor zawiera
   nazw© pliku z peˆn¥ ˜cie¾k¥ dost©pu. W przypadku bˆ©du funkcja
   zwraca kod bˆ©du DOSu.  
*/

unsigned int findConfigFile( char *boardName,
                             char *pathToSearch,
                             char *outputBuffer )
{
union REGS regs;
struct SREGS sregs;

char scratch[128];
char *scratchP;

/* struct DTA definiuje DOSowsk¥ struktur© Disk Transfer Area
   u¾ywan¥ podczas wyszukiwania plik¢w. DTA jest obszarem pami©ci
   u¾ywanym do przekazywania danych pomi©dzy DOSem i programami.
   Funkcja 2Fh przerwania 21h DOSU zwraca adres do tego obszaru.
*/
struct DTA
{  char reserved[21];
   unsigned char attributes;
   unsigned int  creationTime;
   unsigned int  creationDate;
   unsigned long fileSizeInBytes;
   char fileName[13];
   } far *DTA_Ptr;

/* Pobranie bie¾¥cego adresu DTA */
regs.h.ah = 0x2f;                      /* AH = 2Fh - pobranie adresu DTA */
int86x( 0x21, &regs, &regs, &sregs );    /* Wywoˆanie funkcji DOS */
( unsigned long )DTA_Ptr = sregs.es;     /* Pobranie segmentu wska«nika */
( unsigned long )DTA_Ptr <<= 16;  /* Przesuni©cie na odpowiedni¥ pozycj© */
( unsigned long )DTA_Ptr |= regs.x.bx;  /* Dodanie przesuni©cia (offset) */

/* Utworzenie tekstu wzorca nazwy */
strcpy( scratch, pathToSearch );         /* Skopiowanie ˜cie¾ki dost©pu */

if ( scratch[strlen(scratch) - 1] != '\\' )
   strcat( scratch,"\\" );                   /* Dodanie w razie potrzeby
                                                    znaku \ na koäcu */

strcpy( outputBuffer, scratch );             /* Zachowanie tego fragmentu
                                                          na p¢«niej */

strcat( scratch,"?" );                       /* Pierwszy znak to ? */
strcat( scratch, boardName );                /* Dodanie ID karty */
strcat( scratch, ".cfg" );                 /* Dodanie rozszerzenia .CFG */

regs.h.ah = 0x4e;                      /* AH = 4Eh - Znajd« nazw© pliku */
scratchP = scratch;
sregs.ds = FP_SEG( scratchP ); /* DS = segment wska«nika do wzorca nazwy */
regs.x.dx = FP_OFF( scratchP );/* DX = przesuni©cie tego wska«nika */
regs.x.cx = 0;                 /* CX = 0 - poszukiwanie zwykˆych plik¢w */
int86x( 0x21, &regs, &regs, &sregs );        /* Wywoˆanie funkcji DOS */

if ( regs.x.cflag )                          /* Nie znaleziono pliku */
{
   strcpy( outputBuffer, _strerror(NULL) );  /* Powiedz dlaczego */
   return( regs.x.ax );                      /* Zwr¢† kod bˆ©du */
   }
                                             /* Znaleziono plik */
strcat( outputBuffer, DTA_Ptr->fileName );   /* Dodanie nazwy pliku
                                                       do ˜cie¾ki */
return( 0 );                                 /* Zwracamy 0 = powodzenie */
}
