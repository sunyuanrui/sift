# include "grayscale.h"
# include "pixel.h"

void grayscale(SDL_Surface *img)
{
  for (int i = 0; i < img->h; ++i)
    for (int j = 0; j < img->w; ++j)
    {
      struct color c = pix_to_color(get_pixel(img, j, i), img->format);

      int n = 0.21 * (float)c.r + 0.72 * (float)c.g + 0.07 * (float)c.b;

      c.r = c.g = c.b = n;

      put_pixel(img, j, i, to_int(c));
    }
}
