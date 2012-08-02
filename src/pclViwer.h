#include <pcl-1.5/pcl/io/pcd_io.h>
#include <pcl-1.5/pcl/point_types.h>
#include <pcl-1.5/pcl/io/pcd_io.h>
#include <pcl-1.5/pcl/visualization/cloud_viewer.h>
#include <pcl-1.5/pcl/features/normal_3d.h>
#include <boost/thread/thread.hpp>
#include <pcl-1.5/pcl/common/common_headers.h>
#include <pcl-1.5/pcl/visualization/pcl_visualizer.h>
#include <pcl-1.5/pcl/features/normal_3d_omp.h>
#include <boost/shared_ptr.hpp>
#include <QThread>

class PclViwer: public QThread
 {

   public:

    PclViwer();
    void run ();
    void configureViewer();

    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
    /*  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
     viewer->setBackgroundColor (0, 0, 0);
     pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGBA> rgb(cloud);
     viewer->addPointCloud<pcl::PointXYZRGBA> (cloud, rgb, "sample cloud");
     viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
     viewer->addPointCloudNormals<pcl::PointXYZRGBA, pcl::Normal> (cloud, cloud_normals, 10, 0.05, "normals");
     viewer->addCoordinateSystem (1.0);
     viewer->initCameraParameters ();


       while (!viewer->wasStopped ())
     {
       viewer->spinOnce (100);
       boost::this_thread::sleep (boost::posix_time::microseconds (100000));
     }*/

};
