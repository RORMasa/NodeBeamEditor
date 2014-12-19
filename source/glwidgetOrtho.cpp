

/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "glwidgetOrtho.h"
#include "NodeBeam.h"
#include <QInputDialog>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidgetOrtho::GLWidgetOrtho(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 1440;
    ViewHeight = 2.0;
    AddingNodes=0;
    MovingNodes=0;
    Zcoordinate=0;
    SnapToGrid=0;
    RectSelect=0;
    GridSize=0.25;
    CurrentNodeGroup=0;
    ShowArrows=1;
    MeasuringDistance=0;
    ViewOffsetX = 0;
    ViewOffsetY = 0;
    ViewOffsetZ = 0;

    /*For scaling tool*/
    StartDistanceFromOrigin.resize(2);

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    qtGrey = QColor::fromCmykF(0.25, 0.25, 0.25, 0.0);
    setMouseTracking(1);
    //setAutoFillBackground(false);

    //Top view by default
    CurrentViewMode=VIEW_TOP;
    XPointer2 = &YPointer1;
    YPointer2 = &XPointer1;
    ZPointer2 = &ZPointer1;
    inverse_X = 1;
    inverse_Y = 1;

    //blueprint
    for(int i=0; i<6; i++)
    {
        blueprint_scale[i] = 5.0;
        blueprint_opa[i] = 1.0f;
    }
    textureid = 0;
    blueprint_file.resize(6);

    blueprint_viewrot.resize(6);
    blueprint_viewrot[0].setX(0);
    blueprint_viewrot[0].setY(0);
    blueprint_viewrot[0].setZ(270);
    blueprint_viewrot[1].setX(0);
    blueprint_viewrot[1].setY(180);
    blueprint_viewrot[1].setZ(270);
    blueprint_viewrot[2].setX(90);
    blueprint_viewrot[2].setY(180);
    blueprint_viewrot[2].setZ(0);
    blueprint_viewrot[3].setX(90);
    blueprint_viewrot[3].setY(0);
    blueprint_viewrot[3].setZ(0);
    blueprint_viewrot[4].setX(0);
    blueprint_viewrot[4].setY(90);
    blueprint_viewrot[4].setZ(90);
    blueprint_viewrot[5].setX(0);
    blueprint_viewrot[5].setY(270);
    blueprint_viewrot[5].setZ(270);

    pii = 2*qAsin(1);

    backgroundcolor.resize(4);
    backgroundcolor[0] = 0.15;
    backgroundcolor[1] = 0.15;
    backgroundcolor[2] = 0.15;
    backgroundcolor[3] = 0.15;
    gridcolor.resize(4);
    gridcolor[0] = 1.0;
    gridcolor[1] = 1.0;
    gridcolor[2] = 1.0;
    gridcolor[3] = 1.0;

}

GLWidgetOrtho::~GLWidgetOrtho()
{
    delete NBPointer;
}

QSize GLWidgetOrtho::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidgetOrtho::sizeHint() const
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

void GLWidgetOrtho::setViewTop()
{
    xRot = 0;
    yRot = 0;
    zRot = 1440;
    updateGL();
    CurrentViewMode = VIEW_TOP;
    XPointer2 = &YPointer1;
    YPointer2 = &XPointer1;
    ZPointer2 = &ZPointer1;
    inverse_X = 1;
    inverse_Y = 1;
    textureid=0;
}
void GLWidgetOrtho::setViewBottom()
{
    xRot = 2880;
    yRot = 0;
    zRot = 1440;
    updateGL();
    CurrentViewMode = VIEW_BOTTOM;
    XPointer2 = &YPointer1;
    YPointer2 = &XPointer1;
    ZPointer2 = &ZPointer1;
    inverse_X = 1;
    inverse_Y = 0;
    textureid=1;
}

void GLWidgetOrtho::setViewFront()
{
    xRot = 1440;
    yRot = 2880;
    zRot = 0;
    updateGL();
    CurrentViewMode = VIEW_FRONT;
    XPointer2 = &XPointer1;
    YPointer2 = &ZPointer1;
    ZPointer2 = &YPointer1;
    inverse_X = 1;
    inverse_Y = 1;
    textureid=2;

}

void GLWidgetOrtho::setViewBack()
{
    xRot = 1440;
    yRot = 2880;
    zRot = 2880;
    updateGL();
    CurrentViewMode = VIEW_BACK;
    XPointer2 = &XPointer1;
    YPointer2 = &ZPointer1;
    ZPointer2 = &YPointer1;
    inverse_X = 0;
    inverse_Y = 1;
    textureid=3;

}

void GLWidgetOrtho::setViewRight()
{
    xRot = -1440;
    yRot = 0;
    zRot = -1440;
    updateGL();
    CurrentViewMode = VIEW_RIGHT;
    XPointer2 = &ZPointer1;
    YPointer2 = &XPointer1;
    ZPointer2 = &YPointer1;
    inverse_X = 0;
    inverse_Y = 1;
    textureid=4;
}

