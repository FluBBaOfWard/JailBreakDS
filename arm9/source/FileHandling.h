#ifndef FILEHANDLING_HEADER
#define FILEHANDLING_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "Finalizer.h"
#include "JailBreak.h"

#define FILEEXTENSIONS ".zip"

#define GAME_COUNT (6)

extern const ArcadeGame allGames[GAME_COUNT];

int loadSettings(void);
void saveSettings(void);
int loadNVRAM(void);
void saveNVRAM(void);
void loadState(void);
void saveState(void);
bool loadGame(int gameNr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // FILEHANDLING_HEADER
