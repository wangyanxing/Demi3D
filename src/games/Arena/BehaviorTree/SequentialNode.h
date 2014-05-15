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

#ifndef SequentialNode_h__
#define SequentialNode_h__

namespace BehaviorTree
{
    ///Executes behaviors in order
    /** Executes its children in the order they were added.
    If the currently executing child returns BT_FAILURE, BT_ERROR, or BT_RUNNING, this returns the same status.
    If the currently executing child returns BT_SUCCESS, this begins executing the next child, if it exists. 
    It continues in this fashion until one of the children returns BT_FAILURE, BT_ERROR, 
    or BT_RUNNING. If all children have successfully executed, it will return BT_SUCCESS.
    */
    class SequentialNode:public BehaviorTreeInternalNode
    {
    public:
	    /// construct a sequentialNode
	    SequentialNode();

	    BEHAVIOR_STATUS execute(void* agent);
	    
        void init(void* agent);

	    /// Where the SequentialNode currently is executing. -1 indicates the SequentialNode is not executing any child.
	    int currentPosition;
    };
}
#endif // SequentialNode_h__