# USAGE
# python opencv_object_tracking.py
# python opencv_object_tracking.py --video dashcam_boston.mp4 --tracker csrt

# import the necessary packages
import keyboard
import pyautogui
import numpy as np
from imutils.video import VideoStream
from imutils.video import FPS
import argparse
import imutils
import time
import cv2

center = []
corner = []
mx, my = -1, -1
# initialize the FPS throughput estimator
tgt_num = 0
fps = None
first = True
pick = False
moved = False
first_pick = False
# tracker = None
global tracker


def onClick(event, x, y, flags, param):
    global mx, my, px, py, pick, moved, first_pick, first_print
    if event == cv2.EVENT_LBUTTONUP:
        print("SINGLE CLICK: ", x, y)
        px, py = x, y
        pick = True
        first_pick = True
        first_print = True
    if event == cv2.EVENT_MOUSEMOVE:
        mx, my = x, y
        #print("MOUSE MOVE: ", mx, my)
        moved = True


# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", type=str,
                help="path to input video file")
ap.add_argument("-t", "--tracker", type=str, default="kcf",
                help="OpenCV object tracker type")
args = vars(ap.parse_args())

# extract the OpenCV version info
(major, minor) = cv2.__version__.split(".")[:2]

# initialize a dictionary that maps strings to their corresponding
# OpenCV object tracker implementations
OPENCV_OBJECT_TRACKERS = {
    "csrt": cv2.TrackerCSRT_create,
    "kcf": cv2.TrackerKCF_create,
    "boosting": cv2.TrackerBoosting_create,
    "mil": cv2.TrackerMIL_create,
    "tld": cv2.TrackerTLD_create,
    "medianflow": cv2.TrackerMedianFlow_create,
    "mosse": cv2.TrackerMOSSE_create
}


def setTrack():
    # if we are using OpenCV 3.2 OR BEFORE, we can use a special factory
    # function to create our object tracker
    if int(major) == 3 and int(minor) < 3:
        tracker = cv2.Tracker_create(args["tracker"].upper())
    # otherwise, for OpenCV 3.3 OR NEWER, we need to explicity call the
    # approrpiate object tracker constructor:
    else:
        # initialize a dictionary that maps strings to their corresponding
        # OpenCV object tracker implementations
        OPENCV_OBJECT_TRACKERS = {
            "csrt": cv2.TrackerCSRT_create,
            "kcf": cv2.TrackerKCF_create,
            "boosting": cv2.TrackerBoosting_create,
            "mil": cv2.TrackerMIL_create,
            "tld": cv2.TrackerTLD_create,
            "medianflow": cv2.TrackerMedianFlow_create,
            "mosse": cv2.TrackerMOSSE_create
        }

        # grab the appropriate object tracker using our dictionary of
        # OpenCV object tracker objects
        tracker = OPENCV_OBJECT_TRACKERS[args["tracker"]]()


# initialize the bounding box coordinates of the object we are going
# to track
initBB = None

SCREEN_SIZE = (1920, 1080)
# define the codec
fourcc = cv2.VideoWriter_fourcc(*"XVID")
# create the video write object
out = cv2.VideoWriter("output.avi", fourcc, 20.0, (SCREEN_SIZE))

# loop over frames from the video stream

