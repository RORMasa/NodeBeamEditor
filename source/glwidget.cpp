#include <QtWidgets>
#include <QtOpenGL>
#include <QMatrix4x4>

#include <math.h>

#include "glwidget.h"
#include "NodeBeam.h"


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

/* SHADERS */
//For 3D mesh
static const char *vertexShaderSource =
        "attribute vec4 vertex;\n"
        "attribute vec3 normal;\n"
        "varying vec3 vert;\n"
        "varying vec3 vertNormal;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "uniform mat3 normalMatrix;\n"
        "void main() {\n"
        "   vert = vertex.xyz;\n"
        "   vertNormal = normalMatrix * normal;\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "uniform float opacity;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.4, 0.4, 0.4);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, opacity);\n"
    "}\n";

GLWidget::GLWidget(QGLContext *context, QWidget *parent,
     const QGLWidget * shareWidget, Qt::WindowFlags f)
    : QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
{
    dRot = 4510;
    zRot = 3600;
    yRot = dRot*(zRot/5760);
    xRot = dRot*(1-(zRot/5760));
    NodePicking = 0;
    PickedNode1=9999;
    AddingBeamsSingle=0;
    AddingBeamsCont=0;
    AddingJbeam = -1;
    AddingWheels=0;
    CurrentBeamGroup=0;
    ZoomFactor = 1.0;
    ShowNodeNumbers = 0;
    ShowNodeNumbers1 = 0;
    BeamSuggestion = 0;
    PrintPickedNodeName = 0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    qtGrey = QColor::fromCmykF(0.25, 0.25, 0.25, 0.0);
    RotationMode = true;

    TextOverlay = "";

    setAutoFillBackground(false);
    setMouseTracking(true);

    ShowArrows = 1;
    DrawTris = 1;

    pii = 2*qAsin(1);

    backgroundcolor.resize(4);
    backgroundcolor[0] = 0.15;
    backgroundcolor[1] = 0.15;
    backgroundcolor[2] = 0.15;
    backgroundcolor[3] = 0.15;
    gridcolor.resize(4);
    gridcolor[3] = 1.0;
    MeshOpacity = 0.8;

    //View offset in begin
    ViewOffsetX = 0;
    ViewOffsetY = 0;
    ViewOffsetZ = 0;

    //3D Edit system
    Moving3D_ModeX = 0;
    Moving3D_ModeY = 0;
    Moving3D_ModeZ = 0;
    Rotating3D_ModeX = 0;
    Rotating3D_ModeY = 0;
    Rotating3D_ModeZ = 0;
    DegreeToRadiansRatio = (2*pii)/360; //One degree is this many radians
    RectSelect = 0;
    Select_AddToSelection=0;

    MovingNodes=0;
    ScalingNodes=0;
    RotatingNodes=0;

    TrianglePicking=0;


}

GLWidget::~GLWidget()
{
    VAO.release();
    VAO.destroy();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600, 600);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setDRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != dRot)
    {
        dRot = angle;

        //yRot = dRot*(zRot/5760);
        xRot = dRot*(1-(zRot/5760));
        //qDebug() << "kamera rotaatio " << xRot << ", " << yRot;
        updateGL();
    }
}

void GLWidget::draw()
{
    const GLfloat first_pos[4] = {0.0f, 0.0f, 50.0f, 0.0f};
    const GLfloat first_dir[4] = {0.0f, 0.0f, -1.0f, 0.0f};
    const GLfloat second_pos[4] = {0.0f, 0.0f, -50.0f, 0.0f};
    const GLfloat second_dir[4] = {0.0f, 0.0f, 1.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, first_pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, first_dir);
    glLightfv(GL_LIGHT1, GL_POSITION, second_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, second_dir);

    double x_min = -10 - 0.5;
    double x_max = 10 + 0.5;
    double y_min = -10 - 0.5;
    double y_max = 10 + 0.5;

    glLineWidth(1);
    glColor4f(gridcolor[0],gridcolor[1],gridcolor[2],gridcolor[3]);
  //  qDebug() << gridcolor[0] << ", " << gridcolor[1] << ", " << gridcolor[2];
    glBegin(GL_LINES);

    for (int i = -10; i <= 10; i++) {
        if (i != 0) {
            glVertex3d(x_min, (double)i, 0.0);
            glVertex3d(x_max, (double)i, 0.0);
        }
    }
    for (int i = -10; i <= 10; i++) {
        if (i != 0) {
            glVertex3d((double)i, y_min, 0.0);
            glVertex3d((double)i, y_max, 0.0);
        }
    }

    glEnd();

    glColor3f(0,1,0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3d(0, y_min, 0);
    glVertex3d(0, y_max, 0);
    glColor3f(1,0,0);
    glVertex3d(x_min, 0, 0);
    glVertex3d(x_max, 0, 0);
    glColor3f(0,0,1);
    glVertex3d(0, 0, x_min);
    glVertex3d(0, 0, x_max);
    glEnd();

    /* Drawing nodes */
    glPointSize(10);
    glBegin(GL_POINTS);
    int i3 = 0;
    for(int i2=0; i2<NBPointer->NodeGroups.size();i2++)
    {
        if(NBPointer->NodeGroups.at(i2).draw)
        {
            for(int i=0; i<NBPointer->NodeGroups.at(i2).NodeAmount; i++)
            {

                    if(NBPointer->Nodes.at(i3).GlobalID == NBPointer->ActiveNode) glColor4f(1.0f,0.0f,0.0f,1.0f);
                    else if(NBPointer->Nodes.at(i3).GlobalID == HighlightNode) glColor4f(1.0f,1.0f,0.0f,1.0f);
                    else glColor4f(0.4f,0.4f,0.4f,1.0f);

                    glVertex3f(NBPointer->Nodes.at(i3).locX, NBPointer->Nodes.at(i3).locY, NBPointer->Nodes.at(i3).locZ);

                i3++;
            }
        }
    }
    glColor4f(1.0f,1.0f,0.0f,1.0f);
    glEnd();
    glPointSize(15);
    glBegin(GL_POINTS);
    for(int i2=0; i2<NBPointer->SelectedNodes.size();i2++)
    {
        i3 = NBPointer->SelectedNodes.at(i2);
        glVertex3f(NBPointer->Nodes.at(i3).locX, NBPointer->Nodes.at(i3).locY, NBPointer->Nodes.at(i3).locZ);
    }
    glColor4f(0.4f,0.4f,0.4f,1.0f);
    glEnd();

    for(int i2=0; i2<NBPointer->ListTypes.size();i2++)
    {
        //Draw triangles
        if((NBPointer->ListTypes.at(i2).drawtype == 3) && DrawTris)
        {
            glEnable(GL_DEPTH_TEST);
            glBegin(GL_TRIANGLES);
            glColor3f(0.0, 1.0, 0.0);

            for(int i=0; i<NBPointer->ListTypes.at(i2).contaier.size();i++)
            {
                int nodeid = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(0));
                int nodeid1 = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(1));
                int nodeid2 = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(2));
                glVertex3f(NBPointer->Nodes.at(nodeid).locX,NBPointer->Nodes.at(nodeid).locY,NBPointer->Nodes.at(nodeid).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid1).locX,NBPointer->Nodes.at(nodeid1).locY,NBPointer->Nodes.at(nodeid1).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid2).locX,NBPointer->Nodes.at(nodeid2).locY,NBPointer->Nodes.at(nodeid2).locZ);
            }

            glColor3f(1.0, 0.0, 0.0);

            for(int i=0; i<NBPointer->ListTypes.at(i2).contaier.size();i++)
            {
                int nodeid = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(0));
                int nodeid1 = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(1));
                int nodeid2 = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(2));
                glVertex3f(NBPointer->Nodes.at(nodeid).locX,NBPointer->Nodes.at(nodeid).locY,NBPointer->Nodes.at(nodeid).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid2).locX,NBPointer->Nodes.at(nodeid2).locY,NBPointer->Nodes.at(nodeid2).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid1).locX,NBPointer->Nodes.at(nodeid1).locY,NBPointer->Nodes.at(nodeid1).locZ);
            }

            glEnd();
        }
        else if(NBPointer->ListTypes.at(i2).drawtype == 1)
        {
            glLineWidth(2);
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            for(int i=0; i<NBPointer->ListTypes.at(i2).contaier.size();i++)
            {
                int nodeid = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(0));
                int nodeid1 = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(1));
                glVertex3f(NBPointer->Nodes.at(nodeid).locX,NBPointer->Nodes.at(nodeid).locY,NBPointer->Nodes.at(nodeid).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid1).locX,NBPointer->Nodes.at(nodeid1).locY,NBPointer->Nodes.at(nodeid1).locZ);
            }
            glEnd();
        }
    }


    /* Drawing beams */
    glColor4f(0.0f,0.4f,0.6f,1.0f);
    glLineWidth(2);
    glBegin(GL_LINES);
    i3=0;
    int NodeAmount = NBPointer->Nodes.size();

    for(int i2=0;i2<NBPointer->BeamGroups.size();i2++)
    {

        for(int i=0; i<NBPointer->BeamGroups[i2].BeamAmount; i++)
        {

            if(NBPointer->BeamGroups[i2].draw)
            {
                if(NBPointer->Beams[i3].draw)
                {
                    if(i3 == NBPointer->ActiveBeam)
                    {
                        glEnd();
                        glColor4f(0.6f,0.4f,0.0f,1.0f);
                        glLineWidth(4);
                        glBegin(GL_LINES);


                        int Bnode1 = NBPointer->Beams[i3].Node1GlobalID;
                        int Bnode2 = NBPointer->Beams[i3].Node2GlobalID;
                        if((Bnode1<NodeAmount) && (Bnode2<NodeAmount))
                        {
                            //qDebug()<<"Rendering beams 7 .... " << Bnode1 << ", " << Bnode2;
                            glVertex3f(NBPointer->Nodes[Bnode1].locX, NBPointer->Nodes[Bnode1].locY, NBPointer->Nodes[Bnode1].locZ);
                            glVertex3f(NBPointer->Nodes[Bnode2].locX, NBPointer->Nodes[Bnode2].locY, NBPointer->Nodes[Bnode2].locZ);
                            //qDebug()<<"Rendering beams 8 .... ";
                        }
                        glEnd();
                        glLineWidth(2);
                        glBegin(GL_LINES);

                    }
                    else
                    {
                        if(NBPointer->Beams[i3].HasBeamDefs)
                        {
                            float beamred = NBPointer->BeamDefaults[NBPointer->Beams[i3].BeamDefsID].RGB_Color[0]/255.0f;
                            float beamgre = NBPointer->BeamDefaults[NBPointer->Beams[i3].BeamDefsID].RGB_Color[1]/255.0f;
                            float beamblu = NBPointer->BeamDefaults[NBPointer->Beams[i3].BeamDefsID].RGB_Color[2]/255.0f;
                            glColor4f(beamred,beamgre,beamblu,1.0f);
                        }
                        else
                        {
                            glColor4f(0.0f,0.4f,0.6f,1.0f);
                        }


                        int Bnode1 = NBPointer->Beams[i3].Node1GlobalID;
                        int Bnode2 = NBPointer->Beams[i3].Node2GlobalID;
                        if((Bnode1<NodeAmount) && (Bnode2<NodeAmount))
                        {
                            //qDebug()<<"Rendering beams 7 .... " << Bnode1 << ", " << Bnode2;
                            glVertex3f(NBPointer->Nodes[Bnode1].locX, NBPointer->Nodes[Bnode1].locY, NBPointer->Nodes[Bnode1].locZ);
                            glVertex3f(NBPointer->Nodes[Bnode2].locX, NBPointer->Nodes[Bnode2].locY, NBPointer->Nodes[Bnode2].locZ);
                            //qDebug()<<"Rendering beams 8 .... ";
                        }


                    }
                }
            }

            i3++;
        }


    }
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINES);
    //Suggesting beam to be placed, when node detected
    if((AddingBeamsSingle==2) || (AddingBeamsCont==2) || (BeamSuggestion))
    {
        if((BeamNode1 != PickedNode1) && (PickedNode1!=9999))
        {
            glColor4f(1.0f,0.0f,0.0f,1.0f);
            int Bnode1 = BeamNode1;
            int Bnode2 = PickedNode1;
            glVertex3f(NBPointer->Nodes[Bnode1].locX, NBPointer->Nodes[Bnode1].locY, NBPointer->Nodes[Bnode1].locZ);
            glVertex3f(NBPointer->Nodes[Bnode2].locX, NBPointer->Nodes[Bnode2].locY, NBPointer->Nodes[Bnode2].locZ);
        }
    }
    glEnd();
    glLineWidth(2);

    /* Drawing hubwheels */
    for(int i=0; i<NBPointer->Hubwheels.size(); i++)
    {
        NBPointer->CalcHubWheelRotation(i);
        glPushMatrix();
        glTranslatef(NBPointer->Nodes[NBPointer->Hubwheels[i].node1id].locX,NBPointer->Nodes[NBPointer->Hubwheels[i].node1id].locY,NBPointer->Nodes[NBPointer->Hubwheels[i].node1id].locZ);
        glPushMatrix();
        glRotatef(NBPointer->Hubwheels[i].RotationX, 1, 0, 0);
        glRotatef(NBPointer->Hubwheels[i].RotationY, 0, 1, 0);
        glRotatef(NBPointer->Hubwheels[i].RotationZ, 0, 0, 1);
        DrawWheel(NBPointer->Hubwheels[i].radius, NBPointer->Hubwheels[i].width, 16);
        glPopMatrix();
        glPopMatrix();

    }

}

