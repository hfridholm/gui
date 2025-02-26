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

typedef struct gui_window_t gui_window_t;

typedef struct gui_menu_t   gui_menu_t;

typedef struct gui_screen_t gui_screen_t;

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
  gui_screen_t*  screen;
} gui_menu_t;

/*
 *
 */
typedef struct gui_screen_t
{
  char*         title;
  SDL_Window*   window;
  SDL_Renderer* renderer;
  int           width;
  int           height;
  gui_menu_t**  menus;
  size_t        menu_count;
  char*         menu_name;
} gui_screen_t;

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
  gui_screen_t* screen;
  gui_assets_t* assets;
} gui_t;

#endif // GUI_IMPLEMENT
