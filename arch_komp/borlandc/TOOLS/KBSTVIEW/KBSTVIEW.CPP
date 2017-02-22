#include <resident.h>


/*--------------------------------------------------------------------------*/


void DrawKey(char *Symbol, int Code, unsigned char Byte)
{

  if ( (Byte & Code) == Code) SetTextColor(WHITE);
  else SetTextColor(LIGHTGRAY);
  Show(Symbol);

}


/*--------------------------------------------------------------------------*/


void DrawStBytes(KBDSTATUS KbdSt)
{


 SetLogWin(1,5,80,10);

 SetTextColor(LIGHTGRAY);
 SetTextBck(BLACK);
 SetPos(1,1);
 Show("bajt stanu klawiatury nr 1:");

 SetPos(1,3);
 DrawKey("Ins   ",128,KbdSt.StatusByte1);
 DrawKey("CapsLock   ",64,KbdSt.StatusByte1);
 DrawKey("NumLock   ",32,KbdSt.StatusByte1);
 DrawKey("ScrollLock   ",16,KbdSt.StatusByte1);
 DrawKey("Alt   ",8,KbdSt.StatusByte1);
 DrawKey("Ctrl   ",4,KbdSt.StatusByte1);
 DrawKey("Shift lewy   ",2,KbdSt.StatusByte1);
 DrawKey("Shift prawy   ",1,KbdSt.StatusByte1);


 SetLogWin(1,15,80,20);

 SetTextColor(LIGHTGRAY);
 SetTextBck(BLACK);
 SetPos(1,1);
 Show("bajt stanu klawiatury nr 2:");

 SetPos(1,3);
 DrawKey("Ins   ",128,KbdSt.StatusByte2);
 DrawKey("CapsLock   ",64,KbdSt.StatusByte2);
 DrawKey("NumLock   ",32,KbdSt.StatusByte2);
 DrawKey("ScrollLock   ",16,KbdSt.StatusByte2);
 DrawKey("      ",8,KbdSt.StatusByte2);
 DrawKey("       ",4,KbdSt.StatusByte2);
 DrawKey("Alt   lewy   ",2,KbdSt.StatusByte2);
 DrawKey("Ctrl  lewy   ",1,KbdSt.StatusByte2);


}


/*--------------------------------------------------------------------------*/


void main()
{

 KBDSTATUS KbdSt;

 _setcursortype(_NOCURSOR);
 ClrScr();
 SetPos(1,25);
 Show("Esc - wyjscie");

 do
 {
  GetKbdStatus(&KbdSt);
  DrawStBytes(KbdSt);
 }
 while ( (KbdSt.Key.ASCII!=27) || (KbdSt.Key.SCAN!=1) );

 _setcursortype(_NORMALCURSOR);
 SetLogWin(1,1,80,25);
 SetTextColor(LIGHTGRAY);
 SetTextBck(BLACK);
 ClrScr();

}


/*--------------------------------------------------------------------------*/