#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CImage create_cimage(unsigned int components)
{
  CImage image;

  image.components = components;
  image.pixels = calloc(WIDTH * HEIGHT, components);

  return image;
}

void write_image(CImage* image, char* outName, IMAGETYPE type)
{
  stbi_flip_vertically_on_write(1);

  switch (type)
  {
    case TGA:
      stbi_write_tga(
        outName,
        WIDTH, HEIGHT,
        image->components, image->pixels);
      break;
    default:
      printf("not a suppported type!\n");
  }
}

void destroy_image(CImage* image)
{
  free(image->pixels);
}

void set_pixel(CImage* image, int x, int y, char r, char g, char b, char a)
{
  Pixel* pixels = image->pixels;

  Pixel pxValue =
  {
    .r = r,
    .g = g,
    .b = b
  };

  pixels[x + (y * WIDTH)] = pxValue;
}
