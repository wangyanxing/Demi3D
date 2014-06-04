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

#ifndef __PROPERTY_ITEM_H__
#define __PROPERTY_ITEM_H__

#include "FxerPrerequisites.h"
#include "Property.h"

namespace Demi
{
    class DiPropertyItem
    {
    public:

        DiPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) = 0;

        virtual int GetHeight();

        virtual void RearrangeUI(int height) = 0;

    protected:

        DiPanelGroup* mParentGroup{ nullptr };

        DiPropertyBase* mProperty{ nullptr };

        DiPropertyType  mType{ PROPERTY_MAX };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiNumericPropertyItem : public DiPropertyItem
    {
    public:

        DiNumericPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiNumericPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        virtual void RearrangeUI(int height);

    protected:

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::EditBox* mEditBox{ nullptr };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiBoolPropertyItem : public DiPropertyItem
    {
    public:

        DiBoolPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiBoolPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        virtual void RearrangeUI(int height);

    protected:

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::Button* mCheckBox{ nullptr };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiVectorPropertyItem : public DiPropertyItem
    {
    public:

        DiVectorPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiVectorPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        void NotfyExpandButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        int  GetNumElements();

        DiString GetElementName(int i);

        virtual int GetHeight();

        virtual void RearrangeUI(int height);

        void Expand(bool val);

    protected:

        MyGUI::Button*  mExpandBtn{ nullptr };

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::EditBox* mEditBox{ nullptr };

        MyGUI::EditBox* mValueBox[4];

        MyGUI::TextBox* mValueLabel[4];

        bool            mExpanded{ false };
    };

    //////////////////////////////////////////////////////////////////////////

    class DiColorPropertyItem : public DiPropertyItem
    {
    public:

        DiColorPropertyItem(DiPanelGroup* group, DiPropertyBase* prop, DiPropertyType type);

        virtual ~DiColorPropertyItem();

    public:

        virtual void CreateUI(const DiString& caption) override;

        virtual void RearrangeUI(int height);

        void NotfyColorButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

        void NotifyEndDialog(Dialog* _sender, bool _result);

    protected:

        MyGUI::TextBox* mTextBox{ nullptr };

        MyGUI::Button* mColorButton{ nullptr };
    };
}

#endif
