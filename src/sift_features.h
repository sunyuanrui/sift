# ifndef SIFT_FEATURES
# define SIFT_FEATURES

# include <sys/queue.h>
# include <SDL2/SDL.h>

typedef struct Points {
  int x;
  int y;

  SLIST_ENTRY(Points) next;
} Points;

void get_interest(SDL_Surface* img);
void gradient_pyramide(SDL_Surface* img);

# endif
