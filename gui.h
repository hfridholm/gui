/*
 * gui.h
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2025-02-26
 */

#ifndef GUI_H
#define GUI_H

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/*
 * LEFT, RIGHT, TOP, BOTTOM
 * works to index margin and padding
 */
typedef enum gui_pos_t
{
  GUI_LEFT,
  GUI_RIGHT,
  GUI_TOP,
  GUI_BOTTOM,
  GUI_CENTER
} gui_pos_t;

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

/*
 *
 */
typedef struct gui_rect_t
{
  gui_size_t width;
  gui_size_t height;
  gui_size_t margin[4];
  gui_pos_t  xpos;
  gui_pos_t  ypos;
} gui_rect_t;

/*
 *
 */

typedef struct gui_t gui_t;

typedef struct gui_window_t gui_window_t;

typedef struct gui_menu_t   gui_menu_t;

typedef struct gui_assets_t gui_assets_t;


extern gui_t* gui_create(int width, int height, char* title);

extern void   gui_destroy(gui_t** gui);


extern gui_menu_t* gui_menu_create(gui_t* gui, char* name);

extern int         gui_menu_destroy(gui_t* gui, const char* name);


extern gui_window_t* gui_menu_window_create(gui_menu_t* menu, char* name, gui_rect_t gui_rect);

extern int           gui_menu_window_destroy(gui_menu_t* menu, char* name);


extern gui_window_t* gui_window_child_create(gui_window_t* window, char* name, gui_rect_t gui_rect);

extern int           gui_window_child_destroy(gui_window_t* window, char* name);

#endif // GUI_H

/*
 * This header library file uses _IMPLEMENT guards
 *
 * If GUI_IMPLEMENT is defined, the definitions will be included
 */

#ifdef GUI_IMPLEMENT

#include <stdbool.h>

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
 * Load SDL Texture
 */
static inline SDL_Texture* sdl_texture_load(SDL_Renderer* renderer, const char* filepath)
{
  SDL_Surface* surface = IMG_Load(filepath);
  
  if (!surface)
  {
    fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());

    return NULL;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  if (!texture)
  {
    fprintf(stderr, "SDL_CreateTextureFromSurface: %s\n", SDL_GetError());

    return NULL;
  }

  return texture;
}

/*
 * Set target texture of renderer
 */
static inline int sdl_target_set(SDL_Renderer* renderer, SDL_Texture* target)
{
  int status = SDL_SetRenderTarget(renderer, target);

  if (status != 0)
  {
    fprintf(stderr, "SDL_SetRenderTarget: %s\n", SDL_GetError());
  }

  return status;
}

/*
 * Clear the supplied texture
 */
static inline int sdl_target_clear(SDL_Renderer* renderer, SDL_Texture* target)
{
  SDL_Texture* old_target = SDL_GetRenderTarget(renderer);

  // 1. Temporarly set the new target
  if (sdl_target_set(renderer, target) != 0)
  {
    return 1;
  }

  SDL_RenderClear(renderer);

  // 2. Change back to the old target
  if (sdl_target_set(renderer, old_target) != 0)
  {
    return 2;
  }

  return 0;
}

/*
 * Render SDL Texture
 */
static inline int sdl_texture_render(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect)
{
  if (!texture) return 0;

  int status = SDL_RenderCopy(renderer, texture, NULL, rect);

  if (status != 0)
  {
    fprintf(stderr, "SDL_RenderCopy: %s\n", SDL_GetError());
  }

  return status;
}

/*
 * Render texture to target texture
 */
static inline int sdl_target_texture_render(SDL_Renderer* renderer, SDL_Texture* target, SDL_Texture* texture, SDL_Rect* rect)
{
  SDL_Texture* old_target = SDL_GetRenderTarget(renderer);

  // 1. Temporarly set the new target
  if (sdl_target_set(renderer, target) != 0)
  {
    return 1;
  }

  int status = sdl_texture_render(renderer, texture, rect);

  // 2. Change back to the old target
  if (sdl_target_set(renderer, old_target) != 0)
  {
    return 2;
  }

  return (status == 0) ? 0 : 3;
}

/*
 * Resize SDL Texture
 */
static inline void sdl_texture_resize(SDL_Texture** texture, SDL_Renderer* renderer, int width, int height)
{
  sdl_texture_destroy(texture);

  *texture = sdl_texture_create(renderer, width, height);
}

/*
 * Get the absolute pixel size from gui_size
 * in case of relative size, parent_size is used in relation
 */
