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

#include "MersenneTwister.h"

#ifndef ProbabilityNode_h__
#define ProbabilityNode_h__

namespace BehaviorTree
{
    ///Executes behaviors randomly, based on a given set of weights.
    /** The weights are not percentages, but rather simple ratios.
    For example, if there were two children with a weight of one, each would have a 50% chance of being executed.
    If another child with a weight of eight were added, the previous children
    would have a 10% chance of being executed, and the new child would have an 80% chance of being executed.
    This weight system is intended to facilitate the fine-tuning of behaviors.
    */

    class ProbabilityNode:public BehaviorTreeInternalNode
    {
    public:
	    void init(void* object);
	    BEHAVIOR_STATUS execute(void* object);
	    ProbabilityNode::ProbabilityNode();
	    ///Add a child to this node, with the given weight.
	    BehaviorTreeInternalNode* addChild( BehaviorTreeNode* node, double weighting);
	    ///Add a child to this node, with a weight of 1.
	    BehaviorTreeInternalNode* addChild( BehaviorTreeNode* node);
    private:
        DiRand random;
	    double totalSum;
	    BehaviorTreeNode* currentNode;
	    std::map<BehaviorTreeNode*,double> weightingMap;

    };

}
#endif // ProbabilityNode_h__