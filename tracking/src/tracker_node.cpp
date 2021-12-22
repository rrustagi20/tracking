#include <tracking/trackerKCF.hpp>

using namespace kcf;//using namespace track::kcf;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tracker_node");
    ros::NodeHandle nh;

    std::this_thread::sleep_for(std::chrono::seconds(15));
    
    Tracks tracker;
    tracker.init(nh);
    
    ros::Rate loop_rate(30);

    while(ros::ok)
    {
        ros::spinOnce();
        tracker.run();
        loop_rate.sleep();
    }

    return 0;
}