/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DEMI_DATA_MANAGER_H__
#define __MYGUI_DEMI_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

#include "DataStream.h"

namespace MyGUI
{
    class DemiDataStream : public IDataStream
	{
	public:
		bool eof()
        {
            return data->Eof();
        }
		size_t size()
        {
            return data->Size();
        }
		void readline(std::string& _source, Char _delim = '\n')
        {
            static char buffer[1024];
            char delim[2] = {(char)_delim,'\0'};
            data->ReadLine(buffer, 1024, delim);
            _source = buffer;
        }
		size_t read(void* _buf, size_t _count)
        {
            return data->Read(_buf, _count);
        }
        Demi::DiDataStreamPtr data;
	};

	class DemiDataManager : public DataManager
	{
	public:
		DemiDataManager();

		void initialise();
		void shutdown();

		static DemiDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static DemiDataManager* getInstancePtr()
		{
			return static_cast<DemiDataManager*>(DataManager::getInstancePtr());
		}

		/** @see DataManager::getData(const std::string& _name) */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);

	private:

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_DATA_MANAGER_H__
