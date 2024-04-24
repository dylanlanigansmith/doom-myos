

#include "m_argv.h"

#include "doomgeneric.h"
#include "inc.h"
uint32_t* DG_ScreenBuffer = 0;

void M_FindResponseFile(void);
void D_DoomMain (void);


int main(int argc, char* argv)
{
	doomgeneric_Create(argc, argv);

    while (1)
    {
        doomgeneric_Tick();
    }
    
    return 0;
}

void DG_Init()
{
}
void DG_DrawFrame()
{
}

void DG_SleepMs(uint32_t ms)
{
}

uint32_t DG_GetTicksMs()
{
return 0;
}
int DG_GetKey(int * pressed, unsigned char * key)
{
return 0;
}

void DG_SetWindowTitle(const char * title)
{
}
void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;

	M_FindResponseFile();

	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

	DG_Init();

	D_DoomMain ();
}

void _start(){
	main(0,"doom"); 
}