/**
    \file       example_3.c
    \author     Eremin Dmitry (http://github.com/NeonMercury)
    \date       October, 2015
    \brief      Loading *.json file into the application, parsing it to the jfes_value and editing it.
*/

#include "../jfes.h"
#include "examples.h"

/* For malloc and free functions. You can use your own memory functions. */
#include <stdlib.h>

#include <crtdbg.h>

/** Example 2 entry point. */
int example_3_entry(int argc, char **argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    long buffer_size = 2048;
    char *json_data = malloc(buffer_size * sizeof(char));

    if (!get_file_content("samples/example_3.json", json_data, &buffer_size)) {
        return -1;
    }

    jfes_config_t config;
    config.jfes_malloc = malloc;
    config.jfes_free = free;

    jfes_value_t value;
    jfes_status_t status = jfes_parse_to_value(&config, json_data, buffer_size, &value);

    if (jfes_status_is_good(status)) {
        jfes_value_t *children = jfes_get_child(&value, "children", 0);
        if (children) {
            jfes_value_t *child = jfes_create_object_value(&config);

            jfes_set_object_child(&config, child, jfes_create_string_value(&config, "Paul", 0), "first_name", 0);
            jfes_set_object_child(&config, child, jfes_create_integer_value(&config, 1), "age", 0);

            /* And now we wants to rewrite age value with 2. */
            jfes_set_object_child(&config, child, jfes_create_integer_value(&config, 2), "age", 0);
            
            status = jfes_add_to_array(&config, children, child);
        }

        jfes_free_value(&config, &value);
    }

    free(json_data);
    return 0;
}