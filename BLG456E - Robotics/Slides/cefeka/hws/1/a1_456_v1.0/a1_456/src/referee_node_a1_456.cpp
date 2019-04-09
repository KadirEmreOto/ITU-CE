/*
 * referee_node_a1_456.cpp
 *
 * BLG456E Assignment 1 referee
 *
 */
#include "ros/ros.h"
#include "nav_msgs/OccupancyGrid.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <tf/transform_listener.h>
#include <sys/stat.h>

int unknownArea=0;
int allArea=0;
int discoveredArea=0;
int occupiedArea=0;
bool close_when_finished=false;

const unsigned int num_times_to_snapshot=2;
double times_to_snapshot[num_times_to_snapshot]= {60.0,600.0};

std::string mapdir_root="a1_456_results_maps";

ros::Duration spinfreq(0.3);

void mapCallback(const nav_msgs::OccupancyGrid::ConstPtr& msg) {
    std::vector<signed char > map = msg->data;
    unknownArea=0;
    allArea=0;
    occupiedArea=0;
    discoveredArea=0;
    for(size_t i=0; i< map.size(); ++i ) {
        int map_x = i % msg->info.width;
        int map_y = i / msg->info.height;
        allArea++;
        if(map[i]>50) { // 0 – 100 represents how occupied
            occupiedArea++;
            discoveredArea++;
        } else if(map[i]<0) { // -1 means unknown
            unknownArea++;
        } else {
            discoveredArea++;
        }
    }
    /*        ROS_WARN("Discovered obstacle area: %d",occupiedArea); //Success Measure
            ROS_WARN("Unknown area: %d",unknownArea);
            ROS_WARN("All area: %d",allArea);
            ROS_WARN("Pose: x=%f,y=%f,theta=%f",msg->info.origin.position.x,msg->info.origin.position.y,
            2*asin(msg->info.origin.orientation.z)*(msg->info.origin.orientation.w<0?-1:1));   */

}
int main(int argc, char **argv)
{

    int num_snapshotted=0;
//     bool cont=true;
    int status=0;
    ros::init(argc, argv, "print_map_node");
    std::string run_id,run_no;

    tf::TransformListener listener(ros::Duration(10.0));
    ros::NodeHandle n;

    if(n.hasParam("/a1_run_id"))
      n.getParam("/a1_run_id",run_id);
    else
      run_id="XX";
    if(n.hasParam("/a1_run_no"))
      n.getParam("/a1_run_no",run_no);
    else
      run_no="YY";
   
    bool first=true;
    ros::Time timenow;
    ros::Time starttime;
    ros::WallTime startwalltime;
    ros::WallTime walltime;

    std::fstream results;
    results.open("a1_456_results.csv", std::ios::out | std::ios::app);
    if(!results.is_open()){
     std::cout<<"COULDNOT OPEN RESULTS FILE"<<std::endl;
//      exit(1);
    }
    
    mkdir(mapdir_root.c_str(),  S_IRWXU );
    std::stringstream ss;
    ss<<mapdir_root<<"/"<<run_id;
    mkdir(ss.str().c_str(), S_IRWXU );
    ss<<"/"<<run_no;
    mkdir(ss.str().c_str(), S_IRWXU );
    std::string mapdir   = ss.str();
    
    int counter=0;
    double distance_travelled_in_500ms=0;
    tf::StampedTransform transform,transform2;

    ros::Subscriber map_reader =n.subscribe("/map",1,mapCallback);
    std::cout << "start loop"<<std::endl;
    while(ros::ok() ) {
//       while(ros::ok() && cont) {
	results.flush();    
      
        spinfreq.sleep();
        ros::spinOnce();

	if(first){
	 first=false;
	 starttime=ros::Time::now();
	 startwalltime=ros::WallTime::now();
	}
        timenow=ros::Time::now();
        walltime=ros::WallTime::now();
        ros::Duration elapsed=timenow-starttime;

        std::stringstream resultstring;
        resultstring<<run_id<<","<<run_no<<","<<counter<<","<<std::setprecision(8)<<elapsed.toSec()<<","<<discoveredArea<<","<<unknownArea<<","<<occupiedArea<<std::endl;;
//         std::cout<<resultstring.str()<<std::endl;
	std::cout<<"Time elapsed (s): "<<elapsed.toSec()<<" Discovered Area: "<<discoveredArea<<" Unknown Area: "<<unknownArea<<" Occupied Area: "<<occupiedArea<<std::endl;

        if(walltime-startwalltime>ros::WallDuration(40.0*60)) {
            std::cout<<resultstring.str()<<std::endl;
            std::cout<<"This is taking too long in the real world"<<std::endl;
            results<<resultstring.str()<<",This took forever (>40 min) so I gave up."<<std::endl;
            status=1;
            break;
        }
        else {

            try {
                listener.lookupTransform("/map", "/base_link",
                                         ros::Time(0), transform);
                listener.lookupTransform("/map", "/base_link",
                                         ros::Time::now()-ros::Duration(0.5), transform2);

//                 std::cout<<transform.getOrigin()<<std::endl;
//                 std::cout<<transform2.getOrigin()<<std::endl;
                distance_travelled_in_500ms=tf::tfDistance(transform.getOrigin(),transform2.getOrigin());
            }
            catch (tf::TransformException ex) {
                std::cout<<"Exception getting transform:\n"<<ex.what()<<std::endl;
                distance_travelled_in_500ms=0;
                continue;
            }

            if(distance_travelled_in_500ms>100.0) {
                std::cout<<"Robot flew away."<<std::endl;
                results<<resultstring.str()<<",Robot flew away."<<std::endl;
                status=1;
                break;
            }
            if(num_snapshotted<num_times_to_snapshot&& elapsed.toSec() >= times_to_snapshot[num_snapshotted]) {

	      
                std::ostringstream oss;
                oss<<"rosrun map_server map_saver -f "<<mapdir<<"/snapshot"<<times_to_snapshot[num_snapshotted]<<" &";
                system(oss.str().c_str());
                num_snapshotted++;
		results<<resultstring.str()<<"Normal snapshot."<<std::endl;
            }

            if(num_snapshotted>=num_times_to_snapshot && close_when_finished) {
                results<<resultstring.str()<<"Finished run."<<std::endl;
                std::cout<<"Finished run with "<<elapsed.toSec()<<" seconds elapsed"<<std::endl;
                break;
            } 
            counter+=1;
        }

    }

    std::cout<<"Referee is closing, thanks."<<std::endl;

    results.flush();
    results.close();

    return status;
}

