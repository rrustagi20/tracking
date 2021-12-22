#pragma once

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
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tldDataset.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core.hpp>
#include <chrono>
#include <thread>
#include <iostream>

namespace kcf//namespace track::kcf
{
    class Tracks
    {
        // image_transport::ImageTransport it_(const ros::NodeHandle& nh);
        public: Tracks() {}; // I had to manually declare the constructor..?
        void imageCb(const sensor_msgs::ImageConstPtr& msg);
        void init(ros::NodeHandle& nh);//), image_transport::ImageTransport& it_);
        void run();
        
        ros::Publisher coordi_pub_;
        //image_transport::ImageTransport it_(const ros::NodeHandle& nh);
        image_transport::Subscriber image_sub_;
        image_transport::Publisher image_pub_;
    };
    double com_x_img, com_y_img;
    bool flex;// = true;
    cv::Rect2d trackingBox;
    cv::Ptr<cv::Tracker> tracker;// = cv::TrackerKCF::create();
    cv:: Mat frame;

} // namespace part 1