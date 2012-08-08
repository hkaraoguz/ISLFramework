#include "glbubbleviewer.h"
#include <math.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif
#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

#define SPHERE_RADIUS 1000

int frameCounter = 0;

int returnValtoRadius(float val){

    return (SPHERE_RADIUS + SPHERE_RADIUS*2*val);
}

GLBubbleViewer::GLBubbleViewer(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

    startTimer(10);
    setWindowTitle(tr("Sample Buffers"));

    camera_fi= 45*(3.1416/180.0);
    camera_tetha= -90*(3.1416/180.0);
    camera_r= 6000;

    there_is_a_change = true;

    drawType = DRAW_TYPE_NONE;





}
// initialize the GL Scene
void GLBubbleViewer::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 1, 1000000);
    glMatrixMode(GL_MODELVIEW);
}

// gets called when thse scene is resized
void GLBubbleViewer::resizeGL(int w, int h)
{
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1.0, 1, 1000000);
        there_is_a_change = true;

}

// gets called when the scene is updated
void GLBubbleViewer::paintGL()
{
        if(there_is_a_change){

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(60, 1.0, 1, 1000000);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(camera_r*sin(camera_fi)*cos(camera_tetha),
                                  camera_r*sin(camera_fi)*sin(camera_tetha),
                                  camera_r*cos(camera_fi),
                                  0.0,0.0,0.0,0.0,0.0,1.0);
                glEnable(GL_MULTISAMPLE);


                // Draw a sphere that represents the initial bubble surface
                drawSphere();

                // The default draw condition
                if(drawType == DRAW_TYPE_DEFAULT){

                    drawPointsOnly();


                }
                else if(drawType == DRAW_TYPE_SAVE_VIDEO)
                {


                    drawPointsAndSaveFrames();

                    emit frameChanged(0);

                    //this->drawType = DRAW_TYPE_NONE;


                }
               // there_is_a_change = false;
        }
}

void GLBubbleViewer::timerEvent(QTimerEvent *)
{
    update();
}

void GLBubbleViewer::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    there_is_a_change = true;
}

void GLBubbleViewer::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
                camera_tetha -= 0.005*dx;
                if(camera_fi - 0.005*dy>0){
                        camera_fi -= 0.005*dy;
                }
    }
    lastPos = event->pos();
        there_is_a_change = true;
}



void GLBubbleViewer::wheelEvent(QWheelEvent * event)
{
        if (event->orientation() == Qt::Vertical) {
                const int delta = event->delta();
                if((camera_r - 8*delta)>0){
                        camera_r -= 8*delta;
                }
        }
        there_is_a_change = true;
}
//angle,and direction are in degrees,radius is in milimeters.
void GLBubbleViewer::drawSphere(){


    // Draw Sphere
    GLUquadricObj *quadric=gluNewQuadric();

    gluQuadricNormals(quadric, GLU_SMOOTH);
      glColor3f(0,1,0);
      glPushMatrix();
      glTranslatef( 0,0,0 );
      gluSphere(quadric, 1000.0, 30,30);
      glPopMatrix();

     gluDeleteQuadric(quadric);

      // Coordinate system
      glBegin(GL_LINES);


          //red
          glColor3f(1,0,0);

          glVertex3f(0, 0, 0);
          glVertex3f(20000, 0, 0);

          // green
          glColor3f(0,1,0);

          glVertex3f(0, 0, 0);
          glVertex3f(0, 20000, 0);

          // blue
          glColor3f(0,0,1);

          glVertex3f(0, 0, 0);
          glVertex3f(0, 0, 20000);

       glEnd( );

       //range i çiziyor
           glBegin(GL_LINE_LOOP);
               glColor3f(1,1,0);
               glVertex3f(0.0,0.0,0.0);

               double startAngle = -1200;
               double endAngle =   1200;

               for(int i=startAngle;i<=endAngle;i++){

                   glVertex3f(2400*cos(0.0017453*i),2400*sin(0.0017453*i),0.0);

               }
           glEnd();

       //float xy = 0.5*cos(10*3.14/180);



}
void GLBubbleViewer::setDrawType(DrawType drawType){

    this->drawType = drawType;

}
int GLBubbleViewer::getDrawType(){

    return this->drawType;

}
void GLBubbleViewer::drawPointsAndSaveFrames()
{
  //  int frameCounter = 0;

        std::vector<bubblePoint> tempPoints = this->Points;

        for(unsigned long k = 0; k < tempPoints.size(); k++)
        {
            float pan = tempPoints.at(k).panAng;

            float tilt = tempPoints.at(k).tiltAng;

            float val = tempPoints.at(k).val;

            float newVal = returnValtoRadius(val);

          /*  float* x = new float[tempPoints.size()];
            float* y = new float[tempPoints.size()];
            float* z = new float[tempPoints.size()];

            float* x1 = new float[tempPoints.size()];
            float* y1 = new float[tempPoints.size()];
            float* z1 = new float[tempPoints.size()];*/

            float xy =    SPHERE_RADIUS*cos((tilt)*3.14/180);

            float x = xy*cos((pan)*3.14/180);
            float y = xy*sin((pan)*3.14/180);
            float z =  SPHERE_RADIUS*sin(tilt*3.14/180);

             xy =    newVal*cos((tilt)*3.14/180);

            float x1 = xy*cos((pan)*3.14/180);
            float y1 = xy*sin((pan)*3.14/180);
            float z1 =  newVal*sin(tilt*3.14/180);

             glColor3f(1,1,1);

            glBegin(GL_LINES);


             glVertex3f(x, y, z);

             glVertex3f(x1, y1, z1);

            glEnd();



        }

        QImage img =  this->grabFrameBuffer();

        QString path = "frames/frame";

        path.append(QString::number(frameCounter));

        img.save(path,"PNG");

        frameCounter++;




 //   }


}
void GLBubbleViewer::drawPointsOnly()
{

    std::vector<bubblePoint> tempPoints = this->Points;

    for(unsigned long k = 0; k < tempPoints.size(); k++)
    {
        float pan = tempPoints.at(k).panAng;

        float tilt = tempPoints.at(k).tiltAng;

        float val = tempPoints.at(k).val;

        float newVal = returnValtoRadius(val);

        float xy =    SPHERE_RADIUS*cos((tilt)*3.14/180);

        float x = xy*cos((pan)*3.14/180);
        float y = xy*sin((pan)*3.14/180);
        float z =  SPHERE_RADIUS*sin(tilt*3.14/180);

         xy =    newVal*cos((tilt)*3.14/180);

        float x1 = xy*cos((pan)*3.14/180);
        float y1 = xy*sin((pan)*3.14/180);
        float z1 =  newVal*sin(tilt*3.14/180);

         glColor3f(1,1,1);

        glBegin(GL_LINES);


         glVertex3f(x, y, z);

         glVertex3f(x1, y1, z1);

        glEnd();



    }




}
