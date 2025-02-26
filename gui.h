/*
 * gui.h
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2025-02-26
 */

#ifndef GUI_H
#define GUI_H

/*
 *
 */
typedef enum gui_size_type_t
{
  GUI_SIZE_REL, // Relative size to parent
  GUI_SIZE_ABS, // Absolute size in pixels
  GUI_SIZE_MAX  // Max available size
} gui_size_type_t;

/*
 *
 */
typedef struct gui_size_t
{
  gui_size_type_t type;
  union
  {
    float rel;
    int   abs;
  } value;
} gui_size_t;

typedef struct gui_t gui_t;

extern gui_t* gui_create(int width, int height, char* title);

extern void   gui_destroy(gui_t** gui);


extern int gui_menu_create(gui_t* gui, char* name);

extern int gui_menu_destroy(gui_t* gui, const char* name);

#endif // GUI_H

/*
 * This header library file uses _IMPLEMENT guards
 *
 * If GUI_IMPLEMENT is defined, the definitions will be included
 */

#ifdef GUI_IMPLEMENT

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/*
 * Initialize SDL drivers
 */
static inline int sdl_drivers_init(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());

    return 1;
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
  {
    fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());

    return 2;
  }

  if (TTF_Init() == -1)
  {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());

    return 3;
  }

  if (Mix_Init(0) != 0)
  {
    fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());

    return 4;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
  {
    fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());

    return 5;
  }

  return 0;
}

/*
 * Quit SDL drivers
 */
static inline void sdl_drivers_quit(void)
{
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
  Mix_Quit();

  Mix_CloseAudio();
}

/*
 * Create SDL Window
 */
static inline SDL_Window* sdl_window_create(int width, int height, char* title)
{
  SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (!window)
  {
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());

    return NULL;
  }

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  return window;
}

/*
 * Destroy SDL Window
 */
static inline void sdl_window_destroy(SDL_Window** window)
{
  if (!window || !(*window)) return;

  SDL_DestroyWindow(*window);

  *window = NULL;
}

/*
 * Create SDL Renderer
 */
static inline SDL_Renderer* sdl_renderer_create(SDL_Window* window)
{
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  if (!renderer)
  {
    fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());

    return NULL;
  }

  return renderer;
}

/*
 * Destroy SDL Renderer
 */
static inline void sdl_renderer_destroy(SDL_Renderer** renderer)
{
  if (!renderer || !(*renderer)) return;

  SDL_DestroyRenderer(*renderer);

  *renderer = NULL;
}

/*
 * Destroy SDL Texture
 */
static inline void sdl_texture_destroy(SDL_Texture** texture)
{
  if (!texture || !(*texture)) return;

  SDL_DestroyTexture(*texture);

  *texture = NULL;
}

/*
 * Create SDL Texture
 */
static inline SDL_Texture* sdl_texture_create(SDL_Renderer* renderer, int width, int height)
{
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

  if (!texture)
  {
    fprintf(stderr, "SDL_CreateTexture: %s\n", SDL_GetError());

    return NULL;
  }

  if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0)
  {
    fprintf(stderr, "SDL_SetTextureBlendMode: %s", SDL_GetError());

    SDL_DestroyTexture(texture);

    return NULL;
  }

  return texture;
}

/*
 *
 */

typedef struct gui_window_t gui_window_t;

typedef struct gui_menu_t   gui_menu_t;

typedef struct gui_assets_t gui_assets_t;

/*
 *
 */
typedef struct gui_window_t
{
  char*          name;
  int            margin[4];
  int            padding[4];
  SDL_Rect       rect;
  SDL_Texture*   texture;
  gui_window_t** children;
  size_t         child_count;
  gui_window_t*  parent;
  gui_menu_t*    menu;
} gui_window_t;

/*
 *
 */
typedef struct gui_menu_t
{
  char*          name;
  SDL_Texture*   texture;
  gui_window_t** windows;
  size_t         window_count;
  gui_t*         gui;
} gui_menu_t;

/*
 *
 */
typedef struct gui_texture_t
{
  char*        name;
  SDL_Texture* texture;
} gui_texture_t;

/*
 *
 */
typedef struct gui_font_t
{
  char*     name;
  TTF_Font* font;
} gui_font_t;

/*
 *
 */
typedef struct gui_chunk_t
{
  char*      name;
  Mix_Chunk* chunk;
} gui_chunk_t;

/*
 *
 */
typedef struct gui_music_t
{
  char*      name;
  Mix_Music* music;
} gui_music_t;

/*
 *
 */
typedef struct gui_assets_t
{
  gui_texture_t** textures;
  size_t          texture_count;

  gui_font_t**    fonts;
  size_t          font_count;

  gui_chunk_t**   chunks;
  size_t          chunk_count;

  gui_music_t**   musics;
  size_t          music_count;
} gui_assets_t;

/*
 *
 */
