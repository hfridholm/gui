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
    .type = GUI_SIZE_REL,
    .value.rel = 0.5
  };
  
  gui_rect.height = (gui_size_t)
  {
    .type = GUI_SIZE_REL,
    .value.rel = 0.4
  };

  gui_rect.xpos = GUI_RIGHT;
  gui_rect.ypos = GUI_CENTER;

  gui_window_t* parent_window = gui_menu_window_create(main_menu, "parent", gui_rect);

  gui_asset_t assets[] = {
    { "symbol-one", "../minesweeper/assets/textures/symbol-one.png" },
    { "symbol-two", "../minesweeper/assets/textures/symbol-two.png" },
    { "background-field", "../minesweeper/assets/textures/background-field.png" }
  };

  size_t asset_count = sizeof(assets) / sizeof(assets[0]);

  if (gui_textures_load(gui, assets, asset_count) == 0)
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
        printf("width: %d height: %d\n", gui->width, gui->height);

        printf("gui_texture_render: %d\n", gui_texture_render(gui, "main", (char*[]) { NULL }, "symbol-one",
            (gui_size_t) { 0 },
            (gui_size_t) { 0 },
            (gui_size_t) { GUI_SIZE_MAX },
            (gui_size_t) { GUI_SIZE_MAX }
        ));

        printf("gui_texture_render: %d\n", gui_texture_render(gui, "main", (char*[]) { "parent", NULL }, "background-field",
            (gui_size_t) { 0 },
            (gui_size_t) { 0 },
            (gui_size_t) { GUI_SIZE_MAX },
            (gui_size_t) { GUI_SIZE_MAX }
        ));

        printf("gui_texture_render: %d\n", gui_texture_render(gui, "main", (char*[]) { "parent", NULL }, "symbol-two",
            (gui_size_t) { .type = GUI_SIZE_REL, .value.rel = 0.3 },
            (gui_size_t) { 0 },
            (gui_size_t) { .type = GUI_SIZE_REL, .value.rel = 0.5 },
            (gui_size_t) { GUI_SIZE_MAX }
        ));

        printf("gui_render: %d\n", gui_render(gui));

        start_ticks = end_ticks;
      }
    }

    gui_destroy(&gui);
  }

  sdl_quit();

  return 0;
}
