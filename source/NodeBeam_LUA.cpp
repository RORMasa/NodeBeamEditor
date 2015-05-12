
#include "NodeBeam.h"
#include <QDebug>
#include <QString>

using namespace luabridge;

void NodeBeam::RunLUAScript(QString filename)
{
    qDebug() << "Lua!";
    QStringList filenamel = filename.split("/");
    QString msg = "Running Lua script " + filenamel.last();
    emit LUA_log(msg);

    //Intialize Lua state
    L = luaL_newstate();
    luaL_openlibs(L);

    //Register functions to be called from Lua
    getGlobalNamespace(L)
            .beginClass<NodeBeam>("NodeBeam")
            .addConstructor<void (*) (void)>()
            .addFunction("AddNode",&NodeBeam::LuaAddNode)
            .addFunction("AddNodet",&NodeBeam::LuaAddNodeTag)
            .addFunction("AddBeam",&NodeBeam::LuaAddBeam)
            .addFunction("AddComment",&NodeBeam::LuaAddComment)
            .addFunction("Log",&NodeBeam::LuaLog)
            .addFunction("GetNode", &NodeBeam::LuaGetNode)
            .addFunction("GetNodei", &NodeBeam::LuaGetNodei)
            .addFunction("GetNodeCount", &NodeBeam::LuaGetNodeCount)
            .addFunction("GetNodes", &NodeBeam::LuaGetAllNodes)
            .addFunction("GetJbeamText", &NodeBeam::LuaGetJbeamtext)
            .addFunction("SetJbeamText", &NodeBeam::LuaSetJbeamtext)
            .addFunction("InputValue", &NodeBeam::LuaInputValue)
            .endClass();

    //Link this nodebeam object to Lua
    push (L, this);
    lua_setglobal (L, "NB");

    //Run Lua script
    luaL_dofile(L, filename.toStdString().c_str());
    lua_pcall(L, 0, 0, 0);

    msg = "Script finished";
    emit LUA_log(msg);

    lua_close(L);

}

//Add message to script log
void NodeBeam::LuaLog(const std::string msg)
{
    emit LUA_log(QString::fromStdString(msg));
}

//Ability to add comments from LUA script into the JBEAM
void NodeBeam::LuaAddComment(const std::string comment)
{
    LuaComments.AddComment(QString::fromStdString(comment));
    LuaComment = 1; //one, until comment is handled in a node or beam adding
}

//Add node from lua
void NodeBeam::LuaAddNode(const std::string name, float locx, float locy, float locz)
{
    TempNode.NodeName = QString::fromStdString(name);
    TempNode.locX = locx;
    TempNode.locY = locy;
    TempNode.locZ = locz;
    TempNode.tag.clear();
    TempNode.comments.clear();
    if(LuaComment)
    {
        TempNode.comments = LuaComments;
        LuaComment=0;
        LuaComments.clear();
    }
    AddNode();
}

//Add node with a tag from lua
void NodeBeam::LuaAddNodeTag(const std::string name, float locx, float locy, float locz, const std::string tag)
{
    TempNode.NodeName = QString::fromStdString(name);
    TempNode.locX = locx;
    TempNode.locY = locy;
    TempNode.locZ = locz;
    TempNode.tag = tag;
    TempNode.comments.clear();
    if(LuaComment)
    {
        TempNode.comments = LuaComments;
        LuaComment=0;
        LuaComments.clear();
    }
    AddNode();
}

//Add beam from lua
void NodeBeam::LuaAddBeam(const std::string node1, const std::string node2)
{
    TempBeam.Node1Name = QString::fromStdString(node1);
    TempBeam.Node2Name = QString::fromStdString(node2);
    TempBeam.Node1GlobalID = FindBeamNodeByName(TempBeam.Node1Name);
    TempBeam.Node2GlobalID = FindBeamNodeByName(TempBeam.Node2Name);
    if((TempBeam.Node1GlobalID<0) || (TempBeam.Node2GlobalID<0)) TempBeam.draw = 0;
    else TempBeam.draw = 1;
    TempBeam.comments.clear();
    if(LuaComment)
    {
        TempBeam.comments = LuaComments;
        LuaComment=0;
        LuaComments.clear();
    }
    AddBeamT();
}

//Get node by name in Lua
LuaRef NodeBeam::LuaGetNode(const std::string node)
{
    LuaRef n = newTable(L);
    QString name = QString::fromStdString(node);
    for(int i=0; i<Nodes.size(); i++)
    {
        if(Nodes.at(i).NodeName == name)
        {
            n["i"] = i;
            n["x"] = Nodes.at(i).locX;
            n["y"] = Nodes.at(i).locY;
            n["z"] = Nodes.at(i).locZ;
            n["tag"] = Nodes.at(i).tag;
            break;
        }
    }
    return n;
}

