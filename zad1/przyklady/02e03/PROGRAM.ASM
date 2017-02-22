                .MODEL  SMALL

;-------------- DEFINICJE --------------------

ZAKRES 			EQU 7

;-------------- SEGMENT DANYCH ---------------
				
				.DATA
napis           DB      "Witaj swiecie 123!",13,10,"$"

;-------------- SEGMENT KODU ---------------

				.CODE
				
				; Zaladowanie rejestru segmentowego danych
start:			mov     ax, @data
                mov     ds, ax 

				; Wyswietlenie napisu w miejscu ustawienia kursora
                mov     ah, 09h         
                mov     dx, OFFSET napis
                int     21h
				
				; Rozpoczecie zamiany malych liter na duze
				mov		cx, ZAKRES			; licznik petli loop
				mov		si, OFFSET napis	; zaladuj adres napisu do si
petla:			mov		al, [si]			; zaladuj znak z napisu do al
				cmp		al, 'a'				; porownaj al ze znakiem 'a'
				jnae	nastepny_znak		; jesli nie al >= 'a' skok do nastepny_znak
				cmp		al, 'z'				; porownaj al ze znakiem 'z'
				jnbe	nastepny_znak		; jesli nie al <= 'z' skok do nastepny_znak
				sub		al, 32				; konweruj mala liere na duza
				mov		[si], al			; przenies duza litere spowrotem do napisu
nastepny_znak:	inc		si					; przejdz do nastepnego znaku
				loop 	petla				; zmniejsz cx o 1 i skocz do poczatku petli jesli cx = 0 
											
				; Wyswietlenie napisu w miejscu ustawienia kursora
wyswietl:       mov     ah, 09h
                mov     dx, OFFSET napis
                int     21h
				
				; Zakonczenie programu
				mov     ax, 4C00h
                int     21h

;-------------- SEGMENT STOSU ---------------

				.STACK 	100h

;-------------- PUNKT STARTOWY --------------

                END     start
