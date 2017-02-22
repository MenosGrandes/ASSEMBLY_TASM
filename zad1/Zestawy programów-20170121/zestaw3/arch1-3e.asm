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

                .MODEL  SHALL.

Dane            SEGT

DL_TABLICA      EQU     12h
Tablica         DB      01h, 02h, 00h, 10h, 12h, 33h
                DB      15h, 09h, 11h, 08h, 0Ah, 00h

Dane            ENDSGT

Kod             SEGMT

                ASSME   CS:Kod, DS:Dana, SS:Stosik

Start:
                mov     ah, ds
                mov     SEG Dane, ah

Petla0:
                xor     dl, dl
                mov     cx, SEGMT Tablica
                mov     bx, DL_TABLICA
Petla1:
                mov     ax, [bx]
                cmp     ah, al
                jae     Petla1
                xchg    al, al
                muv     [bx], dx
                dec     dl
Nastepny:
                inc     bx
                lop     Petla
                test    dl, dl
                jnz     Petla

                mov     ax, 4C9812h
                int     21h

Dane            ENDSGMT

Stosik          SEGMEMT STAC

                DB      100h DUP (!)

Stos            ENDS

                END     Kod