void GLWidgetOrtho::setViewLeft()
{
    xRot = -1440;
    yRot = 0;
    zRot = 1440;
    updateGL();
    CurrentViewMode = VIEW_LEFT;
    XPointer2 = &ZPointer1;
    YPointer2 = &XPointer1;
    ZPointer2 = &YPointer1;
    inverse_X = 1;
    inverse_Y = 1;
    textureid=5;
}

//! [6]
void GLWidgetOrtho::draw()
{
    /* Light positions */
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

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor4f(0.25f,0.25f,0.25f,0.2f);
    if((CurrentViewMode==VIEW_TOP) || (CurrentViewMode==VIEW_BOTTOM) )
    {

    }
    else if((CurrentViewMode==VIEW_LEFT) || (CurrentViewMode==VIEW_RIGHT) )
    {
        glRotatef(90,0,1,0);
    }
    else if((CurrentViewMode==VIEW_BACK) || (CurrentViewMode==VIEW_FRONT) )
    {
        glRotatef(90,1,0,0);
    }

    if(SnapToGrid)
    {
        int lineamount = 10/GridSize;
        glColor4f(0.75*gridcolor[0],0.75*gridcolor[1],0.75*gridcolor[2],gridcolor[3]);
        glLineWidth(1);
        glBegin(GL_LINES);
        for (int i = -lineamount; i <= lineamount; i++) {
            if (i != 0) {
                glVertex3d(x_min, (double)(GridSize*i), 0.0);
                glVertex3d(x_max, (double)(GridSize*i), 0.0);
            }
        }
        for (int i = -lineamount; i <= lineamount; i++) {
            if (i != 0) {
                glVertex3d((double)(GridSize*i), y_min, 0.0);
                glVertex3d((double)(GridSize*i), y_max, 0.0);
            }
        }
        glEnd();
    }

    glColor4f(gridcolor[0],gridcolor[1],gridcolor[2],gridcolor[3]);
    glLineWidth(3);
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

    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3d(0, y_min, 0);
    glVertex3d(0, y_max, 0);
    glVertex3d(x_min, 0, 0);
    glVertex3d(x_max, 0, 0);

    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);

//    glBegin(GL_TRIANGLES);
//    glVertex3f(-0.1, -0.1, -0.1);
//    glVertex3f(-0.1, -0.1, 0.1);
//    glVertex3f(0.1, -0.1, -0.1);

//    glVertex3f(-0.1, -0.1, +0.1);
//    glVertex3f(0.1, -0.1, 0.1);
//    glVertex3f(0.1, -0.1, -0.1);
//    glEnd();

    //Beams
    glColor4f(0.8f,0.4f,0.01f,1.0f);
    glLineWidth(3);
    glBegin(GL_LINES);
    int NodeAmount = NBPointer->Nodes.size();
    for(int i=0; i<NBPointer->Beams.size(); i++)
    {
        if(NBPointer->Beams[i].draw)
        {
            if(NBPointer->Beams[i].HasBeamDefs)
            {
                float beamred = NBPointer->BeamDefaults[NBPointer->Beams[i].BeamDefsID].RGB_Color[0]/255.0f;
                float beamgre = NBPointer->BeamDefaults[NBPointer->Beams[i].BeamDefsID].RGB_Color[1]/255.0f;
                float beamblu = NBPointer->BeamDefaults[NBPointer->Beams[i].BeamDefsID].RGB_Color[2]/255.0f;
                glColor4f(beamred,beamgre,beamblu,1.0f);
            }
            else
            {
                glColor4f(0.0f,0.4f,0.6f,1.0f);
            }
            int Bnode1 = NBPointer->Beams[i].Node1GlobalID;
            int Bnode2 = NBPointer->Beams[i].Node2GlobalID;

            if((Bnode1<NodeAmount) && (Bnode2<NodeAmount))
            {
                glVertex3f(NBPointer->Nodes[Bnode1].locX, NBPointer->Nodes[Bnode1].locY, NBPointer->Nodes[Bnode1].locZ);
                glVertex3f(NBPointer->Nodes[Bnode2].locX, NBPointer->Nodes[Bnode2].locY, NBPointer->Nodes[Bnode2].locZ);
            }

        }

    }
    glEnd();

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

    //Nodes
    glPointSize(10);
    glBegin(GL_POINTS);
    int i3 = 0;
    for(int i2=0; i2<NBPointer->NodeGroups.size();i2++)
    {
        for(int i=0; i<NBPointer->NodeGroups[i2].NodeAmount; i++)
        {
            if(NBPointer->NodeGroups[i2].draw)
            {
                if(NBPointer->Nodes[i3].GlobalID == NBPointer->ActiveNode) glColor4f(1.0f,0.0f,0.0f,1.0f);
                else glColor4f(0.3f,0.7f,0.7f,1.0f);

                glVertex3f(NBPointer->Nodes[i3].locX, NBPointer->Nodes[i3].locY, NBPointer->Nodes[i3].locZ);
            }
            i3++;
        }
    }
    glEnd();

}