while True:
    # corner = []
    # center = []
    global tracker
    # make a screenshot
    img = pyautogui.screenshot(region=(0, 0, 1920, 1080))
    # convert these pixels to a proper numpy array to work with OpenCV
    frame = np.array(img)
    # convert colors from BGR to RGB
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    #frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #frame_no = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    frame = imutils.resize(frame, width=1200)
    frame1 = imutils.resize(frame, width=1200)
    #frame = cv2.Canny(frame, 50, 200)
    (H, W) = frame.shape[:2]
    #frame1 = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
    # frame1 = cv2.Laplacian(frame1, cv2.CV_64F)
    # frame1 = cv2.Canny(frame1, 50, 200)
    # _, frame1 = cv2.threshold(frame1, 50, 220, cv2.THRESH_BINARY)
    # write the frame cv2.THRESH_BINARY_INV
    # out.write(frame1)

    # check to see if we have reached the end of the stream
    if frame is None:
        break

    # resize the frame (so we can process it faster) and grab the
    # frame dimensions
    #frame1 = imutils.resize(frame1, width=1300)
    #(H, W) = frame1.shape[:2]
    #frame_no = imutils.resize(frame_no, width=1300)
    #(H1, W1) = frame_no.shape[:2]
    # print("frame H, W", H, W, str(frame1.shape))

    # check to see if we are currently tracking an object
    if initBB is not None:
        # grab the new bounding box coordinates of the object
        #print("radi---1")
        test = tracker.update(frame)
        #print("test:",str(test))
        #(success, box) = tracker.update(frame)
        (success, box) = test
        x = int(box[0])
        y = int(box[1])
        w = int(box[2])
        h = int(box[3])
        #print("sucess:",str(success),"x:", str(x), "y:", str(y), "w:", str(w), "h:", str(h))
        #print("success:",str(success))
        #print("box:",str(box))
        # check to see if the tracking was a success
        if success:
            #(x, y, w, h) = [int(v) for v in box]
            cor_x1 = x
            cor_y1 = y
            cor_x2 = x + w
            cor_y2 = y + h
            cent_x = int((x + (x + w)) / 2)
            cent_y = int((y + (y + h)) / 2)

            cv2.rectangle(frame1, (cor_x1, cor_y1), (cor_x2, cor_y2), (0, 255, 0), 2)
            cv2.circle(frame1, (cent_x, cent_y), 3, (0, 255, 0), -1)
            #print("radi---2")
        # update the FPS counter
        fps.update()
        fps.stop()

        # initialize the set of information we'll be displaying on
        # the frame
        info = [
            ("Tracker ", args["tracker"]),
            ("Success ", "Yes" if success else "No"),
            ("FPS ", "{:.2f}".format(fps.fps())),
            ("CENTER ", "x:" + str(cent_x) + " y:" + str(cent_y)),
            ("CORNERS ", "x1:" + str(cor_x1) + " y1:" + str(cor_y1) + " x2:" + str(cor_x2) + " y2:" + str(cor_y2))
            # ("CENTER ", "x:" + str(center[0]) + " y:" + str(center[1]))
        ]

        # loop over the info tuples and draw them on our frame
        for (i, (k, v)) in enumerate(info):
            text = "{}: {}".format(k, v)
            cv2.putText(frame1, text, (10, ((i * 20) + 20)), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    # show the output frame
    cv2.setMouseCallback("Frame", onClick)

    if moved:
        cv2.rectangle(frame1, (mx - 50, my - 50), (mx + 50, my + 50), (255, 0, 0), 1)
        cv2.circle(frame1, (mx, my), 3, (255, 0, 0), -0)
    if pick:
        x1 = px - 50
        y1 = py - 50
        h = 100
        w = 100
        # print("moj BB", x1,y1,h,w)
        initBB = (x1, y1, h, w)
        # if we are using OpenCV 3.2 OR BEFORE, we can use a special factory
        # function to create our object tracker
        if int(major) == 3 and int(minor) < 3:
            tracker = cv2.Tracker_create(args["tracker"].upper())
        # otherwise, for OpenCV 3.3 OR NEWER, we need to explicity call the
        # approrpiate object tracker constructor:
        else:
            # grab the appropriate object tracker using our dictionary of
            # OpenCV object tracker objects
            tracker = OPENCV_OBJECT_TRACKERS[args["tracker"]]()
        tracker_ret = tracker.init(frame, initBB)
        print(str(tracker_ret))
        fps = FPS().start()
        pick = False

    cv2.imshow("Frame", frame1)
    key = cv2.waitKey(1) & 0xFF

    # if the 's' key is selected, we are going to "select" a bounding
    # box to track
    if key == ord("f"):
        ''''''
    if key == ord("d"):
        ''''''
    if key == ord("e"):
        ''''''
        initBB = None
    if key == ord("s"):
        x1 = mx - 50
        y1 = my - 50
        h = 100
        w = 100
        initBB = (x1, y1, h, w)
        if int(major) == 3 and int(minor) < 3:
            tracker = cv2.Tracker_create(args["tracker"].upper())
        else:
            tracker = OPENCV_OBJECT_TRACKERS[args["tracker"]]()
        tracker.init(frame, initBB)
        fps = FPS().start()
        pick = False
    elif key == ord("q"):
        break
cv2.destroyAllWindows()
out.release()
