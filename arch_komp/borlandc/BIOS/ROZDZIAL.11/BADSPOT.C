/* *************************************************************
   BADSPOT.C
   Copyright (c) 1992 - Billy P. Taylor
   *************************************************************
   Program demonstruj�cy u�ycie funkcji przerwania 13h z dyskami
   twardymi i dyskietkami. Znajduje on nieczytelne sektory
   czytaj�c dysk �cie�ka po �cie�ce. Po znalezieniu uszkodzonej
   �cie�ki, czyta j� ponownie sektor po sektorze, aby znale�� te,
   kt�re s� uszkodzone. Program pokazuje r�wnie� r��nice w
   wyznaczaniu pojemno�ci pomi�dzy dyskietkami i dyskami twardymi.

   Aby uruchomi� program, napisz: BADSPOT <litera lub numer nap�du>.
   Litery oznaczaj�ce stacje dyskietek - A...D.
   Numery twardych dysk�w - 80...83.
   ********************************************************** */

/* ******** STA�E i MAKRODEFINICJE ********* */
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

#define DRIVE_DOOR_OPEN 6   /* B��d BIOS - zmiana no�nika */
#define MAX_BPS       512   /* Maksymalna liczba bajt�w na sektor */
#define MAX_DSKT_NUMBER 3   /* Najwi�kszy numer stacji dyskietek */

#define CARRIAGE_RETURN 13

/* Zwracane warto�ci (ERRORLEVEL) */
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
   { 0x05, "Nieudany reset nap�du" },
   { 0x07, "Nieudane pobranie parametr�w nap�du" },
   { 0x09, "Pr�ba u�ycia bufora z przekroczeniem granicy 64KB" },
   { 0x0a, "Odczytano znacznik uszkodzonego sektora" },
   { 0x10, "Wyst�pi� nienaprawialny b��d ECC" },
   { 0x11, "Wyst�pi� naprawialny b��d ECC" },
   { 0x20, "B��d sterownika" },
   { 0x40, "Nieudane ustawianie g�owic" },
   { 0x80, "Nap�d nie reaguje" },
   { 0xaa, "Nap�d nie jest gotowy" },
   { 0xbb, "Nieznany b��d" },
   { 0xcc, "Nieudany zapis" },
   { 0xff, "Nieudane polecenie wykrycia nap�du" },
   {    0, "Nierozpoznany kod b��du!" }
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

printf( "BADSPOT : Znajdowanie uszkodzonych sektor�w dyskietki ");
printf( "lub dysku twardego\n" );

/* Pobranie litery lub numeru nap�du z linii polecenia.
   Uwaga: dyskietki oznaczane s� przez litery,
   a dyski twarde przez liczby.
*/
driveOk = NO;
if ( argc >= 2 )
{
   if ( isalpha( *argv[1] ) )
   {
/* Zamiana litery stacji dyskietek na liczb� i sprawdzenie poprawno�ci */
      drive = toupper( *argv[1] ) - 'A';
      if ( drive <= MAX_DSKT_NUMBER )
         driveOk = YES;
      }
   else
   {
/* Zamiana tekstu liczby na liczb� ca�kowit� */
      drive = atoi( argv[1] );
/* Dozwolone s� warto�ci z zakresu 80...83 */
      if ( (drive >= 80) && (drive <= 83) )
      {
         drive -= 80;       /* Przej�cie do numeracji od 0 */
         drive += 0x80;     /* Ustawienie 7 bitu - dysk twardy */
         driveOk = YES;
         }
      }
   }

if ( !driveOk )
{
   printf( "\Sk�adnia: %s <litera lub numer nap�du>", argv[0] );
   printf( "\n        Stacje dyskietek - A...D." );
   printf( "\n        Dyski twarde - 80...83.\7" );
   exit( BAD_COMMAND_LINE );
   }

