#pragma once

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
  IMAGETYPE type;

  int width;
  int height;
  int compnents;
} ImageProperties;

typedef struct CImage CImage;

CImage* create_image(char* imageName, ImageProperties* imageProperties);

void destroy_image(CImage* image);

void write_image(CImage* image);

void set_pixel(CImage* image, int x, int y, char r, char g, char b, char a);
