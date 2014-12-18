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

#pragma once

#include <QGLWidget>
#include "NodeBeam.h"
#include <QString>


class GLWidgetOrtho;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    bool RotationMode;
    bool AddingNodes;
    int AddingBeamsSingle;
    int AddingBeamsCont;
    int AddingWheels;

    bool NodePicking;
    bool ShowArrows;

    float ZoomFactor;
    int PickedNode1;

    int BeamNode1;
    int BeamNode2;
    QString TextOverlay;
    int CurrentBeamGroup;

    bool ShowNodeNumbers;
    bool ShowNodeNumbers1; //Shows actual ID's, for RoR

    int MovingNodes;
    int ScalingNodes;
    int RotatingNodes;

    double pii;

    QVector <float >backgroundcolor;
    QVector <float> gridcolor;

    float ViewOffsetX;
    float ViewOffsetY;
    float ViewOffsetZ;


public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setDRotation(int angle);
    void setZoom();
    void setNBPointer(NodeBeam *NBPointer1);



signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void NodeBeamUpdated();
    void JBEAM_AddBeamO();

protected:
    void draw();
    void drawpicking();
    void RenderTextInScene(bool names);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void DrawSphere(int segments, int diameter);


private:

    int xRot;
    int yRot;
    int zRot;
    int dRot;
    int xWidth;
    int yHeight;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
    QColor qtGrey;

    NodeBeam *NBPointer;
    //GLWidgetOrtho *glView1;

    void MovingNodes_CalculateMove(QMouseEvent *event);
    float movement_x;
    float movement_y;

    void DrawAxisArrows();
    void DrawWheel(float radius, float width, int rays);
};


