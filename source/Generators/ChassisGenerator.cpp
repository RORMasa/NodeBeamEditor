#include "ChassisGenerator.h"
#include "ui_ChassisGenerator.h"
#include <QString>
#include <QVector>
#include <qmath.h>
#include <QDebug>

ChassisGenerator::ChassisGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChassisGenerator)
{
    ui->setupUi(this);
}

ChassisGenerator::~ChassisGenerator()
{
    delete ui;
}

void ChassisGenerator::EmptyAll()
{
     NodeAmount=0;
     for(i=0;i<15;i++) BeamNumber[i]=0;
     if(width <= 0) width = 1;
     if(height <= 0) height = 1;
     if(length <= 0) length = 1;
     if(segments <= 0) segments = 1;
}

void ChassisGenerator::generate_chassis()
{
     EmptyAll();
     //Nodes
     //SegmentSpacing.
     int NodeNumber=0, TempNode=0;
     float offX, offY, offZ, StartX;

     offX = length/segments;
     offY = -height/2;
     offZ = width/2;

     StartX = -length/2;

     for(i = 0; i < (segments+1); i++)
     {


         for(i2 = 0; i2 < 2; i2++)
         {
             for(i3 = 0; i3 < 2; i3++)
             {

               //set.push_back(vector<nodes>());
               set.resize(set.size()+1);
               set[0].resize(NodeNumber+1);
               set[0][NodeNumber].node_number = NodeNumber;
               set[0][NodeNumber].locx = StartX;
               set[0][NodeNumber].locy = offY;
               set[0][NodeNumber].locz = offZ;

             /*  qDebug() << "Node information for node " << NodeNumber << endl;*/
               qDebug() << set[0][NodeNumber].locx;
               qDebug() << set[0][NodeNumber].locy;
               qDebug() << set[0][NodeNumber].locz;

               NodeNumber++;

               offZ = offZ*(-1);

               }

               offY = offY*(-1);
         }
         StartX = StartX+offX;
     }

     NodeAmount = NodeNumber;
   /*  qDebug() << "Nodes generated: " << NodeAmount << endl;   */
     //Beams

     //SideBeams
     for(i2=0; i2<(segments+1);i2++)
     {
         for(i=0; i<2;i++)
         {
                  bset.resize(bset.size()+1);
                  bset[0].resize(BeamNumber[0]+1);
                  bset[0][BeamNumber[0]].node1 = i+TempNode;
                  bset[0][BeamNumber[0]].node2 = i+TempNode+2;
                 /* qDebug() << "Beam between: " << bset[0][BeamNumber[0]].node1 << " and " << bset[0][BeamNumber[0]].node2 << endl;*/
                  BeamNumber[0]++;
         }
     TempNode = TempNode+4;
     }
     TempNode = 0;

     for(i=0; i<segments; i++)
     {
     for(i2=0; i2<4; i2++)
     {
               bset[0].resize(BeamNumber[0]+1);
               bset[0][BeamNumber[0]].node1 = i2+TempNode;
               bset[0][BeamNumber[0]].node2 = i2+TempNode+4;
             /*  qDebug() << "Beam between: " << bset[0][BeamNumber[0]].node1 << " and " << bset[0][BeamNumber[0]].node2 << endl; */
               BeamNumber[0]++;
     }
     TempNode = TempNode+4;

     }

     TempNode = 0;

     for(i=0; i<segments; i++)
     {
     for(i2=0; i2<2; i2++)
     {
               bset[0].resize(BeamNumber[0]+1);
               bset[0][BeamNumber[0]].node1 = i2+TempNode;
               bset[0][BeamNumber[0]].node2 = i2+TempNode+6;
             /*  qDebug() << "Beam between: " << bset[0][BeamNumber[0]].node1 << " and " << bset[0][BeamNumber[0]].node2 << endl;  */
               BeamNumber[0]++;
     }
     TempNode = TempNode+4;
     }

     TempNode = 2;

     for(i=0; i<segments; i++)
     {
     for(i2=0; i2<2; i2++)
     {
               bset[0].resize(BeamNumber[0]+1);
               bset[0][BeamNumber[0]].node1 = i2+TempNode;
               bset[0][BeamNumber[0]].node2 = i2+TempNode+2;
             /*  qDebug() << "Beam between: " << bset[0][BeamNumber[0]].node1 << " and " << bset[0][BeamNumber[0]].node2 << endl;*/
               BeamNumber[0]++;
     }
     TempNode = TempNode+4;
     }

     //Horizonal Beams
     TempNode = 0;
     for(i=0; i<segments+1; i++)
     {
               bset[1].resize(BeamNumber[1]+1);
               bset[1][BeamNumber[1]].node1 = TempNode;
               bset[1][BeamNumber[1]].node2 = TempNode+1;
               qDebug() << "Beam between: " << bset[1][BeamNumber[1]].node1 << " and " << bset[1][BeamNumber[1]].node2;
               BeamNumber[1]++;

               bset[1].resize(BeamNumber[1]+1);
               bset[1][BeamNumber[1]].node1 = TempNode+2;
               bset[1][BeamNumber[1]].node2 = TempNode+3;
               qDebug() << "Beam between: " << bset[1][BeamNumber[1]].node1 << " and " << bset[1][BeamNumber[1]].node2;
               BeamNumber[1]++;

     TempNode = TempNode+4;
     }

     TempNode = 0;
     for(i=0; i<segments+1; i++)
     {
               bset[1].resize(BeamNumber[1]+1);
               bset[1][BeamNumber[1]].node1 = TempNode;
               bset[1][BeamNumber[1]].node2 = TempNode+3;
            /*   qDebug() << "Beam between: " << bset[1][BeamNumber[1]].node1 << " and " << bset[1][BeamNumber[1]].node2 << endl;  */
               BeamNumber[1]++;

               bset[1].resize(BeamNumber[1]+1);
               bset[1][BeamNumber[1]].node1 = TempNode+1;
               bset[1][BeamNumber[1]].node2 = TempNode+2;
             /*  qDebug() << "Beam between: " << bset[1][BeamNumber[1]].node1 << " and " << bset[1][BeamNumber[1]].node2 << endl;  */
               BeamNumber[1]++;

     TempNode = TempNode+4;
     }


     //FrameX Beams
     TempNode = 0;

     for(i=0; i<segments; i++)
     {
               bset[2].resize(BeamNumber[2]+1);
               bset[2][BeamNumber[2]].node1 = TempNode;
               bset[2][BeamNumber[2]].node2 = TempNode+5;
             /*  qDebug() << "Beam between: " << bset[2][BeamNumber[2]].node1 << " and " << bset[2][BeamNumber[2]].node2 << endl; */
               BeamNumber[2]++;

               bset[2].resize(BeamNumber[2]+1);
               bset[2][BeamNumber[2]].node1 = TempNode+1;
               bset[2][BeamNumber[2]].node2 = TempNode+4;
            /*   qDebug() << "Beam between: " << bset[2][BeamNumber[2]].node1 << " and " << bset[2][BeamNumber[2]].node2 << endl; */
               BeamNumber[2]++;

               bset[2].resize(BeamNumber[2]+1);
               bset[2][BeamNumber[2]].node1 = TempNode+2;
               bset[2][BeamNumber[2]].node2 = TempNode+7;
            /*   qDebug() << "Beam between: " << bset[2][BeamNumber[2]].node1 << " and " << bset[2][BeamNumber[2]].node2 << endl; */
               BeamNumber[2]++;

               bset[2].resize(BeamNumber[2]+1);
               bset[2][BeamNumber[2]].node1 = TempNode+3;
               bset[2][BeamNumber[2]].node2 = TempNode+6;
             /*  qDebug() << "Beam between: " << bset[2][BeamNumber[2]].node1 << " and " << bset[2][BeamNumber[2]].node2 << endl; */
               BeamNumber[2]++;

     TempNode = TempNode+4;
     }

     //FrameMiddle Beams
     TempNode = 0;

     for(i=0; i<segments; i++)
     {
               bset[3].resize(BeamNumber[3]+1);
               bset[3][BeamNumber[3]].node1 = TempNode;
               bset[3][BeamNumber[3]].node2 = TempNode+7;
            /*   qDebug() << "Beam between: " << bset[3][BeamNumber[3]].node1 << " and " << bset[3][BeamNumber[3]].node2 << endl; */
               BeamNumber[3]++;

               bset[3].resize(BeamNumber[3]+1);
               bset[3][BeamNumber[3]].node1 = TempNode+1;
               bset[3][BeamNumber[3]].node2 = TempNode+6;
            /*   qDebug() << "Beam between: " << bset[3][BeamNumber[3]].node1 << " and " << bset[3][BeamNumber[3]].node2 << endl;  */
               BeamNumber[3]++;

               bset[3].resize(BeamNumber[3]+1);
               bset[3][BeamNumber[3]].node1 = TempNode+2;
               bset[3][BeamNumber[3]].node2 = TempNode+5;
             /*  qDebug() << "Beam between: " << bset[3][BeamNumber[3]].node1 << " and " << bset[3][BeamNumber[3]].node2 << endl;  */
               BeamNumber[3]++;

               bset[3].resize(BeamNumber[3]+1);
               bset[3][BeamNumber[3]].node1 = TempNode+3;
               bset[3][BeamNumber[3]].node2 = TempNode+4;
             /*  qDebug() << "Beam between: " << bset[3][BeamNumber[3]].node1 << " and " << bset[3][BeamNumber[3]].node2 << endl;  */
               BeamNumber[3]++;

     TempNode = TempNode+4;
     }
}

