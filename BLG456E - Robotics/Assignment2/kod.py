#!/usr/bin/env python
# A
from __future__ import print_function
import rospy
from geometry_msgs.msg import Transform
from geometry_msgs.msg import Twist
import tf
from tf import transformations

import numpy
from math import cos, sin, atan2, hypot, pi

waypoint=None

#waypoint callback
def waypoint_callback(msg): #  callback

    #***************************************
    #***          Obtain current destination
    #***************************************

    #save waypoint data for printing out in main loop
    global waypoint
    waypoint=msg;


if __name__ == '__main__':

    #setup ROS node, subscribe waypoint_cb to the topic /waypoint_cmd & publish motor commands
    rospy.init_node("crazy_driver_456")
    waypoint_subscriber = rospy.Subscriber("/waypoint_cmd", Transform, waypoint_callback) # <--- set up callback
    motor_command_publisher = rospy.Publisher("/cmd_vel_mux/input/navi", Twist, queue_size=100)
    #you could in principle also subscribe to the laser scan as is done in assignment 1.

    #setup transform cache manager
    listener = tf.TransformListener()

    #start a loop; one loop per second
    delay = rospy.Rate(10.0); # perhaps this could be faster for a controller?

    print("Program Started")

    while not rospy.is_shutdown():


        #***************************************
        #***          Obtain current robot pose
        #***************************************
        
        try:
            #grab the latest available transform from the odometry frame (robot's original location - usually the same as the map unless the odometry becomes inaccurate) to the robot's base.
            
            (translation,orientation) = listener.lookupTransform("/odom", "/base_footprint", rospy.Time(0));
        except  (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException) as e:
            print("EXCEPTION:",e)
            #if something goes wrong with this just go to bed for a second or so and wake up hopefully refreshed.
            delay.sleep()
            continue
        

        #***************************************
        #***          Print current robot pose
        #***************************************

        #Print out the x,y coordinates of the transform
        print("Robot is believed to be at (x,y): (",translation[0],",",translation[1],")")

        #Convert the quaternion-based orientation of the latest message to Euler representation in order to get z axis rotation
        r_xorient, r_yorient, r_zorient = transformations.euler_from_matrix(transformations.quaternion_matrix(orientation))
        robot_theta = r_zorient  # only need the z axis
        print("Robot is believed to have orientation (theta): (",robot_theta,")\n")

        #***************************************
        #***          Print current destination
        #***************************************

        # the waypoint variable is filled in in the waypoint_callback function above, which comes from incoming messages
        # subscribed to in the .Subscriber call above.

        #Print out the x,y coordinates of the latest message
        print("Current waypoint (x,y): (",waypoint.translation.x,",",waypoint.translation.y,")")

        #Convert the quaternion-based orientation of the latest message to angle-axis in order to get the z rotation & print it.
        waypointrotq = [waypoint.rotation.x,waypoint.rotation.y,waypoint.rotation.z,waypoint.rotation.w]
        w_xorient, w_yorient, w_zorient = transformations.euler_from_matrix(transformations.quaternion_matrix(waypointrotq))
        waypoint_theta=w_zorient # only need the z axis
        print("Current waypoint (theta): (",waypoint_theta,")\n")

        #***************************************
        #***          DRIVE THE ROBOT HERE (same as with assignment 1 
        #**           - except you are driving towards a goal not away from an obstacle)
        #***************************************

        # *** This version passes all test cases in my computer :)) ***

        # qr: robot reference frame
        # ql: world reference frame
        # p = position of robot robot
        # r = rotation matrix
        # qr = r * (ql - p)
        # d = (ql - p)

        r = numpy.array([
            [cos(robot_theta), sin(robot_theta)], 
            [-sin(robot_theta), cos(robot_theta)] ])
        
        d = numpy.array([
            [waypoint.translation.x - translation[0]],
            [waypoint.translation.y - translation[1]] ])

        qr = numpy.dot(r, d)
        
        angle = atan2(qr[1][0], qr[0][0]) % (pi * 2)  # calculate the angle 
        speed = max(0.8 * hypot(qr[0][0], qr[1][0]), 0.2)  # calculate the speed according to distance btw waypoint and robot
        # minimum speed can be 0.2 

        if (angle > pi):  # if angle is wide angle
            angle -= 2 * pi  # turn it acute angle


        print("> Calculated Angle:", angle)
        print("> Calculated Speed:", speed, '\n')

        #for containing the motor commands to send to the robot
        motor_command=Twist()

        #I don't know what to do because nobody has programmed me with any smartness,
        #so I'll do what everybody does, and drive very fast straight forwards.
        
        motor_command.linear.x = speed
        motor_command.angular.z = angle 

        motor_command_publisher.publish(motor_command)

        #######################################################################
        #FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME
        #FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME
        #FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME -- FIX ME
        #######################################################################

        delay.sleep()
        # we don't need to call spinOnce like in roscpp as callbacks happen in different threads
    
    
    print("ROS shutdown now I will also go to sleep. I hope I didn't crash. Night night.")
