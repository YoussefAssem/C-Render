#include "stdio.h"
#include "image.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#define swap(x,y) do { int temp = x; x = y; y = temp; } while(0)
#define swapVertex(x,y) do {     uivec3 temp = { x[0], x[1], x[2] }; \
    x[0] = y[0];\
    x[1] = y[1];\
    x[2] = y[2];\
\
    y[0] = temp[0];\
    y[1] = temp[1];\
    y[2] = temp[2]; } while(0)
#define abs(x) (x > 0 ? x  :-(x))

Pixel r = { .r = 255, .g = 0  , .b = 0  };
Pixel g = { .r = 0  , .g = 255, .b = 0  };
Pixel b = { .r = 0  , .g = 0  , .b = 255};
Pixel y = { .r = 255, .g = 255, .b = 0  };
Pixel p = { .r = 255, .g = 0, .b = 255  };
Pixel c = { .r = 0, .g = 255, .b = 255  };
Pixel w = { .r = 255, .g = 255, .b = 255};

typedef int  uivec3[3];
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
      set_pixel(image, y, x, color);

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
        set_pixel(image, x, y, color);

        errorOP += derrorOP;

        if (errorOP > dx)
        {
          y += (y1 > y0 ? 1:-1);
          errorOP -= 2 * dx;
        }
      }
    }
}

void draw_triangle_wire(CImage* image, uivec3 v0, uivec3 v1, uivec3 v2, Pixel color)
{
  draw_line(image, v0[0], v0[1],
                   v1[0], v1[1],
                   color);

  draw_line(image, v1[0], v1[1],
                   v2[0], v2[1],
                   color);

  draw_line(image, v2[0], v2[1],
                   v0[0], v0[1],
                   color);
}

void draw_triangle_solid(CImage* image, uivec3 v0, uivec3 v1, uivec3 v2, Pixel c)
{
  if (v0[1] == v1[1] && v0[1] == v2[1]) return;

	if (v0[1] > v1[1]) swapVertex(v0, v1);
  if (v0[1] > v2[1]) swapVertex(v0, v2);
	if (v1[1] > v2[1]) swapVertex(v1, v2);

	int y = v0[1];

	int dx01 = v1[0] - v0[0];
	int dy01 = v1[1] - v0[1];
	int derror01 = abs(dx01)* 2;
	int error01 = 0;
	int xcounter01 = v0[0];
	int dy012 = dy01 * 2;

	int dx02 = v2[0] - v0[0];
	int dy02 = v2[1] - v0[1];
	int derror02 = abs(dx02) * 2;
	int error02 = 0;
	int xcounter02 = v0[0];
	int dy022 = dy02 * 2;

	int dx12 = v2[0] - v1[0];
	int dy12 = v2[1] - v1[1];
	int derror12 = abs(dx12) * 2;
	int error12 = 0;
	int xcounter12 = v1[0];
	int dy122 = dy12 * 2;

	for (; y < v1[1]; y++)
	{
		error01 += derror01;
		while (error01 > dy01 && dy01 !=0)
		{
			xcounter01 += (v1[0] > v0[0] ? 1 : -1);
			error01 -= dy012;
		}

		error02 += derror02;
		while (error02 > dy02 && dy02 != 0)
		{
			xcounter02 += (v2[0] > v0[0] ? 1 : -1);
			error02 -= dy022;
		}

		if (xcounter01 > xcounter02)
			for (int x = xcounter02; x <= xcounter01; x++)
				set_pixel(image, x, y, c);

		else
			for (int x = xcounter01; x <= xcounter02; x++)
				set_pixel(image, x, y, c);
	}

	for (; y < v2[1]; y++)
	{
		error12 += derror12;
		while (error12 > dy12 && dy12 != 0)
		{
			xcounter12 += (v2[0] > v1[0] ? 1 : -1);
			error12 -= dy122;
		}

		error02 += derror02;
		while (error02 > dy02 && dy02 != 0)
		{
			xcounter02 += (v2[0] > v0[0] ? 1 : -1);
			error02 -= dy022;
		}

		if (xcounter12 > xcounter02)
			for (int x = xcounter02; x <= xcounter12; x++)
				set_pixel(image, x, y, c);

		else
			for (int x = xcounter12; x <= xcounter02; x++)
				set_pixel(image, x, y, c);
		}
}

void draw_primitive(CImage* image, cgltf_primitive* primitive)
{
  unsigned short ind1;
  unsigned short ind2;
  unsigned short ind3;

  fvec3 v0f;
  fvec3 v1f;
  fvec3 v2f;

  uivec3 v0i;
  uivec3 v1i;
  uivec3 v2i;

  for (size_t i = 0; i < primitive->indices->count; i+=3)
  {
    ind1 = cgltf_accessor_read_index(primitive->indices, i + 0);
    ind2 = cgltf_accessor_read_index(primitive->indices, i + 1);
    ind3 = cgltf_accessor_read_index(primitive->indices, i + 2);

    cgltf_accessor_read_float(primitive->attributes[0].data, ind1, v0f, 3);
    cgltf_accessor_read_float(primitive->attributes[0].data, ind2, v1f, 3);
    cgltf_accessor_read_float(primitive->attributes[0].data, ind3, v2f, 3);

    v0i[0] = (v0f[0] + 1.0) * (WIDTH/2.0);
    v0i[1] = (v0f[1] + 1.0) * (HEIGHT/2.0);

    v1i[0] = (v1f[0] + 1.0) * (WIDTH/2.0);
    v1i[1] = (v1f[1] + 1.0) * (HEIGHT/2.0);

    v2i[0] = (v2f[0] + 1.0) * (WIDTH/2.0);
    v2i[1] = (v2f[1] + 1.0) * (HEIGHT/2.0);

    if (WIREFRAME)
      draw_triangle_wire(image, v0i, v1i, v2i , w);
    else
      draw_triangle_solid(image, v0i, v1i, v2i, w);
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

  // uivec3 p11 = {10, 70,0};
  // uivec3 p12 = {50, 160,0};
  // uivec3 p13 = {70, 80,0};
  //
  // uivec3 p21 = {180, 50,0};
  // uivec3 p22 = {150, 1,0};
  // uivec3 p23 = {70, 180,0};
  //
  // uivec3 p31 = {180, 150,0};
  // uivec3 p32 = {120, 160,0};
  // uivec3 p33 = {130, 180,0};
  //
  // draw_triangle_full(&image, p11, p12, p13, r);
  // draw_triangle_full(&image, p21, p22, p23, w);
  // draw_triangle_full(&image, p31, p32, p33, g);

  write_image(&image, "output", TGA);

  //cleanup
  cgltf_free(data);
  destroy_image(&image);

  return 0;
}
