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
#include <QVector4D>

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
    bool BeamSuggestion;

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

    bool PrintPickedNodeName;

    double pii;

    QVector <float >backgroundcolor;
    QVector <float> gridcolor;

    float ViewOffsetX;
    float ViewOffsetY;
    float ViewOffsetZ;

    /* 3D edit system */
    int Moving3D_Mode; //0:none, 1:x, 2:y, 3:z
    bool Moving3D_ModeX;
    bool Moving3D_ModeY;
    bool Moving3D_ModeZ;
    float DegreeToRadiansRatio;

    int Rotating3D_Mode; //0:none, 1:x, 2:y, 3:z
    bool Rotating3D_ModeX;
    bool Rotating3D_ModeY;
    bool Rotating3D_ModeZ;

    int Scaling3D_Mode;
    bool Scaling3D_ModeX;
    bool Scaling3D_ModeY;
    bool Scaling3D_ModeZ;

    //Calculate raytrace vector for 3D rectangle selection
    QVector4D RayTraceVector(int MouseX, int MouseY);
    QVector4D campos;
    QVector4D unitvec;
    int RectSelect; //0 = RectSelect off, 1 = Waiting for user to draw a rect, 2 = Draw a rect
    QPoint RectSelect_start;
    QPoint RectSelect_end;
    QVector4D RectSel_1;
    QVector4D RectSel_2;
    QVector4D RectSel_3;
    QVector4D RectSel_4;
    bool Select_AddToSelection;
    bool ManipulateByStep;

    //Dae loading
    void LoadDae();

    //New system to add any JBEAM content type that depends of nodes
    int AddingJbeam;
    int AddingJbeam_id;
    QVector <int> AddingJbeam_pickednoes;

    bool AddingJbeam_Enable(QString keyword);
    void AddingJbeam_Disable();
    void DisableNodePicker();
    void EnableNodePicker();


    int HighlightNode;

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
    void JBEAM_AddArrayItem(int ListType_id);
    void PrintNodePicked(int node_id);

protected:
    void draw();
    void drawpicking();
    void RenderTextInScene(bool names);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
    void keyPressEvent(QKeyEvent * event);
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
    QPoint RotationStartScreen;
    QPoint RotationCenterScreen;
    QPoint ScaleStartScreen;
    QPoint ScalingCenterScreen;

    void DrawAxisArrows();
    void DrawWheel(float radius, float width, int rays);
    void Draw3DCursor();
    void Draw3DCursor_Scale();
    void Draw3DCursor_Rotate();
    void Draw3DCursor_Picking(int Mode); //Mode 0 = move, 1 = scale, 2 = rotate
    void DrawRectSelect();
    void GetViewMatrices(QMatrix4x4 * ModelviewMatrix, QMatrix4x4 * ProjectionMatrix);

};


