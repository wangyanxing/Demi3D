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

#ifndef BoolCondition_h__
#define BoolCondition_h__

#include "BehaviorTree/Common.h"
namespace BehaviorTree
{
	template <class T = NoClass>
	/// Wraps a function or member pointer that returns a boolean value into a conditional node
	/** To wrap a function pointer or static class member that takes no 
        arguments and returns a boolean, instantiate BoolCondition without a type argument.
		For example, to wrap the function "func", with the signature "bool func()", one would do this:
		BoolCondition<> condition(&func,true);

		To wrap the static method "sMethod" of the class "Class", one would do this:
		BoolCondition<> condition(&Class::sMethod,true);

		To wrap a non-static class member, BoolCondition must be parameterized with the class's type.
		For example, to wrap the method "method" of class "Class", one would do this:
		BoolCondition<Class> condition(&Class:method,true);
	**/
	class BoolCondition: public BehaviorTreeNode
	{
	public:
		BEHAVIOR_STATUS execute(void* agent)
		{
			T* obj = (T*) agent;
			if ((obj->*func)() == check)
				return BT_SUCCESS;
			else
				return BT_FAILURE;
		};

		void init(void* agent)
		{
		};
		/** \param _func the address of the (non-static) class member
			\param _check the value that is being tested for
		*/
		BoolCondition(bool(T::*_func)(), bool _check) : func(_func), func2(NULL)
		{
			check = _check;
		}
		/** \param _func the address of the function or the static class member
			\param _check the value that is being tested for
		*/
		BoolCondition(bool(*_func)(), bool _check) : func2(_func), func(NULL)
		{
			check = _check;
		}
		/** \param _func the address of the const non-static class member
			\param _check the value that is being tested for
		*/
		BoolCondition(bool(T::* const _func)() const,bool _check) 
            : func(reinterpret_cast<bool(T::* const)()>(_func)), func2(NULL)
		{
			check = _check;
		}
	private:
		bool (T::* const func)();
		bool (* const func2)();
		bool check;
	};

	template<>
	BEHAVIOR_STATUS BoolCondition<NoClass>::execute(void* agent)
	{
		if ((*func2)() == check)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
}
#endif // BoolCondition_h__