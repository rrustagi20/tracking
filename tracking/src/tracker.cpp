#include <ros/ros.h>
#include "tracking/com.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
// #include <opencv4/opencv2/tracking.hpp>
// #include <opencv4/opencv2/tracking/tldDataset.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tldDataset.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core.hpp>
#include <chrono>
#include <thread>
#include <iostream>

std::string OPENCV_WINDOW = "Image Window";
bool flag = true;
double com_x_img = 0, com_y_img = 0;
ros::Publisher coordi_pub;

cv::Rect2d trackingBox;
cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
cv:: Mat frame;

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

    public: ImageConverter(): it_(nh_)
    {
        // Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("firefly/camera_nadir/image_raw", 20, &ImageConverter::imageCb, this);
        // image_pub_ = it_.advertise("/image_converter/output_video", 1);
        // publish the coordinates of husky
        coordi_pub = nh_.advertise<tracking::com>("pixel_com_coordi",20); //ERROR : NOT DECLARED IN THIS SCOPE WHY?
        
    }

    void imageCb(const sensor_msgs::ImageConstPtr& msg)
    {
        cv_bridge::CvImagePtr cv_ptr;
        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
        frame = cv_ptr->image;
        if(flag == true)
        {
            trackingBox = cv::selectROI("Select_ROI",frame, true, false);
            cv::destroyWindow("Select_ROI");
            tracker->init(frame, trackingBox);
            flag = false;
        }
        //Update the tracker and draw the rectangle around target if update was successful
        if (tracker->update(frame,trackingBox))
        {
           cv::rectangle(frame, trackingBox, cv::Scalar(255, 0, 0), 2, 1, 0);
        }
        cv::imshow("Video feed", frame);
        cv::waitKey(3);

        com_x_img = float(trackingBox.x)+(float(trackingBox.width))/2.0;
        com_y_img = float(trackingBox.y)+(float(trackingBox.height))/2.0;

        tracking::com msgs;
        msgs.x = com_x_img;
        msgs.y = com_y_img;
        coordi_pub.publish(msgs);
    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_converter");
    ImageConverter ic;
    std::this_thread::sleep_for(std::chrono::seconds(15));
    ros::spin();
    return 0;
}
