# include "pixel.h"

uint32_t to_int(struct color c)
{
  return (c.a << 24) + (c.r << 16) + (c.g << 8) + c.b;
}

struct color pix_to_color(uint32_t b, const SDL_PixelFormat* format)
{
  struct color out;
  SDL_GetRGB(b, format, &(out.r), &(out.g), &(out.b));
  return out;
}

uint32_t get_pixel(SDL_Surface *surface, int x, int y)
{
  uint8_t *p = (Uint8 *)surface->pixels + y * surface->pitch
    + x * surface->format->BytesPerPixel;

  switch(surface->format->BytesPerPixel) {
    case 1:
      return *p;
      break;

    case 2:
      return *(uint16_t *)p;
      break;

    case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN //
      return p[0] << 16 | p[1] << 8 | p[2];
#else //
      return p[0] | p[1] << 8 | p[2] << 16;
#endif //
      break;

    case 4:
      return *(uint32_t *)p;
      break;

    default:
      return 0;
      break; //
  }
}

void put_pixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
  uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch
    + x * surface->format->BytesPerPixel; //

  switch(surface->format->BytesPerPixel) { //
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *)p = pixel;
      break;

    case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN //
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
#else //
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
#endif //
      break;

    case 4:
      *(uint32_t *)p = pixel;
      break;

    default: // Enlève le warning avec -Wswitch-default
      break; //
  }
}
