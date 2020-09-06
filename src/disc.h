#include <curl/curl.h>

// fuck
int disc_init()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}