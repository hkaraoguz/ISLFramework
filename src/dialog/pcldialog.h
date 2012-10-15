#ifndef PCLDIALOG_H
#define PCLDIALOG_H

/*******   PCL DIALOG CLASS: Responsible for ui actions for pcl functions   *****/

#include "kinectgrabber.h"

#include <QDialog>
#include <boost/thread/thread.hpp>
#include <pcl-1.5/pcl/common/common_headers.h>
#include <pcl-1.5/pcl/visualization/pcl_visualizer.h>

class PCprocessing;

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

    void setPCprocessing(PCprocessing* pcprocess);

  //  bool compareNames(const QString& s1,const QString& s2);
    
private slots:
    void on_butSetDSetPath_clicked();

    void on_butLoadItem_clicked();

    void on_butPrevItem_clicked();

    void on_butNextItem_clicked();

    void on_butVoxelGridFilter_clicked();

    void on_butCalNormals_clicked();

    void on_butSaveNormalAngleHist_clicked();


    void on_butRotateCloud_clicked();

    void on_butScalePointCloud_clicked();

    void on_butSavePointCloud_clicked();

    void on_lEditCloudFileName_editingFinished();

    void on_butApplyTransformationtoAll_clicked();

    void on_butGeneratePointCloudBubble_clicked();

    void on_butGeneratePointCloudBubbles_clicked();

    void on_butCalculateBubbleInvariants_clicked();

    void on_butCalculateAllNormalAngleHistogram_clicked();


    void handleKinectFrame(const sensor_msgs::PointCloud2ConstPtr& cloud);

    void handleKinectStart();

    void handleKinectFailed();

    void on_butKinectStart_clicked();

private:
    Ui::PclDialog *ui;

     PCprocessing* pcProcessing;

     KinectGrabber* grabber;

};

#endif // PCLDIALOG_H
