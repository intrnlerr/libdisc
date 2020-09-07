#include "disc_json.h"
#include <stdio.h>
#include <string.h>

static struct disc_json_object* disc_json_object_init()
{
    struct disc_json_object* obj = malloc(sizeof(struct disc_json_object));
    if (obj == NULL) return NULL;
    obj->values = malloc(sizeof(struct disc_json_value));
    if (obj->values == NULL)
    {
        free(obj);
        return NULL;
    }
    obj->items = 0;
    obj->size = 1;
    return obj;
}

static struct disc_json_array* disc_json_array_init()
{
    struct disc_json_object* arr = malloc(sizeof(struct disc_json_array));
    if (arr == NULL) return NULL;
    arr->values = malloc(sizeof(struct disc_json_value));
    if (arr->values == NULL)
    {
        free(arr);
        return NULL;
    }
    arr->items = 0;
    arr->size = 1;
}

static void disc_json_object_insert(struct disc_json_object* obj, struct disc_json_value* v)
{
    if (obj->items >= obj->size)
    {
        // huh
        struct disc_json_value* new = realloc(obj->values, sizeof(struct disc_json_value) * obj->size * 2);
        if (new == NULL)
        {
            fprintf(stderr, "realloc fail =(");
            abort();
        }
        obj->values = new;
        obj->size *= 2;
    }
    obj->values[obj->items] = v;
    obj->items++;
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

static void disc_json_object_insert_obj(struct disc_json_object* obj, struct disc_json_object* new)
{
    struct disc_json_value* val = malloc(sizeof(struct disc_json_value));
    if (val == NULL) return;
    val->data.object = new;
    val->type = OBJECT;
    disc_json_object_insert(obj, val);
};

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
    parser->parent = NULL;
    parser->state = SEEK;
    parser->strindex = 0;
}

void disc_json_parse(struct disc_json_parser* parser, char* data, size_t nmemb)
{
    for (size_t i = 0; i < nmemb; ++i)
    {
        // help
        
        if (data[i] == '{')
        {
            // start object
            if (parser->state == SEEK_OBJVALUE)
            {
                // add object to head
                struct disc_json_object_init* obj = disc_json_object_init();
                if (obj == NULL) abort();
                disc_json_object_insert_obj(parser->head.data.array, obj);
                parser->head.type = OBJECT;
                parser->head.data.object = obj;
                parser->state = SEEK_OBJ;
            } 
            else if (parser->state == READ_ARRAY)
            {
                // do something i guess
            }
            else if (parser->head.type == INVALID)
            {
                if (parser->parent == NULL)
                {
                    struct disc_json_object_init* obj = disc_json_object_init();
                    if (obj == NULL) abort();
                    parser->parent = obj;
                }
                parser->state = SEEK_OBJ;
                parser->head.type = OBJECT;
                parser->head.data.object = parser->parent;
            }
        }
        // string or key
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
                parser->state = SEEK_OBJ;
                disc_json_object_insert_str(parser->head.data.object, parser->stringbuf, parser->strindex);
                parser->strindex = 0;
            }
        }
        else if (parser->state == READ_OBJKEY || parser->state == READ_OBJSTR)
        {
            // add char to strbuf
            parser->stringbuf[parser->strindex] = data[i];
            parser->strindex++;
        }
        else if (data[i] == ':' && parser->state != SEEK_OBJVALUE)
        {
            printf("invalid json!\n");
        }
        
    }
}

void disc_json_parser_free(struct disc_json_parser* parser)
{
    if (parser->parent != NULL)
        disc_json_object_free(parser->parent);
    free(parser);
}
