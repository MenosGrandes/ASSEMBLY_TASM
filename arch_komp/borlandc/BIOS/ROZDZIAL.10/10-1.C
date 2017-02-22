/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-1.  Odczytanie informacji o gnie«dzie EISA
*/


/*
   Odczytanie informacji o wybranym gnie«dzie EISA.
   Zwraca 0 je¾eli operacja udana lub kod bˆ©du, je¾eli nie.
*/

struct EISASlotInfo
{ unsigned int  boardIdHighPart;
  unsigned int  boardIdLowPart;
  unsigned char boardInfo;
  unsigned char cfgUtilMajorVer;
  unsigned char cfgUtilMinorVer;
  unsigned int  checkSum;
  unsigned char functionCount;
  unsigned char functionSummary;
  };

unsigned int readEISASlotInfo( unsigned char slot,
                               struct EISASlotInfo far *slotInfoP )
{
union REGS regs;
struct SREGS sregs;

regs.x.ax = 0xd800;                         /* AX = D800h - podaj info */
regs.h.cl = slot;                           /* CL = numer gniazda */
int86x( 0x15, &regs, &regs, &sregs );       /* Wywoˆanie funkcji BIOS */

if ( regs.x.cflag == 0 )                    /* Je¾eli OK, */
{
   slotInfoP->boardIdHighPart = regs.x.si;  /* SI = ID karty starsze
                                                            16 bit¢w */
   slotInfoP->boardIdLowPart = regs.x.di;   /* DI = ID karty mˆodsze
                                                            16 bit¢w */
   slotInfoP->boardInfo = regs.h.al;        /* AL = informacja o karcie */
   slotInfoP->cfgUtilMajorVer = regs.h.bh;  /* BH = gˆ. nr wersji */
   slotInfoP->cfgUtilMinorVer = regs.h.bl;  /* BL = dod. nr wersji */
   slotInfoP->checkSum = regs.x.cx;         /* CX = suma kontrolna */
   slotInfoP->functionCount = regs.h.dh;    /* DH = licznik funkcji */
   slotInfoP->functionSummary = regs.h.dl;  /* DL = podsumowanie funkcji */
   }

return( regs.h.ah );                       /* AH = 0 gdy sukces lub */
}                                          /*    = kod bˆ©du        */
