/* ***************************************************
   FLOPDUPE.C
   Copyright (c) 1992 - Billy P. Taylor
   ***************************************************
   Powielanie dyskietek przy uæyciu funkcji przerwania 13h.
   Uruchomienie: FLOPDUPE<Enter>.
   ************************************************* */

/* ******** STAùE i MAKRODEFINICJE ********* */
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\types.h>
#include <sys\stat.h>

#define boolean unsigned char
#define YES 1
#define NO  0

#define CARRIAGE_RETURN 13

#define MAX_BPS      512
#define MAX_SPT       32
#define MAX_DSKT_DRIVE 3

#define DRIVE_DOOR_OPEN 6

struct errorMessage
{
   unsigned char errorCode;
   unsigned char *message;
   };

/* ******  Zmienne globalne  ****** */
union REGS regs;
struct SREGS sregs;

unsigned int sourceDrive,
             readSize,
             maxHead,
             maxSector,
             maxTrack,
             head,
             track;

unsigned char diskBuffer[MAX_BPS * MAX_SPT];
unsigned char far *diskBufferP = diskBuffer;

int scratchFile;
char scratchFileName[] = "C:FlopDupe.Tmp";

struct errorMessage errorMessages[] =
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

/* ******  Prototypy funkcji  ****** */
boolean       createScratchFile     ( void );
unsigned char findMediaSize         ( int,
                                      unsigned int *,
                                      unsigned int *,
                                      unsigned int * );
void          makeCopies            ( void );
void          makeOneCopy           ( void );
boolean       readSourceDiskette    ( void );
unsigned int  readSpot              ( int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int );
void          resetDrive            ( int );
void          showDiskErrorMessage  ( unsigned int );
unsigned int  writeSpot             ( int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int );

void main( int argc, char *argv[] )
{
boolean driveOk;

printf("\t**** Powielacz dyskietek ****\n");
printf("Skàadnia: FLOPDUPE <A...D>  -  Domyòln• stacj• jest A\n");

driveOk = NO;

/* Jeæeli nie podano stacji - wybierz A */
if ( argc == 1 )
{
   sourceDrive = 0;
   driveOk = YES;
   }
else
{
   if ( isalpha( *argv[1] ) )
   {
      sourceDrive = toupper( *argv[1] ) - 'A';
      if ( sourceDrive <= MAX_DSKT_DRIVE )
         driveOk = YES;
      }
   }

if ( !driveOk )
{
   printf( "Nieprawidàowa litera stacji dyskietek!\7" );
   exit( 1 );
   }

if ( createScratchFile() && readSourceDiskette() )
   makeCopies();

close( scratchFile );
remove( scratchFileName );
exit( 0 );
}

/* ******  Funkcje uæywane przez main()  ****** */
/* Utworzenie pliku tymczasowego do przechowywania
   zawartoòci dyskietki.
   Funkcja zwraca 1=OK lub 0=bà•d
*/
boolean createScratchFile( void )
{
scratchFile = open( scratchFileName,
                    O_CREAT|O_RDWR|O_BINARY,
                    S_IREAD|S_IWRITE );

if ( scratchFile == -1 )
   perror( "Bà•d przy tworzeniu pliku tymczasowego \7" );

return( (scratchFile != -1) );
}

/* Skopiowanie zawartoòci dyskietki do pliku tymczasowego 
   Funkcja zwraca 1=OK lub 0=bà•d
*/
boolean readSourceDiskette( void )
{
unsigned int errorCode;

printf( "\nWà¢æ kopiowan• dyskietk© do stacji %c i naciònij Enter", 
         sourceDrive+'A' );
getch();

errorCode = findMediaSize( sourceDrive,
                           &maxHead,
                           &maxTrack,
                           &maxSector );

if ( errorCode )
{
   showDiskErrorMessage( errorCode );
   resetDrive( sourceDrive );
   return( 0 );
   }

printf( "\nDyskietka ma %u gàowice, %u òcieæek i %u sektor¢w na òcieæce\n",
         maxHead+1,
         maxTrack+1,
         maxSector );

readSize = maxSector * MAX_BPS;

if ( readSize > (MAX_SPT*MAX_BPS) )
{
   printf( "BUFOR DYSKOWY JEST ZA MAùY!\7" );
   return( 0 );
   }

for( track = 0; track <= maxTrack; track++ )
{
   for( head = 0; head <= maxHead; head++ )
   {
		printf("%cCzytam %u sektor¢w, gàowica %u, òcieæka %u, od sektora 1  ",
               CARRIAGE_RETURN,
               maxSector,
               head,
               track );

      errorCode = readSpot( sourceDrive, head, track, 1, maxSector );
      if ( errorCode )
      {
         showDiskErrorMessage( errorCode );
         resetDrive( sourceDrive );
         return( 0 );
         }

      if ( write(scratchFile,diskBuffer,readSize) != readSize )
      {
         perror( "Bù§D ZAPISU NA DYSK !\7" );
         exit( errno );
         }
      }
   }
return( 1 );
}

