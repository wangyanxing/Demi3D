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

    sm->GetCamera()->SetNearClipDistance(10);
    sm->GetCamera()->SetFarClipDistance(500);
    dbghelper->AddFrustum(sm->GetCamera(), DiColor::Blue);
    
    DiCamera* split0 = sm->GetCamera()->GetSplitFrustum(50, 200);
    dbghelper->AddFrustum(split0, DiColor::Red);

    DiCamera* split1 = sm->GetCamera()->GetSplitFrustum(220, 400);
    dbghelper->AddFrustum(split1, DiColor::Green);

    sm->GetCamera()->SetFarClipDistance(5000);
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