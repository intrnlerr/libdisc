#include "stdlib.h"

enum disc_json_type
{
    JSON_NULL,
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    INVALID,
};

struct disc_json_value
{
    union {
        struct disc_json_object* object;
        struct disc_json_array* array;
        char* string;
        const float number;
    } data;
    enum disc_json_type type;
};

struct disc_json_object
{
    struct disc_json_value** values;
    //const char** keys;
    size_t size;
    size_t items;
};

struct disc_json_array
{
    struct disc_json_value** values;
    size_t size;
    size_t items;
};

static struct disc_json_object* disc_json_object_init();
static struct disc_json_array* disc_json_array_init();

static void disc_json_object_free(struct disc_json_object* obj);
static void disc_json_array_free(struct disc_json_array* arr);

// shit parser =D
enum disc_json_state
{
    SEEK,
    SEEK_OBJ,
    READ_STRING,
    READ_OBJKEY,
    SEEK_OBJVALUE,
    READ_OBJSTR,
    READ_OBJNUM,
    READ_ARRAY
};

static struct disc_json_parser
{
    enum disc_json_state state;
    struct disc_json_object* root;
    struct disc_json_value head;
    char stringbuf[100];
    size_t strindex;
};

// no longer unused =D
struct disc_json_parser* disc_json_parser_init();
void disc_json_parse(struct disc_json_parser* parser, char* data, size_t nmemb);
void disc_json_parser_free(struct disc_json_parser* parser);