static inline int gui_size_abs_get(int parent_size, gui_size_t gui_size)
{
  switch (gui_size.type)
  {
    case GUI_SIZE_REL:
      return parent_size * gui_size.value.rel;

    case GUI_SIZE_ABS:
      return gui_size.value.abs;

    default: case GUI_SIZE_MAX:
      return parent_size;
  }
}

/*
 * Convert GUI Rect to SDL Rect,
 * with the help of parent width and height
 */
static inline SDL_Rect sdl_rect_create(gui_rect_t gui_rect, int parent_width, int parent_height)
{
  int abs_width  = gui_size_abs_get(parent_width,  gui_rect.width);
  int abs_height = gui_size_abs_get(parent_height, gui_rect.height);

  int left_width    = gui_size_abs_get(parent_width,  gui_rect.margin[GUI_LEFT]);
  int right_width   = gui_size_abs_get(parent_width,  gui_rect.margin[GUI_RIGHT]);
  int top_height    = gui_size_abs_get(parent_height, gui_rect.margin[GUI_TOP]);
  int bottom_height = gui_size_abs_get(parent_height, gui_rect.margin[GUI_BOTTOM]);

  int w = MIN(parent_width  - left_width - right_width,   abs_width);
  int h = MIN(parent_height - top_height - bottom_height, abs_height);

  int x;

  switch (gui_rect.xpos)
  {
    case GUI_LEFT:
      x = left_width;
      break;

    case GUI_RIGHT:
      x = parent_width - w - right_width;
      break;

    default: case GUI_CENTER:
      x = (float) (parent_width - w) / .2f;
      break;
  }

  int y;
  
  switch (gui_rect.ypos)
  {
    case GUI_TOP:
      y = top_height;
      break;

    case GUI_BOTTOM:
      y = parent_height - h - bottom_height;
      break;

    default: case GUI_CENTER:
      y = (float) (parent_height - h) / .2f;
      break;
  }
  
  return (SDL_Rect) {x, y, w, h};
}

/*
 *
 */
