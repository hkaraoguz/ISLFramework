#include "pcprocessing.h"
#include "bubbleprocess.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <pcl-1.5/pcl/filters/voxel_grid.h>
#include <pcl-1.5/pcl/common/transforms.h>
#include <Eigen/Geometry>
#include <Eigen/Eigen>
#include <Eigen/Core>


double round(double r) {
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}


QString dataSetPath;
QStringList dataSetItems;
boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;

PCprocessing::PCprocessing()
{
    currentCloud = sensor_msgs::PointCloud2::Ptr(new sensor_msgs::PointCloud2 ());

    currentCloudNormals = pcl::PointCloud<pcl::Normal>::Ptr(new pcl::PointCloud<pcl::Normal>());
}

void PCprocessing::setDataSetPath(QString dir){


    dataSetPath = dir;

}
QString PCprocessing::getDataSetPath(){

    return dataSetPath;

}
void PCprocessing::setDataSetItems(QStringList items){

    if(dataSetItems.size() > 0){

        dataSetItems.clear();
    }


    dataSetItems<<items;

    qDebug()<<"datasetitems: "<<dataSetItems;





}
int PCprocessing::getNumofItems()
{

    return dataSetItems.size();


}
void PCprocessing::setViewer(boost::shared_ptr<pcl::visualization::PCLVisualizer> viwer){

    viewer = viwer;

}
bool PCprocessing::loadItem(int itemNumber, QString fileName, sensor_msgs::PointCloud2::Ptr cloud)
{
   // qDebug()<<"datasetitems:: "<<dataSetItems;



   // Eigen::Affine3f rot;

  //  Eigen::Affine3f rotMat = Eigen::AngleAxisf(ss);

  //  rot.matrix() = ss;

    if(dataSetPath==NULL) return false;

  //  if(dataSetItems.size() == 0) return false;

    QString itemPath = dataSetPath;

    itemPath.append(fileName);

    QString ss;

    ss.setNum(itemNumber);

    itemPath.append(ss);

    itemPath.append(".pcd");

   // itemPath.append(dataSetItems.at(itemNumber));

    if( pcl::io::loadPCDFile(itemPath.toStdString(),*cloud) < 0)return false;

    qDebug()<<"Loaded item path: "<<itemPath;

    pcl::PointCloud<pcl::PointXYZRGB> tempCloud;

    pcl::fromROSMsg(*cloud,tempCloud);



    pcl::toROSMsg(tempCloud,*cloud);


  //   pcl::transformPointCloud(tempCloud,tempCloud,xx*yy);


    viewer->removeAllPointClouds();

    viewer->addPointCloud<pcl::PointXYZRGB>(tempCloud.makeShared());


    viewer->resetCamera();


    return true;



}

void PCprocessing::rotatePointCloud(sensor_msgs::PointCloud2::Ptr input, int rotX, int rotY, int rotZ)
{
    Eigen::Matrix4f yy;

    double xRad = rotX*M_PI/180;

    double yRad = rotY*M_PI/180;

    double zRad = rotZ*M_PI/180;

    yy<<cos(yRad), 0, sin(yRad), 0,
            0, 1, 0, 0,
            -sin(yRad), 0, cos(yRad), 0,
            0, 0, 0, 1;

    Eigen::Matrix4f xx;

        xx<<1, 0, 0, 0,
               0, cos(xRad), -sin(xRad), 0,
               0, sin(xRad), cos(xRad), 0,
               0, 0, 0, 1;


     Eigen::Matrix4f zz;

     zz<<cos(zRad),-sin(zRad), 0, 0,
             sin(zRad), cos(zRad), 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1;



        pcl::PointCloud<pcl::PointXYZRGB> tempCloud;

        pcl::fromROSMsg(*input,tempCloud);


        pcl::transformPointCloud(tempCloud,tempCloud,xx*yy*zz);

        pcl::toROSMsg(tempCloud,*currentCloud);


        viewer->removeAllPointClouds();
        viewer->addPointCloud<pcl::PointXYZRGB>(tempCloud.makeShared());


        viewer->resetCamera();

}
void PCprocessing::scalePointCloud(sensor_msgs::PointCloud2::Ptr input, int scale)
{

    pcl::PointCloud<pcl::PointXYZRGB> tempCloud;

    pcl::fromROSMsg(*input,tempCloud);


    for(long k = 0; k < tempCloud.width; k++){

        pcl::PointXYZRGB pt =  tempCloud.points[k];

        pt.x = pt.x/scale;
        pt.y = pt.y/scale;
        pt.z = pt.z/scale;

        tempCloud.points[k] = pt;


    }

       pcl::toROSMsg(tempCloud,*input);


       viewer->removeAllPointClouds();
       viewer->addPointCloud<pcl::PointXYZRGB>(tempCloud.makeShared());


       viewer->resetCamera();



}

sensor_msgs::PointCloud2::Ptr PCprocessing::getCurrentCloud()
{

    return this->currentCloud;



}
 pcl::PointCloud<pcl::Normal>::Ptr PCprocessing::getCurrentCloudNormals(){


     return this->currentCloudNormals;

 }

