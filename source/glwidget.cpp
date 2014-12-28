
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

#include "glwidget.h"
#include "NodeBeam.h"

#include <QMatrix4x4>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

    dRot = 4510;
    zRot = 3600;
    yRot = dRot*(zRot/5760);
    xRot = dRot*(1-(zRot/5760));
    NodePicking = 0;
    PickedNode1=9999;
    AddingBeamsSingle=0;
    AddingBeamsCont=0;
    AddingWheels=0;
    CurrentBeamGroup=0;
    ZoomFactor = 1.0;
    ShowNodeNumbers = 0;
    ShowNodeNumbers1 = 0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    qtGrey = QColor::fromCmykF(0.25, 0.25, 0.25, 0.0);
    RotationMode = true;

    TextOverlay = "";

    setAutoFillBackground(false);
    setMouseTracking(true);

    ShowArrows = 1;

    pii = 2*qAsin(1);

    backgroundcolor.resize(4);
    backgroundcolor[0] = 0.15;
    backgroundcolor[1] = 0.15;
    backgroundcolor[2] = 0.15;
    backgroundcolor[3] = 0.15;
    gridcolor.resize(4);
    gridcolor[3] = 1.0;

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
}

GLWidget::~GLWidget()
{

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
        qDebug() << "kamera rotaatio " << xRot << ", " << yRot;
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

    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3d(0, y_min, 0);
    glVertex3d(0, y_max, 0);
    glVertex3d(x_min, 0, 0);
    glVertex3d(x_max, 0, 0);

    glEnd();

    /* Drawing nodes */
    glPointSize(10);
    glBegin(GL_POINTS);
    int i3 = 0;
    for(int i2=0; i2<NBPointer->NodeGroups.size();i2++)
    {
        for(int i=0; i<NBPointer->NodeGroups.at(i2).NodeAmount; i++)
        {
            if(NBPointer->NodeGroups.at(i2).draw)
            {
                if(NBPointer->Nodes.at(i3).GlobalID == NBPointer->ActiveNode) glColor4f(1.0f,0.0f,0.0f,1.0f);
                else glColor4f(0.4f,0.4f,0.4f,1.0f);

                glVertex3f(NBPointer->Nodes.at(i3).locX, NBPointer->Nodes.at(i3).locY, NBPointer->Nodes.at(i3).locZ);
            }
            i3++;
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
    if((AddingBeamsSingle==2) || (AddingBeamsCont==2))
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
    glPointSize(40);
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
                    painter.setRenderHint(QPainter::Antialiasing);
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
                    painter.setRenderHint(QPainter::Antialiasing);
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
    glClearColor(backgroundcolor[0], backgroundcolor[1], backgroundcolor[2], backgroundcolor[3]);

    /* Render picking colors in buffer and check what should be picked */
    if(NodePicking || (MovingNodes > 0) || (ScalingNodes > 0) || (RotatingNodes > 0))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.0);
        glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
        glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
        //glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
        glTranslatef(ViewOffsetX, ViewOffsetY, ViewOffsetZ); //Move 3D view around
        if(NodePicking) drawpicking(); //Draw nodes in buffer, each with individual color
        else if(MovingNodes >0) Draw3DCursor_Picking(0); //Draw move 3D cursor in buffer and check
        else if(ScalingNodes >0) Draw3DCursor_Picking(1); //Draw scale 3D cursor in buffer and check
        else if(RotatingNodes > 0) Draw3DCursor_Picking(2); //Draw rotate 3D cursor in buffer and check
        //QGLWidget::swapBuffers(); // Swap buffes to render on screen, only for testing
    }

    /* Render the actual scene itself */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0); //Rotate 3D view
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    //glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glTranslatef(ViewOffsetX, ViewOffsetY, ViewOffsetZ); //Move 3D view around
    draw(); //draw nodes, beams, wheels, lines
    if(MovingNodes > 0) Draw3DCursor();
    else if(ScalingNodes > 0) Draw3DCursor_Scale();
    else if(RotatingNodes > 0) Draw3DCursor_Rotate();
    else if(ShowArrows) DrawAxisArrows();
    glColor3f(0.6, 0.6, 0.6);  //Set text color
    renderText(10, yHeight-20, TextOverlay, QFont( "Arial", 14, QFont::Bold, 0 ) );
    if(ShowNodeNumbers) RenderTextInScene(1);
    else if(ShowNodeNumbers1) RenderTextInScene(0);
    if(RectSelect > 1) DrawRectSelect();

    QGLWidget::swapBuffers();
}

//void GLWidget::resizeGL(int width, int height)
//{
//    int side = qMin(width, height);
//    glViewport((width - side) / 2, (height - side) / 2, side, side);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
//#else
//    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
//#endif
//    glMatrixMode(GL_MODELVIEW);
//}

void GLWidget::resizeGL(int width, int height)
{
    xWidth = width;
    yHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat(width) / height)*ZoomFactor;
    glFrustum(-x, x, -ZoomFactor, ZoomFactor, 4.0, 55.0);
    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    updateGL();
    if (event->buttons() & Qt::LeftButton) {


        if(PickedNode1!=9999)
        {
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

                    NBPointer->AddBeam(BeamNode1, BeamNode2, CurrentBeamGroup);

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

                    NBPointer->AddBeam(BeamNode1, BeamNode2, CurrentBeamGroup);

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
    //qDebug() << "hiiri " << event->x() << ", " << event->y();
    if(AddingBeamsSingle==2)
    {
        updateGL();
    }
    else if(AddingBeamsCont==2)
    {
        updateGL();
    }
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

            //Rounding to 0.1 degrees resolution
            angle = angle*(360.0f/(2*pii));
            QString angletemp = QString::number(angle, 'f', 1);
            angle = angletemp.toFloat();
            angle = angle*((2*pii)/360.0f);

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
            }

            QString Message = "Rotating: ";
            Message.append(QString::number((angle*360.0f/(2*pii))));
            TextOverlay = Message;

            updateGL();
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

        qDebug() << "Left released";

    }

}

/* Mouse wheel Zoom */
void GLWidget::wheelEvent(QWheelEvent * event)
{
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
    qDebug() << "3D cursor picking " << pixel[0] << ", " << pixel[1] << "," << pixel[2];

    if(Mode == 0)
    {
        if(pixel[0] == 255) Moving3D_Mode = 1;
        else if(pixel[1] == 255) Moving3D_Mode = 2;
        else if(pixel[2] == 255) Moving3D_Mode = 3;
        else Moving3D_Mode = 0;
    }
    else if(Mode == 1)
    {
        qDebug() << "skaalaus";
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

    qDebug() << "That's it";

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
    MousePos = inverse_modelview*MousePos*7.465;

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
    qDebug() << normal_1 << normal_2 << normal_3 << normal_4;

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
