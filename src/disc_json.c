#include "disc_json.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static struct disc_json_object* disc_json_object_init()
{
    struct disc_json_object* obj = malloc(sizeof(struct disc_json_object));
    if (obj == NULL) return NULL;
    obj->values = malloc(sizeof(struct disc_json_value*));
    if (obj->values == NULL)
    {
        free(obj);
        return NULL;
    }
    obj->keys = malloc(sizeof(char*));
    if (obj->keys == NULL)
    {
        free(obj->values);
        free(obj);
        return NULL;
    }
    obj->items = 0;
    obj->size = 1;
    return obj;
}

static struct disc_json_array* disc_json_array_init()
{
    struct disc_json_array* arr = malloc(sizeof(struct disc_json_array));
    if (arr == NULL) return NULL;
    arr->values = malloc(sizeof(struct disc_json_value*));
    if (arr->values == NULL)
    {
        free(arr);
        return NULL;
    }
    arr->items = 0;
    arr->size = 1;
    return arr;
}

static inline void disc_json_object_insert(struct disc_json_object* obj, struct disc_json_value* v)
{
    obj->values[obj->items] = v;
    obj->items++;
};

static void disc_json_object_insert_key(struct disc_json_object* obj, char const* v, size_t len)
{
    if (obj->items >= obj->size)
    {
        // huh
        struct disc_json_value** new = realloc(obj->values, sizeof(struct disc_json_value*) * obj->size * 2);
        if (new == NULL)
        {
            fprintf(stderr, "realloc fail =(");
            abort();
        }
        char** newkeys = realloc(obj->keys, sizeof(char*) * obj->size * 2);
        if (newkeys == NULL)
        {
            fprintf(stderr, "realloc fail =(");
            abort();
        }
        obj->values = new;
        obj->keys = newkeys;
        obj->size *= 2;
    }
    char* key = malloc(sizeof(char) * len + 1);
    if (key == NULL)
    {
        return;
    }
    memcpy(key, v, len);
    key[len] = '\0';
    obj->keys[obj->items] = key;
};

static inline void disc_json_object_insert_null(struct disc_json_object* obj)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->type = JSON_NULL;
    disc_json_object_insert(obj, val);
};

static inline void disc_json_object_insert_bool(struct disc_json_object* obj, char bool)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.bool = bool;
    val->type = JSON_BOOL;
    disc_json_object_insert(obj, val);
};

static inline void disc_json_object_insert_num(struct disc_json_object* obj, float num)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.number = num;
    val->type = NUMBER;
    disc_json_object_insert(obj, val);
};

static void disc_json_object_insert_str(struct disc_json_object* obj, const char* str, size_t len)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.string = malloc(sizeof(char) * len + 1);
    if (val->data.string == NULL)
    {
        free(val);
        return;
    }
    val->data.string[len] = '\0';
    memcpy(val->data.string, str, len);
    val->type = STRING;
    disc_json_object_insert(obj, val);
};

static inline void disc_json_object_insert_obj(struct disc_json_object* obj, struct disc_json_object* new)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.object = new;
    val->type = OBJECT;
    disc_json_object_insert(obj, val);
};

static inline void disc_json_object_insert_arr(struct disc_json_object* obj, struct disc_json_array* arr)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.array = arr;
    val->type = ARRAY;
    disc_json_object_insert(obj, val);
}

static void disc_json_array_insert(struct disc_json_array* arr, struct disc_json_value* val)
{
    if (arr->items >= arr->size)
    {
        // huh
        struct disc_json_value** new = realloc(arr->values, sizeof(struct disc_json_value*) * arr->size * 2);
        if (new == NULL)
        {
            fprintf(stderr, "realloc fail array insert");
            abort();
        }
        arr->values = new;
        arr->size *= 2;
    }
    arr->values[arr->items] = val;
    arr->items++;
}

static void disc_json_array_insert_str(struct disc_json_array* arr, const char* str, size_t len)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.string = malloc(sizeof(char) * len + 1);
    if (val->data.string == NULL)
    {
        free(val);
        return;
    }
    val->data.string[len] = '\0';
    memcpy(val->data.string, str, len);
    val->type = STRING;
    disc_json_array_insert(arr, val);
};

