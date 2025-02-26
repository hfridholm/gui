/*
 *
 */

#define GUI_IMPLEMENT
#include "gui.h"

#include <stdio.h>
#include <stdbool.h>

#define FPS 1

/*
 * Main function
 */
int main(int argc, char* argv[])
{
  gui_t* gui = gui_create(800, 600, "My GUI");

  gui_menu_create(gui, "main");

  if (gui)
  {
    Uint32 end_ticks = 0, start_ticks = 0;

    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));

    bool is_running = true;

    while(is_running)
    {
      while(SDL_PollEvent(&event))
      {
        if(event.type == SDL_QUIT)
        {
          is_running = false;

          break;
        }
      }

      end_ticks = SDL_GetTicks();

      if(end_ticks - start_ticks >= 1000 / FPS)
      {
        printf("rendering...\n");

        start_ticks = end_ticks;
      }
    }

    gui_destroy(&gui);
  }

  return 0;
}
