;  ce programme est pour l'assembleur RealView (Keil)
	thumb
	
	include etat.inc
		
	extern StructSon
	extern Position
	
TIM3_CCR3	equ	0x4000043C	; adresse registre PWM	

	area  moncode, code, readonly
	export timer_callback
;
timer_callback	proc
	ldr r1,=StructSon			; chargement de la valeur de StructSon
	ldr r0, =TIM3_CCR3			; adresse du registre de PWM dans r3 
	ldr r2,[r1,#E_POS] 			; on r�cup�re la position actuelle dans r2
	ldr r3,[r1, #E_TAI]			; taille -> r3
	cmp r2,r3 					; comparaison de taille avec la position
	bhs fin_son 				; si on est � la fin, on envoit du silence et on quitte le programme 
	
; sinon 
	push {r4,r5}               
	ldr r4, [r1,#E_SON]			; notre son est maintenant dans r4
	ldrsh r4 ,[r4, r2, LSL #1] ; on acc�de � l'�chantillon trait� maintenant 
	
;mise � l'�chelle de notre �chantillon

	add r4,#0x8000            ; �chantillon toujours positif, on le ram�ne entre 0 et 2^16	
	ldr r5, [r1, #E_RES]	   ; r�cup�re la r�solution
	mul r4,r5				   ; multiplie cette r�solution avec notre �chantillon
	lsr r4,#16 			   ; division par 2^16 
	
	; transmission 
	add r2,#1 				   ; mise � jour de la position 
	str r2,[r1, #E_POS]        ; stockage de la nouvelle position du son
	str r4,[r0] 			   ; stockage l'�chantillon modifi� dans le registre PWM 
	pop {r4,r5};			   
	b	Fin
	
fin_son
	ldr r12, [r1, #E_RES]  ; on charge notre struct
	lsr r12, #1            ; division par 2^1 
	str r12,[r0]           ; on stocke r12 dans le registre de PWM 
	B Fin
		
Fin	bx	lr	
	endp
;
	end