void GLWidget::drawpicking()
{
    //Color picking system, every node is rendered in the buffer
    //with unique color. The buffer is not drawn on widget.
    //When mouse is clicked, the color is checked.
    //Only green and blue colors are used, allows max node amount of 65025.

    //glDisable(GL_DITHER);
    //Nodes
    // Turn off lighting
    glDisable(GL_LIGHTING);

    // Turn off antialiasing
    glDisable (GL_BLEND);
    glDisable(GL_MULTISAMPLE);

    // Turn off texturing
    glDisable(GL_TEXTURE_2D);


    PickedNode1=9999;
    glPointSize(20);
    glBegin(GL_POINTS);

    int i3 = 0;
    for(int i2=0; i2<NBPointer->NodeGroups.size();i2++)
    {
        for(int i=0; i<NBPointer->NodeGroups[i2].NodeAmount; i++)
        {
            if(NBPointer->NodeGroups[i2].draw)
            {
                glColor3f(0,NBPointer->Nodes[i3].green/255.0f,NBPointer->Nodes[i3].blue/255.0f);

                glVertex3f(NBPointer->Nodes[i3].locX, NBPointer->Nodes[i3].locY, NBPointer->Nodes[i3].locZ);
            }
            i3++;
        }
    }
    glEnd();

    GLubyte pixel[3];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);

    //glReadPixels(cursorX,viewport[3]-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
    glReadPixels(lastPos.x(), viewport[3]-lastPos.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) pixel);
    qDebug() << pixel[0] << ", " << pixel[1] << "," << pixel[2];

    for(int i = 0; i < NBPointer->Nodes.size(); i++)
    {
        if(NBPointer->Nodes[i].blue/255.0f == pixel[2]/255.0f)
        {
            if(NBPointer->Nodes[i].green/255.0f == pixel[1]/255.0f)
            {
                PickedNode1 = NBPointer->Nodes[i].GlobalID;
                qDebug() << "Found node number: " << PickedNode1;
                HighlightNode = PickedNode1;
                break;
            }
        }
    }

    glEnable(GL_LIGHTING);

    // Turn on antialiasing
    glEnable (GL_BLEND);
    glEnable(GL_MULTISAMPLE);

    // Turn on texturing
    glEnable(GL_TEXTURE_2D);

}

void GLWidget::drawpicking_triangle()
{
    glDisable(GL_LIGHTING);
    glDisable (GL_BLEND);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_TEXTURE_2D);

    for(int i2=0; i2<NBPointer->ListTypes.size();i2++)
    {
        //Draw triangles
        if((NBPointer->ListTypes.at(i2).drawtype == 3) && DrawTris)
        {
            glEnable(GL_DEPTH_TEST);
            glBegin(GL_TRIANGLES);

            float conv = 1.0f/255.0f;

            for(int i=0; i<NBPointer->ListTypes.at(i2).contaier.size();++i)
            {
                //Draw each triangle with unique color
                int red = (i & 0x0000ff);
                int green = (i & 0x00ff00) >> 8;
                //qDebug() << "varit ovat " << red << ", " << green;
                glColor3f(conv*red, conv*green, conv*i2);

                int nodeid = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(0));
                int nodeid1 = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(1));
                int nodeid2 = NBPointer->ListTypes.at(i2).contaier.at(i).at(NBPointer->ListTypes.at(i2).draworder.at(2));

                glVertex3f(NBPointer->Nodes.at(nodeid).locX,NBPointer->Nodes.at(nodeid).locY,NBPointer->Nodes.at(nodeid).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid1).locX,NBPointer->Nodes.at(nodeid1).locY,NBPointer->Nodes.at(nodeid1).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid2).locX,NBPointer->Nodes.at(nodeid2).locY,NBPointer->Nodes.at(nodeid2).locZ);

                glVertex3f(NBPointer->Nodes.at(nodeid).locX,NBPointer->Nodes.at(nodeid).locY,NBPointer->Nodes.at(nodeid).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid2).locX,NBPointer->Nodes.at(nodeid2).locY,NBPointer->Nodes.at(nodeid2).locZ);
                glVertex3f(NBPointer->Nodes.at(nodeid1).locX,NBPointer->Nodes.at(nodeid1).locY,NBPointer->Nodes.at(nodeid1).locZ);
            }
            glEnd();
        }
    }


    GLubyte pixel[3];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);

    //glReadPixels(cursorX,viewport[3]-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
    glReadPixels(lastPos.x(), viewport[3]-lastPos.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) pixel);
    //qDebug() << pixel[0] << ", " << pixel[1] << "," << pixel[2];

    if(pixel[2]<NBPointer->ListTypes.size())
    {
        int green = pixel[1];
        green = green << 8;
        int color = green + pixel[0];

        if(color<NBPointer->ListTypes.at(pixel[2]).contaier.size())
        {
            qDebug() << "Selection is " << color;
        }

    }

    glEnable(GL_LIGHTING);
    glEnable (GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);

}

/* Rendering node names next of nodes */
void GLWidget::RenderTextInScene(bool names)
{

    if(names)
    {
        glColor4f(1.0f,1.0f,0.0f,1.0f);
        int i3 = 0;
        for(int i2=0; i2<NBPointer->NodeGroups.size();i2++)
        {
            for(int i=0; i<NBPointer->NodeGroups[i2].NodeAmount; i++)
            {
                if(NBPointer->NodeGroups[i2].draw)
                {
                    QPainter painter(this);
                    //painter.setRenderHint(QPainter::Antialiasing);
                    if(NBPointer->Nodes[i3].GlobalID == NBPointer->ActiveNode);
                    else;

                    renderText(NBPointer->Nodes[i3].locX, NBPointer->Nodes[i3].locY+0.02f, NBPointer->Nodes[i3].locZ, NBPointer->Nodes[i3].NodeName, QFont( "times", 10, QFont::Normal, 0 ));
                    painter.end();
                }
                i3++;
            }
        }
    }
    else
    {
        glColor4f(1.0f,1.0f,0.0f,1.0f);
        int i3 = 0;
        for(int i2=0; i2<NBPointer->NodeGroups.size();i2++)
        {
            for(int i=0; i<NBPointer->NodeGroups[i2].NodeAmount; i++)
            {
                if(NBPointer->NodeGroups[i2].draw)
                {
                    QPainter painter(this);
                    //painter.setRenderHint(QPainter::Antialiasing);
                    if(NBPointer->Nodes[i3].GlobalID == NBPointer->ActiveNode);
                    else;

                    renderText(NBPointer->Nodes[i3].locX, NBPointer->Nodes[i3].locY+0.02f, NBPointer->Nodes[i3].locZ, QString::number(NBPointer->Nodes[i3].GlobalID), QFont( "times", 10, QFont::Normal, 0 ));
                    painter.end();
                }
                i3++;
            }
        }
    }
}

/* Draw arrows for X, Y and Z axis */
void GLWidget::DrawAxisArrows()
{

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    //X-Arrow
    glVertex3f(0.9, -0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, 0.05, 0.05);

    glVertex3f(0.9, 0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, -0.05, -0.05);

    glVertex3f(0.9, -0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, -0.05, 0.05);

    glVertex3f(0.9, 0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, 0.05, -0.05);

    glVertex3f(0.9, 0.05, 0.05);
    glVertex3f(0.9, 0.05, -0.05);
    glVertex3f(0.9, -0.05, 0.05);
    glVertex3f(0.9, -0.05, 0.05);
    glVertex3f(0.9, 0.05, -0.05);
    glVertex3f(0.9, -0.05, -0.05);
    glEnd();

    //Tall arrow
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.1, -0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, 0.05, 0.05);

    glVertex3f(0.1, 0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, -0.05, -0.05);

    glVertex3f(0.1, -0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, -0.05, 0.05);

    glVertex3f(0.1, 0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, 0.05, -0.05);

    glVertex3f(0.1, 0.05, 0.05);
    glVertex3f(0.1, 0.05, -0.05);
    glVertex3f(0.1, -0.05, 0.05);
    glVertex3f(0.1, -0.05, 0.05);
    glVertex3f(0.1, 0.05, -0.05);
    glVertex3f(0.1, -0.05, -0.05);

    glEnd();
    //Y-Arrow

    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);

    glVertex3f(0.05, 0.9, 0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.9, 0.05);

    glVertex3f(-0.05, 0.9, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05,0.9,  -0.05);

    glVertex3f(-0.05,0.9,  0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.9, -0.05);

    glVertex3f(0.05, 0.9, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05, 0.9, 0.05);

    glVertex3f(-0.05, 0.9, 0.05);
    glVertex3f(0.05, 0.9, -0.05);
    glVertex3f(0.05, 0.9, 0.05);
    glVertex3f(-0.05, 0.9, -0.05);
    glVertex3f(0.05, 0.9, -0.05);
    glVertex3f(-0.05, 0.9, 0.05);

    glEnd();

    //Tall arrow
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);

    glVertex3f(0.05, 0.1, 0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.1, 0.05);

    glVertex3f(-0.05, 0.1, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05,0.1,  -0.05);

    glVertex3f(-0.05,0.1,  0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.1, -0.05);

    glVertex3f(0.05, 0.1, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05, 0.1, 0.05);

    glVertex3f(-0.05, 0.1, 0.05);
    glVertex3f(0.05, 0.1, -0.05);
    glVertex3f(0.05, 0.1, 0.05);
    glVertex3f(-0.05, 0.1, -0.05);
    glVertex3f(0.05, 0.1, -0.05);
    glVertex3f(-0.05, 0.1, 0.05);

    glEnd();

    //Z Arrow
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0);

    glVertex3f(-0.05, 0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, 0.05, 0.9);

    glVertex3f(0.05, -0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, -0.05, 0.9);

    glVertex3f(-0.05, -0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, 0.05, 0.9);

    glVertex3f(0.05, 0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, -0.05, 0.9);

    glVertex3f(0.05, -0.05, 0.9);
    glVertex3f(-0.05, 0.05, 0.9);
    glVertex3f(0.05, 0.05, 0.9);
    glVertex3f(-0.05, -0.05, 0.9);
    glVertex3f(-0.05, 0.05, 0.9);
    glVertex3f(0.05, -0.05, 0.9);

    glEnd();

    //Tall arrow
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0);

    glVertex3f(-0.05, 0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, 0.05, 0.1);

    glVertex3f(0.05, -0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, -0.05, 0.1);

    glVertex3f(-0.05, -0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, 0.05, 0.1);

    glVertex3f(0.05, 0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, -0.05, 0.1);

    glVertex3f(0.05, -0.05, 0.1);
    glVertex3f(-0.05, 0.05, 0.1);
    glVertex3f(0.05, 0.05, 0.1);
    glVertex3f(-0.05, -0.05, 0.1);
    glVertex3f(-0.05, 0.05, 0.1);
    glVertex3f(0.05, -0.05, 0.1);
    glEnd();

}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    ShaderProgram = new QOpenGLShaderProgram;
    ShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    ShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    ShaderProgram->bindAttributeLocation("vertex",0);
    ShaderProgram->bindAttributeLocation("normal",1);
    ShaderProgram->link();

    ShaderProgram->bind();
    m_projectionmatrixloc = ShaderProgram->uniformLocation("projMatrix");
    m_normalMatrixLoc = ShaderProgram->uniformLocation("normalMatrix");
    m_lightPosLoc = ShaderProgram->uniformLocation("lightPos");
    m_mvmatrixloc = ShaderProgram->uniformLocation("mvMatrix");
    m_opacityloc = ShaderProgram->uniformLocation("opacity"); //For opacity adjustment of the model

    // Light position is fixed.
    ShaderProgram->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 50));

    ShaderProgram->release();

    verts_size = 0;

    glClearColor(backgroundcolor[0], backgroundcolor[1], backgroundcolor[2], backgroundcolor[3]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_COLOR_MATERIAL);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    setAutoBufferSwap(false); //Must be false for color picking, to not render colors on screen
}

