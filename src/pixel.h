# ifndef PIXEL_H
# define PIXEL_H

# include <SDL2/SDL.h>
# include <stdint.h>

struct color {
  uint8_t a;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

uint32_t to_int(struct color c);
struct color pix_to_color(uint32_t b, const SDL_PixelFormat* format);

uint32_t get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, uint32_t pixel);

# endif
