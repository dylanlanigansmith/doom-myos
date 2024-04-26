

#include "m_argv.h"

#include "doomgeneric.h"
#include "inc.h"
#include "doomkeys.h"
#include "../../../../libd/graphics.h"

const int sc_to_doom[128] =
{
    0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6',
    '7',    '8',    '9',    '0',    '-',    '=',    KEY_BACKSPACE, 9,
    'q',    'w',    'e',    'r',    't',    'y',    'u',    'i',
    'o',    'p',    '[',    ']',    13,		KEY_RCTRL, 'a',    's',
    'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';',
    '\'',   '`',    KEY_RSHIFT,'\\',   'z',    'x',    'c',    'v',
    'b',    'n',    'm',    ',',    '.',    '/',    KEY_RSHIFT,KEYP_MULTIPLY,
    KEY_RALT,  ' ',  KEY_CAPSLOCK,KEY_F1,  KEY_F2,   KEY_F3,   KEY_F4,   KEY_F5,
    KEY_F6,   KEY_F7,   KEY_F8,   KEY_F9,   KEY_F10,  /*KEY_NUMLOCK?*/KEY_PAUSE,KEY_SCRLCK,KEY_HOME,
    KEY_UPARROW,KEY_PGUP,KEY_MINUS,KEY_LEFTARROW,KEYP_5,KEY_RIGHTARROW,KEYP_PLUS,KEY_END,
    KEY_DOWNARROW,KEY_PGDN,KEY_INS,KEY_DEL,0,   0,      0,      KEY_F11,
    KEY_F12,  0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      KEY_PRTSCR, 0
};
uint32_t* DG_ScreenBuffer = 0;

void M_FindResponseFile(void);
void D_DoomMain (void*);

char* args[] ={"doom",};

void* fb = 0; //0xfc000000

//extern gfx_info_t* gfxinfo;
#define BYPP 4
#define FB_W fbw//(uint32_t)gfxinfo->w
#define FB_H fbh//(uint32_t)gfxinfo->h
#define PITCH fbp//(uint32_t)gfxinfo->p
//DOOMGENERIC_RESX * DOOMGENERIC_RESY

void try_input();
uint32_t doom_resx = 600;
uint32_t  doom_resy = 400;

int auto_res = 1; //2 == fullscreen 0 = low


void DG_DrawFrame()
{
    static int y_off = 0;
    static int x_off = 0;
    static uint32_t fbw = 0;
    static uint32_t fbh = 0;
    static uint32_t fbp = 0;

    static size_t doom_pitch = 4096;
    if(!fb){ print("no frame buffer set?"); 
        fb = set_gfx_mode();

        gfx_info_t* gi = gfx_get_info();
        fbw =  gi->w;
        fbh = gi->h;
        fbp = gi->p;
        
        doom_pitch = DOOMGENERIC_RESX * BYPP;

        y_off = (FB_H - DOOMGENERIC_RESY) / 2;
        x_off = (fbp - doom_pitch) / 2;
        if(x_off % 64) x_off = x_off - (x_off % 64);
        if(x_off < 0) x_off = 0;

        if(!fb)
            exit(1); 
    }

     
    // x_res: 640, y_res: 400, x_virtual: 640, y_virtual: 400, bpp: 32
    register int y;
    for(y = 0; y < (DOOMGENERIC_RESY + 0); ++y){
        
        register uintptr_t addr = (uintptr_t)fb + ( (y + y_off) * PITCH) + x_off;
        register uintptr_t offset = (uintptr_t)DG_ScreenBuffer + (y * doom_pitch);
        memcpy_fastnfurious(addr, offset, doom_pitch);
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
        case PS2_KEY_Y:
        case PS2_KEY_N:
        case PS2_KEY_1:
        case PS2_KEY_2:
        case PS2_KEY_3:
        case PS2_KEY_4:
        case PS2_KEY_SPACE:
        case PS2_KEY_LEFT_SHIFT:
        case PS2_KEY_LEFT_ALT:
        case PS2_KEY_ESCAPE:
        case PS2_KEY_ENTER:
        case PS2_KEY_BACKSPACE:
        case PS2_KEY_F10:
            inqueue[kw++] = sc_raw;
            kw %= INQUEUE_BUF; break;
        default:
            if(sc < 128){
                inqueue[kw++] = sc_raw;
                kw %= INQUEUE_BUF;
            }
               
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
      /*  case PS2_KEY_Y:
            dk = 'y'; break;
        case PS2_KEY_N:
            dk = 'n'; break;
        case PS2_KEY_1:
            dk = '1'; break;
        case PS2_KEY_2:
            dk = '2'; break;
        case PS2_KEY_3:
            dk = '3'; break;
        case PS2_KEY_4:
            dk = '4'; break;
        case PS2_KEY_F10:
            dk = KEY_F10; break;*/
        default:
            if(sc < 128){
                dk = sc_to_doom[sc];
               // printf("got unknown key %i dk %i\n", sc, dk);
                
            }
            else dk = 0x0;
    }

    if(dk == 0x0) return 0;
    *pressed = (sc_raw & PS2_MAKEORBREAK) ? 0 : 1; //explicit af
    *key = dk; 
  //  printf("got a key! %i %lx / %lx", dk, (uintptr_t)pressed, (uintptr_t)key);

    return 1;
}



int main(int argc, char** argv)
{
    doom_resx = 600;
    doom_resy = 400;
    keyboard_togglemode();
    print("starting doomos!");
    gfx_info_t* gi = gfx_get_info();

    if(!gi) print("we got major gfx issues");
    uint32_t h = (uint32_t)(gi->h);
    if(h > doom_resy && auto_res){
        //we can go up a res
        if(h >= 768){
            doom_resx = 1024;
            doom_resy = 640;
        } if(h >= 1050){
            doom_resx = 1280;
            doom_resy = 960;
        } if(h >= 1200){
            doom_resx = 1050;
            doom_resy = 1400;
        }
        if(h >= 1440){
            doom_resx = 1600;
            doom_resy = 1200;
        }
        if(auto_res == 2 && h >= 1440){
            doom_resx = 1920;
            doom_resy = 1440;
        }
    }
    //game auto sets scaling
    //would be nice to have mode with it at low res in corner and stdout still running


    printf("using gfx mode (%ix%ix%i) \n", doom_resx, doom_resy, gi->bpp);



	doomgeneric_Create(1, args);

    while (1)
    {
        doomgeneric_Tick();
    }
    print("quit doomos!\n");

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
bool gfx_up = False;
void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;
    last_processed_input = 0;
    gfx_up = 0;
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
    gfx_up = True;
    fb = 0; //set_gfx_mode();
}

void DG_Exit(int err)
{
    if(gfx_up)
        set_gfx_mode();
    keyboard_togglemode();
    exit(err);
}