typedef struct gui_t
{
  SDL_Window*   window;
  SDL_Renderer* renderer;
  char*         title;
  int           width;
  int           height;
  gui_menu_t**  menus;
  size_t        menu_count;
  char*         menu_name;
  gui_assets_t* assets;
} gui_t;

/*
 * Create gui
 */
gui_t* gui_create(int width, int height, char* title)
{
  gui_t* gui = malloc(sizeof(gui_t));

  if (!gui)
  {
    return NULL;
  }

  memset(gui, 0, sizeof(gui_t));

  gui->window = sdl_window_create(width, height, title);

  if (!gui->window)
  {
    free(gui);

    return NULL;
  }

  gui->renderer = sdl_renderer_create(gui->window);

  if (!gui->renderer)
  {
    sdl_window_destroy(&gui->window);

    free(gui);

    return NULL;
  }

  gui->title  = title;
  gui->width  = width;
  gui->height = height;

  return gui;
}

/*
 * Only create menu (This is an internal function)
 */
static inline gui_menu_t* _gui_menu_create(gui_t* gui, char* name)
{
  gui_menu_t* menu = malloc(sizeof(gui_menu_t));

  memset(menu, 0, sizeof(gui_menu_t));

  menu->name = name;
  menu->gui  = gui;

  menu->texture = sdl_texture_create(gui->renderer, gui->width, gui->height);

  if (!menu->texture)
  {
    free(menu);

    return NULL;
  }

  return menu;
}

/*
 * Only destroy menu (This is an internal function)
 */
static inline void _gui_menu_destroy(gui_menu_t** menu)
{
  if (!menu || !(*menu)) return;

  for (size_t index = 0; index < (*menu)->window_count; index++)
  {
    // _gui_window_destroy(&(*menu)->windows[index]);
  }

  free((*menu)->windows);

  sdl_texture_destroy(&(*menu)->texture);

  free(*menu);

  *menu = NULL;
}

/*
 * Create menu and add it to GUI
 */
int gui_menu_create(gui_t* gui, char* name)
{
  if (!gui || !name)
  {
    return 1;
  }

  gui_menu_t* menu = _gui_menu_create(gui, name);

  if (!menu)
  {
    _gui_menu_destroy(&menu);

    return 2;
  }

  gui_menu_t** temp_menus = realloc(gui->menus, sizeof(gui_menu_t*) * (gui->menu_count + 1));

  if (!temp_menus)
  {
    sdl_texture_destroy(&menu->texture);

    free(menu);

    return 3;
  }

  gui->menus = temp_menus;

  gui->menus[gui->menu_count++] = menu;

  return 0;
}

/*
 * Get menu by name
 */
static inline gui_menu_t* gui_menu_get(gui_t* gui, const char* name)
{
  if (!gui || !name)
  {
    return NULL;
  }

  for (size_t index = 0; index < gui->menu_count; index++)
  {
    gui_menu_t* menu = gui->menus[index];

    if (menu && strcmp(menu->name, name) == 0)
    {
      return menu;
    }
  }

  return NULL;
}

/*
 * Get index of menu by name
 */
static inline ssize_t gui_menu_index_get(gui_t* gui, const char* name)
{
  if (!gui || !name)
  {
    return -1;
  }

  for (ssize_t index = 0; index < gui->menu_count; index++)
  {
    gui_menu_t* menu = gui->menus[index];

    if (menu && strcmp(menu->name, name) == 0)
    {
      return index;
    }
  }

  return -1;
}

/*
 * Remove menu from GUI and destroy it
 */
int gui_menu_destroy(gui_t* gui, const char* name)
{
  if (!gui || !name)
  {
    return 1;
  }

  ssize_t menu_index = gui_menu_index_get(gui, name);

  if (menu_index == -1)
  {
    return 2;
  }

  _gui_menu_destroy(&gui->menus[menu_index]);


  for (size_t index = menu_index; index < (gui->menu_count - 1); index++)
  {
    gui->menus[index] = gui->menus[index + 1];
  }


  gui_menu_t** temp_menus = realloc(gui->menus, sizeof(gui_menu_t*) * (gui->menu_count - 1));

  if (!temp_menus)
  {
    // Here, the menu is destroyed, but the menus array isn't shrunk
    return 3;
  }

  gui->menus = temp_menus;

  return 0;
}

/*
 * Destroy gui
 */
void gui_destroy(gui_t** gui)
{
  if (!gui || !(*gui)) return;

  for (size_t index = 0; index < (*gui)->menu_count; index++)
  {
    _gui_menu_destroy(&(*gui)->menus[index]);
  }

  if((*gui)->menus) free((*gui)->menus);


  sdl_renderer_destroy(&(*gui)->renderer);

  sdl_window_destroy(&(*gui)->window);

  free(*gui);

  *gui = NULL;
}

#endif // GUI_IMPLEMENT