void GLWidgetOrtho::initializeGL()
{
    //qglClearColor(qtGrey.darker());
    glClearColor(backgroundcolor[0], backgroundcolor[1], backgroundcolor[2], backgroundcolor[3]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_COLOR_MATERIAL);
    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    //static GLfloat lightPosition2[4] = { 0.5, 5.0, -7.0, -1.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    setAutoBufferSwap(true);
    //LoadBlueprint();
}

/* Draw rectangle for rect selection tool */
void GLWidgetOrtho::DrawRect()
{
    float nolla = 0;
    ZPointer1 = &nolla;

    XPointer1 = &Rx0;
    YPointer1 = &Ry0;
    float DrawX0 = **XPointer2;
    float DrawY0 = **YPointer2;
    float DrawZ0 = **ZPointer2;

    XPointer1 = &Rx1;
    YPointer1 = &Ry1;
    float DrawX1 = **XPointer2;
    float DrawY1 = **YPointer2;
    float DrawZ1 = **ZPointer2;

    //qDebug() << DrawX0 << " X, " << DrawX1;
    //qDebug() << DrawY0 << " Y, " << DrawY1;

    glLineStipple(1, 0x3F07);
    glEnable(GL_LINE_STIPPLE);

    glColor4f(0.75f,0.75f,0.75f,0.2f);
    glLineWidth(1);
    glBegin(GL_LINES);

    glVertex3d(DrawX0, DrawY0, DrawZ0);
    glVertex3d(DrawX0, DrawY1, DrawZ0);

    glVertex3d(DrawX1, DrawY0, DrawZ0);
    glVertex3d(DrawX1, DrawY1, DrawZ0);

    glVertex3d(DrawX0, DrawY0, DrawZ0);
    glVertex3d(DrawX1, DrawY0, DrawZ0);

    glVertex3d(DrawX1, DrawY1, DrawZ0);
    glVertex3d(DrawX0, DrawY1, DrawZ0);

    glVertex3d(DrawX0, DrawY0, DrawZ1);
    glVertex3d(DrawX0, DrawY1, DrawZ1);

    glVertex3d(DrawX1, DrawY0, DrawZ1);
    glVertex3d(DrawX1, DrawY1, DrawZ1);

    glVertex3d(DrawX0, DrawY0, DrawZ1);
    glVertex3d(DrawX1, DrawY0, DrawZ1);

    glVertex3d(DrawX1, DrawY1, DrawZ1);
    glVertex3d(DrawX0, DrawY1, DrawZ1);

    glEnd();


    glDisable(GL_LINE_STIPPLE);
}


float GLWidgetOrtho::ScreenXtoSceneX(float ScreenX)
{
    double viewscalefactor = ViewHeight/yHeight;
    float sceneX;
    sceneX = 2*viewscalefactor*(ScreenX-(xWidth*0.5));
    return sceneX;
}