void GLWidget::paintGL()
{  
    /* Set background color */
    glClearColor(backgroundcolor.at(0), backgroundcolor.at(1), backgroundcolor.at(2), backgroundcolor.at(3));

    /* Render picking colors in buffer and check what should be picked */
    if(NodePicking || TrianglePicking || (MovingNodes > 0) || (ScalingNodes > 0) || (RotatingNodes > 0))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.0);
        glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
        glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
        //glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
        glTranslatef(ViewOffsetX, ViewOffsetY, ViewOffsetZ); //Move 3D view around
        if(NodePicking) drawpicking(); //Draw nodes in buffer, each with individual color
        else if(TrianglePicking) drawpicking_triangle();
        else if(MovingNodes >0) Draw3DCursor_Picking(0); //Draw move 3D cursor in buffer and check
        else if(ScalingNodes >0) Draw3DCursor_Picking(1); //Draw scale 3D cursor in buffer and check
        else if(RotatingNodes > 0) Draw3DCursor_Picking(2); //Draw rotate 3D cursor in buffer and check
        //QGLWidget::swapBuffers(); // Swap buffes to render on screen, only for testing
    }

    /* Render the actual scene itself */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0); //Rotate 3D view
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    //glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glTranslatef(ViewOffsetX, ViewOffsetY, ViewOffsetZ); //Move 3D view around
    glEnable(GL_MULTISAMPLE);
    draw(); //draw nodes, beams, wheels, lines

    //Adjust the world matrix for 3D reference mesh
    m_worldmatrix.setToIdentity();
    m_worldmatrix.translate(0,0,-10);
    m_worldmatrix.rotate(xRot / 16.0f, 1, 0, 0);
    m_worldmatrix.rotate(zRot / 16.0f, 0, 0, 1);
    m_worldmatrix.translate(ViewOffsetX, ViewOffsetY, ViewOffsetZ);

    //Bind shaderprogram and vertexarray object, draw the model
    ShaderProgram->bind();
    ShaderProgram->setUniformValue(m_projectionmatrixloc, m_projectionmatrix);
    ShaderProgram->setUniformValue(m_mvmatrixloc, m_camera * m_worldmatrix);
    ShaderProgram->setUniformValue(m_opacityloc, MeshOpacity);
    QMatrix3x3 normalMatrix = m_worldmatrix.normalMatrix();
    ShaderProgram->setUniformValue(m_normalMatrixLoc, normalMatrix);

    VAO.bind();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, verts_size); //draw reference mesh
    VAO.release();

    ShaderProgram->release();

    glDisable(GL_DEPTH_TEST);

    //Draw node names, numbers, cursors
    if(MovingNodes > 0) Draw3DCursor();
    else if(ScalingNodes > 0) Draw3DCursor_Scale();
    else if(RotatingNodes > 0) Draw3DCursor_Rotate();
    else if(ShowArrows); //DrawAxisArrows();
    glColor3f(0.6, 0.6, 0.6);  //Set text color
    renderText(10, yHeight-20, TextOverlay, QFont( "Arial", 14, QFont::Bold, 0 ) );

    if(ShowNodeNumbers) RenderTextInScene(1);
    else if(ShowNodeNumbers1) RenderTextInScene(0);
    if(RectSelect > 1) DrawRectSelect();

    QGLWidget::swapBuffers();

}

