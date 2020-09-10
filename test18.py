import argparse
import select
import socket
import sys
from pynput import mouse
from imutils.video import FPS
import cv2
import numpy as np
import pyautogui
from directkeys import missCtrl
from server import get, set
centx, centy = 0, 0
control_data = "***"
pick_box_width = 24
pick_box_height = 24
captured_screen_w = 800
captured_screen_h = 600
orginal_screen_w = 1920
orginal_screen_h = 1080
in_min_w, in_max_w, in_min_h, in_max_h, out_min_x, out_max_x, out_min_y, out_max_y = 0, 799, 0, 599, -5, 5, 5, -5
np.set_printoptions(threshold=sys.maxsize)
pick = False
moved = False
first_pick = False
target = None
start_pick = False
server = socket.socket()
host = "127.0.0.1"
port = 54000
size = 1024
server.bind((host,port))
server.listen(5)
input = [server]
running = 1
def map(in_val, in_min, in_max, out_min, out_max):
    out_val = out_min + ((in_val - in_min) * (out_max - out_min) / (in_max - in_min))
    return out_val
def onClick(event, x, y, flags, param):
    global mx, my, px, py, pick, moved, first_pick, first_print, target_cord
    if event == cv2.EVENT_LBUTTONUP:
        px, py = x, y
        pick = True
        first_pick = True
    if event == cv2.EVENT_RBUTTONUP:
        first_pick = False
        target_cord = None
    if event == cv2.EVENT_MOUSEMOVE:
        mx, my = x, y
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
        tracker = OPENCV_OBJECT_TRACKERS[args["tracker"]]()
    tracker_ret = tracker.init(frame, target)
    #fps = FPS().start()
    return tracker_ret#, fps
def comm(up):
    inputready,outputready,exceptready = select.select(input,[],[],0)
    for s in inputready:
        if s == server:
            # handle the server socket
            client, address = server.accept()
            input.append(client)
        elif s == sys.stdin:
            # handle standard input
            junk = sys.stdin.readline()
            running = 0
        else:
            # handle all other sockets
            data = s.recv(size)
            if data:
                s.send(up.encode())
                data_str= data.decode("utf-8")
                #print(data_str)
                return data_str
            else:
                s.close()
                input.remove(s)
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
fourcc = cv2.VideoWriter_fourcc(*"XVID")
out = cv2.VideoWriter("output.avi", fourcc, 20.0, (SCREEN_SIZE))
cap = cv2.VideoCapture(0)
if __name__ == '__main__':
    while True:
        #up = "moj odgovor"
        primljeno = comm(control_data)
        img = pyautogui.screenshot(region=(0, 0, captured_screen_w, captured_screen_h))
        frame = np.array(img)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        frame1 = frame.copy()
        frame2 = frame.copy()
        frame1 = cv2.cvtColor(frame1, cv2.COLOR_RGB2GRAY)
        height, width = frame1.shape[:2]
        cv2.setMouseCallback('Object Detector', onClick)
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
            pick = False
        if moved:
            cv2.rectangle(frame1, (int(mx - pick_box_width / 2), int(my - pick_box_height / 2)), (int(mx + pick_box_width / 2), int(my + pick_box_height / 2)), (255, 0, 0), 1)
            cv2.circle(frame1, (mx, my), 3, (255, 0, 0), -0)
        if start_pick == False:
            target_cord_none = (5, 5, 5, 5)
            return_data_none = setTrack(frame, target_cord_none)
            start_pick = True
        if first_pick:
            if target_cord is not None:
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
                    try:
                        target = frame[cor_y1:cor_y2, cor_x1:cor_x2]
                        target = cv2.Canny(target, 50, 200)
                        (tH, tW) = target.shape[:2]
                    except:
                        break
                    cv2.rectangle(frame1, (cor_x1, cor_y1), (cor_x2, cor_y2), (0, 255, 0), 2)
                    cv2.circle(frame1, (cent_x, cent_y), 3, (0, 255, 0), -1)
                    info = [
                        ("Tracker ", args["tracker"]),
                        ("Success ", "Yes" if success else "No")#,
                    ]
                    # loop over the info tuples and draw them on our frame
                    for (i, (k, v)) in enumerate(info):
                        text = "{}: {}".format(k, v)
                        cv2.putText(frame1, text, (100, ((i * 20) + 100)), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
                    cv2.imshow('Object Detector', frame1)
                    center_x_comp = int(width / 2)
                    center_y_comp = int(height / 2 + 10)
                    if cent_x < center_x_comp - 5:
                        #LEFT
                        if cent_x < center_x_comp + 100:
                            centx = -2
                        else:
                            centx = -0.75
                    if cent_x > center_x_comp + 5:
                        #RIGHT
                        if cent_x > center_x_comp - 100:
                            centx = 2
                        else:
                            cent = 0.75
                    if cent_y < center_y_comp - 5:
                        #UP
                        if center_y_comp < center_y_comp + 100:
                            centy = 2
                        else:
                            centy = 0.75
                    if cent_y > center_y_comp + 5:
                        #DOWN
                        if cent_y > center_y_comp - 100:
                            centy = -2
                        else:
                            centy = -0.75
                    #missCtrl(cent_x, cent_y, width, height)
                    #print("cent_x", str(cent_x), "cent_y", str(cent_y))
                    cent_x_cor = map(cent_x, in_min_w, in_max_w, out_min_x, out_max_x)
                    cent_y_cor = map(cent_y, in_min_h, in_max_h, out_min_y, out_max_y)
                    #cent_x_cor = round(cent_x_cor, 5)
                    #cent_y_cor = round(cent_y_cor, 5)
                    #print("cent_x", str(cent_x), "cent_x_cor", str(cent_x_cor), "cent_y", str(cent_y), "cent_y_cor", str(cent_y_cor))
                    #     array          0                    1                          2                 3                     4                 5
                    control_data = str(cent_x_cor) + "," + str(cent_y_cor) + "," + str(width) + "," + str(height) + "," + str(centx) + "," + str(centy)
                    centx, centy, cent_x_cor, cent_y_cor = 0, 0, 0, 0
                else:
                    control_data = "***"
        else:
            cv2.imshow('Object Detector', frame1)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break
        if key == ord("c"):
            first_pick = False
            target_cord = None
    cap.release()
    cv2.destroyAllWindows()