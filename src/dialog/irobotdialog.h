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
    
private slots:
    void on_butRobotForward_clicked();

    void on_butRobotRight_clicked();

    void on_butRobotLeft_clicked();

    void on_butRobotBack_clicked();

private:
    Ui::IRobotDialog *ui;
};

#endif // IROBOTDIALOG_H
