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

#ifndef CountLimitNode_h__
#define CountLimitNode_h__

namespace BehaviorTree
{
	///Limit the number of times this tree section can execute before it becomes blocked off
	class CountLimitNode : public BehaviorTreeInternalNode
	{
	public:
		BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		/**
		\param limit The number of times the tree section can execute before it becomes blocked off.
		\param allow_reinitialize If true, when this node's init method is called, 
         the restriction is lifted. Otherwise, the restriction is never lifted. */
		CountLimitNode(int limit,bool allow_reinitialize = true);
		BehaviorTreeInternalNode* addChild( BehaviorTreeNode* newChild );
	private:
		int limit;
		int current_rep;
		bool allow_reinitialize;
		void initChildren(void* agent);
	};
}
#endif // CountLimitNode_h__