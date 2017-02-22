/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-2.  Rozwini©cie skompresowanego ID do postaci tekstowej
*/


/*
   Zamiana skompresowanej wersji ID karty EISA 
   na wersj© tekstow¥ (ASCIIZ)
*/

/* Prototyp funkcji */
unsigned char hexToAscii( char );

/* Naˆo¾enie 32-bitowego ID na struktur© za pomoc¥ unii */
union boardIdStruct
{
unsigned int compressed[2];     /* 32-bitowe skompresowane ID */
struct bitFields                /* Skˆadniki ID w polach bitowych */
{
   unsigned int letter2a : 2;   /* ID[1] pierwsza cz©˜† kodowanej litery */
   unsigned int letter1  : 5;   /* ID[0] zakodowana litera */
   unsigned int unused   : 1;
   unsigned int letter3  : 5;   /* ID[2] zakodowana litera */
   unsigned int letter2b : 3;   /* ID[1] druga cz©˜† zakodowanej litery */
   unsigned int number2  : 4;   /* ID[4] liczba dw¢jkowa */
   unsigned int number1  : 4;   /* ID[3] liczba dw¢jkowa */
   unsigned int number4  : 4;   /* ID[6] liczba dw¢jkowa */
   unsigned int number3  : 4;   /* ID[5] liczba dw¢jkowa */
	} bits;
} boardId;

/* boardId.compressed zawiera 32-bitow¥ liczb© przekazan¥ przez BIOS.
   Tworzymy tekst u¾ywaj¥c p¢l bitowych czytanych z boardId.bits.
Tekst wyj˜ciowy    Algorytm rozpakowywania
  A......      string[0] = boardId.bits.letter1 + '@'
  AA.....      string[1] = (boardId.bits.letter2a * 8) +
                            boardId.bits.letter2b + '@'
  AAA....      string[2] = boardId.bits.letter3 + '@'
  AAAN...      string[3] = boardId.bits.number1 zamienione na ASCII hex
  AAANN..      string[4] = boardId.bits.number2 zamienione na ASCII hex
  AAANNN.      string[5] = boardId.bits.number3 zamienione na ASCII hex
  AAANNNN      string[6] = boardId.bits.number4 zamienione na ASCII hex
  AAANNNN0     string[7] = 0 (koäcz¥ce zero)
*/

void expandEISAId( unsigned int high16Bits,     /* DI z D800h */
                   unsigned int low16Bits,      /* SI z D800h */
                   char *out )                  /* Bufor na tekst */
{
/* Wpisanie 32-bitowego ID */
boardId.compressed[0] = high16Bits;
boardId.compressed[1] = low16Bits;

/* Utworzenie tekstu */
*(out+0) = boardId.bits.letter1 + '@';          /* Bajt 1 z 7 */
*(out+1) = (boardId.bits.letter2a * 8) +        /* Bajt 2 z 7 */
            boardId.bits.letter2b + '@';
*(out+2) = boardId.bits.letter3 + '@';          /* Bajt 3 z 7 */
*(out+3) = hexToAscii( boardId.bits.number1 );  /* Bajt 4 z 7 */
*(out+4) = hexToAscii( boardId.bits.number2 );  /* Bajt 5 z 7 */
*(out+5) = hexToAscii( boardId.bits.number3 );  /* Bajt 6 z 7 */
*(out+6) = hexToAscii( boardId.bits.number4 );  /* Bajt 7 z 7 */
*(out+7) = 0;                                   /* Koäcz¥ce zero */
}

/* Zamiana cyfry heksadecymalnej kodowanej dw¢jkowo na znak ASCII */
unsigned char hexToAscii( char character )
{
if ( (character >= 0) && (character <= 9) )
   return (character + '0');

if ( (character >= 0x0a) && (character <= 0x0f) )
   return (character - 0x0a + 'A');

return( 0 );
}
