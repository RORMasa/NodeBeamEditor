
#include "NodeBeam.h"
#include <QDebug>
using namespace luabridge;

void NodeBeam::RunLUAScript()
{
    qDebug() << "Lua!";

    //Intialize Lua state
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    //Register functions to be called from Lua
    getGlobalNamespace(L)
            .beginClass<NodeBeam>("NodeBeam")
            .addConstructor<void (*) (void)>()
            .addFunction("AddNode",&NodeBeam::LuaAddNode)
            .endClass();

    //Link this nodebeam object to Lua
    push (L, this);
    lua_setglobal (L, "NB");

    //Run Lua script
    luaL_dofile(L, "script.lua");
    lua_pcall(L, 0, 0, 0);
}


void NodeBeam::LuaPRINT()
{
    qDebug() << "Lua toimii.";
}

void NodeBeam::LuaAddNode(const std::string& name, float locx, float locy, float locz)
{
    TempNode.NodeName = QString::fromStdString(name);
    TempNode.locX = locx;
    TempNode.locY = locy;
    TempNode.locZ = locz;
    qDebug() << "Adding node from lua.";
    AddNode();
}
