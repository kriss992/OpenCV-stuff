import argparse
import sys
from pynput import mouse
from imutils.video import FPS
import cv2
import numpy as np
import pyautogui
from directkeys import missCtrl

pick_box_width = 24
pick_box_height = 24
captured_screen_w = 800
captured_screen_h = 600
orginal_screen_w = 1920
orginal_screen_h = 1080
j_b1, j_b2, j_b3, j_b4 = False, False, False, False
np.set_printoptions(threshold=sys.maxsize)
crop = (0, 0, 0, 0)
matches2 = (0, 0, 0, 0)
crop_area = (0, 0, 0, 0)
found = None
pick = False
moved = False
first_pick = False
target = None
start_pick = False
sel = False
# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-t", "--template", required=False, help="Path to template image")
args = vars(ap.parse_args())

def on_move(x, y):
    global mx, my, moved
    if x >= 0:
        if x <= 800 and y <= 600:
            #mx = int(x / (orginal_screen_w / (orginal_screen_w / (orginal_screen_w / captured_screen_w))))
            #my = int(y / (orginal_screen_h / (orginal_screen_h / (orginal_screen_h / captured_screen_h))))
            moved = True
            mx, my = x, y
            #print("MOUSE MOVE: ", x, y)
        #moved = True
    listener.stop()
def on_click(x, y, button, pressed):
    global px, py, pick, first_pick, target_cord, sel
    if x >= 0:
        if x <= 800 and y <= 600:
            if str(button) == "Button.left":
                ''''''
                #print("SINGLE Left CLICK: ", x, y)
                #first_pick = False
                #target_cord = None
            if str(button) == "Button.right":
                first_pick = False
                target_cord = None
            if str(button) == "Button.middle":
                px, py = x, y
                pick = True
                first_pick = True
    listener.stop()
def on_scroll(x, y, dx, dy):
    listener.stop()
def start(frame, target):
    start = setTrack(frame, target)
    return start

def onClick(event, x, y, flags, param):
    global mx, my, px, py, pick, moved, first_pick, first_print, target_cord
    if event == cv2.EVENT_LBUTTONUP:
        #print("SINGLE CLICK: ", x, y)
        '''px, py = x, y
        pick = True
        first_pick = True
        first_print = True'''
    if event == cv2.EVENT_RBUTTONUP:
        first_pick = False
        target_cord = None
    if event == cv2.EVENT_MOUSEMOVE:
        mx, my = x, y
        #print("MOUSE MOVE: ", mx, my)
        moved = True

def detector(gray, template, tH, tW, treshold):
    found = None
    #for scale in np.linspace(1.0, 1.0, 1)[::-1]:
    for scale in np.linspace(0.1, 1.0, 20)[::-1]:
        #print(str(scale))
        # resize the image according to the scale, and keep track
        # of the ratio of the resizing
        w = int(gray.shape[1] * scale)
        h = int(gray.shape[0] * scale)
        resized = cv2.resize(gray, (w, h), interpolation=cv2.INTER_AREA)
        #resized = imutils.resize(gray, width=int(gray.shape[1] * scale))
        r = gray.shape[1] / float(resized.shape[1])

        # if the resized image is smaller than the template, then break
        # from the loop
        if resized.shape[0] < tH or resized.shape[1] < tW:
            break

        # detect edges in the resized, grayscale image and apply template
        # matching to find the template in the image
        edged = cv2.Canny(resized, 50, 200)
        #result = cv2.matchTemplate(edged, template, cv2.TM_CCOEFF)
        result = cv2.matchTemplate(edged, template, cv2.TM_CCOEFF_NORMED)
        (minVal, maxVal, minLoc, maxLoc) = cv2.minMaxLoc(result)

        # if we have found a new maximum correlation value, then update
        # the bookkeeping variable
        if found is None or maxVal > found[0]:
            found = (maxVal, minVal, maxLoc, minLoc, r)
    (maxVal, minVal, maxLoc, minLoc, r) = found
    (startX, startY) = (int(maxLoc[0] * r), int(maxLoc[1] * r))
    (endX, endY) = (int((maxLoc[0] + tW) * r), int((maxLoc[1] + tH) * r))
    width = endX - startX
    height = endY - startY
    x_cent = endX - int(width / 2)
    y_cent = endY - int(height / 2)
    #print("maxVal:", str(maxVal), "minVal:", str(minVal))
    if maxVal >= treshold:
        #           0       1      2    3       4       5       6       7
        return (startX, startY, endX, endY, x_cent, y_cent, maxVal, minVal, True)
    else:
        #           0       1      2    3       4       5       6       7     8
        return (startX, startY, endX, endY, x_cent, y_cent, maxVal, minVal, None)

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
        tracker = OPENCV_OBJECT_TRACKERS[args["tracker"]]()
    tracker_ret = tracker.init(frame, target)
    #fps = FPS().start()
    return tracker_ret#, fps

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
SCREEN_SIZE = (800, 600)
# define the codec
fourcc = cv2.VideoWriter_fourcc(*"XVID")
# create the video write object
out = cv2.VideoWriter("output.avi", fourcc, 20.0, (SCREEN_SIZE))
cap = cv2.VideoCapture(0)
fcout = 0

