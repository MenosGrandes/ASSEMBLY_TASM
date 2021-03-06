/* *********************************************************
   CIRCLE.C
   Copyright (c) 1992 - Billy P. Taylor
   *********************************************************
   Rysuje okr�gi u�ywaj�c funkcji 0Ch przerwania 10h.
   Algorytm polega na obliczaniu �uku zataczanego wok��
   danego punktu. Obliczenia wykonywane s� jedynie w pierwszej
   �wiartce, a rysowanie odbywa si� symetrycznie we wszystkich
   czterech �wiartkach.

   Aby uruchomi� program napisz CIRCLE <Enter>
   *********************************************************** */

#include <conio.h>
#include <dos.h>
#include <math.h>

/* Poni�sze 4 definicje okre�laj� parametry trybu graficznego */
#define GRAPHICS_MODE    4      /* tryb graficzny BIOS */
#define HRES           320      /* Rozdzielczo�� pozioma */
#define VRES           200      /* Rozdzielczo�� pionowa */
#define COLORS           4      /* Liczba kolor�w */
#define CIRCLE_SIZE (VRES / 2)  /* * 2 = �rednica okr�gu w pikselach */

/* Algorytm zak�ada, �e piksele ekranu s� kwadratowe.
   W rzeczywistych trybach graficznych(320x200, 640x200, 640x350
   lub jeszcze innych) tak jednak nie jest.
   Bez poprawki algorytm b�dzie rysowa� elipsy, a nie okr�gi!
   Aby skompensowa� ten efekt wprowadzamy wsp��czynnik korekcji
   proporcji wsp��rz�dnych ekranu (aspect ratio). Je�eli u�ywasz
   innego trybu graficznego musisz zmieni� sta�� ASPECT. Poni�ej
   znajduj� si� warto�ci dla podstawowych tryb�w graficznych:

   320 x 200 : 1.10           640 x 200 : 2.25
   640 x 350 : 1.30           640 x 480 : 1.05
*/
#define ASPECT     1.10       /* Korekta proporcji */

#define PAGE_0         0      /* Wybrana strona */
#define CENTER_H (HRES/2)     /* �rodek ekranu - o� x */
#define CENTER_V (VRES/2)     /* �rodek ekranu - o� y */

/* Sta�a STEP wyznacza g�sto�� obliczania punkt�w na okr�gu
   (jak g�sto b�d� one rysowane). Im wi�ksza liczba, tym mniej
   wykonywanych b�dzie oblicze�, ale pomi�dzy punktami mog�
   pojawi� si� przerwy. Im mniejsza liczba, tym wolniejsze
   s� obliczenia, ale okr�g wygl�da lepiej.
*/
#define STEP        .03       /* G�sto�� oblicze� */

#define PI               3.14
#define HALF_PI         (PI/2)

/* Prototypy funkcji */
void          calcPoint    ( float, int, unsigned char );
unsigned char getVideoMode ( void );
void          setVideoMode ( unsigned char );
void          writePixel   ( unsigned char, 
                             unsigned char, 
                             unsigned int, 
                             unsigned int );

void main( void )
{
unsigned char color,
              entryVideoMode;
float point;
int width;

/* Zapami�tanie bie��cego trybu karty i prze��czenie na tryb graficzny */
entryVideoMode = getVideoMode();
setVideoMode( GRAPHICS_MODE );

color = 1;    /* Okre�lenie atrybutu dla ka�dego piksela */

for( width = 1; width < CIRCLE_SIZE; width++ )
{
/* Przerwij je�eli naci�ni�to jaki� klawisz */
   if ( kbhit() )
      break;

/* Rysuj okr�g */
   for( point = 0; 
        point < HALF_PI; 
        point += STEP )
      calcPoint( point, width, color );

   color++;                  /* Zmie� kolor */
   color %= COLORS;        /* Nie przekraczaj maksymalnej liczby kolor�w */
   }

/* Pomi� znak, kt�ry spowodowa� przerwanie przez funkcj� kbhit() */
getch();

/* Odtw�rz pocz�tkowy tryb karty graficznej */
setVideoMode( entryVideoMode );
}

/* Oblicz wsp��rz�dne punktu na okr�gu
   i narysuj punkty w czterech �wiartkach */

void calcPoint( float point, 
                int width, 
                unsigned char color )
{
float px, py;
int q1_x, q1_y,
    q2_x, q2_y,
    q3_x, q3_y,
    q4_x, q4_y;

/* Obliczenie punktu na okr�gu */
px  = cos( point ) * width;
py  = sin( point ) * width;

/* Poprawka wsp. x (proporcje osi - ma by� okr�g, a nie elipsa) */
px *= ASPECT;

q1_x = CENTER_H - px;           /* Punkt w �wiartce I    */
q1_y = CENTER_V - py;
q2_x = CENTER_H - px;           /* Punkt w �wiartce II   */
q2_y = CENTER_V + py;
q3_x = CENTER_H + px;           /* Punkt w �wiartce III  */
q3_y = CENTER_V - py;
q4_x = CENTER_H + px;           /* Punkt w �wiartce IV   */
q4_y = CENTER_V + py;

/* Rysowanie pikseli */
writePixel( color, PAGE_0, q1_y, q1_x );
writePixel( color, PAGE_0, q2_y, q2_x );
writePixel( color, PAGE_0, q3_y, q3_x );
writePixel( color, PAGE_0, q4_y, q4_x );
}

/* Ustawienie wybranego trybu graficznego */
void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - ustawienie trybu graficznego */
regs.h.al = newMode;          /* AL = tryb graficzny */
int86( 0x10, &regs, &regs );  /* Wywo�anie funkcji BIOS */
}

/* Odczytanie bie��cego trybu graficznego */
unsigned char getVideoMode( void )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0x0F - podaj tryb graficzny */
int86( 0x10, &regs, &regs );  /* Wywo�anie funkcji BIOS */
return( regs.h.al );          /* AL = bie��cy tryb graficzny */
}

/* Narysowanie piksela w trybie graficznym */
void writePixel( unsigned char color, 
                 unsigned char videoPage, 
                 unsigned int row, 
                 unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0c;             /* AH = 0x0C - narysuj piksel */
regs.h.al = color;            /* AL = kolor */
regs.h.bh = videoPage;        /* BH = strona na kt�rej nale�y narysowa� */
regs.x.cx = column;           /* CX = numer kolumny */
regs.x.dx = row;              /* DX = numer wiersza */
int86( 0x10, &regs, &regs );  /* Wywo�anie funkcji BIOS */
}