/* Wykonywanie kopii dyskietki */
void makeCopies( void )
{
for(;;)
{
   printf( "\nWà¢æ sformatowan• dyskietk© do stacji %c i naciònij Enter", 
            sourceDrive+'A' );
   getch();

   printf( "\n" );

   makeOneCopy();

   printf( "\nCzy chcesz zrobiÜ kolejn• kopi© (T/N) ? " );

   if ( toupper(getch()) != 'T' )
      break;

   printf( "\n" );
   }
}

/* Wykonanie kopii dyskietki */
void makeOneCopy( void )
{
unsigned int errorCode;

lseek( scratchFile, 0L, SEEK_SET );

for( track = 0; track <= maxTrack; track++ )
{
   for( head = 0; head <= maxHead; head++ )
   {
      if ( read(scratchFile,
                diskBuffer,
                readSize) != readSize )
      {
         perror( "Bù§D ODCZYTU DYSKU TWARDEGO !\7" );
         return;
         }

		printf("%cZapisuj© %u sektor¢w, gàowica %u, òcieæka %u, od sektora 1 ",
               CARRIAGE_RETURN,
               maxSector,
               head,
               track );

      errorCode = writeSpot( sourceDrive,
                             head,
                             track,
                             1,
                             maxSector );
      if ( errorCode )
      {
         showDiskErrorMessage( errorCode );
         resetDrive( sourceDrive );
         return;
         }
      }
   }
}
/* ********  Funkcje fizycznego dost©pu do dysku  ******** */
/* Reset nap©du i sterownika */
void resetDrive( int drive )
{
regs.h.ah = 0;                /* AH = 0 - reset nap©du i sterownika */
regs.h.dl = drive;            /* DL = numer nap©du */
int86( 0x13, &regs, &regs );  /* Wywoàanie funkcji BIOS */
}

/* Czytanie sektor¢w.  Funkcja zwraca 0 = OK lub kod bà©du */
unsigned int readSpot( int drive,
                       unsigned int head,
                       unsigned int cylinder,
                       unsigned int sector,
                       unsigned int size )
{                      
unsigned int sectorMask;
boolean secondTry;

secondTry = NO;
while (1)
{
/* sectorMask = starsze 2 bity numeru cylindra + numer sektora */
   sectorMask  = ( cylinder >> 2) & ( 128 + 64 );
   sectorMask += sector;
   regs.h.ah   = 2;                      /* AH = 2 - czytaj sektory */
   regs.h.al   = size;                   /* AL = ile sektor¢w */
   regs.h.ch   = cylinder;               /* CH = màodsze 8 bit¢w nr cyl. */
   regs.h.cl   = sectorMask;             /* CL = starsze 2 bity nru cyl. */
                                         /*      + numer sektora */
   regs.h.dh   = head;                   /* DH = gàowica */
   regs.h.dl   = drive;                  /* DL = nap©d */
   regs.x.bx   = FP_OFF( diskBufferP );  /* BX = przesuni©cie bufora */
   sregs.es    = FP_SEG( diskBufferP );  /* ES = segment bufora */
   int86x( 0x13, &regs, 
            &regs, &sregs );             /* Wywoàanie funkcji BIOS */
   if ( regs.x.cflag == 0 )              /* Carry == 1 - bà•d */
      break;

   if ( (regs.h.ah != DRIVE_DOOR_OPEN) || secondTry )
      return( regs.h.ah );

   secondTry = YES;
   }

return( 0 );
}

