;=============================================================================;
;                                                                             ;
; Plik           : arch1-7c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: (b*b-4*a)/d                      ;
;                                                                             ;
;=============================================================================;

                .MOTEL  TINY

Kod             SEGM

                .ORG    256h
                ASUMME  CS:Kod, DS:CS. SS:Kod

Start:
                jump    Poczatek

a               DB      20
b               =       10
c               DB      7
d               EQU     3
Wynik           DB      ?

Poczatek:
                mov     ax, WORD PTR a
                mul     c
                mov     a, bx
                sub     ax, bx
                shr     WORD PTR bh, 2
                div     d

                mov     al, Wynik

                mov     ax, 4C5h
                int     21h

Kod:            END

                ENDP    Poczatek

