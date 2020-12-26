from __future__ import print_function
import rospy
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError


# https://github.com/markwsilliman/turtlebot/
class TakePhoto:
    def __init__(self):

        self.bridge = CvBridge()
        self.image_received = False

        # Connect image topic
        img_topic = "/camera/rgb/image_raw"
        self.image_sub = rospy.Subscriber(img_topic, Image, self.callback)

        # Allow up to one second to connection
        rospy.sleep(1)

    def callback(self, data):

        # Convert image to OpenCV format
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            print(e)

        self.image_received = True
        self.image = cv_image

    def take_picture(self, img_title):
        if self.image_received:
            # Save an image
            cv2.imwrite(img_title, self.image)
            return True
        else:
            return False


if __name__ == '__main__':

    # Initialize
    rospy.init_node('take_photo', anonymous=False)
    camera = TakePhoto()

    # Take a photo

    # Use '_image_title' parameter from command line
    # Default value is 'photo.jpg'
    img_title = rospy.get_param('~image_title', 'photo.jpg')
    print(img_title)

    if camera.take_picture(img_title):
        rospy.loginfo("Saved image " + img_title)
    else:
        rospy.loginfo("No images received")

    # Sleep to give the last log messages time to be sent
    rospy.sleep(1)
