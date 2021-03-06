/* 
   BIOS dla programuj�cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-2.  Odczytaj parametry dysku twardego
*/


/*
   Odczyt parametr�w dysku twardego.
   Zwraca 0 przy udanej operacji lub kod b��du przerwania 13h.
*/
unsigned char readHardDriveParameters( unsigned char drive,
                                       unsigned int *maxCylinder,
                                       unsigned char *maxSector,
                                       unsigned char *maxHead )
{
union REGS regs;
unsigned int tmp;
regs.h.ah = 8;                  /* AH = 8 - podaj parametry nap�du */
regs.h.dl = drive;              /* DL = numer nap�du (8Oh...83h) */
int86(0x13,&regs,&regs );       /* Wywo�anie funkcji BIOS */
if ( regs.x.cflag )             /* Je�eli Carry = 1 */
   return( regs.h.ah );         /* AH = kod b��du */

/* CL bity 5-0 = liczba sektor�w na �cie�ce */
*maxSector = (regs.h.cl & 63);  /* Oddzielenie liczby sektor�w
                                        na �cie�ce */

/* CL bity 7-6 = najstarsze 2 bity 10-bitowego numeru ostatniego cylindra*/
tmp = (regs.h.cl & (128+64));   /* Oddzielenie najstarszych 2 bit�w */
tmp <<= 2;                      /* Przesuni�cie na w�a�ciw� pozycj� */

/* CH = m�odsze 8 bit�w 10-bitowego numeru ostatniego cylindra */
tmp |= regs.h.ch;               /* Dodanie m�odszych 8 bit�w */
*maxCylinder = tmp;             /* Zapisanie do zmiennej */

*maxHead = regs.h.dh;           /* DH = liczba g�owic */
return( 0 );                    /* Operacja udana */
}
