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
  int xy = pix_to_color(get_pixel(img, x, y), img->format).r;

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

  return atan2((y1 - y_1), (x1 - x_1));
}

/*double* get_hist(SDL_Surface* img, int x, int y, double sigma)
{
  double* out = calloc(36, sizeof(double));
  return out;  
}*/

int is_interest(int j, int i, SDL_Surface* img)
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

  return ((me > p1 && me > p2 && me > p3 && me > p4 && me > p5 && me > p6 && me > p7 && me > p8
            && remove_edges(img, j, i)) ||
          (me < p1 && me < p2 && me < p3 && me < p4 && me < p5 && me < p6 && me < p7 && me < p8
            && remove_edges(img, j, i)));
}

struct slisthead get_interest(SDL_Surface* img, double sigma)
{
  sigma = sigma;
  struct slisthead head;
  SLIST_INIT(&head);

  int nb_dots = 0;

  for (int i = 1; i < img->h - 1; ++i)
  {
    for (int j = 1; j < img->w - 1; ++j)
    {
      if (is_interest(j, i, img))
      {
        double hist[8] = {0};

        for (int xx = j - 1.6 * sigma; xx <= j + 1.6 * sigma; ++xx)
        {
          for (int yy = i - 1.6 * sigma; yy <= i + 1.6 * sigma; ++yy)
          {
            double mm = compute_m(img, xx, yy);
            double gauss = exp(-pow(j - xx, 2) - pow(i - yy, 2));
            hist[(int)(((compute_g(img, xx, yy) + 3.1416) * 57.3) / 45)] += mm * gauss;
          }
        }

        int maxi = 0;
        double maxg = hist[0];
        for (int ig = 1; ig < 8; ++ig)
        {
          if (hist[ig] > maxg)
          {
            maxi = ig;
            maxg = hist[ig];
          }
        }
        
        Points* p = malloc(sizeof(Points));

        for (int h = 0; h < 128; ++h)
          p->hist[h] = 0;

        for (int ii = -2; ii < 2; ++ii)
        {
          for (int jj = -2; jj < 2; ++jj)
          {
            for (int iii = i + ii*4; iii < i + (ii + 1) * 4; ++iii)
            {
              for (int jjj = j + jj*4; jjj < j + (jj + 1) * 4; ++jjj)
              {
                double mm = compute_m(img, jjj, iii);
                double angle = ((compute_g(img, jjj, iii) + 3.1416) * 57.3) / 45;
                p->hist[((ii + 2) * 4 + (jj + 2)) * 8 + (int)angle] += mm;
              }
            }
          }
        }

        for (int h = 0; h < 128; ++h)
          p->hist[h] = p->hist[h] > 0.2 ? 0.2 : p->hist[h];

        p->x = j;
        p->y = i;
        p->g = maxi * 45;
        SLIST_INSERT_HEAD(&head, p, next);
        ++nb_dots;

        double p80 = 0.8 * maxg;
        for (int ig = 1; ig < 8; ++ig)
        {
          if (hist[ig] >= p80 && ig != maxi)
          {
            Points* p2 = malloc(sizeof(Points));
            p2->x = j;
            p2->y = i;
            p2->g = maxi * 45;
            for (int a = 0; a < 128; ++a)
              p2->hist[a] = p->hist[a];
            SLIST_INSERT_HEAD(&head, p2, next);
            ++nb_dots;
          }
        }

        draw_circle(img, 10, j, i);
      }
    }
  }
  printf("    points = %d\n", nb_dots);
  return head;
}

int find(Points* p, struct slisthead l)
{
  Points* p1 = malloc(sizeof(Points));
  SLIST_FOREACH(p1, &l, next)
  {
    if (p->x / p->size == p1->x / p1->size
        && p->y / p->size == p1->y / p1->size)
      return 1;
  }
  return 0;
}

struct slisthead gradient_pyramide(SDL_Surface* img)
{
  grayscale(img);
  SDL_Surface*** octaves = malloc(5 * sizeof(SDL_Surface**));
  octaves = octaves;

  struct slisthead descript;
  SLIST_INIT(&(descript));

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
      if (1)
      {
        float **cov = get_gaussian_filter(sigma);
        int r = sigma * 2;
        int s = r * 2 + 1;

        octaves[i][j] = convolution(octaves[i][j], cov, s);
        struct slisthead head = get_interest(octaves[i][j], sigma);

        Points* p = malloc(sizeof(Points));
        SLIST_FOREACH(p, &head, next)
        {
          Points* np = malloc(sizeof(Points));
          np->x = p->x;
          np->y = p->y;
          np->g = p->g;
          np->sigma = sigma;
          np->size = ratio;
          for (int a = 0; a < 128; ++a)
            np->hist[a] = p->hist[a];

          SLIST_INSERT_HEAD(&(descript), np, next);
        }

        sigma *= sqrt(2) / 2;

        char buf[256];
        snprintf(buf, sizeof buf, "f%d%d", i, j);

        SDL_SaveBMP(octaves[i][j], buf);
      }
    }
  }

  return descript;
}

double dist_p(Points* a, Points* b)
{
  double sum = 0;
  for (int j = 0; j < 128; ++j)
  {
    sum += pow(a->hist[j] - b->hist[j], 2);
  }
  return sqrt(sum);
}

void match(struct slisthead l1, struct slisthead l2)
{
  Points* p1 = malloc(sizeof(Points));
  double nb = 0;
  double re = 0;
  SLIST_FOREACH(p1, &l1, next)
  {
    Points* max = NULL;
    double dist = -1;
    Points* max2 = NULL; //FIXME
    max2 = max2;
    double dist1 = -1;   //FIXME

    Points* p2 = malloc(sizeof(Points));
    SLIST_FOREACH(p2, &l2, next)
    {
      double distm = dist_p(p1, p2);
      if (dist == -1 || distm < dist)
      {
        dist1 = dist;
        max2 = max;
        max = p2;
        dist = distm;
      }
    }
    if (dist / dist1 < 0.8)
      re++;
    nb++;
  }
  printf("%f\n", re/nb);
}
