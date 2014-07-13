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
	TextureBrowseControl::TextureBrowseControl(const DiString& mediaFilter, const DiString& honPackFilter)
        : Dialog(),
		mTextures(nullptr),
        mMediaFilter(mediaFilter),
        mHonPackFilter(honPackFilter)
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
			if ((*box->getItemDataAt<std::pair<std::string, std::string>>(index)).first == mCurrentTextureName)
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
        {
            mTextures->addItem(std::make_pair(*item, mForceTexture));
        }
	}

	void TextureBrowseControl::notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			mCurrentTextureName = (*_sender->getItemDataAt<std::pair<std::string, std::string>>(_index)).first;
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
    
    void TextureBrowseControl::enablePackSel(bool val)
    {
        mBtnSelMedia->setVisible(val);
        mBtnSelHonPack->setVisible(val);
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
        
        bool textureMode = mForceTexture.empty();
        
        if(mK2PackMode)
        {
            auto& children = filetree->children;
            for(auto& cd : children)
            {
                std::string file = mCurrentDir.c_str();
                file += "/";
                
                std::string realFile = file;
                file += textureMode ? cd.second->fileName.ExtractBaseName().c_str() : cd.second->fileName.c_str();
                realFile += cd.second->fileName.c_str();
                
                if(DiK2Configs::K2ArchiveExists(realFile, textureMode))
                {
                    if(mForceTexture.empty())
                        DiAssetManager::GetInstance().ManualLoadAsset<DiTexture>(file);
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
                std::regex regexpattern(mMediaFilter.c_str(),std::regex_constants::extended);
                if(std::regex_search(str.c_str(), regexpattern))
                {
                    texs.push_back(str.c_str());
                }
            }
            
            setTextures(texs);
        }
    }
    
    void TextureBrowseControl::scanResources(MyGUI::TreeControl::Node* root, DiFileTree* filetree)
    {
        for (auto i = filetree->children.begin(); i != filetree->children.end(); ++i)
        {
            DiFileTree* cur = i->second;
            MyGUI::TreeControl::Node* pNode = new MyGUI::TreeControl::Node(cur->fileName.c_str(), cur->folder ? "Folder" : "File");
            pNode->setData(cur);
            if (!cur->folder)
                pNode->setPrepared(true);
            root->add(pNode);
        }
    }

    void TextureBrowseControl::scanFiles()
    {
        SAFE_DELETE(mResources);
        
        MyGUI::TreeControl::Node* root = mResourcesTree->getRoot();
        root->removeAll();
        
        DiFileTree* nullfile = nullptr;
        root->setData(nullfile);

        if(mK2PackMode)
        {
            if (!DiK2Configs::RESOURCE_PACK)
                return;
            
            if (DiK2Configs::TEXTURE_PACK && DiK2Configs::TEXTURE_PACK != DiK2Configs::RESOURCE_PACK)
            {
                DiK2Configs::TEXTURE_PACK->GenerateFileTree(mResources, mHonPackFilter);
                auto prefix = mResources->FindChildren("00000000");
                auto ret = prefix ? prefix : mResources;
                DiK2Configs::RESOURCE_PACK->GenerateFileTree(ret, mHonPackFilter);
                scanResources(root, ret);
            }
            else
            {
                DiK2Configs::RESOURCE_PACK->GenerateFileTree(mResources, mHonPackFilter);
                scanResources(root, mResources);
            }
        }
        else
        {
            mResources = DiAssetManager::GetInstance().GenerateFileTree(mMediaFilter);
            scanResources(root, mResources);
        }
    }
} // namespace tools
