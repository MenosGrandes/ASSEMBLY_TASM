/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-4.  Pobranie bajtu stanu portu szeregowego
*/


/* Pobranie informacji o stanie linii i stanie modemu */
void readSerialPortStatus( unsigned char whichPort,
                           unsigned char *lineStatus,
                           unsigned char *modemStatus )
{
union REGS regs;

regs.h.ah = 3;                 /* AH = funkcja 3                 */
regs.x.dx = whichPort;         /* DX = port szeregowy - 0,1,2,3  */
int86( 0x14, &regs, &regs );   /* Wywoˆanie funkcji              */

*lineStatus = regs.h.ah;       /* Zwraca: AH = stan linii        */
*modemStatus = regs.h.al;      /*    oraz AL = stan modemu       */
}
