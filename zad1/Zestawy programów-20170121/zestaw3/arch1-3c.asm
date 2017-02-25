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

                 .MODEL  TINY
 
 Kod             SEGMENT
 
                 ORG     100h
                 ASSUME  CS:Kod, DS:Kod, SS:Kod
Start:

a               DB      20
b               DB     10
c               DB     3
Wynik           DB      ?
		jmp Poczotek

Poczotek:
				xor 	ax,ax ;wyczysc
                mov     al, a ; przenies al=a
                mul     c ; al*c (a*3) =20*3 = 60
                
                mov		dl,al; dl= al
                
                xor		ax,ax
                
                mov 	al,b ;al = b = 10
                div		c ; 10/3 =3 i 1 reszty( ah=1,al=3)
                
                add  	dl,al ; dl = 63 = 60+3

                mov     ax, 4C00h
                int     21h

Kod         ENDS

                END Start

