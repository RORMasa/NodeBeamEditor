#include <QString>
#include <QtTest>
#include <QDebug>
#include "../../NodeBeam.h"
#include <QApplication>

class NodeBeamTests : public QObject
{
    Q_OBJECT

public:
    NodeBeamTests();
    ~NodeBeamTests();
    NodeBeam * NB;

    bool LoadTestMaterial(QString filename);

private Q_SLOTS:
    void addnode();
    void deletenode();
    void parsejbeam();
    void parsejbeam2();
    void triangletest();
};

NodeBeamTests::NodeBeamTests()
{
    NB = new NodeBeam;
    qDebug() << "Loaded list types: " << NB->ListTypes.size();
}

NodeBeamTests::~NodeBeamTests()
{
    delete NB;
}

void NodeBeamTests::addnode()
{
    bool ok = 1;
    NB->TempNode.locX = 0.1;
    NB->TempNode.locY = 0.2;
    NB->TempNode.locZ = 0.3;
    int node = NB->AddNode();

    if(NB->Nodes.size() != (node+1)) ok = 0;

    QVERIFY2(ok == 1, "Failure");
}

void NodeBeamTests::deletenode()
{
    //Things to check
    //-All beams connecting to this node are removed
    //-All other components connecting to the node are removed

    NB->DeleteNode(0);
    QVERIFY2(true, "Failure");
}

void NodeBeamTests::parsejbeam()
{
    //Reset the nodebeam
    NB->clear();
    QString problem = "File could not be opened";
    bool ok = 1;

    if(!LoadTestMaterial(tr("triangletest.jbeam"))) ok=0;
    else
    {
        if(NB->Nodes.size() != 4)
        {
            ok = 0;
            problem = "Node count does not match";
        }
        if(NB->Beams.size() != 1)
        {
            ok = 0;
            problem = "Beam count does not match";
        }
        for(int i=0; i<NB->ListTypes.size();i++)
        {
            if(NB->ListTypes.at(i).keyword == "triangles")
            {
                if(NB->ListTypes.at(i).contaier.size() != 2)
                {
                    ok = 0;
                    problem = "triangle count does not match";
                }
            }
        }
    }

    QVERIFY2(ok == 1, problem.toStdString().c_str());
}
void NodeBeamTests::parsejbeam2()
{
    QStringList testJbeams;
    testJbeams << "FixCommaTest.jbeam";

    bool ok = 1;
    QString problem = "Error at ";

    for(int i=0; i<testJbeams.size(); i++)
    {
        NB->clear();
        NB->JbeamParsingTemp.clear();
        if(!LoadTestMaterial(testJbeams.at(i)))
        {
            ok = 0;
        }
    }
    QVERIFY2(ok == 1, problem.toStdString().c_str());
}

//Load file with two triangles and remove one node
void NodeBeamTests::triangletest()
{
    //Reset the nodebeam
    NB->clear();
    QString problem = "File could not be opened";
    bool ok = 1;

    if(!LoadTestMaterial(tr("triangletest.jbeam"))) ok=0;
    else
    {
        qDebug() << NB->Nodes.at(0).NodeName;
        NB->DeleteNode(0);
        qDebug() << NB->Nodes.size();

        for(int i=0; i<NB->ListTypes.size();i++)
        {
            if(NB->ListTypes.at(i).keyword == "triangles")
            {
                if(NB->ListTypes.at(i).contaier.size() != 0)
                {
                    ok = 0;
                    problem = "Triangle count does not match";
                }
            }
        }
    }

    QVERIFY2(ok == 1, problem.toStdString().c_str());

}

//Read JBEAM file test contents
bool NodeBeamTests::LoadTestMaterial(QString filename)
{
    bool ok = 1;
    QFile file;
    file.setFileName(filename);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);

        #ifndef QT_NO_CURSOR
            QApplication::setOverrideCursor(Qt::WaitCursor);
        #endif

        QString FileContents = in.readAll();

        #ifndef QT_NO_CURSOR
            QApplication::restoreOverrideCursor();
        #endif

        QJsonParseError ParseError;
        ParseError = NB->ParseJBEAM_TextEditP1(FileContents);
        if(ParseError.offset>0)
        {
            ok = 0;
        }
        else NB->ParseJBEAM_TextEditP2();
        file.close();
    }
    if(ok) return true;
    else return false;
}

QTEST_MAIN(NodeBeamTests)

#include "tst_nodebeamtests.moc"
