import argparse
import sys
import cv2
import imutils
import numpy as np
import pyautogui

matches = (0, 0, 0, 0)
found = None
pick = False
moved = False
first_pick = False
target = None
# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-t", "--template", required=False, help="Path to template image")
args = vars(ap.parse_args())
def search(frame, ROI_cent_x, ROI_cent_y, ROI_width, ROI_height, treshold):
    height, width = frame.shape[:2]
    y_max = height
    x_max = width
    '''
    column_start = ROI_cent_x - int(ROI_width / 2)
    if column_start < 0: column_start = 0
    if column_start >= width: column_start = width - 1
    column_end = ROI_cent_x + int(ROI_width / 2)
    if column_end < 0: column_end = 0
    if column_end >= width: column_end = width - 1
    row_start = ROI_cent_y - int(ROI_height / 2)
    if row_start < 0: row_start = 0
    if row_start >= height: row_start = height - 1
    row_end = ROI_cent_y + int(ROI_height / 2)
    if row_end < 0: row_end = 0
    if row_end >= height: row_end = height - 1
    '''
    top_left_x = ROI_cent_x - int(ROI_width / 2)
    if top_left_x < 0: top_left_x = 0
    if top_left_x >= width: top_left_x = width - 1
    bottom_right_x = ROI_cent_x + int(ROI_width / 2)
    if bottom_right_x < 0: bottom_right_x = 0
    if bottom_right_x >= width: bottom_right_x = width - 1
    top_left_y = ROI_cent_y - int(ROI_height / 2)
    if top_left_y < 0: top_left_y = 0
    if top_left_y >= height: top_left_y = height - 1
    bottom_right_y = ROI_cent_y + int(ROI_height / 2)
    if bottom_right_y < 0: bottom_right_y = 0
    if bottom_right_y >= height: bottom_right_y = height - 1

    sum = mask(frame, top_left_y, bottom_right_y, top_left_x, bottom_right_x, ROI_cent_x, ROI_cent_y)
    count = 0
    for y in range(0, y_max):
        if (y < ROI_height - 1):
            if (y == 0):
                temp1 = 1
            else:
                temp1 = y + 1
            ROI_height_temp1 = ROI_height - temp1
            ROI_height_temp2 = ROI_height - ROI_height_temp1
            top_left_y = 0
            bottom_right_y = top_left_y + ROI_height_temp2 - 1
        elif ((ROI_height + y) > (y_max - 1)):
            ROI_height_temp2 = y_max - y
            top_left_y = y_max - ROI_height_temp2 - 1
            bottom_right_y = 0
        else:
            ROI_height_temp2 = ROI_height
            top_left_y = y
            bottom_right_y = y + ROI_height_temp2 - 1
        for x in range(0, x_max):
            if (x < ROI_width - 1):
                if (x == 0):
                    temp1 = 1
                else:
                    temp1 = y + 1
                ROI_width_temp1 = ROI_width - temp1
                ROI_width_temp2 = ROI_width - ROI_width_temp1
                top_left_x = 0
                bottom_right_x = top_left_x + ROI_width_temp2 - 1
            elif ((ROI_width + x) > (x_max - 1)):
                ROI_width_temp2 = x_max - x
                top_left_x = x_max - ROI_width_temp2 - 1
                bottom_right_x = 0
            else:
                ROI_width_temp2 = ROI_width
                top_left_x = x
                bottom_right_x = x + ROI_width_temp2 - 1
            sum1 = mask(frame, top_left_y, bottom_right_y, top_left_x, bottom_right_x, ROI_cent_x, ROI_cent_y)

            #string = str(count) + "."
            #print(string, "ROI_width_temp2", str(ROI_width_temp2), "ROI_height_temp2", str(ROI_height_temp2))
            #print(string, "ROI_width_temp2", str(ROI_width_temp2), "ROI_height_temp2", str(ROI_height_temp2))
            #count = count + 1
    return sum
