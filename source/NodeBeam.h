#pragma once

#include <QString>
#include <QStringList>
#include <QVector>
#include <QObject>
#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QVector3D>

/*LUA*/
extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include "LuaBridge/LuaBridge.h"

/**Node Beam Editor **/


class Comments
{
public:
    Comments();
    ~Comments();
    bool AddComment(QString Comment);
    bool RemoveComment(int id);
    QString ReadComment(int id);
    int size();
    void clear();
    QStringList JBEAM_Comments;

private:

};

struct Node
{
    int GlobalID;
    QString NodeName;
    float locX;
    float locY;
    float locZ;
    QString Properties;

    //The number of group in which this node belongs to
    int GroupID;

    //Rigs of Rods nodes type, 0 = nodes, 1 = nodes2;
    bool RoRNodeType;

    //for color picking feature values between 0-255
    int green;
    int blue;

    //Comments for JBEAM file, comment will be in file
    //before this node
    Comments comments;

    //Totally unique ID, should be only calculated at node creation
    float UID;
    void CalcUID();
    void clear();

    //For Lua scripting, a way to mark a node during script.
    std::string tag;
};

struct NodeGroup
{
    int NodeGroupID;
    int NodeAmount;
    QString NodeGroupName;
    QString nameprefix;
    //Node defaults can be added later
    bool draw;
};

struct Beam
{
    int Node1GlobalID;
    QString Node1Name;
    int Node2GlobalID;
    QString Node2Name;
    QString Properties;
    int BeamGroupID;
    int BeamDefsID;
    bool HasBeamDefs;
    bool draw;

    //Comments for JBEAM file, comment will be in file
    //before this node
    Comments comments;

    void clear();
};

struct KeyValue
{
    QString name;
    QString value;
};

struct BeamDefs
{
    //Several values only for editor use
    //Unigue ID for this set of beam defaults
    int ID_BeamDefs;
    QString Name;
    //Color for GUI
    QVector <int> RGB_Color;

    //BeamNG Defaults;
    QVector <int> ValueTypes;
    QVector <KeyValue> KeyValuePairs;

};

struct BeamGroup
{
    int BeamGroupID;
    int BeamAmount;
    QString BeamGroupName;
    //Beam defaults to be added later
    bool draw;
};

struct Hubwheel
{
    QString name;
    QString group;
    int node1id;
    QString node1;
    int node2id;
    QString node2;
    int nodeSid;
    QString nodeS;
    int nodeArmid;
    QString nodeArm;
    int wheelDir;

    int arguments; //id of arguments

    //for glWidget only
    float RotationX;
    float RotationZ;
    float RotationY;
    float width;
    float radius;
};

/* Temp container for nodes and beams that
 * should be added or removed from JBEAM */

class JBEAM_Temp
{
public:
    JBEAM_Temp();
    ~JBEAM_Temp();

    //New nodes and beams
    QVector < QVector <Node> > nodes;
    QVector < QVector <Beam> > beams;

    //Nodes and beams to be removed
    QVector < Node > delete_nodes;
    QVector < Beam > delete_beams;

    bool NodesToAdd;
    bool BeamsToAdd;

    void clear();
    void AddNode(Node node);
    void AddBeam(Beam beam);
    void NewNodeGroup();
    void NewBeamGroup();
    void DeleteNode(Node node);
    void DeleteBeam(Beam beam);
};

/* Any JBEAM content type based on a list, that depends of nodes
 * for example; hubwheels, wheels, coltris, hydros, thrusters
 */
class JBEAM_ListType
{
public:

    JBEAM_ListType();
    ~JBEAM_ListType();
    QString keyword; //Keyword in JBEAM
    int nodeamount; //Amount of nodes in one instance
    QStringList nodenames; //Option name for each node, for example wheelnode 1, wheelnode 2
    enum drawtypes {NONE, BEAM, WHEEL, TRIANGLE};
    int drawtype;
    QString JBEAM_template; //template where node numbers are added
    QString JBEAM_tableheader;
    QVector <int> node_positions; //Position of each node in the template
    QVector <int> draworder; //Which nodes are drawn and in which order

    //Fucntions
    bool Add(QVector <int> item);
    //bool Remove();
    //void Clear();
    //int Count();
    void NodeRemoved(int nodeid);

