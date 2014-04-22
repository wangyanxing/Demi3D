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
		std::string filepath = getDataPath(_name);
		if (filepath.empty())
			return nullptr;

		std::ifstream* stream = new std::ifstream();
		stream->open(filepath.c_str(), std::ios_base::binary);

		if (!stream->is_open())
		{
			delete stream;
			return nullptr;
		}

		DataFileStream* data = new DataFileStream(stream);

		return data;
	}

	bool DemiDataManager::isDataExist(const std::string& _name)
	{
		const VectorString& files = getDataListNames(_name);
		return files.size() == 1;
	}

	const VectorString& DemiDataManager::getDataListNames(const std::string& _pattern)
	{
		static VectorString result;
		result.clear();

		return result;
	}

	const std::string& DemiDataManager::getDataPath(const std::string& _name)
	{
		static std::string path;
		VectorString result;
		
		return path;
	}

	void DemiDataManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		ArhivInfo info;
		info.name = MyGUI::UString(_name).asWStr();
		info.recursive = _recursive;
		mPaths.push_back(info);
	}

} // namespace MyGUI
