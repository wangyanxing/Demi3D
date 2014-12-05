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

#ifndef ArenaGUIProperty_h__
#define ArenaGUIProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaProperty.h"

namespace Demi
{
    /** state property
     */
    class ArGUIProperty : public ArProperty
    {
        DEFINE_PROPERTY_ID(PROPERTY_GUI)

    public:

        ArGUIProperty();

        ~ArGUIProperty();

    public:

        void Update(float dt);

    private:
        
        void InitHPBar();
        
        MyGUI::ImageBox* createIcon(const DiString& texture, DiVec2 pos, DiVec2 size, MyGUI::Widget* parent = nullptr);
        
        MyGUI::ImageBox* mHPBar{ nullptr };
    };
}

#endif
