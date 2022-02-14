#include <stdint.h>
#define PSF_FONT_MAGIC 0x864ab572

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
extern char *fb;
/* number of bytes in each line, it's possible it's not screen width * bytesperpixel! */
extern int scanline;

extern char _binary_font_psf_start;
extern char _binary_font_psf_end;

#define PIXEL uint32_t /* pixel pointer */

void putchar (
        /* note that this is int, not char as it's a unicode character */
        unsigned short int c,
        /* cursor position on screen, in characters not in pixels */
        int cx,
        int cy,
        /* foreground and background colors, say 0xFFFFFF and 0x000000 */
        uint32_t fg,
        uint32_t bg)
{
        /* cast the address to PSF header struct */
        PSF_font *font = (PSF_font *)&_binary_font_psf_start;
        /* we need to know how many bytes encode one row */
        int bytesperline = (font->width + 7) / 8;

        /* get the glyph for the character. If there's no
           glyph for a given character, we'll display the first glyph. */

        unsigned char *glyph = (unsigned char *)&_binary_font_psf_start + font->headersize +
                               (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;

        /* calculate the upper left corner on screen where we want to display.
           we only do this once, and adjust the offset later. This is faster. */
        int offs = (cy * font->height * scanline) + (cx * (font->width + 1) * sizeof (PIXEL));

        /* finally display pixels according to the bitmap */
        int x, y, line, mask;
        for (y = 0; y < font->height; y++) {
                /* save the starting position of the line */
                line = offs;
                mask = 1 << (font->width - 1);
                /* display a row */
                for (x = 0; x < font->width; x++) {
                        *((PIXEL *)(fb + line)) = *((unsigned int *)glyph) & mask ? fg : bg;
                        /* adjust to the next pixel */
                        mask >>= 1;
                        line += sizeof (PIXEL);
                }
                /* adjust to the next line */
                glyph += bytesperline;
                offs += scanline;
        }
}