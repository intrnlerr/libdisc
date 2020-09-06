#include "disc_app.h"

struct disc_app* disc_app_init()
{
    struct disc_app* app = malloc(sizeof(struct disc_app));
    app->curl_handle = curl_easy_init();
};

void disc_app_free(struct disc_app* app)
{
    curl_easy_cleanup(app->curl_handle);
    free(app);
}

int disc_app_authorize(struct disc_app* app)
{
    
    return 0;
}