void GLWidget::resizeGL(int width, int height)
{
    xWidth = width;
    yHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat(width) / height)*ZoomFactor;
    glFrustum(-x, x, -ZoomFactor, ZoomFactor, 4.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    m_projectionmatrix.setToIdentity();
    m_projectionmatrix.frustum(-x, x, -ZoomFactor, ZoomFactor, 4.0, 1000.0);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    updateGL();
    if (event->buttons() & Qt::LeftButton) {


        if(PickedNode1!=9999)
        {
            //qDebug() << "JBEAM_Adding: " << AddingJbeam;
            //qDebug() << "JBEAM_count: " << NBPointer->ListTypes.at(AddingJbeam_id).nodeamount;
            if(AddingBeamsSingle==1)
            {
                BeamNode1=PickedNode1;
                AddingBeamsSingle++;
                PickedNode1=9999;

            }
            else if(AddingBeamsSingle==2)
            {
                if(BeamNode1 != PickedNode1)
                {
                    BeamNode2=PickedNode1;
                    PickedNode1=9999;

                    NBPointer->AddBeam(BeamNode1, BeamNode2, CurrentBeamGroup,1);

                    TextOverlay = "Beam added between nodes ";
                    TextOverlay.append(NBPointer->Nodes[BeamNode1].NodeName);
                    TextOverlay.append(" and ");
                    TextOverlay.append(NBPointer->Nodes[BeamNode2].NodeName);
                    AddingBeamsSingle=1;
                    updateGL();
                    PickedNode1=9999;
                    qDebug() << "Beam added";
                    emit NodeBeamUpdated();
                    emit JBEAM_AddBeamO();
                }
                else
                {
                    PickedNode1=9999;
                }

            }
            if(AddingBeamsCont==1)
            {
                BeamNode1=PickedNode1;
                AddingBeamsCont++;
                PickedNode1=9999;
            }
            else if(AddingBeamsCont==2)
            {
                if(BeamNode1 != PickedNode1)
                {
                    BeamNode2=PickedNode1;
                    PickedNode1=9999;

                    NBPointer->AddBeam(BeamNode1, BeamNode2, CurrentBeamGroup,1);

                    TextOverlay = "Beam added between nodes ";
                    TextOverlay.append(NBPointer->Nodes[BeamNode1].NodeName);
                    TextOverlay.append(" and ");
                    TextOverlay.append(NBPointer->Nodes[BeamNode2].NodeName);

                    updateGL();
                    PickedNode1=9999;
                    BeamNode1=BeamNode2;
                    qDebug() << "Beam added";
                    emit NodeBeamUpdated();
                    emit JBEAM_AddBeamO();
                }
                else
                {
                    PickedNode1=9999;
                }
            }
            else if(AddingWheels==1)
            {
                NBPointer->TempHubwheel.node1id = PickedNode1;
                AddingWheels++;
                PickedNode1=9999;
                qDebug() << "wheel node 1";
                TextOverlay = "Choose 2nd wheelnode";
            }
            else if(AddingWheels==2)
            {
                NBPointer->TempHubwheel.node2id = PickedNode1;
                AddingWheels++;
                PickedNode1=9999;
                qDebug() << "wheel node 2";
                TextOverlay = "Choose arm node";
            }
            else if(AddingWheels==3)
            {
                NBPointer->TempHubwheel.nodeArmid = PickedNode1;
                AddingWheels++;
                PickedNode1=9999;
                qDebug() << "wheel node 3";
                TextOverlay = "Choose S node";
            }
            else if(AddingWheels==4)
            {
                NBPointer->TempHubwheel.nodeSid = PickedNode1;
                AddingWheels=1;
                PickedNode1=9999;
                NBPointer->AddHubWheel();
                emit NodeBeamUpdated();
                qDebug() << "wheel node 4";

            }
            else if(AddingJbeam>=0)
            {
                if(AddingJbeam == 0)
                {
                    AddingJbeam_pickednoes.clear();
                    AddingJbeam_pickednoes.append(PickedNode1);
                    BeamNode1=PickedNode1;

                    if(NBPointer->ListTypes.at(AddingJbeam_id).drawtype == NBPointer->ListTypes.at(AddingJbeam_id).BEAM)
                    {
                        BeamSuggestion = 1;
                    }

                    PickedNode1=9999;
                    TextOverlay = NBPointer->ListTypes.at(AddingJbeam_id).nodenames.at(1);
                    AddingJbeam++;
                }
                else if(AddingJbeam < (NBPointer->ListTypes.at(AddingJbeam_id).nodeamount-1))
                {
                    AddingJbeam_pickednoes.append(PickedNode1);
                    TextOverlay = NBPointer->ListTypes.at(AddingJbeam_id).nodenames.at(AddingJbeam+1);
                    PickedNode1=9999;
                    BeamSuggestion = 0;
                    AddingJbeam++;
                }
                else if(AddingJbeam == (NBPointer->ListTypes.at(AddingJbeam_id).nodeamount-1))
                {
                    AddingJbeam_pickednoes.append(PickedNode1);
                    QString test = "[";
                    test+= NBPointer->ListTypes.at(AddingJbeam_id).JBEAM_template;
                    for(int i4=0; i4<NBPointer->ListTypes.at(AddingJbeam_id).nodeamount;i4++)
                    {
                        test = test.arg(NBPointer->Nodes.at(AddingJbeam_pickednoes.at(i4)).NodeName);
                    }
                    test+="]";
                    qDebug() << test;

                    NBPointer->ListTypes[AddingJbeam_id].Add(AddingJbeam_pickednoes);
                    emit JBEAM_AddArrayItem(AddingJbeam_id);

                    AddingJbeam = 0;
                    PickedNode1=9999;
                    TextOverlay = NBPointer->ListTypes.at(AddingJbeam_id).nodenames.at(0);
                    AddingJbeam_pickednoes.clear();
                    BeamSuggestion = 0;
                    //emit JBEAM_AddItem;

                }
            }
            else if(PrintPickedNodeName)
            {
                emit PrintNodePicked(PickedNode1);
                PickedNode1=9999;
            }
        }
        /* Check if user has dragged the move arrows */
        //Lock the moving until mouse is released.
        if(MovingNodes>0)
        {
            if(Moving3D_Mode == 1)
            {
                Moving3D_ModeX = 1;
            }
            else if(Moving3D_Mode == 2)
            {
                Moving3D_ModeY = 1;
            }
            else if(Moving3D_Mode == 3)
            {
                Moving3D_ModeZ = 1;
            }
        }
        else if(ScalingNodes>0)
        {
            if(Scaling3D_Mode == 1)
            {
                Scaling3D_ModeX = 1;
            }
            else if(Scaling3D_Mode == 2)
            {
                Scaling3D_ModeY = 1;
            }
            else if(Scaling3D_Mode == 3)
            {
                Scaling3D_ModeZ = 1;
            }
            /* Set scaling start point */
            ScaleStartScreen.setX(event->x());
            ScaleStartScreen.setY(event->y());

            /* Calculate scaling center point on screen */
            /* Get modelview matrix */
            GLfloat matriisi[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, matriisi);

            QVector4D row1(matriisi[0],matriisi[1],matriisi[2],matriisi[3]);
            QVector4D row2(matriisi[4],matriisi[5],matriisi[6],matriisi[7]);
            QVector4D row3(matriisi[8],matriisi[9],matriisi[10],matriisi[11]);
            QVector4D row4(matriisi[12],matriisi[13],matriisi[14],matriisi[15]);

            QMatrix4x4 modelview;
            modelview.setRow(0,row1);
            modelview.setRow(1,row2);
            modelview.setRow(2,row3);
            modelview.setRow(3,row4);

            /* Get projection matrix */
            glGetFloatv(GL_PROJECTION_MATRIX, matriisi);

            QVector4D row5(matriisi[0],matriisi[1],matriisi[2],matriisi[3]);
            QVector4D row6(matriisi[4],matriisi[5],matriisi[6],matriisi[7]);
            QVector4D row7(matriisi[8],matriisi[9],matriisi[10],matriisi[11]);
            QVector4D row8(matriisi[12],matriisi[13],matriisi[14],matriisi[15]);

            QMatrix4x4 projection;
            projection.setRow(0,row5);
            projection.setRow(1,row6);
            projection.setRow(2,row7);
            projection.setRow(3,row8);

            /* Get selection center position */
            QVector4D SelectionCenter = NBPointer->SelectionCenterPos.toVector4D();
            SelectionCenter.setW(1.0f); // set W to 1 for a point

            /* Transform center position from world coordinates to screen coordinates */
            QVector4D ScalingCenter = projection.transposed()*modelview.transposed()*SelectionCenter;

            //Only X and Y are needed
            ScalingCenterScreen.setX(((ScalingCenter.x()/ScalingCenter.w()+1)/2)*xWidth);
            ScalingCenterScreen.setY(((-ScalingCenter.y()/ScalingCenter.w()+1)/2)*yHeight);

            qDebug() << "Scaling middlepoint in screen coordinates is " << ScalingCenterScreen;

            /* Take a copy of original node coordinates of selected nodes */
            NBPointer->TempNodes.clear();
            for(int i=0; i<NBPointer->SelectedNodes.size();i++)
            {
                NBPointer->TempNodes.append(NBPointer->Nodes.at(NBPointer->SelectedNodes.at(i)));
                NBPointer->TempNodes[i].locX = NBPointer->TempNodes[i].locX - NBPointer->SelectionCenterPos.x();
                NBPointer->TempNodes[i].locY = NBPointer->TempNodes[i].locY - NBPointer->SelectionCenterPos.y();
                NBPointer->TempNodes[i].locZ = NBPointer->TempNodes[i].locZ - NBPointer->SelectionCenterPos.z();
            }
        }
        //Lock the rotating until mouse is released
        else if(RotatingNodes>0)
        {
            if(Rotating3D_Mode == 1)
            {
                Rotating3D_ModeX = 1;
            }
            else if(Rotating3D_Mode == 2)
            {
                Rotating3D_ModeY = 1;
            }
            else if(Rotating3D_Mode == 3)
            {
                Rotating3D_ModeZ = 1;
            }
            /* Set rotation start point */
            RotationStartScreen.setX(event->x());
            RotationStartScreen.setY(event->y());

            /* Calculate rotation center point on screen */
            /* Get modelview matrix */
            GLfloat matriisi[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, matriisi);

            QVector4D row1(matriisi[0],matriisi[1],matriisi[2],matriisi[3]);
            QVector4D row2(matriisi[4],matriisi[5],matriisi[6],matriisi[7]);
            QVector4D row3(matriisi[8],matriisi[9],matriisi[10],matriisi[11]);
            QVector4D row4(matriisi[12],matriisi[13],matriisi[14],matriisi[15]);

            QMatrix4x4 modelview;
            modelview.setRow(0,row1);
            modelview.setRow(1,row2);
            modelview.setRow(2,row3);
            modelview.setRow(3,row4);

            /* Get projection matrix */
            glGetFloatv(GL_PROJECTION_MATRIX, matriisi);

            QVector4D row5(matriisi[0],matriisi[1],matriisi[2],matriisi[3]);
            QVector4D row6(matriisi[4],matriisi[5],matriisi[6],matriisi[7]);
            QVector4D row7(matriisi[8],matriisi[9],matriisi[10],matriisi[11]);
            QVector4D row8(matriisi[12],matriisi[13],matriisi[14],matriisi[15]);

            QMatrix4x4 projection;
            projection.setRow(0,row5);
            projection.setRow(1,row6);
            projection.setRow(2,row7);
            projection.setRow(3,row8);

            /* Get selection center position */
            QVector4D SelectionCenter = NBPointer->SelectionCenterPos.toVector4D();
            SelectionCenter.setW(1.0f); // set W to 1 for a point

            /* Transform center position from world coordinates to screen coordinates */
            QVector4D RotationCenter = projection.transposed()*modelview.transposed()*SelectionCenter;

            //Only X and Y are needed
            RotationCenterScreen.setX(((RotationCenter.x()/RotationCenter.w()+1)/2)*xWidth);
            RotationCenterScreen.setY(((-RotationCenter.y()/RotationCenter.w()+1)/2)*yHeight);

            qDebug() << "Rotation middlepoint in screen coordinates is " << RotationCenterScreen;

            /* Take a copy of original node coordinates of selected nodes */
            NBPointer->TempNodes.clear();
            for(int i=0; i<NBPointer->SelectedNodes.size();i++)
            {
                NBPointer->TempNodes.append(NBPointer->Nodes.at(NBPointer->SelectedNodes.at(i)));
                NBPointer->TempNodes[i].locX = NBPointer->TempNodes[i].locX - NBPointer->SelectionCenterPos.x();
                NBPointer->TempNodes[i].locY = NBPointer->TempNodes[i].locY - NBPointer->SelectionCenterPos.y();
                NBPointer->TempNodes[i].locZ = NBPointer->TempNodes[i].locZ - NBPointer->SelectionCenterPos.z();
            }

        }
        /* 3D Rectangle selection begin */
        else if(RectSelect > 0)
        {
            RectSelect = 2;
            RectSelect_start.setX(event->x());
            RectSelect_start.setY(event->y());
        }

    }
    updateGL();

    /*
    //Ray trace test
    GLfloat matriisi[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matriisi);

    QVector4D row1(matriisi[0],matriisi[1],matriisi[2],matriisi[3]);
    QVector4D row2(matriisi[4],matriisi[5],matriisi[6],matriisi[7]);
    QVector4D row3(matriisi[8],matriisi[9],matriisi[10],matriisi[11]);
    QVector4D row4(matriisi[12],matriisi[13],matriisi[14],matriisi[15]);

    QMatrix4x4 modelview;
    modelview.setRow(0,row1);
    modelview.setRow(1,row2);
    modelview.setRow(2,row3);
    modelview.setRow(3,row4);
    modelview = modelview.transposed();
    modelview = modelview.inverted();

    //RayTrace test point
    QVector4D unitvector = RayTraceVector(lastPos.x(),lastPos.y());
    unitvec = unitvector;
    campos = modelview.column(3);
    */

}

