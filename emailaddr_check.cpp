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
 *
 * This state expect next input event to be the first
 * charactor of the user name.
 */
DECLARE_STATE(EmailAddrBegin, EmailAddrStEvent);

/**
 * @brief State EmailAddrUserName
 * 
 * This state expect the next input event to be the charactor
 * in the user name, not the first char.
 */
DECLARE_STATE(EmailAddrUserName, EmailAddrStEvent);

/**
 * @brief State EmailAddrLastDomainBegin
 *
 * This state expect the next input event to be the first
 * char of the last domain name.
 * In domain secion of email addr, there is at least two domain name.
 */
DECLARE_STATE(EmailAddrLastDomainBegin, EmailAddrStEvent);

/**
 * @brief State EmailAddrLastDomain
 *
 * This state expect the next input event to be the char of
 * the last domain name, not the first char.
 */
DECLARE_STATE(EmailAddrLastDomain, EmailAddrStEvent);

/**
 * @brief State EmailAddrDomainBegin
 *
 * This state expect the next input event to be the first char 
 * of the other(not the last) domain name.
 */
DECLARE_STATE(EmailAddrDomainBegin, EmailAddrStEvent);


/**
 * @brief State EmailAddrDomain
 *
 * This state expect the next input event to be the char 
 * of the other(not the last) domain name, not the first char.
 */
DECLARE_STATE(EmailAddrDomain, EmailAddrStEvent);


// It is not perfect: 
// We need put the template specializtion to the gtl namespace.
// VC2012 can accept without doing this, but g++ 4.4.7 couldn't.
namespace gtl
{

/**
 * @brief State EmailAddrBegin
 *  First character of email addr must be digit or letter
 */
IMPL_STATE_ACTION(EmailAddrBegin, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event))
    {// change to state EmailAddrUserName
        context.setState(EmailAddrUserName);
        _DEBUG_PRINTF_("EmailAddrBegin change to EmailAddrUserName\n");
        return Action::AS_CONTINUE;
    }
    return Action::AS_ERROR;
}

/**
 * @brief State EmailAddrUserName
 *  regex: [a-zA-Z\._0-9]
 */
IMPL_STATE_ACTION(EmailAddrUserName, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event) || event == '.' || event == '_')
    {
        _DEBUG_PRINTF_("Stay in EmailAddrUserName\n");
        return Action::AS_CONTINUE;
    }
    else if (event == '@')
    {// change to state EmailAddrLastDomainBegin
        context.setState(EmailAddrLastDomainBegin);
        return Action::AS_CONTINUE;
    }
    _DEBUG_PRINTF_("EmailAddrUserName failed\n");
    return Action::AS_ERROR;
}

/**
 * @brief State EmailAddrLastDomainBegin
 *  First charactor of domain name, after '@'
 *  regex: [a-zA-Z0-9]
 */
IMPL_STATE_ACTION(EmailAddrLastDomainBegin, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event))
    {// change to state EmailAddrDomain
        context.setState(EmailAddrLastDomain);
        _DEBUG_PRINTF_("EmailAddrLastDomainBegin change to EmailAddrLastDomain\n");
        return Action::AS_CONTINUE;
    }
    _DEBUG_PRINTF_("EmailAddrLastDomainBegin failed\n");
    return Action::AS_ERROR;
}

/**
 * @brief State EmailAddrLastDomain
 *  regex: [a-zA-Z0-9\-]
 */
IMPL_STATE_ACTION(EmailAddrLastDomain, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event) || event == '-')
    {
        _DEBUG_PRINTF_("Stay in EmailAddrUserName\n");
        return Action::AS_CONTINUE;
    }
    else if (event == '.')
    {// change to state EmailAddrDomainBegin, next domain name
        context.setState(EmailAddrDomainBegin);
        _DEBUG_PRINTF_("EmailAddrLastDomain change to EmailAddrDomainBegin\n");
        return Action::AS_CONTINUE;
    }
    _DEBUG_PRINTF_("EmailAddrLastDomain failed\n");
    return Action::AS_ERROR;
}

/**
 * @brief State EmailAddrDomainBegin
 *  regex: [a-zA-Z0-9\-]
 */
IMPL_STATE_ACTION(EmailAddrDomainBegin, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event))
    {// change to state EmailAddrDomain
        context.setState(EmailAddrDomain);
        _DEBUG_PRINTF_("EmailAddrDomainBegin change to EmailAddrDomain\n");
        return Action::AS_END;// EmailAddrDomain is an end state
    }
    _DEBUG_PRINTF_("EmailAddrDomainBegin failed\n");
    return Action::AS_ERROR;
}

/**
 * @brief State EmailAddrDomain
 *  regex: [a-zA-Z0-9\-]
 */
IMPL_STATE_ACTION(EmailAddrDomain, EmailAddrStEvent)
{
    state.dummy();// avoid warning
    if (isalnum(event) || event == '-')
    {
        _DEBUG_PRINTF_("Stay in EmailAddrUserName\n");
        return Action::AS_END;// EmailAddrDomain is an end state
    }
    else if (event == '.')
    {// change to state EmailAddrDomainBegin, next domain name
        context.setState(EmailAddrDomainBegin);
        _DEBUG_PRINTF_("EmailAddrDomain change to EmailAddrDomainBegin\n");
        return Action::AS_CONTINUE;
    }
    _DEBUG_PRINTF_("EmailAddrDomain failed\n");
    return Action::AS_ERROR;
}

}// end-namespace gtl


bool validEmailAddr(std::string emailAddr)
{
    if (emailAddr.length() < MIN_EMAILADDR_LEN)
        return false;
    EmailAddrStMach stMachine(emailAddr.c_str(), emailAddr.length());
    return stMachine.run(EmailAddrBegin);
}



