/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#include "DiPch.h"
#include "ConverterMgr.h"

DiString		srcName;
DiString		destName;
ConverterMgr*	converterMgr = NULL;
bool			isAnimation = false;

void parseArgs(int numArgs, char **args)
{
	srcName = args[1];
	srcName.ToLower();
	if(srcName.CheckFileExtension("skeleton") ||
		srcName.CheckFileExtension("skeleton"))
	{
		isAnimation = true;
	}

	if(numArgs == 2)
	{
		destName = srcName.ExtractDirName();
		DiString temp = srcName.ExtractFileName().ExtractBaseName();
		destName.append(temp);
		destName.append(!isAnimation?".model":".motion");
	}
	else if(numArgs == 3)
	{
		destName = args[2];
	}
}

int main(int numargs, char** args)
{
	if (numargs < 2)
	{
        return -1;
    }

	parseArgs(numargs, args);

	converterMgr = new ConverterMgr();

	CovOptions option;
	option.source = srcName.c_str();
	option.dest   = destName.c_str();

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(srcName.ExtractDirName().c_str(),"FileSystem");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (isAnimation)
	{
		converterMgr->SkeletonToMotion(option);
	}
	else
	{
		converterMgr->MeshToModel(option);
	}

	delete converterMgr;

    return 0;
}
