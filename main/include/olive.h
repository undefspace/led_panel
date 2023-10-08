// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef OLIVE_H_
#define OLIVE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef OLIVECDEF
#define OLIVECDEF
#endif

#ifndef OLIVEC_AA_RES
#define OLIVEC_AA_RES 2
#endif

#define OLIVEC_SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define OLIVEC_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define OLIVEC_ABS(T, x) (OLIVEC_SIGN(T, x)*(x))

typedef struct {
    const uint8_t* alpha_image;
    const size_t* glyph_map;
    size_t width, height, stride, img_stride;
} Olivec_Font;

typedef struct {
    uint32_t *pixels;
    size_t width;
    size_t height;
    size_t stride;
} Olivec_Canvas;

#define OLIVEC_CANVAS_NULL ((Olivec_Canvas) {0})
#define OLIVEC_PIXEL(oc, x, y) (oc).pixels[(y) * ((oc).stride) + (x)]

OLIVECDEF Olivec_Canvas olivec_canvas(uint32_t *pixels, size_t width, size_t height, size_t stride);
OLIVECDEF Olivec_Canvas olivec_subcanvas(Olivec_Canvas oc, int x, int y, int w, int h);
OLIVECDEF bool olivec_in_bounds(Olivec_Canvas oc, int x, int y);
OLIVECDEF void olivec_blend_color(uint32_t *c1, uint32_t c2);
OLIVECDEF void olivec_blend_color_fraction(uint32_t *c1, uint32_t c2, uint8_t fraction);
OLIVECDEF void olivec_fill(Olivec_Canvas oc, uint32_t color);
OLIVECDEF void olivec_rect(Olivec_Canvas oc, int x, int y, int w, int h, uint32_t color);
OLIVECDEF void olivec_frame(Olivec_Canvas oc, int x, int y, int w, int h, size_t thiccness, uint32_t color);
OLIVECDEF void olivec_circle(Olivec_Canvas oc, int cx, int cy, int r, uint32_t color);
OLIVECDEF void olivec_ellipse(Olivec_Canvas oc, int cx, int cy, int rx, int ry, uint32_t color);
// TODO: lines with different thiccness
OLIVECDEF void olivec_line(Olivec_Canvas oc, int x1, int y1, int x2, int y2, uint32_t color);
OLIVECDEF void olivec_line_aa(Olivec_Canvas oc, float x0, float y0, float x1, float y1, uint32_t color);
OLIVECDEF bool olivec_normalize_triangle(size_t width, size_t height, int x1, int y1, int x2, int y2, int x3, int y3, int *lx, int *hx, int *ly, int *hy);
OLIVECDEF bool olivec_barycentric(int x1, int y1, int x2, int y2, int x3, int y3, int xp, int yp, int *u1, int *u2, int *det);
OLIVECDEF void olivec_triangle(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
OLIVECDEF void olivec_triangle3c(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t c1, uint32_t c2, uint32_t c3);
OLIVECDEF void olivec_triangle3z(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float z1, float z2, float z3);
OLIVECDEF void olivec_triangle3uv(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float z1, float z2, float z3, Olivec_Canvas texture);
OLIVECDEF void olivec_triangle3uv_bilinear(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float z1, float z2, float z3, Olivec_Canvas texture);
OLIVECDEF void olivec_text(Olivec_Canvas oc, const char *text, int tx, int ty, Olivec_Font font, uint32_t color);
OLIVECDEF void olivec_sprite_blend(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite);
OLIVECDEF void olivec_image_draw(Olivec_Canvas oc, int ox, int oy, Olivec_Canvas sprite, uint8_t alpha);
OLIVECDEF void olivec_sprite_copy(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite);
OLIVECDEF void olivec_sprite_copy_bilinear(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite);
OLIVECDEF uint32_t olivec_pixel_bilinear(Olivec_Canvas sprite, int nx, int ny, int w, int h);

typedef struct {
    // Safe ranges to iterate over.
    int x1, x2;
    int y1, y2;

    // Original uncut ranges some parts of which may be outside of the canvas boundaries.
    int ox1, ox2;
    int oy1, oy2;
} Olivec_Normalized_Rect;

// The point of this function is to produce two ranges x1..x2 and y1..y2 that are guaranteed to be safe to iterate over the canvas of size pixels_width by pixels_height without any boundary checks.
//
// Olivec_Normalized_Rect nr = {0};
// if (olivec_normalize_rect(x, y, w, h, WIDTH, HEIGHT, &nr)) {
//     for (int x = nr.x1; x <= nr.x2; ++x) {
//         for (int y = nr.y1; y <= nr.y2; ++y) {
//             OLIVEC_PIXEL(oc, x, y) = 0x69696969;
//         }
//     }
// } else {
//     // Rectangle is invisible cause it's completely out-of-bounds
// }
OLIVECDEF bool olivec_normalize_rect(int x, int y, int w, int h,
                                     size_t canvas_width, size_t canvas_height,
                                     Olivec_Normalized_Rect *nr);

#endif // OLIVE_H_
