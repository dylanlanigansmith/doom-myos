

#include "m_argv.h"

#include "doomgeneric.h"
#include "inc.h"
uint32_t* DG_ScreenBuffer = 0;

void M_FindResponseFile(void);
void D_DoomMain (void*);

char* args[] ={"doom",};
int main(int argc, char** argv)
{
    
    print("starting doom!\n");
  

   
	doomgeneric_Create(1, args);

    while (!keyboard_is_down(PS2_KEY_BACKSLASH))
    {
        doomgeneric_Tick();
    }
    
    return 0;
}

void DG_Init()
{
    print("init doom\n");
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



void* mmwad = 0; //uh we can guess the size lol

void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;
    
	M_FindResponseFile();

	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);
    print("created frame buffer\n");

    print("mmap WAD\n");

    mmwad = mmap_file("DOOM.iwad");

    if(!mmwad){
        print("WAD IS A WAD OF SHIT!");
    }

    printf("wad loaded at %lx\n", (uintptr_t)mmwad);
	DG_Init();
    print("calling doom main!\n");
    yield();
	D_DoomMain (mmwad);
}

void _start(){
	main(0,0); 

     exit(1);
}