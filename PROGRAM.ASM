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

                .MODEL  SMALL
.stack
Dane            SEGMENT

DL_TABLICA      EQU     12h
Tablica         DB      01h, 02h, 00h, 10h, 12h, 33h,15h, 09h, 11h, 08h, 0Ah, 00h

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
				mov		cx, DL_TABLICA
				sub		cx,dx
Petla1:
				mov 	ax,[bx]
                cmp     ah,al
                jbe     Nastepny
                xchg    ah, al
Nastepny:
                inc     bx
                dec		cx
                loop     Petla1
				inc		dx
				;cmp		
				;jbe Petla0
				
				
                mov     ax, 4C00h
                int     21h

Kod            ENDS


END     Start