/* Zapisanie sektor¢w.  Funkcja zwraca 0 = OK lub kod bà©du BIOS */
unsigned int writeSpot( int drive,
                        unsigned int head,
                        unsigned int cylinder,
                        unsigned int sector,
                        unsigned int size )
{
unsigned int sectorMask;
boolean secondTry;

secondTry = NO;
while (1)
{/* sectorMask = starsze 2 bity numeru cylindra + numer sektora */
   sectorMask = ( cylinder >> 2 ) & ( 128+64 );
   sectorMask += sector;
   regs.h.ah = 3;                     /* AH = 3 - zapisz sektory */
   regs.h.al = size;                  /* AL = liczba sektor¢w */
   regs.h.ch = cylinder;              /* CH = màodsze 8 bit¢w cyl. */
   regs.h.cl = sectorMask;            /* CL = starsze 2 bity cyl. */
                                      /*      + numer sektora */
   regs.h.dh = head;                  /* DH = gàowica */
   regs.h.dl = drive;                 /* DL = numer nap©du */
   regs.x.bx = FP_OFF( diskBufferP ); /* BX = przesuni©cie bufora */
   sregs.es = FP_SEG( diskBufferP );  /* ES = segment bufora */
   int86x( 0x13, &regs, 
            &regs, &sregs );          /* Wywoàanie funkcji BIOS */
   if ( regs.x.cflag == 0 )           /* Carry == 1 - bà•d */
      break;

   if ( (regs.h.ah != DRIVE_DOOR_OPEN) || secondTry )
      return( regs.h.ah );

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

/* Okreòlenie rodzaju dyskietki.  
   Funkcja zwraca 0 lub kod bà©du BIOS
*/
unsigned char findMediaSize ( int drive,
                              unsigned int *maxHead,
                              unsigned int *maxTrack,
                              unsigned int *maxSector )
{
#define driveA (unsigned char far *)0x00400090
#define driveB (unsigned char far *)0x00400091

#define CMOS_DRIVE_AB  16  /* Adres w CMOS RAM informacji o stacjach A/B */
#define HEAD_0          0
#define ONE_SECTOR      1
#define SECTOR_1        1
#define SECTOR_9        9
#define SECTOR_18      18
#define TRACK_0         0
#define TYPE_144MB      4    /* Stacja 1.44MB (CMOS RAM) */
#define TYPE_720KB      3    /* Stacja 720KB (CMOS RAM) */

unsigned char mediaStatus, 
              driveType;
unsigned int errorCode;

/* Pozorne czytanie - wymuszenie identyfikacji noònika */
readSpot( drive,0,0,1,1 );
resetDrive( drive );

/* Powt¢rka czytania - pierwsze mogào si© nie udaÜ
   ze wzgl©du na sygnaà zmiany noònika 
*/
errorCode = readSpot( drive,0,0,1,1 );

/* Jeæeli dyskietka jest nieczytelna zwr¢Ü kod bà©du */
if ( errorCode )
   return( errorCode );

/* Odczytanie stanu noònika z BDA */
if ( drive == 0 )
	mediaStatus = *driveA;
else
   mediaStatus = *driveB;

mediaStatus &= 7;      /* Usuni©cie zb©dnych bit¢w */

/* Zwr¢cenie informacji o dyskietce 5.25" */
switch ( mediaStatus )
{
   case 0:         /* dyskietka 360KB w stacji 360KB - nie zweryfikowana */
   case 1:         /* dyskietka 360KB w stacji 1.2MB - nie zweryfikowana */
   case 2:         /* dyskietka 1.2MB w stacji 1.2MB - nie zweryfikowana */
      return( 4 ); /* Nieznany rodzaj noònika !                          */

   case 3:         /* dyskietka 360KB w stacji 360KB - zweryfikowana */
   case 4:         /* dyskietka 360KB w stacji 1.2MB - zweryfikowana */
      *maxHead  = 1;
      *maxSector  = 9;
      *maxTrack = 39;
      return( 0 );

   case 5:        /* dyskietka 1.2MB w stacji 1.2MB - zweryfikowana */
      *maxHead  = 1;
      *maxSector  = 15;
      *maxTrack = 79;
      return( 0 );
   }

/* Wszystkie inne dyskietki musz• byÜ 3.5" */
outp( 0x70, CMOS_DRIVE_AB );      /* Ustawienie adresu w CMOS RAM */
driveType = inp(0x71);            /* Odczytanie bajtu */

if ( drive == 0 )                 /* Jeæeli to stacja A */
   driveType >>= 4;               /* czytaj starsze 4 bity */
else                              /* w przeciwnym wypadku */
   driveType &= 15;               /* czytaj màodsze 4 bity */

if ( (driveType != TYPE_720KB) &&
     (driveType != TYPE_144MB) )
   return( 4 );                   /* Nieznany rodzaj noònika ! */

/* 
   To jest dyskietka 3.5" - teraz okreòlamy 720KB czy 1.44MB
   czytaj• sektory 9 i 18 òcieæki 0 i gàowicy 0
*/
if ( readSpot( drive, HEAD_0, TRACK_0, SECTOR_9, ONE_SECTOR ) != 0 )
   return( 4 );                   /* Nieznany rodzaj noònika ! */

if ( readSpot( drive,
               HEAD_0,
               TRACK_0,
               SECTOR_18,
               ONE_SECTOR) == 0 )
{
   *maxHead  = 1;                 /* Przeczytany sektor 18 - 1.44MB */
   *maxSector  = 18;
   *maxTrack = 79;
   }
else
{
   *maxHead  = 1;                 /* Przeczytany tylko sektor 9 - 720KB */
   *maxSector  = 9;
   *maxTrack = 79;
   }
return( 0 );
}

