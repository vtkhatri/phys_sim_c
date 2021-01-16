#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct Space {
  uint width;
  uint height;
  uint* space;
};

void sim(int, int);
void display(struct Space);

int main()
{
  uint width = 40;
  uint height = 40;

  sim(width, height);
  return 0;
}

void sim(int width, int height)
{
  uint* space = (uint *) calloc (width*height, sizeof(uint));
  struct Space screen_space = {
    width,
    height,
    space
  };

  while(1) {
    // Start simulating from bottom to prevent particles considered for the sim twice
    // Only works as particles currently only fall down
    for (int y = height-1; y >= 0; y--) {
      for (int x = width-1; x >= 0; x--) {
        if (space[x*width+y]) {
          switch(space[y*width+x]) {
          case 1: // Sand
            if (space[(y+1)*width+x] != 1) { // swapping with particle below if it's not sand
              uint temp = space[(y+1)*width+x];
              space[(y+1)*width+x] = space[y*width+x];
              space[y*width+x] = temp;
            } else if (space[(y+1)*width+x-1] != 1) { // swapping with particle below and to the left if it's not sand
              uint temp = space[(y+1)*width+x-1];
              space[(y+1)*width+x-1] = space[y*width+x];
              space[y*width+x] = temp;
            } else if (space[(y+1)*width+x+1] != 1) { // swapping with particle below and to the right if it's not sand
              uint temp = space[(y+1)*width+x+1];
              space[(y+1)*width+x+1] = space[y*width+x];
              space[y*width+x] = temp;
            }
          case 2: // Water
            if (space[(y+1)*width+x] == 0) { // replacing air below
              space[(y+1)*width+x] = 2;
              space[y*width+x] = 0;
            } else if (space[(y+1)*width+x-1] == 0) { // replacing air below and to the left
              space[(y+1)*width+x-1] = 2;
              space[y*width+x] = 0;
            } else if (space[(y+1)*width+x+1] == 0) { // replacing air below and to the right
              space[(y+1)*width+x+1] = 2;
              space[y*width+x] = 0;
            } else if (space[(y*width)+x-1] == 0) { // replacing air to the left if can't go down anywhere
              space[(y*width)+x-1] = 2;
              space[y*width+x] = 0;
            } else if (space[(y*width)+x+1] == 0) { // replacing air to the right if can't go dow anywhere
              space[(y*width)+x+1] = 2;
              space[y*width+x] = 0;
            }
          }
        }
      }
      if (y==0) {
        if (space[width*4/3]==0) space[width/3] = 1; // put sand if there's nothing below on 1st line, 1/3 width
        if (space[width*5/3]==0) space[width/3] = 2; // put water if there's nothing below on 1st line, 2/3 width
      }
    }

    // Display all particles once simulation is done and sleep for 16ms for 60 "frames"
    display(screen_space);
    usleep(16000);
  }
  return;
}


void display(struct Space screen_space)
{
  char pixel;
  system("clear");
  for (int y = 0; y < screen_space.height; y++) {
    for (int x = 0; x < screen_space.width; x++) {
      switch(screen_space.space[(y*screen_space.width)+x]) {
      case 1:
        pixel = 's';
        break;
      case 2:
        pixel = 'w';
        break;
      default:
        pixel = ' ';
        break;
      }
      printf("%c", pixel);
    }
    printf("\n");
  }
  return;
}
