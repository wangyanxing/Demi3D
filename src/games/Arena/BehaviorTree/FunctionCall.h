/************************************************************************/
/*                                                                      */
/* This file was adapted from libbehavior                               */
/* Website: https://code.google.com/p/libbehavior/                      */
/* License: http://opensource.org/licenses/BSD-3-Clause                 */
/*                                                                      */
/* Using by Demi3D                                                      */
/*  __  ___  __  __  __                                                 */
/* |  \|_ |\/||   _)|  \                                                */
/* |__/|__|  ||  __)|__/                                                */
/*                                                                      */
/* https://github.com/wangyanxing/Demi3D                                */
/*                                                                      */
/************************************************************************/

#ifndef FunctionCall_h__
#define FunctionCall_h__

#include "BehaviorTree/Common.h"

namespace BehaviorTree
{
	template <class T = NoClass>
	/// Wraps a function or member pointer into a Behavior Tree Node.
	/** To wrap a void function pointer or static class member that takes a generic void* argument , instantiate FunctionCall without a type argument.
		For example, to wrap the function "func", with the signature "void func(void*)" and the argument "arg", one would do this:
		FunctionCall<> functionNode(&func,arg);

		To wrap the static method "sMethod" of the class "Class", one would do this:
		FunctionCall<> staticMethodNode(&Class::sMethod,arg);

		To wrap a non-static class member, FunctionCall must be parameterized with the class's type.
		For example, to wrap the method "method" of class "Class", one would do this:
		FunctionCall<Class> classMethodNode(&Class:method,arg);
	**/
	class FunctionCall: public BehaviorTreeNode
	{
	public:
		BEHAVIOR_STATUS execute(void* agent)
		{
			T* obj = (T*) agent;
			(obj->*func)(arg);
		};

		void init(void* agent){};

		/** \param _func the address of the (non-static) class member
			\param _arg the genericized argument to the function
		*/
		FunctionCall(void(T::*_func)(void* _arg),void* _arg = (void*)NULL) : func(_func), func2(NULL),arg(_arg){}

		/** \param _func the address of the function or the static class member
			\param _arg the genericized argument to the function
		*/
		FunctionCall(void(*_func)(void* _arg),void* _arg = (void*)NULL) : func2(_func), func(NULL),arg(_arg){}

		/** \param _func the address of the const non-static class member
			\param _arg the genericized argument to the function
		*/
		FunctionCall(void(T::* const _func)(void* _arg) const,void* _arg = (void*)NULL) 
            : func(reinterpret_cast<void(T::* const)()>(_func)), func2(NULL),arg(_arg){}

	private:
		void (T::* const func)(void* _arg);
		void (* const func2)(void* _arg);
		void* arg;
	};

	template<>
	BEHAVIOR_STATUS FunctionCall<NoClass>::execute(void* agent)
	{
		(*func2)(arg);
		return BT_SUCCESS;
	}
}
#endif // FunctionCall_h__