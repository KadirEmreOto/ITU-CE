#!/usr/bin/env python
## AK
## explorer_node_py.py
##
## BLG456E Assignment 1 skeleton
##
## Instructions: Change the laser_callback function to make the robot explore more
## intelligently, using its sensory data (the laser range array).
##
## Advanced: If you want to make use of the robot's mapping subsystem then you can
## make use of the map in the mapping_callback function.
##
##

## Common ROS headers.
import rospy
## Required for some printing options
import sys
import math

## This is needed for the data structure containing the motor command.
from geometry_msgs.msg import Twist
## This is needed for the data structure containing the laser scan
from sensor_msgs.msg import LaserScan
## This is needed for the data structure containing the map (which you may not use).
from nav_msgs.msg import OccupancyGrid

## The following function is a "callback" function that is called back whenever a new laser scan is available.
## That is, this function will be called for every new laser scan.
##
## --------------------------------------------------------------------------------
## ----------CHANGE THIS FUNCTION TO MAKE THE ROBOT EXPLORE INTELLIGENTLY----------
## --------------------------------------------------------------------------------
##

# Finite State Machine :)
states = {
    0: "nearing to wall",
    1: "following the wall",
    2: "turning and adjusting route"
}

state = 0
counter = 0

def laser_callback(data):
    global state
    global counter
    global motor_command_publisher

    motor_command = Twist()

    # some thing is wrong, no object around me, I should find a wall first
    # if math.isnan(data.ranges[0]) and math.isnan(data.ranges[-1]) and data.ranges[len(data.ranges)/2]:
        # state = 0

    if state == 0:  # nearing to wall
        if 0.5 < data.ranges[len(data.ranges)/2] < 1:
            state = 1
            motor_command.angular.z = -8

        else:
            motor_command.linear.x = 0.5

    elif state == 1:  # following the wall
        if data.ranges[-1] > 3.5:  # moving away from wall, turn left a little
            motor_command.angular.z = +0.3

        elif data.ranges[-1] < 1.2:  # getting closer to wall, turn right a little
            motor_command.angular.z = -0.3

        elif math.isnan(data.ranges[-1]):  # wall is not my left, what? Try to find the wall, change state
            state = 2

        elif 0.5 < data.ranges[len(data.ranges)/2] < 1 or \
            0.5 < data.ranges[len(data.ranges)/2 - 60] < 1 or \
            0.5 < data.ranges[len(data.ranges)/2 + 60] < 1:  # there are an object on my way,
            motor_command.angular.z = -3                 # also there are a wall that a follow on my left, turn right

        else:
            motor_command.linear.x = 0.5

    elif state == 2:  # turning and adjusting route
        if counter < 3:
            motor_command.linear.x = 0.5
            counter += 1

        else:
            motor_command.angular.z = +3
            state = 1
            counter = 0


    motor_command_publisher.publish(motor_command)

    print 'State:', states[state]
    print 'The distance to the rightmost scanned point is: ', data.ranges[0]
    print 'The distance to the leftmost scanned point is: ', data.ranges[-1]
    print 'The distance to the middle scanned point is: ', data.ranges[len(data.ranges)/2]
    print ""

## You can also make use of the map which is being built by the "gslam_mapping" subsystem
## There is some code here to help but you can understand the API also by looking up the OccupancyGrid message and its members (this is the API for the message)
## If you want me to explain the data structure, I will - just ask me in advance of class
def map_callback(data):
    chatty_map = False
    if chatty_map:
        print "-------MAP---------"
        ## Here x and y has been incremented with five to make it fit in the terminal
        ## Note that we have lost some map information by shrinking the data
        for x in range(0,data.info.width-1,5):
            for y in range(0,data.info.height-1,5):
                index = x+y*data.info.width
                if data.data[index] > 50:
                    ## This square is occupied
                    sys.stdout.write('X')
                elif data.data[index] >= 0:
                    ## This square is unoccupied
                    sys.stdout.write(' ')
                else:
                    sys.stdout.write('?')
            sys.stdout.write('\n')
        sys.stdout.flush()
        print "-------------------"

## This is the method we initilize everything
def explorer_node():
    print "1"
    ## We must always do this when starting a ROS node - and it should be the first thing to happen
    rospy.init_node('amble')
    print "2"
    ## Here we declare that we are going to publish "Twist" messages to the topic /cmd_vel_mux/navi. It is defined as global because we are going to use this publisher in the laser_callback.
    global motor_command_publisher
    motor_command_publisher = rospy.Publisher('/cmd_vel_mux/input/navi', Twist, queue_size = 10)
    print "3"
    ## Here we set the function laser_callback to recieve new laser messages when they arrive
    rospy.Subscriber("/scan", LaserScan, laser_callback, queue_size = 1000)
    print "4"
    ## Here we set the function map_callback to recieve new map messages when they arrive from the mapping subsystem
    rospy.Subscriber("/map", OccupancyGrid, map_callback, queue_size = 1000)
    print "5"
    ## spin is an infinite loop but it lets callbacks to be called when a new data available. That means spin keeps this node not terminated and run the callback when nessessary.
    rospy.spin()

if __name__ == '__main__':
    print "program started"
    explorer_node()