static inline void disc_json_array_insert_obj(struct disc_json_array* arr, struct disc_json_object* obj)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.object = obj;
    val->type = OBJECT;
    disc_json_array_insert(arr, val);
}

static inline void disc_json_array_insert_arr(struct disc_json_array* arr, struct disc_json_array* new)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.object = new;
    val->type = ARRAY;
    disc_json_array_insert(arr, val);
}

struct disc_json_value* disc_json_object_get(struct disc_json_object* obj, const char* key)
{
    if (obj->items == 0) return NULL;
    for (size_t i = 0; i < obj->items; ++i)
    {
        if (strcmp(obj->keys[i], key) == 0) return obj->values[i];
    }
    return NULL;
}

static void disc_json_object_free(struct disc_json_object* obj)
{
    for(size_t i = 0; i < obj->items; ++i)
    {
        switch (obj->values[i]->type)
        {
        case OBJECT:
            disc_json_object_free(obj->values[i]->data.object);
            free(obj->values[i]);
            break;
        case ARRAY:
            disc_json_array_free(obj->values[i]->data.array);
            free(obj->values[i]);
            break;
        case STRING:
            free(obj->values[i]->data.string);
            free(obj->values[i]);
            break;
        }
    }
    free(obj->values);
    free(obj);
}

static void disc_json_array_free(struct disc_json_array* arr)
{
    for(size_t i = 0; i < arr->items; ++i)
    {
        switch (arr->values[i]->type)
        {
        case OBJECT:
            disc_json_object_free(arr->values[i]->data.object);
            free(arr->values[i]);
            break;
        case ARRAY:
            disc_json_array_free(arr->values[i]->data.array);
            free(arr->values[i]);
            break;
        case STRING:
            free(arr->values[i]->data.string);
            free(arr->values[i]);
            break;
        }
    }
    free(arr->values);
    free(arr);
}

struct disc_json_parser* disc_json_parser_init()
{
    struct disc_json_parser* parser = malloc(sizeof(struct disc_json_parser));
    if (parser == NULL) return NULL;
    parser->head.type = INVALID;
    parser->root = NULL;
    parser->state = SEEK;
    parser->strindex = 0;
    parser->stackpos = 0;
    return parser;
}

void disc_json_parser_reset(struct disc_json_parser* parser)
{
    parser->head.type = INVALID;
    if (parser->root != NULL)
    {
        disc_json_object_free(parser->root);
        parser->root = NULL;
    }
    parser->state = SEEK;
    parser->strindex = 0;
    parser->stackpos = 0;
}

