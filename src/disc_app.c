#include "disc_app.h"

size_t disc_app_writecallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    // TODO: this is pretty bad
    struct disc_app* app = userdata;
    disc_json_parse(app->parser, ptr, nmemb);
    return nmemb;
}

struct disc_app* disc_app_init()
{
    struct disc_app* app = malloc(sizeof(struct disc_app));
    if (app == NULL) return app;
    app->curl_handle = curl_easy_init();
    app->parser = disc_json_parser_init();
    app->auth_header = NULL;

    curl_easy_setopt(app->curl_handle, CURLOPT_USERAGENT, "DiscordBot ("LIBDISC_URL", "LIBDISC_VER")");
    curl_easy_setopt(app->curl_handle, CURLOPT_WRITEFUNCTION, disc_app_writecallback);
    curl_easy_setopt(app->curl_handle, CURLOPT_WRITEDATA, app);
    return app;
};



void disc_app_free(struct disc_app* app)
{
    curl_slist_free_all(app->auth_header);
    curl_easy_cleanup(app->curl_handle);
    disc_json_parser_free(app->parser);
    free(app);
}

void disc_app_settoken(struct disc_app* app, const char* token)
{
    char buf[100] = "Authorization: Bot ";
    strcat_s(buf, 100, token);
    app->auth_header = curl_slist_append(app->auth_header, buf);
    
    CURLcode t = curl_easy_setopt(app->curl_handle, CURLOPT_HTTPHEADER, app->auth_header);
}

int disc_app_get(struct disc_app* app, const char* url)
{
    curl_easy_setopt(app->curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(app->curl_handle, CURLOPT_HTTPGET, 1);
    disc_json_parser_reset(app->parser);
    CURLcode res = curl_easy_perform(app->curl_handle);
    long rescode = 0;
    curl_easy_getinfo(app->curl_handle, CURLINFO_RESPONSE_CODE, &rescode);
    return rescode;
}

int disc_app_post(struct disc_app* app, const char* url)
{
    curl_easy_setopt(app->curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(app->curl_handle, CURLOPT_POST, 1);
    disc_json_parser_reset(app->parser);
    CURLcode res = curl_easy_perform(app->curl_handle);
    long rescode = 0;
    curl_easy_getinfo(app->curl_handle, CURLINFO_RESPONSE_CODE, &rescode);
    return rescode;
}

void disc_app_client_cred_grant(struct disc_app* app, const char* id, const char* secret)
{
    char userpwd[100]; 
    if (sprintf_s(userpwd, 100, "%s:%s", id, secret) < 1)
        return;
    curl_easy_setopt(app->curl_handle, CURLOPT_USERPWD, userpwd);
    curl_easy_setopt(app->curl_handle, CURLOPT_COPYPOSTFIELDS, "grant_type=client_credentials&scope=identify");
    disc_app_post(app, LIBDISC_ENDPOINT("/oauth2/token"));
}

// oauth pleas;e ;;;
int disc_app_authorize(struct disc_app* app)
{
    return 0;
}

