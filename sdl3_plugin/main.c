#include <default.h>
#include <sdl3-proxy.h>

simplejs_object_t *sdl3_object = NULL;

simplejs_status_t SIMPLEJS_PLUGIN_API plugin_main(simplejs_object_t *global_object)
{
    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        status = SIMPLEJS_STATUS_UNSUCCESSFUL;
        goto result;
    }

    SIMPLEJS_REQUIRE_SUCCESS(simplejs_builtin_create_dynamic_object(&sdl3_object), result, status);

    simplejs_variable_t temp_property;
    simplejs_variable_t temp_in;

    simplejs_variable_init_fast_string(&temp_property, SDL_PROPERTY);
    simplejs_variable_init_object(&temp_in, sdl3_object);

    SIMPLEJS_REQUIRE_SUCCESS(simplejs_object_set_property_value(global_object, &temp_property, &temp_in), result, status);

    SIMPLEJS_REQUIRE_SUCCESS(init_sdl3_proxy(), result, status);

    simplejs_printf("SDL3 plugin exited with '%s'\n", simplejs_get_status_string(status));

result:
    if (!SIMPLEJS_SUCCESS(status))
    {
        if (sdl3_object)
        {
            simplejs_number_t temp_number = {0};

            simplejs_variable_init_fast_string(&temp_property, SDL_PROPERTY);
            simplejs_variable_init_number(&temp_in, &temp_number);

            simplejs_object_set_property_value(global_object, &temp_property, &temp_in);

            simplejs_object_release(sdl3_object);
        }
    }

    return status;
}
