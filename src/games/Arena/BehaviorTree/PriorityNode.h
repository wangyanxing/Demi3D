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

#ifndef PriorityNode_h__
#define PriorityNode_h__

namespace BehaviorTree
{
    ///Executes behaviors in priority order until one of them is successful.
    /** Attempts to execute children in the order they were added. 
    - If a child returns BT_FAILURE, begin executing the next child if there is one, else return BT_FAILURE.
    - If a child returns BT_ERROR, return BT_ERROR.
    - If a child returns BT_SUCCESS, return BT_SUCCESS.
    - If a child returns BT_RUNNING, return BT_RUNNING.
    */
    class PriorityNode : public BehaviorTreeInternalNode
    {
    public:
	    BEHAVIOR_STATUS execute(void* agent);
	    int currentPosition;
	    PriorityNode::PriorityNode();
	    void init(void* agent);

    };
}
#endif // PriorityNode_h__