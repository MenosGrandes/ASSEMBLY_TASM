/* *************************************************************
   BADSPOT.C
   Copyright (c) 1992 - Billy P. Taylor
   *************************************************************
   Program demonstruj•cy uæycie funkcji przerwania 13h z dyskami
   twardymi i dyskietkami. Znajduje on nieczytelne sektory
   czytaj•c dysk òcieæka po òcieæce. Po znalezieniu uszkodzonej
   òcieæki, czyta j• ponownie sektor po sektorze, aby znale´Ü te,
   kt¢re s• uszkodzone. Program pokazuje r¢wnieæ r¢ænice w
   wyznaczaniu pojemnoòci pomi©dzy dyskietkami i dyskami twardymi.

   Aby uruchomiÜ program, napisz: BADSPOT <litera lub numer nap©du>.
   Litery oznaczaj•ce stacje dyskietek - A...D.
   Numery twardych dysk¢w - 80...83.
   ********************************************************** */

/* ******** STAùE i MAKRODEFINICJE ********* */
#include <ctype.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define boolean unsigned char
#define YES 1
#define NO  0

#define DRIVE_DOOR_OPEN 6   /* Bà•d BIOS - zmiana noònika */
#define MAX_BPS       512   /* Maksymalna liczba bajt¢w na sektor */
#define MAX_DSKT_NUMBER 3   /* Najwi©kszy numer stacji dyskietek */

#define CARRIAGE_RETURN 13

/* Zwracane wartoòci (ERRORLEVEL) */
#define ALL_SECTORS_READABLE 0
#define DISK_HAS_BAD_SECTORS 1
#define BAD_COMMAND_LINE     2
#define BAD_DRIVE_SPECIFIED  3
#define INSUFFICIENT_MEMORY  8

/* ********  Zmienne globalne  ******** */
unsigned int readSize,
             maxHead,
             maxSector,
             maxTrack,
             head,
             track,
             sector,
             returnCode = ALL_SECTORS_READABLE;

unsigned char oneSector[MAX_BPS];
unsigned char far *diskBuffer = oneSector;

struct errorMessage
{
   unsigned char errorCode;
   unsigned char *message;
   } errorMessages[] =
{
   { 0x01, "Niepoprawne polecenie" },
   { 0x02, "Nie znaleziono znacznika adresowego" },
   { 0x04, "Nie znaleziono sektora" },
   { 0x05, "Nieudany reset nap©du" },
   { 0x07, "Nieudane pobranie parametr¢w nap©du" },
   { 0x09, "Pr¢ba uæycia bufora z przekroczeniem granicy 64KB" },
   { 0x0a, "Odczytano znacznik uszkodzonego sektora" },
   { 0x10, "Wyst•pià nienaprawialny bà•d ECC" },
   { 0x11, "Wyst•pià naprawialny bà•d ECC" },
   { 0x20, "Bà•d sterownika" },
   { 0x40, "Nieudane ustawianie gàowic" },
   { 0x80, "Nap©d nie reaguje" },
   { 0xaa, "Nap©d nie jest gotowy" },
   { 0xbb, "Nieznany bà•d" },
   { 0xcc, "Nieudany zapis" },
   { 0xff, "Nieudane polecenie wykrycia nap©du" },
   {    0, "Nierozpoznany kod bà©du!" }
   };

/* ********  Prototypy funkcji  ******** */
unsigned char findMediaSize         ( int,
                                      unsigned int *,
                                      unsigned int *,
                                      unsigned int *);
boolean       readSourceDiskette    ( void );
unsigned int  readSpot              ( int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int);
void          resetDrive            ( int );
void          showDiskErrorMessage  ( unsigned int );

