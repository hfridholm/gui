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

/*
 *
 */
void game_render(gui_t* gui)
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

  printf("gui_texture_render: %d\n", gui_texture_render(gui, "main", (char*[]) { "back-button", NULL }, "square-exploded",
      (gui_size_t) { 0 },
      (gui_size_t) { 0 },
      (gui_size_t) { GUI_SIZE_MAX },
      (gui_size_t) { GUI_SIZE_MAX }
  ));
  
  printf("gui_text_render: %d\n", gui_text_render(gui, "main", (char*[]) { "back-button", NULL }, "BACK",
    (gui_rect_t) {
      .width = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 50
      },
      .height = (gui_size_t)
      {
        .type = GUI_SIZE_NONE
      }
    }, "default", (SDL_Color) {255, 255, 255}
  ));
}

/*
 *
 */
void* screen_resize_handle(gui_t* gui, int width, int height)
{
  printf("Screen resized (%d x %d)\n", width, height);

  game_render(gui);

  return NULL;
}

/*
 *
 */
void* square_click_handle(gui_t* gui, int x, int y)
{
  printf("Square clicked (%d, %d)\n", x, y);

  gui_window_t* window = gui_x_and_y_window_get(gui, x, y);

  if (window)
  {
    int window_x = x - window->sdl_rect.x;
    int window_y = y - window->sdl_rect.y;

    printf("Mouse down left window (%s) x: %d y: %d\n", window->name, window_x, window_y);

    if (strcmp(window->name, "back-button") == 0)
    {
      gui_stop(gui);
    }
  }
  else
  {
    printf("Mouse down left menu x: %d y: %d\n", x, y);
  }

  return NULL;
}

/*
 *
 */
void* key_down_handle(gui_t* gui, int key)
{
  printf("key down: %d\n", key);

  return NULL;
}

/*
 * Create gui events and add handlers
 */
void gui_events_create(gui_t* gui)
{
  printf("gui_event_create: %d\n", gui_event_create(gui, "mouse-down-left",
    (gui_event_handler_t) {
      .type = GUI_EVENT_HANDLER_MOUSE,
      .handler.mouse = &square_click_handle
    })
  );

  printf("gui_event_create: %d\n", gui_event_create(gui, "mouse-down-left",
    (gui_event_handler_t) {
      .type = GUI_EVENT_HANDLER_KEY,
      .handler.key = &key_down_handle
    })
  );

  printf("gui_event_create: %d\n", gui_event_create(gui, "resize",
    (gui_event_handler_t) {
      .type = GUI_EVENT_HANDLER_RESIZE,
      .handler.resize = &screen_resize_handle
    })
  );
}

/*
 * Load all assets for gui
 */
void gui_assets_load(gui_t* gui)
{
  gui_asset_t textures[] = {
    { "symbol-one", "../minesweeper/assets/textures/symbol-one.png" },
    { "symbol-two", "../minesweeper/assets/textures/symbol-two.png" },
    { "square-exploded", "../minesweeper/assets/textures/square-exploded.png" },
    { "background-field", "../minesweeper/assets/textures/background-field.png" }
  };

  size_t texture_count = sizeof(textures) / sizeof(gui_asset_t);

  if (gui_textures_load(gui, textures, texture_count) == 0)
  {
    printf("Loaded textures\n");
  }


  gui_asset_t fonts[] = {
    { "default", "../minesweeper/assets/fonts/font.ttf" }
  };

  size_t font_count = sizeof(fonts) / sizeof(gui_asset_t);

  if (gui_fonts_load(gui, fonts, font_count) == 0)
  {
    printf("Loaded fonts\n");
  }
}

/*
 * Create menus and windows in gui
 */
void gui_windows_create(gui_t* gui)
{
  gui_menu_t* main_menu = gui_menu_create(gui, "main");

  gui_menu_window_create(main_menu, "parent",
    (gui_rect_t) {
      .width = (gui_size_t) {
        .type = GUI_SIZE_REL,
        .value.rel = 0.5
      },
      .height = (gui_size_t)
      {
        .type = GUI_SIZE_REL,
        .value.rel = 0.4
      },
      .xpos = GUI_RIGHT,
      .ypos = GUI_CENTER
    }
  );

  gui_menu_window_create(main_menu, "back-button",
    (gui_rect_t) {
      .width = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 100
      },
      .height = (gui_size_t)
      {
        .type = GUI_SIZE_ABS,
        .value.abs = 100
      },
      .xpos = GUI_LEFT,
      .ypos = GUI_TOP,
      .margin[GUI_TOP] = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 10
      },
      .margin[GUI_LEFT] = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 10
      }
    }
  );
}

/*
 * Setup gui by, creating menus and windows
 */
void gui_setup(gui_t* gui)
{
  gui_windows_create(gui);

  gui_assets_load(gui);

  gui_events_create(gui);
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

  gui_t* gui = gui_create(800, 600, "Minesweeper");

  if (gui)
  {
    gui_setup(gui);

    game_render(gui);

    gui_start(gui, 1);

    gui_destroy(&gui);
  }

  sdl_quit();

  return 0;
}
