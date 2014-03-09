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

int main(int argc, char *argv[])
{
    DemoConfig cfg("Demi3D Sample - Quake3 Bsp Level");
    cfg.sceneType = "Bsp";
    DemiDemo app(cfg);

    app.SetInitCallback([](){
        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->SetAmbientColor(DiColor::Black);
        sm->LoadScene("chiropteradm.bsp");
    });

    //app.SetUpdateCallback([](){});
	
    app.Open();

	return 0;
}