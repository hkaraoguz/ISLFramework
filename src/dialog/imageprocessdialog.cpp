#include "imageprocessdialog.h"
#include "ui_imageprocessdialog.h"

ImageProcessDialog::ImageProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProcessDialog)
{
    ui->setupUi(this);
}

ImageProcessDialog::~ImageProcessDialog()
{
    delete ui;
}
