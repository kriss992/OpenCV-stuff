
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat display, tar, ser_area, ser_area_display, src, orginal;
HWND hwndDesktop;

bool cursorMove, leftClick, rightClick, middleClick, selected = false;
bool first = false;
int lx, ly, rx, ry, mx, my, x, y = 0;
int height, width, srcheight, srcwidth;
int first_px, first_py, last_px, last_py;
int cursor = 20;
int key = 0;

bool track_temp = true; 
bool track_object = false;
string track_type = "TEMPLATE TRACKING";
string track_acquired = "NO";

double res_minVal, res_maxVal, res_matchVal, res_minLocx, res_minLocy, res_maxLocx, res_maxLocy, res_matchLocx, res_matchLocy;
bool available = false;

int lx_m, ly_m, lx_M, ly_M;
int x_1, y_1, x_2, y_2, w1, h1, w2, h2, old_x1, old_y1;
int off_x, off_y;

void MouseCallBackFunc(int event, int cx, int cy, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN) { lx = cx; ly = cy; leftClick = true; }
    else if (event == EVENT_RBUTTONDOWN) { rx = cx; ry = cy; rightClick = true; selected = false; track_acquired  = "NO"; }
    else if (event == EVENT_MBUTTONDOWN)
    {
        mx = cx; my = cy;
        if (middleClick == false) { middleClick = true; }
        else { middleClick = false; }
    }
    else if (event == EVENT_MOUSEMOVE) { x = cx; y = cy; cursorMove = true; /*cout << "X: " << cx << ", Y: " << cy << endl;*/ }
}

Mat hwnd2mat(HWND hwnd)
{
    HDC hwindowDC, hwindowCompatibleDC;

    HBITMAP hbwindow;
    Mat src, display;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    //srcheight = windowsize.bottom;
    //srcwidth = windowsize.right;
    srcheight = /*windowsize.bottom -*/ windowsize.bottom / 1; //defines the area that is captured on the computer screen
    srcwidth = /*windowsize.right -*/ windowsize.right / 1;
    height = windowsize.bottom / 2;  //change this to whatever size you want to resize to
    width = windowsize.right / 2;
    //height = 600;
    //width = 800;
    first_px = 0; first_py = 0; last_px = width - 1; last_py = height - 1;
    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    display = src;
    return src;
}

//void MatchingMethod(int, void*)

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
double* seeker(Mat source, Mat target, double treshold)
{
    double tresh = treshold;
    Mat result;
    /// Create the result matrix
    int result_cols = source.cols - target.cols + 1;
    int result_rows = source.rows - target.rows + 1;

    result.create(result_rows, result_cols, CV_32FC1);
    /// Do the Matching and Normalize
    //Matching methodes: 0->TM_SQDIFF 1->TM_SQDIFF_NORMED 2->TM_CCORR 3->TM_CCORR_NORMED 4->TM_COEFF 5->TM_COEFF_NORMED
    int match_method = TM_SQDIFF_NORMED;
    //matchTemplate(source, target, result, match_method);
    matchTemplate(source, target, result, match_method);
    //normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    /// Localizing the best match with minMaxLoc
    double minVal, maxVal, matchVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    //cout << "minVal: " << minVal << ", maxVal: " << maxVal << ", minLoc: " << minLoc << ", maxLoc: " << maxLoc << endl;
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
    {
        matchLoc = minLoc;
        matchVal = minVal;
    }
    else
    {
        matchLoc = maxLoc;
        matchVal = maxVal;
    }
    if (matchVal <= treshold)
    {
        res_minVal = minVal;
        res_maxVal = maxVal;
        res_matchVal = matchVal;
        res_minLocx = minLoc.x;
        res_minLocy = minLoc.y;
        res_maxLocx = maxLoc.x;
        res_maxLocy = maxLoc.y;
        res_matchLocx = matchLoc.x;
        res_matchLocy = matchLoc.y;
        available = true;
        track_acquired = "YES";
    }
    else
    {
        res_minVal = 0;
        res_maxVal = 0;
        res_matchVal = 0;
        res_minLocx = 0;
        res_minLocy = 0;
        res_maxLocx = 0;
        res_maxLocy = 0;
        res_matchLocx = 0;
        res_matchLocy = 0;
        available = false;
        track_acquired = "NO";
    }
    /*cout << "minVal: " << res_minVal << ", maxVal: " << res_maxVal << ", matchVal: " << res_matchVal << ", minLoc.x: " << res_minLocx << ", minhLoc.y: " << res_minLocy <<
        ", maxLoc.x: " << res_maxLocx << ", maxLoc.y: " << res_maxLocy << ", matchLoc.x: " << res_matchLocx << ", matchLoc.y: " << res_matchLocy << ", available: " << available << endl;*/
    return 0;
}

