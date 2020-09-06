
enum disc_encoding_type
{
    json,
    etf
};

struct disc_gateway_con 
{
    int v;
    disc_encoding_type encoding;
    unsigned char compression;
};