void GLWidgetOrtho::paintGL()
{
    glClearColor(backgroundcolor[0], backgroundcolor[1], backgroundcolor[2], backgroundcolor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(ViewOffsetX, ViewOffsetY, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    if(blueprint_file[textureid].length()>0) DrawBlueprint();
    draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(255, 255, 255, 20);
    if(ShowArrows) DrawAxisArrows();
    DrawRect();
    //glRectf(Rx0, Ry0, Rx1, Ry1);
    glDisable(GL_BLEND);

    glColor3f(0.6, 0.6, 0.6);
    if(TextOverlay.length()>0) renderText(10, yHeight-20, TextOverlay, QFont(  "Arial", 14, QFont::Bold, 0 ) );
    //QGLWidget::swapBuffers();
    //renderText(10, yHeight-20, "TextOverlay", QFont( "courier", 10, QFont::Normal, 0 ) );


}

void GLWidgetOrtho::resizeGL(int width, int height)
{
    xWidth = width;
    yHeight = height;
    //int side = qMin(width, height);
    //glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat(width) / height)*ViewHeight;
    glOrtho(-x, x, -ViewHeight, +ViewHeight, 4.0, 35.0);

    glMatrixMode(GL_MODELVIEW);

}

void GLWidgetOrtho::mousePressEvent(QMouseEvent *event)
{
    if(AddingNodes)
    {
        if (event->buttons() & Qt::LeftButton) {
            if(SnapToGrid)
            {
                double viewscalefactor = ViewHeight/yHeight;
                float locx = 2*viewscalefactor*(event->x()-(xWidth*0.5))-ViewOffsetX;
                float locy = 2*viewscalefactor*(event->y()-(yHeight*0.5))+ViewOffsetY;
                float tempx;
                float tempy;
                int lineamount = 10/GridSize;

                for (int i = -lineamount; i <= lineamount; i++) {

                    if(locx < (GridSize*i))
                    {
                        tempx=i;
                        if((locx+GridSize*0.5)<i*GridSize) tempx = tempx-1;
                        locx = tempx*GridSize;
                        break;
                    }
                }
                for (int i = -lineamount; i <= lineamount; i++) {

                    if(locy < (GridSize*i))
                    {
                        tempy=i;
                        if((locy+GridSize*0.5)<i*GridSize) tempy = tempy-1;
                        locy = tempy*GridSize;
                        break;
                    }
                }

                if(inverse_X) locx = -locx;
                if(inverse_Y) locy = -locy;
                XPointer1 = &locx;
                YPointer1 = &locy;
                ZPointer1 = &Zcoordinate;

                int Node_id = NBPointer->AddNodeT(**XPointer2, **YPointer2, **ZPointer2,CurrentNodeGroup, NBPointer->CurrentNamePrefix);
                emit NodeBeamUpdated();
                emit JBEAM_AddNodeO();
                updateGL();

            }
            else
            {

                double viewscalefactor = ViewHeight/yHeight;

                float locx = 2*viewscalefactor*(event->x()-(xWidth*0.5))-ViewOffsetX;
                float locy = 2*viewscalefactor*(event->y()-(yHeight*0.5))+ViewOffsetY;

                if(inverse_X) locx = -locx;
                if(inverse_Y) locy = -locy;
                XPointer1 = &locx;
                YPointer1 = &locy;
                ZPointer1 = &Zcoordinate;

                int Node_id = NBPointer->AddNodeT(**XPointer2, **YPointer2, **ZPointer2,CurrentNodeGroup, NBPointer->CurrentNamePrefix);
                emit NodeBeamUpdated();
                emit JBEAM_AddNodeO();
                updateGL();

            }

        }

    }
    else if(RectSelect)
    {
        if(!GotFirstOne)
        {
            if (event->buttons() & Qt::LeftButton) {
                double viewscalefactor = ViewHeight/yHeight;
                Rx0 = 2*viewscalefactor*(event->x()-(xWidth*0.5))-ViewOffsetX;
                Ry0 = 2*viewscalefactor*(event->y()-(yHeight*0.5))+ViewOffsetY;
                qDebug() << Rx0 << " x0, y0 " << Ry0;
                if(inverse_X) Rx0 = -Rx0;
                if(inverse_Y) Ry0 = -Ry0;

                GotFirstOne=1;
            }
        }

    }
    else if(MeasuringDistance)
    {
        if(!GotFirstOne)
        {
            if (event->buttons() & Qt::LeftButton) {
                double viewscalefactor = ViewHeight/yHeight;
                Rx0 = 2*viewscalefactor*(event->x()-(xWidth*0.5));
                Ry0 = 2*viewscalefactor*(event->y()-(yHeight*0.5));
                qDebug() << Rx0 << " x0, y0 " << Ry0;
                if(inverse_X) Rx0 = -Rx0;
                if(inverse_Y) Ry0 = -Ry0;

                GotFirstOne=1;
            }
        }
    }
    else if(MovingNodes>0)
    {
        movement_x_calc=0;
        movement_y_calc=0;
        gridstep=1;
    }
    else if(ScalingNodes>0)
    {
        qDebug()<<"Setting scaling relative point";
        if (event->buttons() & Qt::LeftButton) {
            OriginalLocationsX.clear();
            OriginalLocationsY.clear();
            OriginalLocationsZ.clear();
            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                OriginalLocationsX.resize(OriginalLocationsX.size()+1);
                OriginalLocationsY.resize(OriginalLocationsY.size()+1);
                OriginalLocationsZ.resize(OriginalLocationsZ.size()+1);
                OriginalLocationsX[i5] = NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX;
                OriginalLocationsY[i5] = NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY;
                OriginalLocationsZ[i5] = NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ;
            }
            float distance_x = event->x()-(xWidth*0.5f);
            float distance_y = event->y()-(yHeight*0.5f);
            StartDistanceFromOrigin[0] = sqrt((distance_x*distance_x+distance_y*distance_y));

        }
    }
    else if(RotatingNodes>0)
    {
        if (event->buttons() & Qt::LeftButton) {
            OriginalLocationsX.clear();
            OriginalLocationsY.clear();
            OriginalLocationsZ.clear();
            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                OriginalLocationsX.resize(OriginalLocationsX.size()+1);
                OriginalLocationsY.resize(OriginalLocationsY.size()+1);
                OriginalLocationsZ.resize(OriginalLocationsZ.size()+1);
                OriginalLocationsX[i5] = NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX;
                OriginalLocationsY[i5] = NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY;
                OriginalLocationsZ[i5] = NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ;
            }

            float distance_x = event->x()-(xWidth*0.5f);
            float distance_y = event->y()-(yHeight*0.5f);
            float distance_t = sqrt((distance_x*distance_x+distance_y*distance_y));

            float cos_alfa = qAcos(distance_x/distance_t);
            float sin_alfa = qAsin(distance_y/distance_t);

            //float pii = 355.0f/113.0f;

            if(sin_alfa>0)
            {
                StartDistanceFromOrigin[0] = cos_alfa;
            }
            else
            {
                StartDistanceFromOrigin[0] = (2*pii)-cos_alfa;
            }

        }
    }
    lastPos = event->pos();
}

