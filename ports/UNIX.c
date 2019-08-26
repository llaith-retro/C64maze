#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include"vic_font.h"
#include"sid_tune.h"
#include"UNIX.h"
#include <c64maze.h>
#include <time.h>
/* local definitions */
#define VIC_II_START 53248U
#define VIC_II_Y_SCROLL 53265U
#define     BMM 32  // Monochromatic high resolution mode
#define VIC_II_SCREEN_CHAR 53272U
#define SCREEN_BORDER 53280U

#define BASE 0xA000U
#define COLOR_MEM 0x8C00U


/* local vars */

SDL_Rect drect;
SDL_Renderer *ren;
SDL_Event event;
SDL_Window *win;

static unsigned char pix_pos[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
unsigned int cnt;
#define STACK_SIZE 256
unsigned char stackSize[STACK_SIZE];

unsigned char *list1;
unsigned char *ptr1;
unsigned char wsh1;

unsigned char *list2;
unsigned char *ptr2;
unsigned char wsh2;

unsigned char *list3;
unsigned char *ptr3;
unsigned char wsh3;


struct font{
    unsigned char *pDesc;   /* Pointer to the character raster array */
    unsigned int pos[256]; /* Position of the symbol */
    unsigned char incX;     /* Increment in X (-1 means a proportional font) */
    unsigned char incY;     /* Increment in Y */
    unsigned char magnification;  /* Magnification */
};

struct font f;

/* local funcs defs */
static void port_process_voice(unsigned char **ptr, unsigned char *sid_pointer,
    unsigned char *wsh);
/* funcs */
void port_pset(unsigned int x, unsigned int y)
{
    SDL_RenderDrawPoint(ren, x, y);
}

void port_clearHGRpage(void)
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0x00, 0x66, 0x00, 0xFF);
}

void port_clearMazeRegion(void)
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0x00, 0x66, 0x00, 0xFF);
}

/** Switch on the HGR monochrome graphic mode.
*/
void port_graphics_init(void)
{
    if(SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        return -1;
    }
    win = SDL_CreateWindow("C64MAZE", 2, 2, 
        SIZEX, 
        SIZEY,
        SDL_WINDOW_SHOWN);
    if(win == NULL) {
        puts("error when creating window");
        puts(SDL_GetError());
        return -1;
    }
    ren = SDL_CreateRenderer(win, -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(ren == NULL) {
        puts("error when creating renderer");
        puts(SDL_GetError());
        return -1;
    }
}

void port_vert_line(unsigned short x1, unsigned short y1, unsigned short y2)
{
    SDL_RenderDrawLine(ren, x1,y1,x1,y2);
}

void port_diag_line(unsigned short x1, unsigned short y1, unsigned short ix,
    short incx, short incy)
{
    SDL_RenderDrawLine(ren, x1, y1, x1+incx, y1+incy);
}

void port_hor_line(unsigned short x1, unsigned short x2, unsigned short y1)
{
    SDL_RenderDrawLine(ren, x1, y1, x2, y1);
}

void port_printat(unsigned short x, unsigned short y, char *s)
{
    /* TODO, now printing to CMDLINE */
    printf("%s\n\r", s);
}

/* Plot a line using the Bresenham algorithm
   from Nelson Johnson, "Advanced Graphics in C"
   ed. Osborne, McGraw-Hill.
   Horizontal and vertical lines need to be considerably
   speeded up by using a direct access to video RAM.
*/
void port_line(unsigned short x1, unsigned short y1,
        unsigned short x2, unsigned short y2)
{
    SDL_RenderDrawLine(ren, x1, y1, x2, y2);
}

unsigned long port_get_time(void)
{
    return time(NULL);
}

void port_colour_banner(void)
{
    unsigned char x;
    unsigned char y;

    for(x=25;x<40;++x) {
        for(y=0;y<25;++y) {
            pset(x, y);
        }
    }
}

long port_get_current_time(void)
{
    return time(NULL);
}

static void port_process_voice(unsigned char **ptr, unsigned char *sid_pointer,
    unsigned char *wsh)
{
    /* TODO */
}
unsigned char port_sound_irq(void)
{
    /* TODO */
}

void port_start_sound(unsigned char *l1, unsigned char *l2, unsigned char *l3)
{
    /* TODO */
}

void port_loadVICFont(unsigned char magnification)
{
    unsigned int i;
    /* Load the font tables */
    for(i=0; i<256; ++i)
        f.pos[i]=0;

    for(i=' '; i<='~'; ++i)
        f.pos[i]=(i-' '+1)*8;

    f.pDesc=vic_font;
    f.incX=8;    /* Increment in X (-1 would mean a proportional font) */
    f.incY=8;    /* Increment in Y */
    f.magnification=magnification;
}

char port_getch(void)
{
    SDL_WaitEvent(&event);
    SDL_PollEvent(&event);
    return event.key.keysym.sym;
}

void port_fflushMazeRegion(void)
{
    SDL_SetRenderDrawColor(ren, 0x00, 0x66, 0x00, 0xFF);
    SDL_RenderPresent(ren);
}
