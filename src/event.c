// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/event.h"

#include "SDL2/SDL.h"

static void abort_on_null(const void* ptr);
static ge_keycode_t sdl_scancode_to_keycode(SDL_Scancode scancode);

bool ge_fill_event(ge_event_t* restrict event, void* restrict internal_event)
{
  abort_on_null(event);
  abort_on_null(internal_event);
  // Clear the event completely
  memset(event, 0, sizeof(ge_event_t));
  event->type = GE_EVENT_NONE;
  // Cast the internal event back to the real SDL event
  SDL_Event* sdl_event = (SDL_Event*) internal_event;
  // Fill the event for the events we support
  if (sdl_event->type == SDL_KEYDOWN) {
    event->type = GE_EVENT_KEYDOWN;
    event->keyup_data.timestamp_ms = sdl_event->key.timestamp;
    event->keyup_data.keycode = sdl_scancode_to_keycode(sdl_event->key.keysym.scancode);
    event->keyup_data.is_repeat = sdl_event->key.repeat;
    return true;
  }
  else if (sdl_event->type == SDL_KEYUP) {
    event->type = GE_EVENT_KEYUP;
    event->keyup_data.timestamp_ms = sdl_event->key.timestamp;
    event->keyup_data.keycode = sdl_scancode_to_keycode(sdl_event->key.keysym.scancode);
    return true;
  }
  return false;
}

const char* ge_keycode_to_str(ge_keycode_t keycode)
{
  switch (keycode) {
  case GE_KEYCODE_A:
    return "A";
  case GE_KEYCODE_B:
    return "B";
  case GE_KEYCODE_C:
    return "C";
  case GE_KEYCODE_D:
    return "D";
  case GE_KEYCODE_E:
    return "E";
  case GE_KEYCODE_F:
    return "F";
  case GE_KEYCODE_G:
    return "G";
  case GE_KEYCODE_H:
    return "H";
  case GE_KEYCODE_I:
    return "I";
  case GE_KEYCODE_J:
    return "J";
  case GE_KEYCODE_K:
    return "K";
  case GE_KEYCODE_L:
    return "L";
  case GE_KEYCODE_M:
    return "M";
  case GE_KEYCODE_N:
    return "N";
  case GE_KEYCODE_O:
    return "O";
  case GE_KEYCODE_P:
    return "P";
  case GE_KEYCODE_Q:
    return "Q";
  case GE_KEYCODE_R:
    return "R";
  case GE_KEYCODE_S:
    return "S";
  case GE_KEYCODE_T:
    return "T";
  case GE_KEYCODE_U:
    return "U";
  case GE_KEYCODE_V:
    return "V";
  case GE_KEYCODE_W:
    return "W";
  case GE_KEYCODE_X:
    return "X";
  case GE_KEYCODE_Y:
    return "Y";
  case GE_KEYCODE_Z:
    return "Z";
  case GE_KEYCODE_0:
    return "0";
  case GE_KEYCODE_1:
    return "1";
  case GE_KEYCODE_2:
    return "2";
  case GE_KEYCODE_3:
    return "3";
  case GE_KEYCODE_4:
    return "4";
  case GE_KEYCODE_5:
    return "5";
  case GE_KEYCODE_6:
    return "6";
  case GE_KEYCODE_7:
    return "7";
  case GE_KEYCODE_8:
    return "8";
  case GE_KEYCODE_9:
    return "9";
  case GE_KEYCODE_UP:
    return "UP";
  case GE_KEYCODE_DOWN:
    return "DOWN";
  case GE_KEYCODE_LEFT:
    return "LEFT";
  case GE_KEYCODE_RIGHT:
    return "RIGHT";
  case GE_KEYCODE_APOSTROPHE:
    return "APOSTROPHE";
  case GE_KEYCODE_BACKSLASH:
    return "BACKSLASH";
  case GE_KEYCODE_COMMA:
    return "COMMA";
  case GE_KEYCODE_EQUALS:
    return "EQUALS";
  case GE_KEYCODE_GRAVE:
    return "GRAVE";
  case GE_KEYCODE_LEFTBRACKET:
    return "LEFTBRACKET";
  case GE_KEYCODE_MINUS:
    return "MINUS";
  case GE_KEYCODE_PERIOD:
    return "PERIOD";
  case GE_KEYCODE_RIGHTBRACKET:
    return "RIGHTBRACKET";
  case GE_KEYCODE_SEMICOLON:
    return "SEMICOLON";
  case GE_KEYCODE_SLASH:
    return "SLASH";
  case GE_KEYCODE_SPACE:
    return "SPACE";
  case GE_KEYCODE_TAB:
    return "TAB";
  case GE_KEYCODE_F1:
    return "F1";
  case GE_KEYCODE_F2:
    return "F2";
  case GE_KEYCODE_F3:
    return "F3";
  case GE_KEYCODE_F4:
    return "F4";
  case GE_KEYCODE_F5:
    return "F5";
  case GE_KEYCODE_F6:
    return "F6";
  case GE_KEYCODE_F7:
    return "F7";
  case GE_KEYCODE_F8:
    return "F8";
  case GE_KEYCODE_F9:
    return "F9";
  case GE_KEYCODE_F10:
    return "F10";
  case GE_KEYCODE_F11:
    return "F11";
  case GE_KEYCODE_F12:
    return "F12";
  case GE_KEYCODE_LALT:
    return "LALT";
  case GE_KEYCODE_LCTRL:
    return "LCTRL";
  case GE_KEYCODE_LGUI:
    return "LGUI";
  case GE_KEYCODE_LSHIFT:
    return "LSHIFT";
  case GE_KEYCODE_RALT:
    return "RALT";
  case GE_KEYCODE_RCTRL:
    return "RCTRL";
  case GE_KEYCODE_RGUI:
    return "RGUI";
  case GE_KEYCODE_RSHIFT:
    return "RSHIFT";
  case GE_KEYCODE_BACKSPACE:
    return "BACKSPACE";
  case GE_KEYCODE_CAPSLOCK:
    return "CAPSLOCK";
  case GE_KEYCODE_DELETE:
    return "DELETE";
  case GE_KEYCODE_END:
    return "END";
  case GE_KEYCODE_ESCAPE:
    return "ESCAPE";
  case GE_KEYCODE_HOME:
    return "HOME";
  case GE_KEYCODE_INSERT:
    return "INSERT";
  case GE_KEYCODE_PAGEDOWN:
    return "PAGEDOWN";
  case GE_KEYCODE_PAGEUP:
    return "PAGEUP";
  case GE_KEYCODE_RETURN:
    return "RETURN";
  default:
    return "UNKNOWN";
  }
}

