# import the necessary packages
import argparse
from pynput import mouse
import pyautogui
import numpy as np
import imutils
import cv2

# initialize the FPS throughput estimator
from imutils.video import FPS

pick = False
moved = False
first_pick = False
wp = 10
hp = 10
target = None
fps = None
tracker = None
frame = None
frame1 = None
def on_move(x, y):
    global mx, my, moved
    if x >= 0:
        mx = int(x / (1920 / 1200))
        my = int(y / (1920 / 1200))
        #mx, my = x, y
        # print("MOUSE MOVE: ", x, y)
        moved = True
    listener.stop()
def on_click(x, y, button, pressed):
    global px, py, pick, first_pick
    if x >= 0:
        if str(button) == "Button.left":
            print("SINGLE Left CLICK: ", x, y)
            #px, py = x, y
            px = int(x / (1920 / 1200))
            py = int(y / (1920 / 1200))
            pick = True
            first_pick = True
    listener.stop()
def on_scroll(x, y, dx, dy):
    listener.stop()
def onClick(event, x, y, flags, param):
    global mx, my, px, py, pick, moved, first_pick
    if event == cv2.EVENT_LBUTTONUP:
        print("SINGLE CLICK: ", x, y)
        px, py = x, y
        pick = True
        first_pick = True
    if event == cv2.EVENT_MOUSEMOVE:
        mx, my = x, y
        # print("MOUSE MOVE: ", x, y)
        moved = True
def setTrack(frame, target):
    global tracker
    # if we are using OpenCV 3.2 OR BEFORE, we can use a special factory
    # function to create our object tracker
    if int(major) == 3 and int(minor) < 3:
        tracker = cv2.Tracker_create(args["tracker"].upper())
    # otherwise, for OpenCV 3.3 OR NEWER, we need to explicity call the
    # approrpiate object tracker constructor:
    else:
        # initialize a dictionary that maps strings to their corresponding
        # grab the appropriate object tracker using our dictionary of
        # OpenCV object tracker objects
        #tracker = cv2.TrackerKCF_GRAY
        tracker = OPENCV_OBJECT_TRACKERS[args["tracker"]]()
        #tracker = cv2.TrackerKCF_GRAY
    tracker_ret = tracker.init(frame, target)
    fps = FPS().start()
    return tracker_ret, fps


# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", type=str,
                help="path to input video file")
ap.add_argument("-t", "--tracker", type=str, default="csrt",
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
SCREEN_SIZE = (1920, 1080)
# define the codec
fourcc = cv2.VideoWriter_fourcc(*"XVID")
# create the video write object
out = cv2.VideoWriter("output.avi", fourcc, 20.0, (SCREEN_SIZE))
frame_count = 0
while True:
    # make a screenshot
    img = pyautogui.screenshot(region=(0, 0, 1920, 1080))
    # executes if image is available
    if img:
        # convert these pixels to a proper numpy array to work with OpenCV
        frame = np.array(img)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        #frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        frame = imutils.resize(frame, width=1200)
        #frame = cv2.Canny(frame, 50, 200)
        frame1 = frame.copy()
        # frame1 = cv2.Canny(frame1, 50, 200)
        #frame1 = cv2.Canny(frame1, 50, 200)
        if frame1 is None:
            break
        #(H, W) = frame.shape[:2]
        #cv2.setMouseCallback("Frame", onClick)

        listener = mouse.Listener(
            on_move=on_move,
            on_click=on_click,
            on_scroll=on_scroll)
        listener.start()
        if moved:
            cv2.rectangle(frame1, (mx - wp, my - hp), (mx + wp, my + hp), (255, 0, 0), 1)
            cv2.circle(frame1, (mx, my), 3, (255, 0, 0), -0)
        if pick:
            x1 = int(px - wp)
            if x1 < 0: x1 = 0
            y1 = int(py - hp)
            if y1 < 0: y1 = 0
            target = (x1, y1 , wp * 2, hp * 2)
            #target = frame[y1:y2, x1:x2]
            # target = cv2.cvtColor(target, cv2.COLOR_BRG2GRAY)
            # cv2.imshow("Target", target)
            # _, target = cv2.threshold(target, 50, 220, cv2.THRESH_BINARY)
            # target = cv2.Canny(target, 50, 200)
            # (tH_t, tW_t) = target.shape[:2]
            # target = imutils.resize(target, width=int(tW_t / 2))
            #(tH, tW) = target.shape[:2]
            return_data = setTrack(frame, target)
            fps = return_data[1]
            pick = False
        if first_pick:
            if target is not None:
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
                    # initialize the set of information we'll be displaying on
                    # the frame
                    fps.update()
                    fps.stop()
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
                        cv2.putText(frame1, text, (100, ((i * 20) + 100)), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
                else:
                    print("izgubil","frame",str(frame_count))
        #_, frame = cv2.threshold(frame, 50, 220, cv2.THRESH_BINARY)
        #frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow("Frame", frame1)
        print("frame", str(frame_count))
        frame_count = frame_count + 1
        # cv2.imshow("Frame1", frame1)
        key = cv2.waitKey(1) & 0xFF
        # if the 's' key is selected, we are going to "select" a bounding
        # box to track
        if key == ord("f"):
            first_pick = False
        if key == ord("d"):
            ''''''
        if key == ord("e"):
            ''''''
        if key == ord("s"):
            x1 = int(mx - wp)
            if x1 < 0: x1 = 0
            y1 = int(my - hp)
            if y1 < 0: y1 = 0
            target = (x1, y1 , wp * 2, hp * 2)
            return_data = setTrack(frame1, target)
            fps = return_data[1]
            first_pick = True
        elif key == ord("q"):
            break
    else:
        print("nema slike")
        break
cv2.destroyAllWindows()
out.release()
