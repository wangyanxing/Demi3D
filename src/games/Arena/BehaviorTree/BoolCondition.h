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
	class BoolCondition: public BehaviorTreeNode
	{
	public:
		BEHAVIOR_STATUS execute(void* agent)
		{
            if (func(agent) == check)
				return BT_SUCCESS;
			else
				return BT_FAILURE;
		};

		void init(void* agent)
		{
		}

		/** \param _func the functional object
			\param _check the value that is being tested for
		*/
        BoolCondition(std::function<bool(void*)> _func, bool _check)
            : func(_func)
		{
			check = _check;
		}
	
	private:

        std::function<bool(void*)> func;
		
		bool check;
	};
}
#endif // BoolCondition_h__