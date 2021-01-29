#include "stdio.h"
#include "image.h"

#define swap(x,y) do { int temp = x; x = y; y = temp; } while(0)
#define abs(x) (x > 0 ? x  :-(x))

Pixel r = { .r = 255, .g = 0  , .b = 0  };
Pixel g = { .r = 0  , .g = 255, .b = 0  };
Pixel b = { .r = 0  , .g = 0  , .b = 255};
Pixel y = { .r = 255, .g = 255, .b = 0  };
Pixel p = { .r = 255, .g = 0, .b = 255  };
Pixel c = { .r = 0, .g = 255, .b = 255  };
Pixel w = { .r = 255, .g = 255, .b = 255};

void draw_line(CImage* image, int x0, int y0, int x1, int y1, Pixel color)
{
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);

  float derrorOP = 0.0;
  float errorOP = 0.0;

  if (dx < dy)
  {
    swap(dx, dy);
    swap(x0, y0);
    swap(x1, y1);

    if(x0 > x1)
    {
      swap(x0, x1);
      swap(y0, y1);
    }

    derrorOP = 2 * dy;

    for (size_t x = x0, y = y0; x < x1; x++)
    {
      set_pixel(image, y, x, color.r, color.g, color.b, (char)255);

      errorOP += derrorOP;

      if (errorOP > dx)
      {
        y += (y1 > y0 ? 1:-1);
        errorOP -= 2 * dx;
      }
    }
  }

  else
  {
      if(x0 > x1)
      {
        swap(x0, x1);
        swap(y0, y1);
      }

      derrorOP = 2 * dy;

      for (size_t x = x0, y = y0; x < x1; x++)
      {
        set_pixel(image, x, y, color.r, color.g, color.b, (char)255);

        errorOP += derrorOP;

        if (errorOP > dx)
        {
          y += (y1 > y0 ? 1:-1);
          errorOP -= 2 * dx;
        }
      }
    }
}

int main()
{
  ImageProperties prop = {
    .type = TGA,
    .width = 100,
    .height = 100,
    .compnents = 3
  };

  CImage* image = create_image("out", &prop);

  draw_line(image, 0, 0, 99, 0, r);
  draw_line(image, 0, 0, 75, 25, g);
  draw_line(image, 0, 0, 50, 50, b);
  draw_line(image, 0, 0, 25, 75, y);
  draw_line(image, 0, 0, 0, 99, p);

  draw_line(image, 99, 99, 99,   0, c);
  draw_line(image, 99, 99,  75,  25, w);
  draw_line(image, 99, 99,  50,  50, r);
  draw_line(image, 99, 99,  25,  75, g);
  draw_line(image, 99, 99,   0, 99, b);

  write_image(image);

  destroy_image(image);

  return 0;
}
