/**************************************************************************
    File:       CommonPrerequisites.h
    Creator:    demiwangya
**************************************************************************/

#ifndef MiscPrerequisites_h__
#define MiscPrerequisites_h__

#include "PlatformSpecific.h"
#include "MiscDefines.h"
#include "DebugAssert.h"

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
}

#endif