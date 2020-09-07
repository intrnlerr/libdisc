#include "disc_endpoint.h"

int disc_get_guild(struct disc_app* app, disc_snowflake id)
{
    char buf[100];
    sprintf_s(buf, 100, LIBDISC_ENDPOINT("/guilds/")"%llu", id);
    disc_app_get(app, buf);
}

int disc_get_gateway(struct disc_app* app)
{
    disc_app_get(app, LIBDISC_ENDPOINT("/gateway"));
}
