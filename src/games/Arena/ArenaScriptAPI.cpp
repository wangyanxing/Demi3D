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

#include "ArenaPch.h"
#include "ArenaScriptAPI.h"
#include "ArenaGame.h"
#include "ArenaGameApp.h"

using namespace Demi;

void ar_openLevel(const char* configFile)
{
    ArGameApp::Get()->GetGame()->LoadLevel(configFile);
}

void ar_setHero(const char* configFile)
{
    ArGameApp::Get()->GetGame()->SetHero(configFile);
}

void ar_spawnNPC(const char* configFile)
{

}
