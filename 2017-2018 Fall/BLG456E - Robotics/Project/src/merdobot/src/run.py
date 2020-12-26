#!/usr/bin/env python
import rospy
import os
import math

from threading import Thread
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan
from nav_msgs.msg import OccupancyGrid
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
import actionlib
from actionlib_msgs.msg import *
from geometry_msgs.msg import Pose, Point, Quaternion

from ball_finder import ball_control, listener
from camera import TakePhoto


# Finite State Machine :)
states = {
    0: "nearing to wall",
    1: "following the wall",
    2: "turning and adjusting route",
    3: "adjusting the angle",
    4: "nearing the ball",
    5: "returning the ball"
}

state = 0
counter = 0
current_job = None
img_title = rospy.get_param('~image_title', 'photo.jpg')
camera = TakePhoto()


def laser_callback(data):
    global state
    global counter
    global current_job
    global motor_command_publisher

    motor_command = Twist()
    control = (None, None, None)

    if current_job is None and listener.color is None:
        return

    if current_job is None and listener.color is not None:
        current_job = listener.color
        state = 0

    if current_job is not None and listener.color is None:
        state = 5

    elif camera.take_picture(img_title):
        control = ball_control(img_title)

        if state < 3 and control[0]:
            state = 3

    # some thing is wrong, no object around me, I should find a wall first
    # if math.isnan(data.ranges[0]) and math.isnan(data.ranges[-1]) and data.ranges[len(data.ranges)/2]:
    #     state = 0

    if state == 0:  # nearing to wall
        if 0.5 < data.ranges[len(data.ranges)/2] < 2:
            state = 1
            motor_command.angular.z = +8

        else:
            motor_command.linear.x = 0.5

    elif state == 1:  # following the wall
        if data.ranges[0] > 4.2:  # moving away from wall, turn right a little
            motor_command.angular.z = -0.3

        elif data.ranges[0] < 3.8:  # getting closer to wall, turn left a little
            motor_command.angular.z = +0.3

        elif math.isnan(data.ranges[0]):  # wall is not my right, what? Try to find the wall, change state
            state = 2

        elif 0.5 < data.ranges[len(data.ranges)/2] < 1 or \
            0.5 < data.ranges[len(data.ranges)/2 - 60] < 1 or \
            0.5 < data.ranges[len(data.ranges)/2 + 60] < 1:  # there are an object on my way,
            motor_command.angular.z = +3                 # also there are a wall that a follow on my right, turn left

        else:
            motor_command.linear.x = 0.5

    elif state == 2:  # turning and adjusting route
        if counter < 3:
            motor_command.linear.x = 0.5
            counter += 1

        else:
            motor_command.angular.z = -3
            state = 1
            counter = 0

    elif state == 3:  #
        if not control[0]:
            state = 0

        if control[1] > 0.515:
            motor_command.angular.z = -0.1

        elif control[1] < 0.485:
            motor_command.angular.z = +0.1

        else:
            # Thread(target=os.popen, args=("""rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_right, effort: -5, duration: -1}" """,)).start()
            os.popen("""rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_right, effort: -5, duration: -1}" """).close()  #ok open the right arm
            os.popen("""rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_left, effort: 5, duration: -1}" """).close()  # ok open the left arm
            state = 4

    elif state == 4:  #
        if control[1] > 0.53:
            motor_command.angular.z = -0.1

        elif control[1] < 0.47:
            motor_command.angular.z = +0.1

        if control[2] < 0.0657:
            motor_command.linear.x = +0.3

        else:
            os.popen("""rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_right, effort: 5, duration: -1}" """).close()  # ok close the right arm
            os.popen("""rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_left, effort: -5, duration: -1}" """).close()  # ok close the left arm
            state = 5

    elif state == 5:
        motor_command.angular.z = +0.1
        motor_command.linear.x = -0.3
        os.popen("""rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_right, effort: 25, duration: -1}" """).close()  # ok close the right arm
        os.popen("""rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_left, effort: -25, duration: -1}" """).close()  # ok close the left arm

    motor_command_publisher.publish(motor_command)

    print 'State:', states[state]
    print 'Object Information:', control
    print 'Wanted Object Color: ', listener.color if listener.color else "---"
    print 'The distance to the rightmost scanned point is: ', data.ranges[0]
    print 'The distance to the leftmost scanned point is: ', data.ranges[-1]
    print 'The distance to the middle scanned point is: ', data.ranges[len(data.ranges)/2]
    print ""


