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

#ifndef HonFxerPrerequisites_h__
#define HonFxerPrerequisites_h__

#include "MyGUI_Prerequest.h"
#include "FxPrerequisites.h"
#include "Property.h"
#include "PropertyTypes.h"

namespace Demi
{
    class HonFxerApp;
    class CurveEditor;
    class ColorEditor;
    class K2ModelViewer;
    class ContextMenu;
    class DiEditorManager;
    class DiBaseEditorObj; 
    class DiPanelGroup;
    class DiPropertyItem;
    
    typedef DiProperty<DiTexturePtr, PROPERTY_TEXTURE> DiTextureProperty;
}

namespace tools
{
    class BackgroundControl;
    class MainPaneControl;
    class FilesView;
    class AnimationView;
    class ModelView;
    class KeyFrameBarControl;
    class CommonToolsControl;
    class SetResLocWindow;
    class SettingsManager;
    class ViewerHelper;
    class SetGameLocWindow;
    class PanelView;
    class PropertiesControl;
    class TextureBrowseControl;
    class ToolbarControl;
}

namespace MyGUI
{
    class TreeControl;
    class TreeControlItem;
}

using namespace tools;

#endif