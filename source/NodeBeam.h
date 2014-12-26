#pragma once

#include <QString>
#include <QVector>
#include <QObject>
#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QVector3D>

/*LUA*/
//extern "C"
//{
//#include "lua.h"
//#include "lualib.h"
//#include "lauxlib.h"
//}
//#include "LuaBridge/LuaBridge.h"

/**Node Beam Editor **/

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

class NodeBeam
{

public:

    bool EditorMode; //BeamNG <==> ROR axises

    QVector<Node> Nodes;
    QVector<Beam> Beams;
    QVector<Hubwheel> Hubwheels;
    QVector<NodeGroup> NodeGroups;
    QVector<BeamGroup> BeamGroups;
    QVector<BeamDefs> NodeArguments; //Node arguments
    QVector<BeamDefs> BeamDefaults; //Beam arguments
    QVector<BeamDefs> HWArguments; //Hubwheel arguments

    NodeBeam();
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

    /*File import from BeamNG*/
    void ImportBeamNG(const QString &fileName);
    void ImportBeamNG_NumberValue(QString TempStr, int ParsingMode);
    /*New JBeam tree importer exporter*/
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
    QVector <Beam> TempBeams2;
    QVector <Beam> TempBeams3;
    Node TempNode;
    QVector <Node> TempNodes;
    Hubwheel TempHubwheel;
    NodeGroup TempNodeGroup;
    BeamGroup TempBeamGroup;
    BeamDefs TempBeamDefaults;
    //void ResetTempNode();

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

    /* Beam functions */
    void AddBeam(int Node1ID, int Node2ID, int BeamGroup);
    void AddBeamT();
    void DeleteBeam(int BeamIndex);

    /* Group functions */
    int NewBeamGroup(QString groupname); //Make new beam group
    int MoveToGroup(int NodeGlobalID, int GroupID); //Move node to group
    void MoveToGroupBeam(); //Move beam to group
    void DeleteNodeGroup(int NodeGroupID); //Delete node group

    /* Duplicate nodes */
    void DuplicateNodes();

    /* Select nodes */
    void SelectNodesLoc(float x0, float y0, float z0, float x1, float y1, float z1);

    /* Make new set of Beam arguments */
    void NewBeamArguments();
    void AddBeamArgument(QString key, QString value);
    void NewBeamArgumentsFromPrevious();
    void ModifyBeamArgument(QString key, QString value, int BeamdefsSet);

    /* Find node by name */
    int FindNodeByName(QString nodename);
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
    //void RunLUAScript();
    //int LUAtesti(lua_State *L);

    /* 3D Editing */
    bool Editing3D_CalculateSelectionCenter();

    //Center point of nodes in Selected nodes
    QVector3D SelectionCenterPos;

    //3D Rectangle selection
    void SelectNodes3D(QVector4D RectSel_1V, QVector4D RectSel_2V,
                       QVector4D RectSel_3V, QVector4D RectSel_4V,
                       QVector4D CameraLocation);



    /* JBEAM TextEdit parsing functions */
    QJsonParseError ParseJBEAM_TextEdit(QByteArray JbeamInputText);

    /* JBEAM Save As */
    bool JBEAM_SaveAs(const QString &fileName, QString JBEAM_Text);

private:

    /*When parsing a line from a file, in grid_index
    is the current position, for example in nodes section */
    int grid_index;

    /* JBEAM TextEdit parsing functions */
    void JBEAM_FixCommas_NextChar(QString &sample, QChar &nextchar, int &nextchar_i);
    void JBEAM_FixCommas_PrevChar(QString &sample, QChar &prevchar, int &prevchar_i);
    QByteArray JBEAM_FixCommas(QByteArray JbeamText);
    bool JBEAM_ParseNodesArray(QJsonArray JbeamNodesArray);
    bool JBEAM_ParseBeamsArray(QJsonArray JbeamBeamsArray);
    QByteArray JBEAM_RemoveComments(QByteArray JbeamText);
    void JBEAM_ParseComment(QString comment);

};

