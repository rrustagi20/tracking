#include <tracking/trackerKCF.hpp>

namespace kcf//namespace track::kcf
{
    // Tracks::Tracks() {}
    // { bool tr = true; }

    void Tracks::init(ros::NodeHandle& nh)
    {
        image_transport::ImageTransport it_(nh);
        image_sub_ = it_.subscribe("/image", 20, &Tracks::imageCb, this);
        coordi_pub_ = nh.advertise<tracking::com>("pixel_com_coordi",20);
        com_x_img = 0, com_y_img = 0;
        flex = true;
        tracker = cv::TrackerKCF::create();
    }

    void Tracks::run()
    {
        tracking::com msgs;
        msgs.x = com_x_img;
        msgs.y = com_y_img;
        coordi_pub_.publish(msgs);
    }

    void Tracks::imageCb(const sensor_msgs::ImageConstPtr& msg)
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
        // cv::InputArray fur = frame;
        if(flex == true)
        {
            trackingBox = cv::selectROI(frame, false, false);
            tracker->init(frame, trackingBox);
            flex = false;
        }
        //Update the tracker and draw the rectangle around target if update was successful
        if (tracker->update(frame, trackingBox))
        {
        //std::cout<<"yay\n";
           cv::rectangle(frame, trackingBox, cv::Scalar(255, 0, 0), 2, 1, 0);
        }
        //Display the frame
        cv::imshow("Video feed", frame);
        cv::waitKey(3);
        //approximating pixel COM coordinates of the bot
        com_x_img = float(trackingBox.x)+(float(trackingBox.width))/2.0;
        com_y_img = float(trackingBox.y)+(float(trackingBox.height))/2.0;

    }
}