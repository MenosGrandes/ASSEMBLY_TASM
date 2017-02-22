/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-5.  Odczytanie stanu niebuforowanych klawiszy 
*/


/*
   Zwraca bie¾¥cy stan klawiszy Shift, Ctrl, Alt, 
   Num Lock, Ins i Caps Lock
*/

void readKeyStatus( unsigned char *keyStatus1,
                    unsigned char *keyStatus2 )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 lub 12h - odczyt stanu klawiszy */
int86( 0x16, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

*keyStatus1 = regs.h.al;    /* AL = stan klawiszy dla wszystkich
                                      rodzaj¢w klawiatur */
*keyStatus2 = regs.h.ah;    /* AH = dodatkowe informacje dla funkcji 12h */

/* Znaczenie bit¢w w rejestrze AL:
      bit 7 : 1 = Insert On, wˆ¥czony tryb wstawiania
      bit 6 : 1 = Caps Lock wˆ¥czony
      bit 5 : 1 = Num Lock wˆ¥czony
      bit 4 : 1 = Scroll Lock wˆ¥czony
      bit 3 : 1 = naci˜ni©ty jest klawisz Alt
      bit 2 : 1 = naci˜ni©ty jest klawisz Ctrl
      bit 1 : 1 = naci˜ni©ty jest lewy klawisz Shift
      bit 0 : 1 = naci˜ni©ty jest prawy klawisz Shift
   Znaczenie bit¢w rejestru AH (tylko funkcja 12h):
      bit 7 : 1 = naci˜ni©ty jest klawisz SysReq
      bit 6 : 1 = naci˜ni©ty jest klawisz Caps Lock
      bit 5 : 1 = naci˜ni©ty jest klawisz Num Lock
      bit 4 : 1 = naci˜ni©ty jest klawisz Scroll Lock
      bit 3 : 1 = naci˜ni©ty jest prawy klawisz Alt
      bit 2 : 1 = naci˜ni©ty jest prawy klawisz Ctrl
      bit 1 : 1 = naci˜ni©ty jest lewy klawisz Alt
      bit 0 : 1 = naci˜ni©ty jest lewy klawisz Ctrl
*/
}                             /* Niczego nie zwracamy */
