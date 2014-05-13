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

#include "ArenaPch.h"
#include "BehaviorTree/BehaviorTree.h"

using namespace BehaviorTree;
using namespace std;

void SequentialNode::init(void* agent)
{
	currentPosition = -1;
	for (BehaviorTreeListIter iter = children.begin(); iter!= children.end(); iter++)
		(*iter)->init(agent);
}

SequentialNode::SequentialNode()
{
	currentPosition = -1;
}

BEHAVIOR_STATUS SequentialNode::execute(void* agent)
{
	if (currentPosition == -1) //starting out
	{
		init(agent);
		currentPosition = 0;
	}

	if (children.size() == 0)
		return BT_SUCCESS;

	BehaviorTreeNode* currentTask = children.at(currentPosition);
	BEHAVIOR_STATUS result = currentTask->execute(agent);

	while(result == BT_SUCCESS)
	{
		if (currentPosition == children.size()-1) //finished last task
		{
			currentPosition = -1; //indicate we are not running anything
			return BT_SUCCESS;
		}
		else
		{
			currentPosition++;
			currentTask = children.at(currentPosition);
			result = currentTask->execute(agent);
		}
	}
	if (result == BT_FAILURE)
		currentPosition = -1;
	return result;
}