    //Container
    QVector < QVector < int > > contaier;

private:


};

class NodeBeam : public QObject
{
   Q_OBJECT

public:
    bool EditorMode; //BeamNG <==> ROR axises
    bool TextBoxoutdated;

    QVector<Node> Nodes;
    QVector<Beam> Beams;
    QVector<JBEAM_ListType> ListTypes;
    QVector<Hubwheel> Hubwheels;
    QVector<NodeGroup> NodeGroups;
    QVector<BeamGroup> BeamGroups;
    QVector<BeamDefs> NodeArguments; //Node arguments
    QVector<BeamDefs> BeamDefaults; //Beam arguments
    QVector<BeamDefs> HWArguments; //Hubwheel arguments

    NodeBeam(QObject *parent  = 0);
    ~NodeBeam(void);

    int ActiveNode;
    int ActiveBeam;
    int ActiveNodeGroup;
    int ActiveBeamGroup;

    /* Clear nodebeam, deletes all contents */
    void clear();

    /*File import from Rigs of Rods */
    void ImportNBFile(const QString &fileName);
    void ParseLine(QString line, int ParsingMode);

    /* File export to RoR */
    void ExportRoR(const QString &fileName);

    /*File export to BeamNG */
    void ExportBeamNG(const QString &fileName);

    /*File import from BeamNG
     * old and outdated     */
    void ImportBeamNG(const QString &fileName);
    void ImportBeamNG_NumberValue(QString TempStr, int ParsingMode);
    void ReadJBeamTree(QString fileName);
    void WriteInJBeamTree(QString slotname);
    void ImportJSON(const QString &fileName);

    /* File import Wavefront OBJ, places new nodes and beams in temp containers */
    bool ImportWavefrontOBJ(const QString &fileName);

    /* Open and Save current vehicle project to Editors own file format*/
    void OpenProject(const QString &fileName);
    void SaveAs(const QString &fileName);

    QString VehicleName;
    QString VehicleInGameName;
    QVector<QString> VehicleAuthors;

    float GlobalMass;
    float minimass;
    float loadmass;

    //For color picking, the color counters
    //Couners are incremented when new nodes are added
    //Every node must have individual color
    int green;
    int blue;

    //Temprorary values can be saved in these
    Beam TempBeam;
    QVector <Beam> TempBeams;
    QList < QVector <Beam> > TempBeamsL;

    Node TempNode;
    QVector <Node> TempNodes;
    Hubwheel TempHubwheel;
    NodeGroup TempNodeGroup;
    BeamGroup TempBeamGroup;
    BeamDefs TempBeamDefaults;

    //JBEAM temp containers, for new nodes
    //and beams going to JBEAM
    JBEAM_Temp JBEAM_temp;

    //The line where JBEAM parse error occured
    int JbeamErrorLine;

    //Currently selected nodes, by GlobalID
    QVector <int> SelectedNodes;
    QVector <int> SelectedNodes2;

    //Currently selected beams
    QVector <int> SelectedBeams;

    /* Add node functions */
    int AddNodeT(float locx, float locy, float locz, int NodeGroupID, QString NodeName);
    int AddNode();
    int AddNode(int NodeGroupID);
    void ExtrudeNodes();

    /* Delete node */
    void DeleteNode(int NodeGlobalID);

    //Put selected nodes in delete queue, but don't delete
    void cutNodes();

    /* Beam functions */
    void AddBeam(int Node1ID, int Node2ID, int BeamGroup, bool jbeamAdd);
    void AddBeamT(bool jbeamAdd);
    void DeleteBeam(int BeamIndex);

    /* Group functions */
    int NewBeamGroup(QString groupname); //Make new beam group
    int MoveToGroup(int NodeGlobalID, int GroupID); //Move node to group
    void MoveToGroupBeam(); //Move beam to group
    void DeleteNodeGroup(int NodeGroupID); //Delete node group

    /* Duplicate nodes */
    void DuplicateNodes();

    /* Merge nodes */
    void MergeSelectedNodes(float distance);
    float CalcDistance(Node node1, Node node2);

    /* Select nodes */
    void SelectNodesLoc(float x0, float y0, float z0, float x1, float y1, float z1); //2D Rectangle selection
    void SelectNodes3D(QVector4D RectSel_1V, QVector4D RectSel_2V, //3D Rectangle selection
                       QVector4D RectSel_3V, QVector4D RectSel_4V,
                       QVector4D CameraLocation, bool AddToSelection);

