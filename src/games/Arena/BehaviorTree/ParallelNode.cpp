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
#include <iostream>

using namespace BehaviorTree;
using namespace std;

ParallelNode::ParallelNode(FAILURE_POLICY failurePolicy, SUCCESS_POLICY successPolicy)
{
	failPolicy = failurePolicy;
	succeedPolicy = successPolicy;
	childrenStatus = NULL;
}
void ParallelNode::init(void* agent)
{
	for (BehaviorTreeListIter iter = children.begin(); iter!= children.end(); iter++)
		(*iter)->init(agent);

	if (childrenStatus != NULL)
		delete childrenStatus;
	childrenStatus = new ChildrenStatusMap();
	for (unsigned int i = 0 ; i<children.size(); i++)
		childrenStatus->insert( make_pair(children.at(i),BT_RUNNING));
}

BEHAVIOR_STATUS ParallelNode::execute(void* agent)
{
	if (childrenStatus == NULL)
		init(agent);
	// go through all children and update the childrenStatus
	for (unsigned int i = 0 ; i<children.size(); i++)
	{
		BehaviorTreeNode* node = children[i];
		if ((*childrenStatus)[node] == BT_RUNNING)
		{
			BEHAVIOR_STATUS status = node->execute(agent);
			if (status == BT_FAILURE)
			{
				if (failPolicy == FAIL_ON_ONE)
				{
					init(agent);
					return BT_FAILURE;
				}
				else
				{
					(*childrenStatus)[node] = BT_FAILURE;
				}
			}
			if (status == BT_SUCCESS)
				(*childrenStatus)[node] = BT_SUCCESS;
		}
		if ((*childrenStatus)[node] == BT_FAILURE && failPolicy == FAIL_ON_ALL) //theoretically the failPolicy check is not needed
		{
			BEHAVIOR_STATUS status = node->execute(agent);
			(*childrenStatus)[node] = status;
		}
	}

	//look through the childrenStatus and see if we have met any of our end conditions
	ChildrenStatusMap::iterator iter;
	bool sawSuccess = false;
	bool sawAllFails = true;
	bool sawAllSuccess = true;
	for (iter = childrenStatus->begin(); iter != childrenStatus->end() ; iter++)
	{
		switch((*iter).second)
		{
		case BT_SUCCESS:
			//can't instantly return success for succeedOnOne policy if failOnOne is also true, because failOnOne overrides succeedOnOne
			if (succeedPolicy == SUCCEED_ON_ONE && failPolicy != FAIL_ON_ONE)
			{
				init(agent);
				return BT_SUCCESS;
			}
			sawSuccess = true;
			sawAllFails = false;
			break;
		case BT_FAILURE:
			if (failPolicy == FAIL_ON_ONE)
			{
				init(agent);
				return BT_FAILURE;
			}
			sawAllSuccess = false;
			break;
		case BT_RUNNING:
			sawAllFails = false;
			sawAllSuccess = false;
			//optimization for early exit
			if (failPolicy == FAIL_ON_ALL && succeedPolicy == SUCCEED_ON_ALL)
			{
				return BT_RUNNING;
			}
			break;
		}
	}
	if (failPolicy == FAIL_ON_ALL && sawAllFails)
	{
		init(agent);
		return BT_FAILURE;
	}
	else if (succeedPolicy == SUCCEED_ON_ALL && sawAllSuccess || succeedPolicy == SUCCEED_ON_ONE && sawSuccess)
	{
		init(agent);
		return BT_SUCCESS;
	}
	else
	{
		return BT_RUNNING;
	}
}