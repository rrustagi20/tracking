#include <pose_estimate/pose_estimate.hpp>

Eigen::Vector3f pixel, coordi_cam_frame, world_frame; // x_pixel,y_pixel,1 - homegenised coordinates
Eigen::Vector3f drone_pose,husky_pose,error_pose;
Eigen::Matrix3f R,K,drone_z,camtoDrone;
int count = 0, flag = 0;

void calc_pose()
{
    //camera to drone coordinate system rotation matrix
    camtoDrone <<  0.0f,-1.0f, 0.0f,
                  -1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f,-1.0f;
    // coordi_drone_frame = K_inv . z_coordi . pixel
    coordi_cam_frame = (K.inverse())*drone_z*pixel;
    // world_frame = R_inv . coordi_cam_frame + camera_coordi_wrt_world_origin
    world_frame = (R.inverse())*(coordi_cam_frame);
    // world_frame = world_frame + camera_coordi_wrt_world_origin;
    world_frame = world_frame + (camtoDrone*drone_pose);
    // converting from drone frame to camera frame
    world_frame = camtoDrone*world_frame;
    // error in estimation and ground truth odometry of husky
    for(int i =0; i<3; i++)
        error_pose[i]=husky_pose[i]-world_frame[i];
    error_pose[2]=husky_pose[2]-husky_pose[2];
    // print or store world frame coordinates
    ROS_INFO("husky pose: x=%f,y=%f,z=%f] \n",husky_pose[0],husky_pose[1],husky_pose[2]);
    ROS_INFO("estimated: x=%f,y=%f,z=%f | [error: x=%f,y=%f,z=%f] \n",world_frame[0],world_frame[1],world_frame[2],
                                                        error_pose[0], error_pose[1], error_pose[2]);
    return;
}

void callMeBack(tracking::com coords)
{
    pixel[0] = coords.x;
    pixel[1] = coords.y;
    pixel[2] = 1.00f;
    // ROS_INFO("x=%f,y=%f,z=%f",pixel[0],pixel[1],pixel[2]);
    return;
}

void callUsBack(sensor_msgs::CameraInfo camera_params)
{
    if(flag==0) //Intrinsic Params do not change
    {
        for(int i =0; i<3; i++)
            for(int j=0; j<3; j++)
            {
                K(i,j) = camera_params.K[count];
                count++;
            }
        // ROS_INFO("K- /n %f %f %f /n %f %f %f /n %f %f %f /n",K(0,0),K(0,1),K(0,2),K(1,0),K(1,1),K(1,2),K(2,0),K(2,1),K(2,2));
    }
    count=0;
    for(int i =0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            R(i,j) = camera_params.R[count];
            count++;
        }
    // ROS_INFO("R- /n %f %f %f /n %f %f %f /n %f %f %f /n",R(0,0),R(0,1),R(0,2),R(1,0),R(1,1),R(1,2),R(2,0),R(2,1),R(2,2));
    flag++;
    calc_pose();
    return;
}

void weCallBack(nav_msgs::Odometry drone_odom)
{
    drone_pose[0] = drone_odom.pose.pose.position.x;
    drone_pose[1] = drone_odom.pose.pose.position.y;
    drone_pose[2] = drone_odom.pose.pose.position.z;

    //scaling_factor provided camera is nadir!
    for(int i =0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            if(i==j)
                drone_z(i,j) = drone_odom.pose.pose.position.z;
            else
                drone_z(i,j) = 0.0f;
        }
    return;
}

void callyouBack(gazebo_msgs::ModelStates husky_odom)
{
    husky_pose[0] = husky_odom.pose[2].position.x;
    husky_pose[1] = husky_odom.pose[2].position.y;
    husky_pose[2] = husky_odom.pose[2].position.z;
    return;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "posed");
    ros::NodeHandle nh;
    std::this_thread::sleep_for(std::chrono::seconds(21));
    ros::Subscriber sub = nh.subscribe("pixel_com_coordi",20,callMeBack);
    ros::Subscriber sub_drone = nh.subscribe("/firefly/ground_truth/odometry",10,weCallBack);
    ros::Subscriber sub_cam = nh.subscribe("/firefly/camera_downward/camera_info",10,callUsBack);
    ros::Subscriber sub_husky = nh.subscribe("gazebo/model_states",10,callyouBack);
    ros::spin();
    return 0;
}