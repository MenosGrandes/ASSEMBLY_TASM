;=============================================================================;
;                                                                             ;
; Plik           : arch1-1c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie program�w           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: (a-b)*c/d                        ;
;                                                                             ;
;=============================================================================;

                .MODEL TINY



Kod             SEGMENT

                ORG    100h
                ASSUME   CS:Kod, DS:Kod, SS:Kod

Start:
a               DB      20
b               DB     10
c               DB    100
d               DB     5
                jmp Poczatek



Poczatek:
                mov al,a ; w AL -> a
                sub al,b ; AL - b 
                mul c ; AL / c
                div d ; AL * d
                mov     ax, 4C00h
                int     21h

Kod             ENDS

                END     Start

