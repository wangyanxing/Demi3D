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
#include "FilesView.h"
#include "ZipArchive.h"
#include "MyGUI_TreeControlItem.h"

namespace tools
{
    FilesView::FilesView(MyGUI::Widget* _parent) :
		wraps::BaseLayout("FilesView.layout", _parent)
        , mResourcesTree(nullptr)
        , mResources(nullptr)
	{
	    assignWidget(mResourcesTree, "ResourcesTree");

        mResourcesTree->eventTreeNodePrepare += newDelegate(this, &FilesView::notifyTreeNodePrepare);

        scanFiles();
	}

    FilesView::~FilesView()
	{
        if (mResources)
        {
            mResources->Release();
            DI_DELETE mResources;
            mResources = nullptr;
        }

        mResourcesTree->eventTreeNodePrepare -= newDelegate(this, &FilesView::notifyTreeNodePrepare);
	}

    void FilesView::notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
        if (pNode == pTreeControl->getRoot())
            return;

        pNode->removeAll();

        DiFileTree* filetree = *(pNode->getData<DiFileTree*>());
        for (auto i = filetree->children.begin(); i != filetree->children.end(); ++i)
        {
            DiFileTree* cur = i->second;
            bool folder = !cur->children.empty();
            MyGUI::TreeControl::Node* nd = new MyGUI::TreeControl::Node(cur->fileName.c_str(), folder ? "Folder" : "File");
            if (folder)
                nd->setData(cur);
            else
                nd->setPrepared(true);
            pNode->add(nd);
        }
    }

    void FilesView::scanFiles()
    {
        DiZipArchive zip("C:\\Users\\Yanxing\\Desktop\\resources_KillerNurseAluna.zip");
        zip.Load();
        mResources = zip.GenerateFileTree("*.mdf");

        MyGUI::TreeControl::Node* root = mResourcesTree->getRoot();
        for (auto i = mResources->children.begin(); i != mResources->children.end(); ++i)
        {
            DiFileTree* cur = i->second;
            bool folder = !cur->children.empty();
            MyGUI::TreeControl::Node* pNode = new MyGUI::TreeControl::Node(cur->fileName.c_str(), folder ? "Folder" : "File");
            if (folder)
                pNode->setData(cur);
            else
                pNode->setPrepared(true);
            root->add(pNode);
        }
    }

} // namespace tools
