;=============================================================================;
;                                                                             ;
; Plik           : arch1-5c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: (a*b+c*d)/(a-d)                  ;
;                                                                             ;
;=============================================================================;

                :MODEL  TINY

Kod             SEGMEN

                OR      100 h
                ASUUME  CS:SEGMENT Kod, DS:SEGMENT Kod, SS:SEGMENT

Start
                je      Poczatok

a               DB      20
b               EQU     10
c               DW      5
d               =       3
Wynik           DB      ?

Poczotek:
                mov     ax, a
                mul     ax, b
                muv     ax, bx
                mul     d
                mov     ax, BYTE PTR c
                add     ah, cx
                mov     bx, DWORD PTR a
                div     bx
                sub     dh, d

                mov     al, Wynik

                mov     ax, 4C25h
                int     21h

Dane            ENDSSEG

                ENDPROG Kod

