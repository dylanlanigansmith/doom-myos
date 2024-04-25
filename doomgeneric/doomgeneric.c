

#include "m_argv.h"

#include "doomgeneric.h"
#include "inc.h"
#include "doomkeys.h"

uint32_t* DG_ScreenBuffer = 0;

void M_FindResponseFile(void);
void D_DoomMain (void*);

char* args[] ={"doom",};

void* fb = 0; //0xfc000000
#define FB_W 1024
#define FB_H 768
#define PITCH 4096
//DOOMGENERIC_RESX * DOOMGENERIC_RESY

void try_input();



const int y_off = (FB_H - DOOMGENERIC_RESY) / 2;
const size_t doom_pitch = DOOMGENERIC_RESX * 4ull;
void DG_DrawFrame()
{
    if(!fb){ print("no frame buffer set?"); return; }
    // x_res: 640, y_res: 400, x_virtual: 640, y_virtual: 400, bpp: 32
    register int y;
    for(y = 0; y < (DOOMGENERIC_RESY + 0); ++y){
        
        register uintptr_t addr = (uintptr_t)fb + ( (y + y_off) * PITCH);
        register uintptr_t offset = (uintptr_t)DG_ScreenBuffer + (y * doom_pitch);
        memcpy(addr, offset, doom_pitch);
    }

    try_input();

}

#define INQUEUE_BUF 16
uint8_t inqueue[INQUEUE_BUF] = {0};
int kw = 0;
int kr = 0;
uint64_t last_processed_input = 0;


void try_input()
{
    uint64_t keytick =  keyboard_lastinput();
    if(keytick <= last_processed_input) return 0;

    last_processed_input = keytick;

    uint8_t sc_raw = keyboard_get_lastevent(); 
    
    if(sc_raw == 0xff || !sc_raw) return 0;
    uint8_t sc = sc_raw & 0x7f;
    switch(sc){
        case PS2_KEY_W: // up
        case PS2_KEY_S: //down
        case PS2_KEY_A: //left
        case PS2_KEY_D: //right
        case PS2_KEY_SPACE:
        case PS2_KEY_LEFT_SHIFT:
        case PS2_KEY_LEFT_ALT:
        case PS2_KEY_ESCAPE:
        case PS2_KEY_ENTER:
        case PS2_KEY_BACKSPACE:
            inqueue[kw++] = sc_raw;
            kw %= INQUEUE_BUF;
        default:
            return;
    }

}
int DG_GetKey(register int * pressed, register unsigned char * key){
    if(!pressed || !key) return 0;

     

     if(kw == kr) return 0; //empty q
        
    uint8_t sc_raw = inqueue[kr++];
    kr %= INQUEUE_BUF;
    
    
    if(sc_raw == 0xff || !sc_raw) return 0;
    uint8_t sc = sc_raw & 0x7f;
    unsigned char dk =0x0;
    switch(sc){
        case PS2_KEY_W: // up
            dk = KEY_UPARROW; break;
        case PS2_KEY_S: //down
            dk = KEY_DOWNARROW; break;
        case PS2_KEY_A: //left
            dk = KEY_LEFTARROW; break;
        case PS2_KEY_D: //right
            dk = KEY_RIGHTARROW; break;
        case PS2_KEY_SPACE:
            dk = KEY_USE; break;
        case PS2_KEY_LEFT_SHIFT:
            dk = KEY_FIRE; break;
        case PS2_KEY_LEFT_ALT:
            dk = KEY_LALT; break; 
        case PS2_KEY_ESCAPE:
            dk = KEY_ESCAPE; break;
        case PS2_KEY_ENTER:
            dk = KEY_ENTER; break;
        case PS2_KEY_BACKSPACE:
            dk = KEY_BACKSPACE; break;
        default:
            dk = 0x0;
    }

    if(dk == 0x0) return 0;
    *pressed = (sc_raw & PS2_MAKEORBREAK) ? 0 : 1; //explicit af
    *key = dk; 
  //  printf("got a key! %i %lx / %lx", dk, (uintptr_t)pressed, (uintptr_t)key);

    return 1;
}




int main(int argc, char** argv)
{
    
    print("starting doomos!\n");
	doomgeneric_Create(1, args);

    while (1)
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
        while(sys_gettick() + msl < t){ //either we get scheduled during this or we dont but its a waste sleeping for 1ms with my 1000hz/10ms a slice scheduler
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


//this is externed and used directly by our custom doom wad reading impl
//lazy but it works
void* mmwad = 0; //also i hardcoded the size lol, but it doesnt seem to matter bc i tried a few diff. WADs without remembering that i did and NBD.....
                                                // my custom WAD class + how eager this game is when the WAD is mmaped probably dealt with it

void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;
    last_processed_input = 0;
	M_FindResponseFile();

	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

    fb = 0;

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
    printf("DoomSetWindow: %s \n", title);
    //now we know we should be rendering!!!
    sleep_ms(1000);
    yield();
    fb = set_gfx_mode();
}

void _start(){
    keyboard_togglemode();
	main(0,0); 
    keyboard_togglemode();
    set_gfx_mode();
     exit(1);
}