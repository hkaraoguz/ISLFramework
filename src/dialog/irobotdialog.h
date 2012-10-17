#ifndef IROBOTDIALOG_H
#define IROBOTDIALOG_H

#include <QDialog>

namespace Ui {
class IRobotDialog;
}

class IRobotDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit IRobotDialog(QWidget *parent = 0);
    ~IRobotDialog();
    
private:
    Ui::IRobotDialog *ui;
};

#endif // IROBOTDIALOG_H
