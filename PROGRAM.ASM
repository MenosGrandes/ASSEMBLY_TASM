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

                MODEL  TINY

Kod             SEGMENT
	
                ORG      100h
                ASSUME  CS:Kod, DS:Kod, SS:Kod

Start:
        a               DB      20
		b               DB      10
		c               DB      5
		d               DB      3
                
         jmp      Poczotek


Poczotek:
			xor ax,ax
			mov al,a
			mul b ;al = a*b
			mov bl,al
			mov al,c
			mul d ;al = c*d
			add al,bl ; bl=a*b + c*d
			mov bl,a
			sub bl,d ; d= d-a
			div bl ;(a*b+c*d)/(a-d)
			
			


                mov     ax, 4C25h
                int     21h

Kod            ENDS

END Start

