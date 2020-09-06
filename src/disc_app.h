#include <curl/curl.h>

// oauth i guess
struct disc_app
{
    CURL* curl_handle;
    const char* access_token;
    const char* refresh_token;
    const char* scope;
};

struct disc_app* disc_app_init();
void disc_app_free(struct disc_app* app);
// actually hell
int disc_app_authorize(struct disc_app* app);