if __name__ == '__main__':
    while True:
        img = pyautogui.screenshot(region=(0, 0, captured_screen_w, captured_screen_h))
        frame = np.array(img)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        '''w = int(frame.shape[1] * 0.5)
        h = int(frame.shape[0] * 0.5)
        frame = cv2.resize(frame, (w, h), interpolation=cv2.INTER_AREA)'''
        frame1 = frame.copy()
        frame2 = frame.copy()
        frame1 = cv2.cvtColor(frame1, cv2.COLOR_RGB2GRAY)
        #frame = imutils.resize(frame, width=1200)
        #frame1 = imutils.resize(frame, width=1200)
        # Get height and width of webcam frame
        height, width = frame1.shape[:2]
        #print("height:",str(height),"width:",str(width))
        cv2.setMouseCallback('Object Detector', onClick)
        listener = mouse.Listener(
            #on_move=on_move,
            on_click=on_click#,
            #on_scroll=on_scroll
        )
        listener.start()
        #cropped = cv2.Canny(cropped, 50, 200)
        #start = start(frame, (1, 1, 5, 5))
        if pick:
            x1 = int(px - pick_box_width / 2)
            if x1 < 0: x1 = 0
            y1 = int(py - pick_box_height / 2)
            if y1 < 0: y1 = 0
            x2 = int(px + pick_box_width / 2)
            if x2 < 0: x2 = 0
            y2 = int(py + pick_box_height / 2)
            if y2 < 0: y2 = 0
            target_cord = (x1, y1, pick_box_width, pick_box_height)
            target = frame[y1:y2, x1:x2]
            target = cv2.Canny(target, 50, 200)
            (tH, tW) = target.shape[:2]
            return_data = setTrack(frame, target_cord)
            #fps = return_data[1]
            pick = False
        if moved:
            cv2.rectangle(frame1, (int(mx - pick_box_width / 2), int(my - pick_box_height / 2)), (int(mx + pick_box_width / 2), int(my + pick_box_height / 2)), (255, 0, 0), 1)
            cv2.circle(frame1, (mx, my), 3, (255, 0, 0), -0)
        if start_pick == False:
            target_cord_none = (5, 5, 5, 5)
            return_data_none = setTrack(frame, target_cord_none)
            #fps_none = return_data_none[1]
            start_pick = True
        if first_pick:
            if target_cord is not None:
                # grab the new bounding box coordinates of the object
                test = tracker.update(frame)
                (success, box) = test
                x = int(box[0])
                y = int(box[1])
                w = int(box[2])
                h = int(box[3])
                if success:
                    cor_x1 = x
                    cor_y1 = y
                    cor_x2 = x + w
                    cor_y2 = y + h
                    cent_x = int((x + (x + w)) / 2)
                    cent_y = int((y + (y + h)) / 2)
                    #missileControl(cent_x, cent_y, width, height)
                    #missCtrl(cent_x, cent_y, width, height)
                    #if w > height:
                    try:
                        target = frame[cor_y1:cor_y2, cor_x1:cor_x2]
                        target = cv2.Canny(target, 50, 200)
                        (tH, tW) = target.shape[:2]
                    except:
                        break
                    '''target = frame[cor_y1:cor_y2, cor_x1:cor_x2]
                    target = cv2.Canny(target, 50, 200)
                    (tH, tW) = target.shape[:2]'''
                    cv2.rectangle(frame1, (cor_x1, cor_y1), (cor_x2, cor_y2), (0, 255, 0), 2)
                    cv2.circle(frame1, (cent_x, cent_y), 3, (0, 255, 0), -1)
                    #fps.update()
                    #fps.stop()
                    info = [
                        ("Tracker ", args["tracker"]),
                        ("Success ", "Yes" if success else "No")#,
                        #("FPS ", "{:.2f}".format(fps.fps())),
                        #("CENTER ", "x:" + str(cent_x) + " y:" + str(cent_y)),
                        #("CORNERS ", x1:" + str(cor_x1) + " y1:" + str(cor_y1) + " x2:" + str(cor_x2) + " y2:" + str(cor_y2))
                    ]
                    # loop over the info tuples and draw them on our frame
                    for (i, (k, v)) in enumerate(info):
                        text = "{}: {}".format(k, v)
                        cv2.putText(frame1, text, (100, ((i * 20) + 100)), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
                    cv2.imshow('Object Detector', frame1)
                    #missCtrl(cent_x, cent_y, width, height)

                '''else:
                    treshold = 0.4
                    matches2 = detector(frame, target, tH, tW, treshold)
                    if matches2[8] != None:
                        cv2.circle(frame1, (matches2[4], matches2[5]), 3, (255, 0, 0), -0)
                        cv2.rectangle(frame1, (matches2[0], matches2[1]), (matches2[2], matches2[3]), 255, 1)
                        x1_t = int(matches2[0])
                        y1_t = int(matches2[1])
                        x2_t = int(matches2[2])
                        y2_t = int(matches2[3])
                        w_t = int(y2_t - y1_t)
                        h_t = int(x2_t - x1_t)
                        #print("w_t:", str(w_t), "h_t:", str(h_t))
                        x_c = int(x1_t + w_t / 2)
                        y_c = int(y1_t + h_t / 2)
                        if h_t > height / 2:
                            x1 = int(x_c - pick_box_width / 2)
                            if x1 < 0: x1 = 0
                            y1 = int(y_c - pick_box_height / 2)
                            if y1 < 0: y1 = 0
                            x2 = int(x_c + pick_box_width / 2)
                            if x2 < 0: x2 = 0
                            y2 = int(y_c + pick_box_height / 2)
                            if y2 < 0: y2 = 0
                        else:
                            x1 = int(matches2[0])
                            if x1 < 0: x1 = 0
                            y1 = int(matches2[1])
                            if y1 < 0: y1 = 0
                            x2 = int(matches2[2])
                            if x2 < 0: x2 = 0
                            y2 = int(matches2[3])
                            if y2 < 0: y2 = 0
                        if matches2[6] >= treshold:
                            target = frame[y1:y2, x1:x2]
                            cv2.imshow('Target', target)
                            target = cv2.Canny(target, 50, 200)
                            (tH, tW) = target.shape[:2]
                            target_cord = (x1, y1, tW, tH)
                            return_data = setTrack(frame, target_cord)
                            fps = return_data[1]
                        cv2.imshow('Object Detector', frame1)'''
        else:
            cv2.imshow('Object Detector', frame1)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):  # 13 is the Enter Key
            break
        if key == ord("j"):  # 13 is the Enter Key
            #j.set_axis(pyvjoy.HID_USAGE_X, 0x2000)
            ''''''
        if key == ord("k"):  # 13 is the Enter Key
            ''''''
            #j.set_axis(pyvjoy.HID_USAGE_Y, 0x6000)
        if key == ord("l"):  # 13 is the Enter Key
            ''''''
            #j.set_axis(pyvjoy.HID_USAGE_X, 0x6000)
        if key == ord("o"):  # 13 is the Enter Key
            ''''''
            #j.set_axis(pyvjoy.HID_USAGE_Y, 0x2000)
        if key == ord("s"): # cancels detection procedure
            first_pick = False
            target_cord = None
            ''''''
        if key == ord("c"):  # cancels detection procedure
            px = mx
            py = my
            pick = True
            first_pick = True
            sel = True
        if key == ord("x"):  # cancels detection procedure
            first_pick = False
            target_cord = None

        if(fcout < 5):
            fcout = fcout + 1
        else:
            fcout = 0
    cap.release()
    cv2.destroyAllWindows()