#include "disc_endpoint.h"

int disc_get_gateway(struct disc_app* app)
{
    disc_app_get(app, LIBDISC_ENDPOINT("/gateway"));
}