/* ********  MAIN  ******** */
void main( int argc, char *argv[] )
{
int drive;
boolean driveOk;
unsigned int errorCode;

printf( "BADSPOT : Znajdowanie uszkodzonych sektor¢w dyskietki ");
printf( "lub dysku twardego\n" );

/* Pobranie litery lub numeru nap©du z linii polecenia.
   Uwaga: dyskietki oznaczane s• przez litery,
   a dyski twarde przez liczby.
*/
driveOk = NO;
if ( argc >= 2 )
{
   if ( isalpha( *argv[1] ) )
   {
/* Zamiana litery stacji dyskietek na liczb© i sprawdzenie poprawnoòci */
      drive = toupper( *argv[1] ) - 'A';
      if ( drive <= MAX_DSKT_NUMBER )
         driveOk = YES;
      }
   else
   {
/* Zamiana tekstu liczby na liczb© caàkowit• */
      drive = atoi( argv[1] );
/* Dozwolone s• wartoòci z zakresu 80...83 */
      if ( (drive >= 80) && (drive <= 83) )
      {
         drive -= 80;       /* Przejòcie do numeracji od 0 */
         drive += 0x80;     /* Ustawienie 7 bitu - dysk twardy */
         driveOk = YES;
         }
      }
   }

if ( !driveOk )
{
   printf( "\Skàadnia: %s <litera lub numer nap©du>", argv[0] );
   printf( "\n        Stacje dyskietek - A...D." );
   printf( "\n        Dyski twarde - 80...83.\7" );
   exit( BAD_COMMAND_LINE );
   }

/* Odczytanie parametr¢w nap©du */
errorCode = findMediaSize( drive, &maxHead, &maxTrack, &maxSector );
if ( errorCode )
{
   showDiskErrorMessage( errorCode );
   resetDrive( drive );
   exit( BAD_DRIVE_SPECIFIED );
   }

printf( "\n\Nap©d %Xh. %u gàowic(e), %u òcieæki, %u sektor¢w na òcieæce\n",
        drive,
        maxHead+1,
        maxTrack+1,
        maxSector);

/* Alokacja bufora òcieæki */
readSize = maxSector * MAX_BPS;
diskBuffer = malloc( readSize );
if ( diskBuffer == NULL )
{
   perror( "Bù§D ALOKACJI PAMI®CI DLA BUFORA DYSKOWEGO : \7" );
   exit( INSUFFICIENT_MEMORY );
   }

/* Czytanie dysku òcieæka po òcieæce */
for( track = 0; track <= maxTrack; track++ )
{
   for( head = 0; head <= maxHead; head++ )
   {
      printf( "%c\Czytam %u sektor¢w, gàowica %u, òcieæka %u, sektor 1   ",
              CARRIAGE_RETURN,
              maxSector,
              head,
              track);

      errorCode = readSpot( drive, head, track, 1, maxSector );
      if ( errorCode )
      {
         showDiskErrorMessage( errorCode );
         resetDrive( drive );
         printf( "\n" );

/* Jeæeli nast•pià bà•d, czytaj sektor po sekorze */
         for( sector = 1; sector <= maxSector; sector++ )
         {
            printf( "%cCzytam 1 sektor, gàowica %u, òcieæka %u, sektor %u",
                    CARRIAGE_RETURN,
                    head,
                    track,
                    sector);
            errorCode = readSpot( drive, head, track, sector, 1 );

/* Wyòwietl nieczytelne sektory */
            if ( errorCode )
            {
               showDiskErrorMessage( errorCode );
               returnCode = DISK_HAS_BAD_SECTORS;
               printf( "\n" );
               resetDrive( drive );
               }
            }
         printf( "\n" );
         }
      }
   }
exit( returnCode );
}

/* ********  Funkcje fizycznego dost©pu do dysku  ******** */
/* Reset Nap©du i sterownika */
void resetDrive( int driveNumber )
{
union REGS regs;
regs.h.ah = 0;                /* AH = 0 - reset nap©du i sterownika */
regs.h.dl = driveNumber;      /* DL = numer nap©du */
int86( 0x13, &regs, &regs );  /* Wywoàanie funkcji BIOS */
}

