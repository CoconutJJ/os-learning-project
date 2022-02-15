#include "multiboot.h"
#include <stdint.h>

#define PSF_FONT_MAGIC 0x864ab572
#define LINES          100
#define COLUMNS        80
#define ATTRIBUTE      7
#define VIDEO          0xB8000

typedef struct {
        uint32_t magic;         /* magic bytes to identify PSF */
        uint32_t version;       /* zero */
        uint32_t headersize;    /* offset of bitmaps in file, 32 */
        uint32_t flags;         /* 0 if there's no unicode table */
        uint32_t numglyph;      /* number of glyphs */
        uint32_t bytesperglyph; /* size of each glyph */
        uint32_t height;        /* height in pixels */
        uint32_t width;         /* width in pixels */
} PSF_font;
/* the linear framebuffer */
char *fb, *video;
multiboot_info_t *mbi;

int scanline, xpos, ypos; /* also known as framebuffer_pitch. */
extern char _binary_fonts_8x16_psfu_start;
extern char _binary_fonts_8x16_psfu_end;

#define PIXEL uint32_t

static void putchar (int c)
{
        if (c == '\n' || c == '\r') {
        newline:
                xpos = 0;
                ypos++;
                if (ypos >= LINES) ypos = 0;
                return;
        }

        *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
        *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

        xpos++;
        if (xpos >= COLUMNS) goto newline;
}

void putchar_pixbuf (unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg)
{
        PSF_font *font = (PSF_font *)&_binary_fonts_8x16_psfu_start;
        int bytesperline = (font->width + 7) / 8;

        unsigned char *glyph = (unsigned char *)&_binary_fonts_8x16_psfu_start + font->headersize +
                               (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;

        int offs = (cy * font->height * scanline) + (cx * (font->width + 1) * sizeof (PIXEL));

        int x, y, line, mask;
        for (y = 0; y < font->height; y++) {
                line = offs;
                mask = 1 << (font->width - 1);
                for (x = 0; x < font->width; x++) {
                        *((PIXEL *)(fb + line)) = *((unsigned int *)glyph) & mask ? fg : bg;
                        mask >>= 1;
                        line += sizeof (PIXEL);
                }
                glyph += bytesperline;
                offs += scanline;
        }
}

void kernel_main (unsigned long multiboot_magic_number, unsigned long multiboot_struct_addr)
{
        if (multiboot_magic_number != MULTIBOOT_BOOTLOADER_MAGIC) {
                return;
        }

        mbi = (multiboot_info_t *)multiboot_struct_addr;

        fb = mbi->framebuffer_addr;
        scanline = mbi->framebuffer_pitch;
        video = VIDEO;
        putchar ((int)'e');
        putchar ((int)'n');
        putchar ((int)'t');

        putchar_pixbuf ((short)'!', 0, 0, 0xFFFFFFFF, 0);
}
