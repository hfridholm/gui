/*
 *
 */

#define GUI_IMPLEMENT
#include "gui.h"

#include <stdio.h>

/*
 * Main function
 */
int main(int argc, char* argv[])
{
  gui_size_t size = {
    .type = GUI_SIZE_MAX,
    .value.rel = 0.56
  };

  switch (size.type)
  {
    case GUI_SIZE_ABS:
      printf("Absolute size: %d\n", size.value.abs);
      break;

    case GUI_SIZE_REL:
      printf("Relative size: %f\n", size.value.rel);
      break;

    case GUI_SIZE_MAX:
      printf("Max size\n");
      break;

    default:
      break;
  }

  return 0;
}