void GLWidget::setZoom()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat(xWidth) / yHeight)*ZoomFactor;
    glFrustum(-x, x, -ZoomFactor, ZoomFactor, 4.0, 55.0);
    glMatrixMode(GL_MODELVIEW);
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //Set keyboard focus to this widget, if mouse moves on the widget
    this->setFocus();
    //qDebug() << "hiiri " << event->x() << ", " << event->y();
    HighlightNode = 99999;
    if(AddingBeamsSingle==2)
    {
        updateGL();
    }
    else if(AddingBeamsCont==2)
    {
        updateGL();
    }
    else if(AddingJbeam>-1) updateGL();
    else if(PrintPickedNodeName>0) updateGL();
    else if(AddingWheels==1)
    {
        TextOverlay = "Choose 1st wheel node";
    }
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {

        if(MovingNodes==2)
        {
            MovingNodes_CalculateMove(event);

            for(int i5=0; i5<NBPointer->SelectedNodes2.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes2[i5]].locX += -0.010f*(movement_x);

            }
            updateGL();
            NBPointer->TextBoxoutdated=1;
        }
        else if(MovingNodes==3)
        {
            MovingNodes_CalculateMove(event);

            for(int i5=0; i5<NBPointer->SelectedNodes2.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes2[i5]].locY += 0.010f*(movement_y);

            }
            updateGL();
        }
        else if(MovingNodes==4)
        {
            MovingNodes_CalculateMove(event);

            for(int i5=0; i5<NBPointer->SelectedNodes2.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes2[i5]].locZ += -0.010f*(movement_x);

            }
            updateGL();
            NBPointer->TextBoxoutdated=1;
        }
        else if(MovingNodes>0)
        {
            if(Moving3D_ModeX)
            {
                MovingNodes_CalculateMove(event);

                for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
                {
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX += -0.010f*movement_x*qCos(DegreeToRadiansRatio*(zRot/16))*ZoomFactor;
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX += 0.010f*movement_y*qSin(DegreeToRadiansRatio*(zRot/16))*ZoomFactor;
                }
                NBPointer->Editing3D_CalculateSelectionCenter();
                updateGL();
            }
            else if(Moving3D_ModeY)
            {
                MovingNodes_CalculateMove(event);

                for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
                {
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY += 0.010f*movement_x*qSin(DegreeToRadiansRatio*(zRot/16));
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY += 0.010f*movement_y*qCos(DegreeToRadiansRatio*(zRot/16));

                }
                NBPointer->Editing3D_CalculateSelectionCenter();
                updateGL();
            }
            else if(Moving3D_ModeZ)
            {
                MovingNodes_CalculateMove(event);

                for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
                {
                       NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ += -0.010f*movement_y*qSin(DegreeToRadiansRatio*(xRot/16));

                }
                NBPointer->Editing3D_CalculateSelectionCenter();
                updateGL();
            }
            NBPointer->TextBoxoutdated=1;
        }
        else if(ScalingNodes>0)
        {
            QVector2D ScalingVectorBegin;
            ScalingVectorBegin.setX(ScaleStartScreen.x()-ScalingCenterScreen.x());
            ScalingVectorBegin.setY(ScaleStartScreen.y()-ScalingCenterScreen.y());
            QVector2D ScalingVector;
            ScalingVector.setX(event->x()-ScalingCenterScreen.x());
            ScalingVector.setY(event->y()-ScalingCenterScreen.y());

            float ScalingFactor = ScalingVector.length()/ScalingVectorBegin.length();
            ScalingFactor = (ScalingFactor-1)*0.5f + 1;
            qDebug()<<  "scaling vector is " << ScalingVector;
            if(Scaling3D_ModeX)
            {
                qDebug() << "Scalingx";
                for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
                {
                    float XCoordinate = NBPointer->TempNodes.at(i5).locX;
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX = XCoordinate*ScalingFactor + NBPointer->SelectionCenterPos.x();

                }
                updateGL();
            }
            else if(Scaling3D_ModeY)
            {
                for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
                {
                    float YCoordinate = NBPointer->TempNodes.at(i5).locY;
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY = YCoordinate*ScalingFactor + NBPointer->SelectionCenterPos.y();

                }
                updateGL();
            }
            else if(Scaling3D_ModeZ)
            {
                for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
                {
                    float ZCoordinate = NBPointer->TempNodes.at(i5).locZ;
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ = ZCoordinate*ScalingFactor + NBPointer->SelectionCenterPos.z();

                }
                updateGL();
            }
            NBPointer->TextBoxoutdated=1;
        }
        else if(RotatingNodes>0)
        {
            QVector2D RotationVec;
            RotationVec.setX(event->x()-RotationCenterScreen.x());
            RotationVec.setY(event->y()-RotationCenterScreen.y());
            float angle = qAcos(RotationVec.x()/RotationVec.length());
            float angle2 = qAsin(RotationVec.y()/RotationVec.length());

            QVector2D BeginVec;
            BeginVec.setX(RotationStartScreen.x()-RotationCenterScreen.x());
            BeginVec.setY(RotationStartScreen.y()-RotationCenterScreen.y());
            float angle3 = qAcos(BeginVec.x()/BeginVec.length());
            float angle4 = qAsin(BeginVec.y()/BeginVec.length());

            if(angle2<0)
            {
                if(angle4<0)
                {
                    angle = angle - angle3;
                }
                else
                {
                    if((pii-angle3)<angle) angle = pii+(pii-angle)-angle3;
                    else angle = -angle-angle3;
                    angle = -angle;
                }
            }
            else
            {
                if(angle4<0)
                {
                    if((pii-angle3)<angle) angle = pii+(pii-angle)-angle3;
                    else angle = -angle-angle3;
                }
                else angle = -(angle - angle3);
            }

            if(ManipulateByStep)
            {
                //Rounding to 10 degrees resolution
                angle = angle*(360.0f/(2*pii))/10;
                QString angletemp = QString::number(angle, 'f', 0);
                angle = angletemp.toFloat();
                angle = angle*((2*pii)/360.0f)*10;
            }
            else
            {
                //Rounding to 0.1 degrees resolution
                angle = angle*(360.0f/(2*pii));
                QString angletemp = QString::number(angle, 'f', 1);
                angle = angletemp.toFloat();
                angle = angle*((2*pii)/360.0f);
            }

            if(Rotating3D_ModeX)
            {
                //Correct the rotation direction based on camera angle
                if(zRot < 2880) angle = -angle;
                if(dRot < 2880) angle = -angle;

                float a1 = qCos(angle);
                float b1 = qSin(angle);
                float a2 = qSin(angle);
                float b2 = qCos(angle);

                for(int i=0; i<NBPointer->SelectedNodes.size();i++)
                {
                    float YCoordinate = NBPointer->TempNodes.at(i).locY;
                    float ZCoordinate = NBPointer->TempNodes.at(i).locZ;
                    NBPointer->Nodes[NBPointer->SelectedNodes[i]].locY = a1*YCoordinate - b1*ZCoordinate + NBPointer->SelectionCenterPos.y();
                    NBPointer->Nodes[NBPointer->SelectedNodes[i]].locZ = a2*YCoordinate + b2*ZCoordinate + NBPointer->SelectionCenterPos.z();
                }
                QString Message = "Rotating X: ";
                Message.append(QString::number((angle*360.0f/(2*pii))));
                TextOverlay = Message;

            }
            else if(Rotating3D_ModeY)
            {
                if((zRot > 1440) && (zRot < 4320)) angle = -angle;
                if(dRot < 2880) angle = -angle;

                float a1 = qCos(angle);
                float b1 = qSin(angle);
                float a2 = qSin(angle);
                float b2 = qCos(angle);

                for(int i=0; i<NBPointer->SelectedNodes.size();i++)
                {
                    float XCoordinate = NBPointer->TempNodes.at(i).locX;
                    float ZCoordinate = NBPointer->TempNodes.at(i).locZ;
                    NBPointer->Nodes[NBPointer->SelectedNodes[i]].locX = a1*XCoordinate - b1*ZCoordinate + NBPointer->SelectionCenterPos.x();
                    NBPointer->Nodes[NBPointer->SelectedNodes[i]].locZ = a2*XCoordinate + b2*ZCoordinate + NBPointer->SelectionCenterPos.z();
                }

                QString Message = "Rotating Y: ";
                Message.append(QString::number((angle*360.0f/(2*pii))));
                TextOverlay = Message;

            }
            else if(Rotating3D_ModeZ)
            {
                if((dRot > 4320) || (dRot < 1440)) angle = -angle;

                float a1 = qCos(angle);
                float b1 = qSin(angle);
                float a2 = qSin(angle);
                float b2 = qCos(angle);
                for(int i=0; i<NBPointer->SelectedNodes.size();i++)
                {
                    float YCoordinate = NBPointer->TempNodes.at(i).locY;
                    float XCoordinate = NBPointer->TempNodes.at(i).locX;
                    NBPointer->Nodes[NBPointer->SelectedNodes[i]].locY = a1*YCoordinate - b1*XCoordinate + NBPointer->SelectionCenterPos.y();
                    NBPointer->Nodes[NBPointer->SelectedNodes[i]].locX = a2*YCoordinate + b2*XCoordinate + NBPointer->SelectionCenterPos.x();
                }

                QString Message = "Rotating Z: ";
                Message.append(QString::number((angle*360.0f/(2*pii))));
                TextOverlay = Message;

            }
            updateGL();
            NBPointer->TextBoxoutdated=1;
        }
        else if(RectSelect > 0)
        {
            updateGL();
            /* 3D Rectangle selection ends */
            RectSelect_end.setX(event->x());
            RectSelect_end.setY(event->y());
            QPoint x1;
            QPoint y1;

            /*
            y1 - - - - - - end
            |               |
            |               |
            start - - - - - x1
            */

            x1.setX(RectSelect_end.x());
            x1.setY(RectSelect_start.y());

            y1.setX(RectSelect_start.x());
            y1.setY(RectSelect_end.y());

            RectSel_1 = RayTraceVector(RectSelect_start.x(), RectSelect_start.y());
            RectSel_2 = RayTraceVector(x1.x(), x1.y());
            RectSel_3 = RayTraceVector(y1.x(), y1.y());
            RectSel_4 = RayTraceVector(RectSelect_end.x(), RectSelect_end.y());
        }

    } else if (event->buttons() & Qt::RightButton) {

        setZRotation(zRot + 8 * dx);
        setDRotation(dRot + 8 * dy);

    }
    //qDebug()<< "3D view "<< event->pos();
    lastPos = event->pos();

    /*
    qDebug() << "x " << xRot/16 << "z " << zRot/16;
    qDebug() << " xr " << qSin(((2*pii)/360)*(xRot/16));
    qDebug() << " zr " << qSin(((2*pii)/360)*(zRot/16));
    float dotpr = qSin(((2*pii)/360)*(zRot/16));
    qDebug() << "X-liike " << dotpr;
    float dotpr2 = qCos(((2*pii)/360)*(zRot/16));
    qDebug() << "Y-liike " << dotpr2;
    */
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        if(MovingNodes>0)
        {
            /* User has released the mouse, => stop moving nodes */
            Moving3D_ModeX = 0;
            Moving3D_ModeY = 0;
            Moving3D_ModeZ = 0;
        }
        else if(ScalingNodes>0)
        {
            Scaling3D_ModeX = 0;
            Scaling3D_ModeY = 0;
            Scaling3D_ModeZ = 0;
            NBPointer->Editing3D_CalculateSelectionCenter();
        }
        else if(RotatingNodes>0)
        {
            Rotating3D_ModeX = 0;
            Rotating3D_ModeY = 0;
            Rotating3D_ModeZ = 0;
            TextOverlay.clear();
        }
        else if(RectSelect > 1)
        {
            /* 3D Rectangle selection ends */
            RectSelect_end.setX(event->x());
            RectSelect_end.setY(event->y());
            QPoint x1;
            QPoint y1;

            /*
            y1 - - - - - - end
            |               |
            |               |
            start - - - - - x1
            */

            x1.setX(RectSelect_end.x());
            x1.setY(RectSelect_start.y());

            y1.setX(RectSelect_start.x());
            y1.setY(RectSelect_end.y());

            RectSel_1 = RayTraceVector(RectSelect_start.x(), RectSelect_start.y());
            RectSel_2 = RayTraceVector(x1.x(), x1.y());
            RectSel_3 = RayTraceVector(y1.x(), y1.y());
            RectSel_4 = RayTraceVector(RectSelect_end.x(), RectSelect_end.y());

            NBPointer->SelectNodes3D(RectSel_1, RectSel_2, RectSel_3, RectSel_4, campos, Select_AddToSelection);
            NBPointer->Editing3D_CalculateSelectionCenter();
            updateGL();
            RectSelect = 1;
        }

        //qDebug() << "Left released";

    }

}

/* Mouse wheel Zoom */
void GLWidget::wheelEvent(QWheelEvent * event)
{
    this->updateGL();
    float NewZoom = ZoomFactor - event->delta()*0.001f;
    if(NewZoom > 0) ZoomFactor = NewZoom;
    resizeGL(this->width(),this->height());
    updateGL();
}

void GLWidget::MovingNodes_CalculateMove(QMouseEvent *event)
{
    movement_x = lastPos.x();
    movement_y = lastPos.y();
    movement_x  -= event->x();
    movement_y  -= event->y();
}

void GLWidget::setNBPointer(NodeBeam* NBPointer1)
{
    NBPointer = NBPointer1;
}

