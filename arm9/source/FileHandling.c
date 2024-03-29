#include <nds.h>
#include <stdio.h>

#include "FileHandling.h"
#include "Shared/EmuMenu.h"
#include "Shared/EmuSettings.h"
#include "Shared/FileHelper.h"
#include "Shared/EmubaseAC.h"
#include "Main.h"
#include "Gui.h"
#include "Cart.h"
#include "Gfx.h"
#include "io.h"

static const char *const folderName = "acds";
static const char *const settingName = "settings.cfg";

ConfigData cfg;
static int selectedGame = 0;

const ArcadeGame allGames[GAME_COUNT] = {
	AC_GAME("jailbrek",  "Jail Break", jailbrekRoms)
	AC_GAME("jailbrekb", "Jail Break (bootleg)", jailbrekbRoms)
	AC_GAME("manhatan",  "Manhattan 24 Bunsyo (Japan)", manhatanRoms)
	AC_GAME("finalizr", "Finalizer - Super Transformation (set 1)", finalizrRoms)
	AC_GAME("finalizra", "Finalizer - Super Transformation (set 2)", finalizraRoms)
	AC_GAME("finalizrb", "Finalizer - Super Transformation (bootleg)", finalizrbRoms)
};

//---------------------------------------------------------------------------------
int loadSettings() {
	FILE *file;

	if (findFolder(folderName)) {
		return 1;
	}
	if ( (file = fopen(settingName, "r")) ) {
		fread(&cfg, 1, sizeof(ConfigData), file);
		fclose(file);
		if (!strstr(cfg.magic,"cfg")) {
			infoOutput("Error in settings file.");
			return 1;
		}
	}
	else {
		infoOutput("Couldn't open file:");
		infoOutput(settingName);
		return 1;
	}

	gScaling     = cfg.scaling & 1;
	gFlicker     = cfg.flicker & 1;
	gGammaValue  = cfg.gammaValue;
	emuSettings  = cfg.emuSettings & ~EMUSPEED_MASK; // Clear speed setting.
	sleepTime    = cfg.sleepTime;
	joyCfg       = (joyCfg & ~0x400)|((cfg.controller & 1)<<10);
	strlcpy(currentDir, cfg.currentPath, sizeof(currentDir));
	gDipSwitch0  = cfg.dipSwitchJB0;
	gDipSwitch1  = cfg.dipSwitchJB1;
	gDipSwitch2  = cfg.dipSwitchJB2;
	gDipSwitch3  = cfg.dipSwitchJB3;

	infoOutput("Settings loaded.");
	return 0;
}
void saveSettings() {
	FILE *file;

	strcpy(cfg.magic,"cfg");
	cfg.scaling     = gScaling & 1;
	cfg.flicker     = gFlicker & 1;
	cfg.gammaValue  = gGammaValue;
	cfg.emuSettings = emuSettings & ~EMUSPEED_MASK; // Clear speed setting.
	cfg.sleepTime   = sleepTime;
	cfg.controller  = (joyCfg>>10) & 1;
	strlcpy(cfg.currentPath, currentDir, sizeof(cfg.currentPath));
	cfg.dipSwitchJB0 = gDipSwitch0;
	cfg.dipSwitchJB1 = gDipSwitch1;
	cfg.dipSwitchJB2 = gDipSwitch2;
	cfg.dipSwitchJB3 = gDipSwitch3;

	if (findFolder(folderName)) {
		return;
	}
	if ( (file = fopen(settingName, "w")) ) {
		fwrite(&cfg, 1, sizeof(ConfigData), file);
		fclose(file);
		infoOutput("Settings saved.");
	}
	else {
		infoOutput("Couldn't open file:");
		infoOutput(settingName);
	}
}

int loadNVRAM() {
	return 0;
}

void saveNVRAM() {
}

void loadState() {
	loadDeviceState(folderName);
}

void saveState() {
	saveDeviceState(folderName);
}

//---------------------------------------------------------------------------------
static bool loadRoms(int gameNr, bool doLoad) {
	return loadACRoms(ROM_Space, allGames, gameNr, ARRSIZE(allGames), doLoad);
}

bool loadGame(int gameNr) {
	cls(0);
	drawText(" Checking roms", 10, 0);
	if (loadRoms(gameNr, false)) {
		return true;
	}
	drawText(" Loading roms", 10, 0);
	loadRoms(gameNr, true);
	selectedGame = gameNr;
	strlcpy(currentFilename, allGames[selectedGame].gameName, sizeof(currentFilename));
	setEmuSpeed(0);
	loadCart(gameNr,0);
	if (emuSettings & AUTOLOAD_STATE) {
		loadState();
	}
	else if (emuSettings & AUTOLOAD_NVRAM) {
		loadNVRAM();
	}
	return false;
}