//Get node by index in nodes vector in Lua
LuaRef NodeBeam::LuaGetNodei(int i)
{
    LuaRef n = newTable(L);
    if(Nodes.size()>i)
    {
        n["name"] = Nodes.at(i).NodeName.toStdString();
        n["x"] = Nodes.at(i).locX;
        n["y"] = Nodes.at(i).locY;
        n["z"] = Nodes.at(i).locZ;
        n["tag"] = Nodes.at(i).tag;
    }
    return n;
}

//Get all nodes in Lua
LuaRef NodeBeam::LuaGetAllNodes()
{
    LuaRef nodes = newTable(L);
    for(int i=0; i<Nodes.size(); i++)
    {
        LuaRef node = newTable(L);
        node["name"] = Nodes.at(i).NodeName.toStdString();
        node["x"] = Nodes.at(i).locX;
        node["y"] = Nodes.at(i).locY;
        node["z"] = Nodes.at(i).locZ;
        node["tag"] = Nodes.at(i).tag;
        nodes[i] = node;

    }
    return nodes;
}

//Get node amount in Lua
int NodeBeam::LuaGetNodeCount()
{
    return Nodes.size();
}

//Get contents of JBEAM textbox in Lua.
LuaRef NodeBeam::LuaGetJbeamtext()
{
    LuaRef text = newTable(L);
    emit LUA_getJbeamText();
    text["jbeam"] = this->Lua_Jbeamtext.toStdString();
    return text;
}

//Set contents of JBEAM textbox in Lua.
void NodeBeam::LuaSetJbeamtext(const std::string text)
{
    emit LUA_setJbeamText(QString::fromStdString(text));
}

//Get input value from user
LuaRef NodeBeam::LuaInputValue(const std::string htmlfile)
{
    emit LUA_HtmlInputDialog(QString::fromStdString(htmlfile));

    LuaRef value = newTable(L);
    if(this->Lua_InputDialogResult.size()>0)
    {
        for(int i=0; i<Lua_InputDialogResult.size();i++)
        {
            value[Lua_InputDialogResult.at(i).at(0).toStdString()] = Lua_InputDialogResult.at(i).at(1).toStdString();
        }
    }
    return value;
}


/* == Comments class == */
Comments::Comments()
{

}
Comments::~Comments()
{
    JBEAM_Comments.clear();
}
bool Comments::AddComment(QString Comment)
{
    JBEAM_Comments.append(Comment);
    return true;
}
bool Comments::RemoveComment(int id)
{
    if(id<JBEAM_Comments.size())
    {
        JBEAM_Comments.removeAt(id);
        return true;
    }
    else return false;
}
QString Comments::ReadComment(int id)
{
    if(id<JBEAM_Comments.size())
    {
        return JBEAM_Comments.at(id);
    }
    else
    {
        QString empty = "";
        return empty;
    }
}
int Comments::size()
{
    return JBEAM_Comments.size();
}
void Comments::clear()
{
    JBEAM_Comments.clear();
}

/* Container for temp beams and nodes going to JBEAM widget */
JBEAM_Temp::JBEAM_Temp()
{
    QVector <Node> NewNodes;
    nodes.append(NewNodes);

    QVector <Beam> NewBeams;
    beams.append(NewBeams);

    NodesToAdd=0;
    BeamsToAdd=0;
}

JBEAM_Temp::~JBEAM_Temp()
{
    clear();
}

void JBEAM_Temp::clear()
{
    nodes.clear();
    beams.clear();
    delete_nodes.clear();
    delete_beams.clear();

    QVector <Node> NewNodes;
    nodes.append(NewNodes);

    QVector <Beam> NewBeams;
    beams.append(NewBeams);

    NodesToAdd = 0;
    BeamsToAdd = 0;
}

void JBEAM_Temp::AddNode(Node node)
{
    nodes[nodes.size()-1].append(node);
}

void JBEAM_Temp::AddBeam(Beam beam)
{
    beams[beams.size()-1].append(beam);
}
void JBEAM_Temp::NewNodeGroup()
{
    QVector <Node> NewNodes;
    nodes.append(NewNodes);
}
void JBEAM_Temp::NewBeamGroup()
{
    QVector <Beam> NewBeams;
    beams.append(NewBeams);
}
void JBEAM_Temp::DeleteNode(Node node)
{
    delete_nodes.append(node);
}
void JBEAM_Temp::DeleteBeam(Beam beam)
{
    delete_beams.append(beam);
}
