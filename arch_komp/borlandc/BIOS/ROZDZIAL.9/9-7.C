/*
   BIOS dla programuj�cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-7.  Uruchomienie i skasowanie timera
*/


/* Skasowanie timera */
void cancelTimer( void )
{
union REGS regs;

regs.h.ah = 0x83;            /* AH = 83h - Ustawienie/skasowanie timera */
regs.h.al = 1;                  /* AL = 1 skasowanie timera */
int86( 0x15, &regs, &regs );    /* Wywo�anie funkcji BIOS */
}                               /* Niczego nie zwracamy */


unsigned char doneFlag;      /* Zmienna znacznikowa zako�czenia odliczania
                                (zmienna globalna) */

/*
   Ustawienia i uruchomienie timera na n mikrosekund.  
   Zwraca 0 = timer uruchomiony lub 
   >0 = timer nie uruchomiony, poniewa� jest obecnie zaj�ty.
 */
unsigned int startTimer( unsigned long timeValue,
                         unsigned char *doneFlagP )
{
union REGS regs;
struct SREGS sregs;

*doneFlagP = 0;                   /* Wyzerowanie zmiennej znacznikowej */

regs.h.ah = 0x83;            /* AH = 83h - Ustawienie/skasowanie timera */
regs.h.al = 0;                         /* AL = 0 ustawienie timera */
regs.x.cx = (timeValue >> 16);         /* CX = starsze 16 bit�w czasu */
regs.x.dx = timeValue;                 /* DX = m�odsze 16 bit�w czasu */
regs.x.bx = FP_OFF(doneFlagP);   /* BX = przesuni�cie (offset) znacznika */
sregs.es  = FP_SEG(doneFlagP);         /* ES = segment znacznika */
int86x( 0x15, &regs, &regs, &sregs );  /* Wywo�anie funkcji BIOS */
return( regs.x.cflag );                /* Zwracamy znacznik Carry */
}

/* 
   Makrodefinicja zwracaj�ca 1 je�eli up�yn�� zadany czas 
   lub 0 je�eli timer jeszcze pracuje.  Spos�b u�ycia:
      if ( timerExpired(doneFlag) )
      {
         }
*/
#define timerExpired( timerVariable ) ( timerVariable != 0 )