void ChassisGenerator::MoveToEditor()
{
    qDebug() << "Moving to editor";
    int nodecounter = NBPointer->Nodes.size();
    qDebug() << "Moving nodes";

    //create empty node group.

    qDebug()<<"Creating node group.";
    NBPointer->NodeGroups.resize(NBPointer->NodeGroups.size()+1);
    NBPointer->NodeGroups[NBPointer->NodeGroups.size()-1].NodeGroupName = "Chassis";
    NBPointer->NodeGroups[NBPointer->NodeGroups.size()-1].NodeGroupID = NBPointer->NodeGroups.size()-1;
    NBPointer->NodeGroups[NBPointer->NodeGroups.size()-1].draw=1;

    for(int i=0; i<set[0].size() ;i++)
    {

        NBPointer->NodeGroups[NBPointer->NodeGroups.size()-1].NodeAmount++;

        QString NodeNamePrefix = "cha_";
        NodeNamePrefix.append(QString::number(i));
        NBPointer->Nodes.resize(NBPointer->Nodes.size()+1);
        NBPointer->Nodes[NBPointer->Nodes.size()-1].NodeName = NodeNamePrefix;
        NBPointer->Nodes[NBPointer->Nodes.size()-1].GlobalID = NBPointer->Nodes.size()-1;
        NBPointer->Nodes[NBPointer->Nodes.size()-1].locY=set[0][i].locx;
        NBPointer->Nodes[NBPointer->Nodes.size()-1].locZ=set[0][i].locy;
        NBPointer->Nodes[NBPointer->Nodes.size()-1].locX=set[0][i].locz;
        NBPointer->Nodes[NBPointer->Nodes.size()-1].green=NBPointer->green;
        NBPointer->Nodes[NBPointer->Nodes.size()-1].blue=NBPointer->blue;

        NBPointer->green++;
        if(NBPointer->green > 255)
        {
            NBPointer->green = 0;
            NBPointer->blue++;
        }

    }
    qDebug() << "Moving beams";
    NBPointer->BeamGroups.resize(NBPointer->BeamGroups.size()+1);
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].draw=1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount=0;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupName="FrameSide";
    for(int i=0; i<bset[0].size();i++)
    {
        NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount++;
        NBPointer->Beams.resize(NBPointer->Beams.size()+1);
        NBPointer->Beams[NBPointer->Beams.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node1GlobalID=bset[0][i].node1 + nodecounter;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node2GlobalID=bset[0][i].node2 + nodecounter;
    }

    NBPointer->BeamGroups.resize(NBPointer->BeamGroups.size()+1);
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].draw=1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount=0;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupName="FrameHorizontal";
    for(int i=0; i<bset[1].size();i++)
    {
        NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount++;
        NBPointer->Beams.resize(NBPointer->Beams.size()+1);
        NBPointer->Beams[NBPointer->Beams.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node1GlobalID=bset[1][i].node1 + nodecounter;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node2GlobalID=bset[1][i].node2 + nodecounter;
    }

    NBPointer->BeamGroups.resize(NBPointer->BeamGroups.size()+1);
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].draw=1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount=0;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupName="FrameX";
    for(int i=0; i<bset[2].size();i++)
    {
        NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount++;
        NBPointer->Beams.resize(NBPointer->Beams.size()+1);
        NBPointer->Beams[NBPointer->Beams.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node1GlobalID=bset[2][i].node1 + nodecounter;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node2GlobalID=bset[2][i].node2 + nodecounter;
    }

    NBPointer->BeamGroups.resize(NBPointer->BeamGroups.size()+1);
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].draw=1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount=0;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
    NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamGroupName="FrameMiddle";
    for(int i=0; i<bset[3].size();i++)
    {
        NBPointer->BeamGroups[NBPointer->BeamGroups.size()-1].BeamAmount++;
        NBPointer->Beams.resize(NBPointer->Beams.size()+1);
        NBPointer->Beams[NBPointer->Beams.size()-1].BeamGroupID=NBPointer->BeamGroups.size()-1;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node1GlobalID=bset[3][i].node1 + nodecounter;
        NBPointer->Beams[NBPointer->Beams.size()-1].Node2GlobalID=bset[3][i].node2 + nodecounter;
    }

    for(int i10=0; i10<NBPointer->Beams.size(); i10++)
    {
        NBPointer->Beams[i10].Node1Name=NBPointer->Nodes[NBPointer->Beams[i10].Node1GlobalID].NodeName;
        NBPointer->Beams[i10].Node2Name=NBPointer->Nodes[NBPointer->Beams[i10].Node2GlobalID].NodeName;
    }

}

