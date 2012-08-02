#ifndef PCLDIALOG_H
#define PCLDIALOG_H

/*******   PCL DIALOG CLASS: Responsible for ui actions for pcl functions   *****/

#include <QDialog>
#include <boost/thread/thread.hpp>
#include <pcl-1.5/pcl/common/common_headers.h>
#include <pcl-1.5/pcl/visualization/pcl_visualizer.h>

namespace Ui {
class PclDialog;
}

class PclDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PclDialog(QWidget *parent = 0);
    ~PclDialog();
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viwer;

    void initializeView();
    
private slots:
    void on_butSetDSetPath_clicked();

    void on_butLoadItem_clicked();

    void on_butPrevItem_clicked();

    void on_butNextItem_clicked();

    void on_butVoxelGridFilter_clicked();

    void on_butCalNormals_clicked();

    void on_butSaveNormalAngleHist_clicked();

private:
    Ui::PclDialog *ui;
};

#endif // PCLDIALOG_H