/* Odczytanie sektor¢w.
   Zwraca 0 przy powodzeniu lub kod bà©du przerwania 13h
*/
unsigned int readSpot( int drive,
                       unsigned int head,
                       unsigned int cylinder,
                       unsigned int sector,
                       unsigned int sectorsToRead )
{
union REGS regs;
struct SREGS sregs;

unsigned char far *bufferPtr;
unsigned int sectorMask;
boolean secondTry;

bufferPtr = diskBuffer;
secondTry = NO;

while(1)
{
   sectorMask  = ( cylinder >> 2); /* We´ starsze 2 bity numeru cylindra */
   sectorMask &= ( 128 + 64 );     /* Wyzeruj pozostaàe */
   sectorMask += sector;           /* Numer sektora do màodszych 6 bit¢w */

   regs.h.ah   = 2;                /* AH = 2 - czytaj sektory */
   regs.h.al   = sectorsToRead;    /* AL = liczba sektor¢w */
   regs.h.ch   = cylinder;         /* CH = màodsze 8 bit¢w nru cylindra */
   regs.h.cl   = sectorMask;       /* CL = sektor+2 bity nru cylindra */
   regs.h.dh   = head;                 /* DH = gàowica */
   regs.h.dl   = drive;                /* DL = nap©d */
   regs.x.bx   = FP_OFF( bufferPtr );  /* BX = przesuni©cie bufora */
   sregs.es    = FP_SEG( bufferPtr );  /* ES = segment bufora */

   int86x( 0x13, &regs, 
            &regs, &sregs );           /* Wywoàanie funkcji BIOS */

   if ( regs.x.cflag == 0 )            /* Carry = 0 - OK */
      break;

   if ( (regs.h.ah != DRIVE_DOOR_OPEN) || secondTry )
      return( regs.h.ah );    /* Powr¢t, jeæeli bà•d jest nienaprawialny */

   secondTry = YES;
   }

return( 0 );
}

/* Wyòwietlenie komunikatu o bà©dzie przerwania 13h */
void showDiskErrorMessage( unsigned int errorCode )
{
int i;

printf( "\7\nBù§D %u : ",errorCode );

i = 0;
while( (errorMessages[i].errorCode != errorCode) &&
       (errorMessages[i].errorCode != 0) )
   i++;

printf( "%s", errorMessages[i].message );
}

