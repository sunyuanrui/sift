# include "sift_features.h"
# include "pixel.h"
# include "tools.h"

# include <math.h>
# include <string.h>

# include <SDL2/SDL2_rotozoom.h>

void draw_circle(SDL_Surface* img, int r, int j, int i)
{
  struct color c = {.a = 0, .r = 255, .g=0, .b=0};
  for (float angle = 0.0; angle<360; angle++)
  {
    int x = j - r * cos(angle);
    int y = i - r * sin(angle);
    if (x < img->w && y < img->h && x >= 0 && y >= 0)
      put_pixel(img, x, y, to_int(c));
  }
}



int remove_edges(SDL_Surface* img, int x, int y)
{
  int x1 = pix_to_color(get_pixel(img, x + 1, y), img->format).r;
  int x_1 = pix_to_color(get_pixel(img, x - 1, y), img->format).r;
  int y1 = pix_to_color(get_pixel(img, x, y + 1), img->format).r;
  int y_1 = pix_to_color(get_pixel(img, x, y - 1), img->format).r;
  int xy11 = pix_to_color(get_pixel(img, x + 1, y + 1), img->format).r;
  int xy_1_1 = pix_to_color(get_pixel(img, x - 1, y - 1), img->format).r;
  int xy1_1 = pix_to_color(get_pixel(img, x + 1, y - 1), img->format).r;
  int xy_11 = pix_to_color(get_pixel(img, x - 1, y + 1), img->format).r;
  int xy = pix_to_color(get_pixel(img, x - 1, y + 1), img->format).r;

  double dxx = x1 + x_1 - 2 * xy;
  double dyy = y1 + y_1 - 2 * xy;

  double dxy = (xy11 + xy_11 + xy1_1 + xy_1_1) / 4.0;

  double tr = dxx + dyy;
  double det = dxx * dyy -  dxy * dxy;

  if (det <= 0)
    return 0;
  double r = 10.0;
  double R = pow(tr, 2) / det;
  double rth = pow(r + 1, 2) / r;

  return R < rth;
}

double compute_m(SDL_Surface* img, int x, int y)
{
  double x1 = pix_to_color(get_pixel(img, x + 1, y), img->format).r;
  double x_1 = pix_to_color(get_pixel(img, x - 1, y), img->format).r;
  double y1 = pix_to_color(get_pixel(img, x, y + 1), img->format).r;
  double y_1 = pix_to_color(get_pixel(img, x, y - 1), img->format).r;

  return sqrt(pow(x1 - x_1, 2) + pow(y1 - y_1, 2));
}

double compute_g(SDL_Surface* img, int x, int y)
{
  double x1 = pix_to_color(get_pixel(img, x + 1, y), img->format).r;
  double x_1 = pix_to_color(get_pixel(img, x - 1, y), img->format).r;
  double y1 = pix_to_color(get_pixel(img, x, y + 1), img->format).r;
  double y_1 = pix_to_color(get_pixel(img, x, y - 1), img->format).r;

  return atan((y1 - y_1) / (x1 - x_1));
}

void get_interest(SDL_Surface* img)
{
  SLIST_HEAD(, Points) head;
  SLIST_INIT(&head);

  int nb_dots = 0;

  for (int i = 1; i < img->h - 1; ++i)
  {
    for (int j = 1; j < img->w - 1; ++j)
    {
      int me = pix_to_color(get_pixel(img, j, i), img->format).r;

      int p1 = pix_to_color(get_pixel(img, j - 1, i - 1), img->format).r;
      int p2 = pix_to_color(get_pixel(img, j - 1, i - 0), img->format).r;
      int p3 = pix_to_color(get_pixel(img, j - 1, i + 1), img->format).r;
      int p4 = pix_to_color(get_pixel(img, j - 0, i - 1), img->format).r;
      int p5 = pix_to_color(get_pixel(img, j - 0, i + 1), img->format).r;
      int p6 = pix_to_color(get_pixel(img, j + 1, i - 1), img->format).r;
      int p7 = pix_to_color(get_pixel(img, j + 1, i - 0), img->format).r;
      int p8 = pix_to_color(get_pixel(img, j + 1, i + 1), img->format).r;

      if (me > p1 && me > p2 && me > p3 && me > p4 && me > p5 && me > p6 && me > p7 && me > p8
         && remove_edges(img, j, i))
      {
        Points* p = malloc(sizeof(p));
        p->x = j;
        p->y = i;

        SLIST_INSERT_HEAD(&head, p, next);

        //struct color c = {.a = 0, .r = 255, .g=0, .b=0};
        //put_pixel(img, j, i, to_int(c));
        draw_circle(img, compute_m(img, j, i) / 10, j, i);
        ++nb_dots;
      }
      else if (me < p1 && me < p2 && me < p3 && me < p4 && me < p5 && me < p6 && me < p7 && me < p8
          && remove_edges(img, j, i))

      {
        Points* p = malloc(sizeof(p));
        p->x = j;
        p->y = i;

        SLIST_INSERT_HEAD(&head, p, next);

        //struct color c = {.a = 0, .r = 0, .g=255, .b=0};
        //put_pixel(img, j, i, to_int(c));
        draw_circle(img, compute_m(img, j, i) / 10, j, i);
        ++nb_dots;
      }
    }
  }
  printf("    points = %d\n", nb_dots);
}

void gradient_pyramide(SDL_Surface* img)
{
  grayscale(img);
  SDL_Surface*** octaves = malloc(5 * sizeof(SDL_Surface**));
  octaves = octaves;

  for (int i = 0; i < 5; ++i)
  {
    double ratio = pow(2, -i);
    printf("size = %f\n", ratio);
    octaves[i] = malloc(4 * sizeof(SDL_Surface*));

    double sigma = 1;

    for (int j = 0; j < 4; ++j)
    {
      printf("  sigma = %f\n", sigma);
      octaves[i][j] = rotozoomSurface(img, 0.0, ratio, 1);
      if (j != 0 && j != 3)
      {
        float **cov = get_gaussian_filter(sigma);
        int r = sigma * 2;
        int s = r * 2 + 1;

        octaves[i][j] = convolution(octaves[i][j], cov, s);
        get_interest(octaves[i][j]);
        sigma *= sqrt(2);

        char buf[256];
        snprintf(buf, sizeof buf, "f%d%d", i, j);

        SDL_SaveBMP(octaves[i][j], buf);
      }
    }
  }
}
