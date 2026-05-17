#pragma once
#include <simplejs/default.h>
#include <simplejs/object.h>
#include <simplejs/variable.h>
#include <simplejs/mm/gc.h>
#include <simplejs/lib/s_string.h>
#include <simplejs/builtin_object/dynamic_object.h>
#include <SDL3/SDL.h>

#define SDL_PROPERTY "SDL3"
#define SDL_POINTER_PROPERTY "pointer"

extern simplejs_object_t *sdl3_object;

void *read_pointer_object(simplejs_object_t *pointer_object);
simplejs_status_t create_pointer_object(void *pointer, simplejs_object_t **out);