void GLWidgetOrtho::mouseReleaseEvent(QMouseEvent *event)
{
    if(RectSelect)
    {
        double viewscalefactor = ViewHeight/yHeight;

        Rx1 = 2*viewscalefactor*(event->x()-(xWidth*0.5))-ViewOffsetX;
        Ry1 = 2*viewscalefactor*(event->y()-(yHeight*0.5))+ViewOffsetY;

        if(inverse_X) Rx1 = -Rx1;
        if(inverse_Y) Ry1 = -Ry1;

        GotFirstOne=0;
        float tempf;
        if(Rx0>Rx1)
        {
            tempf = Rx1;
            Rx1=Rx0;
            Rx0=tempf;
        }
        if(Ry0>Ry1)
        {
            tempf = Ry1;
            Ry1=Ry0;
            Ry0=tempf;

        }

        float depth = -99;
        XPointer1 = &Rx0;
        YPointer1 = &Ry0;
        ZPointer1 = &depth;
        float correctedX0 = **XPointer2;
        float correctedY0 = **YPointer2;
        float correctedZ0 = **ZPointer2;

        float depth2 = 99;
        XPointer1 = &Rx1;
        YPointer1 = &Ry1;
        ZPointer1 = &depth2;
        float correctedX1 = **XPointer2;
        float correctedY1 = **YPointer2;
        float correctedZ1 = **ZPointer2;

        NBPointer->SelectNodesLoc(correctedX0,correctedY0,correctedZ0,correctedX1,correctedY1,correctedZ1);

        emit SelectionUpdated();

        Rx0=0;
        Ry0=0;
        Rx1=0;
        Ry1=0;
    }
    else if(MeasuringDistance)
    {
        double viewscalefactor = ViewHeight/yHeight;

        Rx1 = 2*viewscalefactor*(event->x()-(xWidth*0.5));
        Ry1 = 2*viewscalefactor*(event->y()-(yHeight*0.5));

        if(inverse_X) Rx1 = -Rx1;
        if(inverse_Y) Ry1 = -Ry1;

        float depth = 0;
        XPointer1 = &Rx0;
        YPointer1 = &Ry0;
        ZPointer1 = &depth;
        distance_1.setX(**XPointer2);
        distance_1.setY(**YPointer2);
        distance_1.setZ(**ZPointer2);

        XPointer1 = &Rx1;
        YPointer1 = &Ry1;
        distance_2.setX(**XPointer2);
        distance_2.setY(**YPointer2);
        distance_2.setZ(**ZPointer2);

        GotFirstOne=0;

        Rx0=0;
        Ry0=0;
        Rx1=0;
        Ry1=0;

        float distx = distance_1.x() - distance_2.x();
        float disty = distance_1.y() - distance_2.y();
        float distz = distance_1.z() - distance_2.z();
        float distr = sqrt(distx*distx + disty*disty + distz+distz);
        qDebug()<< "Distance is " << distr/(2*viewscalefactor);

        if(SetScaleByDistance)
        {
            bool ok;
            float realdist = QInputDialog::getDouble(this, tr("QInputDialog::getText()"),
                                                     tr("Actual known distance:"), 1.0,
                                                     0, 1000, 5,&ok);
            if(ok)
            {
                //Scalefactor
                float factor = realdist/distr;

                blueprint_scale[textureid] = blueprint_scale[textureid]*factor;


            }
        }


    }
}

void GLWidgetOrtho::MovingNodes_CalculateMove(QMouseEvent *event)
{
    if(SnapToGrid)
    {
        double viewscalefactor = 2*(ViewHeight/yHeight);
        double step = GridSize/viewscalefactor;
        qDebug() << "step is " << step;

        movement_x = lastPos.x();
        movement_y = lastPos.y();
        movement_x  -= event->x();
        movement_y  -= event->y();

        movement_x_calc += movement_x;
        movement_y_calc += movement_y;
        qDebug() << "movement_x_calc " << movement_x_calc;

        if(movement_x_calc>= step)
        {
            movement_x = step;
            movement_x_calc = 0;
        }
        else if(movement_x_calc<= -step)
        {
            movement_x = -step;
            movement_x_calc = 0;
        }
        else
        {
            movement_x = 0;
        }

        if(movement_y_calc>= step)
        {
            movement_y = step;
            movement_y_calc = 0;
        }
        else if(movement_y_calc<= -step)
        {
            movement_y = -step;
            movement_y_calc = 0;
        }
        else
        {
            movement_y = 0;
        }

    }
    else
    {
        movement_x = lastPos.x();
        movement_y = lastPos.y();
        movement_x  -= event->x();
        movement_y  -= event->y();
    }
}

