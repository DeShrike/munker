#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "pnghelper.h"

#define WIDTH 800
#define HEIGHT 600

#define BALLRADIUS 40
#define LINETHICKNESS 2

#define BALLCOLS (WIDTH / (BALLRADIUS * 3))
#define BALLROWS (HEIGHT / (BALLRADIUS * 3))

#define BALLCOUNT (BALLROWS * BALLCOLS)
#define COLORCOUNT (sizeof(colors) / sizeof(uint32_t))

#define FILENAMES "munker-white-%04d.png"

#define COLOR_OF_LINE(y) (pixels[(y) * WIDTH)

typedef struct {
   int x;
   int y;
   int dy;
   int col;
} Ball;

uint32_t pixels[WIDTH * HEIGHT] = { 0 };
int rowcolorindex[HEIGHT] = { 0 };

uint32_t colors[] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
uint32_t ballcolor = 0xFF555555;

Ball balls[BALLCOUNT] = { 0 };

void create_balls()
{
   int w = WIDTH / BALLCOLS;
   int h = HEIGHT / BALLROWS;
   int b = 0;
   for (int y = 0; y < BALLROWS; ++y)
   {
      for (int x = 0; x < BALLCOLS; ++x)
      {
         #define O 30
         int ox = rand() % O - (O / 2);
         int oy = rand() % O - (O / 2);
         int xx = x * w + (w / 2) + ox;
         int yy = y * h + (h / 2) + oy;
         balls[b].x = xx;
         balls[b].y = yy;
         balls[b].dy = 1;
         balls[b].col = rand() % COLORCOUNT;
         b++;
      }
   }
}

void generate_bg()
{
   int y = 0;
   int col = 0;
   while (y < HEIGHT)
   {
      for (int i = 0; i <= LINETHICKNESS; i++)
      {
         rowcolorindex[y] = col;
         for (int x = 0; x < WIDTH; ++x)
         {
      		pixels[WIDTH * y + x] = colors[col];
         }

         y++;
      }

      col += 1;
      col = col % COLORCOUNT;
   }
}

bool inball(int x, int y)
{
   float p = (float)x * (float)x + (float)y * (float)y;
   float pp = sqrt(p);
   return (pp <= BALLRADIUS);
}

void draw_ball(int b)
{
   int x = balls[b].x;
   int y = balls[b].y;
   for (int i = 0; i < BALLRADIUS * 2; i++)
   {
      for (int j = 0; j < BALLRADIUS * 2; j++)
      {
         int xx = i - BALLRADIUS;
         int yy = j - BALLRADIUS;

         int xxx = xx + x;
         int yyy = yy + y;
         if (xxx < 0 || yyy < 0 || xxx >= WIDTH || yyy >= HEIGHT)
         {
            continue;
         }

         if (balls[b].col == rowcolorindex[yyy])
         {
            continue;
         }

         if (inball(xx, yy))
         {
      		pixels[WIDTH * yyy + xxx] = ballcolor;
         }
      }
   }
}

void draw_balls()
{
   for (int b = 0; b < BALLCOUNT; ++b)
   {
      draw_ball(b);
   }
}

void generate_frame(int frame)
{
   char title[100];
   sprintf(title, "Munker-White %04d", frame);
   printf("%s\n", title);

   generate_bg();
   draw_balls();

   char filename[255];
   sprintf(filename, FILENAMES, frame);
   printf("Writing %s\n", filename);
	write_png_file(filename, title, WIDTH, HEIGHT, pixels);
}

int main()
{
   srand(time(NULL));
   create_balls();
   generate_frame(1);
	printf("Done\n");

	return 0;
}
