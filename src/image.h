#pragma once

#define WIREFRAME    0

#define WIDTH        2000
#define HEIGHT       2000
#define COMPONENTS   3

typedef enum
{
  PNG, BMP, TGA, JPG, HDR
} IMAGETYPE;

typedef struct
{
  char r;
  char g;
  char b;
} Pixel;

typedef struct
{
  unsigned int components;
  void* pixels;
}CImage;

CImage create_cimage(unsigned int components);

void write_image(CImage* image, char* outName, IMAGETYPE type);

void set_pixel(CImage* image, int x, int y, Pixel c);

void destroy_image(CImage* image);
