#include "stdio.h"
#include "image.h"

int main()
{
  ImageProperties prop = {
    .type = TGA,
    .width = 100,
    .height = 100,
    .compnents = 3
  };

  CImage* image = create_image("out", &prop);

  set_pixel(image, 50, 50, (char)0, (char)0, (char)0, (char)0);

  write_image(image);

  destroy_image(image);

  return 0;
}