# This is the method we initialize everything
def explorer_node():
    print "1"
    # We must always do this when starting a ROS node - and it should be the first thing to happen
    rospy.init_node('amble')
    print "2"
    # Here we declare that we are going to publish "Twist" messages to the topic /cmd_vel_mux/navi. It is defined as
    # global because we are going to use this publisher in the laser_callback.
    global motor_command_publisher
    motor_command_publisher = rospy.Publisher('/cmd_vel_mux/input/navi', Twist, queue_size = 10)
    print "3"
    # Here we set the function laser_callback to recieve new laser messages when they arrive
    rospy.Subscriber("/scan", LaserScan, laser_callback, queue_size=1000)
    print "4"
    # spin is an infinite loop but it lets callbacks to be called when a new data available. That means spin keeps
    # this node not terminated and run the callback when nessessary.
    rospy.spin()


'''
class GoToPose():
    def __init__(self):

        self.goal_sent = False

        # What to do if shut down (e.g. Ctrl-C or failure)
        rospy.on_shutdown(self.shutdown)

        # Tell the action client that we want to spin a thread by default
        self.move_base = actionlib.SimpleActionClient("move_base", MoveBaseAction)
        rospy.loginfo("Wait for the action server to come up")

        # Allow up to 5 seconds for the action server to come up
        self.move_base.wait_for_server(rospy.Duration(5))

    def goto(self, pos, quat):

        # Send a goal
        self.goal_sent = True
        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = 'map'
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose = Pose(Point(pos['x'], pos['y'], 0.000),
                                     Quaternion(quat['r1'], quat['r2'], quat['r3'], quat['r4']))

        # Start moving
        self.move_base.send_goal(goal)

        # Allow TurtleBot up to 120 seconds to complete task
        success = self.move_base.wait_for_result(rospy.Duration(120))

        state = self.move_base.get_state()
        result = False

        if success and state == GoalStatus.SUCCEEDED:
            result = True
        else:
            self.move_base.cancel_goal()

        self.goal_sent = False
        return result

    def shutdown(self):
        if self.goal_sent:
            self.move_base.cancel_goal()
        rospy.loginfo("Stop")
        rospy.sleep(1)


if __name__ == '__main__':
    try:
        rospy.init_node('nav_test', anonymous=False)
        navigator = GoToPose()

        # Customize the following values so they are appropriate for your location
        position = {'x': 0.000 'y': 0.000}
        quaternion = {'r1': 0.000, 'r2': 0.000, 'r3': 0.000, 'r4': 1.000}

        rospy.loginfo("Going to inital position to leave to ball! Hooray!!")
        success = navigator.goto(position, quaternion)

        if success:
            rospy.loginfo("reached initial position leaving the ball!")
            os.popen(
                """rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_right, effort: -5, duration: -1}" """).close()  # ok open the right arm
            os.popen(
                """rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_left, effort: 5, duration: -1}" """).close()  # ok open the left arm

            # left the ball. Now move back and close your arms
            position = {'x': 0.000 'y': -2.000}
            quaternion = {'r1': 0.000, 'r2': 0.000, 'r3': 0.000, 'r4': -1.000}
            rospy.loginfo("Stepping back from the ball!")
            successInner = navigator.goto(position, quaternion)
            if success:
                # Okay stepped back from the ball. Now it can close its arms
                os.popen(
                    """rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_right, effort: 5, duration: -1}" """).close()  # ok close the right arm
                os.popen(
                    """rosservice call /gazebo/apply_joint_effort "{joint_name: beyz_left, effort: -5, duration: -1}" """).close()  # ok close the left arm
            else:
                rospy.sleep(1)

        else:
            rospy.loginfo("I got lost! :(")
            rospy.sleep(1)

    except rospy.ROSInterruptException:
        rospy.loginfo("Ctrl-C caught. Quitting")
'''

if __name__ == '__main__':
    print "program started"
    explorer_node()
    listener.stop = True
