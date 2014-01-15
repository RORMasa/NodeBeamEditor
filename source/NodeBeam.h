#pragma once

#include <QString>
#include <QVector>
#include <QObject>
#include <QXmlStreamReader>

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

    /*File import Wavefront OBJ*/
    void ImportWavefrontOBJ(const QString &fileName);

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
    Node TempNode;
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
    void AddNodeT(float locx, float locy, float locz, int NodeGroupID, QString NodeName);
    int AddNode();
    int AddNode(int NodeGroupID);
    void ExtrudeNodes();

    /* Delete node */
    void DeleteNode(int NodeGlobalID);

    /* Add beam functions*/
    void AddBeam(int Node1ID, int Node2ID, int BeamGroup);
    void AddBeamT();
    int NewBeamGroup(QString groupname);

    /* Delete Beam */
    void DeleteBeam(int BeamIndex);

    /* Move node to group */
    int MoveToGroup(int NodeGlobalID, int GroupID);

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
    bool NewNodeNamePrefix(QString prefix);

    QString NewNodeName(QString prefix);

    bool CheckDoubles(QString NameSuggestion);
    int CheckPrefixExistance(QString prefix);

    /* Hubwheels */
    void AddHubWheel(); //put values in temphubwheel before calling this
    void CalcHubWheelRotation(int wheelid);
    double calculate_length(double x, double y);

private:

    /*When parsing a line from a file, in grid_index
    is the current position, for example in nodes section */
    int grid_index;




};
