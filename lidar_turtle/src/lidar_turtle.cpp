#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <visualization_msgs/MarkerArray.h>
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Pose.h"

//#include "object_detection.h"
ros::Publisher pub;


void msgCallback(const visualization_msgs::MarkerArray::ConstPtr &msg) //const: 상수
{
	//visualization_msgs::Marker marker_msg;
    //geometry_msgs::Pose pose;
    //ROS_INFO("receive = %d", msg -> markers.size());
	//ROS_INFO("%d", msg -> markers[1].type);
	// int cnt = 0;
	// double x_gap = 25;
	// double y_gap = 25;
	// double minimum = 25 * 25 + 25 * 25;
	int cnt = 0;
	double x_gap = 25;
	double y_gap = 25;
	double minimum = 25 * 25 + 25 * 25;

	for(int i = 0; i < msg -> markers.size(); i++)
	{
		if(msg -> markers[i].type == 2)
		{
			
			if(minimum > ((msg -> markers[i].pose.position.x) * (msg -> markers[i].pose.position.x)) + ((msg -> markers[i].pose.position.y) * (msg -> markers[i].pose.position.y)))
			{
				minimum = ((msg -> markers[i].pose.position.x) * (msg -> markers[i].pose.position.x)) + ((msg -> markers[i].pose.position.y) * (msg -> markers[i].pose.position.y));
				x_gap = msg -> markers[i].pose.position.x;
				y_gap = msg -> markers[i].pose.position.y;
				cnt++;
			}
		}
		
	}
	ROS_INFO("cnt: %d", cnt);
	ROS_INFO("x: %f", x_gap);
	ROS_INFO("y: %f", y_gap);
	ROS_INFO("minimum: %f", minimum);

	ros::NodeHandle nh;
	pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
	geometry_msgs::Twist t;
	if (x_gap != 25)
	{
		if(x_gap > 0)
		{
			//t.angular.z = -(2 * 3.14);// 순수 각도
			t.linear.x = 1;//r * 쎄타
			pub.publish(t);
			// ros::spinOnce();
		}
		else if(x_gap < 0)
		{
			t.linear.x = -1;//r * 쎄타
			pub.publish(t);
			// ros::spinOnce();
		}
	}
    //loop_rate.sleep();

	// double min_x = msg -> markers[1].pose.position.x;
	// double min_y = msg -> markers[1].pose.position.y;

	// double minimum = ((msg -> markers[1].pose.position.x) * (msg -> markers[1].pose.position.x)) + ((msg -> markers[1].pose.position.y) * (msg -> markers[1].pose.position.y));
	// //double minimum = 25*25;
	// // double min_x = 25;
	// // double min_y = 25;

	// for (int i = 1; i < msg -> markers.size(); i++)
	// {
	// 	// if((msg -> markers[i].pose.position.x) * (msg -> markers[i].pose.position.x) != 0)
	// 	// {
	// 	// 	if(min_x > msg -> markers[i].pose.position.x)
	// 	//  	{
	// 	//  		min_x = msg -> markers[i].pose.position.x;
	// 	//  		//ROS_INFO("receive_x = %f", min_x);
	// 	//  	}
	// 	// }
		
	// 	//double minimum = ((msg -> markers[i].pose.position.x) * (msg -> markers[i].pose.position.x)) + ((msg -> markers[i].pose.position.y) * (msg -> markers[i].pose.position.y));
	// 	//ROS_INFO("receive %d = %f", i, minimum);
		
	// 	//if(minimum > msg -> markers[i].pose.position.y)
	// 	//double minimum = ((msg -> markers[1].pose.position.x) * (msg -> markers[1].pose.position.x)) + ((msg -> markers[1].pose.position.y) * (msg -> markers[1].pose.position.y));
		
	// 	if(minimum > ((msg -> markers[i].pose.position.x) * (msg -> markers[i].pose.position.x)) + ((msg -> markers[i].pose.position.y) * (msg -> markers[i].pose.position.y)))
	// 	{
	// 	 	minimum = ((msg -> markers[i].pose.position.x) * (msg -> markers[i].pose.position.x)) + ((msg -> markers[i].pose.position.y) * (msg -> markers[i].pose.position.y));
	// 	// //	min_x = msg -> markers[i].pose.position.x;
	// 	// //	min_y = msg -> markers[i].pose.position.y;
	// 	// }


	// 	// if(min_y > msg -> markers[i].pose.position.y)
	// 	// {
	// 	// 	min_x = msg -> markers[i].pose.position.x;
	// 	//  	min_y = msg -> markers[i].pose.position.y;
			
	// 	// // 		//ROS_INFO("receive_x = %f", min_x);
	// 	}

	// }
    // // ROS_INFO("receive_x = %f", min_x);
	// // ROS_INFO("receive_y = %f", min_y);
	// ROS_INFO("receive_minimum = %f", minimum);
	// //ROS_INFO("receive_x = %f", min_x);
	// //ROS_INFO("receive_y = %f", min_y);

	// // ROS_INFO("receive msg = %f", msg -> pose.position.y);
	// // ROS_INFO("receive msg = %f", msg -> pose.position.z);
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "lidar_turtle");
	ros::NodeHandle nh;

	ros::Subscriber sub = nh.subscribe("/marker_array", 100, msgCallback);
	ros::spin(); // 어떤 값이 들어오기 전까지 대기
	return 0;
}