
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

        yRot = dRot*(zRot/5760);
        xRot = dRot*(1-(zRot/5760));
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
    qDebug() << gridcolor[0] << ", " << gridcolor[1] << ", " << gridcolor[2];
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
        for(int i=0; i<NBPointer->NodeGroups[i2].NodeAmount; i++)
        {
            if(NBPointer->NodeGroups[i2].draw)
            {
                if(NBPointer->Nodes[i3].GlobalID == NBPointer->ActiveNode) glColor4f(1.0f,0.0f,0.0f,1.0f);
                else glColor4f(0.4f,0.4f,0.4f,1.0f);

                glVertex3f(NBPointer->Nodes[i3].locX, NBPointer->Nodes[i3].locY, NBPointer->Nodes[i3].locZ);
            }
            i3++;
        }
    }
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
    glClearColor(backgroundcolor[0], backgroundcolor[1], backgroundcolor[2], backgroundcolor[3]);
    if(NodePicking)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.0);
        glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
        glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
        glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
        drawpicking();
        //QGLWidget::swapBuffers();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    draw(); //draw nodes, beams, wheels, lines
    if(ShowArrows) DrawAxisArrows();
    glColor3f(0.6, 0.6, 0.6);

    renderText(10, yHeight-20, TextOverlay, QFont( "Arial", 14, QFont::Bold, 0 ) );

    if(ShowNodeNumbers) RenderTextInScene(1);
    else if(ShowNodeNumbers1) RenderTextInScene(0);

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
    }
    updateGL();

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

    } else if (event->buttons() & Qt::RightButton) {

        setZRotation(zRot + 8 * dx);
        setDRotation(dRot + 8 * dy);

    }
    //qDebug()<< "3D view "<< event->pos();
    lastPos = event->pos();
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