'''
    count = 0
    row_first = 0
    column_first = 0
    for y in range(0, y_max):
        if (y < ROI_height - 1):
            if(y == 0):
                top_left_y = y
                bottom_right_y = y
            else:
                top_left_y = 0
                bottom_right_y = y
            ROI_height_calc = bottom_right_y - top_left_y
        elif ((ROI_height + y) > (y_max - 1)):
            top_left_y = y
            bottom_right_y = 0
            ROI_height_calc = bottom_right_y - top_left_y
        else:
            top_left_y = y
            bottom_right_y =  y + int(ROI_height / 2)
            ROI_height_calc = bottom_right_y - top_left_y
        string = str(count) + "."
        print(string,str(ROI_height_calc))
        count = count + 1
    return sum

'''



def mask(frame, row_start, row_end, column_start, column_end, ROI_cent_x, ROI_cent_y):
    pix_sum = 0
    pix = 0
    for row in range(row_start, row_end):
        for column in range(column_start, column_end):
            pix = frame[row, column]
            pix_sum = pix[0] + pix_sum
        if row == row_end - 1:
            return pix_sum, row_start, row_end, column_start, column_end, ROI_cent_x, ROI_cent_y

def onClick(event, x, y, flags, param):
    global mx, my, px, py, pick, moved, first_pick
    if event == cv2.EVENT_LBUTTONUP:
        print("SINGLE CLICK: ", x, y)
        px, py = x, y
        pick = True
        first_pick = True
    if event == cv2.EVENT_MOUSEMOVE:
        mx, my = x, y
        #print("MOUSE MOVE: ", mx, my)
        moved = True

def detector(gray, template, tH, tW):
    found = None

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
        (_, maxVal, _, maxLoc) = cv2.minMaxLoc(result)

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
            found = (maxVal, maxLoc, r)
    (_, maxLoc, r) = found
    (startX, startY) = (int(maxLoc[0] * r), int(maxLoc[1] * r))
    (endX, endY) = (int((maxLoc[0] + tW) * r), int((maxLoc[1] + tH) * r))
    return (startX, startY, endX, endY)

SCREEN_SIZE = (1920, 1080)
# define the codec
fourcc = cv2.VideoWriter_fourcc(*"XVID")
# create the video write object
out = cv2.VideoWriter("output.avi", fourcc, 20.0, (SCREEN_SIZE))
cap = cv2.VideoCapture(0)

fcout = 0
first = True
while True:
    img = pyautogui.screenshot(region=(0, 0, 1920, 1080))
    frame = np.array(img)
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    frame = imutils.resize(frame, width=100)
    frame1 = frame
    # Get webcam images
    # ret, frame = cap.read()

    # Get height and width of webcam frame
    height, width = frame1.shape[:2]

    # Define ROI Box Dimensions (Note some of these things should be outside the loop)
    top_left_x = int(0)
    top_left_y = int(0)
    bottom_right_x = int(width)
    bottom_right_y = int(height)

    cv2.setMouseCallback('Object Detector', onClick)
    if pick:
        #print(str(frame1.size))
        #print(str(width),str(height))
        #print("***************************************************************")
        ROI_h = 20
        ROI_w = 20
        treshold = 0.8
        #sum = mask(frame1, px, py, ROI_w, ROI_h, treshold)
        found = search(frame1, px, py, ROI_w, ROI_h, treshold)
        print("***************************************************************")
        print(str(found[0]))
        print(str(found))
        print("***************************************************************")
        ROI_cent_x = int(found[5])
        ROI_cent_y = int(found[6])
        cv2.rectangle(frame1, (found[3], found[1]), (found[4], found[2]), (0, 255, 0), 1)
        cv2.circle(frame1, (int(found[5]), int(found[6])), 3, (0, 255, 0), -0)
        #cv2.rectangle(frame1, (sum[3], sum[1]), (sum[4], sum[2]), (0, 255, 0), 2)
        #cv2.circle(frame1, (int(sum[5]), int(sum[6])), 3, (255, 0, 0), -1)
        pick = False
    if first_pick:
        cv2.rectangle(frame1, (found[3], found[1]), (found[4], found[2]), (0, 255, 0), 1)
        cv2.circle(frame1, (int(found[5]), int(found[6])), 3, (0, 255, 0), -0)
    if moved:
        cv2.rectangle(frame1, (mx - 10, my - 10), (mx + 10, my + 10), (255, 0, 0), 1)
        cv2.circle(frame1, (mx, my), 3, (255, 0, 0), -0)
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