/* Okreòlenie parametr¢w nap©du.
   Zwraca 0 przy powodzeniu lub kod bà©du przerwania 13h
*/
unsigned char findMediaSize( int drive,
                             unsigned int *maxHead,
                             unsigned int *maxTrack,
                             unsigned int *maxSector )
{
#define CMOS_DRIVE_AB      16    /*  Pozycja w CMOS RAM informacji
                                     o typie stacji A i B */
#define FIRST_HARD_DRIVE 0x80
#define HEAD_0              0
#define ONE_SECTOR          1
#define SECTOR_1            1
#define SECTOR_9            9
#define SECTOR_18          18
#define TRACK_0             0
#define TYPE_720KB          3    /* Maska dla stacji 720KB (CMOS RAM) */
#define TYPE_144MB          4    /* Maska dla stacji 1.44MB (CMOS RAM) */

#define driveA (unsigned char far *)0x00400090
#define driveB (unsigned char far *)0x00400091

#define CMOS_CMD  0x70
#define CMOS_DATA 0x71

union REGS regs;

unsigned char mediaStatus, 
              driveType;

unsigned int errorCode;

if ( drive >= FIRST_HARD_DRIVE )  /* Dla dysk¢w twardych */
{
/* Sprawdzenie czy nap©d istnieje */
   regs.h.ah = 0x10;         /* AH = 10h - sprawdzenie gotowoòci nap©du */
   regs.h.dl = drive;             /* DL = numer nap©du */
   int86( 0x13, &regs, &regs );   /* Wywoàanie funkcji BIOS */
   if ( regs.x.cflag )            /* Carry = 1 - bà•d */
      return( regs.h.ah );        /* AH = kod bà©du */

/* Odczytanie z BIOSu parametr¢w dysku twardego */
   regs.h.ah = 8;                 /* AH = 8 - podaj parametry dysku */
   regs.h.dl = drive;             /* DL = numer nap©du */
   int86( 0x13, &regs, &regs );   /* Wywoàanie funkcji BIOS */
   if ( regs.x.cflag )            /* Carry = 1 - bà•d */
      return( regs.h.ah );        /* AH = kod bà©du */

   *maxHead  = regs.h.dh;         /* DH = liczba gàowic */
                               /* CL = maks. nr cylindra, starsze 2 bity */
                                  /*      màodsze 6 bit¢w - sekt/òcieæk© */

   *maxSector  = regs.h.cl & 63;  /* Sektory w màodszych 6 bitach */
   *maxTrack = regs.h.cl;         /* Starsze 2 bity nru cylindra */
   *maxTrack &= ( 128 + 64 );     /* Wyzerowanie pozostaàych bit¢w */
   *maxTrack <<= 2;               /* Przesuni©cie na wàaòciw• pozycj© */
   *maxTrack |= regs.h.ch;        /* Dodanie màodszych 8 bit¢w */
   }
else                              /* Dla dyskietki */
{
/* Pozorne czytanie potrzebne do zidentyfikowania dyskietki przez BIOS */
   readSpot( drive, 0, 0, 1, 1 );
   resetDrive( drive );

/* Powt¢rka czytania - pierwsze czytanie mogào si© nie powieòÜ
   z powodu zmiany noònika */
   errorCode = readSpot( drive, 0, 0, 1, 1 );

/* Jeæeli nie da si© odczytaÜ, to zwr¢Ü bà•d */
   if ( errorCode )
      return( errorCode );

/* Odczytaj stan noònika z BDA */
   if ( drive == 0 )
      mediaStatus = *driveA;
   else
      mediaStatus = *driveB;

   mediaStatus &= 7;              /* Usuni©cie niepotrzebnych bit¢w */

/* Zwr¢cenie informacji o dyskietce 5,25" */
   switch ( mediaStatus )
   {
      case 0:      /* Dyskietka 360KB w stacji 360KB - nie zweryfikowana */
      case 1:      /* Dyskietka 360KB w stacji 1.2MB - nie zweryfikowana */
      case 2:      /* Dyskietka 1.2MB w stacji 1.2MB - nie zweryfikowana */
         return( 4 );       /* Nieznany rozmiar !                        */

      case 3:      /* Dyskietka 360KB w stacji 360KB - zweryfikowana     */
      case 4:      /* Dyskietka 360KB w stacji 1.2MB - zweryfikowana     */
         *maxHead  = 1;
         *maxSector  = 9;
         *maxTrack = 39;
         return( 0 );

      case 5:      /* Dyskietka 1.2MB w stacji 1.2MB - zweryfikowana     */
         *maxHead  = 1;
         *maxSector  = 15;
         *maxTrack = 79;
         return( 0 );
      }

/* Wszystko inne musi byÜ dyskietk• 3.5".  Czytamy parametry z CMOS RAM */
   outp( CMOS_CMD, CMOS_DRIVE_AB );
   driveType = inp( CMOS_DATA );

   if ( drive == 0 )              /* Jeæeli to stacja A */
      driveType >>= 4;            /*    badamy starsze 4 bity */
   else                           /* w przeciwnym wypadku */
      driveType &= 15;            /*    badamy màodsze 4 bity */

   if ( (driveType != TYPE_720KB) &&
        (driveType != TYPE_144MB) )
      return( 4 );                /* Nie moæna ustaliÜ typu noònika ! */

/* Zidentyfikowano stacj© 3.5" 
   sprawdzamy czy dyskietka ma 720KB czy 1.44MB
   czytaj•c sektory 9 i 18 òcieæki 0, gàowicy 0.
*/
   if ( readSpot(drive, HEAD_0, TRACK_0, SECTOR_9, ONE_SECTOR) != 0 )
      return( 4 );                /* Nie moæna ustaliÜ rodzaju noònika ! */

   if ( readSpot(drive, HEAD_0, TRACK_0, SECTOR_18, ONE_SECTOR) == 0 )
   {
/* Przeczytano 18-ty sektor - dyskietka 1.44MB */
      *maxHead  = 1;
      *maxSector  = 18;
      *maxTrack = 79;
      }
   else
   {
/* Przeczytano jedynie 9-ty sektor - dyskietka  720KB */
      *maxHead  = 1;
      *maxSector  = 9;
      *maxTrack = 79;
      }
   }
return( 0 );
}
