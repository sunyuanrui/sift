# ifndef SIFT_FEATURES
# define SIFT_FEATURES

# include <sys/queue.h>
# include <SDL2/SDL.h>

typedef struct Points {
  int x;
  int y;

  double g;
  double sigma;
  double size;
  double hist[128];

  SLIST_ENTRY(Points) next;
} Points;

SLIST_HEAD(slisthead, Points) get_interest(SDL_Surface* img, double sigma);
struct slisthead gradient_pyramide(SDL_Surface* img);
void match(struct slisthead l1, struct slisthead l2);

# endif
