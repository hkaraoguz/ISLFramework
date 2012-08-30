#ifndef IMAGEPROCESSDIALOG_H
#define IMAGEPROCESSDIALOG_H

#include <QDialog>

namespace Ui {
class ImageProcessDialog;
}

class ImageProcessDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProcessDialog(QWidget *parent = 0);
    ~ImageProcessDialog();
    
private:
    Ui::ImageProcessDialog *ui;
};

#endif // IMAGEPROCESSDIALOG_H
