/*
 *
 */

#define GUI_IMPLEMENT
#include "gui.h"

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define FPS 1

/*
 *
 */
static inline int game_render(gui_t* gui)
{

}

/*
 * Main function
 */
int main(int argc, char* argv[])
{
  if (sdl_init() != 0)
  {
    fprintf(stderr, "sdl_init: %s", strerror(errno));

    return 1;
  }

  gui_t* gui = gui_create(800, 600, "My GUI");

  gui_menu_t* main_menu = gui_menu_create(gui, "main");


  gui_rect_t gui_rect = { 0 };

  gui_rect.width = (gui_size_t)
  {
    .type = GUI_SIZE_MAX
  };
  
  gui_rect.height = (gui_size_t)
  {
    .type = GUI_SIZE_REL,
    .value.rel = 0.4
  };

  gui_rect.xpos = GUI_CENTER;
  gui_rect.ypos = GUI_CENTER;

  gui_window_t* parent_window = gui_menu_window_create(main_menu, "parent", gui_rect);

  gui_asset_t assets[] = {
    { "symbol-one", "../minesweeper/assets/textures/symbol-one.png" },
    { "symbol-two", "../minesweeper/assets/textures/symbol-two.png" }
  };

  if (gui_textures_load(gui, assets, 2) == 0)
  {
    printf("Loaded textures\n");
  }

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
        if (gui_menu_texture_render(main_menu, "symbol-one", 0, 0, -1, -1) == 0)
        {
          printf("Rendered texture\n");
        }

        if (gui_window_texture_render(parent_window, "symbol-two", 0, 0, 100, 100) == 0)
        {
          printf("Rendered texture\n");
        }

        if (gui_window_texture_render(parent_window, "symbol-one", 0, 100, 100, 100) == 0)
        {
          printf("Rendered texture\n");
        }

        printf("gui_render: %d\n", gui_render(gui));

        start_ticks = end_ticks;
      }
    }

    gui_destroy(&gui);
  }

  sdl_quit();

  return 0;
}
