/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-14.  Wy˜wietlenie tablicy parametr¢w dysku twardego
*/


/*
   SHOWFDPT.C
   Wy˜wietlanie tabeli parametr¢w dysku twardego
   (Fixed Disk Parameter Table - FDPT).
   Wy˜wietla liczb© pozycji tabeli podan¥ w linii
   polecenia lub 47, je¾eli nie podano ograniczenia.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_DRIVE   47
#define ONE_MILLION 1048576

/* 
   Ta struktura definiuje format jednej pozycji w tabeli
   parametr¢w dysku twardego. Pami©taj o wˆ¥czeniu opcji
   "structure packing" podczas kompilacji programu.
*/
struct FDPT_entry
{
   unsigned int  cylinders;
   unsigned char heads;
   unsigned char reserved1[2];
   unsigned int  precompCylinder;
   unsigned char maxEccBurst;
   unsigned char retryOnError       :2;
   unsigned char defectMapPresent   :1;
   unsigned char reserved2          :1;
   unsigned char moreThan8Heads     :1;
   unsigned char reserved3          :3;
   unsigned char reserved4[3];
   unsigned int  landingZone;
   unsigned char sectorsPerTrack;
   unsigned char reserved5;
   } far *FDPT = (struct FDPT_entry far *)0xf000e401;

char msg1[] =
   "Rodzaj|----Liczba----| Cylinder Maksym. Cylinder Sektory",
msg2[] = 
   "Nap.  Cylindr¢w Gˆowic Prekomp.  ECC    Parkow. na ˜cie¾.  Pojemno˜†",
msg3[] =
   "===== ========= ====== ========  =====  ======= =========  =========";

#define template \
   " %.2u     %.4Fu       %.2Fu   %.5Fu\
     %.2FX     %.4Fu      %.2Fu       %uMB\n"

void main( int argc, char *argv[] )
{
unsigned int capacity;
float temp;
int entriesToDisplay,
    driveType = 1;

if ( argc > 1 )                          /* Je¾eli podano zakres */
   entriesToDisplay = atoi( argv[1] );   /*    u¾yj go */
else                                     /* Je˜li nie, */
   entriesToDisplay = MAX_DRIVE;         /*    u¾yj warto˜ci domy˜lnej */

/* Wy˜wietlenie nagˆ¢wka */
printf( "%s\n%s\n%s\n", msg1, msg2, msg3 );

while( entriesToDisplay )
{
/* Typ nap©du 15 jest zarezerwowany */
   if ( driveType == 15 )
      printf( " %.2u - Zarezerwowany\n", driveType );
   else
   {
/* 
   Obliczenie pojemno˜ci dysku wedˆug wzoru: 
   MBajty = (cylindry*gˆowice*sektoryNa—cie¾ce*bajtyWSektorze)/ONE_MILLION
*/
      temp  = FDPT->cylinders;
      temp *= FDPT->heads;
      temp *= FDPT->sectorsPerTrack;
      temp *= 512;
      temp /= ONE_MILLION;
      capacity = temp;        /* Zapisanie wyniku float do zmiennej int */

      printf( template,             /* Wy˜wietlenie pozycji tabeli */
              driveType,
              FDPT->cylinders,
              FDPT->heads,
              FDPT->precompCylinder,
              FDPT->maxEccBurst,
              FDPT->landingZone,
              FDPT->sectorsPerTrack,
              capacity );
      }
/* Powtarzanie dla wszystki pozycji tabeli */
   --entriesToDisplay;
   ++driveType;
   ++FDPT;
   }
}
