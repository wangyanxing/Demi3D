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
	class FunctionCall: public BehaviorTreeNode
	{
	public:
		BEHAVIOR_STATUS execute(void* agent)
		{
            return func(agent, arg);
		};

		void init(void* agent){};

		/** \param _func the address of the (non-static) class member
			\param _arg the genericized argument to the function
		*/
        FunctionCall(std::function<BEHAVIOR_STATUS(void*, void*)> _func, void* _arg = nullptr) 
            : func(_func), arg(_arg){}


	private:
		
        std::function<BEHAVIOR_STATUS(void*, void*)> func;

        void* arg{ nullptr };
	};
}
#endif // FunctionCall_h__