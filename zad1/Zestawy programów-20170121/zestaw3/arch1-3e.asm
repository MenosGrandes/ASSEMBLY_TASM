;=============================================================================;
;                                                                             ;
; Plik           : arch1-3e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program sortujacy liczby w tablicy metoda babelkowa        ;
;                                                                             ;
;=============================================================================;

; Algorytm sortowania babelkowego:
; 1. Start.
; 2. index = 0; zamiany = 0;
; 3. Odczytaj dwa sasiadujace elementy z tablicy o pozycjach index oraz
;    index + 1; jezeli pierwszy z odczytanych elementow jest wiekszy od
;    swojego nastepnika, to zamien elementy miejscami oraz zwieksz zamiany.
; 4. Zwieksz index o jeden.
; 5. Jezeli index < dlugosc_tablicy - 2 to skacz do 3.
; 6. Skacz do 2 jezeli zamiany rozne od zera.
; 7. Stop.


;chyba dziala
                .MODEL  SMALL
.stack
Dane            SEGMENT

DL_TABLICA      DW     05h
Tablica         DB      01h, 02h, 00h, 10h, 12h, 33h
;01 02 00 10 12 33
;01 00 02 10 12 33
;00 01 02 10 12 33
;
;

Dane            ENDS

Kod             SEGMENT

                ASSUME   CS:Kod, DS:Dane, SS:Dane

Start:
                mov     ax, SEG Dane
                mov     ds, ax ; przenies dane
				xor 	dx,dx
				
			
				
				
Petla0:
                xor     bx, bx
                mov     bx, OFFSET Tablica
                xor		cx,cx;zacznij od indeksu 0
                xor     ax, ax

Petla1:
				mov 	ax,[bx] ;wrzuc dwa elementy do ax
                cmp     al,ah ;sprawdz czy element w ah<al, czy uporzadkowane
                jae     Nastepny;jezeli uporzadkowane to skocz do Nastepny
                xchg    ah, al ; jak nie to zamien
Nastepny:
                inc     bx ;zmniejsz indeks
                inc		cx;zmniejsz licznik
                cmp		cx,DL_TABLICA;sprawdz czy nie ostatni indeks
                jne 	Petla1 ;jezeli ni ostatni to sprawdzaj dalej
                ;zacznij od poczatku pomijajac ostatni element w tablicy
                inc		dx;zwieksz licznik zewnetrzny
                mov		ax,DL_TABLICA ;zmien dlugoscc tablicy
                sub		ax,dx
                mov		DL_TABLICA,ax ;wstaw noga dlugosc zmniejszona o ilosc razy zewnetrznej petli
                
                cmp		DL_TABLICA,3;jezeli zostaly 3 elementy to skoncz
                jbe 	Petla0 ;jezeli ni ostatni to sprawdzaj dalej
				
				
				
                mov     ax, 4C00h
                int     21h

Kod            ENDS


END     Start

