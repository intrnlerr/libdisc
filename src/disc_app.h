#ifndef LIBDISC_APP_H
#define LIBDISC_APP_H

#include "curl/curl.h"
#include "disc_types.h"
#include "disc_json.h"

#define LIBDISC_DISCORDAPI_URL "https://discord.com/api/v6"
#define LIBDISC_ENDPOINT(url) LIBDISC_DISCORDAPI_URL ## url
#define LIBDISC_CREATEMESSAGE(channel) LIBDISC_ENDPOINT("/channels/") ## channel ## "/messages"

// oauth i guess
struct disc_app
{
    CURL* curl_handle;
    struct curl_slist* auth_header;
    struct disc_json_parser* parser;
};

LIBDISC_EXPORT struct disc_app* disc_app_init();
LIBDISC_EXPORT void disc_app_free(struct disc_app* app);

LIBDISC_EXPORT void disc_app_settoken(struct disc_app* app, const char* token);
// actually im bad at reading
// LIBDISC_EXPORT void disc_app_client_cred_grant(struct disc_app* app, const char* id, const char* secret);
// LIBDISC_EXPORT int disc_app_authorize(struct disc_app* app);
LIBDISC_EXPORT int disc_app_get(struct disc_app* app, const char* endpoint);
LIBDISC_EXPORT int disc_app_post(struct disc_app* app, const char* endpoint);
LIBDISC_EXPORT int disc_app_post_json(struct disc_app* app, const char* endpoint, const char* data, size_t size);

#endif // !LIBDISC_APP_H