//void ChassisGenerator::write_file()
//{
//     //Export the NB to a fully RoR compatible truck file

//     //output filename
//     ofstream output_file ("Export/Chassis.truck");
//     qDebug() << "Exporting file" << endl;
//     if (output_file.is_open())
//     {
//        output_file << "Chassis Export";
//        output_file << "\n\n";
//        output_file << fixed << setprecision(6);

//        output_file << "globals\n";
//        output_file << ";dry mass, cargo mass, material\n";
//        output_file << "0.0,  0.0,     tracks/semi\n";


//        output_file << "\nengine\n";
//        output_file << ";min rpm,max rpm,torque, diff,R, N,1st,	2nd,3rd,4th,5th,\n";
//        output_file << "1000, 2200, 1568, 12.00, 5.7, 1.00, 5.7, 3.1, 2.0, 1.5, 1.05, 0.75, 0.55, 0.45, -1\n";


//        output_file << "\nengoption\n";
//        output_file << "0.45, t, 3025, 0.003, 0.001, 0.005\n";

//        output_file << "\ntorquecurve\n";
//        output_file << "0,      0.000000\n";
//        output_file << "600,    2.000000\n";
//        output_file << "700,    0.750000\n";
//        output_file << "1000,   0.950000\n";
//        output_file << "1400,   1.000000\n";
//        output_file << "2200,   1.000000\n";
//        output_file << "2350,   0.800000\n";
//        output_file << "3300,   0.100000\n";
//        output_file << "3500,   0.000000\n";