/* Draw hubwheel or wheel */
void GLWidget::DrawWheel(float radius, float width, int rays)
{
    float step = (2.0f*pii)/rays;
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    glColor3f(1.0, 1.0, 0.0);
    for (int i = 0; i < rays; i++)
    {
        glVertex3d(width, (radius*qSin(i*step)),radius*(qCos(i*step)));
        glVertex3d(width, (radius*qSin((i+1)*step)),radius*(qCos((i+1)*step)));

        glVertex3d(0, (radius*qSin(i*step)),(radius*qCos(i*step)));
        glVertex3d(0, (radius*qSin((i+1)*step)),(radius*qCos((i+1)*step)));

        glVertex3d(width, (radius*qSin(i*step)),(radius*qCos(i*step)));
        glVertex3d(0, (radius*qSin(i*step)),(radius*qCos(i*step)));
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/* Draw 3D Edit cursor */
/* Draw arrows for X, Y and Z axis */
void GLWidget::Draw3DCursor()
{
    glPushMatrix();

    GLfloat centerx = NBPointer->SelectionCenterPos.x();
    GLfloat centery = NBPointer->SelectionCenterPos.y();
    GLfloat centerz = NBPointer->SelectionCenterPos.z();
    glTranslatef(centerx, centery, centerz);
    glScalef(ZoomFactor,ZoomFactor,ZoomFactor);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);


    //X-Arrow
    glVertex3f(0.9, -0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, 0.05, 0.05);

    glVertex3f(0.9, 0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, -0.05, -0.05);

    glVertex3f(0.9, -0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, -0.05, 0.05);

    glVertex3f(0.9, 0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9, 0.05, -0.05);

    glVertex3f(0.9, 0.05, 0.05);
    glVertex3f(0.9, 0.05, -0.05);
    glVertex3f(0.9, -0.05, 0.05);
    glVertex3f(0.9, -0.05, 0.05);
    glVertex3f(0.9, 0.05, -0.05);
    glVertex3f(0.9, -0.05, -0.05);
    glEnd();

    //Tall arrow
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.1, -0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, 0.05, 0.05);

    glVertex3f(0.1, 0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, -0.05, -0.05);

    glVertex3f(0.1, -0.05, -0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, -0.05, 0.05);

    glVertex3f(0.1, 0.05, 0.05);
    glVertex3f(1, 0, 0);
    glVertex3f(0.1, 0.05, -0.05);

    glVertex3f(0.1, 0.05, 0.05);
    glVertex3f(0.1, 0.05, -0.05);
    glVertex3f(0.1, -0.05, 0.05);
    glVertex3f(0.1, -0.05, 0.05);
    glVertex3f(0.1, 0.05, -0.05);
    glVertex3f(0.1, -0.05, -0.05);

    glEnd();
    //Y-Arrow

    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);

    glVertex3f(0.05, 0.9, 0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.9, 0.05);

    glVertex3f(-0.05, 0.9, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05,0.9,  -0.05);

    glVertex3f(-0.05,0.9,  0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.9, -0.05);

    glVertex3f(0.05, 0.9, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05, 0.9, 0.05);

    glVertex3f(-0.05, 0.9, 0.05);
    glVertex3f(0.05, 0.9, -0.05);
    glVertex3f(0.05, 0.9, 0.05);
    glVertex3f(-0.05, 0.9, -0.05);
    glVertex3f(0.05, 0.9, -0.05);
    glVertex3f(-0.05, 0.9, 0.05);

    glEnd();

    //Tall arrow
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);

    glVertex3f(0.05, 0.1, 0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.1, 0.05);

    glVertex3f(-0.05, 0.1, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05,0.1,  -0.05);

    glVertex3f(-0.05,0.1,  0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.05, 0.1, -0.05);

    glVertex3f(0.05, 0.1, -0.05);
    glVertex3f(0, 1, 0);
    glVertex3f(0.05, 0.1, 0.05);

    glVertex3f(-0.05, 0.1, 0.05);
    glVertex3f(0.05, 0.1, -0.05);
    glVertex3f(0.05, 0.1, 0.05);
    glVertex3f(-0.05, 0.1, -0.05);
    glVertex3f(0.05, 0.1, -0.05);
    glVertex3f(-0.05, 0.1, 0.05);

    glEnd();

    //Z Arrow
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0);

    glVertex3f(-0.05, 0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, 0.05, 0.9);

    glVertex3f(0.05, -0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, -0.05, 0.9);

    glVertex3f(-0.05, -0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, 0.05, 0.9);

    glVertex3f(0.05, 0.05, 0.9);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, -0.05, 0.9);

    glVertex3f(0.05, -0.05, 0.9);
    glVertex3f(-0.05, 0.05, 0.9);
    glVertex3f(0.05, 0.05, 0.9);
    glVertex3f(-0.05, -0.05, 0.9);
    glVertex3f(-0.05, 0.05, 0.9);
    glVertex3f(0.05, -0.05, 0.9);

    glEnd();

    //Tall arrow
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0);

    glVertex3f(-0.05, 0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, 0.05, 0.1);

    glVertex3f(0.05, -0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, -0.05, 0.1);

    glVertex3f(-0.05, -0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(-0.05, 0.05, 0.1);

    glVertex3f(0.05, 0.05, 0.1);
    glVertex3f(0, 0, 1);
    glVertex3f(0.05, -0.05, 0.1);

    glVertex3f(0.05, -0.05, 0.1);
    glVertex3f(-0.05, 0.05, 0.1);
    glVertex3f(0.05, 0.05, 0.1);
    glVertex3f(-0.05, -0.05, 0.1);
    glVertex3f(-0.05, 0.05, 0.1);
    glVertex3f(0.05, -0.05, 0.1);
    glEnd();

    glPopMatrix();

}

void GLWidget::Draw3DCursor_Scale()
{
    glPushMatrix();

    GLfloat centerx = NBPointer->SelectionCenterPos.x();
    GLfloat centery = NBPointer->SelectionCenterPos.y();
    GLfloat centerz = NBPointer->SelectionCenterPos.z();
    glTranslatef(centerx, centery, centerz);
    glScalef(ZoomFactor,ZoomFactor,ZoomFactor);
    //Y arrow
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);

    glVertex3f(0.05, 0.05, -0.05);
    glVertex3f(-0.05, 0.05, -0.05);
    glVertex3f(0.05, 1, -0.05);

    glVertex3f(-0.05, 0.05, -0.05);
    glVertex3f(-0.05, 1, -0.05);
    glVertex3f(0.05, 1, -0.05);

    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(0.05, 1, 0.05);
    glVertex3f(-0.05, 0.05, 0.05);

    glVertex3f(-0.05, 0.05, 0.05);
    glVertex3f(0.05, 1, 0.05);
    glVertex3f(-0.05, 1, 0.05);

    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(0.05, 0.05, -0.05);
    glVertex3f(0.05, 1, 0.05);

    glVertex3f(0.05, 0.05, -0.05);
    glVertex3f(0.05, 1, -0.05);
    glVertex3f(0.05, 1, 0.05);

    glVertex3f(-0.05, 0.05, 0.05);
    glVertex3f(-0.05, 1, 0.05);
    glVertex3f(-0.05, 0.05, -0.05);

    glVertex3f(-0.05, 0.05, -0.05);
    glVertex3f(-0.05, 1, 0.05);
    glVertex3f(-0.05, 1, -0.05);

    glVertex3f(-0.05, 1, 0.05);
    glVertex3f(0.05, 1, 0.05);
    glVertex3f(-0.05, 1, -0.05);

    glVertex3f(-0.05, 1, -0.05);
    glVertex3f(0.05, 1, 0.05);
    glVertex3f(0.05, 1, -0.05);

    glVertex3f(-0.05, 0.05, 0.05);
    glVertex3f(-0.05, 0.05, -0.05);
    glVertex3f(0.05, 0.05, 0.05);

    glVertex3f(-0.05, 0.05, -0.05);
    glVertex3f(0.05, 0.05, -0.05);
    glVertex3f(0.05, 0.05, 0.05);


    //X-arrow
    glColor3f(1.0, 0.0, 0.0);

    glVertex3f(0.05, 0.05, -0.05);
    glVertex3f(1, 0.05, -0.05);
    glVertex3f(0.05, -0.05, -0.05);

    glVertex3f(0.05, -0.05, -0.05);
    glVertex3f(1, 0.05, -0.05);
    glVertex3f(1, -0.05, -0.05);

    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(1, 0.05, 0.05);

    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(1, -0.05, 0.05);
    glVertex3f(1, 0.05, 0.05);

    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(1, 0.05, 0.05);
    glVertex3f(0.05, 0.05, -0.05);

    glVertex3f(0.05, 0.05, -0.05);
    glVertex3f(1, 0.05, 0.05);
    glVertex3f(1, 0.05, -0.05);

    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(0.05, -0.05, -0.05);
    glVertex3f(1, -0.05, 0.05);

    glVertex3f(0.05, -0.05, -0.05);
    glVertex3f(1, -0.05, -0.05);
    glVertex3f(1, -0.05, 0.05);

    glVertex3f(1, -0.05, 0.05);
    glVertex3f(1, -0.05, -0.05);
    glVertex3f(1, 0.05, 0.05);

    glVertex3f(1, -0.05, -0.05);
    glVertex3f(1, 0.05, -0.05);
    glVertex3f(1, 0.05, 0.05);

    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(0.05, -0.05, -0.05);

    glVertex3f(0.05, -0.05, -0.05);
    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(0.05, 0.05, -0.05);

    //Z-arrow
    glColor3f(0.0, 0.0, 1.0);

    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(-0.05, -0.05, 0.05);
    glVertex3f(0.05, -0.05, 1);

    glVertex3f(-0.05, -0.05, 0.05);
    glVertex3f(-0.05, -0.05, 1);
    glVertex3f(0.05,  -0.05, 1);

    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(0.05,  0.05, 1);
    glVertex3f(-0.05, 0.05, 0.05);

    glVertex3f(-0.05, 0.05, 0.05);
    glVertex3f(0.05, 0.05, 1);
    glVertex3f(-0.05, 0.05, 1);

    glVertex3f(0.05, 0.05, 0.05);
    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(0.05,  0.05, 1);

    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(0.05,  -0.05, 1);
    glVertex3f(0.05,  0.05, 1);

    glVertex3f(-0.05, 0.05, 0.05);
    glVertex3f(-0.05,  0.05, 1);
    glVertex3f(-0.05, -0.05, 0.05);

    glVertex3f(-0.05, -0.05, 0.05);
    glVertex3f(-0.05,  0.05, 1);
    glVertex3f(-0.05,  -0.05, 1);

    glVertex3f(-0.05,  0.05, 1);
    glVertex3f(0.05, 0.05, 1);
    glVertex3f(-0.05, -0.05, 1);

    glVertex3f(-0.05, -0.05, 1);
    glVertex3f(0.05, 0.05, 1);
    glVertex3f(0.05, -0.05, 1);

    glVertex3f(-0.05, 0.05, 0.05);
    glVertex3f(-0.05, -0.05, 0.05);
    glVertex3f(0.05, 0.05, 0.05);

    glVertex3f(-0.05, -0.05, 0.05);
    glVertex3f(0.05, -0.05, 0.05);
    glVertex3f(0.05, 0.05, 0.05);

    glEnd();



    glPopMatrix();

}

