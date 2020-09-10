#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/features2d/features2d.hpp>

#include <opencv2/calib3d.hpp>

#include <Windows.h>
#include <iostream>
#include <cstring>

//#include "opencv2/calib3d.hpp"
//#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;

int key = 0;

int heightScr = 1080;
int widthScr = 1920;
int offX1, offY1, offX2, offY2;

bool first = true;

bool cursorMove, leftClick, rightClick, middleClick, selected = false;
int lx, ly, rx, ry, mx, my, x, y;
int cursor = 200;

int img_w, img_h, tar_w, tar_h, lx_m, ly_m, lx_M, ly_M, first_px, first_py, last_px, last_py;
int* sum_arr;

int offxLock = 0, offyLock = 0, offxLockOld = 0, offyLockOld = 0, lockX = 0, lockY = 0;

int frameCnt = 0;
int test = 0;

Mat imgOrg, imgHud, seekArea, seekArea2, tar, tarCan, filter, result, tarTemp;

vector<KeyPoint> keypoints_1, keypoints_2;
Mat descriptors_1, descriptors_2;


//Ptr<FeatureDetector> detector = ORB::create();
//Ptr<DescriptorExtractor> descriptor = ORB::create();

Ptr<Feature2D> orb = ORB::create(500, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);
vector<DMatch> matches;
Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
float treshold = 0.05f;

//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
//vector<DMatch> matches;
//vector< DMatch > good_matches;

Mat GetScreenShot(void);
void MouseCallBackFunc(int event, int cx, int cy, int flags, void* userdata);
int matcherFnc(Mat img, Mat temp, float treshold);

