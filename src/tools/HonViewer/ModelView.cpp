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

#include "ViewerPch.h"
#include "ModelView.h"
#include "K2ModelView.h"
#include "HonViewerApp.h"

namespace tools
{
    ModelView::ModelView(MyGUI::Widget* _parent) :
		wraps::BaseLayout("ModelView.layout", _parent)
	{
        
	}

    ModelView::~ModelView()
	{
	}

    void ModelView::UpdateModelInfo()
    {
    }
    
} // namespace tools
