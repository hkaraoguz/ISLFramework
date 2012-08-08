#ifndef BUBBLEPROCESSDIALOG_H
#define BUBBLEPROCESSDIALOG_H

#include <QDialog>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
namespace Ui {
class BubbleProcessDialog;
}

class BubbleProcessDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BubbleProcessDialog(QWidget *parent = 0);
    ~BubbleProcessDialog();

    void initializeView();
    
private slots:
    void on_But_bubbleFilesSetRootDir_clicked();

    void on_But_fetchBubbleFiles_clicked();

    void on_But_chooseFile_clicked();

    void on_ComboBox_fetchedBubbles_currentIndexChanged(int index);

    void on_But_readPoseData_clicked();

    void on_But_transformBubbles_clicked();

    void on_But_calcDiff_clicked();

private:
    Ui::BubbleProcessDialog *ui;

public slots:
    void handleGLBubbleViewerFrameChanged(int frameNumber);
};

#endif // BUBBLEPROCESSDIALOG_H