static void abort_on_null(const void* ptr)
{
  if (ptr == NULL) {
    abort();
  }
}

static ge_keycode_t sdl_scancode_to_keycode(SDL_Scancode scancode)
{
  switch (scancode) {
  case SDL_SCANCODE_A:
    return GE_KEYCODE_A;
  case SDL_SCANCODE_B:
    return GE_KEYCODE_B;
  case SDL_SCANCODE_C:
    return GE_KEYCODE_C;
  case SDL_SCANCODE_D:
    return GE_KEYCODE_D;
  case SDL_SCANCODE_E:
    return GE_KEYCODE_E;
  case SDL_SCANCODE_F:
    return GE_KEYCODE_F;
  case SDL_SCANCODE_G:
    return GE_KEYCODE_G;
  case SDL_SCANCODE_H:
    return GE_KEYCODE_H;
  case SDL_SCANCODE_I:
    return GE_KEYCODE_I;
  case SDL_SCANCODE_J:
    return GE_KEYCODE_J;
  case SDL_SCANCODE_K:
    return GE_KEYCODE_K;
  case SDL_SCANCODE_L:
    return GE_KEYCODE_L;
  case SDL_SCANCODE_M:
    return GE_KEYCODE_M;
  case SDL_SCANCODE_N:
    return GE_KEYCODE_N;
  case SDL_SCANCODE_O:
    return GE_KEYCODE_O;
  case SDL_SCANCODE_P:
    return GE_KEYCODE_P;
  case SDL_SCANCODE_Q:
    return GE_KEYCODE_Q;
  case SDL_SCANCODE_R:
    return GE_KEYCODE_R;
  case SDL_SCANCODE_S:
    return GE_KEYCODE_S;
  case SDL_SCANCODE_T:
    return GE_KEYCODE_T;
  case SDL_SCANCODE_U:
    return GE_KEYCODE_U;
  case SDL_SCANCODE_V:
    return GE_KEYCODE_V;
  case SDL_SCANCODE_W:
    return GE_KEYCODE_W;
  case SDL_SCANCODE_X:
    return GE_KEYCODE_X;
  case SDL_SCANCODE_Y:
    return GE_KEYCODE_Y;
  case SDL_SCANCODE_Z:
    return GE_KEYCODE_Z;
  case SDL_SCANCODE_0:
    return GE_KEYCODE_0;
  case SDL_SCANCODE_1:
    return GE_KEYCODE_1;
  case SDL_SCANCODE_2:
    return GE_KEYCODE_2;
  case SDL_SCANCODE_3:
    return GE_KEYCODE_3;
  case SDL_SCANCODE_4:
    return GE_KEYCODE_4;
  case SDL_SCANCODE_5:
    return GE_KEYCODE_5;
  case SDL_SCANCODE_6:
    return GE_KEYCODE_6;
  case SDL_SCANCODE_7:
    return GE_KEYCODE_7;
  case SDL_SCANCODE_8:
    return GE_KEYCODE_8;
  case SDL_SCANCODE_9:
    return GE_KEYCODE_9;
  case SDL_SCANCODE_UP:
    return GE_KEYCODE_UP;
  case SDL_SCANCODE_DOWN:
    return GE_KEYCODE_DOWN;
  case SDL_SCANCODE_LEFT:
    return GE_KEYCODE_LEFT;
  case SDL_SCANCODE_RIGHT:
    return GE_KEYCODE_RIGHT;
  case SDL_SCANCODE_APOSTROPHE:
    return GE_KEYCODE_APOSTROPHE;
  case SDL_SCANCODE_BACKSLASH:
    return GE_KEYCODE_BACKSLASH;
  case SDL_SCANCODE_COMMA:
    return GE_KEYCODE_COMMA;
  case SDL_SCANCODE_EQUALS:
    return GE_KEYCODE_EQUALS;
  case SDL_SCANCODE_GRAVE:
    return GE_KEYCODE_GRAVE;
  case SDL_SCANCODE_LEFTBRACKET:
    return GE_KEYCODE_LEFTBRACKET;
  case SDL_SCANCODE_MINUS:
    return GE_KEYCODE_MINUS;
  case SDL_SCANCODE_PERIOD:
    return GE_KEYCODE_PERIOD;
  case SDL_SCANCODE_RIGHTBRACKET:
    return GE_KEYCODE_RIGHTBRACKET;
  case SDL_SCANCODE_SEMICOLON:
    return GE_KEYCODE_SEMICOLON;
  case SDL_SCANCODE_SLASH:
    return GE_KEYCODE_SLASH;
  case SDL_SCANCODE_SPACE:
    return GE_KEYCODE_SPACE;
  case SDL_SCANCODE_TAB:
    return GE_KEYCODE_TAB;
  case SDL_SCANCODE_F1:
    return GE_KEYCODE_F1;
  case SDL_SCANCODE_F2:
    return GE_KEYCODE_F2;
  case SDL_SCANCODE_F3:
    return GE_KEYCODE_F3;
  case SDL_SCANCODE_F4:
    return GE_KEYCODE_F4;
  case SDL_SCANCODE_F5:
    return GE_KEYCODE_F5;
  case SDL_SCANCODE_F6:
    return GE_KEYCODE_F6;
  case SDL_SCANCODE_F7:
    return GE_KEYCODE_F7;
  case SDL_SCANCODE_F8:
    return GE_KEYCODE_F8;
  case SDL_SCANCODE_F9:
    return GE_KEYCODE_F9;
  case SDL_SCANCODE_F10:
    return GE_KEYCODE_F10;
  case SDL_SCANCODE_F11:
    return GE_KEYCODE_F11;
  case SDL_SCANCODE_F12:
    return GE_KEYCODE_F12;
  case SDL_SCANCODE_LALT:
    return GE_KEYCODE_LALT;
  case SDL_SCANCODE_LCTRL:
    return GE_KEYCODE_LCTRL;
  case SDL_SCANCODE_LGUI:
    return GE_KEYCODE_LGUI;
  case SDL_SCANCODE_LSHIFT:
    return GE_KEYCODE_LSHIFT;
  case SDL_SCANCODE_RALT:
    return GE_KEYCODE_RALT;
  case SDL_SCANCODE_RCTRL:
    return GE_KEYCODE_RCTRL;
  case SDL_SCANCODE_RGUI:
    return GE_KEYCODE_RGUI;
  case SDL_SCANCODE_RSHIFT:
    return GE_KEYCODE_RSHIFT;
  case SDL_SCANCODE_BACKSPACE:
    return GE_KEYCODE_BACKSPACE;
  case SDL_SCANCODE_CAPSLOCK:
    return GE_KEYCODE_CAPSLOCK;
  case SDL_SCANCODE_DELETE:
    return GE_KEYCODE_DELETE;
  case SDL_SCANCODE_END:
    return GE_KEYCODE_END;
  case SDL_SCANCODE_ESCAPE:
    return GE_KEYCODE_ESCAPE;
  case SDL_SCANCODE_HOME:
    return GE_KEYCODE_HOME;
  case SDL_SCANCODE_INSERT:
    return GE_KEYCODE_INSERT;
  case SDL_SCANCODE_PAGEDOWN:
    return GE_KEYCODE_PAGEDOWN;
  case SDL_SCANCODE_PAGEUP:
    return GE_KEYCODE_PAGEUP;
  case SDL_SCANCODE_RETURN:
    return GE_KEYCODE_RETURN;
  default:
    return GE_KEYCODE_UNKNOWN;
  }
}
