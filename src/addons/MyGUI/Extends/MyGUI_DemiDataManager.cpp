/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiDataManager.h"
#include "MyGUI_DemiDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include <fstream>
#include "AssetManager.h"

namespace MyGUI
{

	DemiDataManager::DemiDataManager() :
		mIsInitialise(false)
	{
	}

	void DemiDataManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DemiDataManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* DemiDataManager::getData(const std::string& _name)
	{
        Demi::DiDataStreamPtr data = Demi::DiAssetManager::GetInstance().OpenArchive(_name.c_str());
        DemiDataStream* ds = new DemiDataStream();
        ds->data = data;
		return ds;
	}

	bool DemiDataManager::isDataExist(const std::string& _name)
	{
        return Demi::DiAssetManager::GetInstance().HasArchive(_name.c_str()) ||
               Demi::DiAssetManager::GetInstance().HasAsset(_name.c_str());
	}

	const VectorString& DemiDataManager::getDataListNames(const std::string& _pattern)
	{
        static VectorString result;
		result.clear();
        
        ArchivePtr ar = Demi::DiAssetManager::GetInstance().GetArchive(Demi::DiAssetManager::GetInstance().GetBasePath());
        
        if(!ar)
        {
            return result;
        }
        
        DiStringVecPtr rets = ar->Find(_pattern.c_str());
        for (auto it = rets->begin(); it != rets->end(); ++it)
        {
            result.push_back(it->c_str());
        }

		return result;
	}

	const std::string& DemiDataManager::getDataPath(const std::string& _name)
	{
        static std::string ret;
		ret = Demi::DiAssetManager::GetInstance().GetArchivePath(_name.c_str()).c_str();
        return ret;
	}

} // namespace MyGUI
