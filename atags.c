#include "atags.h"
#include "uart.h"
#include "strings.h"

typedef void (*write_t)(uint8_t const* p, uint32_t count);

#define ATAG_NONE       0x00000000
#define ATAG_CORE       0x54410001
#define ATAG_MEM        0x54410002
#define ATAG_VIDEOTEXT  0x54410003
#define ATAG_RAMDISK    0x54410004
#define ATAG_INITRD2    0x54420005
#define ATAG_SERIAL     0x54410006
#define ATAG_REVISION   0x54410007
#define ATAG_VIDEOLFB   0x54410008
#define ATAG_CMDLINE    0x54410009

typedef struct
{
    uint32_t size;
    uint32_t tag;
} atag_header;

static void write_line(char const* str, write_t write)
{
    write((uint8_t const*)str, strlen(str));
    write((uint8_t const*)"\r\n", 2);
}

struct
{
    uint32_t flags;
    uint32_t pagesize;
    uint32_t rootdev;
} core;
struct
{
    uint32_t size;
    uint32_t start;
} mem;
struct
{
    uint8_t x;
    uint8_t y;
    uint16_t page;
    uint8_t mode;
    uint8_t cols;
    uint16_t ega;
    uint8_t lines;
    uint8_t isvga;
    uint16_t points;
} video;
struct
{
    uint32_t flags;
    uint32_t size;
    uint32_t start;
} ramdisk;
struct
{
    uint32_t start;
    uint32_t size;
} initrd2;
struct
{
    uint32_t low;
    uint32_t high;
} serial;
struct
{
    uint32_t rev;
} rev;
struct
{
    uint16_t lfb_width;
    uint16_t lfb_height;
    uint16_t lfb_depth;
    uint16_t lfb_linelength;
    uint32_t lfb_base;
    uint32_t lfb_size;
    uint8_t red_size;
    uint8_t red_pos;
    uint8_t green_pos;
    uint8_t blue_size;
    uint8_t blue_pos;
    uint8_t rsvd_size;
    uint8_t rsvd_pos;
} video_lfb;

typedef struct
{
    atag_header hdr;
    char commandline[1];
} atag_cmdline;

void print_atags(uart_funcs_t* funcs, uint32_t atags)
{
    atag_header* atag = (atag_header*)0x100;
    do
    {
        switch(atag->tag)
        {
            case ATAG_CORE:
                write_line("CORE", funcs->write);
                break;
            case ATAG_MEM:
                write_line("MEM", funcs->write);
                break;
            case ATAG_CMDLINE:
                write_line("CMDLINE", funcs->write);
                write_line(((atag_cmdline*)atag)->commandline, funcs->write);
                break;
        }
        atag = (atag_header*)((uint32_t)atag + (atag->size << 2));
    } while(atag->tag);
}

