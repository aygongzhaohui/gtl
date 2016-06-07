/**
 * @file state_machineT.h
 * @brief   
 * @author GongZhaohui
 * @version 
 * @date 2016-06-07
 */

#ifndef _GTL_STATE_MACHINE_H_
#define _GTL_STATE_MACHINE_H_

namespace gtl
{


/**
 * @class Action
 * @brief Action class
 *      Use to wrap the static function template: doAction
 */
class Action
{
public:
    /**
     * @brief  Need implement by user, to handle the input event
     *   in the state st. "context" object is an instance of StateMachine
     *   and could change the state by it.
     *
     * @tparam CONTEXT
     * @tparam EVENT
     * @tparam STATE
     * @param   context
     * @param   st
     * @param   e
     *
     * @return  
     *   1: could recieve more event
     *   0: to be in an end state
     *  -1: error
     */
    template <class CONTEXT, typename EVENT, class STATE>
    static int doAction(CONTEXT & context, STATE & st, EVENT & e)
    {
        return 0;
    }
};// end-class Action

/**
 * @class  IStateT
 * @brief  Abstract class template: IStateT
 *
 * @tparam CONTEXT
 *   The type of context
 * @tparam EVENT
 *   The type of event
 */
template <class CONTEXT, typename EVENT>
class IStateT
{
public:
    /**
     * @brief   This interface define the abstract function
     *          for different state class
     *
     * @param   context
     *          A reference of the state holder
     * @param   e
     *          An event to trigger the action
     *
     * @return  
     *   1: could recieve more event
     *   0: to be in an end state
     *  -1: error
     */
    virtual int action(CONTEXT & context, EVENT & e) = 0;
};// end-class IStateT


/**
 * @class  StateT 
 * @brief  State class template
 *
 * @tparam CONTEXT
 *         The type of state holder
 * @tparam EVENT
 *         The type of event
 * @tparam STATE
 *         The type of real state class
 */
template <class CONTEXT, typename EVENT, class STATE>
class StateT : public IStateT<CONTEXT, EVENT>
{
public:
    /**
     * @brief   Implement of the IState interface
     * Using template function "doAction" to implement flexible polymorphism
     *
     * @param   context
     * @param   e
     *
     * @return  
     */
    int action(CONTEXT & context, EVENT & e)
    {
		STATE st;
        return Action::doAction(context, st, e);
    }
};// end-class StateT

/**
 * @class StateMachine
 * @brief  A class template 
 *
 * @tparam EVENT
 *   The type of the event
 */
template <typename EVENT>
class StateMachine
{
public:
    typedef IStateT<StateMachine<EVENT>, EVENT> IState;

public:
    StateMachine() : p_state_(NULL)
    {
    }

    /**
     * @brief   Set the state to the context
     *
     * @param   s
     *   The state will set to.
     */
    void setState(IState & s)
    {
        p_state_ = &s;
    }

    /**
     * @brief   Using current state to handle the event
     *
     * @param   e
     *   The input event.
     *
     * @return
     *   1: could recieve more event
     *   0: to be in an end state
     *  -1: error
     */
    int action(EVENT & e)
    {
        if (!p_state_) return -1; 
        return p_state_->action(*this, e);
    }

private:
    IState * p_state_;
};// end-class StateMachine



}// end-namespace gtl


/**
 * @def     DECLARE_STATE(stateName, eventType)
 * @brief   Declare a real state by this macro
 * @param   stateName
 *  The state name to indicate the state, and can be used
 *  in the program.
 * @param   eventType
 *  The type of the event.
 */
#define DECLARE_STATE(stateName, eventType) \
	class C##stateName : public gtl::StateT<gtl::StateMachine<eventType>, eventType, C##stateName> \
	{} stateName;

/**
 * @def     IMPL_STATE_ACTION(stateName, eventType)
 * @brief   The prefix of implement of the state action.
 *   Specific code could write in the "{}".
 *   Var context can be used as the StateMachine;
 *   Var e can be used as the input event.
 * @param   stateName
 *  The state name to indicate the state, and can be used
 *  in the program.
 * @param   eventType
 *  The type of the event.
 */
#define IMPL_STATE_ACTION(stateName, eventType) \
    template<> int gtl::Action::doAction(gtl::StateMachine<eventType> & context,\
										C##stateName & state, eventType & e)


#endif



