#include "stdio.h"
#include "image.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#define swap(x,y) do { int temp = x; x = y; y = temp; } while(0)
#define abs(x) (x > 0 ? x  :-(x))

Pixel r = { .r = 255, .g = 0  , .b = 0  };
Pixel g = { .r = 0  , .g = 255, .b = 0  };
Pixel b = { .r = 0  , .g = 0  , .b = 255};
Pixel y = { .r = 255, .g = 255, .b = 0  };
Pixel p = { .r = 255, .g = 0, .b = 255  };
Pixel c = { .r = 0, .g = 255, .b = 255  };
Pixel w = { .r = 255, .g = 255, .b = 255};

typedef uint  uivec3[3];
typedef uint16_t  usvec3[3];
typedef float fvec3[3];

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

void draw_triangle(CImage* image, fvec3 v0, fvec3 v1, fvec3 v2)
{
  draw_line(image, (v0[0] + 1.0) * (WIDTH/2.0), (v0[1]+1.0) * (HEIGHT/2.0),
                   (v1[0] + 1.0) * (WIDTH/2.0), (v1[1]+1.0) * (HEIGHT/2.0),
                    w);

  draw_line(image, (v1[0] + 1.0) * (WIDTH/2.0), (v1[1]+1.0) * (HEIGHT/2.0),
                   (v2[0] + 1.0) * (WIDTH/2.0), (v2[1]+1.0) * (HEIGHT/2.0),
                    w);

  draw_line(image, (v2[0] + 1.0) * (WIDTH/2.0), (v2[1]+1.0) * (HEIGHT/2.0),
                   (v0[0] + 1.0) * (WIDTH/2.0), (v0[1]+1.0) * (HEIGHT/2.0),
                    w);
}

void draw_primitive(CImage* image, cgltf_primitive* primitive)
{
  unsigned short ind1;
  unsigned short ind2;
  unsigned short ind3;

  fvec3 v0;
  fvec3 v1;
  fvec3 v2;

  for (size_t i = 0; i < primitive->indices->count; i+=3)
  {
    ind1 = cgltf_accessor_read_index(primitive->indices, i + 0);
    ind2 = cgltf_accessor_read_index(primitive->indices, i + 1);
    ind3 = cgltf_accessor_read_index(primitive->indices, i + 2);

    cgltf_accessor_read_float(primitive->attributes[0].data, ind2, v1, 3);
    cgltf_accessor_read_float(primitive->attributes[0].data, ind1, v0, 3);
    cgltf_accessor_read_float(primitive->attributes[0].data, ind3, v2, 3);

    draw_triangle(image, v0, v1, v2);
  }
}

void draw_mesh(CImage* image, cgltf_mesh* mesh)
{
  for (size_t i = 0; i < mesh->primitives_count; i++)
  {
    cgltf_primitive* primitive = &(mesh->primitives[i]);

    draw_primitive(image, primitive);
  }
}

void draw_scene(cgltf_data* scene,CImage* image)
{
  if (!scene->meshes_count)
    return;

  for (size_t i = 0; i < scene->scenes[0].nodes_count; i++)
  {
    if (scene->scenes[0].nodes[i]->mesh)
      draw_mesh(image, scene->scenes[0].nodes[i]->mesh);
  }
}

cgltf_data* load_scene(char* filename)
{
  cgltf_data* data = NULL;

  cgltf_options options = {0};
  cgltf_result sceneResult = cgltf_parse_file(&options, filename, &data);
  if (sceneResult != cgltf_result_success)
  {
    printf("error loading scene file");
    return 0;
  }

  cgltf_result bufferResult = cgltf_load_buffers(&options, data, filename);
  if (bufferResult != cgltf_result_success)
  {
    printf("error loading scene buffers");
    return 0;
  }

  return data;
}

int main()
{
  CImage image = create_cimage(3);

  cgltf_data* data = load_scene("scene.gltf");

  draw_scene(data, &image);

  write_image(&image, "output", TGA);

  //cleanup
  cgltf_free(data);
  destroy_image(&image);

  return 0;
}