//        output_file << "\nminimass\n";
//        output_file << "5\n";

//        output_file << "\nbrakes\n";
//        output_file << "10000, 10000\n";

//        output_file << "\nnodes\n";

//        qDebug() << "[.";

//        for(i=0;i<NodeAmount;i++)
//        {
//                       output_file <<  set[0][i].node_number;
//                       output_file << ", ";
//                       output_file <<  set[0][i].locx;
//                       output_file << ", ";
//                       output_file <<  set[0][i].locy;
//                       output_file << ", ";
//                       output_file <<  set[0][i].locz;
//                       output_file << ", l 50\n";
//                       qDebug() << ".";
//        }
//        output_file << "\n\n\nbeams\n";

//        output_file << ";FrameSides\n";
//        output_file << "set_beam_defaults 39000000, 2000, 1000000, 8000000, 0.05, tracks/beam, 0.1\n";

//     for(i=0; i<BeamNumber[0]; i++)
//     {
//              output_file << bset[0][i].node1 << ", " << bset[0][i].node2 << ", v\n";
//              qDebug() << ".";
//     }
//        output_file << ";FrameHorizonal\n";
//        output_file << "set_beam_defaults 500000, 3500, 7000000, 8000000, 0.05, tracks/beam, 0.1\n";
//     for(i=0; i<BeamNumber[1]; i++)
//     {
//              output_file << bset[1][i].node1 << ", " << bset[1][i].node2 << ", v\n";
//              qDebug() << ".";
//     }

//        output_file << ";FrameX\n";
//        output_file << "set_beam_defaults 250000, 2500, 5000000, 8000000, 0.05, tracks/beam, 0.1\n";
//     for(i=0; i<BeamNumber[2]; i++)
//     {
//              output_file << bset[2][i].node1 << ", " << bset[2][i].node2 << ", v\n";
//              qDebug() << ".";
//     }

//        output_file << ";FrameMiddle\n";
//        output_file << "set_beam_defaults 250000, 2500, 5000000, 8000000, 0.05, tracks/beam, 0.1\n";
//     for(i=0; i<BeamNumber[3]; i++)
//     {
//              output_file << bset[3][i].node1 << ", " << bset[3][i].node2 << ", v\n";
//              qDebug() << ".";
//     }

//     output_file << "\ncameras\n";
//     output_file << ";center, back, left\n";
//     output_file << "1, 5, 0\n";

//     output_file << "\ncinecamn\n";
//     output_file << ";  x,   y,   z, <---------8 bindings--------->, spring, damping\n";
//     output_file << "0.66, 2.0, 1.8, 0, 1, 2, 3, 4, 5, 6, 7, 8000.0,   800.0\n\n\n";

//     output_file << "end";

//     output_file.close();
//     qDebug() << ".]" << endl;
//     qDebug() << "Complete." << endl;
//     }

//    else qDebug() << "Cannot create output file.";
//}


void ChassisGenerator::on_buttonBox_accepted()
{
    width = ui->lineEdit->text().toFloat();
    height = ui->lineEdit_2->text().toFloat();
    length = ui->lineEdit_3->text().toFloat();
    segments = ui->lineEdit_4->text().toFloat();
    generate_chassis();
    MoveToEditor();
}