    /* Mirror selected nodes */
    void MirrorNodes(int axis);

    /* Make new set of Beam arguments */
    void NewBeamArguments();
    void AddBeamArgument(QString key, QString value);
    void NewBeamArgumentsFromPrevious();
    void ModifyBeamArgument(QString key, QString value, int BeamdefsSet);

    /* Find node by name */
    int FindNodeByName(QString *nodename);
    int FindBeamNodeByName(QString nodename);

    /* New name system */
    QVector <QString> NodeNamePrefixes;
    QVector <int> NodeNameCalculators;
    int DefaultPrefixCalculator;
    QString CurrentNamePrefix;

    void NewNodeNamePrefix();
    void NewNodeNamePrefix(QString prefix);

    QString NewNodeName(QString prefix);

    bool CheckDoubles(QString NameSuggestion);
    int CheckPrefixExistance(QString prefix);

    /* Hubwheels */
    void AddHubWheel(); //put values in temphubwheel before calling this
    void CalcHubWheelRotation(int wheelid);
    double calculate_length(double x, double y);

    /* LUA Script */
    void RunLUAScript(QString filename);
    //Lua state
    lua_State* L;

    //Functions for LUA
    void LuaLog(const std::string msg);
    void LuaAddNode(const std::string name, float locx, float locy, float locz);
    void LuaAddNodeTag(const std::string name, float locx, float locy, float locz, const std::string tag);
    void LuaAddBeam(const std::string node1, const std::string node2);
    void LuaAddComment(const std::string comment);
    luabridge::LuaRef LuaGetNode(const std::string node);
    luabridge::LuaRef LuaGetNodei(int i);
    luabridge::LuaRef LuaGetAllNodes();
    int LuaGetNodeCount();
    luabridge::LuaRef LuaGetJbeamtext();
    void LuaSetJbeamtext(const std::string text);
    luabridge::LuaRef LuaInputValue(const std::string htmlfile);

    //Result of html input dialog will be returned here for lua
    QList <QStringList> Lua_InputDialogResult;

    //Comments that are added to JBEAM from Lua script.
    Comments LuaComments;
    bool LuaComment;

    QString Lua_Jbeamtext;

    /* 3D Editing */
    //Calculate the middle point of selected nodes
    bool Editing3D_CalculateSelectionCenter();

    //Center point of nodes in Selected nodes
    QVector3D SelectionCenterPos;

    /* JBEAM TextEdit parsing functions */
    QJsonParseError ParseJBEAM_TextEdit(QString JbeamTextStr);
    QJsonParseError ParseJBEAM_TextEditP1(QString JbeamTextStr);
    void ParseJBEAM_TextEditP2();

    /* JBEAM Save As */
    bool JBEAM_SaveAs(const QString &fileName, QString JBEAM_Text);

    //Temp container when parsing multiple files
    QList < QList <QJsonObject> > JbeamParsingTemp;

signals:
    //Write a message to scipts tab's logging textbox from LUA script.
    void LUA_log(QString msg);
    void LUA_getJbeamText();
    void LUA_setJbeamText(QString text);
    void LUA_HtmlInputDialog(QString htmlfile);

    void nodeCountChanged(int count);
    void beamCountChanged(int count);

private:

    /*When parsing a line from a file, in grid_index
    is the current position, for example in nodes section */
    int grid_index;

    /* JBEAM TextEdit parsing functions */
    void JBEAM_FixCommas_NextChar(QString &sample, QChar &nextchar, int &nextchar_i);
    void JBEAM_FixCommas_PrevChar(QString &sample, QChar &prevchar, int &prevchar_i);
    QByteArray JBEAM_FixCommas(QByteArray JbeamText);
    bool JBEAM_ParseNodesArray(QJsonArray JbeamNodesArray);
    bool JBEAM_ParseBeamsArray(QJsonArray *JbeamBeamsArray);
    bool JBEAM_ParseOtherArray(QJsonArray JbeamArray, int ListType_id);
    QByteArray JBEAM_RemoveComments(QByteArray JbeamText);
    void JBEAM_ParseComment(QString comment);

    //Load jbeam content types from json files
    void Load_ListTypes();

};

