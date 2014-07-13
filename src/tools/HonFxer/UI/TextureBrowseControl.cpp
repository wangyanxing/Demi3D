/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "FxerPch.h"
#include "TextureBrowseControl.h"
#include "MyGUI_TreeControlItem.h"
#include "ZipArchive.h"


#include <regex>

namespace tools
{
	TextureBrowseControl::TextureBrowseControl() :
		Dialog(),
		mTextures(nullptr)
	{
		initialiseByAttributes(this);

		assignBase(mTextures, "Textures");
        
        assignWidget(mResourcesTree, "ResourcesTree");
        assignWidget(mBtnSelMedia, "SelMedia");
        assignWidget(mBtnSelHonPack, "SelHonPack");
        
        

        mBtnSelMedia->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseSelPackButtonClick);
        mBtnSelHonPack->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseSelPackButtonClick);
        
		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseButtonClickOk);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureBrowseControl::notifyMouseButtonClickCancel);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TextureBrowseControl::notifyWindowButtonPressed);

		MyGUI::ItemBox* box = mTextures->getItemBox();
		box->eventChangeItemPosition += MyGUI::newDelegate(this, &TextureBrowseControl::notifyChangeItemPosition);

		mMainWidget->setVisible(false);
        
        mResourcesTree->eventTreeNodePrepare += newDelegate(this, &TextureBrowseControl::notifyTreeNodePrepare);
        mResourcesTree->eventTreeNodeSelected += newDelegate(this, &TextureBrowseControl::notifyTreeNodeSelected);
        
        scanFiles();
	}

	TextureBrowseControl::~TextureBrowseControl()
	{
		MyGUI::ItemBox* box = mTextures->getItemBox();
		box->eventChangeItemPosition -= MyGUI::newDelegate(this, &TextureBrowseControl::notifyChangeItemPosition);
        
        SAFE_DELETE(mResources);
	}
    
    void TextureBrowseControl::switchPackMode(bool usePack)
    {
        if(mK2PackMode != usePack)
        {
            mK2PackMode = usePack;
            scanFiles();
        }
    }

	void TextureBrowseControl::onDoModal()
	{
		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
	}

	void TextureBrowseControl::onEndModal()
	{
	}
    
	void TextureBrowseControl::notifyMouseSelPackButtonClick(MyGUI::Widget* _sender)
    {
        if(static_cast<MyGUI::Button*>(_sender)->getStateSelected())
            return;
        
        mBtnSelMedia->setStateSelected(!mBtnSelMedia->getStateSelected());
        mBtnSelHonPack->setStateSelected(!mBtnSelMedia->getStateSelected());
        
        switchPackMode(mBtnSelHonPack->getStateSelected());
    }

	void TextureBrowseControl::notifyMouseButtonClickCancel(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
	}

	void TextureBrowseControl::notifyMouseButtonClickOk(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, true);
	}

	void TextureBrowseControl::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		eventEndDialog(this, false);
	}

	const std::string& TextureBrowseControl::getTextureName() const
	{
		return mCurrentTextureName;
	}

	void TextureBrowseControl::setTextureName(const std::string& _value)
	{
		mCurrentTextureName = _value;

		MyGUI::ItemBox* box = mTextures->getItemBox();

		size_t indexSelected = MyGUI::ITEM_NONE;
		for (size_t index = 0; index < box->getItemCount(); ++index)
		{
			if (*box->getItemDataAt<std::string>(index) == mCurrentTextureName)
			{
				indexSelected = index;
				break;
			}
		}

		box->setIndexSelected(indexSelected);
	}

	void TextureBrowseControl::setTextures(const MyGUI::VectorString& _textures)
	{
		mTextures->removeAllItems();

		for (MyGUI::VectorString::const_iterator item = _textures.begin(); item != _textures.end(); ++item)
			mTextures->addItem((*item));
	}

	void TextureBrowseControl::notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			mCurrentTextureName = *_sender->getItemDataAt<std::string>(_index);
		else
			mCurrentTextureName = "";
	}
    
    void TextureBrowseControl::notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
        if (pNode == pTreeControl->getRoot())
            return;
        
        pNode->removeAll();
        
        DiFileTree* filetree = *(pNode->getData<DiFileTree*>());
        for (auto i = filetree->children.begin(); i != filetree->children.end(); ++i)
        {
            DiFileTree* cur = i->second;
            MyGUI::TreeControl::Node* nd = new MyGUI::TreeControl::Node(cur->fileName.c_str(), cur->folder ? "Folder" : "File");
            nd->setData(cur);
            if (!cur->folder)
                nd->setPrepared(true);
            pNode->add(nd);
        }
    }
    
    void TextureBrowseControl::notifyTreeNodeSelected(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
        if (!pNode || pNode->emptyData())
            return;
        
        DiFileTree* filetree = *(pNode->getData<DiFileTree*>(false));
        if (!filetree)
            return;
        
        if(!filetree->folder)
            filetree = filetree->parent;
        
        MyGUI::VectorString texs;

        DiString fullpath = filetree->fileName;
        
        auto cur = filetree->parent;
        while (cur->parent)
        {
            fullpath = cur->fileName + DiString("/") + fullpath;
            cur = cur->parent;
        }
        
        if(mCurrentDir == fullpath)
            return;
        
        mCurrentDir = fullpath;
        
        if(mK2PackMode)
        {
            auto& children = filetree->children;
            for(auto& cd : children)
            {
                std::string file = mCurrentDir.c_str();
                file += "/";
                
                std::string realFile = file;
                file += cd.second->fileName.ExtractBaseName().c_str();
                realFile += cd.second->fileName.c_str();
                
                if(DiK2Configs::K2ArchiveExists(realFile, true))
                {
                    auto tex = DiAssetManager::GetInstance().ManualLoadAsset<DiTexture>(file);
                    texs.push_back(file);
                }
            }
            
            setTextures(texs);
        }
        else
        {
            auto ret = DiAssetManager::GetInstance().FindDirArchive(fullpath);
            if(!ret)
            {
                DI_WARNING("Failed to locate the path: %s", fullpath.c_str());
                setTextures(texs);
                return;
            }
            
            auto files = ret->Find("*.*", false, false);
            
            for(auto str : *files)
            {
                str.ToLower();
                std::regex regexpattern("(\\.dds|\\.tga|\\.png|\\.jpg)$",std::regex_constants::extended);
                if(std::regex_search(str.c_str(), regexpattern))
                {
                    texs.push_back(str.c_str());
                }
            }
            
            setTextures(texs);
        }
    }

    void TextureBrowseControl::scanFiles()
    {
        MyGUI::TreeControl::Node* root = mResourcesTree->getRoot();
        root->removeAll();

        if(mK2PackMode)
        {
            DiZipArchive* zip = DiK2Configs::RESOURCE_PACK;
            if (!zip)
                return;
            mResources = zip->GenerateFileTree("*.dds");
        }
        else
        {
            mResources = DiAssetManager::GetInstance().GenerateFileTree("(\\.dds|\\.tga|\\.png|\\.jpg)$");
        }
        
        DiFileTree* nullfile = nullptr;
        root->setData(nullfile);
        for (auto i = mResources->children.begin(); i != mResources->children.end(); ++i)
        {
            DiFileTree* cur = i->second;
            MyGUI::TreeControl::Node* pNode = new MyGUI::TreeControl::Node(cur->fileName.c_str(), cur->folder ? "Folder" : "File");
            pNode->setData(cur);
            if (!cur->folder)
                pNode->setPrepared(true);
            root->add(pNode);
        }
    }
} // namespace tools
