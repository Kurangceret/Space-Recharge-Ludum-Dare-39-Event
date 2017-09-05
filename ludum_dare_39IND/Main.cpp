/*#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif*/ // SFML_STATIC
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Application.h"
#include <iostream>
#include "Helper/Constant.h"


extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

int main()
{
	lua_State* luaState = luaL_newstate();
	luaL_openlibs(luaState);

	if (luaL_dofile(luaState, std::string(Constant::settingScriptDir
		+ "WindowSettingScript.lua").c_str()) != 0)
	{
		std::cout << lua_tostring(luaState, -1) << std::endl;
		assert(false == true);
	}
	luabridge::LuaRef windowSettingTable = luabridge::LuaRef::fromStack(luaState, -1);

	sf::VideoMode cur_desktop_mode = sf::VideoMode::getDesktopMode();
	if (cur_desktop_mode.width < 1600.f || cur_desktop_mode.height < 900.f){
		windowSettingTable["width"] = 1366.f;
		windowSettingTable["height"] = 768.f;
	}

	Application game(windowSettingTable, luaState);

	try{
		game.run();
	}
	catch (luabridge::LuaException e){
		std::cout << e.what() << std::endl;
	}

	return 0;
}