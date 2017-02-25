;=============================================================================;
;                                                                             ;
; Plik           : arch1-2e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: (3*a-b/a)*(d+3)                  ;
;                                                                             ;
;=============================================================================;
;dziala
                .MODEL SMALL

Dane            SEGMENT

a               DW     5
b               DW     10
c               DW     3
d               DW     5
wynik1			Dw     ?
ENDS           Dane

Kod             SEGMENT

                ASSUME   CS:Kod, DS:Dane, SS:Kod


Start:
                mov     ax, SEG Dane
                mov     ds, ax;przenies dane do ax
                
				xor 	ax,ax ;wyczysc
                mov     ax, a; bx = a
                mul     c ; bl=a*d
                xor     bx,bx
                mov		bx, ax ;bx=ax (bx = 3*a) =15
                
                mov     ax,b ; ax=b
                div		a ; ax=b/a =  2 
                
                sub 	bx , ax ; (3*a) -(b/a) 13 wpisuje do BX
                mov		ax,bx; w ax wpisz to co obliczono
                mov 	dx,d ;do DX wpisz D
                add 	dx,3 ;DX+3
                mul 	dx ;AX * DX
                
                
                mov     ax, 4C00h
                int     21h

Kod            ENDS

				END     Start

