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

typedef char EmailAddrStEvent;
typedef gtl::StateMachine<EmailAddrStEvent> EmailAddrStMach;

DECLARE_STATE(EmailAddrBegin, EmailAddrStEvent);
DECLARE_STATE(EmailAddrUserName, EmailAddrStEvent);
DECLARE_STATE(EmailAddrDomainBegin, EmailAddrStEvent);
DECLARE_STATE(EmailAddrEnd, EmailAddrStEvent);


IMPL_STATE_ACTION(EmailAddrBegin, EmailAddrStEvent)
{
    if (isalnum(event))
    {
        context.setState(EmailAddrEnd);
        printf("EmailAddrBegin change to EmailAddrEnd\n");
        return 1;
    }
    printf("EmailAddrBegin failed\n");
    return -1;
}

IMPL_STATE_ACTION(EmailAddrEnd, EmailAddrStEvent)
{
    if (isalnum(event))
    {
        printf("EmailAddrEnd change to finish\n");
        return 0;
    }
    printf("EmailAddrEnd failed\n");
    return -1;
}

int main()
{
    EmailAddrStMach stMachine;
    stMachine.setState(EmailAddrBegin);
    EmailAddrStEvent c = 'a';
    int ret = 0;
    while ((ret = stMachine.action(c)) == 1)
    {
        c = '.';
    }
    return 0;
}