void GLWidgetOrtho::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if(RectSelect==1)
        {
            double viewscalefactor = ViewHeight/yHeight;

            Rx1 = 2*viewscalefactor*(event->x()-(xWidth*0.5))-ViewOffsetX;
            Ry1 = 2*viewscalefactor*(event->y()-(yHeight*0.5))+ViewOffsetY;
            if(inverse_X) Rx1 = -Rx1;
            if(inverse_Y) Ry1 = -Ry1;

            updateGL();
        }
        else if(MovingNodes==1)
        {
            double viewscalefactor = 2*(ViewHeight/yHeight);

            MovingNodes_CalculateMove(event);

            if(inverse_X) movement_x = -movement_x;
            if(inverse_Y) movement_y = -movement_y;

            float movement_z = 0;
            XPointer1 = &movement_x;
            YPointer1 = &movement_y;
            ZPointer1 = &movement_z;


            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX += -viewscalefactor*(**XPointer2);
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY += -viewscalefactor*(**YPointer2);
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ += -viewscalefactor*(**ZPointer2);
            }
            updateGL();
        }
        else if(MovingNodes==2)
        {
            double viewscalefactor = 2*(ViewHeight/yHeight);

            MovingNodes_CalculateMove(event);

            if(inverse_X) movement_x = -movement_x;
            if(inverse_Y) movement_y = -movement_y;

            float movement_z = 0;
            XPointer1 = &movement_x;
            YPointer1 = &movement_y;
            ZPointer1 = &movement_z;

            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX += -viewscalefactor*(**XPointer2);
            }
            updateGL();
        }
        else if(MovingNodes==3)
        {
            double viewscalefactor = 2*(ViewHeight/yHeight);

            MovingNodes_CalculateMove(event);

            if(inverse_X) movement_x = -movement_x;
            if(inverse_Y) movement_y = -movement_y;

            float movement_z = 0;
            XPointer1 = &movement_x;
            YPointer1 = &movement_y;
            ZPointer1 = &movement_z;

            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY += -viewscalefactor*(**YPointer2);
            }
            updateGL();
        }
        else if(MovingNodes==4)
        {
            double viewscalefactor = 2*(ViewHeight/yHeight);

            MovingNodes_CalculateMove(event);

            if(inverse_X) movement_x = -movement_x;
            if(inverse_Y) movement_y = -movement_y;

            float movement_z = 0;
            XPointer1 = &movement_x;
            YPointer1 = &movement_y;
            ZPointer1 = &movement_z;

            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                    NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ += -viewscalefactor*(**ZPointer2);
            }
            updateGL();
        }
        else if(ScalingNodes==1)
        {
            float scalefactor;
            float scalefactor_x  = (event->x()-(xWidth*0.5f));
            float scalefactor_y  = (event->y()-(yHeight*0.5f));

            scalefactor = sqrt((scalefactor_x*scalefactor_x + scalefactor_y*scalefactor_y));
            scalefactor = scalefactor/StartDistanceFromOrigin[0];

            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX = OriginalLocationsX[i5]*(scalefactor);
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY = OriginalLocationsY[i5]*(scalefactor);
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ = OriginalLocationsZ[i5]*(scalefactor);

            }
            updateGL();
        }
        else if(ScalingNodes==2)
        {
            float scalefactor;
            float scalefactor_x  = (event->x()-(xWidth*0.5f));
            float scalefactor_y  = (event->y()-(yHeight*0.5f));

            scalefactor = sqrt((scalefactor_x*scalefactor_x + scalefactor_y*scalefactor_y));
            scalefactor = scalefactor/StartDistanceFromOrigin[0];

            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locX = OriginalLocationsX[i5]*(scalefactor);

            }
            updateGL();
        }
        else if(ScalingNodes==3)
        {
            float scalefactor;
            float scalefactor_x  = (event->x()-(xWidth*0.5f));
            float scalefactor_y  = (event->y()-(yHeight*0.5f));

            scalefactor = sqrt((scalefactor_x*scalefactor_x + scalefactor_y*scalefactor_y));
            scalefactor = scalefactor/StartDistanceFromOrigin[0];

            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locY = OriginalLocationsY[i5]*(scalefactor);

            }
            updateGL();
        }
        else if(ScalingNodes==4)
        {
            float scalefactor;
            float scalefactor_x  = (event->x()-(xWidth*0.5f));
            float scalefactor_y  = (event->y()-(yHeight*0.5f));

            scalefactor = sqrt((scalefactor_x*scalefactor_x + scalefactor_y*scalefactor_y));
            scalefactor = scalefactor/StartDistanceFromOrigin[0];

            for(int i5=0; i5<NBPointer->SelectedNodes.size(); i5++)
            {
                NBPointer->Nodes[NBPointer->SelectedNodes[i5]].locZ = OriginalLocationsZ[i5]*(scalefactor);

            }
            updateGL();
        }
        else if(RotatingNodes==1)
        {
            float distance_x = event->x()-(xWidth*0.5f);
            float distance_y = event->y()-(yHeight*0.5f);
            float distance_t = sqrt((distance_x*distance_x+distance_y*distance_y));

            float cos_alfa = qAcos(distance_x/distance_t);
            float sin_alfa = qAsin(distance_y/distance_t);

            //float pii = 355.0f/113.0f;

            float angle;
            if(sin_alfa>0)
            {
                angle = cos_alfa;
            }
            else
            {
                angle = (2*pii)-cos_alfa;
            }

            angle = StartDistanceFromOrigin[0] - angle;

            if(CurrentViewMode == VIEW_LEFT) angle = -angle;

            //Rounding to 0.1 degrees resolution
            angle = angle*(360.0f/(2*pii));
            QString angle2 = QString::number(angle, 'f', 1);
            angle = angle2.toFloat();
            angle = angle*((2*pii)/360.0f);

            float a1 = qCos(angle);
            float b1 = qSin(angle);
            float a2 = qSin(angle);
            float b2 = qCos(angle);

            for(int i2=0; i2<NBPointer->SelectedNodes.size();i2++)
            {
                float XCoordinate = OriginalLocationsY[i2];
                float YCoordinate = OriginalLocationsZ[i2];
                NBPointer->Nodes[NBPointer->SelectedNodes[i2]].locY = a1*XCoordinate - b1*YCoordinate;
                NBPointer->Nodes[NBPointer->SelectedNodes[i2]].locZ = a2*XCoordinate + b2*YCoordinate;
            }
            updateGL();
            QString Message = "Rotating X: ";
            Message.append(QString::number((angle*360.0f/(2*pii))));
            TextOverlay = Message;
            qDebug()<<"Angle is " << angle;
        }
        else if(RotatingNodes==2)
        {
            float distance_x = event->x()-(xWidth*0.5f);
            float distance_y = event->y()-(yHeight*0.5f);
            float distance_t = sqrt((distance_x*distance_x+distance_y*distance_y));

            float cos_alfa = qAcos(distance_x/distance_t);
            float sin_alfa = qAsin(distance_y/distance_t);

            //float pii = 355.0f/113.0f;

            float angle;
            if(sin_alfa>0)
            {
                angle = cos_alfa;
            }
            else
            {
                angle = (2*pii)-cos_alfa;
            }

            angle = StartDistanceFromOrigin[0] - angle;

            if(CurrentViewMode == VIEW_FRONT) angle = -angle;

            //Rounding to 0.1 degrees resolution
            angle = angle*(360.0f/(2*pii));
            QString angle2 = QString::number(angle, 'f', 1);
            angle = angle2.toFloat();
            angle = angle*((2*pii)/360.0f);

            float a1 = qCos(angle);
            float b1 = qSin(angle);
            float a2 = qSin(angle);
            float b2 = qCos(angle);

            for(int i2=0; i2<NBPointer->SelectedNodes.size();i2++)
            {
                float XCoordinate = OriginalLocationsX[i2];
                float YCoordinate = OriginalLocationsZ[i2];
                NBPointer->Nodes[NBPointer->SelectedNodes[i2]].locX = a1*XCoordinate - b1*YCoordinate;
                NBPointer->Nodes[NBPointer->SelectedNodes[i2]].locZ = a2*XCoordinate + b2*YCoordinate;
            }
            updateGL();
            QString Message = "Rotating Y: ";
            Message.append(QString::number((angle*360.0f/(2*pii))));
            TextOverlay = Message;
            qDebug()<<"Angle is " << angle;
        }
        else if(RotatingNodes==3)
        {
            float distance_x = event->x()-(xWidth*0.5f);
            float distance_y = event->y()-(yHeight*0.5f);
            float distance_t = sqrt((distance_x*distance_x+distance_y*distance_y));

            float cos_alfa = qAcos(distance_x/distance_t);
            float sin_alfa = qAsin(distance_y/distance_t);

            //float pii = 355.0f/113.0f;

            float angle;
            if(sin_alfa>0)
            {
                angle = cos_alfa;
            }
            else
            {
                angle = (2*pii)-cos_alfa;
            }

            angle = StartDistanceFromOrigin[0] - angle;

            if(CurrentViewMode == VIEW_BOTTOM) angle = -angle;

            //Rounding to 0.1 degrees resolution
            angle = angle*(360.0f/(2*pii));
            QString angle2 = QString::number(angle, 'f', 0);
            angle = angle2.toFloat();

            angle = angle*((2*pii)/360.0f);



            float a1 = qCos(angle);
            float b1 = qSin(angle);
            float a2 = qSin(angle);
            float b2 = qCos(angle);

            for(int i2=0; i2<NBPointer->SelectedNodes.size();i2++)
            {
                float XCoordinate = OriginalLocationsX[i2];
                float YCoordinate = OriginalLocationsY[i2];
                NBPointer->Nodes[NBPointer->SelectedNodes[i2]].locX = a1*XCoordinate - b1*YCoordinate;
                NBPointer->Nodes[NBPointer->SelectedNodes[i2]].locY = a2*XCoordinate + b2*YCoordinate;
            }
            updateGL();
            QString Message = "Rotating Z: ";
            Message.append(QString::number((angle*360.0f/(2*pii))));
            TextOverlay = Message;
        }

    }
    else if (event->buttons() & Qt::RightButton)
    {

        //Panning the view
        ViewOffsetX = ViewOffsetX + 2*(ViewHeight/yHeight)*(event->x() - lastPos.x());
        ViewOffsetY = ViewOffsetY + -2*(ViewHeight/yHeight)*(event->y() - lastPos.y());

        qDebug() << "näkymä siirtyy: " << ViewOffsetX << ", " << ViewOffsetY;
        updateGL();

    }
    //qDebug()<< "ortogonal view "<< event->pos();
    lastPos = event->pos();
}

