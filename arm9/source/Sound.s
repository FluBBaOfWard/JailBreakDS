#ifdef __arm__

#include "asm_defs.h"
#include "SN76496/SN76496.i"

	.global soundInit
	.global soundReset
	.global setMuteSoundGUI
	.global setMuteSoundGame
	.global SN_0_W
	.global VLM_R
	.global VLM_W
	.global sn76496_0
	.global ipcDataUncached

	.extern pauseEmulation


	.syntax unified
	.arm

	.section .text
	.align 2
;@----------------------------------------------------------------------------
soundInit:
	.type soundInit STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}
	ldr r0,=ipcData
	blx memUncached
	str r0,ipcDataUncached
	ldr r0,=sn76496_0
	blx memUncached
	str r0,sn76496Uncached

	ldmfd sp!,{lr}
	bx lr

;@----------------------------------------------------------------------------
soundReset:
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}
	ldr r1,sn76496Uncached
	mov r0,#1
	bl sn76496Reset				;@ Sound
	ldmfd sp!,{lr}
	bx lr

;@----------------------------------------------------------------------------
setMuteSoundGUI:
	.type   setMuteSoundGUI STT_FUNC
;@----------------------------------------------------------------------------
	ldr r1,=pauseEmulation		;@ Output silence when emulation paused.
	ldrb r0,[r1]
	strb r0,muteSoundGUI
	cmp r0,#0
	moveq r1,#FIFO_UNPAUSE<<20
	movne r1,#FIFO_APU_PAUSE<<20
	mov r0,#15
	b fifoSendValue32
	bx lr
;@----------------------------------------------------------------------------
setMuteSoundGame:			;@ For System E ?
;@----------------------------------------------------------------------------
	strb r0,muteSoundGame
	bx lr

;@----------------------------------------------------------------------------
SN_0_W:
;@----------------------------------------------------------------------------
	stmfd sp!,{r3,lr}
	ldr r1,sn76496Uncached
	bl sn76496W
	ldmfd sp!,{r3,lr}
	bx lr

;@----------------------------------------------------------------------------
VLM_W:
;@----------------------------------------------------------------------------
	cmp r12,#0x4000
	bne notVLMPins
	mov r1,r0
	ldr r0,=vlm5030Chip
	ldr r0,[r0]
	stmfd sp!,{r0,r1,r3,lr}
	mov r1,r1,lsr#1
	and r1,r1,#1
	blx VLM5030_ST

	ldmfd sp!,{r0,r1}
	mov r1,r1,lsr#2
	and r1,r1,#1
	blx VLM5030_RST
	ldmfd sp!,{r3,pc}
notVLMPins:
	cmp r12,#0x5000
	bne empty_W
	mov r1,r0
	ldr r0,=vlm5030Chip
	ldr r0,[r0]
	stmfd sp!,{r3,lr}
	blx VLM5030_WRITE8
	ldmfd sp!,{r3,pc}
;@----------------------------------------------------------------------------
VLM_R:
;@----------------------------------------------------------------------------
	cmp r12,#0x6000
	bne empty_R
vlmBusy:
	stmfd sp!,{r3,lr}
	ldr r0,=vlm5030Chip
	ldr r0,[r0]
	blx VLM5030_BSY
	cmp r0,#0
	movne r0,#1
	ldmfd sp!,{r3,pc}

;@----------------------------------------------------------------------------

ipcDataUncached:
	.long 0
muteSound:
muteSoundGUI:
	.byte 0
muteSoundGame:
	.byte 0
	.space 2
sn76496Uncached:
	.long 0

	.section .bss
sn76496_0:
	.space snSize
;@----------------------------------------------------------------------------
	.end
#endif // #ifdef __arm__
