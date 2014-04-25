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
#include "K2ModelView.h"
#include "HonViewerApp.h"
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
        mResourcesTree->eventTreeNodeSelected += newDelegate(this, &FilesView::notifyTreeNodeSelected);

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
            MyGUI::TreeControl::Node* nd = new MyGUI::TreeControl::Node(cur->fileName.c_str(), cur->folder ? "Folder" : "File");
            nd->setData(cur);
            if (!cur->folder)
                nd->setPrepared(true);
            pNode->add(nd);
        }
    }

    void FilesView::scanFiles()
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        DiZipArchive zip("L:/Games/hon_res_misc/hon_res_misc.zip");
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        DiZipArchive zip("/Users/wangya/Demi/media_hon/buildings.zip");
#endif
        zip.Load();
        mResources = zip.GenerateFileTree("*.mdf");

        MyGUI::TreeControl::Node* root = mResourcesTree->getRoot();
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

    void FilesView::notifyTreeNodeSelected(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
        if (!pNode || pNode->emptyData())
        {
            return;
        }
        DiFileTree* filetree = *(pNode->getData<DiFileTree*>(false));
        if (!filetree || filetree->folder)
        {
            return;
        }

        DiString fullpath = pNode->getText().asUTF8().c_str();
        MyGUI::TreeControl::Node* cur = pNode->getParent();
        while (cur->getParent())
        {
            fullpath = cur->getText().asUTF8().c_str() + DiString("/") + fullpath;
            cur = cur->getParent();
        }
        DI_DEBUG("Model %s selected", fullpath.c_str());
        HonViewerApp::GetViewerApp()->GetModelViewer()->LoadModel(fullpath);
    }

} // namespace tools
