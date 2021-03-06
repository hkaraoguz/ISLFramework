#ifndef IMAGEPROCESSDIALOG_H
#define IMAGEPROCESSDIALOG_H

#include <QDialog>

class PCprocessing;

namespace Ui {
class ImageProcessDialog;
}

class ImageProcessDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProcessDialog(QWidget *parent = 0);

    ImageProcessDialog(QWidget *parent, PCprocessing* pcprocess);

    ImageProcessDialog(QWidget *parent, int imageWidth,int imageHeight,int focalLengthPixels);

    void initView();

    ~ImageProcessDialog();
    void setPCprocessing(PCprocessing* pcprocess);



    // This is for holding filter file names
    QStringList filters;


    // This is for holding the Image File names
    QStringList imageFileNames;


    
private slots:

    void on_but_LoadImage_clicked();

    void on_butSetDSetPath_clicked();

    void on_butLoadFilter_clicked();

    void on_butApplyAll_clicked();

    void on_butGenerateHueBubble_clicked();

    void on_butAddtoInputFileNames_clicked();

    void on_butAddtoFilterNames_clicked();

    void on_butRemoveInputFileNames_clicked();

    void on_butRemoveFilterNames_clicked();

    void on_horsliderSatUpper_valueChanged(int value);

    void on_horsliderSatLower_valueChanged(int value);

    void on_horsliderValUpper_valueChanged(int value);

    void on_horsliderValLower_valueChanged(int value);

    void on_butApplyFilter_clicked();

private:

    Ui::ImageProcessDialog *ui;

    PCprocessing* pcProcess;


    int imageHeight;

    int imageWidth;

    int focalLengthPixels;

};

#endif // IMAGEPROCESSDIALOG_H
