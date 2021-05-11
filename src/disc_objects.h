#include "disc_types.h"

struct disc_obj_user {
    disc_snowflake id;
};

struct disc_obj_message {
    disc_snowflake id;
    disc_snowflake channel_id;
    struct disc_obj_user author; 
};

struct disc_obj_message_create {
    char* content;
};

