/**
 * @file emailaddr_check.cpp
 * @brief
 * @author GongZhaohui
 * @version
 * @date 2016-06-07
 */

#include <stdio.h>
#include <ctype.h>
#include <string>
#include "state_machineT.h"

#ifdef _DEBUG_
#define _DEBUG_PRINTF_ printf
#else
#define _DEBUG_PRINTF_
#endif

#define MIN_EMAILADDR_LEN    (3)

typedef char EmailAddrStEvent;
typedef gtl::StateMachine<EmailAddrStEvent> EmailAddrStMach;

/**
 * @brief State EmailAddrBegin
 *  First character of email addr must be digit or letter
 */
DECLARE_STATE(EmailAddrBegin, EmailAddrStEvent);

/**
 * @brief State EmailAddrUserName
 *  regex: [a-zA-Z\._0-9]
 */
DECLARE_STATE(EmailAddrUserName, EmailAddrStEvent);

/**
 * @brief State EmailAddrDomainBegin
 *  First charactor of domain name, after '@' or '.'
 *  regex: [a-zA-Z0-9]
 */
DECLARE_STATE(EmailAddrDomainBegin, EmailAddrStEvent);

/**
 * @brief State EmailAddrDomainBegin
 *  regex: [a-zA-Z0-9\-]
 */
DECLARE_STATE(EmailAddrDomain, EmailAddrStEvent);

/**
 * @brief State EmailAddrEnd
 *  regex: [a-zA-Z0-9\-]
 */
//DECLARE_STATE(EmailAddrEnd, EmailAddrStEvent);


IMPL_STATE_ACTION(EmailAddrBegin, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event))
    {// change to state EmailAddrUserName
        context.setState(EmailAddrUserName);
        _DEBUG_PRINTF_("EmailAddrBegin change to EmailAddrUserName\n");
        return 1;
    }
    return -1;
}

IMPL_STATE_ACTION(EmailAddrUserName, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event) || event == '.' || event == '\\')
    {
        _DEBUG_PRINTF_("Stay in EmailAddrUserName\n");
        return 1;
    }
    else if (event == '@')
    {// change to state EmailAddrDomainBegin
        context.setState(EmailAddrDomainBegin);
        return 1;
    }
    _DEBUG_PRINTF_("EmailAddrUserName failed\n");
    return -1;
}

IMPL_STATE_ACTION(EmailAddrDomainBegin, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event))
    {// change to state EmailAddrDomain
        context.setState(EmailAddrDomain);
        _DEBUG_PRINTF_("EmailAddrDomainBegin change to EmailAddrDomain\n");
        return 0;// EmailAddrDomainBegin is an end state
    }
    _DEBUG_PRINTF_("EmailAddrDomainBegin failed\n");
    return -1;
}

IMPL_STATE_ACTION(EmailAddrDomain, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event) || event == '-')
    {
        _DEBUG_PRINTF_("Stay in EmailAddrUserName\n");
        return 0;// EmailAddrDomain is an end state
    }
    else if (event == '.')
    {// change to state EmailAddrDomainBegin, next domain name
        context.setState(EmailAddrDomainBegin);
        _DEBUG_PRINTF_("EmailAddrDomain change to EmailAddrDomainBegin\n");
        return 1;
    }
    _DEBUG_PRINTF_("EmailAddrDomain failed\n");
    return -1;
}

bool validEmailAddr(std::string emailAddr)
{
    EmailAddrStMach stMachine;
    stMachine.setState(EmailAddrBegin);
    if (emailAddr.length() < MIN_EMAILADDR_LEN)
        return false;
    int ret = -1;
    for (unsigned i = 0; i < emailAddr.length(); ++i)
    {
        EmailAddrStEvent c = emailAddr.at(i);
        ret = stMachine.action(c);
        if (ret < 0) return false;
    }
    if (!ret) return true;
    return false;
}

int main(int argc, char ** argv)
{
    if (validEmailAddr(argv[1]))
        printf("Good! %s is an valid email address\n", argv[1]);
    else
        printf("Bad! %s isn't an valid email address\n", argv[1]);
    return 0;
}



