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
#ifndef MiscPrerequisites_h__
#define MiscPrerequisites_h__

#include "PlatformSpecific.h"
#include "MiscDefines.h"
#include "DebugAssert.h"
#include "SmartPtr.h"

namespace Demi
{
    class    DiVec2;
    class    DiVec3;
    class    DiVec4;
    class    DiMat3;
    class    DiMat4;
    class    DiQuat;
    class    DiPlane;
    class    DiAABB;
    class    DiRadian;
    class    DiDegree;
    class    DiAngle;
    class    DiRay;
    class    DiSphere;
    class    DiColor;
    class    DiCmdArgs;
    class    DiDataStream;
    class    DiTimer;
    class    DiAutoCrashHandler;
    class    DiDynamicAttributeFactory;
    class    DiDynamicAttribute;

    class    DiXMLFile;
    class    DiXMLElement;
    class    DiXmlNodeInterface;
    class    DiXmlNodeRef;
    class    DiXmlNode;
    class    DiXmlNodePool;
    class    DiXmlNodeReuse;
    class    DiXmlTemplateRegistry;
    struct   DiXmlStringPoolInterface;
    struct   DiXmlParserInterface;
    struct   DiXmlUtilsInterface;
    
	typedef shared_ptr<DiDataStream>    DiDataStreamPtr;
}

#endif