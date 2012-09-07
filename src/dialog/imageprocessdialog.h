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
    ~ImageProcessDialog();
    void setPCprocessing(PCprocessing* pcprocess);
    
private slots:

    void on_but_LoadImage_clicked();

    void on_butSetDSetPath_clicked();

    void on_butLoadFilter_clicked();

    void on_butApplyAll_clicked();

    void on_cBoxTransposeFilter_stateChanged(int arg1);

    void on_butGenerateHueBubble_clicked();

private:

    Ui::ImageProcessDialog *ui;

    PCprocessing* pcProcess;



};

#endif // IMAGEPROCESSDIALOG_H
