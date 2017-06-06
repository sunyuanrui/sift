# include <stdio.h>
# include <stdlib.h>

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL2_rotozoom.h>

# include "tools.h"
# include "sift_features.h"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "No image\n");
    exit(1);
  }

  int quit = 0;
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);

  SDL_Surface* image = IMG_Load(argv[1]);

  if(!image) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }

  SDL_Window* window = SDL_CreateWindow("SDL2 Displaying Image",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image->w, image->h, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);


  /*float scale = 1;//1.1 * sqrt(2);

  float **cov = get_gaussian_filter(scale);

  int r = scale * 2;
  int s = r * 2 + 1;

  for (int i = 0; i < s; ++i)
  {
    for (int j = 0; j < s; ++j)
      printf("%f ", cov[i][j]);
    printf("\n");
  }

  image = convolution(image, cov, s);

  printf("Convolution: OK\n");

  grayscale(image);*/
  gradient_pyramide(image);

  /*FILE* f = fopen("hist.csv", "w");

  int* hist = histogram_1d(image);
  for (int i = 0; i < 256; ++i)
    fprintf(f, "%d;", i);
  fprintf(f, "\n");
  for (int i = 0; i < 256; ++i)
    fprintf(f, "%d;", hist[i]);*/

  printf("Grayscale: OK\n");

  //get_interest(image);

  float zoom = (image->w > image->h ? image->w : image->h) / 600.0;
  image = rotozoomSurface(image, 0.0, zoom, 1);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

  while (!quit)
  {
    SDL_WaitEvent(&event);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    switch (event.type)
    {
      case SDL_QUIT:
        quit = 1;
        break;
    }
  }

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  SDL_Quit();

  return 0;
}
