/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje dyskowe
*/


/* Przerwanie 13h / 00h - Reset nap©du (dyskietek lub dysku twardego) */

unsigned char resetDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - reset nap©du */
regs.h.dl = drive;            /* DL = nap©d */
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 01h - Podaj stan zakoäczenia ostatniej operacji
                   wej˜cia/wyj˜cia (dyskietka lub dysk twardy) */

unsigned char readDriveStatus( unsigned char drive )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - podaj stan ostatniej operacji */
regs.h.dl = drive;            /* DL = nap©d */
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 02h - Czytaj sektory (dyskietka lub dysk twardy) */

unsigned char readSectors( unsigned char drive, 
                           unsigned char todo, 
                           unsigned char head, 
                           unsigned int cylinder, 
                           unsigned char sector, 
                           unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 2;                         /* AH = 2 - czytaj sektory */
regs.h.al = todo;                      /* AL = liczba sektor¢w */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;                      /* DH = numer gˆowicy */
regs.h.dl = drive;                     /* DL = numer nap©du */
regs.h.ch = cylinder;                  /* CH = mˆodsze 8 bit¢w nru cylindra */
regs.h.cl = cylinder >> 8;             /* CL = starsze 2 bity nru cylindras */
regs.h.cl <<= 6;                       /*      na pozycji xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = numer sektora */
sregs.es  = FP_SEG(bufferPtr);         /* ES = segment bufora */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 03h - Zapisz sektory (dyskietka lub dysk twardy) */

unsigned char writeSectors( unsigned char drive, 
                            unsigned char todo, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector, 
                            unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 3;                         /* AH = 3 - zapisz sektory */
regs.h.al = todo;                      /* AL = liczba sektor¢w */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;                      /* DH = numer gˆowicy */
regs.h.dl = drive;                     /* DL = numer nap©du */
regs.h.ch = cylinder;                  /* CH = mˆodsze 8 bit¢w nru cylindra */
regs.h.cl = cylinder >> 8;             /* CL = starsze 2 bity nru cylindra */
regs.h.cl <<= 6;                       /*      na pozycji xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = numer sektora */
sregs.es  = FP_SEG(bufferPtr);         /* ES = segment bufora */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 04h - Weryfikuj sektory (dyskietka lub dysk twardy) */

unsigned char verifySectors( unsigned char drive, 
                             unsigned char numberOfSectors, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector,
                             unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 4;                         /* AH = 4 - weryfikuj sektory */
regs.h.al = numberOfSectors;           /* AL = liczba sektor¢w */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;                      /* DH = numer gˆowicy */
regs.h.dl = drive;                     /* DL = numer nap©du */
regs.h.ch = cylinder;                  /* CH = mˆodsze 8 bit¢w nru cylindra */
regs.h.cl = cylinder >> 8;             /* CL = starsze 2 bity nru cylindra */
regs.h.cl <<= 6;                       /*      na pozycji xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = numer sektora */
sregs.es  = FP_SEG(bufferPtr);         /* ES = sefment bufora */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 05h - formatuj ˜cie¾k© (dyskietka) */

unsigned char formatTrack( unsigned char drive, 
                           unsigned char head, 
                           unsigned char track, 
                           unsigned char numberOfSectors, 
                           unsigned char far *trackMap )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 5;                         /* AH = 5 - formatuj ˜cie¾k© */
regs.h.al = numberOfSectors;           /* AL = liczba sektor¢w */
regs.h.ch = track;                     /* CH = numer ˜cie¾ki */
regs.h.dh = head;                      /* DH = numer gˆowicy */
regs.h.dl = drive;                     /* DL = numer nap©du */
regs.x.bx = FP_OFF(trackMap);          /* BX = przesuni©cie (offset) mapy ˜cie¾ki */
sregs.es  = FP_SEG(trackMap);          /* ES = segment mapy ˜cie¾ki */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 05h - Formatuj ˜cie¾k© (dysk twardy) */

unsigned char formatHdTrack( unsigned char drive, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector, 
                             unsigned char far *flagTable )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 5;                         /* AH = 5 - formatuj ˜cie¾k© */
regs.x.bx = FP_OFF(flagTable);         /* BX = przesuni©cie (offset)
                                                     tablicy flagtbl */
regs.h.dh = head;                      /* DH = numer gˆowicy */
regs.h.dl = drive;                     /* DL = numer nap©du */
regs.h.ch = cylinder;                  /* CH = mˆodsze 8 bit¢w nru cylindra */
regs.h.cl = cylinder >> 8;             /* CL = starsze 2 bity nru cylindra */
regs.h.cl <<= 6;                       /*      na pozycji xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = numer sektora */
sregs.es  = FP_SEG(flagTable);         /* ES = segment adresu flagtbl */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 08h - Czytaj parametry stacji (dyskietka) */

unsigned char getDisketteGeometry( unsigned char drive,
                                   unsigned int *maxTrack,
                                   unsigned char *maxSector,
                                   unsigned char *maxHead,
                                   unsigned char *nrDrives,
                                   unsigned char far **fdpt )
{
union REGS regs;
struct SREGS sregs;

unsigned char far *pCh;

regs.h.ah = 8;                         /* AH = 8 - czytaj paramety stacji */
regs.h.dl = drive;                     /* DL = numer stacji */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */

if ( regs.x.cflag )                    /* Je¾eli bˆ¥d... */
   return( regs.h.ah );                /* Zwr¢† AH = stan */

*maxSector = regs.h.cl & 63;           /* CL ..xxxxxx = maks. nr sektora */
*maxTrack = regs.h.ch;                 /* CH = maks. nr ˜cie¾ki */
*maxHead = regs.h.dh;                  /* DH = maks. nr gˆowicy */
*nrDrives = regs.h.dl;                 /* DL = liczba stacji */

(unsigned long)pCh = sregs.es;         /* ES = segment adresu tablicy */
(unsigned long)pCh <<= 16;             /* Przesuni©cie na miejsce */
(unsigned long)pCh |= regs.x.di;       /* DI = przesuni©cie (offset) tablicy */

*fdpt = pCh;                           /* Przepisz do parametru */
return( 0 );                           /* Zwr¢† 0 = OK */
}



/* Przerwanie 13h / 08h - Czytaj parametry dysku (dysk twardy) */

unsigned char getHdGeometry( unsigned char drive,
                             unsigned int *maxCylinder,
                             unsigned char *maxSector,
                             unsigned char *maxHead,
                             unsigned char *nrDrives,
                             unsigned char far **fdpt )
{
union REGS regs;
struct SREGS sregs;

unsigned int tmp;
unsigned char far *pCh;

regs.h.ah = 8;                         /* AH = 8 - czytaj paramerty dysku */
regs.h.dl = drive;                     /* DL = numer dysku */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */

if ( regs.x.cflag )                    /* Je¾eli bˆ¥d... */
   return( regs.h.ah );                /* Zwr¢† AH = stan */

*maxSector = regs.h.cl & 63;           /* CL ..xxxxxx = maks. nr sektora */

tmp = regs.h.cl & (128+64);            /* CL xx...... = 2 starsze bity liczby cyl. */
tmp <<= 2;                             /* Poˆ¥czenie ich z CH */
tmp |= regs.h.ch;                      /* CH = mˆodsze 8 bit¢w liczby cyl. */

*maxCylinder = tmp;                    /* Przepisanie */
*maxHead = regs.h.dh;                  /* DH = maks. nr gˆowicy */
*nrDrives = regs.h.dl;                 /* DL = liczby dysk¢w */

(unsigned long)pCh = sregs.es;         /* ES = segment adresu tablicy parametr¢w */
(unsigned long)pCh <<= 16;             /* Ustawienie na miejscu */
(unsigned long)pCh |= regs.x.di;       /* DI = przesuni©cie (offset) adresu
                                                  tablicy parametr¢w */

*fdpt = pCh;                           /* Przepisanie */
return( 0 );                           /* Zwr¢† 0 = OK */
}



/* Przerwanie 13h / 09h - Inicjalizacja parametr¢w dysku (dysk tawrdy) */

unsigned char initHdController( unsigned char drive )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - inicjalizacja par. dysku */
regs.h.dl = drive;            /* DL = numer dysku */
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 0Ah - Czytaj "dˆugie" sektory (dysk twardy) */

unsigned char readLong( unsigned char drive, 
                        unsigned char numberOfSectors, 
                        unsigned char head, 
                        unsigned int cylinder, 
                        unsigned char sector, 
                        unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 0x0a;                      /* AH = 0x0A - czytaj "dˆugie" sektory */
regs.h.al = numberOfSectors;           /* AL = liczba sektor¢w */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;                      /* DH = numer gˆowicy */
regs.h.dl = drive;                     /* DL = numer dysku */
regs.h.ch = cylinder;                  /* CH = mˆodsze 8 bit¢w nru cylindra */
regs.h.cl = cylinder >> 8;             /* CL = starsze 2 bity nru cylindra */
regs.h.cl <<= 6;                       /*      na pozycji xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = numer sektora */
sregs.es  = FP_SEG(bufferPtr);         /* ES = segment adresu bufora */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 0Bh - Zapisz "dˆugie" sektory (dysk twardy) */

unsigned char writeLong( unsigned char drive, 
                         unsigned char numberOfSectors, 
                         unsigned char head, 
                         unsigned int cylinder, 
                         unsigned char sector, 
                         unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 0x0b;                      /* AH = 0x0B - zapisz dˆugie sektory */
regs.h.al = numberOfSectors;           /* AL = liczby sektor¢w */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;                      /* DH = numer gˆowicy */
regs.h.dl = drive;                     /* DL = numer dysku */
regs.h.ch = cylinder;                  /* CH = mˆodsze 8 bit¢w nru cylindra */
regs.h.cl = cylinder >> 8;             /* CL = starsze 2 bity nru cylindra */
regs.h.cl <<= 6;                       /*      na pozycji xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = numer sektora */
sregs.es  = FP_SEG(bufferPtr);         /* ES = segment adresu bufora */
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 0Ch - Id« do cylindra (dysk twardy) */

unsigned char seekCylinder( unsigned char drive, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector )
{
union REGS regs;

regs.h.ah = 0x0c;             /* AH = 0x0C - id« do cylindra */
regs.h.dh = head;             /* DH = numer gˆowicy*/
regs.h.dl = drive;            /* DL = numer nap©du*/
regs.h.ch = cylinder;         /* CH = mˆodsze 8 bit¢w nru cylindra */
regs.h.cl = cylinder >> 8;    /* CL = starsze 2 bity nru cylindra */
regs.h.cl <<= 6;              /*      na pozycji xx...... */
regs.h.cl |= ( sector & 63 ); /* CL ..xxxxxx = numer sektora */
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 0Dh - Alternatywny reset dysku (dysk twardy) */

unsigned char altDiskReset( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x0d;             /* AH = 0x0D - alt. reset dysku */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 10h - Sprawdzenie gotowo˜ci dysku (dysk twardy) */

unsigned char testDriveReady( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x10;             /* AH = 0x10 - sprawdzenie gotowo˜ci */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 11h - Rekalibracja dysku (dysk twardy) */

unsigned char recalibrateDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x11;             /* AH = 0x11 - rekalibracja dysku */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 12h - Wykonaj diagnostyk© RAM sterownika (dysk twardy) */

unsigned char doControllerRAMDiagnostic( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x12;             /* AH = 0x12 - wykonaj diagnostyk© */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 13h - Wykonanie diagnostyki sterownika (dysk twardy) */

unsigned char doControllerDiagnostic( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x13;             /* AH = 0x13 - wykonaj diagnostyk© */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 14h - Wykonaj diagnostyk© sterownika (dysk twardy) */

unsigned char executeControllerDiagnostic( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x14;             /* AH = 0x14 - wykonaj diagnostyk© */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 15h - Czytaj typ stacji (dyskietka) */

unsigned char getDriveType( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x15;             /* AH = 0x15 - czytaj typ stacji */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return ( regs.h.ah );         /* AH = stan */
}



/* Przerwanie 13h / 15h - Czytaj typ dysku (dysk twardy) */

unsigned char driveIsValid( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x15;             /* AH = 0x15 - czytaj typ dysku */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



unsigned char getSectorsOnDrive( unsigned char drive,
                                 unsigned long *out )
{
union REGS regs;

*out = 0L;
regs.h.ah = 0x15;             /* AH = 0x15 - czytaj typ dysku */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
if ( !regs.x.cflag )          /* Je¾eli OK... */
{
   *out = regs.x.cx;          /* CX = liczba sektor¢w - starsze 16 bit¢w */
   *out <<= 16;
   *out += regs.x.dx;         /* DX = liczba sektor¢w - mˆodsze 16 bit¢w */
   }
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 13h / 16h - Sprawd« stan linii zmiany no˜nika (dyskietka) */

unsigned char getChangeSignal( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x16;             /* AH = 0x16 - czytaj stan linii
                                                zmiany no˜nika */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return ( regs.h.ah );         /* AH = stan (0, 1, lub 6) */
}



/* Przerwanie 13h / 17h - Ustaw typ dyskietki do formatowania (dyskietka) */

unsigned char setDriveType( unsigned char drive, 
                            unsigned char type )
{
union REGS regs;

regs.h.ah = 0x17;             /* AH = 0x17 - ustaw rodzaj stacji/no˜nika */
regs.h.al = type;             /* AL = rodzaj stacji/no˜nika */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return ( regs.h.ah );         /* AH = stan */
}



/* Przerwanie 13h / 18h - Ustaw typ no˜nika do formatowania (dyskietka) */

unsigned char setMediaType( unsigned char drive, 
                            unsigned char maxTracks,
                            unsigned char sectorsPerTrack,
                            unsigned char far **tablePtr )
{
union REGS regs;
struct SREGS sregs;

unsigned char far *pCh;

regs.h.ah = 0x18;                      /* AH = 0x18 - ustaw rodzaj no˜nika */
regs.h.ch = maxTracks;                 /* CH = maks. nr ˜cie¾ki */
regs.h.cl = sectorsPerTrack;           /* CL = sektory na ˜cie¾ce */
regs.h.dl = drive;                     /* DL = numer nap©du*/
int86x( 0x13, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */

if ( !regs.x.cflag )                   /* Je¾eli OK... */
{
   (unsigned long)pCh = sregs.es;      /* ES = segment adresu
                                               tablicy parametr¢w */
   (unsigned long)pCh <<= 16;          /* Przesuni©cie na wˆa˜ciw¥ pozycj© */
   (unsigned long)pCh |= regs.x.di;    /* DI = przesuni©cie (offset) adresu
                                               tablicy parametr¢w */
   *tablePtr = pCh;                    /* Przepisanie */
   }

return( regs.h.ah );                   /* AH = stan */
}



/* Przerwanie 13h / 19h - Parkuj gˆowice (dysk twardy) */

unsigned char parkHeads( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x19;             /* AH = 0x19 - parkuj gˆowice */
regs.h.dl = drive;            /* DL = numer nap©du*/
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}