typedef struct gui_window_t
{
  char*          name;
  gui_rect_t     gui_rect;
  SDL_Rect       sdl_rect;
  SDL_Texture*   texture;
  gui_window_t** children;
  size_t         child_count;
  bool           is_child;
  union
  {
    gui_window_t* window; // is_child: true
    gui_menu_t*   menu;   // is_child: false
  } parent;
  gui_t*         gui;
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
 * Only destroy window (This is an internal function)
 */
static inline void _gui_window_destroy(gui_window_t** window)
{
  if (!window || !(*window)) return;

  for (size_t index = 0; index < (*window)->child_count; index++)
  {
    _gui_window_destroy(&(*window)->children[index]);
  }

  free((*window)->children);

  sdl_texture_destroy(&(*window)->texture);

  free(*window);

  *window = NULL;
}

/*
 * Get index of menu window by name
 */
static inline ssize_t gui_menu_window_index_get(gui_menu_t* menu, const char* name)
{
  if (!menu || !name)
  {
    return -1;
  }

  for (ssize_t index = 0; index < menu->window_count; index++)
  {
    gui_window_t* window = menu->windows[index];

    if (window && strcmp(window->name, name) == 0)
    {
      return index;
    }
  }

  return -1;
}

/*
 * Get index of window child by name
 */
static inline ssize_t gui_window_child_index_get(gui_window_t* window, const char* name)
{
  if (!window || !name)
  {
    return -1;
  }

  for (ssize_t index = 0; index < window->child_count; index++)
  {
    gui_window_t* child = window->children[index];

    if (child && strcmp(child->name, name) == 0)
    {
      return index;
    }
  }

  return -1;
}

/*
 * Remove child from window and destroy it
 */
int gui_window_child_destroy(gui_window_t* window, char* name)
{
  if (!window || !name)
  {
    return 1;
  }

  ssize_t child_index = gui_window_child_index_get(window, name);

  if (child_index == -1)
  {
    return 2;
  }

  _gui_window_destroy(&window->children[child_index]);


  for (size_t index = child_index; index < (window->child_count - 1); index++)
  {
    window->children[index] = window->children[index + 1];
  }


  gui_window_t** temp_children = realloc(window->children, sizeof(gui_window_t*) * (window->child_count - 1));

  if (!temp_children)
  {
    // Here, the child is destroyed, but the children array isn't shrunk
    return 3;
  }

  window->children = temp_children;

  return 0;
}

/*
 * Remove window from menu and destroy it
 */
int gui_menu_window_destroy(gui_menu_t* menu, char* name)
{
  if (!menu || !name)
  {
    return 1;
  }

  ssize_t window_index = gui_menu_window_index_get(menu, name);

  if (window_index == -1)
  {
    return 2;
  }

  _gui_window_destroy(&menu->windows[window_index]);


  for (size_t index = window_index; index < (menu->window_count - 1); index++)
  {
    menu->windows[index] = menu->windows[index + 1];
  }


  gui_window_t** temp_windows = realloc(menu->windows, sizeof(gui_window_t*) * (menu->window_count - 1));

  if (!temp_windows)
  {
    // Here, the window is destroyed, but the windows array isn't shrunk
    return 3;
  }

  menu->windows = temp_windows;

  return 0;
}

/*
 * Create window and add it to menu
 */
gui_window_t* gui_menu_window_create(gui_menu_t* menu, char* name, gui_rect_t gui_rect)
{
  if (!menu || !name)
  {
    return NULL;
  }

  gui_t* gui = menu->gui;

  if (!gui)
  {
    return NULL;
  }

  gui_window_t* window = malloc(sizeof(gui_window_t));

  if (!window)
  {
    return NULL;
  }

  memset(window, 0, sizeof(gui_window_t));

  window->name = name;
  window->gui  = gui;

  window->is_child = false;
  window->parent.menu = menu;

  window->gui_rect = gui_rect;

  window->sdl_rect = sdl_rect_create(window->gui_rect, gui->width, gui->height);

  window->texture = sdl_texture_create(gui->renderer, window->sdl_rect.w, window->sdl_rect.h);

  if (!window->texture)
  {
    free(window);

    return NULL;
  }


  gui_window_t** temp_windows = realloc(menu->windows, sizeof(gui_window_t*) * (menu->window_count + 1));

  if (!temp_windows)
  {
    _gui_window_destroy(&window);

    return NULL;
  }

  menu->windows = temp_windows;

  menu->windows[menu->window_count++] = window;

  return window;
}

/*
 * Create child window and add it to window
 */
gui_window_t* gui_window_child_create(gui_window_t* window, char* name, gui_rect_t gui_rect)
{
  if (!window || !name)
  {
    return NULL;
  }

  gui_t* gui = window->gui;

  if (!gui)
  {
    return NULL;
  }

  gui_window_t* child = malloc(sizeof(gui_window_t));

  if (!child)
  {
    return NULL;
  }

  memset(child, 0, sizeof(gui_window_t));

  child->name = name;
  child->gui  = gui;

  child->is_child = true;
  child->parent.window = window;

  child->gui_rect = gui_rect;

  child->sdl_rect = sdl_rect_create(child->gui_rect, window->sdl_rect.w, window->sdl_rect.h);

  child->texture = sdl_texture_create(gui->renderer, child->sdl_rect.w, child->sdl_rect.h);

  if (!child->texture)
  {
    free(child);

    return NULL;
  }


  gui_window_t** temp_children = realloc(window->children, sizeof(gui_window_t*) * (window->child_count + 1));

  if (!temp_children)
  {
    _gui_window_destroy(&child);

    return NULL;
  }

  window->children = temp_children;

  window->children[window->child_count++] = child;

  return child;
}

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
    _gui_window_destroy(&(*menu)->windows[index]);
  }

  free((*menu)->windows);

  sdl_texture_destroy(&(*menu)->texture);

  free(*menu);

  *menu = NULL;
}

/*
 * Create menu and add it to GUI
 */
gui_menu_t* gui_menu_create(gui_t* gui, char* name)
{
  if (!gui || !name)
  {
    return NULL;
  }

  gui_menu_t* menu = _gui_menu_create(gui, name);

  if (!menu)
  {
    return NULL;
  }

  gui_menu_t** temp_menus = realloc(gui->menus, sizeof(gui_menu_t*) * (gui->menu_count + 1));

  if (!temp_menus)
  {
    _gui_menu_destroy(&menu);

    return NULL;
  }

  gui->menus = temp_menus;

  gui->menus[gui->menu_count++] = menu;

  return menu;
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

  free((*gui)->menus);


  sdl_renderer_destroy(&(*gui)->renderer);

  sdl_window_destroy(&(*gui)->window);

  free(*gui);

  *gui = NULL;
}

#endif // GUI_IMPLEMENT
