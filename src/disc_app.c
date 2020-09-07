#include "disc_app.h"

size_t disc_app_writecallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    struct disc_app* app = userdata;
    disc_json_parse(app->parser, ptr, nmemb);
}

struct disc_app* disc_app_init()
{
    struct disc_app* app = malloc(sizeof(struct disc_app));
    if (app == NULL) return app;
    app->curl_handle = curl_easy_init();
    app->parser = disc_json_parser_init();

    curl_easy_setopt(app->curl_handle, CURLOPT_USERAGENT, "DiscordBot ("LIBDISC_URL", "LIBDISC_VER")");
    curl_easy_setopt(app->curl_handle, CURLOPT_WRITEFUNCTION, disc_app_writecallback);
    curl_easy_setopt(app->curl_handle, CURLOPT_WRITEDATA, app);
    return app;
};



void disc_app_free(struct disc_app* app)
{
    curl_easy_cleanup(app->curl_handle);
    disc_json_parser_free(app->parser);
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

