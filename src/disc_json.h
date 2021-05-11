#include "stdlib.h"
#include "disc_types.h"

enum disc_json_type
{
    JSON_NULL,
    JSON_BOOL,
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
        float number;
        char bool;
    } data;
    enum disc_json_type type;
};

struct disc_json_object
{
    struct disc_json_value** values;
    char** keys;
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

// O(n) basic search
LIBDISC_EXPORT struct disc_json_value* disc_json_object_get(struct disc_json_object* obj, const char* key);

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
    READ_OBJTFN,
    SEEK_ARRAY,
    READ_ARRAYSTR
};

struct disc_json_queue {
    struct disc_json_object** buffer;
    struct disc_json_object** buffer_end;
    struct disc_json_object** data_start;
    struct disc_json_object** data_end;
};

struct disc_json_queue* disc_json_queue_init(size_t size);
struct disc_json_object* disc_json_queue_dequeue(struct disc_json_queue* queue);

struct disc_json_parser
{
    enum disc_json_state state;
    struct disc_json_value head;
    struct disc_json_value stack[20];
    struct disc_json_queue* queue;
    char stringbuf[200];
    size_t stackpos;
    size_t strindex;
};

// no longer unused =D
struct disc_json_parser* disc_json_parser_init();
// void disc_json_parser_reset(struct disc_json_parser* parser);
void disc_json_parse(struct disc_json_parser* parser, char* data, size_t nmemb);
void disc_json_parser_free(struct disc_json_parser* parser);
