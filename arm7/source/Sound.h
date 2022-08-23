#ifndef SOUND_HEADER
#define SOUND_HEADER

extern void soundInit(void);
extern void soundReset(void);
extern void setMuteSoundGUI(bool);
extern void soundRenderer(void);
extern void soundMixer(int length, s16* buffer);

extern void *sn76496Ptr;

#endif // SOUND_HEADER
