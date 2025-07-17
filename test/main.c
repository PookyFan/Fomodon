#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "sip_messages_test.h"

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test__sip_prepare_register_request__should_fail_on_zero_size_buffer, setup_sip_messages_test),
        cmocka_unit_test_setup(test__sip_prepare_register_request__should_fail_on_too_small_nonzero_size_buffer, setup_sip_messages_test),
        cmocka_unit_test_setup(test__sip_prepare_register_request__should_succeed_and_prepare_register_request_with_big_enough_buffer, setup_sip_messages_test),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}