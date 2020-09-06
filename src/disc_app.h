#ifndef LIBDISC_APP_H
#define LIBDISC_APP_H

#include "curl/curl.h"
#include "disc_types.h"



#define LIBDISC_DISCORDAPI_URL "https://discord.com/api/v6"
#define LIBDISC_ENDPOINT(url) LIBDISC_DISCORDAPI_URL ## url

// oauth i guess
struct disc_app
{
    CURL* curl_handle;

    const char* access_token;
    const char* refresh_token;
    const char* scope;
};

LIBDISC_EXPORT struct disc_app* disc_app_init();
LIBDISC_EXPORT void disc_app_free(struct disc_app* app);
// actually hell
LIBDISC_EXPORT int disc_app_authorize(struct disc_app* app);
LIBDISC_EXPORT int disc_app_get(struct disc_app* app, const char* endpoint);

#endif // !LIBDISC_APP_H