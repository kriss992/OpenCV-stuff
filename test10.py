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
crop_area = (0, 0, 0, 0)
found = None
pick = False
moved = False
first_pick = False
target = None
second_img = False
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

def detector(gray, template, tH, tW, treshold):
    found = None
    #for scale in np.linspace(1.0, 1.0, 1)[::-1]:
    for scale in np.linspace(0.6, 1.0, 5)[::-1]:
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
    print("maxVal:", str(maxVal), "minVal:", str(minVal))
    if maxVal >= treshold:
        #           0       1      2    3       4       5       6       7
        return (startX, startY, endX, endY, x_cent, y_cent, maxVal, minVal, True)
    else:
        #           0       1      2    3       4       5       6       7     8
        return (startX, startY, endX, endY, x_cent, y_cent, maxVal, minVal, None)

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
    w = int(frame.shape[1] * 0.25)
    h = int(frame.shape[0] * 0.25)
    frame = cv2.resize(frame, (w, h), interpolation=cv2.INTER_AREA)
    frame1 = cv2.resize(frame, (w, h), interpolation=cv2.INTER_AREA)
    #frame = imutils.resize(frame, width=1200)
    #frame1 = imutils.resize(frame, width=1200)

    # Get height and width of webcam frame
    height, width = frame1.shape[:2]
    #print("height:",str(height),"width:",str(width))
    pick_box_width = 20
    pick_box_height = 20
    cv2.setMouseCallback('Object Detector', onClick)
    #cropped = cv2.Canny(cropped, 50, 200)

    if first_pick == False:
        ''''''
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
        #cv2.imshow("Target", target)
        #_, target = cv2.threshold(target, 50, 220, cv2.THRESH_BINARY)
        target = cv2.Canny(target, 50, 200)
        #(tH_t, tW_t) = target.shape[:2]
        #target = imutils.resize(target, width=int(tW_t / 2))
        (tH, tW) = target.shape[:2]
        pick = False
    if moved:
        cv2.rectangle(frame1, (int(mx - pick_box_width / 2), int(my - pick_box_height / 2)), (int(mx + pick_box_width / 2), int(my + pick_box_height / 2)), (255, 0, 0), 1)
        #cv2.rectangle(frame1, (int(mx - pick_box_width / 2), int(my - pick_box_height / 2)), (int(mx + pick_box_width / 2), int(my + pick_box_height / 2)), (255, 0, 0), 1)
        cv2.circle(frame1, (mx, my), 3, (255, 0, 0), -0)
    if first_pick:
        treshold = 0.4
        #matches2 = detector(cropped, target, tH, tW, treshold)
        #matches2 = detector(frame, target, tH, tW, treshold)
        if second_img:
            matches2 = detector(crop, target, tH, tW, treshold)
        else:
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
            x_c = int(x1_t + w_t / 2)
            y_c = int(y1_t + h_t / 2)
            if (w_t >= (width / 2)) or (h_t >= (height / 2)):
                x1 = int(x_c - pick_box_width / 2)
                if x1 < 0: x1 = 0
                y1 = int(y_c - pick_box_height / 2)
                if y1 < 0: y1 = 0
                x2 = int(x_c + pick_box_width / 2)
                if x2 < 0: x2 = 0
                y2 = int(y_c + pick_box_height / 2)
                if y2 < 0: y2 = 0
                x1r = int(x1 - pick_box_width * 2)
                if x1r < 0: x1r = 0
                y1r = int(y1 - pick_box_height * 2)
                if y1r < 0: y1r = 0
                x2r = int(x2 + pick_box_width * 2)
                if x2r < 0: x2r = 0
                y2r = int(y2 + pick_box_height * 2)
                if y2r < 0: y2r = 0
            else:
                x1 = x1_t
                if x1 < 0: x1 = 0
                y1 = y1_t
                if y1 < 0: y1 = 0
                x2 = x2_t
                if x2 < 0: x2 = 0
                y2 = y2_t
                if y2 < 0: y2 = 0
                x1r = int(x1 - pick_box_width * 2)
                if x1r < 0: x1r = 0
                y1r = int(y1 - pick_box_height * 2)
                if y1r < 0: y1r = 0
                x2r = int(x2 + pick_box_width * 2)
                if x2r < 0: x2r = 0
                y2r = int(y2 + pick_box_height * 2)
                if y2r < 0: y2r = 0
            #print("x1:", str(x1), "y1:", str(y1), "x2:", str(x2), "y2:", str(y2))
            if matches2[6] >= treshold:
                cv2.rectangle(frame1, (x1r, y1r), (x2r, y2r), (255, 0, 0), 1)
                target = frame[y1:y2, x1:x2]
                cv2.imshow('Target', target)
                target = cv2.Canny(target, 50, 200)
                (tH, tW) = target.shape[:2]
                crop = frame.copy()
                crop = crop[y1r:y2r, x1r:x2r]
        else:
            '''first_pick = False'''
        second_img = True
    cv2.imshow('Object Detector', frame1)
    w = int(frame1.shape[1] * 2)
    h = int(frame1.shape[0] * 2)
    frame2 = cv2.resize(frame1, (w, h), interpolation=cv2.INTER_AREA)
    cv2.imshow('Object Detector Upsized', frame2)
    '''frame3 = cv2.Laplacian(frame2, cv2.CV_64F)
    cv2.imshow("Laplace", frame3)
    frame4 = cv2.GaussianBlur(frame3, (5, 5), cv2.BORDER_DEFAULT)
    cv2.imshow("Laplace-Gauss", frame4)
    frame5 = cv2.Canny(frame2, 50, 200)
    cv2.imshow("Canny", frame5)
    frame6 = cv2.GaussianBlur(frame5, (5, 5), cv2.BORDER_DEFAULT)
    cv2.imshow("Canny-Gauss", frame6)'''
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):  # 13 is the Enter Key
        break
    if key == ord("s"): # cancels detection procedure
        if(first_pick == False):
            first_pick = False
        else:
            first_pick = False
    if(fcout < 5):
        fcout = fcout + 1
    else:
        fcout = 0
cap.release()
cv2.destroyAllWindows()