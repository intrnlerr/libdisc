#include "disc_gateway.h"
#include <libwebsockets.h>

struct disc_gateway_con {
    struct lws_context* context;
    struct disc_json_parser* parser;
    enum disc_encoding_type encoding;
    const char* token;
    unsigned char compression;
    uint16_t intents;
    uint32_t last_sequence;
    uint32_t heartbeat;
    uint8_t ack_count;
    void* user_data;
    disc_gateway_callback* user_callback;
};

static void send_identify_payload(struct lws* wsi)
{
    
}

// TODO: make this less bad

static int disc_gateway_def_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    struct disc_gateway_con* connection = user;
    switch (reason)
    {
    case LWS_CALLBACK_PROTOCOL_INIT:
        printf("callback_init\n");
        // some init things here i guess
        break;
    case LWS_CALLBACK_CLIENT_CLOSED:
        // TODO: reconnection
        printf("connection to gateway closed\n");
        break;
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        if (in != NULL)
        {
            fwrite(in, 1, len, stdout);
        }
        break;
    case LWS_CALLBACK_CLIENT_RECEIVE:
        // handle discord event
        // printf("%.*s\n", len, (char*)in);
        disc_json_parse(connection->parser, in, len);
        
        struct disc_json_object* root = disc_json_queue_dequeue(connection->parser->queue);
        if (!root)
            break;
        struct disc_json_value* v_op = disc_json_object_get(root, "op");
        int op = (int)v_op->data.number;
        
        

        switch (op)
        {
        case 0:
            connection->last_sequence = disc_json_object_get(root, "s")->data.number;
            if (connection->user_callback)
                connection->user_callback(root, connection, connection->user_data);
            return 0;
        case 9:
            // TODO: reconnection
            // invalid session
            return 1;
        case 10:
            // get heartbeat interval
            struct disc_json_value* d = disc_json_object_get(root, "d");
            connection->heartbeat = disc_json_object_get(d->data.object, "heartbeat_interval")->data.number;
            // send identify payload
            char identify_buf[LWS_PRE + 200];
            int written = sprintf_s(identify_buf + LWS_PRE, 200, "{\"op\":2,\"d\":{\"token\":\"%s\",\"properties\":{\"$os\":\""DISC_CUR_OS"\",\"$browser\":\"libdisc\",\"$device\":\"libdisc\"}}}", connection->token);
            lws_write(wsi, identify_buf+LWS_PRE, written, LWS_WRITE_TEXT);
            lws_set_timer_usecs(wsi, (uint64_t)connection->heartbeat * 1000);
            break;
        case 11:
            connection->ack_count++;
            break;
        }
        printf("rx: op%d\n", op);
        break;
    case LWS_CALLBACK_TIMER:
        // discord heartbeat
        printf("tx: heartbeat\n");
        if (connection->ack_count == 0)
        {
            // kill connection
            lws_close_reason(wsi, LWS_CLOSE_STATUS_PROTOCOL_ERR, NULL, 0);
            return 1;
        }
        connection->ack_count = 0;
        char buf[LWS_PRE + 100] = "";
        int written = 0;
        if (connection->last_sequence != 0)
        {
            written = sprintf_s(buf + LWS_PRE, 100, "{\"op\":1,\"d\":%u}", connection->last_sequence);
        }
        else
        {
            memcpy(buf + LWS_PRE, "{\"op\":1,\"d\":null}", 18);
            written = 18;
        }
        lws_write(wsi, buf + LWS_PRE, written, LWS_WRITE_TEXT);
        lws_set_timer_usecs(wsi, (uint64_t)connection->heartbeat*1000);
        break;
    }
    return 0;
}

struct disc_gateway_con* disc_gateway_con_init(struct disc_app* app, const char* token, disc_gateway_callback* cb, void* user_data)
{
    struct disc_gateway_con* connection = malloc(sizeof(struct disc_gateway_con));
    if (connection == NULL) return NULL;
    connection->last_sequence = 0;
    connection->parser = disc_json_parser_init();
    connection->token = token;
	connection->user_callback = cb;
    connection->user_data = user_data;
    if (!connection->parser)
    {
        free(connection);
        return NULL;
    }

    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(struct lws_context_creation_info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    struct lws_protocols protocols[] = {
        {"disc_gateway_def_cb", disc_gateway_def_callback, 0, 1024, 0, connection, 0},
        {NULL, NULL, 0, 0,}
    };
    info.protocols = protocols;

    connection->context = lws_create_context(&info);
    struct lws_client_connect_info cinfo;
    memset(&cinfo, 0, sizeof(struct lws_client_connect_info));

    cinfo.context = connection->context;
    cinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_ALLOW_INSECURE;
    disc_get_gateway(app);
    struct disc_json_object* response = disc_json_queue_dequeue(app->parser->queue);
    cinfo.port = 443;
    cinfo.address = response->values[0]->data.string + 6;
    cinfo.host = cinfo.address;
    cinfo.origin = cinfo.address;
    cinfo.path = "/";
    cinfo.protocol = "disc_gateway_def_cb";
    cinfo.userdata = connection;

    printf("connect ws attempt to: %s\n", cinfo.address);
    
    
    struct lws* a = lws_client_connect_via_info(&cinfo);
    if (a == NULL)
    {
        printf("\nconnection failed\n");
    }
    
    return connection;
}

void disc_gateway_con_free(struct disc_gateway_con* con)
{
    lws_context_destroy(con->context);
    free(con);
}

int disc_gateway_service(struct disc_gateway_con* con)
{
    return lws_service(con->context, 1);
}
