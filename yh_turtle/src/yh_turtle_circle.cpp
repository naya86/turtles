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
    ros::init(argc, argv, "yh_turtle_circle");

    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
    // ros::Publisher pub2 = nh.advertise<turtlesim::Pose>("/turtle1/pose", 1000);
    //ros::ServiceClient client1 = nh.serviceClient<turtlesim::Spawn>("/turtlesim/Spawn");
    ros::ServiceClient client = nh.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");
    ros::ServiceClient client_pen = nh.serviceClient<turtlesim::SetPen>("/turtle1/set_pen");
    
    ros::Rate loop_rate(10);
    int cnt = 0;
    int r = atoll(argv[1]);
    
    // turtlesim::Pose p;
    // p.x = 3;
    // p.y = 3;
    // p.theta = 0;
    // pub2.publish(p);
    // ROS_INFO("sent");
    
    // turtlesim::Spawn srv;
    // srv.request.x = 3.0;
    // srv.request.y = 3.0;
    // srv.request.theta = 0.0;
    // if(client1.call(srv))
    // {
    //     ROS_INFO("sent");    
    // }
    // else //서버로 요청이 안 보내졌으면
	// {
	// 	ROS_ERROR("Failed to call service");
	// 	return 1;
	// }
    turtlesim::SetPen pen;
    pen.request.off = 1;
    if (client_pen.call(pen))
    {
        turtlesim::TeleportAbsolute srv;
        srv.request.x = 5.544445;
        srv.request.y = 5.544445 + r;
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

    while(ros::ok())
    {
        if((cnt<4) | ((r == 1)&(cnt<4)))
        {
            geometry_msgs::Twist t;
            t.angular.z = -(2 * 3.14);// 순수 각도
            t.linear.x = 2 * 3.14 * r;//r * 쎄타
            pub.publish(t);
            ros::spinOnce();
        }
        else
        {
            break;
        }
        
        ++cnt;
        loop_rate.sleep();
    }
    return 0;
}