void GLWidgetOrtho::setNBPointer(NodeBeam* NBPointer1)
{
    NBPointer = NBPointer1;

}

void GLWidgetOrtho::DrawAxisArrows()
{

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    //X-Arrow
    glVertex3f(0.9f, -0.05f, 0.05f);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9f, 0.05f, 0.05f);

    glVertex3f(0.9f, 0.05f, -0.05f);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9f, -0.05f, -0.05f);

    glVertex3f(0.9f, -0.05f, -0.05f);
    glVertex3f(1, 0, 0);
    glVertex3f(0.9f, -0.05f, 0.05f);

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

    glVertex3f(0.05f, 0.1f, 0.05f);
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

void GLWidgetOrtho::DrawBlueprint()
{

    float scalef1 = blueprint_h[textureid]/blueprint_w[textureid];
    float height = scalef1*blueprint_scale[textureid];
    float width = blueprint_scale[textureid];
    float OffsetX = -width/2;
    float OffsetY = height/2;
    float OffsetZ = -15;
    if((textureid % 2) != 0) OffsetZ = 15;

    OffsetX += blueprint_offX[textureid];
    OffsetY += blueprint_offY[textureid];

    if(inverse_X) OffsetX = -OffsetX;
    if(inverse_Y) OffsetY = -OffsetY;

    XPointer1 = &OffsetX;
    YPointer1 = &OffsetY;
    ZPointer1 = &OffsetZ;

    //blueprint_offX[textureid];

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //push matrix, to not rotate and transform all objects in scene
    glPushMatrix();


    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, blueprint_opa[textureid]);

    //move it to correct position
    glTranslatef(**XPointer2, **YPointer2, **ZPointer2);

    //rotate the blueprint
    glRotatef(blueprint_viewrot[textureid].x(), 1, 0, 0);
    glRotatef(blueprint_viewrot[textureid].y(), 0, 1, 0);
    glRotatef(blueprint_viewrot[textureid].z(), 0, 0, 1);

    glBindTexture(GL_TEXTURE_2D, texture[textureid]);
    glBegin(GL_QUADS);

    //Draw textured quad for the blueprint
    glNormal3f(0,0,1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0,  0.0f);

    glNormal3f(0,0,1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( width, 0,  0.0f);

    glNormal3f(0,0,1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( width,  height,  0.0f);

    glNormal3f(0,0,1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0,  height,  0.0f);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);

    glPopMatrix();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void GLWidgetOrtho::LoadBlueprint(QString fileName)
{
    //glDisable(GL_LIGHTING);
    //glDisable(GL_LIGHT0);

    blueprint_file[textureid] = fileName;

    QImage t;
    QImage b;

    if (!b.load(fileName))
    {
        qDebug()<<"fail";
    }
    else
    {
        t = QGLWidget::convertToGLFormat( b );
        blueprint_w[textureid] = t.width();
        blueprint_h[textureid] = t.height();
        glGenTextures( 1, &texture[textureid] );
        glBindTexture( GL_TEXTURE_2D, texture[textureid] );
        glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }

    updateGL();
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
}

/* Draw hubwheel or wheel */
void GLWidgetOrtho::DrawWheel(float radius, float width, int rays)
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
