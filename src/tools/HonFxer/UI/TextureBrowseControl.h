/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __TEXTURE_BROWSE_CONTROL_H__
#define __TEXTURE_BROWSE_CONTROL_H__

#include "FxerPrerequisites.h"
#include "Dialog.h"
#include "BaseItemBox.h"
#include "TextureBrowseCell.h"
#include "MyGUI_TreeControl.h"

namespace tools
{
	ATTRIBUTE_CLASS_LAYOUT(TextureBrowseControl, "FxTextureBrowseControl.layout");
	class TextureBrowseControl : public Dialog
	{
	public:
		TextureBrowseControl();
		virtual ~TextureBrowseControl();

		const std::string& getTextureName() const;
		void setTextureName(const std::string& _value);

		void setTextures(const MyGUI::VectorString& _textures);

	protected:
		virtual void onDoModal();
		virtual void onEndModal();
        
        void scanFiles();
        
        void switchPackMode(bool usePack);

	private:
        
        void notifyMouseSelPackButtonClick(MyGUI::Widget* _sender);
    
		void notifyMouseButtonClickOk(MyGUI::Widget* _sender);
		void notifyMouseButtonClickCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);
		void notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index);
        
        void notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);
        void notifyTreeNodeSelected(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(TextureBrowseControl, mOk, "OK");
		MyGUI::Button* mOk;

		ATTRIBUTE_FIELD_WIDGET_NAME(TextureBrowseControl, mCancel, "Cancel");
		MyGUI::Button* mCancel;

		wraps::BaseItemBox<TextureBrowseCell>* mTextures;

		std::string mCurrentTextureName;
        
        MyGUI::TreeControl* mResourcesTree{ nullptr };
        
        DiFileTree* mResources{ nullptr };
        
        DiString mCurrentDir;
        
        bool mK2PackMode{ false };
        
        MyGUI::Button* mBtnSelMedia{ nullptr };
        
        MyGUI::Button* mBtnSelHonPack{ nullptr };
	};

} // namespace tools

#endif // __TEXTURE_BROWSE_CONTROL_H__
