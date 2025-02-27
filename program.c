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
  gui_clear(gui);

  printf("width: %d height: %d\n", gui->width, gui->height);

  printf("gui_texture_render: %d\n", gui_texture_render(gui, "first", (char*[]) { NULL }, "symbol-one",
      (gui_size_t) { 0 },
      (gui_size_t) { 0 },
      (gui_size_t) { GUI_SIZE_MAX },
      (gui_size_t) { GUI_SIZE_MAX }
  ));

  printf("gui_texture_render: %d\n", gui_texture_render(gui, "first", (char*[]) { "parent", NULL }, "background-field",
      (gui_size_t) { 0 },
      (gui_size_t) { 0 },
      (gui_size_t) { GUI_SIZE_MAX },
      (gui_size_t) { GUI_SIZE_MAX }
  ));

  printf("gui_texture_render: %d\n", gui_texture_render(gui, "first", (char*[]) { "parent", NULL }, "symbol-two",
      (gui_size_t) { .type = GUI_SIZE_REL, .value.rel = 0.3 },
      (gui_size_t) { 0 },
      (gui_size_t) { .type = GUI_SIZE_REL, .value.rel = 0.5 },
      (gui_size_t) { GUI_SIZE_MAX }
  ));

  printf("gui_texture_render: %d\n", gui_texture_render(gui, "first", (char*[]) { "second-button", NULL }, "square-exploded",
      (gui_size_t) { 0 },
      (gui_size_t) { 0 },
      (gui_size_t) { GUI_SIZE_MAX },
      (gui_size_t) { GUI_SIZE_MAX }
  ));
  
  if (gui->curr_window && strcmp(gui->curr_window->name, "second-button") == 0)
  {
    gui_text_render(gui, "first", (char*[]) { "second-button", NULL }, "Click?",
      (gui_rect_t) {
        .width = (gui_size_t) {
          .type = GUI_SIZE_MAX
        },
        .height = (gui_size_t)
        {
          .type = GUI_SIZE_NONE
        }
      }, "default", (SDL_Color) {0, 255, 0}
    );
  }
  else
  {
    gui_text_render(gui, "first", (char*[]) { "second-button", NULL }, "SEconD",
      (gui_rect_t) {
        .width = (gui_size_t) {
          .type = GUI_SIZE_MAX
        },
        .height = (gui_size_t)
        {
          .type = GUI_SIZE_NONE
        }
      }, "default", (SDL_Color) {0, 255, 255}
    );
  }

  printf("gui_text_render: %d\n", gui_text_render(gui, "second", (char*[]) { "first-button", NULL }, "FiRSt",
    (gui_rect_t) {
      .width = (gui_size_t) {
        .type = GUI_SIZE_MAX
      },
      .height = (gui_size_t)
      {
        .type = GUI_SIZE_NONE
      }
    }, "default", (SDL_Color) {0, 255, 0}
  ));

  gui_window_border_render(gui->curr_window, (sdl_border_t) {
    .thickness = 10,
    .opacity = 255,
    .color = (SDL_Color) { 255, 0, 255 }
  });
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
void* menu_button_handle(gui_t* gui, gui_window_t* window)
{
  if (window)
  {
    if (strcmp(window->name, "second-button") == 0)
    {
      gui_active_menu_set(gui, "second");

      game_render(gui);
    }

    if (strcmp(window->name, "first-button") == 0)
    {
      gui_active_menu_set(gui, "first");

      game_render(gui);
    }
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
 *
 */
void* window_enter_event_handle(gui_t* gui, gui_window_t* window)
{
  game_render(gui);
}

/*
 *
 */
void* window_exit_event_handle(gui_t* gui, gui_window_t* window)
{
  game_render(gui);
}

/*
 * Create gui events and add handlers
 */
void gui_events_create(gui_t* gui)
{
  printf("gui_event_create: %d\n", gui_event_create(gui, "mouse-down-left",
    (gui_event_handler_t) {
      .type = GUI_EVENT_HANDLER_WINDOW,
      .handler.window = &menu_button_handle
    })
  );

  printf("gui_event_create: %d\n", gui_event_create(gui, "key-down",
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

  printf("gui_event_create: %d\n", gui_event_create(gui, "window-enter",
    (gui_event_handler_t) {
      .type = GUI_EVENT_HANDLER_WINDOW,
      .handler.window = window_enter_event_handle
    })
  );

  printf("gui_event_create: %d\n", gui_event_create(gui, "window-exit",
    (gui_event_handler_t) {
      .type = GUI_EVENT_HANDLER_WINDOW,
      .handler.window = window_exit_event_handle
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
void gui_menu_first_setup(gui_t* gui)
{
  gui_menu_t* menu = gui_menu_create(gui, "first");

  gui_menu_window_create(menu, "parent",
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
      .xpos = GUI_POS_RIGHT,
      .ypos = GUI_POS_CENTER
    },
    (sdl_border_t)
    {
      .thickness = 10,
      .opacity = 255,
      .color = (SDL_Color) {255, 0, 255}
    }
  );

  gui_menu_window_create(menu, "second-button",
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
      .xpos = GUI_POS_RIGHT,
      .ypos = GUI_POS_TOP,
      .top = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 10
      },
      .left = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 10
      }
    },
    (sdl_border_t) { 0 }
  );
}

/*
 *
 */
void gui_menu_second_setup(gui_t* gui)
{
  gui_menu_t* menu = gui_menu_create(gui, "second");

  gui_menu_window_create(menu, "first-button",
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
      .xpos = GUI_POS_LEFT,
      .ypos = GUI_POS_TOP,
      .top = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 10
      },
      .left = (gui_size_t) {
        .type = GUI_SIZE_ABS,
        .value.abs = 10
      }
    },
    (sdl_border_t) { 0 }
  );
}

/*
 * Setup gui by, creating menus and windows
 */
void gui_setup(gui_t* gui)
{
  gui_menu_first_setup(gui);

  gui_menu_second_setup(gui);


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

    gui_active_menu_set(gui, "first");

    game_render(gui);

    gui_start(gui, 1);

    gui_destroy(&gui);
  }

  sdl_quit();

  return 0;
}