int main(int argc, char** argv)
{
	cursor = atoi(argv[1]);
	cout << "cursor size px: " << cursor << endl;

	//namedWindow("Display window", WINDOW_NORMAL);
	while (key != 27)
	{
		imgOrg = GetScreenShot();
		imgHud = imgOrg.clone();
		//rectangle(imgHud, Point(offX1, offY1), Point(imgHud.cols - offX2, imgHud.rows - offY2), Scalar::all(0), 2, 8, 0);
		//seekArea = imgOrg(Rect(offX1, offY1, imgHud.cols - 2 * offX2, imgHud.rows - 2 * offY2)).clone();
		setMouseCallback("Display seek area", MouseCallBackFunc, NULL);

		first_px = 0; first_py = 0; last_px = imgHud.cols; last_py = imgHud.rows;

		seekArea = imgOrg(Rect(400, 300, imgHud.cols - 600, imgHud.rows - 500)).clone();
		seekArea2 = seekArea.clone();
		//filter = seekArea.clone();
		//Canny(filter, filter, 50, 200);

		//filter = imgOrg.clone();
		//Canny(filter, filter, 50, 200);
		if (selected == true)
		{
			Size k(5, 5);
			filter = seekArea.clone();
			cvtColor(filter, filter, COLOR_BGR2GRAY);
			Canny(filter, filter, 100, 200);
			blur(filter, filter, k);
			bitwise_not(filter, filter);
			tarTemp = tar.clone();
			cvtColor(tarTemp, tarTemp, COLOR_BGR2GRAY);
			Canny(tarTemp, tarTemp, 100, 200);
			blur(tarTemp, tarTemp, k);
			bitwise_not(tarTemp, tarTemp);

			//test = matcherFnc(seekArea, tar, treshold);
			test = matcherFnc(filter, tarTemp, treshold);
		}
		if (cursorMove == true)
		{
			rectangle(seekArea2, Point(x - int(cursor / 2), y - int(cursor / 2)), Point(x + int(cursor / 2), y + int(cursor / 2)), Scalar::all(0), 2, 8, 0);
			circle(seekArea2, Point(x, y), 4, Scalar::all(0), 1, LINE_8);
		}
		if (leftClick == true)
		{
			leftClick = false;

			int w1, h1;
			lx_m = lx - int(cursor / 2);
			if (lx_m < first_px) { lx_m = first_px; }
			ly_m = ly - int(cursor / 2);
			if (ly_m < first_py) { ly_m = first_py; }
			lx_M = lx + int(cursor / 2);
			if (lx_M > last_px) { lx_M = last_px; }
			ly_M = ly + int(cursor / 2);
			if (ly_M > last_py) { ly_M = last_py; }
			w1 = lx_M - lx_m;
			h1 = ly_M - ly_m;

			tar = seekArea(Rect(lx_m, ly_m, w1, h1)).clone();

			//seekArea = imgOrg(Rect(offxLock, offyLock, cursor * 3, cursor * 3)).clone();
			//rectangle(imgHud, Point(offxLock, offyLock), Point(lx_m + 2 * cursor, ly_m + 2 * cursor), Scalar::all(0), 1, 8, 0);

			//tarCan = tar.clone();
			//Canny(tar, tarCan, 50, 200);

			selected = true;
			//imshow("Target", tar);
			//test = matcherFnc(seekArea, tar, 10);
		}
		//imshow("Display window", imgHud);
		imshow("Display seek area", seekArea2);
		key = waitKey(10); // you can change wait time 
	}
}
void MouseCallBackFunc(int event, int cx, int cy, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN) { lx = cx; ly = cy; leftClick = true; }
	else if (event == EVENT_RBUTTONDOWN) { rx = cx; ry = cy; rightClick = true; selected = false; first = true; }
	else if (event == EVENT_MBUTTONDOWN)
	{
		mx = cx; my = cy;
		if (middleClick == false) { middleClick = true; }
		else { middleClick = false; }
	}
	else if (event == EVENT_MOUSEMOVE) { x = cx; y = cy; cursorMove = true; }
}
Mat GetScreenShot(void)
{
	HDC hdc = GetDC(NULL); // get the desktop device context
	HDC hDest = CreateCompatibleDC(hdc); // create a device context to use yourself

	// get the height and width of the screen
	//int heightScr = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	//int widthScr = GetSystemMetrics(SM_CXVIRTUALSCREEN); 

	Mat mat;
	mat.create(heightScr, widthScr, CV_8UC4);//create corresponding Mat

	// create a bitmap
	HBITMAP hbDesktop = CreateCompatibleBitmap(hdc, widthScr, heightScr);

	// use the previously created device context with the bitmap
	SelectObject(hDest, hbDesktop);

	// copy from the desktop device context to the bitmap device context
	// call this once per 'frame'
	BitBlt(hDest, 0, 0, widthScr, heightScr, hdc, 0, 0, SRCCOPY);
	BITMAPINFOHEADER bi = { sizeof(bi), widthScr, -heightScr, 1, 32, BI_RGB };
	GetDIBits(hdc, hbDesktop, 0, heightScr, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	// after the recording is done, release the desktop context you got..
	ReleaseDC(NULL, hdc);

	// ..delete the bitmap you were using to capture frames..
	DeleteObject(hbDesktop);

	// ..and delete the context you created
	DeleteDC(hDest);

	//return a Mat image into calling function
	//cvtColor(mat, mat, COLOR_BGR2GRAY);
	//Canny(mat, mat, 50, 200);
	//bitwise_not(mat, mat);
	//GaussianBlur(mat, mat, Size(9, 9), 0);
	//resize(mat, mat, Size(mat.cols * 0.75, mat.rows * 0.75), 2, 2);
	return mat;
}
/*int matcherFnc(Mat img, Mat temp, double treshold)
{
	keypoints_1.clear(); keypoints_2.clear();
	matches.clear();
	good_matches.clear();

	detector->detect(temp, keypoints_2);
	detector->detect(img, keypoints_1);
	descriptor->compute(temp, keypoints_2, descriptors_2);
	descriptor->compute(img, keypoints_1, descriptors_1);
	matcher->match(descriptors_1, descriptors_2, matches);
	double min_dist = 100000, max_dist = 0;
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i].distance <= max(2 * min_dist, 50.0))
		{
			good_matches.push_back(matches[i]);
		}
	}

	drawMatches(img, keypoints_1, temp, keypoints_2, good_matches, seekArea2, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	////-- Localize the object
	//vector<Point2f> obj;
	//vector<Point2f> scene;
	//for (size_t i = 0; i < good_matches.size(); i++)
	//{
	//	//-- Get the keypoints from the good matches
	//	obj.push_back(keypoints_2[good_matches[i].queryIdx].pt);
	//	scene.push_back(keypoints_1[good_matches[i].trainIdx].pt);
	//}
	//Mat H = findHomography(obj, scene, RANSAC);
	////-- Get the corners from the image_1 ( the object to be "detected" )
	//vector<Point2f> obj_corners(4);
	//obj_corners[0] = Point2f(0, 0);
	//obj_corners[1] = Point2f(temp.cols, 0);
	//obj_corners[2] = Point2f(temp.cols, temp.rows);
	//obj_corners[3] = Point2f(0, temp.rows);
	//vector<Point2f> scene_corners(4);
	//perspectiveTransform(obj_corners, scene_corners, H);
	////-- Draw lines between the corners (the mapped object in the scene - image_2 )

	//line(seekArea2, scene_corners[0] + Point2f(temp.cols, 0), scene_corners[1] + Point2f(temp.cols, 0), Scalar(0, 255, 0), 4);
	//line(seekArea2, scene_corners[1] + Point2f(temp.cols, 0), scene_corners[2] + Point2f(temp.cols, 0), Scalar(0, 255, 0), 4);
	//line(seekArea2, scene_corners[2] + Point2f(temp.cols, 0), scene_corners[3] + Point2f(temp.cols, 0), Scalar(0, 255, 0), 4);
	//line(seekArea2, scene_corners[3] + Point2f(temp.cols, 0), scene_corners[0] + Point2f(temp.cols, 0), Scalar(0, 255, 0), 4);

	return 0;
}*/
int matcherFnc(Mat img, Mat temp, float treshold)
{
	keypoints_1.clear();
	keypoints_2.clear();
	matches.clear();
	//good_matches.clear();

	// Variables to store keypoints and descriptors
	std::vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;

	// Detect ORB features and compute descriptors.
	orb->detectAndCompute(img, Mat(), keypoints1, descriptors1);
	orb->detectAndCompute(temp, Mat(), keypoints2, descriptors2);

	// Match features.
	matcher->match(descriptors1, descriptors2, matches, Mat());

	// Sort matches by score
	sort(matches.begin(), matches.end());

	// Remove not so good matches
	const int numGoodMatches = matches.size() * treshold;
	matches.erase(matches.begin() + numGoodMatches, matches.end());


	// Draw top matches
	//Mat imMatches;
	drawMatches(img, keypoints1, temp, keypoints2, matches, seekArea2);
	//drawMatches(img, keypoints1, temp, keypoints2, matches, seekArea2, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//imwrite("matches.jpg", imMatches);


	// Extract location of good matches
	std::vector<Point2f> points1, points2;

	for (size_t i = 0; i < matches.size(); i++)
	{
		points1.push_back(keypoints1[matches[i].queryIdx].pt);
		points2.push_back(keypoints2[matches[i].trainIdx].pt);
	}

	// Find homography
	Mat h = findHomography(points1, points2, RANSAC);
	//cout << "h: " << h << endl;
	// Use homography to warp image
	warpPerspective(img, img, h, temp.size());
	imshow("rotated", img);
	return 0;
}