void disc_json_parse(struct disc_json_parser* parser, char* data, size_t nmemb)
{
    for (size_t i = 0; i < nmemb; ++i)
    {
        // help
        // string or key
        if (data[i] == '\\')
        {
            if (data[i + 1] == '"')
            {
                parser->stringbuf[parser->strindex++] = '\n';
            }
        }
        else if (data[i] == '"')
        {
            // read_name or read_obj for string
            if (parser->state == SEEK_OBJ)
            {
                // i find key start
                parser->state = READ_OBJKEY;

            }
            else if (parser->state == READ_OBJKEY)
            {
                // finish read
                disc_json_object_insert_key(parser->head.data.object, parser->stringbuf, parser->strindex);
                parser->state = SEEK_OBJVALUE;
                parser->strindex = 0;
            }
            else if (parser->state == SEEK_OBJVALUE)
            {
                parser->state = READ_OBJSTR;
            }
            else if (parser->state == READ_OBJSTR)
            {
                // finish string value of object
                disc_json_object_insert_str(parser->head.data.object, parser->stringbuf, parser->strindex);
                parser->state = SEEK_OBJ;
                parser->strindex = 0;
            }
            else if (parser->state == SEEK_ARRAY)
            {
                parser->state = READ_ARRAYSTR;
            }
            else if (parser->state == READ_ARRAYSTR)
            {
                disc_json_array_insert_str(parser->head.data.array, parser->stringbuf, parser->strindex);
                parser->state = SEEK_ARRAY;
                parser->strindex = 0;
            }
        }
        else if (data[i] == ',')
        {
            if (parser->state == READ_OBJNUM)
            {
                parser->stringbuf[parser->strindex] = '\0';
                disc_json_object_insert_num(parser->head.data.object, strtof(parser->stringbuf, NULL));
                parser->state = SEEK_OBJ;
                parser->strindex = 0;
            }
        }
        else if (parser->state == READ_OBJKEY || parser->state == READ_OBJSTR || parser->state == READ_OBJNUM || parser->state == READ_ARRAYSTR)
        {
            // add char to strbuf
            parser->stringbuf[parser->strindex] = data[i];
            parser->strindex++;
        }
        else if (data[i] == '{')
        {
            // start object
            if (parser->state == SEEK_OBJVALUE)
            {
                // add object to head
                struct disc_json_object* obj = disc_json_object_init();
                if (obj == NULL) abort();
                disc_json_object_insert_obj(parser->head.data.object, obj);
                parser->stack[parser->stackpos] = parser->head;
                parser->stackpos++;
                parser->head.type = OBJECT;
                parser->head.data.object = obj;
                parser->state = SEEK_OBJ;
            } 
            else if (parser->state == SEEK_ARRAY)
            {
                // do something i guess
                struct disc_json_object* obj = disc_json_object_init();
                if (obj == NULL) abort();
                disc_json_array_insert_obj(parser->head.data.array, obj);
                parser->stack[parser->stackpos] = parser->head;
                parser->stackpos++;
                parser->head.type = OBJECT;
                parser->head.data.object = obj;
                parser->state = SEEK_OBJ;
            }
            else if (parser->head.type == INVALID)
            {
                if (parser->root == NULL)
                {
                    struct disc_json_object* obj = disc_json_object_init();
                    if (obj == NULL) abort();
                    parser->root = obj;
                }
                parser->state = SEEK_OBJ;
                parser->head.type = OBJECT;
                parser->head.data.object = parser->root;
            }
        }
        else if (data[i] == '[')
        {
            // create array
            if (parser->state == SEEK_OBJVALUE)
            {
                // add object to head
                struct disc_json_array* arr = disc_json_array_init();
                if (arr == NULL) abort();
                disc_json_object_insert_arr(parser->head.data.object, arr);
                parser->stack[parser->stackpos] = parser->head;
                parser->stackpos++;
                parser->head.type = ARRAY;
                parser->head.data.array = arr;
                parser->state = SEEK_ARRAY;
            }
            else if (parser->state == SEEK_ARRAY)
            {
                // do something i guess
                struct disc_json_array* arr = disc_json_array_init();
                if (arr == NULL) abort();
                disc_json_array_insert_arr(parser->head.data.object, arr);
                parser->stack[parser->stackpos] = parser->head;
                parser->stackpos++;
                parser->head.type = ARRAY;
                parser->head.data.array = arr;
            }
        }
        else if (data[i] == '}' || data[i] == ']')
        {
            // go down a level
            parser->head = parser->stack[--parser->stackpos];
            if (parser->head.type == OBJECT) parser->state = SEEK_OBJ;
            else if (parser->head.type == ARRAY) parser->state = SEEK_ARRAY;
        }
        else if (parser->state == SEEK_OBJVALUE)
        {
            if (data[i] == 'n' && strncmp(data + i, "null", 4) == 0)
            {
                // null value
                disc_json_object_insert_null(parser->head.data.object);
                i += 3;
                parser->state = SEEK_OBJ;
            }
            else if (data[i] == 't' && strncmp(data + i, "true", 4) == 0)
            {
                disc_json_object_insert_bool(parser->head.data.object, 1);
                i += 3;
                parser->state = SEEK_OBJ;
            }
            else if (data[i] == 'f' && strncmp(data + i, "false", 5) == 0)
            {
                disc_json_object_insert_bool(parser->head.data.object, 0);
                i += 4;
                parser->state = SEEK_OBJ;
            }
            else if (isdigit(data[i]))
            {
                parser->state = READ_OBJNUM;
                parser->stringbuf[parser->strindex] = data[i];
                parser->strindex++;
            }
        }
        else if (data[i] == ':' && parser->state != SEEK_OBJVALUE)
        {
            printf("invalid json at index %d\n", i);
        }
        
    }
}

void disc_json_parser_free(struct disc_json_parser* parser)
{
    if (parser->root != NULL)
        disc_json_object_free(parser->root);
    free(parser);
}
