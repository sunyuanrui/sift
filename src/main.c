# include <stdio.h>
# include <stdlib.h>

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL2_rotozoom.h>

# include "tools.h"

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
  IMG_Init(IMG_INIT_JPG);

  SDL_Window* window = SDL_CreateWindow("SDL2 Displaying Image",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, 0);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Surface* image = IMG_Load(argv[1]);
  grayscale(image);

  float **cov = get_gaussian_filter(2);

  for (int i = 0; i < 2 * 2 * 2 + 1; ++i)
  {
    for (int j = 0; j < 2 * 2 * 2 + 1; ++j)
      printf("%f ", cov[i][j]);
    printf("\n");
  }

  image = convolution(image, cov, 2 * 2 * 2 + 1);

  printf("Convolution: OK\n");

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
