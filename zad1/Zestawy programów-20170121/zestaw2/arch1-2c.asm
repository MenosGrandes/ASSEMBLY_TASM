                .MODEL  SMALL

;-------------- SEGMENT DANYCH ---------------
				
Dane            SEGMENT
napis           DB      "Witaj swiecie 123!",13,10,"$"
Dane            ENDS

;-------------- SEGMENT KODU ---------------

Kod             SEGMENT
                ASSUME  CS:Kod, DS:Dane, SS:Stosik

				; Zaladowanie rejestru segmentowego danych
start:			mov     ax, SEG Dane
                mov     ds, ax 

				; Wyswietlenie napisu w miejscu ustawienia kursora
                mov     ah, 09h         
                mov     dx, OFFSET napis
                int     21h
				
				; Rozpoczecie zamiany malych liter na duze
				mov		si, OFFSET napis	; zaladuj adres napisu do si
petla:			mov		al, [si]			; zaladuj znak z napisu do al
				cmp		al, '$'				; porownaj al ze znakiem '$'
				je		wyswietl			; czy rowne, jesli tak skocz do wyswietl, jesli nie idz dalej
				cmp		al, 'a'				; porownaj al ze znakiem 'a'
				jnae	nastepny_znak		; jesli nie al >= 'a' skok do nastepny_znak
				cmp		al, 'z'				; porownaj al ze znakiem 'z'
				jnbe	nastepny_znak		; jesli nie al <= 'z' skok do nastepny_znak
				sub		al, 32				; konweruj mala liere na duza
				mov		[si], al			; przenies duza litere spowrotem do napisu
nastepny_znak:	inc		si					; przejdz do nastepnego znaku
				jmp		petla				; skocz do poczatku petli

				; Wyswietlenie napisu w miejscu ustawienia kursora
wyswietl:       mov     ah, 09h
                mov     dx, OFFSET napis
                int     21h
				
				; Zakonczenie programu
				mov     ax, 4C00h
                int     21h
Kod             ENDS

;-------------- SEGMENT STOSU ---------------

Stosik          SEGMENT STACK
                DB      100h DUP (?)
Stosik          ENDS

;-------------- PUNKT STARTOWY --------------

                END     start
