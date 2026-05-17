#include <sdl3-proxy.h>

simplejs_status_t sdl3_proxy_create_window(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;

    char tempStringBuffer[4096];

    SDL_Window *sdl_window = NULL;
    simplejs_object_t *object_window = NULL;

    if (function_header->argument_count < 4)
    {
        goto error;
    }

    char *title_string;
    simplejs_variable_to_string(&function_header->arguments[0], tempStringBuffer, sizeof(tempStringBuffer), &title_string);

    simplejs_variable_t *width_var = &function_header->arguments[1];
    simplejs_variable_t *height_var = &function_header->arguments[2];
    simplejs_variable_t *flags_var = &function_header->arguments[3];

    sdl_window = SDL_CreateWindow(title_string, simplejs_variable_get_int(width_var), simplejs_variable_get_int(height_var), simplejs_variable_get_int(flags_var));
    if (!sdl_window)
    {
        status = SIMPLEJS_STATUS_ALLOCATION_ERROR;
        goto error;
    }

    SIMPLEJS_REQUIRE_SUCCESS(create_pointer_object(sdl_window, &object_window), error, status);

    simplejs_variable_t ret_var;
    simplejs_variable_init_object(&ret_var, object_window);
    simplejs_variable_assign(out, &ret_var);

    simplejs_gc_add_object(object_window);

    return SIMPLEJS_STATUS_SUCCESS;
error:
    if (!SIMPLEJS_SUCCESS(status))
    {
        if (sdl_window)
            SDL_DestroyWindow(sdl_window);
        if (object_window)
            simplejs_object_release(object_window);

        simplejs_variable_t undef_var = {0};

        simplejs_variable_assign(out, &undef_var);
    }

    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_create_renderer(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;

    simplejs_object_t *object_renderer = NULL;
    char tempStringBuffer[4096];

    if (function_header->argument_count < 2)
    {
        goto error;
    }

    simplejs_variable_t *window_var = &function_header->arguments[0];

    char *name_string = NULL;
    simplejs_variable_to_string(&function_header->arguments[1], tempStringBuffer, sizeof(tempStringBuffer), &name_string);

    simplejs_object_t *object_window = window_var->value.object;
    if (window_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
    {
        status = SIMPLEJS_STATUS_INVALID_PARAMETER;
        goto error;
    }

    SDL_Window *sdl_window = read_pointer_object(object_window);

    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
    if (!sdl_renderer)
    {
        status = SIMPLEJS_STATUS_ALLOCATION_ERROR;
        goto error;
    }

    SIMPLEJS_REQUIRE_SUCCESS(create_pointer_object(sdl_renderer, &object_renderer), error, status);

    simplejs_variable_t ret_var;
    simplejs_variable_init_object(&ret_var, object_renderer);
    simplejs_variable_assign(out, &ret_var);

    simplejs_gc_add_object(object_renderer);

    return SIMPLEJS_STATUS_SUCCESS;
error:
    if (!SIMPLEJS_SUCCESS(status))
    {
        if (sdl_renderer)
            SDL_DestroyRenderer(sdl_renderer);
        if (object_renderer)
            simplejs_object_release(object_renderer);

        simplejs_variable_t undef_var = {0};
        simplejs_variable_assign(out, &undef_var);
    }

    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_destroy_renderer(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    if (function_header->argument_count < 1)
        goto error;

    simplejs_variable_t *renderer_var = &function_header->arguments[0];

    simplejs_object_t *object_renderer = renderer_var->value.object;
    if (renderer_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
        goto error;

    SDL_Renderer *sdl_renderer = read_pointer_object(object_renderer);
    if (!sdl_renderer)
        goto error;

    SDL_DestroyRenderer(sdl_renderer);

error:
    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_destroy_window(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    if (function_header->argument_count < 1)
        goto error;

    simplejs_variable_t *window_var = &function_header->arguments[0];

    simplejs_object_t *object_window = window_var->value.object;
    if (window_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
        goto error;

    SDL_Window *sdl_window = read_pointer_object(object_window);
    if (!sdl_window)
        goto error;

    SDL_DestroyWindow(sdl_window);

error:
    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_create_event(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;
    simplejs_object_t *object_event = NULL;

    SIMPLEJS_REQUIRE_SUCCESS(simplejs_builtin_create_dynamic_object(&object_event), error, status);

    simplejs_variable_t ret_var;
    simplejs_variable_init_object(&ret_var, object_event);
    simplejs_variable_assign(out, &ret_var);

    simplejs_gc_add_object(object_event);

    return SIMPLEJS_STATUS_SUCCESS;
error:
    if (!SIMPLEJS_SUCCESS(status))
    {
        if (object_event)
            simplejs_object_release(object_event);

        simplejs_variable_t undef_var = {0};
        simplejs_variable_assign(out, &undef_var);
    }

    return SIMPLEJS_STATUS_SUCCESS;
}

typedef struct
{
    char *name;
    void *pointer;
    bool is_string;
    simplejs_number_type_t type;
} poll_list_t;

simplejs_status_t sdl3_proxy_poll_event(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;
    simplejs_object_t *object_event = NULL;
    if (function_header->argument_count < 1)
    {
        goto error;
    }

    simplejs_variable_t *event_var = &function_header->arguments[0];

    object_event = event_var->value.object;
    if (event_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
    {
        status = SIMPLEJS_STATUS_INVALID_PARAMETER;
        goto error;
    }

    SDL_Event event;
    bool result = SDL_PollEvent(&event);

    simplejs_number_t temp_num;
    simplejs_variable_t temp_in;

    poll_list_t poll_list[] = {
        {"type", &event.type, false, SIMPLEJS_NUMBER_TYPE_UI32},
        {"timestamp", &event.common.timestamp, false, SIMPLEJS_NUMBER_TYPE_UI64}};

    for (int i = 0; i < sizeof(poll_list) / sizeof(*poll_list); i++)
    {
        simplejs_variable_t temp_property;

        poll_list_t *entry = &poll_list[i];
        double value;

        switch (entry->type)
        {
        case SIMPLEJS_NUMBER_TYPE_IPTR:
            value = *(intptr_t *)entry->pointer;
            break;
        case SIMPLEJS_NUMBER_TYPE_I32:
            value = *(int32_t *)entry->pointer;
            break;

        case SIMPLEJS_NUMBER_TYPE_I64:
            value = *(int64_t *)entry->pointer;
            break;

        case SIMPLEJS_NUMBER_TYPE_UIPTR:
            value = *(uintptr_t *)entry->pointer;
            break;
        case SIMPLEJS_NUMBER_TYPE_UI32:
            value = *(uint32_t *)entry->pointer;
            break;

        case SIMPLEJS_NUMBER_TYPE_UI64:
            value = *(uint64_t *)entry->pointer;
            break;

        case SIMPLEJS_NUMBER_TYPE_F32:
            value = *(float *)entry->pointer;
            break;

        case SIMPLEJS_NUMBER_TYPE_F64:
            value = *(double *)entry->pointer;
            break;

        default:
            break;
        }

        simplejs_variable_init_fast_string(&temp_property, entry->name);

        temp_num.type = SIMPLEJS_NUMBER_TYPE_F64;
        temp_num.value.f64 = value;
        simplejs_variable_init_number(&temp_in, &temp_num);

        SIMPLEJS_REQUIRE_SUCCESS(simplejs_object_set_property_value(object_event, &temp_property, &temp_in), error, status);
    }

    temp_num.type = SIMPLEJS_NUMBER_TYPE_UI32;
    temp_num.value.ui32 = result;

    simplejs_variable_init_number(&temp_in, &temp_num);
    simplejs_variable_assign(out, &temp_in);

    return SIMPLEJS_STATUS_SUCCESS;
error:
    if (!SIMPLEJS_SUCCESS(status))
    {
        simplejs_variable_t undef_var = {0};
        simplejs_variable_assign(out, &undef_var);
    }

    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_set_render_draw_color(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    if (function_header->argument_count < 5)
        goto error;

    simplejs_variable_t *renderer_var = &function_header->arguments[0];

    simplejs_variable_t *r_var = &function_header->arguments[1];
    simplejs_variable_t *g_var = &function_header->arguments[2];
    simplejs_variable_t *b_var = &function_header->arguments[3];
    simplejs_variable_t *a_var = &function_header->arguments[4];

    simplejs_object_t *object_renderer = renderer_var->value.object;
    if (renderer_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
    {
        simplejs_printf("invalid object_renderer\n");
        goto error;
    }

    SDL_Renderer *sdl_renderer = read_pointer_object(object_renderer);
    if (!sdl_renderer)
    {
        simplejs_printf("invalid sdl_renderer\n");
        goto error;
    }

    SDL_SetRenderDrawColor(sdl_renderer, simplejs_variable_get_int(r_var), simplejs_variable_get_int(g_var), simplejs_variable_get_int(b_var), simplejs_variable_get_int(a_var));

error:
    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_render_clear(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    if (function_header->argument_count < 1)
        goto error;

    simplejs_variable_t *renderer_var = &function_header->arguments[0];

    simplejs_object_t *object_renderer = renderer_var->value.object;
    if (renderer_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
        goto error;

    SDL_Renderer *sdl_renderer = read_pointer_object(object_renderer);
    if (!sdl_renderer)
        goto error;

    SDL_RenderClear(sdl_renderer);

error:
    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_render_present(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    if (function_header->argument_count < 1)
        goto error;

    simplejs_variable_t *renderer_var = &function_header->arguments[0];

    simplejs_object_t *object_renderer = renderer_var->value.object;
    if (renderer_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
        goto error;

    SDL_Renderer *sdl_renderer = read_pointer_object(object_renderer);
    if (!sdl_renderer)
        goto error;

    SDL_RenderPresent(sdl_renderer);

error:
    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_set_render_vsync(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    if (function_header->argument_count < 2)
        goto error;

    simplejs_variable_t *renderer_var = &function_header->arguments[0];
    simplejs_variable_t *mode_var = &function_header->arguments[1];

    simplejs_object_t *object_renderer = renderer_var->value.object;
    if (renderer_var->type != SIMPLEJS_VARIABLE_TYPE_OBJECT)
        goto error;

    SDL_Renderer *sdl_renderer = read_pointer_object(object_renderer);
    if (!sdl_renderer)
        goto error;

    SDL_SetRenderVSync(sdl_renderer, simplejs_variable_get_int(mode_var));

error:
    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_get_ticks_ns(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    simplejs_number_t tmp_num;
    simplejs_variable_t tmp_var;

    tmp_num.type = SIMPLEJS_NUMBER_TYPE_F64;
    tmp_num.value.f64 = (double)SDL_GetTicksNS();

    simplejs_variable_init_number(&tmp_var, &tmp_num);
    simplejs_variable_assign(out, &tmp_var);

    return SIMPLEJS_STATUS_SUCCESS;
}

simplejs_status_t sdl3_proxy_print(simplejs_function_header_t *function_header, simplejs_variable_t *out)
{
    char tempStringBuffer[4096];
    char *string = NULL;

    if (function_header->argument_count < 1)
        goto error;

    simplejs_variable_t *var = &function_header->arguments[0];

    simplejs_variable_to_string(var, tempStringBuffer, sizeof(tempStringBuffer), &string);

    simplejs_printf("var: %s\n", string);

error:
    return SIMPLEJS_STATUS_SUCCESS;
}

typedef struct sdl3_function_table
{
    char *name;
    simplejs_function_proxy_t proxy;
    uint64_t value;
} sdl3_function_table_t;

sdl3_function_table_t sdl3_function_table[] = {
    {"CreateWindow", sdl3_proxy_create_window},
    {"CreateRenderer", sdl3_proxy_create_renderer},
    {"DestroyRenderer", sdl3_proxy_destroy_renderer},
    {"DestroyWindow", sdl3_proxy_destroy_window},
    {"CreateEvent", sdl3_proxy_create_event},
    {"PollEvent", sdl3_proxy_poll_event},

    {"SetRenderDrawColor", sdl3_proxy_set_render_draw_color},
    {"RenderClear", sdl3_proxy_render_clear},
    {"RenderPresent", sdl3_proxy_render_present},

    {"SetRenderVSync", sdl3_proxy_set_render_vsync},

    {"GetTicksNS", sdl3_proxy_get_ticks_ns},

    {"Print", sdl3_proxy_print},

    {"SDL_EVENT_QUIT", NULL, SDL_EVENT_QUIT},
    {"SDL_WINDOW_OPENGL", NULL, SDL_WINDOW_OPENGL},

    {"SDL_RENDERER_VSYNC_DISABLED", NULL, 0},
    {"SDL_RENDERER_VSYNC_ENABLED", NULL, 1},
    {"SDL_RENDERER_VSYNC_ADAPTIVE", NULL, -1},
};

simplejs_status_t init_sdl3_proxy()
{
    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;

    for (int i = 0; i < sizeof(sdl3_function_table) / sizeof(*sdl3_function_table); i++)
    {
        sdl3_function_table_t *entry = &sdl3_function_table[i];

        simplejs_variable_t temp_in;

        if (entry->proxy)
        {
            simplejs_function_t temp_function;
            temp_function.type = SIMPLEJS_FUNCTION_TYPE_PROXY;
            temp_function.value.proxy = entry->proxy;

            simplejs_variable_init_function(&temp_in, &temp_function);
        }
        else
        {
            simplejs_number_t temp_num;
            temp_num.type = SIMPLEJS_NUMBER_TYPE_UI64;
            temp_num.value.ui64 = entry->value;

            simplejs_variable_init_number(&temp_in, &temp_num);
        }

        simplejs_variable_t temp_property;
        simplejs_variable_init_fast_string(&temp_property, entry->name);

        SIMPLEJS_REQUIRE_SUCCESS(simplejs_object_set_property_value(sdl3_object, &temp_property, &temp_in), result, status);
    }

result:
    return status;
}
