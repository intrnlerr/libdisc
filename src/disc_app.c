#include "disc_app.h"

struct disc_app* disc_app_init()
{
    struct disc_app* app = malloc(sizeof(struct disc_app));
    app->curl_handle = curl_easy_init();

    curl_easy_setopt(app->curl_handle, CURLOPT_USERAGENT, "DiscordBot ("LIBDISC_URL", "LIBDISC_VER")");
    return app;
};

void disc_app_free(struct disc_app* app)
{
    curl_easy_cleanup(app->curl_handle);
    free(app);
}

int disc_app_get(struct disc_app* app, const char* url)
{
    curl_easy_setopt(app->curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(app->curl_handle, CURLOPT_HTTPGET, 1);
    CURLcode res = curl_easy_perform(app->curl_handle);
    
}

// oauth pleas;e ;;;
int disc_app_authorize(struct disc_app* app)
{
    
    return 0;
}

