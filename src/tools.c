# include "tools.h"
# include "pixel.h"

# include <math.h>

# define M_PI 3.14159265358979323846

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

SDL_Surface* convolution(SDL_Surface *img, float* matrix[], int s)
{
  SDL_Surface* new = SDL_CreateRGBSurface(0, img->w - s + 1, img->h - s + 1, 32, 0, 0, 0, 0);

  for (int i = 0; i < new->h; ++i)
  {
    for (int j = 0; j < new->w; ++j)
    {
      int r = 0;
      int g = 0;
      int b = 0;

      struct color c;
      for (int k = -s / 2; k <= s / 2; ++k)
      {
        for (int l = -s / 2; l <= s / 2; ++l)
        {
          struct color p = pix_to_color(get_pixel(img, j + k + (s / 2), i + l + (s / 2)), img->format);
          r += p.r * matrix[k + s / 2][l + s / 2];
          g += p.g * matrix[k + s / 2][l + s / 2];
          b += p.b * matrix[k + s / 2][l + s / 2];
        }
      }

      r = r < 0 ? 0 : r;
      r = r > 255 ? 255 : r;
      g = g < 0 ? 0 : g;
      g = g > 255 ? 255 : g;
      b = b < 0 ? 0 : b;
      b = b > 255 ? 255 : b;

      c.r = r;
      c.g = g;
      c.b = b;

      put_pixel(new, j, i, to_int(c));
    }
  }
  return new;
}

float** get_gaussian_filter(float sigma)
{
  int r = 2 * sigma;
  int s = 2 * r + 1;

  float ** out = malloc(s * sizeof(float*));
  for (int i = 0; i < s; ++i)
    out[i] = calloc(sizeof(float), s);

  float kernelFactor = 0.0;
  float e = 0.0;

  for (int i = -r; i <= r; ++i)
  {
    for (int j = -r; j <= r; ++j)
    {
      /*e = (-1 / (M_PI * sigma * sigma * sigma * sigma)) *
           (1 - ((i * i + j * j) / (2 * sigma * sigma))) *
           exp(-((i * i + j * j) / (2 * sigma * sigma)));*/
      e = exp(-((i * i + j * j) / 2 / sigma / sigma)) *
          (i*i+j*j-2*sigma*sigma)/sigma/sigma/sigma/sigma;

      kernelFactor += e;
      //printf("%f\n", kernelFactor);
      out[i + r][j + r] = e;
    }
  }

  for (int i = -r; i <= r; ++i)
  {
    for (int j = -r; j <= r; ++j)
    {
      //out[i + r][j + r] /= kernelFactor;
    }
  }

  return out;
}
