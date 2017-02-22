;=============================================================================;
;                                                                             ;
; Plik           : arch1-2c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program znajdujacy najmniejsza liczbe w tablicy            ;
;                                                                             ;
;=============================================================================;

                .MODEL  SMALL
                

Dane            SEGMENT

Tablica         DB      1Ah, 02h, 00h, 10h, 12h, 33h, 15h, 09h, 11h, 08h, 0Ah, 00h

Najwieksza      DB      ?

Dane            ENDS

Kod             SEGMENT


                ASSUME  CS:Kod, DS:Dane

Start:
		MOV	AX, seg dane		; zaladowanie poczatku segmentu danych do AX
		MOV	DS, AX			; zaladowanie AX do DS
		MOV	AX, 00			; wyzerowanie AX
		MOV 	CX, 11; 11 wywola	; CX (licznik petli) = 11
		MOV	BL, [Tablica]		; zaladowanie pierwszego elementu tablicy do BL
		
		JMP	Petla			; skok do etykiety "Petla"

Petla:	
       		MOV	SI, CX			; zaladowanie do SI wartosci licznika
		MOV	AL, [Tablica + SI]	; zaladowanie do AL elementu tablicy o indeksie rownym licznikowi-1
		CMP     AL, BL			; zaladowanie BL do AL
		JNBE	Zamien			; wykonaj jesli AL > BL
       		JBE	PoPetli			; jezeli AL <= Bl, to skok do etykiety "PoPetli"
       	
Zamien:

		MOV	BL, AL			; zaladowanie AL do BL

PoPetli:		       	
       	
       		LOOP	Petla        		; skok do etykiety "Petla"
Skok:
              
                MOV     byte PTR Najwieksza, BL	; zaladowanie wartosci z BL do zmiennej "Najwieksza"
				
		MOV	ax, 00			; wyzerowanie ax
                MOV     ah, 4Ch			; instrukcje przerwania
                INT     21h

Kod		ENDS
END START


