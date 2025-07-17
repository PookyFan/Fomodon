#include <stdio.h>
#include <string.h>
#include "hal.h"
#include "SIPConfig.h"
#include "sip_messages.h"

#define STRSIZE(s) sizeof(s)-1
#define MSGPART(s) {s,STRSIZE(s)}
#define VARMSGPART(s) {s,0}
#define MSGPARTRAW(s) {s,sizeof(s)}

#define RANDOM_STRING_SIZE 8
#define CRLFPART MSGPART(crlf)
#define MESSAGE_PART_SENTINEL {NULL, 0}

struct auth_challenge_data
{
    char* realm;
    char* nonce;
    char* opaque;
    char* qop;
};

struct sip_context
{
    struct auth_challenge_data challenge_data;
    unsigned int cseq_int;
    char cseq_str[4];
    char tag[RANDOM_STRING_SIZE];
    char call_id[RANDOM_STRING_SIZE];
};

typedef struct message_part
{
    const char* part_string;
    unsigned int part_size; //Excluding terminating null character
} message_part_t;

struct sip_context sip_data;

const char crlf[] = "\r\n";

const char register_request_line[] = "REGISTER sip:" SIP_SERVER_ADDR " SIP/2.0";

const char register_identity_part[] = "\"" SIP_CONTACT_NAME "\" <sip:" SIP_USERNAME "@" SIP_SERVER_ADDR ">";

const char register_tag_part[] = ";tag=";

const char register_to_header[] = "To: ";

const char register_from_header[] = "From: ";

const char register_call_id_header[] = "Call-ID: ";

const char register_cseq_header[] = "CSeq: ";

const char register_contact_header[] = "Contact: ";

const char register_accept_header[] = "Accept: text/plain";

const char register_authorization_header[] = "Authorization: ";

const message_part_t register_msg_parts[] = {
    MSGPART(register_request_line), CRLFPART,
    MSGPART(register_from_header), MSGPART(register_identity_part), MSGPART(register_tag_part), MSGPARTRAW(sip_data.tag), CRLFPART,
    MSGPART(register_to_header), MSGPART(register_identity_part), CRLFPART,
    MSGPART(register_call_id_header), MSGPARTRAW(sip_data.call_id), CRLFPART,
    MSGPART(register_cseq_header), VARMSGPART(sip_data.cseq_str), CRLFPART,

    //todo: non-mandatory headers like Contact and Accept

    MESSAGE_PART_SENTINEL
};

inline static bool auth_data_present()
{
    return NULL != sip_data.challenge_data.realm;
}

static void generate_random_tag(unsigned char tag_buf[RANDOM_STRING_SIZE])
{
    uint32_t rand_high = hal_rand();
    uint32_t rand_low = hal_rand();
    memcpy(tag_buf, &rand_high, sizeof(rand_high));
    memcpy(tag_buf + sizeof(rand_high), &rand_low, sizeof(rand_low));

    //Since we need to generate printable characters only, we will first
    //fix random values to be between 0 and 93, and then add decimal value
    //of first character we allow to be generated, which is 33 ('!' character).
    //As 93 + 33 == 126 (last printable's decimal value), we end up generating printable ASCII only.
    //We don't use space (with decimal value of 32) to have continuous random string.

    const char rand_first = '!';
    const char rand_max = 126 - rand_first + 1; //Add 1 since we're going to modulo divide by it
    unsigned int i;
    for(i = 1; i < RANDOM_STRING_SIZE - 1; ++i)
    {
        unsigned char curr_char = (tag_buf[i] % rand_max) + rand_first;
        tag_buf[i] = (curr_char == '"') ? rand_first : curr_char; //We also don't want '"' as it marks tag's boundary
    }

    tag_buf[0] = tag_buf[RANDOM_STRING_SIZE - 1] = '"';
}

//Assemble message from parts, and return total length of the assembled message
static int assemble_message(char* msg_buf, unsigned int buf_size, const message_part_t* msg_parts_ptr)
{
    int current_msg_size = 0;
    do
    {
        const char* part_string = msg_parts_ptr->part_string;
        unsigned int size = msg_parts_ptr->part_size;
        if(0 == size)
            size = strlen(part_string);

        if((current_msg_size + size) > buf_size)
            return -current_msg_size;

        memcpy(msg_buf + current_msg_size, part_string, size);
        current_msg_size += size;
    } while (NULL != (++msg_parts_ptr)->part_string);

    if(auth_data_present())
    {
        //todo: insert auth data, and increment current_msg_size accordingly
    }

    return current_msg_size;
}

static void increment_cseq()
{
    snprintf(sip_data.cseq_str, sizeof(sip_data.cseq_str), "%u", ++sip_data.cseq_int);
}

void sip_initialize_context()
{
    memset(&sip_data, 0, sizeof(sip_data));
    generate_random_tag(sip_data.call_id);
    generate_random_tag(sip_data.tag);
}

int sip_prepare_register_request(char* msg_buf, unsigned int buf_size)
{
    increment_cseq();
    int header_size = assemble_message(msg_buf, buf_size, register_msg_parts);
    if(header_size <= 0)
    {
        //todo: log it as error (or maybe one level above?)
        return header_size;
    }

    int msg_end_mark_size = sizeof(crlf);
    if((header_size + msg_end_mark_size) > buf_size)
    {
        //todo: log it as error (or maybe one level above?)
        return -(header_size + msg_end_mark_size);
    }

    memcpy(msg_buf + header_size, crlf, msg_end_mark_size);
    return header_size + msg_end_mark_size;
}