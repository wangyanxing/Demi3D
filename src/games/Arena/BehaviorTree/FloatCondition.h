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

#ifndef FloatCondition_h__
#define FloatCondition_h__

#include "BehaviorTree/Common.h"
#include "BehaviorTree/FPCompare.h"

namespace BehaviorTree
{
	/// These are the numerical tests that are available for floating point numbers.
	/**Because of Floating Point precision and rounding issues, testing for exact equalities 
    is unwise. Therefore, the FloatCondition constructor accepts an optional value to determine how strict equality checks are. */
	enum FLOAT_TEST {LESS_THAN_FP,GREATER_THAN_FP,LESS_OR_CLOSE,GREATER_OR_CLOSE,CLOSE,NOT_CLOSE};
	template <class T = NoClass>

	/// Wraps a function or member pointer that returns a float value into a conditional node
	/** To wrap a function pointer or static class member that takes no arguments
    and returns a float, instantiate FloatCondition without a type argument.
	For example, to wrap the function "func", with the signature "float func()", one would do this:
	FloatCondition<> condition(&func,CLOSE,5.0f);

	To wrap the static method "sMethod" of the class "Class", one would do this:
	FloatCondition<> condition(&Class::sMethod,CLOSE,5.0f);

	To wrap a non-static class member, FloatCondition must be parameterized with the class's type.
	For example, to wrap the method "method" of class "Class", one would do this:
	FloatCondition<Class> condition(&Class:method,CLOSE,5.0f);
	**/
	class FloatCondition: public BehaviorTreeNode
	{
	public:
		/// Runs the test given in the constructor, and returns BT_SUCCESS if the test passes, or BT_FAILURE if the test fails.
		BEHAVIOR_STATUS execute(void* agent)
		{
			float objVal = getObjVal(agent);
			bool status;
			switch (test)
			{
			case LESS_THAN_FP:		status = (objVal < val) && !AlmostEqual2sComplement(objVal,val,ulps); break;
			case GREATER_THAN_FP:	status = (objVal > val) && !AlmostEqual2sComplement(objVal,val,ulps); break;
			case LESS_OR_CLOSE:		status = (objVal <= val)|| AlmostEqual2sComplement(objVal,val,ulps); break;
			case GREATER_OR_CLOSE:	status = (objVal >= val)|| AlmostEqual2sComplement(objVal,val,ulps); break;
			case CLOSE:				status = AlmostEqual2sComplement(objVal,val,ulps); break;
			case NOT_CLOSE:			status = !AlmostEqual2sComplement(objVal,val,ulps); break;
			}

			if (status)
				return BT_SUCCESS;
			else
				return BT_FAILURE;
		};

		void init(void* agent)
		{
		};

		/** \param _func the address of the const class member
			\param _test the mathematical operation to perform on the return value of _func
			\param _val the 'right side' of the mathematical expression the node performs
			\param _ulps the number of "units in the last place" two floating point numbers can differ
            by and still be considered 'close' The default value of 2^25 will consider 5.000000000 and 
            5.000001 to be 'close'. However, the number of digits the numbers can differ by will
            vary depending on their size, due to how they are stored on the computer. 
		*/
		FloatCondition(float(T::* const _func)() const, FLOAT_TEST _test, float _val,int _ulps = 2^22) 
            : func(reinterpret_cast<float(T::* const)()>(_func)), func2(NULL)
		{
			test = _test;
			val = _val;
			ulps = _ulps;
		}

		/** \param _func the address of the class member
			\param _test the mathematical operation to perform on the return value of _func
			\param _val the 'right side' of the mathematical expression the node performs
			\param _ulps the number of "units in the last place" two floating point numbers can differ by and 
            still be considered 'close' The default value of 2^25 will consider 5.000000000 and 5.000001 to be
            'close'. However, the number of digits the numbers can differ by will vary 
            depending on their size, due to how they are stored on the computer. 
		*/
		FloatCondition(float(T::*_func)(), FLOAT_TEST _test, float _val,int _ulps = 2^22)
            : func(_func), func2(NULL)
		{
			test = _test;
			val = _val;
			ulps = _ulps;

		}
		/** \param _func the address of the function or static class member
		\param _test the mathematical operation to perform on the return value of _func
		\param _val the 'right side' of the mathematical expression the node performs
		\param _ulps the number of "units in the last place" two floating point numbers can differ by and still 
        be considered 'close' The default value of 2^25 will consider 5.000000000 and 5.000001 to be 'close'. 
        However, the number of digits the numbers can differ by will vary 
        depending on their size, due to how they are stored on the computer. 
		*/
		FloatCondition(float(*_func)(), FLOAT_TEST _test, float _val,int _ulps = 2^22)
            : func2(_func), func(NULL)
		{
			test = _test;
			val = _val;
			ulps = _ulps;
		}

		
	private:
		float (T::* const func)();
		float (* const func2)();
		FLOAT_TEST test;
		float val;
		int ulps;
		float getObjVal(void* agent)
		{
			T* obj = (T*) agent;
			return (obj->*func)();
		}
	};

	template<>
	float FloatCondition<NoClass>::getObjVal(void* agent)
	{
		return (*func2)();
	}
}
#endif // FloatCondition_h__