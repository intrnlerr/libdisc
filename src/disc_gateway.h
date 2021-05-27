#include "disc_endpoint.h"

#define DISC_INTENT_GUILDS                   (1 << 0)
#define DISC_INTENT_GUILD_MEMBERS            (1 << 1)
#define DISC_INTENT_GUILD_BANS               (1 << 2)
#define DISC_INTENT_GUILD_EMOJIS             (1 << 3)
#define DISC_INTENT_GUILD_INTEGRATIONS       (1 << 4)
#define DISC_INTENT_GUILD_WEBHOOKS           (1 << 5)
#define DISC_INTENT_GUILD_INVITES            (1 << 6)
#define DISC_INTENT_GUILD_VOICE_STATES       (1 << 7)
#define DISC_INTENT_GUILD_PRESENCES          (1 << 8)
#define DISC_INTENT_GUILD_MESSAGES           (1 << 9)
#define DISC_INTENT_GUILD_MESSAGE_REACTIONS  (1 << 10)
#define DISC_INTENT_GUILD_MESSAGE_TYPING     (1 << 11)
#define DISC_INTENT_DIRECT_MESSAGES          (1 << 12)
#define DISC_INTENT_DIRECT_MESSAGE_REACTIONS (1 << 13)
#define DISC_INTENT_DIRECT_MESSAGE_TYPING    (1 << 14)

typedef int disc_gateway_callback(struct disc_json_object* msg, struct disc_gateway_con* con, void* user);

enum disc_encoding_type
{
    json,
    etf // unsuported
};

struct disc_gateway_con;

LIBDISC_EXPORT struct disc_gateway_con* disc_gateway_con_init(struct disc_app* app, const char* token, disc_gateway_callback* cb, void* user_data);
LIBDISC_EXPORT void disc_gateway_con_free(struct disc_gateway_con* con);

LIBDISC_EXPORT int disc_gateway_service(struct disc_gateway_con* c);
