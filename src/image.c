#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct CImage
{
  char* imageName;

  ImageProperties imageProperties;

  void* pixels;
};

void write_image(CImage* image)
{
  stbi_flip_vertically_on_write(1);
  
  switch (image->imageProperties.type)
  {
    case TGA:
      stbi_write_tga(
        image->imageName,
        image->imageProperties.width, image->imageProperties.height,
        image->imageProperties.compnents, image->pixels);
      break;
    default:
      printf("not a suppported type!\n");
  }
}

CImage* create_image(char* imageName, ImageProperties* imageProperties)
{
  CImage* image = malloc( sizeof(CImage) );

  image->imageName = malloc(strlen(imageName) + 1);
  strcpy(image->imageName, imageName);

  image->imageProperties.type = imageProperties->type;
  image->imageProperties.width = imageProperties->width;
  image->imageProperties.height = imageProperties->height;
  image->imageProperties.compnents = imageProperties->compnents;

  image->pixels = calloc(imageProperties->width * imageProperties->height, imageProperties->compnents);

  return image;
}

void destroy_image(CImage* image)
{
  free(image->imageName);
  free(image->pixels);
  free(image);
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

  pixels[x + (y * image->imageProperties.width)] = pxValue;
}
