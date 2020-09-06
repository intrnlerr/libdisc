#include <curl/curl.h>

#define LIBDISC_URL "https://github.com/intrnlerr/libdisc"
#define LIBDISC_VER "0.1"

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

__declspec(dllexport) struct disc_app* disc_app_init();
__declspec(dllexport) void disc_app_free(struct disc_app* app);
// actually hell
__declspec(dllexport) int disc_app_authorize(struct disc_app* app);
__declspec(dllexport) int disc_app_get(struct disc_app* app, const char* endpoint);