void key_command(int key) // 1=49 2=50 3=51 4=52 
{ 
    if (key == 43)
    { 
        if (cursor < 50) 
        { 
            cursor = cursor + 5; 
        } 
    } 
    else if (key == 45)
    {
        if (cursor > 5)
        {
            cursor = cursor - 5;
        }
    }
    else if (key == 49)
    {
        track_temp = true;
        track_object = false;
        track_type = "TEMPLATE TRACKING";
    }
    else if (key == 50)
    {
        track_temp = false;
        track_object = true;
        track_type = "OBJECT TRACKING";
    }
}
void template_match_tracking()
{
    if (leftClick == true)
    {
        lx_m = lx - int(cursor / 2); if (lx_m < first_px) { lx_m = first_px; }
        ly_m = ly - int(cursor / 2); if (ly_m < first_py) { ly_m = first_py; }
        lx_M = lx + int(cursor / 2); if (lx_M > last_px) { lx_M = last_px; }
        ly_M = ly + int(cursor / 2); if (ly_M > last_py) { ly_M = last_py; }
        w1 = lx_M - lx_m;
        h1 = ly_M - ly_m;
        leftClick = false;
        tar = src(Rect(lx_m, ly_m, w1, h1)).clone();
        selected = true;
        ser_area = src.clone();
    }
    if (selected == true)
    {
        ser_area = src.clone();
        double* rez;
        double treshold = 99;
        treshold = treshold / 100;
        treshold = 1 - treshold;
        rez = seeker(ser_area, tar, treshold);
        //cout << "minVal: " << res_minVal << ", maxVal: " << res_maxVal << ", matchVal: " << res_matchVal << ", minLoc: [" << res_minLocx << "," << res_minLocy
        //    << "], maxLoc: [" << res_maxLocx << "," << res_maxLocy << "], matchLoc: [" << res_matchLocx << "," << res_matchLocy << "], available: " << available << endl;
        if (available == true)
        {
            rectangle(display, Point(res_matchLocx, res_matchLocy), Point(res_matchLocx + cursor, res_matchLocy + cursor), Scalar::all(0), 2, 8, 0);
            circle(display, Point(res_matchLocx + int(cursor / 2), res_matchLocy + int(cursor / 2)), 4, Scalar::all(0), 2, LINE_8);
            tar = ser_area(Rect(res_matchLocx, res_matchLocy, cursor, cursor)).clone();
        }
        putText(display, "TRACKING TYPE: " + track_type, Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        putText(display, "TRACK ACQUIRED: " + track_acquired, Point(20, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        imshow("Target", tar);
        imshow("output", display);
        key = waitKey(60); // you can change wait time
        key_command(key);
    }
    else
    {
        putText(display, "TRACKING TYPE: " + track_type, Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        putText(display, "TRACK ACQUIRED: " + track_acquired, Point(20, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //Rect roi( x,y,w,h );
        /*Rect roi(199, 199, 1580, 680);
        Mat area = src(roi);
        imshow("ROI", area);*/
        imshow("output", display);
        key = waitKey(60); // you can change wait time
        key_command(key);
    }
}
void object_tracking()
{
    if (leftClick == true)
    {
        leftClick = false;
        selected = true;
    }
    if (selected == true)
    {
        if (available == true)
        {

        }
        putText(display, "TRACKING TYPE: " + track_type, Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        putText(display, "TRACK ACQUIRED: " + track_acquired, Point(20, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //imshow("Target", tar);
        imshow("output", display);
        key = waitKey(60); // you can change wait time
        key_command(key);
    }
    else
    {
        putText(display, "TRACKING TYPE: " + track_type, Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        putText(display, "TRACK ACQUIRED: " + track_acquired, Point(20, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //putText(display, "TEST STRING!!!!", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
        //Rect roi( x,y,w,h );
        /*Rect roi(199, 199, 1580, 680);
        Mat area = src(roi);
        imshow("ROI", area);*/
        imshow("output", display);
        key = waitKey(60); // you can change wait time
        key_command(key);
    }
}
int main(int argc, char** argv)
{
    //Mat display, tar, ser_area, ser_area_display, src, orginal;
    //HWND hwndDesktop;
    hwndDesktop = GetDesktopWindow();
    namedWindow("output", WINDOW_NORMAL);
    string trackerTypes[8] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" };
    // Create a tracker
    string trackerType = trackerTypes[2];
    cout << trackerType << endl;
    cout << CV_MINOR_VERSION << endl;
    Ptr<Tracker> tracker;
    
    if (trackerType == "BOOSTING") { tracker = TrackerBoosting::create(); }
    if (trackerType == "MIL") { tracker = TrackerMIL::create(); }
    if (trackerType == "KCF") { tracker = TrackerKCF::create(); }
    if (trackerType == "TLD") { tracker = TrackerTLD::create(); }
    if (trackerType == "MEDIANFLOW") { tracker = TrackerMedianFlow::create(); }
    if (trackerType == "GOTURN") { tracker = TrackerGOTURN::create(); }
    if (trackerType == "MOSSE") { tracker = TrackerMOSSE::create(); }
    if (trackerType == "CSRT") { tracker = TrackerCSRT::create(); }
    while (key != 27)
    {
        orginal = hwnd2mat(hwndDesktop);
        //cvtColor(orginal, orginal, cv::COLOR_BGR2GRAY);
        //Canny(orginal, orginal, 50, 200);
        display = orginal.clone();
        src = orginal.clone();
        /*Mat src;
        if (middleClick == false) { src = imread("bomb_resized.jpg"); }
        else { src = imread("bomb_resized_empty.jpg"); }
        int height, width;
        height = 100;  //change this to whatever size you want to resize to
        width = 100;*/
        first_px = 0; first_py = 0; last_px = width - 1; last_py = height - 1;
        setMouseCallback("output", MouseCallBackFunc, NULL);
        if (cursorMove == true)
        {
            rectangle(display, Point(x - int(cursor / 2), y - int(cursor / 2)), Point(x + int(cursor / 2), y + int(cursor / 2)), Scalar::all(0), 2, 8, 0);
            circle(display, Point(x, y), 4, Scalar::all(0), 2, LINE_8);
        }
        if ( track_temp == true)
        {
            template_match_tracking();
        }
        else if ( track_object == true )
        {
            object_tracking();
        }
        /*if (leftClick == true)
        {
            lx_m = lx - int(cursor / 2); if (lx_m < first_px) { lx_m = first_px; }
            ly_m = ly - int(cursor / 2); if (ly_m < first_py) { ly_m = first_py; }
            lx_M = lx + int(cursor / 2); if (lx_M > last_px) { lx_M = last_px; }
            ly_M = ly + int(cursor / 2); if (ly_M > last_py) { ly_M = last_py; }
            w1 = lx_M - lx_m;
            h1 = ly_M - ly_m;
            leftClick = false;
            tar = src(Rect(lx_m, ly_m, w1, h1)).clone();
            selected = true;
            ser_area = src.clone();
        }
        if (selected == true)
        {
            ser_area = src.clone();
            double* rez;
            double treshold = 99;
            treshold = treshold / 100;
            treshold = 1 - treshold;
            rez = seeker(ser_area, tar, treshold);
            cout << "minVal: " << res_minVal << ", maxVal: " << res_maxVal << ", matchVal: " << res_matchVal << ", minLoc: [" << res_minLocx << ","<< res_minLocy 
                << "], maxLoc: [" << res_maxLocx << "," << res_maxLocy << "], matchLoc: [" << res_matchLocx << "," << res_matchLocy << "], available: " << available << endl;
            if (available == true)
            {
                rectangle(display, Point(res_matchLocx, res_matchLocy), Point(res_matchLocx + cursor, res_matchLocy + cursor), Scalar::all(0), 2, 8, 0);
                circle(display, Point(res_matchLocx + int(cursor / 2), res_matchLocy + int(cursor / 2)), 4, Scalar::all(0), 2, LINE_8);
                tar = ser_area(Rect(res_matchLocx, res_matchLocy, cursor, cursor)).clone();
            }
            putText(display, "TRACKING TYPE: " + track_type, Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            putText(display, "TRACK ACQUIRED: " + track_acquired, Point(20, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            //putText(display, "TEST STRING!!!!", Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            //putText(display, "TEST STRING!!!!", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            imshow("Target", tar);
            imshow("output", display);
            key = waitKey(60); // you can change wait time
            key_command(key);
        }
        else
        {
            putText(display, "TRACKING TYPE: " + track_type, Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            putText(display, "TRACK ACQUIRED: " + track_acquired, Point(20, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            //putText(display, "TEST STRING!!!!", Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            //putText(display, "TEST STRING!!!!", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
            //Rect roi( x,y,w,h );
            ///*Rect roi(199, 199, 1580, 680);
            ///Mat area = src(roi);
            //imshow("ROI", area);
            imshow("output", display);
            key = waitKey(60); // you can change wait time
            key_command(key);
        }*/
    }
}