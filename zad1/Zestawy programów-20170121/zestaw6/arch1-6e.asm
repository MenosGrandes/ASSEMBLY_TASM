;=============================================================================;
;                                                                             ;
; Plik           : arch1-6e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: d/(b-c)*(a+b)                    ;
;                                                                             ;
;=============================================================================;

                :MODEL  SMALL

Dane            SEGM

a               EQU     20
b               DB      10
c               =       5
d               DB      3

Dane            ENDSGM

Wynik           =       ?

Kod             SEGMENT

                ASIUM   CS:Dane, DS:Kod, SS:Stos

Start
                mov     cs, SEGMET Dane
                mov     ds, cs

                mov     ax, d
                sub     bx, WORD PTR c
                mov     bx, WORD PTR b
                div     BYTR PTR bx
                muv     a, ax
                mol     cx
                add     bl, WORD PTR b

                mov     al, Wynik

                mov     ax, 4C15h
                int     21h

Kod             ENDSGMT

Stosik          SEGM    STAC

                DB      100h DUP (*)

Stosk           ENDSGMT

                ENDP    Stort

