#include <default.h>

void *read_pointer_object(simplejs_object_t *pointer_object)
{
    void *pointer = NULL;

    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;
    simplejs_variable_t temp_out;
    simplejs_number_t *temp_number = &temp_out.value.number;

    simplejs_variable_t temp_property;
    simplejs_variable_init_fast_string(&temp_property, SDL_POINTER_PROPERTY);

    SIMPLEJS_REQUIRE_SUCCESS(simplejs_object_get_property_value(pointer_object, &temp_property, &temp_out), result, status);

    pointer = (void *)temp_number->value.uiptr;

result:
    return pointer;
}

simplejs_status_t create_pointer_object(void *pointer, simplejs_object_t **out)
{
    simplejs_status_t status = SIMPLEJS_STATUS_SUCCESS;
    simplejs_object_t *object;

    SIMPLEJS_REQUIRE_SUCCESS(simplejs_builtin_create_dynamic_object(&object), result, status);

    simplejs_number_t temp_number = {0};
    temp_number.type = SIMPLEJS_NUMBER_TYPE_UIPTR;
    temp_number.value.uiptr = (uintptr_t)pointer;

    simplejs_variable_t temp_in;
    simplejs_variable_init_number(&temp_in, &temp_number);

    simplejs_variable_t temp_property;
    simplejs_variable_init_fast_string(&temp_property, SDL_POINTER_PROPERTY);

    SIMPLEJS_REQUIRE_SUCCESS(simplejs_object_set_property_value(object, &temp_property, &temp_in), result, status);

    simplejs_builtin_set_dynamic_object_read_only(object, true);

    *out = object;

result:
    return status;
}
