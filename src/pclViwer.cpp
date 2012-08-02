// pclDeneme.cpp : Defines the entry point for the console application.
//

#include "pclViwer.h"

PclViwer::PclViwer():viewer(new pcl::visualization::PCLVisualizer ("3D Viewer"))
{

 //viewer(new pcl::visualization::PCLVisualizer ("3D Viewer"));
/*
    viewer->setBackgroundColor (0, 0, 0);

    viewer->addCoordinateSystem (1.0);
    viewer->initCameraParameters ();*/
  //pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGBA> rgb(cloud);

}
void PclViwer::run()
{
  //  boost::shared_ptr<pcl::visualization::PCLVisualizer> viwer(new pcl::visualization::PCLVisualizer ("3D Viewer"));

   // viewer(new pcl::visualization::PCLVisualizer ("3D Viewer"));

    configureViewer();


    while (!viewer->wasStopped ())
    {
        viewer->spinOnce(100);

        boost::this_thread::sleep (boost::posix_time::microseconds (100000));
    //QThread::msleep(100);

    }




}
void PclViwer::configureViewer(){

    viewer->setBackgroundColor (0, 0, 0);

    viewer->addCoordinateSystem (1.0);
    viewer->initCameraParameters ();



}
 

//int _tmain(int argc, _TCHAR* argv[])
//{
/*	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);

  if (pcl::io::loadPCDFile<pcl::PointXYZRGBA> ("kinect.pcd", *cloud) == -1) //* load the file
  {
    PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
    return (-1);
  }
  std::cout << "Loaded "
            << cloud->width * cloud->height
            << " data points from test_pcd.pcd with the following fields: "
            << std::endl;
 /* for (size_t i = 0; i < cloud->points.size (); ++i)
    std::cout << "    " << cloud->points[i].x
              << " "    << cloud->points[i].y
              << " "    << cloud->points[i].z << std::endl;*/

  // Create the normal estimation class, and pass the input dataset to it
 // pcl::NormalEstimationOMP<pcl::PointXYZRGBA, pcl::Normal> ne;

 // ne.setInputCloud (cloud);

  // Create an empty kdtree representation, and pass it to the normal estimation object.
  // Its content will be filled inside the object, based on the given input dataset (as no other search surface is given).
 // pcl::search::KdTree<pcl::PointXYZRGBA>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGBA> ());
 // ne.setSearchMethod (tree);

  // Output datasets
 // pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);

  // Use all neighbors in a sphere of radius 3cm
 // ne.setRadiusSearch (0.03);

  // Compute the features
 // ne.compute (*cloud_normals);

   //pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
   //viewer.showCloud (cloud_normals);

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
//	SimpleOpenNIViewer v;
//	v.run ();

    //return 0;
//}

