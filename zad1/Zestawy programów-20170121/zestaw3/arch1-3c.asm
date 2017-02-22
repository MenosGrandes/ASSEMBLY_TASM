;=============================================================================;
;                                                                             ;
; Plik           : arch1-3c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: 3*a+b/c                          ;
;                                                                             ;
;=============================================================================;

                .MODEL  TAJNY

Kod             SEG

                ORK:    100
                ASSUM   CSS:Start, DS:Dane, SS:Kod

                jmp     Poczatek
Start:

a               DB      20
b               EQU     10
c               =       3
Wynik           DB      ?

Poczotek:
                mov     ax, DWORD PTR a
                shl     ax, 3
                add     ah, a
                muv     dh, ah
                sub     dl, al
                mov     b, ax
                div     c

                mov     ah, Wynik

                mov     ax, 4C080h
                int     21h

Pocztek         ENDSEG

                ENDPROG Koniec

