include(LuaTargets)

set(LUA_FILES
	Actions.lua
	AddAppDirectory.lua
	DebugAxes.lua
	getScriptFilename.lua
	nav-app-skeleton.lua
	nav-testbed-launcher.lua
	Navigator.lua
	osgnav-testbed.lua
	osgTools.lua
	runfile.lua
	Scene.lua
	sleep.lua
	StateMachine.lua
	StockModels.lua
	Text.lua
	TransparentGroup.lua
	vrjlua-config.lua
	vrjlua-init.lua)


add_lua_target(luaFiles "${CMAKE_BINARY_DIR}/${LUA_DIR}" ${LUA_FILES})

set_property(TARGET
	luaFiles
	PROPERTY
	PROJECT_LABEL
	"VRJLua Lua Scripts")

# Install into the install location, as well
install_lua_target(luaFiles
	DESTINATION
	${LUA_DIR}
	COMPONENT
	runtime)
