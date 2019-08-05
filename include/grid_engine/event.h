// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_EVENT_H_
#define GE_EVENT_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum ge_event_type {
  GE_EVENT_NONE = 0,
  GE_EVENT_KEYDOWN,
  GE_EVENT_KEYUP,
} ge_event_type_t;

typedef enum ge_keycode {
  GE_KEYCODE_UNKNOWN = 0,

  // Letters
  GE_KEYCODE_A,
  GE_KEYCODE_B,
  GE_KEYCODE_C,
  GE_KEYCODE_D,
  GE_KEYCODE_E,
  GE_KEYCODE_F,
  GE_KEYCODE_G,
  GE_KEYCODE_H,
  GE_KEYCODE_I,
  GE_KEYCODE_J,
  GE_KEYCODE_K,
  GE_KEYCODE_L,
  GE_KEYCODE_M,
  GE_KEYCODE_N,
  GE_KEYCODE_O,
  GE_KEYCODE_P,
  GE_KEYCODE_Q,
  GE_KEYCODE_R,
  GE_KEYCODE_S,
  GE_KEYCODE_T,
  GE_KEYCODE_U,
  GE_KEYCODE_V,
  GE_KEYCODE_W,
  GE_KEYCODE_X,
  GE_KEYCODE_Y,
  GE_KEYCODE_Z,

  // Numbers
  GE_KEYCODE_0,
  GE_KEYCODE_1,
  GE_KEYCODE_2,
  GE_KEYCODE_3,
  GE_KEYCODE_4,
  GE_KEYCODE_5,
  GE_KEYCODE_6,
  GE_KEYCODE_7,
  GE_KEYCODE_8,
  GE_KEYCODE_9,

  // Arrows
  GE_KEYCODE_UP,
  GE_KEYCODE_DOWN,
  GE_KEYCODE_LEFT,
  GE_KEYCODE_RIGHT,

  // Other characters
  GE_KEYCODE_APOSTROPHE,
  GE_KEYCODE_BACKSLASH,
  GE_KEYCODE_COMMA,
  GE_KEYCODE_EQUALS,
  GE_KEYCODE_GRAVE,
  GE_KEYCODE_LEFTBRACKET,
  GE_KEYCODE_MINUS,
  GE_KEYCODE_PERIOD,
  GE_KEYCODE_RIGHTBRACKET,
  GE_KEYCODE_SEMICOLON,
  GE_KEYCODE_SLASH,
  GE_KEYCODE_SPACE,
  GE_KEYCODE_TAB,

  // Function keys
  GE_KEYCODE_F1,
  GE_KEYCODE_F2,
  GE_KEYCODE_F3,
  GE_KEYCODE_F4,
  GE_KEYCODE_F5,
  GE_KEYCODE_F6,
  GE_KEYCODE_F7,
  GE_KEYCODE_F8,
  GE_KEYCODE_F9,
  GE_KEYCODE_F10,
  GE_KEYCODE_F11,
  GE_KEYCODE_F12,

  // Modifiers
  GE_KEYCODE_LALT,
  GE_KEYCODE_LCTRL,
  GE_KEYCODE_LGUI,
  GE_KEYCODE_LSHIFT,
  GE_KEYCODE_RALT,
  GE_KEYCODE_RCTRL,
  GE_KEYCODE_RGUI,
  GE_KEYCODE_RSHIFT,

  // Control keys
  GE_KEYCODE_BACKSPACE,
  GE_KEYCODE_CAPSLOCK,
  GE_KEYCODE_DELETE,
  GE_KEYCODE_END,
  GE_KEYCODE_ESCAPE,
  GE_KEYCODE_HOME,
  GE_KEYCODE_INSERT,
  GE_KEYCODE_PAGEDOWN,
  GE_KEYCODE_PAGEUP,
  GE_KEYCODE_RETURN,
} ge_keycode_t;

typedef struct ge_keydown_event {
  uint32_t timestamp_ms;
  ge_keycode_t keycode;
  bool is_repeat;
} ge_keydown_event_t;

typedef struct ge_keyup_event {
  uint32_t timestamp_ms;
  ge_keycode_t keycode;
} ge_keyup_event_t;

typedef struct ge_event {
  ge_event_type_t type;
  union {
    ge_keydown_event_t keydown_data;
    ge_keyup_event_t keyup_data;
  };
} ge_event_t;

bool ge_fill_event(ge_event_t* restrict event, void* restrict internal_event);

const char* ge_keycode_to_str(ge_keycode_t keycode);

#endif  // GE_EVENT_H_
