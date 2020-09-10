#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;

bool cursorMove, leftClick, rightClick, middleClick, selected = false;
bool first = false;
int lx, ly, rx, ry, mx, my, x, y = 0;
int height, width, srcheight, srcwidth;
int first_px, first_py, last_px, last_py;

double res_minVal, res_maxVal, res_matchVal, res_minLocx, res_minLocy, res_maxLocx, res_maxLocy, res_matchLocx, res_matchLocy;
bool available;

void MouseCallBackFunc(int event, int cx, int cy, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN) { lx = cx; ly = cy; leftClick = true; }
    else if (event == EVENT_RBUTTONDOWN) { rx = cx; ry = cy; rightClick = true; selected = false; }
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
    srcheight = /*windowsize.bottom -*/ windowsize.bottom / 2;
    srcwidth = /*windowsize.right -*/ windowsize.right / 2;
    height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
    width = windowsize.right / 1;
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
    matchTemplate(source, target, result, match_method);
    //normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    /// Localizing the best match with minMaxLoc
    double minVal, maxVal, matchVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    cout << "minVal: " << minVal << ", maxVal: " << maxVal << ", minLoc: " << minLoc << ", maxLoc: " << maxLoc << endl;
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
    }
    /*cout << "minVal: " << res_minVal << ", maxVal: " << res_maxVal << ", matchVal: " << res_matchVal << ", minLoc.x: " << res_minLocx << ", minhLoc.y: " << res_minLocy <<
        ", maxLoc.x: " << res_maxLocx << ", maxLoc.y: " << res_maxLocy << ", matchLoc.x: " << res_matchLocx << ", matchLoc.y: " << res_matchLocy << ", available: " << available << endl;*/
    return 0;
}

int main(int argc, char** argv)
{
    Mat display, tar;
    HWND hwndDesktop = GetDesktopWindow();
    namedWindow("output", WINDOW_NORMAL);
    int key = 0;

    while (key != 27)
    {
        int lx_m, ly_m, lx_M, ly_M, w, h;

        //Mat src = hwnd2mat(hwndDesktop);

        Mat src;
        if (middleClick == false) { src = imread("bomb_resized.jpg"); }
        else { src = imread("bomb_resized_empty.jpg"); }
        int height, width;
        height = 100;  //change this to whatever size you want to resize to
        width = 100;

        first_px = 0; first_py = 0; last_px = width - 1; last_py = height - 1;

        cvtColor(src, src, cv::COLOR_BGR2GRAY);
        src.copyTo(display);
        //set the callback function for any mouse event
        setMouseCallback("output", MouseCallBackFunc, NULL);
        if (cursorMove == true)
        {
            rectangle(display, Point(x - 25, y - 25), Point(x + 25, y + 25), Scalar::all(0), 2, 8, 0);
            circle(display, Point(x, y), 4, Scalar::all(0), 2, LINE_8);
        }
        if (leftClick == true)
        {
            lx_m = lx - 25; ly_m = ly - 25; lx_M = lx + 25; ly_M = ly + 25;
            if (lx_m < first_px) { lx_m = 0; }
            if (ly_m < first_py) { ly_m = 0; }
            if (lx_M > last_px) { lx_M = last_px; }
            if (ly_M > last_py) { ly_M = last_py; }
            w = lx_M - lx_m;
            h = ly_M - ly_m;
            leftClick = false;
            src(Rect(lx_m, ly_m, w, h)).copyTo(tar);
            imshow("Target", tar);
            selected = true;
        }
        if (selected == true)
        {
            double* rez;
            double treshold = 90;
            treshold = treshold / 100;
            treshold = 1 - treshold;
            rez = seeker(src, tar, treshold);
            /*cout << "minVal: " << res_minVal << ", maxVal: " << res_maxVal << ", matchVal: " << res_matchVal << ", minLoc.x: " << res_minLocx << ", minhLoc.y: " << res_minLocy <<
                ", maxLoc.x: " << res_maxLocx << ", maxLoc.y: " << res_maxLocy << ", matchLoc.x: " << res_matchLocx << ", matchLoc.y: " << res_matchLocy << ", available: " << available << endl;*/
            if (available == true)
            {
                rectangle(display, Point(res_matchLocx, res_matchLocy), Point(res_matchLocx + 50, res_matchLocy + 50), Scalar::all(0), 2, 8, 0);
                circle(display, Point(res_matchLocx + 25, res_matchLocy + 25), 4, Scalar::all(0), 2, LINE_8);
            }
            imshow("output", display);
            key = waitKey(60); // you can change wait time

        }
        else
        {
            imshow("output", display);
            key = waitKey(60); // you can change wait time
        }
    }
}