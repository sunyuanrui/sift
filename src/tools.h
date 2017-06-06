# ifndef GRAYSCALE_H
# define GRAYSCALE_H

# include <SDL2/SDL.h>
# include <stddef.h>


void grayscale(SDL_Surface *img);
SDL_Surface* convolution(SDL_Surface *img, float* matrix[], int s);
float** get_gaussian_filter(float sigma);
int* histogram_1d(SDL_Surface* image);

# endif
