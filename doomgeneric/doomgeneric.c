

#include "m_argv.h"

#include "doomgeneric.h"
#include "inc.h"
uint32_t* DG_ScreenBuffer = 0;

void M_FindResponseFile(void);
void D_DoomMain (void*);

char* args[] ={"doom",};

void* fb = 0; //0xfc000000
#define FB_W 1024
#define FB_W 768
#define PITCH 4096

void DG_DrawFrame()
{
    // x_res: 640, y_res: 400, x_virtual: 640, y_virtual: 400, bpp: 32
    for(int y = 0; y < 400; ++y){
        
        uintptr_t addr = (uintptr_t)fb + (y * 4096);
        uintptr_t offset = (uintptr_t)DG_ScreenBuffer + (y * 640 * 4);
        memcpy(addr, offset, 640 * 4);
    }



}


int main(int argc, char** argv)
{
    
    print("starting doomos!\n");
	doomgeneric_Create(1, args);

    while (!keyboard_is_down(PS2_KEY_BACKSLASH))
    {
        doomgeneric_Tick();
    }
    print("quit doomos!\n");
    return 0;
}


uint64_t start_tick = 0;
void DG_Init()
{
    start_tick = sys_gettick();
    printf("init doom @ %li\n", start_tick);
}


void DG_SleepMs(uint32_t ms)
{
    uint64_t msl = (uint64_t)ms;
    if(msl < 15){
        register uint64_t t =  sys_gettick();
        while(sys_gettick() + msl < t){
            __asm__ volatile ("nop; nop; nop; nop;");
        }
    } else{
        
        sleep_ms(msl);
    }


}

uint32_t DG_GetTicksMs()
{
    return sys_gettick() - start_tick; //"1000hz" dont hurt at all ;))))
}



int DG_GetKey(int * pressed, unsigned char * key)
{
    return 0;
}




//this is externed and used directly by our custom doom wad reading impl
//lazy but it works
void* mmwad = 0; //also i hardcoded the size lol

void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;
    
	M_FindResponseFile();

	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

    fb = set_gfx_mode();

    printf("created frame buffer %lx\n", fb);

    print("mmap WAD\n");

    mmwad = mmap_file("DOOM.WAD");

    if(!mmwad){
        print("WAD IS A WAD OF SHIT!");
    }

    printf("wad loaded at %lx\n", (uintptr_t)mmwad);
	DG_Init();
    print("calling doom main!\n");
   // yield();
	D_DoomMain (mmwad);
}

void DG_SetWindowTitle(const char * title)
{
    //printf("DoomSetWindow: %s \n", title);
    //now we know we should be rendering!!!
}

void _start(){
	main(0,0); 

     exit(1);
}