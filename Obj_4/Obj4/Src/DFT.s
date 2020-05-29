	thumb
	area	marom, data, readonly

	export M2
	import TabCos
	import TabSin
	area	maram3, data, readwrite
	area	moncode, code, readonly

M2 proc
	;ro ->k et r1-> l'adresse du signal
	push {lr}
	ldr r2,=TabCos ; l'adresse de tab cos est charg� dans r2
	push{r0}; sauvegarde de k
	;calcul de cos*cos
	bl Somme
	SMULL r2,r3,r0,r0; cos� r�sultat de 64 bits mis dans deux registres
	pop{r0} ; on r�cup�re k
	;calcul de sin*sin
	ldr r2,=TabSin ; l'adresse de tab sin est charg� dans r2
	push{r3, r2} ; on sauvegarde r4 et r2
	bl Somme
	pop{r3,r2} ; on r�cup�re r4 et r2
	SMLAL r2,r3,r0,r0 ;Multiplication avec accumulation, les 32 bits de poids fort dont dans r3
	mov r0,r3;on r�cup�re les 32 bits de poids forts
	pop {pc}
	endp
		
Somme proc
	mov r3,#64 ;r3 contient N
	;Cr�ation du masque dans r12
	mov r12,r3; r12=N
	sub r12,#1; r12=N-1
	push{r4}
	mov r4,#0; r4 contiendra notre somme
loop
	sub r3,#1; r3=N-1
	cmp r3,#-1	; on somme de N-1 � 0
	BEQ sortie
	push{r5} ; sauvegarde de j
	;on va charg� dans r1 notre signal x(i)
	;soit au premier tour on ajoute x(0)
	;on le met dans r6
	;�tendue sur 32 bits sign� donc on rajoute le sh
	ldrsh r5,[r1,r3, lsl #1]; x(i) -> r5
	;calcul de i*k modulo N
	mul r0,r3
	and r0,r12 ; masque � R0 -> r0 modulo N
	ldrsh r0,[r2,r0, lsl #1]; -> entr�e d'indice i*k %N 
	mul r5,r5,r0; r5=r5*cos
	add r4,r5; ; r4=r4+r5 cumul de la somme de x(i)*cos
	pop{r5}; restaure j,  j=k
	mov r0,r5; Restaure k gr�ce � j
	b loop
sortie	
	mov r0,r4; mettre le r�sultat dans r0
	pop{r4}
	bx lr
	endp

	end