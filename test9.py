import argparse
import sys
from matplotlib import pyplot as plt
import cv2
import imutils
import numpy as np
import pyautogui
np.set_printoptions(threshold=sys.maxsize)
crop = (0, 0, 0, 0)
matches2 = (0, 0, 0, 0)
found = None
pick = False
moved = False
first_pick = False
target = None
# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-t", "--template", required=False, help="Path to template image")
args = vars(ap.parse_args())

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
def detector2(gray, template, tH, tW):
    ''''''

def detector(gray, template, tH, tW):
    found = None
    #for scale in np.linspace(1.0, 1.0, 1)[::-1]:
    for scale in np.linspace(0.2, 1.0, 20)[::-1]:
        # resize the image according to the scale, and keep track
        # of the ratio of the resizing
        resized = imutils.resize(gray, width=int(gray.shape[1] * scale))
        r = gray.shape[1] / float(resized.shape[1])

        # if the resized image is smaller than the template, then break
        # from the loop
        if resized.shape[0] < tH or resized.shape[1] < tW:
            break

        # detect edges in the resized, grayscale image and apply template
        # matching to find the template in the image
        edged = cv2.Canny(resized, 50, 200)
        result = cv2.matchTemplate(edged, template, cv2.TM_CCOEFF)
        (minVal, maxVal, minLoc, maxLoc) = cv2.minMaxLoc(result)

        # check to see if the iteration should be visualized
        if args.get("visualize", False):
            # draw a bounding box around the detected region
            clone = np.dstack([edged, edged, edged])
            cv2.rectangle(clone, (maxLoc[0], maxLoc[1]), (maxLoc[0] + tW, maxLoc[1] + tH), (0, 0, 255), 2)
            cv2.imshow("Visualize", clone)
            cv2.waitKey(0)

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
    return (startX, startY, endX, endY, x_cent, y_cent, maxVal, minVal)

SCREEN_SIZE = (1920, 1080)
# define the codec
fourcc = cv2.VideoWriter_fourcc(*"XVID")
# create the video write object
out = cv2.VideoWriter("output.avi", fourcc, 20.0, (SCREEN_SIZE))
cap = cv2.VideoCapture(0)
fcout = 0
while True:
    img = pyautogui.screenshot(region=(0, 0, 1920, 1080))
    frame = np.array(img)
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    frame = imutils.resize(frame, width=1200)
    frame1 = frame

    # Get height and width of webcam frame
    height, width = frame1.shape[:2]
    pick_box_width = 50
    pick_box_height = 50
    cv2.setMouseCallback('Object Detector', onClick)
    top_left_x = int(width / 6)
    top_left_y = int((height / 2) + (height / 4))
    bottom_right_x = int((width / 6) * 5)
    bottom_right_y = int((height / 2) - (height / 4))
    cropped = frame1[bottom_right_y:top_left_y, top_left_x:bottom_right_x]
    # cropped = cv2.Canny(cropped, 50, 200)
    if pick:

        x1 = int(px - pick_box_width / 2)
        if x1 < 0: x1 = 0
        y1 = int(py - pick_box_height / 2)
        if y1 < 0: y1 = 0
        x2 = int(px + pick_box_width / 2)
        if x2 < 0: x2 = 0
        y2 = int(py + pick_box_height / 2)
        if y2 < 0: y2 = 0
        target = frame[y1:y2, x1:x2]
        #_, target = cv2.threshold(target, 50, 220, cv2.THRESH_BINARY)
        target = cv2.Canny(target, 50, 200)
        (tH, tW) = target.shape[:2]
        pick = False
    if moved:
        cv2.rectangle(frame1, (mx - int(pick_box_width / 2), my - int(pick_box_height / 2)), (mx +int (pick_box_width / 2), my + int(pick_box_height / 2)), (255, 0, 0), 1)
        cv2.circle(frame1, (mx, my), 3, (255, 0, 0), -0)
    if first_pick:
        matches2 = detector(cropped, target, tH, tW)
        target = frame[matches2[1] + bottom_right_y: matches2[3] + bottom_right_y, matches2[0] + top_left_x: matches2[2] + top_left_x]
        target = cv2.Canny(target, 50, 200)
        (tH, tW) = target.shape[:2]
        '''
        if (fcout >= 1):
            matches2 = detector(cropped, target, tH, tW)
            target = frame[matches2[1] + bottom_right_y : matches2[3] + bottom_right_y, matches2[0] + top_left_x : matches2[2] + top_left_x]
            target = cv2.Canny(target, 50, 200)
            (tH, tW) = target.shape[:2]
            fcout = 0
        '''
        cv2.rectangle(frame1, (matches2[0] + top_left_x, matches2[1] + bottom_right_y) , (matches2[2] + top_left_x, matches2[3] + bottom_right_y), (0, 0, 255), 1)
        cv2.circle(frame1, (matches2[4] + top_left_x, matches2[5] + bottom_right_y), 3, (255, 0, 0), -0)
    cv2.rectangle(frame1, (top_left_x, top_left_y), (bottom_right_x, bottom_right_y), 255, 1)
    cv2.imshow('Object Detector', frame1)
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):  # 13 is the Enter Key
        break
    if key == ord("s"): # cancels detection procedure
        if(first_pick == False):
            first_pick = True
        else:
            first_pick = False
    if(fcout < 5):
        fcout = fcout + 1
    else:
        fcout = 0
cap.release()
cv2.destroyAllWindows()