/* Odczytanie parametr�w nap�du */
errorCode = findMediaSize( drive, &maxHead, &maxTrack, &maxSector );
if ( errorCode )
{
   showDiskErrorMessage( errorCode );
   resetDrive( drive );
   exit( BAD_DRIVE_SPECIFIED );
   }

printf( "\n\Nap�d %Xh. %u g�owic(e), %u �cie�ki, %u sektor�w na �cie�ce\n",
        drive,
        maxHead+1,
        maxTrack+1,
        maxSector);

/* Alokacja bufora �cie�ki */
readSize = maxSector * MAX_BPS;
diskBuffer = malloc( readSize );
if ( diskBuffer == NULL )
{
   perror( "B��D ALOKACJI PAMI�CI DLA BUFORA DYSKOWEGO : \7" );
   exit( INSUFFICIENT_MEMORY );
   }

/* Czytanie dysku �cie�ka po �cie�ce */
for( track = 0; track <= maxTrack; track++ )
{
   for( head = 0; head <= maxHead; head++ )
   {
      printf( "%c\Czytam %u sektor�w, g�owica %u, �cie�ka %u, sektor 1   ",
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

/* Je�eli nast�pi� b��d, czytaj sektor po sekorze */
         for( sector = 1; sector <= maxSector; sector++ )
         {
            printf( "%cCzytam 1 sektor, g�owica %u, �cie�ka %u, sektor %u",
                    CARRIAGE_RETURN,
                    head,
                    track,
                    sector);
            errorCode = readSpot( drive, head, track, sector, 1 );

/* Wy�wietl nieczytelne sektory */
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

/* ********  Funkcje fizycznego dost�pu do dysku  ******** */
/* Reset Nap�du i sterownika */
void resetDrive( int driveNumber )
{
union REGS regs;
regs.h.ah = 0;                /* AH = 0 - reset nap�du i sterownika */
regs.h.dl = driveNumber;      /* DL = numer nap�du */
int86( 0x13, &regs, &regs );  /* Wywo�anie funkcji BIOS */
}

/* Odczytanie sektor�w.
   Zwraca 0 przy powodzeniu lub kod b��du przerwania 13h
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
   sectorMask  = ( cylinder >> 2); /* We� starsze 2 bity numeru cylindra */
   sectorMask &= ( 128 + 64 );     /* Wyzeruj pozosta�e */
   sectorMask += sector;           /* Numer sektora do m�odszych 6 bit�w */

   regs.h.ah   = 2;                /* AH = 2 - czytaj sektory */
   regs.h.al   = sectorsToRead;    /* AL = liczba sektor�w */
   regs.h.ch   = cylinder;         /* CH = m�odsze 8 bit�w nru cylindra */
   regs.h.cl   = sectorMask;       /* CL = sektor+2 bity nru cylindra */
   regs.h.dh   = head;                 /* DH = g�owica */
   regs.h.dl   = drive;                /* DL = nap�d */
   regs.x.bx   = FP_OFF( bufferPtr );  /* BX = przesuni�cie bufora */
   sregs.es    = FP_SEG( bufferPtr );  /* ES = segment bufora */

   int86x( 0x13, &regs, 
            &regs, &sregs );           /* Wywo�anie funkcji BIOS */

   if ( regs.x.cflag == 0 )            /* Carry = 0 - OK */
      break;

   if ( (regs.h.ah != DRIVE_DOOR_OPEN) || secondTry )
      return( regs.h.ah );    /* Powr�t, je�eli b��d jest nienaprawialny */

   secondTry = YES;
   }

return( 0 );
}

/* Wy�wietlenie komunikatu o b��dzie przerwania 13h */
void showDiskErrorMessage( unsigned int errorCode )
{
int i;

printf( "\7\nB��D %u : ",errorCode );

i = 0;
while( (errorMessages[i].errorCode != errorCode) &&
       (errorMessages[i].errorCode != 0) )
   i++;

printf( "%s", errorMessages[i].message );
}

/* Okre�lenie parametr�w nap�du.
   Zwraca 0 przy powodzeniu lub kod b��du przerwania 13h
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

if ( drive >= FIRST_HARD_DRIVE )  /* Dla dysk�w twardych */
{
/* Sprawdzenie czy nap�d istnieje */
   regs.h.ah = 0x10;         /* AH = 10h - sprawdzenie gotowo�ci nap�du */
   regs.h.dl = drive;             /* DL = numer nap�du */
   int86( 0x13, &regs, &regs );   /* Wywo�anie funkcji BIOS */
   if ( regs.x.cflag )            /* Carry = 1 - b��d */
      return( regs.h.ah );        /* AH = kod b��du */

/* Odczytanie z BIOSu parametr�w dysku twardego */
   regs.h.ah = 8;                 /* AH = 8 - podaj parametry dysku */
   regs.h.dl = drive;             /* DL = numer nap�du */
   int86( 0x13, &regs, &regs );   /* Wywo�anie funkcji BIOS */
   if ( regs.x.cflag )            /* Carry = 1 - b��d */
      return( regs.h.ah );        /* AH = kod b��du */

   *maxHead  = regs.h.dh;         /* DH = liczba g�owic */
                               /* CL = maks. nr cylindra, starsze 2 bity */
                                  /*      m�odsze 6 bit�w - sekt/�cie�k� */

   *maxSector  = regs.h.cl & 63;  /* Sektory w m�odszych 6 bitach */
   *maxTrack = regs.h.cl;         /* Starsze 2 bity nru cylindra */
   *maxTrack &= ( 128 + 64 );     /* Wyzerowanie pozosta�ych bit�w */
   *maxTrack <<= 2;               /* Przesuni�cie na w�a�ciw� pozycj� */
   *maxTrack |= regs.h.ch;        /* Dodanie m�odszych 8 bit�w */
   }
else                              /* Dla dyskietki */
{
/* Pozorne czytanie potrzebne do zidentyfikowania dyskietki przez BIOS */
   readSpot( drive, 0, 0, 1, 1 );
   resetDrive( drive );

/* Powt�rka czytania - pierwsze czytanie mog�o si� nie powie��
   z powodu zmiany no�nika */
   errorCode = readSpot( drive, 0, 0, 1, 1 );

/* Je�eli nie da si� odczyta�, to zwr�� b��d */
   if ( errorCode )
      return( errorCode );

/* Odczytaj stan no�nika z BDA */
   if ( drive == 0 )
      mediaStatus = *driveA;
   else
      mediaStatus = *driveB;

   mediaStatus &= 7;              /* Usuni�cie niepotrzebnych bit�w */

/* Zwr�cenie informacji o dyskietce 5,25" */
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

/* Wszystko inne musi by� dyskietk� 3.5".  Czytamy parametry z CMOS RAM */
   outp( CMOS_CMD, CMOS_DRIVE_AB );
   driveType = inp( CMOS_DATA );

   if ( drive == 0 )              /* Je�eli to stacja A */
      driveType >>= 4;            /*    badamy starsze 4 bity */
   else                           /* w przeciwnym wypadku */
      driveType &= 15;            /*    badamy m�odsze 4 bity */

   if ( (driveType != TYPE_720KB) &&
        (driveType != TYPE_144MB) )
      return( 4 );                /* Nie mo�na ustali� typu no�nika ! */

/* Zidentyfikowano stacj� 3.5" 
   sprawdzamy czy dyskietka ma 720KB czy 1.44MB
   czytaj�c sektory 9 i 18 �cie�ki 0, g�owicy 0.
*/
   if ( readSpot(drive, HEAD_0, TRACK_0, SECTOR_9, ONE_SECTOR) != 0 )
      return( 4 );                /* Nie mo�na ustali� rodzaju no�nika ! */

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
