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

#include "Demi.h"
#include "DemoFrame.h"
#include "DebugHelper.h"

void InitScene()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    DiDebugHelperPtr dbghelper = make_shared<DiDebugHelper>();
    sm->AttachObject(dbghelper);
    dbghelper->AddLine(DiVec3(100, 0, 0), DiVec3(-100,0,0), DiColor::Red);
    dbghelper->AddLine(DiVec3(0, 100, 0), DiVec3(0,-100,0), DiColor::Red);
    
    DiAABB aabb;
    aabb.SetExtents(100, 100, 100, -100, -100, -100);
    dbghelper->AddBoundingBox(aabb, DiColor::Green);
}

void UpdateScene()
{
}

int main(int argc, char *argv[])
{
    DemiDemo app( DemoConfig("Demi test: Gui"));
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open();

	return 0;
}