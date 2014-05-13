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

#ifndef ParallelNode_h__
#define ParallelNode_h__

namespace BehaviorTree
{
    ///Execute behaviors in parallel
    /** There are two policies that control the flow of execution. The first is the policy for failure, and the second is the policy for success.
    For failure, the options are "fail when one child fails" and "fail when all children fail".
    For success, the options are similarly "complete when one child completes", and "complete when all children complete".
    */
    class ParallelNode:public BehaviorTreeInternalNode
    {
    public:
	    BEHAVIOR_STATUS execute(void* agent);
	    void init(void* agent);
	    /* \param failurePolicy Determines how many of the node's children must fail before the node fails
	       \param successPolicy Determines how many of the node's children must succeed before the node succeeds */
	    ParallelNode::ParallelNode( FAILURE_POLICY failurePolicy = FAIL_ON_ALL, SUCCESS_POLICY successPolicy = SUCCEED_ON_ALL);

    private:
	    typedef std::map<BehaviorTreeNode*,BEHAVIOR_STATUS> ChildrenStatusMap;
	    ChildrenStatusMap* childrenStatus;
	    FAILURE_POLICY failPolicy;
	    SUCCESS_POLICY succeedPolicy;
    };
}

#endif // ParallelNode_h__