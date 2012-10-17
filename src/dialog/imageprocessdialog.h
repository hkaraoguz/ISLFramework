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

    ~ImageProcessDialog();
    void setPCprocessing(PCprocessing* pcprocess);

    // This is for holding the bubble File Names
    QStringList bubbleFileNames;

    // This is for holding filter file names
    QStringList filters;


    // This is for holding the Image File names
    QStringList imageFiles;


    //This is for holding the invariant File Names
    QStringList invariantFileNames;
    
private slots:

    void on_but_LoadImage_clicked();

    void on_butSetDSetPath_clicked();

    void on_butLoadFilter_clicked();

    void on_butApplyAll_clicked();

    void on_cBoxTransposeFilter_stateChanged(int arg1);

    void on_butGenerateHueBubble_clicked();

    void on_butGenerateInvariants_clicked();

    void on_butAddtoBubbleFileList_clicked();



    void on_butAddtoInputFileNames_clicked();

    void on_butAddtoFilterNames_clicked();

    void on_butAddtoInvariantNames_clicked();

private:

    Ui::ImageProcessDialog *ui;

    PCprocessing* pcProcess;



};

#endif // IMAGEPROCESSDIALOG_H
