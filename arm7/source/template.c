/*---------------------------------------------------------------------------------
---------------------------------------------------------------------------------*/
#include <nds.h>
//#include <dswifi7.h>
#include "c_defs.h"
#include "SoundHandler.h"
#include "VLM5030/vlm5030.h"
#include "Sound.h"

struct vlm5030_info *vlm5030Chip;
struct ipcStruct *ipcData;
//---------------------------------------------------------------------------------
void VBlankHandler(void) {
//---------------------------------------------------------------------------------
	soundInterrupt();
//	Wifi_Update();
}


//---------------------------------------------------------------------------------
void VCountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

volatile bool exitflag = false;

//---------------------------------------------------------------------------------
void powerButtonCB() {
//---------------------------------------------------------------------------------
	exitflag = true;
}

//---------------------------------------------------------------------------------
int main() {
//---------------------------------------------------------------------------------
	readUserSettings();

	irqInit();
	// Start the RTC tracking IRQ
	initClockIRQ();
	fifoInit();

	SetYtrigger(80);

//	installWifiFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VCountHandler);
	irqSet(IRQ_VBLANK, VBlankHandler);

//	irqEnable( IRQ_TIMER1 | IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);
	irqEnable( IRQ_VBLANK | IRQ_VCOUNT);

	setPowerButtonCB(powerButtonCB);   

	while (!fifoCheckAddress(FIFO_USER_06)) {	// Wait for the value of ipc_region
		swiWaitForVBlank();
	}
	ipcData = fifoGetAddress(FIFO_USER_08);
	sn76496Ptr = fifoGetAddress(FIFO_USER_07);
	vlm5030Chip = fifoGetAddress(FIFO_USER_06);

	soundStartup();
	// Keep the ARM7 mostly idle
	while (!exitflag) {
		swiWaitForVBlank();
		soundRenderer();
		if ( 0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
			exitflag = true;
		}
	}
	return 0;
}