void GLWidget::Draw3DCursor_Rotate()
{
    glPushMatrix();

    GLfloat centerx = NBPointer->SelectionCenterPos.x();
    GLfloat centery = NBPointer->SelectionCenterPos.y();
    GLfloat centerz = NBPointer->SelectionCenterPos.z();
    glTranslatef(centerx, centery, centerz);
    glScalef(ZoomFactor,ZoomFactor,ZoomFactor);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);

    float angle = DegreeToRadiansRatio*15;

    //Draw X circle
    glColor3f(1.0, 0.0, 0.0);
    for(int i=0; i<24; i++)
    {
        glVertex3f(-0.05, qSin(angle*i)*1.00, qCos(angle*i)*1.00);
        glVertex3f(0.0, qSin(angle*i)*1.05, qCos(angle*i)*1.05);
        glVertex3f(-0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);

        glVertex3f(-0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);
        glVertex3f(0.0, qSin(angle*i)*1.05, qCos(angle*i)*1.05);
        glVertex3f(0.0, qSin(angle*(i+1))*1.05, qCos(angle*(i+1))*1.05);

        glVertex3f(-0.05, qSin(angle*i)*1.00, qCos(angle*i)*1.00);
        glVertex3f(-0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);
        glVertex3f(0.0, qSin(angle*i)*0.95, qCos(angle*i)*0.95);

        glVertex3f(-0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);
        glVertex3f(0.0, qSin(angle*(i+1))*0.95, qCos(angle*(i+1))*0.95);
        glVertex3f(0.0, qSin(angle*i)*0.95, qCos(angle*i)*0.95);

        glVertex3f(0.05, qSin(angle*i)*1.00, qCos(angle*i)*1.00);
        glVertex3f(0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);
        glVertex3f(0.0, qSin(angle*i)*1.05, qCos(angle*i)*1.05);

        glVertex3f(0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);
        glVertex3f(0.0, qSin(angle*(i+1))*1.05, qCos(angle*(i+1))*1.05);
        glVertex3f(0.0, qSin(angle*i)*1.05, qCos(angle*i)*1.05);

        glVertex3f(0.05, qSin(angle*i)*1.00, qCos(angle*i)*1.00);
        glVertex3f(0.0, qSin(angle*i)*0.95, qCos(angle*i)*0.95);
        glVertex3f(0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);

        glVertex3f(0.05, qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00);
        glVertex3f(0.0, qSin(angle*i)*0.95, qCos(angle*i)*0.95);
        glVertex3f(0.0, qSin(angle*(i+1))*0.95, qCos(angle*(i+1))*0.95);
    }

    //Draw Y circle
    glColor3f(0.0, 1.0, 0.0);
    for(int i=0; i<24; i++)
    {
        glVertex3f(qSin(angle*i)*1.00,-0.05, qCos(angle*i)*1.00);
        glVertex3f(qSin(angle*i)*1.05,0.0, qCos(angle*i)*1.05);
        glVertex3f(qSin(angle*(i+1))*1.00,-0.05, qCos(angle*(i+1))*1.00);

        glVertex3f(qSin(angle*(i+1))*1.00,-0.05, qCos(angle*(i+1))*1.00);
        glVertex3f(qSin(angle*i)*1.05,0.0, qCos(angle*i)*1.05);
        glVertex3f(qSin(angle*(i+1))*1.05,0.0, qCos(angle*(i+1))*1.05);

        glVertex3f(qSin(angle*i)*1.00,-0.05, qCos(angle*i)*1.00);
        glVertex3f(qSin(angle*(i+1))*1.00,-0.05, qCos(angle*(i+1))*1.00);
        glVertex3f(qSin(angle*i)*0.95,0.0, qCos(angle*i)*0.95);

        glVertex3f(qSin(angle*(i+1))*1.00,-0.05, qCos(angle*(i+1))*1.00);
        glVertex3f(qSin(angle*(i+1))*0.95,0.0, qCos(angle*(i+1))*0.95);
        glVertex3f(qSin(angle*i)*0.95,0.0, qCos(angle*i)*0.95);

        glVertex3f(qSin(angle*i)*1.00, 0.05,  qCos(angle*i)*1.00);
        glVertex3f(qSin(angle*(i+1))*1.00, 0.05,  qCos(angle*(i+1))*1.00);
        glVertex3f(qSin(angle*i)*1.05 ,0.0 , qCos(angle*i)*1.05);

        glVertex3f(qSin(angle*(i+1))*1.00, 0.05,  qCos(angle*(i+1))*1.00);
        glVertex3f(qSin(angle*(i+1))*1.05, 0.0,  qCos(angle*(i+1))*1.05);
        glVertex3f(qSin(angle*i)*1.05, 0.0,  qCos(angle*i)*1.05);

        glVertex3f(qSin(angle*i)*1.00, 0.05,  qCos(angle*i)*1.00);
        glVertex3f(qSin(angle*i)*0.95, 0.0,  qCos(angle*i)*0.95);
        glVertex3f(qSin(angle*(i+1))*1.00, 0.05,  qCos(angle*(i+1))*1.00);

        glVertex3f(qSin(angle*(i+1))*1.00, 0.05,  qCos(angle*(i+1))*1.00);
        glVertex3f(qSin(angle*i)*0.95, 0.0,  qCos(angle*i)*0.95);
        glVertex3f(qSin(angle*(i+1))*0.95, 0.0,  qCos(angle*(i+1))*0.95);
    }
    //Draw Z circle
    glColor3f(0.0, 0.0, 1.0);
    for(int i=0; i<24; i++)
    {
        glVertex3f(qSin(angle*i)*1.00, qCos(angle*i)*1.00, -0.05);
        glVertex3f(qSin(angle*i)*1.05, qCos(angle*i)*1.05, 0.0);
        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, -0.05);

        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, -0.05);
        glVertex3f(qSin(angle*i)*1.05, qCos(angle*i)*1.05, 0.0);
        glVertex3f(qSin(angle*(i+1))*1.05, qCos(angle*(i+1))*1.05, 0.0);

        glVertex3f(qSin(angle*i)*1.00, qCos(angle*i)*1.00, -0.05);
        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, -0.05);
        glVertex3f(qSin(angle*i)*0.95, qCos(angle*i)*0.95, 0.0);

        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, -0.05);
        glVertex3f(qSin(angle*(i+1))*0.95, qCos(angle*(i+1))*0.95, 0.0);
        glVertex3f(qSin(angle*i)*0.95, qCos(angle*i)*0.95, 0.0);

        glVertex3f(qSin(angle*i)*1.00, qCos(angle*i)*1.00, 0.05);
        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, 0.05);
        glVertex3f(qSin(angle*i)*1.05, qCos(angle*i)*1.05, 0.0);

        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, 0.05);
        glVertex3f(qSin(angle*(i+1))*1.05, qCos(angle*(i+1))*1.05, 0.0);
        glVertex3f(qSin(angle*i)*1.05, qCos(angle*i)*1.05, 0.0);

        glVertex3f(qSin(angle*i)*1.00, qCos(angle*i)*1.00, 0.05);
        glVertex3f(qSin(angle*i)*0.95, qCos(angle*i)*0.95, 0.0);
        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, 0.05);

        glVertex3f(qSin(angle*(i+1))*1.00, qCos(angle*(i+1))*1.00, 0.05);
        glVertex3f(qSin(angle*i)*0.95, qCos(angle*i)*0.95, 0.0);
        glVertex3f(qSin(angle*(i+1))*0.95, qCos(angle*(i+1))*0.95, 0.0);
    }

    glEnd();

    glPopMatrix();

}


/* Draw 3D cursor picking */
void GLWidget::Draw3DCursor_Picking(int Mode)
{
    //Each 3D cursor arrow is rendered with unique color, and color is checked
    //there, where the mouse is clicked, to detect which arrow the user wants to move

    //glDisable(GL_DITHER);
    //Nodes
    // Turn off lighting
    glDisable(GL_LIGHTING);

    // Turn off antialiasing
    glDisable (GL_BLEND);
    glDisable(GL_MULTISAMPLE);

    // Turn off texturing
    glDisable(GL_TEXTURE_2D);

    GLubyte pixel[3];
    GLint viewport[4];

    //Get values of OpenGL viewport. viewport[3] will be the height of the viewport
    glGetIntegerv(GL_VIEWPORT,viewport);

    if(Mode==0) Draw3DCursor();
    else if(Mode == 1) Draw3DCursor_Scale();
    else if(Mode == 2) Draw3DCursor_Rotate();

    glReadPixels(lastPos.x(), viewport[3]-lastPos.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) pixel);
    //qDebug() << "3D cursor picking " << pixel[0] << ", " << pixel[1] << "," << pixel[2];

    if(Mode == 0)
    {
        if(pixel[0] == 255) Moving3D_Mode = 1;
        else if(pixel[1] == 255) Moving3D_Mode = 2;
        else if(pixel[2] == 255) Moving3D_Mode = 3;
        else Moving3D_Mode = 0;
    }
    else if(Mode == 1)
    {

        if(pixel[0] == 255) Scaling3D_Mode = 1;
        else if(pixel[1] == 255) Scaling3D_Mode = 2;
        else if(pixel[2] == 255) Scaling3D_Mode = 3;
        else Scaling3D_Mode = 0;
    }
    else if(Mode == 2)
    {
        if(pixel[0] == 255) Rotating3D_Mode = 1;
        else if(pixel[1] == 255) Rotating3D_Mode = 2;
        else if(pixel[2] == 255) Rotating3D_Mode = 3;
        else Rotating3D_Mode = 0;
    }


    glEnable(GL_LIGHTING);

    // Turn on antialiasing
    glEnable (GL_BLEND);
    glEnable(GL_MULTISAMPLE);

    // Turn on texturing
    glEnable(GL_TEXTURE_2D);

}

void GLWidget::DrawSphere(int segments, int diameter)
{
    double radius = diameter/2.0;
    double angle = 180/(segments);
    double angle1=0;
    QVector<double> xcoords;
    QVector<double> ycoords;
    QVector<double> scalefactors;
    for(int i=0; i < segments; i++)
    {
        xcoords.append(radius*sin(angle1));
        ycoords.append(radius*cos(angle1));
        qDebug() << "Angle is:";
        qDebug() << angle1;
        qDebug() << xcoords[i] << ", " << ycoords[i];
        scalefactors.append(xcoords[i]/radius);
        angle1 = angle1+angle;

    }

    glColor3f(0.0f,0.0f,1.0f); //blue color
//    glBegin(GL_QUAD_STRIP);

//    for(int i=0; i<segments; i++)
//    {

//        glVertex3f(xcoords[i], ycoords[i], 0);
//        glVertex3f(xcoords[i]*scalefactors[i], ycoords[i]*scalefactors[i], 0.2);

//    }
//    glEnd;

}

/* Calculate ray trace vector from camera center position
 * to mouse position on near clipping plane */
QVector4D GLWidget::RayTraceVector(int MouseX, int MouseY)
{
    /* Get modelview matrix and projection matrix */
    QMatrix4x4 modelview;
    QMatrix4x4 projection;
    GetViewMatrices(&modelview,&projection);

    /* Inverse both matrixes */
    bool InverseOk = 0;
    QMatrix4x4 inverse_modelview = modelview.inverted(&InverseOk);
    if(!InverseOk) qDebug() << "MATRIX INVERSE FAILURE";

    InverseOk = 0;
    QMatrix4x4 inverse_projection = projection.inverted(&InverseOk);
    if(!InverseOk) qDebug() << "MATRIX INVERSE FAILURE";

    /* Camera position is 4th column of the inverse_modelview matrix */
    QVector4D CameraPos = inverse_modelview.column(3);
    campos=CameraPos;

    /* Mouse position to normalized coordinates */
    float posX = (float)MouseX;
    float posY = (float)MouseY;
    posX = (posX/xWidth)*2.0f -1.0f;
    posY = -((posY/yHeight)*2.0f - 1.0f);

    QVector4D MousePos;
    MousePos.setW(1);
    MousePos.setX(posX);
    MousePos.setY(posY);
    MousePos.setZ(0.0f);

    /* Calculate mouse position in actual scene 3D coordinates
        with the inverse matrixes */
    MousePos = inverse_projection*MousePos;
    MousePos = inverse_modelview*MousePos*7.965;

    /* Calculate the ray trace vector */
    QVector4D RayTraceVector = MousePos - CameraPos;
    RayTraceVector.normalize();

    return RayTraceVector;

}