void PCprocessing::applyVoxelGridFilter(sensor_msgs::PointCloud2::Ptr input)
{

     qDebug()<<"PointCloud before filtering: "<< input->width * input->height<< " data points (" << QString::fromStdString( pcl::getFieldsList (*input) )<< ").";;

     sensor_msgs::PointCloud2::Ptr filtered_cloud(new sensor_msgs::PointCloud2 ());

     // Create the filtering object
     pcl::VoxelGrid<sensor_msgs::PointCloud2> sor;
     sor.setInputCloud(input);
     sor.setLeafSize (0.01f, 0.01f, 0.01f);
     sor.filter (*filtered_cloud);

     qDebug()<< "PointCloud after filtering: " << filtered_cloud->width * filtered_cloud->height;//<< " data points (" << QString::fromStdString( pcl::getFieldsList (*filtered_cloud) )<< ").";

     currentCloud.reset();

     currentCloud = filtered_cloud;

     filtered_cloud.reset();

     qDebug()<<"Current cloud use count: "<<currentCloud.use_count();

     pcl::PointCloud<pcl::PointXYZ> tempCloud;

     pcl::fromROSMsg(*currentCloud,tempCloud);

     viewer->removeAllPointClouds();
     viewer->addPointCloud<pcl::PointXYZ>(tempCloud.makeShared());


     viewer->resetCamera();


}
void PCprocessing::calculateNormals(sensor_msgs::PointCloud2::Ptr input)
{


    // Create the normal estimation class, and pass the input dataset to it
    pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> ne;

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr tempCloud (new pcl::PointCloud<pcl::PointXYZRGB>());

    pcl::fromROSMsg(*input,*tempCloud);

    ne.setInputCloud (tempCloud);

    // Create an empty kdtree representation, and pass it to the normal estimation object.
    // Its content will be filled inside the object, based on the given input dataset (as no other search surface is given).
    pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGB> ());
    ne.setSearchMethod (tree);

    // Output datasets
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);

    if(currentCloudNormals->size() > 0) currentCloudNormals.reset(new pcl::PointCloud<pcl::Normal>());

    //currentCloudNormals = cloud_normals;

    // Use all neighbors in a sphere of radius 3cm
    ne.setRadiusSearch (0.03);

    // Compute the features
    ne.compute (*currentCloudNormals);

   // pcl::Normal pt = cloud_normals->points.at(0);

  //  qDebug()<<"normal x :"<<pt.normal_x;

    viewer->addPointCloudNormals<pcl::PointXYZRGB, pcl::Normal> (tempCloud, currentCloudNormals, 10, 0.05, "normals");

    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_COLOR, 1.0, 0.0, 0.0, "normals");
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 3, "normals");

    viewer->resetCamera();


}
bool PCprocessing::saveNormalAngleHistogram(pcl::PointCloud<pcl::Normal>::Ptr normals,int itemNumber)
{
    if(dataSetPath==NULL) return false;

   // if(dataSetItems.size() == 0) return false;

    QString itemPath = dataSetPath;

  //  itemPath.append("n");

    itemPath.append("normalAngleHistBubble_");

    QString ss;

    ss.setNum(itemNumber);

    itemPath.append(ss);

    itemPath.append(".m");

   // itemPath.append(dataSetItems.at(itemNumber));

    qDebug()<<"Save normal histogram bubble: "<<itemPath;


    QFile file(itemPath);

    QFile file2("normals.txt");

    if(!file.open(QFile::WriteOnly|QFile::Text) || !file2.open(QFile::WriteOnly|QFile::Text)){


        return false;
    }

    if(normals->size() <= 0) return false;

    QVector< QVector<int> > angles(360, QVector<int>(360));

    std::vector<bubblePoint> result;

    QTextStream stream(&file);

  //  QTextStream stream2(&file2);

    for(unsigned int i = 0; i < normals->size(); i+=5)
    {

        pcl::Normal pt = normals->points.at(i);

        double pan = atan2(pt.normal_z,pt.normal_x)*180/3.14159;

        pan = round(pan);

        double xy = sqrt(pt.normal_x*pt.normal_x + pt.normal_z*pt.normal_z);

        double tilt = atan2(pt.normal_y,xy)*180/3.14159;

        tilt = round(tilt);

        if(pan < 0)pan += 360;
        else if(pan > 359) pan -=360;

        if(tilt < 0) tilt += 360;
        else if(tilt > 359) tilt -=360;

        angles[pan][tilt] += 1;

   //     stream2<<pt.normal_x<<" "<<pt.normal_y<<" "<<pt.normal_z<<" "<<pan<<" "<<" "<<tilt<<"\n";



    }

    for(int i = 0; i < 360; i++){
        for(int j = 0; j < 360; j++){
          //  if(angles[i][j]!=0)

                stream<<i<<" "<<j<<" "<<(double)angles[i][j]/100<<"\n";

        }

    }


    file.close();

    file2.close();

    return true;
}
bool PCprocessing::saveNormalPanTiltHistograms(pcl::PointCloud<pcl::Normal>::Ptr normals)
{

    return false;

}
bool PCprocessing::savePointCloud(int itemNumber, QString fileName){


    if(dataSetPath==NULL) return false;

    if(dataSetItems.size() == 0) return false;

    QString itemPath = dataSetPath;

    itemPath.append("n");

    itemPath.append(fileName);

    QString ss;

    ss.setNum(itemNumber);

    itemPath.append(ss);

    itemPath.append(".pcd");

   // itemPath.append(dataSetItems.at(itemNumber));

    qDebug()<<itemPath;

    pcl::PointCloud<pcl::PointXYZRGB> tempCloud;

    pcl::fromROSMsg(*currentCloud,tempCloud);

    pcl::io::savePCDFileBinary(itemPath.toStdString(),tempCloud);

    return false;
}
