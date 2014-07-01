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

#ifndef __HOT_KEY_MANAGER_H__
#define __HOT_KEY_MANAGER_H__

#include "FxerPrerequisites.h"
#include "HotKeyCommand.h"

namespace tools
{

	class HotKeyManager :
		public MyGUI::Singleton<HotKeyManager>
	{
	public:
		HotKeyManager();
		virtual ~HotKeyManager();

		void initialise();
		void shutdown();

		bool onKeyEvent(MyGUI::KeyCode _key);

	private:
		void loadXml(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

		void addCommand(HotKeyCommand& _command);

	private:
		typedef std::map<std::string, MyGUI::KeyCode> MapKeys;
		MapKeys mKeyNames;

		typedef std::vector<HotKeyCommand> VectorCommand;
		typedef std::map<MyGUI::KeyCode, VectorCommand> MapCommand;
		MapCommand mCommands;
	};

} // namespace tools

#endif // __HOT_KEY_MANAGER_H__