/* Draw box for 3D rectangle selection tool */
void GLWidget::DrawRectSelect()
{
    QVector4D test,RectSel_1V,RectSel_2V,RectSel_3V,RectSel_4V;

    /*
    glBegin(GL_POINTS);
    glColor3f(0.0, 1.0, 0.0);
    for(int i=0; i<20 ; i++)
    {
        glColor3f(0.0, 0.03*i, 0.0);
        test = campos + RectSel_1*i;
        glVertex3d(test.x(), test.y(), test.z());
    }

    for(int i=0; i<20 ; i++)
    {
        glColor3f(0.0, 0.03*i, 0.0);
        test = campos + RectSel_2*i;
        glVertex3d(test.x(), test.y(), test.z());
    }

    for(int i=0; i<20 ; i++)
    {
        glColor3f(0.0, 0.03*i, 0.0);
        test = campos + RectSel_3*i;
        glVertex3d(test.x(), test.y(), test.z());
    }

    for(int i=0; i<20 ; i++)
    {
        glColor3f(0.0, 0.03*i, 0.0);
        test = campos + RectSel_4*i;
        glVertex3d(test.x(), test.y(), test.z());
    }

    glEnd();*/

    glLineStipple(1, 0x3F07);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor4f(0.75f,0.75f,0.75f,0.2f);
    RectSel_1V = (campos + RectSel_1*5);
    RectSel_2V = (campos + RectSel_2*5);
    RectSel_3V = (campos + RectSel_3*5);
    RectSel_4V = (campos + RectSel_4*5);

    glVertex3d(RectSel_1V.x(), RectSel_1V.y(), RectSel_1V.z());
    glVertex3d(RectSel_2V.x(), RectSel_2V.y(), RectSel_2V.z());

    glVertex3d(RectSel_2V.x(), RectSel_2V.y(), RectSel_2V.z());
    glVertex3d(RectSel_4V.x(), RectSel_4V.y(), RectSel_4V.z());

    glVertex3d(RectSel_4V.x(), RectSel_4V.y(), RectSel_4V.z());
    glVertex3d(RectSel_3V.x(), RectSel_3V.y(), RectSel_3V.z());

    glVertex3d(RectSel_3V.x(), RectSel_3V.y(), RectSel_3V.z());
    glVertex3d(RectSel_1V.x(), RectSel_1V.y(), RectSel_1V.z());
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    //normals test
    /* Calculate normals for each plane */
    QVector3D vec1 = RectSel_1V.toVector3D();
    QVector3D vec2 = RectSel_2V.toVector3D();
    QVector3D vec3 = RectSel_3V.toVector3D();
    QVector3D vec4 = RectSel_4V.toVector3D();

    QVector3D normal_1 = QVector3D::crossProduct(vec1,vec2);
    QVector3D normal_2 = QVector3D::crossProduct(vec3,vec1);
    QVector3D normal_3 = QVector3D::crossProduct(vec4,vec3);
    QVector3D normal_4 = QVector3D::crossProduct(vec2,vec4);
    //qDebug() << normal_1 << normal_2 << normal_3 << normal_4;

    /* Draw selection plane normals
    glBegin(GL_LINES);
    QVector3D campos2 = campos.toVector3D();
    QVector3D normal = campos2 + normal_1;
    glVertex3d(normal.x(), normal.y(), normal.z());
    glVertex3d(campos.x(), campos.y(), campos.z());

    normal = campos2 + normal_2;
    glVertex3d(normal.x(), normal.y(), normal.z());
    glVertex3d(campos.x(), campos.y(), campos.z());

    normal = campos2 + normal_3;
    glVertex3d(normal.x(), normal.y(), normal.z());
    glVertex3d(campos.x(), campos.y(), campos.z());

    normal = campos2 + normal_4;
    glVertex3d(normal.x(), normal.y(), normal.z());
    glVertex3d(campos.x(), campos.y(), campos.z());


    glEnd();*/

}

/* Move 3D view around */
void GLWidget::keyPressEvent(QKeyEvent * event)
{
    /* Calculate camera direction vector */
    QVector3D CameraDirection = RayTraceVector(this->width()/2, this->height()/2).toVector3D();
    CameraDirection.normalize();

    QVector3D CameraUpVector(0,0,1);

    QVector3D CameraLeft = QVector3D::crossProduct(CameraDirection,CameraUpVector);

    if(event->key() == Qt::Key_4)
    {
        ViewOffsetX += CameraLeft.x()*0.2f;
        ViewOffsetY += CameraLeft.y()*0.2f;
        updateGL();
    }
    else if(event->key() == Qt::Key_6)
    {
        ViewOffsetX -= CameraLeft.x()*0.2f;
        ViewOffsetY -= CameraLeft.y()*0.2f;
        updateGL();
    }
    else if(event->key() == Qt::Key_8)
    {
        ViewOffsetX -= CameraDirection.x()*0.2f;
        ViewOffsetY -= CameraDirection.y()*0.2f;
        //ViewOffsetZ -= CameraDirection.z()*0.2f;
        updateGL();
    }
    else if(event->key() == Qt::Key_2)
    {
        ViewOffsetX += CameraDirection.x()*0.2f;
        ViewOffsetY += CameraDirection.y()*0.2f;
        //ViewOffsetZ += CameraDirection.z()*0.2f;
        updateGL();
    }
    else if(event->key() == Qt::Key_7)
    {
        ViewOffsetZ -= 0.1f;
        updateGL();
    }
    else if(event->key() == Qt::Key_1)
    {
        ViewOffsetZ += 0.1f;
        updateGL();
    }
    /* Move/Scale/Rotate by step */
    else if(event->key() == Qt::Key_Control)
    {
        ManipulateByStep=1;
    }
    else if(event->key() == Qt::Key_Shift)
    {
        this->Select_AddToSelection = 1;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Shift)
    {
            this->Select_AddToSelection = 0;
    }
    else if(event->key() == Qt::Key_Control)
    {
        ManipulateByStep=0;
    }
}

/* Get projection and model view matrix as transposed */
void GLWidget::GetViewMatrices(QMatrix4x4 * ModelviewMatrix, QMatrix4x4 * ProjectionMatrix)
{
    /* Get modelview matrix */
    GLfloat matriisi[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matriisi);

    QVector4D row1(matriisi[0],matriisi[1],matriisi[2],matriisi[3]);
    QVector4D row2(matriisi[4],matriisi[5],matriisi[6],matriisi[7]);
    QVector4D row3(matriisi[8],matriisi[9],matriisi[10],matriisi[11]);
    QVector4D row4(matriisi[12],matriisi[13],matriisi[14],matriisi[15]);

    QMatrix4x4 modelview;
    modelview.setRow(0,row1);
    modelview.setRow(1,row2);
    modelview.setRow(2,row3);
    modelview.setRow(3,row4);
    modelview = modelview.transposed();

    /* Get projection matrix */
    glGetFloatv(GL_PROJECTION_MATRIX, matriisi);

    QVector4D row5(matriisi[0],matriisi[1],matriisi[2],matriisi[3]);
    QVector4D row6(matriisi[4],matriisi[5],matriisi[6],matriisi[7]);
    QVector4D row7(matriisi[8],matriisi[9],matriisi[10],matriisi[11]);
    QVector4D row8(matriisi[12],matriisi[13],matriisi[14],matriisi[15]);

    QMatrix4x4 projection;
    projection.setRow(0,row5);
    projection.setRow(1,row6);
    projection.setRow(2,row7);
    projection.setRow(3,row8);
    projection = projection.transposed();

    /* Return matrices */
    *ModelviewMatrix = modelview;
    *ProjectionMatrix = projection;

}

void GLWidget::LoadDae()
{

}

bool GLWidget::AddingJbeam_Enable(QString keyword)
{
    bool ok=0;
    for(int i=0;i<NBPointer->ListTypes.size();i++)
    {
        if(NBPointer->ListTypes.at(i).keyword == keyword)
        {
            if(NBPointer->ListTypes.at(i).nodeamount>0)
            {
                AddingJbeam_id = i;
                AddingJbeam = 0;
                NodePicking = 1;
                TextOverlay = NBPointer->ListTypes.at(i).nodenames.at(0);
                ok=1;
            }
        }
    }
    if(!ok)
    {
        AddingJbeam = 0;

        return false;
    }
    else return true;
}

void GLWidget::AddingJbeam_Disable()
{
    AddingJbeam = -1;
    NodePicking = 0;
    BeamSuggestion = 0;
    TextOverlay.clear();
}

void GLWidget::EnableNodePicker()
{
    NodePicking = 1;
    PrintPickedNodeName = 1;
}

void GLWidget::DisableNodePicker()
{
    NodePicking = 0;
    PrintPickedNodeName = 0;
}

/* Reference model loading, create buffer object and vertex array object */
bool GLWidget::LoadRefMesh(QString filename)
{
    QVector <float> verts;

    //Load OBJ file
    if(this->LoadObj(filename, verts))
    {
        verts_size = verts.size();

        //Create buffer object
        if(buffer.size()>0) buffer.destroy();
        buffer.create();
        buffer.bind();
        buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        buffer.allocate(verts.constData(), verts.size()*sizeof(GLfloat));

        //Create vertex array object
        if(VAO.isCreated()) VAO.destroy();
        VAO.create();
        VAO.bind();

        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        buffer.bind();
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        buffer.release();
        VAO.release();
    }
}

/* Load vertices, normals and faces from OBJ file and store in QVector */
bool GLWidget::LoadObj(QString filename, QVector<float> &vertices)
{
    QVector <QVector <float> > tempvertices;
    QVector <QVector <float> > tempnormals;

    QFileInfo fileinfo(filename);
    if(fileinfo.exists())
    {
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly))
        {
            QString contents = file.readAll();
            file.close();

            QStringList rows = contents.split("\n");
            for(int i=0; i<rows.size(); i++)
            {
                rows[i] = rows.at(i).simplified();
                QRegExp separator(" +");
                QStringList row = rows.at(i).split(separator);
                if((row.at(0) == "v") && (row.length()>=4))
                {
                    QVector <float> vertex;
                    vertex.append(row.at(1).toFloat());
                    vertex.append(row.at(3).toFloat());
                    vertex.append(row.at(2).toFloat());
                    vertex[1] = -vertex.at(1);
                    tempvertices.append(vertex);
                }
                else if((row.at(0) == "vn") && (row.length()==4))
                {
                    QVector <float> normal;
                    normal.append(row.at(1).toFloat());
                    normal.append(row.at(3).toFloat());
                    normal.append(row.at(2).toFloat());
                    tempnormals.append(normal);
                }
            }

            for(int i=0; i<rows.size(); i++)
            {
                QRegExp separator(" +");
                QStringList row = rows.at(i).split(separator);
                if((row.at(0) == "f") && (row.length()>=4))
                {
                    QStringList tempface;
                    for(int i2=1; i2<row.length();i2++)
                    {
                        int textcoord = row.at(i2).indexOf("/");
                        if(textcoord >= 0)
                        {
                            QString temp;
                            for(int i3=0; i3<row.at(i2).length();i3++)
                            {
                                if(row.at(i2).at(i3) == '/') break;
                                else temp.append(row.at(i2).at(i3));
                            }
                            tempface.append(temp);
                        }
                        else tempface.append(row.at(i2));
                    }
                    if(tempface.size() == 3)
                    {
                        //it is a triangle
                        for(int i2=0; i2<tempface.size(); i2++)
                        {
                            int vertexloc = tempface.at(i2).toInt()-1;
                            if((vertexloc<tempvertices.size()) && (vertexloc>-1))
                            {
                                vertices.append(tempvertices.at(vertexloc).at(0));
                                vertices.append(tempvertices.at(vertexloc).at(1));
                                vertices.append(tempvertices.at(vertexloc).at(2));
                                vertices.append(1);
                                vertices.append(0);
                                vertices.append(0);
                            }
                        }
                    }
                    else if(tempface.size() >= 4)
                    {
                        //Turn quad to two triangles
                        QStringList face1, face2;
                        face1.append(tempface.at(0));
                        face1.append(tempface.at(1));
                        face1.append(tempface.at(2));
                        face2.append(tempface.at(0));
                        face2.append(tempface.at(2));
                        face2.append(tempface.at(3));

                        for(int i2=0; i2<face1.size(); i2++)
                        {
                            int vertexloc = face1.at(i2).toInt()-1;
                            if((vertexloc<tempvertices.size()) && (vertexloc>-1))
                            {
                                vertices.append(tempvertices.at(vertexloc).at(0));
                                vertices.append(tempvertices.at(vertexloc).at(1));
                                vertices.append(tempvertices.at(vertexloc).at(2));
                                vertices.append(1);
                                vertices.append(0);
                                vertices.append(0);
                            }
                        }

                        for(int i2=0; i2<face2.size(); i2++)
                        {
                            int vertexloc = face2.at(i2).toInt()-1;
                            if((vertexloc<tempvertices.size()) && (vertexloc>-1))
                            {
                                vertices.append(tempvertices.at(vertexloc).at(0));
                                vertices.append(tempvertices.at(vertexloc).at(1));
                                vertices.append(tempvertices.at(vertexloc).at(2));
                                vertices.append(1);
                                vertices.append(0);
                                vertices.append(0);
                            }
                        }
                    }
                }
            }
            return true;
        }
        else return false;
    }
    else return false;
}
