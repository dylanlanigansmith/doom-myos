#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include "inc.h"

#define DOOMGENERIC_RESX doom_resx//640 1024
#define DOOMGENERIC_RESY doom_resy//400 640
extern uint32_t doom_resx, doom_resy;

extern uint32_t* DG_ScreenBuffer;

void doomgeneric_Create(int argc, char **argv);
void doomgeneric_Tick();


#define WADMAP_LEN 12408292 
extern void* mmwad;
//Implement below functions for your platform
void DG_Init();
void DG_DrawFrame();
void DG_SleepMs(uint32_t ms);
uint32_t DG_GetTicksMs();
int DG_GetKey(register int * pressed, register unsigned char * key);
void DG_SetWindowTitle(const char * title);

void DG_Exit(int err);

#endif //DOOM_GENERIC
