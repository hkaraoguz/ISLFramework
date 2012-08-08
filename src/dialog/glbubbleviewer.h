#ifndef GLBUBBLEVIEWER_H
#define GLBUBBLEVIEWER_H

#include "bubbleprocess.h"

#include <QGLWidget>
#include <QtOpenGL>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


enum DrawType{

    DRAW_TYPE_NONE = -1,
    DRAW_TYPE_DEFAULT= 0,
    DRAW_TYPE_SAVE_VIDEO= 1


};

class GLBubbleViewer : public QGLWidget
{
    Q_OBJECT
public:
    GLBubbleViewer(QWidget* parent = 0);

    void setDrawType(DrawType drawType);

    int getDrawType();

    // Points that will be drawn on the sphere
    std::vector<bubblePoint> Points;





protected:

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void timerEvent(QTimerEvent *);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent * event);





private:
        void drawSphere();

        void drawPointsAndSaveFrames();

        void drawPointsOnly();

        //void drawImage();

        float camera_fi;
        float camera_tetha;
        float camera_r;
        QPoint lastPos;

        bool there_is_a_change;

        enum DrawType drawType;
signals:
        void frameChanged(int frame);

};

#endif // GLBUBBLEVIEWER_H
