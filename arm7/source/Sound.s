#ifdef __arm__

#include "asm_defs.h"
#include "SN76496/SN76496.i"

	.global soundInit
	.global soundReset
	.global soundMixer
	.global soundRenderer
	.global setMuteSoundGUI
	.global setMuteSoundGame
	.global sn76496Ptr


;@----------------------------------------------------------------------------

	.syntax unified
	.arm

	.section .text
	.align 2
;@----------------------------------------------------------------------------
soundInit:
	.type soundInit STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}

//	ldr snptr,=SN76496_0
//	ldr r1,=FREQTBL
//	bl SN76496_init				;@ sound

	ldmfd sp!,{lr}
//	bx lr

;@----------------------------------------------------------------------------
soundReset:
	.type soundReset STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}
	ldr r0,=ipcData
	ldr r0,[r0]
	str r0,ipcDataPtr
//	mov r0,#0
//	str r0,lastScanline
	ldmfd sp!,{lr}
	bx lr

;@----------------------------------------------------------------------------
setMuteSoundGUI:
	.type   setMuteSoundGUI STT_FUNC
;@----------------------------------------------------------------------------
	strb r0,muteSoundGUI
	bx lr
;@----------------------------------------------------------------------------
setMuteSoundGame:			;@ For System E ?
;@----------------------------------------------------------------------------
	strb r0,muteSoundGame
	bx lr
;@----------------------------------------------------------------------------
soundRenderer:
	.type soundRenderer STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r7,lr}
	ldr r0,pcmPtr0
	str r0,renderPtr0
	ldr r0,pcmPtr1
	str r0,renderPtr1
	ldr r4,ipcDataPtr
	mov r5,#0				;@ scanline we're executing
	mov r6,#0				;@ whole cycles for SN76496
	mov r7,#0				;@ whole cycles for VLM5030
scanlineLoop:
	ldr r0,[r4,#currentScanline]
	cmp r5,r0
	bpl scanlineLoop
	sub r3,r0,r5
	mov r5,r0
	mov r1,#96				;@ Cycles per scanline for SN76496
	mul r1,r3,r1
	add r6,r6,r1
	mov r1,#116				;@ Cycles per scanline for VLM5030
	mul r1,r3,r1
	add r7,r7,r1
	mov r0,r6,lsr#4			;@ output frequency is OSC/16
	sub r6,r6,r0,lsl#4

	ldr r1,renderPtr0
	add r2,r1,r0,lsl#1
	str r2,renderPtr0
	ldr r2,sn76496Ptr
	bl sn76496Mixer

	mov r2,r7,lsr#8			;@ output frequency is OSC/128
	sub r7,r7,r2,lsl#8
	ldr r1,renderPtr1
	add r0,r1,r2,lsl#1
	str r0,renderPtr1
	ldr r0,=vlm5030Chip
	ldr r0,[r0]
	bl vlm5030_update_callback

	ldr r0,=262
	cmp r5,r0
	bmi scanlineLoop

	ldmfd sp!,{r4-r7,lr}
	bx lr
;@----------------------------------------------------------------------------
soundMixer:					;@ r0=length, r1=pointer
	.type soundMixer STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r0,r1,r4-r10,lr}

	ldr r2,muteSound
	cmp r2,#0
	bne silenceMix

	mov r5,r0,lsl#1			;@ r5 should now be 1578.
	ldr r2,pcmPtr0
	ldr r6,renderPtr0
	sub r6,r6,r2
	mov r6,r6,lsr#1			;@ bytes to samples, should be 1572 (96*262/16).
	rsb r7,r6,#0

	ldr r8,pcmPtr1
	ldr r9,renderPtr1
	sub r9,r9,r8
//	mov r9,r9,lsr#1			;@ bytes to samples, should be 196.5 (96*262/128).
	rsb r10,r5,#0

	ldr lr,filter
mixLoop:
	ldrsh r3,[r2]
	adds r7,r7,r6
	subcs r7,r7,r5
	addcs r2,r2,#2

	ldrsh r4,[r2]
	adds r7,r7,r6
	subcs r7,r7,r5
	addcs r2,r2,#2

	add r3,r3,r4

	ldrsh r4,[r8]
	adds r10,r10,r9
	subcs r10,r10,r5
	addcs r8,r8,#2

	add r3,r3,r4,lsl#2
	add lr,lr,r3
	mov lr,lr,asr#1
	mov r3,lr,asr#2

	subs r0,r0,#1
	strhpl r3,[r1],#2
	bgt mixLoop

	str lr,filter
	ldmfd sp!,{r0,r1,r4-r10,lr}
	bx lr

;@----------------------------------------------------------------------------

silenceMix:
	ldmfd sp!,{r0,r1}
	mov r12,r0
	mov r2,#0
silenceLoop:
	subs r12,r12,#1
	strhpl r2,[r1],#2
	bhi silenceLoop

	ldmfd sp!,{r4-r10,lr}
	bx lr

;@----------------------------------------------------------------------------
pcmPtr0:	.long WAVBUFFER
pcmPtr1:	.long WAVBUFFER+0x1000
renderPtr0:	.long WAVBUFFER
renderPtr1:	.long WAVBUFFER+0x1000

lfsr:
	.long 0x8000
filter:
	.long 0
ipcDataPtr:
	.long 0
sn76496Ptr:
	.long 0
lastScanline:
	.long 0
muteSound:
muteSoundGUI:
	.byte 0
muteSoundGame:
	.byte 0
	.space 2	// This is padding for muteSound

	.section .bss
	.align 2
WAVBUFFER:
	.space 0x2000
;@----------------------------------------------------------------------------
	.end
#endif // #ifdef __arm__
