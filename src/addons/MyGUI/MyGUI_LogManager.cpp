/*!
	@file
	@author		Albert Semenov
	@date		01/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_FileLogListener.h"
#include "MyGUI_ConsoleLogListener.h"
#include "MyGUI_LevelLogFilter.h"
#include "MyGUI_LogSource.h"
#include <time.h>

#include "LogManager.h"

namespace MyGUI
{
    static Demi::LogLevel ToDemiLogLevel(MyGUI::LogLevel::Enum level)
    {
        switch (level)
        {
        case LogLevel::Warning:
        case LogLevel::Error:
            return Demi::LOG_LEVEL_WARNING;
        case LogLevel::Critical:
            return Demi::LOG_LEVEL_ERROR;
        case LogLevel::Info:
        default:
            return Demi::LOG_LEVEL_LOG;
        }
    }

	LogManager* LogManager::msInstance = nullptr;

	LogManager::LogManager()
	{
		msInstance = this;
	}

	LogManager::~LogManager()
	{
		msInstance = nullptr;
	}

	LogManager& LogManager::getInstance()
	{
		if (msInstance == nullptr)
		{
			MYGUI_DBG_BREAK;
			MYGUI_BASE_EXCEPT("Singleton instance LogManager was not created", "MyGUI");
		}
		return *msInstance;
	}

	LogManager* LogManager::getInstancePtr()
	{
		return msInstance;
	}

	void LogManager::log(const std::string& _section, LogLevel _level, const std::string& _message, const char* _file, int _line)
	{
        Demi::DiLogManager::GetInstance().Output(ToDemiLogLevel(_level.getValue()), _file, _line, "[MyGUI]%s", _message.c_str());
	}

} // namespace MyGUI
