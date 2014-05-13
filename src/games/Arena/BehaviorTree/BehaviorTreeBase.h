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

#ifndef BehaviorTreeBase_h__
#define BehaviorTreeBase_h__

#include <vector>
#include <map>

namespace BehaviorTree
{
	/// Enumerates the states every node can be in after execution during a particular time step
	/**
	- BT_SUCCESS indicates the node has completed running during this time step.
	- BT_FAILURE indicates the node has determined it will not be able to complete its task.
	- BT_RUNNING indicates that the node has successfully moved forward during this time step, but the task is not yet complete.
	*/
	enum BEHAVIOR_STATUS 
    {
        BT_SUCCESS,
        BT_FAILURE,
        BT_RUNNING
    };

	class BehaviorTreeNode;
	/// A standard vector of Behavior Tree nodes. Provided for convenience.
	typedef std::vector<BehaviorTreeNode*> BehaviorTreeList;
	/// A standard iterator of a BehaviorTreeList. Provided for convenience.
	typedef BehaviorTreeList::iterator BehaviorTreeListIter;

	/// Enumerates the options for when a parallel node is considered to have failed.
	/**
	- FAIL_ON_ONE indicates that the node will return failure as soon as one of its children fails.
	- FAIL_ON_ALL indicates that all of the node's children must fail before it returns failure.

	If FAIL_ON_ONE and SUCEED_ON_ONE are both active and are both trigerred in the same time step, failure will take precedence.
	*/
	enum FAILURE_POLICY {FAIL_ON_ONE,FAIL_ON_ALL};
	/// Enumerates the options for when a parallel node is considered to have succeeded.
	/**
	- SUCCEED_ON_ONE indicates that the node will return success as soon as one of its children succeeds.
	- SUCCEED_ON_ALL indicates that all of the node's children must succeed before it returns success.
	*/
	enum SUCCESS_POLICY {SUCCEED_ON_ONE,SUCCEED_ON_ALL};


	///Abstract base clase for Behavior Tree Nodes
	class BehaviorTreeNode
	{
	public:
		/// This method is invoked by the node's parent when the node should be run.
		virtual BEHAVIOR_STATUS execute(void* agent) = 0;
		/// This method will be invoked before the node is executed for the first time.
		virtual void init(void* agent) = 0;
	};

	/// Abstract base class for Behavior Tree nodes with children
	class BehaviorTreeInternalNode:public BehaviorTreeNode
	{

	public:
		virtual BEHAVIOR_STATUS execute(void* agent) = 0;
		virtual void init(void* object) = 0;
		/// Add a child to this node. Takes ownership of the child.
		virtual BehaviorTreeInternalNode* BehaviorTreeInternalNode::addChild(BehaviorTreeNode* newChild)
		{
			children.push_back(newChild);
			return this;
		};

        virtual ~BehaviorTreeInternalNode()
        {
            BehaviorTreeListIter iter;
            for (iter = children.begin(); iter != children.end(); iter++)
            {
                delete *iter;
            }
        }

	protected:
		BehaviorTreeList children;
		
	};

	///Always returns the BT_RUNNING status
	class AlwaysRunning: public BehaviorTreeNode
	{
		BEHAVIOR_STATUS execute(void* agent)
		{
			return BT_RUNNING;
		}
		void init(void* agent){};
	};

	///Always returns the BT_SUCCESS status
	class AlwaysSuccess: public BehaviorTreeNode
	{
		BEHAVIOR_STATUS execute(void* agent)
		{
			return BT_SUCCESS;
		}
		void init(void* agent){};
	};

	///Always returns the BT_FAILURE status
	class AlwaysFailure: public BehaviorTreeNode
	{
		BEHAVIOR_STATUS execute(void* agent)
		{
			return BT_FAILURE;
		}
		void init(void* agent){};
	};

	///Returns BT_RUNNING for the specified number of iterations, then returns BT_SUCCESS after that
	class SuccessAfter: public BehaviorTreeNode
	{
	public:
		int n;
		int total;
		BEHAVIOR_STATUS execute(void* agent)
		{
			if (n == 0)
			{
				return BT_SUCCESS;
			}
			else
			{
				n--;
				return BT_RUNNING;
			}
		}
		void init(void* agent)
		{
			n = total;
		};
		SuccessAfter::SuccessAfter(int t)
		{
			total = t;
			n = total;
		}
	};

	///Returns BT_RUNNING for the specified number of iterations, then returns BT_FAILURE after that
	class FailureAfter: public BehaviorTreeNode
	{
	public:
		int n;
		int total;
		BEHAVIOR_STATUS execute(void* agent)
		{
			if (n == 0)
			{
				return BT_FAILURE;
			}
			else
			{
				n--;
				return BT_RUNNING;
			}
		}
		void init(void* agent)
		{
			n = total;
		};
		FailureAfter::FailureAfter(int t)
		{
			total = t;
			n = total;
		}
	};
}
#endif // BehaviorTreeBase_h__