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
#include <QVector3D>
#include "NodeBeam.h"
#include "glwidget.h"

class GLWidgetOrtho : public QGLWidget
{
    Q_OBJECT

public:
    GLWidgetOrtho(QWidget *parent = 0);
    ~GLWidgetOrtho();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    bool AddingNodes;
    double ViewHeight; //in meters
    int MovingNodes;
    int ScalingNodes;
    int RotatingNodes;
    bool SnapToGrid;
    bool RectSelect;
    float GridSize;
    float Zcoordinate;
    bool ShowArrows;

    enum viewmodes {VIEW_TOP, VIEW_RIGHT, VIEW_LEFT, VIEW_BACK, VIEW_FRONT, VIEW_BOTTOM};
    int CurrentViewMode;

    int CurrentNodeGroup;

    void LoadBlueprint(QString fileName);

    //Blueprints
    int textureid;  //id indicating the current view/image in use
    float blueprint_scale[6]; //scale factors
    QVector <QString> blueprint_file; //filenames
    float blueprint_offX[6];
    float blueprint_offY[6];
    float blueprint_opa[6]; //Opacity of blueprint 0 - 1.0

    double pii;

    QVector <float >backgroundcolor;
    QVector <float> gridcolor;

    //Measuring distance
    bool MeasuringDistance;
    bool SetScaleByDistance;
    QVector3D distance_1;
    QVector3D distance_2;

    void resizeGL(int width, int height);

public slots:
    void setNBPointer(NodeBeam *NBPointer1);
    void setViewTop();
    void setViewBottom();
    void setViewFront();
    void setViewBack();
    void setViewRight();
    void setViewLeft();

signals:
    void NodeBeamUpdated();
    void SelectionUpdated();
    void JBEAM_AddNodeO();
    void JBEAM_UpdateO();


protected:
    void draw();
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    /* View rotation and sizes*/
    int xRot;
    int yRot;
    int zRot;
    int xWidth;
    int yHeight; //in pixels

    /* View offset in for panning the view */
    float ViewOffsetX;
    float ViewOffsetY;
    float ViewOffsetZ;

    double mouseRollPos;
    QPoint lastPos; //Last mouse position before current position
    QColor qtGreen;
    QColor qtPurple;
    QColor qtGrey;

    NodeBeam *NBPointer;
    //GLWidget *glView1;

    /* Rectangle selection tool */
    bool GotFirstOne;
    float Rx0;
    float Ry0;
    float Rz0;
    float Rx1;
    float Ry1;
    float Rz1;
    void DrawRect();

    /* Moving nodes */
    void MovingNodes_CalculateMove(QMouseEvent *event);
    float movement_x;
    float movement_y;
    //Snap to grid
    bool gridstep;
    float movement_x_calc;
    float movement_y_calc;

    /* Scaling tool */
    QVector <float> StartDistanceFromOrigin;
    QVector <float> OriginalLocationsX;
    QVector <float> OriginalLocationsY;
    QVector <float> OriginalLocationsZ;

    /* Screen axises */
    float * XPointer1;
    float * YPointer1;
    float * ZPointer1;

    //Actual axises are these
    float ** XPointer2;
    float ** YPointer2;
    float ** ZPointer2;

    //For different view directions, need to inverse the screencoordinates for tools sometimes
    bool inverse_X;
    bool inverse_Y;

    /* Overlay text */
    QString TextOverlay;

    float ScreenXtoSceneX(float ScreenX);

    /*Arrows indicating the axises*/
    void DrawAxisArrows();
    void DrawWheel(float radius, float width, int rays);

    /* Draw buttons */
    void DrawUI();
    void DrawUI_Button(QString text, int ButtonLocX, int ButtonLocY, float ScaleFactor);

    /* Blueprint functions */
    GLuint texture[6]; //Storage for blueprint images
    void DrawBlueprint(); //drawing function
    float blueprint_h[6]; //height in pixels
    float blueprint_w[6]; //width in pixels
    QVector <QVector3D> blueprint_viewrot;



};


