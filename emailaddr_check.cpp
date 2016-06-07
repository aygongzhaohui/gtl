/**
 * @file emailaddr_check.cpp
 * @brief
 * @author GongZhaohui
 * @version
 * @date 2016-06-07
 */

#include <stdio.h>
#include <ctype.h>
#include "state_machineT.h"

DECLARE_STATE(EmailAddrBegin, char);
DECLARE_STATE(EmailAddrEnd, char);

IMPL_STATE_ACTION(EmailAddrBegin, char)
{
    if (isalnum(e))
    {
        context.setState(EmailAddrEnd);
        printf("EmailAddrBegin change to EmailAddrEnd\n");
        return 1;
    }
    return -1;
}

IMPL_STATE_ACTION(EmailAddrEnd, char)
{
    if (isalnum(e))
    {
        printf("EmailAddrEnd change to finish\n");
        return 0;
    }
    return -1;
}

int main()
{
    gtl::StateMachine<char> stMachine;
    stMachine.setState(EmailAddrBegin);
    char c = 'a';
    int ret = 0;
    while ((ret = stMachine.action(c)) == 1);
    return 0;
}



