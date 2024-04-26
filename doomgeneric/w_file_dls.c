#include "inc.h"
//#include <stdio.h>
#include "doomgeneric.h"
#include "m_misc.h"
#include "w_file.h"
#include "z_zone.h"

typedef struct
{
    wad_file_t wad;
    void* ptr;
} dls_wad_file_t;

extern wad_file_class_t dls_wad_file;

static wad_file_t *W_dls_OpenFile(char *path)
{
    dls_wad_file_t *result;
   

    

    // Create a new stdc_wad_file_t to hold the file handle.

    result = Z_Malloc(sizeof(dls_wad_file_t), PU_STATIC, 0);
    result->wad.file_class = &dls_wad_file;
    result->wad.mapped = mmwad;
    result->wad.length = WADMAP_LEN; //dunno
    result->ptr = mmwad;
    printf("opened our wad 'file' %lx",(uintptr_t)result->ptr);
    return &result->wad;
}

static void W_dls_CloseFile(wad_file_t *wad)
{
    dls_wad_file_t *dls_wad;

    dls_wad = (dls_wad_file_t *) wad;

    
    Z_Free(dls_wad);
}

// Read data from the specified position in the file into the 
// provided buffer.  Returns the number of bytes read.

size_t W_dls_Read(wad_file_t *wad, unsigned int offset,
                   void *buffer, size_t buffer_len)
{
    dls_wad_file_t *dls_wad;
    size_t result;

     dls_wad = (dls_wad_file_t *) wad;

    // Jump to the specified position in the file.

    
    register uintptr_t addr = (uintptr_t)(dls_wad->ptr) + (uintptr_t)(offset) ;
    size_t end = (uintptr_t)(dls_wad->ptr) + (uintptr_t)(dls_wad->wad.length) ;
    register size_t red = (addr + buffer_len) > end ? (end - addr) : buffer_len;
    printf("\nreading WAD %i (size %li) addr %lx, end %lx\n", offset, red, addr, end);

        register void* buf =  buffer;
    memcpy(buf, (void*)addr, red);
    // Read into the buffer.

    
    return red;
}


wad_file_class_t dls_wad_file = 
{
    W_dls_OpenFile,
    W_dls_CloseFile,
    W_dls_Read,
};


