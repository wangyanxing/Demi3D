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

#ifndef IntCondition_h__
#define IntCondition_h__

#include "BehaviorTree/Common.h"

namespace BehaviorTree
{
	/// These are the numerical tests that are available for integers
	enum INT_TEST {LESS_THAN,GREATER_THAN,LESS_OR_EQ,GREATER_OR_EQ,EQUAL,NOT_EQUAL};
	template <class T = NoClass>
	/// Wraps a function or member pointer that returns an integer value into a conditional node
	/** To wrap a function pointer or static class member that takes 
    no arguments and returns an int, instantiate IntCondition without a type argument.
	For example, to wrap the function "func", with the signature "int func()", one would do this:
	IntCondition<> condition(&func,EQUAL,5);

	To wrap the static method "sMethod" of the class "Class", one would do this:
	IntCondition<> condition(&Class::sMethod,EQUAL,5);

	To wrap a non-static class member, IntCondition must be parameterized with the class's type.
	For example, to wrap the method "method" of class "Class", one would do this:
	IntCondition<Class> condition(&Class:method,EQUAL,5);
	**/
	class IntCondition: public BehaviorTreeNode
	{
	public:
		/// Runs the test given in the constructor, and returns BT_SUCCESS if the test passes, or BT_FAILURE if the test fails.
		BEHAVIOR_STATUS execute(void* agent)
		{
			T* obj = (T*) agent;
			bool status;
			int objVal = (obj->*func)();
			switch (test)
			{
				case LESS_THAN:		status = (objVal < val); break;
				case GREATER_THAN:	status = (objVal > val); break;
				case LESS_OR_EQ:	status = (objVal <= val); break;
				case GREATER_OR_EQ: status = (objVal >= val); break;
				case EQUAL:			status = (objVal == val); break;
				case NOT_EQUAL:		status = (objVal != val); break;
			}

			if (status)
				return BT_SUCCESS;
			else
				return BT_FAILURE;
		};

		void init(void* agent)
		{
		};
		/** \param _func the address of the class member
			\param _test the mathematical operation to perform on the return value of _func
			\param _val the 'right side' of the mathematical expression the node performs
		*/
		IntCondition(int(T::* const _func)(), INT_TEST _test, int _val) : func(_func), func2(NULL)
		{
			test = _test;
			val = _val;
		}
		/** \param _func the address of the function or static class member
			\param _test the mathematical operation to perform on the return value of _func
			\param _val the 'right side' of the mathematical expression the node performs
		*/
		IntCondition(int(* const _func)(), INT_TEST _test, int _val) : func2(_func), func(NULL)
		{
			test = _test;
			val = _val;
		}
		/** \param _func the address of the const class member
			\param _test the mathematical operation to perform on the return value of _func
			\param _val the 'right side' of the mathematical expression the node performs
		*/
		IntCondition(int(T::* const _func)() const, INT_TEST _test, int _val) : func(reinterpret_cast<int(T::* const)()>(_func)), func2(NULL)
		{
			test = _test;
			val = _val;
		}
	private:
		int (T::* const func)() ; //A member function pointer of the class T, being stored as const since we don't modify it
		int (* const func2)();   //A static method or function pointer, being stored as const since we don't modify it
		INT_TEST test;
		int val;

	};

	template<>
	BEHAVIOR_STATUS IntCondition<NoClass>::execute(void* agent)
	{
		bool status;
		switch (test)
		{
		case LESS_THAN:		status = ((*func2)() < val); break;
		case GREATER_THAN:	status = ((*func2)() > val); break;
		case LESS_OR_EQ:	status = ((*func2)() <= val); break;
		case GREATER_OR_EQ: status = ((*func2)() >= val); break;
		case EQUAL:			status = ((*func2)() == val); break;
		case NOT_EQUAL:		status = ((*func2)() != val); break;
		}

		if (status)
			return BT_SUCCESS;
		else
			return BT_FAILURE;
	}
}
#endif // IntCondition_h__