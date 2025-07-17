#include <stdlib.h>
#include "sip_messages.h"

const char expected_register_req[] = 
    "REGISTER sip:sipserver.addr SIP/2.0\r\n"
    "From: \"FomodonTest\" <sip:fomodon_test@sipserver.addr>;tag=\"yMW(4f\"\r\n"
    "To: \"FomodonTest\" <sip:fomodon_test@sipserver.addr>\r\n"
    "Call-ID: \"A^9}Wl\"\r\n"
    "CSeq: 1\r\n"
    "\r\n";

int setup_sip_messages_test(void** state)
{
    srand(6543210);
    sip_initialize_context();
    return 0;
}

void test__sip_prepare_register_request__should_fail_on_zero_size_buffer(void** state)
{
    char buf[1];
    int result = sip_prepare_register_request(buf, 0);
    assert_int_equal(0, result);
}

void test__sip_prepare_register_request__should_fail_on_too_small_nonzero_size_buffer(void** state)
{
    char buf[64];
    int result = sip_prepare_register_request(buf, sizeof(buf));
    assert_true(result < 0);
}

void test__sip_prepare_register_request__should_succeed_and_prepare_register_request_with_big_enough_buffer(void** state)
{
    char buf[256];
    int result = sip_prepare_register_request(buf, sizeof(buf));
    assert_true(result > 0);
    assert_int_equal(sizeof(expected_register_req), result);
    assert_string_equal(expected_register_req, buf);
}