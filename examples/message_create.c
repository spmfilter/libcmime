#include <stdio.h>

#include "../src/cmime_message.h"

int main(int argc, char const *argv[]) {
    CMimeMessage_T *msg = cmime_message_new();

    cmime_message_free(msg);
    return 0;
}