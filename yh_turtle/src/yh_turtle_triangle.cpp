#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/TeleportAbsolute.h>
#include <turtlesim/SetPen.h>
#include <cstdlib>
#include <stdio.h>

//거북이 위치 바꾸기
//원 그리게 하기

// bool Turtle::teleportAbsoluteCallback(turtlesim::TeleportAbsolute::Request& req, turtlesim::TeleportAbsolute::Response&)
// {
//   teleport_requests_.push_back(TeleportRequest(req.x, req.y, req.theta, 0, false));
//   return true;
// }

int main(int argc, char** argv)
{
    ros::init(argc, argv, "yh_turtle_triangle");

    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
    ros::ServiceClient client = nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");
    ros::ServiceClient client_pen = nh.serviceClient<turtlesim::SetPen>("/turtle1/set_pen");
    ros::Rate loop_rate(10);
    ros::Time start_time;
    ros::Time end_time;

    int cnt = 1;
    int speed = atoll(argv[1]);
    int distance = atoll(argv[2]);
    
    geometry_msgs::Twist t;
    

    // t.linear.y = 0;
    // t.linear.z = 0;
    // t.angular.x = 0;
    // t.angular.y = 0;
    


    turtlesim::SetPen pen;
    pen.request.off = 1;
    if (client_pen.call(pen))
    {
        turtlesim::TeleportAbsolute srv;
        srv.request.x = 5.544445;
        srv.request.y = 5.544445 + distance;
        srv.request.theta = 0;
        client.call(srv);
        pen.request.off = 0;
        client_pen.call(pen);
        ROS_INFO("sent");
    }
    else //서버로 요청이 안 보내졌으면
	{
		ROS_ERROR("Failed to call service");
		return 1;
	}

    while (ros::ok())
    {        
        if(cnt%2==0)
        {
            t.angular.z = -22.0*2.0/7.0/3.0;
            t.linear.x = 0;
        }
        else
        {
            t.angular.z = 0;
            t.linear.x = speed;
            // if (speed < 0)
            // {
            //     t.linear.x = -speed;
            // }
            // else
            // {
            //     t.linear.x = speed;
            // }
        }
        start_time = ros::Time::now();
        double current_distance = 0;
        while(current_distance < distance)
        {
            pub.publish(t);
            end_time = ros::Time::now();
            ros::Duration duration = end_time - start_time;
            double duration_sec = duration.toSec();
            current_distance = speed*duration_sec;
        }
        cnt++;
    }
    return 0;
}