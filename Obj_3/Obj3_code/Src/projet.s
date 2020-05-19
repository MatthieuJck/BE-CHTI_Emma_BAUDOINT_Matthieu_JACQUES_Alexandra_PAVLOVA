; ce programme est pour l'assembleur RealView (Keil)
	thumb

	include etat.inc
	extern StructSon
	area  maram, data, readwrite

TIM3_CCR3	equ	0x4000043C	; adresse registre PWM
OFFSET	EQU	32768 ;offset qui correspond � 2^15 soit 2^16/2	
		
	area  moncode, code, readonly
;
	export timer_callback
	



timer_callback	proc

	push{lr}
	push{r4,r5}
	;initialisation
	ldr r3, =TIM3_CCR3; on met l'adresse du registre de PWM dans r3
	ldr r0, =StructSon 	; met @ de notre struct dans r0
	;gestion de la fin de notre son
	ldr r1, [r0,#E_POS]; on r�cup�re notre position actuelle
	ldr r2, [r0, #E_TAI]; on r�cup�re la taille de notre �chantillon
	add r2,#1 ;on se trouve dans r4 juste apr�s notre son
	cmp r1,r2 ; on regarde o� on en est dans notre son
	BEQ Fin_Son ; on quitte le programme si on est � la fin 
	
	;sinon
	ldr r4,[r0,#E_SON]; on met notre �chantillon de son dans r4
	ldrsh r4, [r4,r1,LSL #1]; acc�s � l'�chantillon actuel
	
	;modification de notre �chantillon pour correpondre aux crit�res
	add r4, #OFFSET;on s'assure que notre valeur soit positive
	ldrsh r5,[r0,#E_RES] ; on va ensuite chercher la r�solution
	mul r4,r5 ; on multiplie ensuite notre �chantillon avec la r�solution
	lsr r4, #16; division par 2^16
	
	;transmission
	str r4,[r3]; on stocke l'�chanitllon modifi� dans PWM
	add r1,#1;mise � jour de la position
	str r1,[r0,#E_POS];mise � jour
	B Sortie
	
Fin_Son	
	mov r2, #0;on met r2 � 0
	str r2,[r3];Mettre le PWM � 0 (afin de ne plus avoir de son � la fin du signal)
	
Sortie
	pop{pc}
	endp
	end