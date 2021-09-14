#include <nds.h>

#include "JailBreak.h"
#include "Gfx.h"
#include "Sound.h"
#include "SN76496/SN76496.h"
#include "K005849/K005849.h"
#include "ARM6809/ARM6809.h"


int packState(void *statePtr) {
	int size = 0;
	size += sn76496SaveState(statePtr+size, &sn76496_0);
	size += k005849SaveState(statePtr+size, &k005849_0);
	size += m6809SaveState(statePtr+size, &m6809OpTable);
	return size;
}

void unpackState(const void *statePtr) {
	int size = 0;
	size += sn76496LoadState(&sn76496_0, statePtr+size);
	size += k005849LoadState(&k005849_0, statePtr+size);
	m6809LoadState(&m6809OpTable, statePtr+size);
}

int getStateSize() {
	int size = 0;
	size += sn76496GetStateSize();
	size += k005849GetStateSize();
	size += m6809GetStateSize();
	return size;
}

static const ArcadeRom jailbrekRoms[13] = {
	// ROM_REGION( 0x10000, "maincpu", 0 )
	{"507p03.11d", 0x4000, 0xa0b88dfd},
	{"507p02.9d",  0x4000, 0x444b7d8e},
	// ROM_REGION( 0x20000, "gfx1", 0 )
	{"507j04.3e",  0x4000, 0x0d269524},
	{"507j05.4e",  0x4000, 0x27d4f6f4},
	{"507j06.5e",  0x4000, 0x717485cb},
	{"507j07.3f",  0x4000, 0xe933086f},
	{"507l08.4f",  0x4000, 0xe3b7a226},
	{"507j09.5f",  0x4000, 0x504f0912},
	// ROM_REGION( 0x0240, "proms", 0 )
	{"507j10.1f",  0x0020, 0xf1909605},
	{"507j11.2f",  0x0020, 0xf70bb122},
	{"507j13.7f",  0x0100, 0xd4fe5c97},
	{"507j12.6f",  0x0100, 0x0266c7db},
	// ROM_REGION( 0x4000, "vlm", 0 ) /* speech rom */
	{"507l01.8c",  0x4000, 0x0c8a3605},
};

static const ArcadeRom jailbrekbRoms[9] = {
	// ROM_REGION( 0x10000, "maincpu", 0 )
	{"1.k6",    0x8000, 0xdf0e8fc7},
	// ROM_REGION( 0x20000, "gfx1", 0 )
	{"5.f6",    0x8000, 0x081d2eea},
	{"4.g6",    0x8000, 0xe34b93b8},
	{"3.h6",    0x8000, 0xbf67a8ff},
	// ROM_REGION( 0x0240, "proms", 0 )
	{"prom.j2", 0x0020, 0xf1909605},
	{"prom.i2", 0x0020, 0xf70bb122},
	{"prom.d6", 0x0100, 0xd4fe5c97},
	{"prom.e6", 0x0100, 0x0266c7db},
	// ROM_REGION( 0x2000, "vlm", 0 ) /* speech rom */
	{"2.i6",    0x2000, 0xd91d15e3},
	// ROM_REGION( 0x0004, "plds", 0 )
	//{"k4.bin",  0x0001, NO_DUMP ) /* PAL16L8 */
	//{"a7.bin",  0x0001, NO_DUMP ) /* PAL16R4 */
	//{"g9.bin",  0x0001, NO_DUMP ) /* PAL16R6 */
	//{"k8.bin",  0x0001, NO_DUMP ) /* PAL16L8 */
};

static const ArcadeRom manhatanRoms[13] = {
	// ROM_REGION( 0x10000, "maincpu", 0 )
	{"507n03.11d", 0x4000, 0xe5039f7e},
	{"507n02.9d",  0x4000, 0x143cc62c},
	// ROM_REGION( 0x20000, "gfx1", 0 )
	{"507j04.3e",  0x4000, 0x0d269524},
	{"507j05.4e",  0x4000, 0x27d4f6f4},
	{"507j06.5e",  0x4000, 0x717485cb},
	{"507j07.3f",  0x4000, 0xe933086f},
	{"507j08.4f",  0x4000, 0x175e1b49},
	{"507j09.5f",  0x4000, 0x504f0912},
	// ROM_REGION( 0x0240, "proms", 0 )
	{"507j10.1f",  0x0020, 0xf1909605},
	{"507j11.2f",  0x0020, 0xf70bb122},
	{"507j13.7f",  0x0100, 0xd4fe5c97},
	{"507j12.6f",  0x0100, 0x0266c7db},
	// ROM_REGION( 0x4000, "vlm", 0 ) /* speech rom */
	{"507p01.8c",  0x4000, 0x973fa351},
};

const ArcadeGame games[GAME_COUNT] = {
	{"jailbrek",  "Jail Break", 13, jailbrekRoms},
	{"jailbrekb", "Jail Break (bootleg)", 8, jailbrekbRoms},
	{"manhatan",  "Manhattan 24 Bunsyo (Japan)", 13, manhatanRoms},
};
