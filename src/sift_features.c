# include "sift_features.h"
# include "pixel.h"

void get_interest(SDL_Surface* img)
{
  SLIST_HEAD(, Points) head;
  SLIST_INIT(&head);

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

      if (me > p1 && me > p2 && me > p3 && me > p4 && me > p5 && me > p6 && me > p7 && me > p8)
      {
        Points* p = malloc(sizeof(p));
        p->x = j;
        p->y = i;

        SLIST_INSERT_HEAD(&head, p, next);

        struct color c = {.a = 0, .r = 255, .g=0, .b=0};
        put_pixel(img, j, i, to_int(c));
      }
      else if (me < p1 && me < p2 && me < p3 && me < p4 && me < p5 && me < p6 && me < p7 && me < p8)
      {
        Points* p = malloc(sizeof(p));
        p->x = j;
        p->y = i;

        SLIST_INSERT_HEAD(&head, p, next);

        struct color c = {.a = 0, .r = 0, .g=255, .b=0};
        put_pixel(img, j, i, to_int(c));
      }
    }
  }
}
