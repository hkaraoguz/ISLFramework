#ifndef IROBOTDIALOG_H
#define IROBOTDIALOG_H

#include <QDialog>
class Irobot;
namespace Ui {
class IRobotDialog;
}

class IRobotDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit IRobotDialog(QWidget *parent = 0);

    IRobotDialog(QWidget *parent, Irobot* aRobot);

    ~IRobotDialog();

    Irobot* robot;

signals:
    void setFirstOrientation();
    
private slots:
    void on_butRobotForward_clicked();

    void on_butRobotRight_clicked();

    void on_butRobotLeft_clicked();

    void on_butRobotBack_clicked();

    void on_butRobotStop_clicked();

    void on_butIRobotConnect_clicked();

    void on_butIRobotSetFirstOrientation_clicked();

private:
    Ui::IRobotDialog *ui;

    void initView();
};

#endif // IROBOTDIALOG_H
