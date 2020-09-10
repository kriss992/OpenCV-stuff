#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <Windows.h>
#include <iostream>
#include <cstring>
#include <string>

using namespace std;
using namespace cv;

int key = 0;

int heightScr = 1080;
int widthScr = 1920;
int offX1, offY1, offX2, offY2;

bool first = true;

bool cursorMove, leftClick, rightClick, middleClick, selected = false, blankScr = false, firstMatch = false;
int lx, ly, rx, ry, mx, my, x, y;
int cursor = 20;

int img_w, img_h, tar_w, tar_h, lx_m, ly_m, lx_M, ly_M, first_px, first_py, last_px, last_py;
int* sum_arr;

int offxLock = 0, offyLock = 0, offxLock20 = 0, offyLock20 = 0, offxLockOld = 0, offyLockOld = 0, lockX = 0, lockY = 0;

int frameCnt = 0;
double tresholdVal = 0.99;
double firstMatchVal = 0.0;

int centX = 0, centY = 0, buff = 20, manX = 0, manY = 0, posX = 0, posY = 0;

Mat imgOrg, imgHud, seekArea, seekArea2, tar, tarCan, filter, result, tarTemp, blank;

string track_type = "TEMPLATE TRACKING";
string track_acquired = "NO";
string cursor_size = "1";
string anwserWrite = "";
string anwserRead = "";

int val = 0;
//char response[1024] = "my response";


Mat GetScreenShot(void);
void MouseCallBackFunc(int event, int cx, int cy, int flags, void* userdata);
int* seeker(Mat img, Mat temp, double treshold, int match_method);
void keyCommand(int key);

string fileMapWrite(string msg);
string fileMapRead();

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cursor = cursor;
	}
	else
	{
		cursor = atoi(argv[1]);
		//cout << "cursor size px: " << cursor << endl;
	}
	//namedWindow("Display window", WINDOW_NORMAL);
	while (key != 27)
	{
		imgOrg = GetScreenShot();
		imgHud = imgOrg.clone();
		//rectangle(imgHud, Point(offX1, offY1), Point(imgHud.cols - offX2, imgHud.rows - offY2), Scalar::all(0), 2, 8, 0);
		//seekArea = imgOrg(Rect(offX1, offY1, imgHud.cols - 2 * offX2, imgHud.rows - 2 * offY2)).clone();
		setMouseCallback("Display window", MouseCallBackFunc, NULL);

		first_px = 0; first_py = 0; last_px = imgHud.cols; last_py = imgHud.rows;

		//filter = imgOrg.clone();
		//Canny(filter, filter, 50, 200);
		if (cursorMove == true)
		{
			rectangle(imgHud, Point(x - int(cursor / 2), y - int(cursor / 2)), Point(x + int(cursor / 2), y + int(cursor / 2)), Scalar::all(0), 2, 8, 0);
			circle(imgHud, Point(x, y), 4, Scalar::all(0), 1, LINE_8);
		}
		if (selected == true)
		{
			if (leftClick == false)
			{
				if (lockX == 2 * cursor) { offxLock = offxLock; }
				else if (lockX < 2 * cursor) { offxLock = offxLock - (2 * cursor - lockX); }
				else if (lockX > 2 * cursor) { offxLock = offxLock + (lockX - 2 * cursor); }
				if (lockY == 2 * cursor) { offyLock = offyLock; }
				else if (lockY < 2 * cursor) { offyLock = offyLock - (2 * cursor - lockY); }
				else if (lockY > 2 * cursor) { offyLock = offyLock + (lockY - 2 * cursor); }

				if (offxLock < 0) { selected = false; tarCan.release(); seekArea.release(); continue; }
				else if (offyLock < 0) { selected = false;  tarCan.release(); seekArea.release(); continue; }
				else if (offxLock + 5 * cursor >= imgOrg.cols) { selected = false; tarCan.release(); seekArea.release(); continue; }
				else if (offyLock + 5 * cursor >= imgOrg.rows) { selected = false;  tarCan.release(); seekArea.release(); continue; }
			}
			else if (leftClick == true)
			{
				leftClick = false;
				firstMatch = true;
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

				lockX = lx_m;
				lockY = ly_m;
				offxLock = lx_m - cursor * 2;
				offyLock = ly_m - cursor * 2;
				//cout << "lockX: " << lockX << " lockY: " << lockY << " offxLock: " << offxLock << " offyLock: " << offyLock << endl;
				if (offxLock < 0) { leftClick = false; selected = false; continue; }
				else if (offyLock < 0) { leftClick = false; selected = false; continue; }
				else if (offxLock + 5 * cursor >= imgOrg.cols) { leftClick = false; selected = false; continue; }
				else if (offyLock + 5 * cursor >= imgOrg.rows) { leftClick = false; selected = false; continue; }
				else
				{
					tar = imgOrg(Rect(lockX, lockY, cursor, cursor)).clone();
					tarCan = tar.clone();
					selected = true;
				}
			}
			seekArea = imgOrg(Rect(offxLock, offyLock, cursor * 5, cursor * 5)).clone();
			rectangle(imgHud, Point(offxLock, offyLock), Point(offxLock + 5 * cursor, offyLock + 5 * cursor), Scalar::all(0), 1, 8, 0);
			filter = seekArea.clone();
			/*
			cvtColor(filter, filter, COLOR_BGR2GRAY);
			Canny(filter, filter, 50, 200);
			GaussianBlur(filter, filter, Size(9, 9), 0);
			cvtColor(tarCan, tarCan, COLOR_BGR2GRAY);
			Canny(tarCan, tarCan, 50, 200);
			GaussianBlur(tarCan, tarCan, Size(9, 9), 0);
			*/
			sum_arr = seeker(filter, tarCan, tresholdVal, TM_SQDIFF_NORMED);
			if (sum_arr[0] == 1)
			{
				if (firstMatch == true) { firstMatchVal = sum_arr[3]; firstMatch = false; }
				lockX = sum_arr[8];
				lockY = sum_arr[9];
				rectangle(imgHud, Point(lockX + offxLock, lockY + offyLock), Point(lockX + cursor + offxLock, lockY + cursor + offyLock), Scalar::all(0), 2, 8, 0);
				circle(imgHud, Point(lockX + offxLock + int(cursor / 2), lockY + offyLock + int(cursor / 2)), 4, Scalar::all(0), 1, LINE_8);
				tar = imgOrg(Rect(lockX + offxLock, lockY + offyLock, cursor, cursor)).clone();
				tarCan = tar.clone();

				//int centX = 0, centY = 0, buff = 0, manX = 0, manY = 0, posX = 0, posY = 0;

				centX = int(imgHud.cols / 2);
				centY = int(imgHud.rows / 2);

				posX = lockX + offxLock + int(cursor / 2);
				posY = lockY + offyLock + int(cursor / 2);

				manX = 0;
				manY = 0;

				if (posX < (centX - buff)) { manX = 2; }
				else if (posX > (centX + buff)) { manX = -2; }
				else if ((posX < (centX - buff)) && (posX > (centX + buff))) { manX = 0; }

				if (posY < (centY - buff)) { manY = 2; }
				else if (posY > (centY + buff)) { manY = -2; }
				else if ((posY < (centY - buff)) && (posY > (centY + buff))) { manY = 0; }

				string x = to_string(manX);
				string y = to_string(manY);
				string val = x + "," + y;
				anwserWrite = fileMapWrite(val);
				anwserRead = fileMapRead();
				//cout << " aswserWrite: " << anwserWrite << " anwserRead: " << anwserRead << endl;
				//cout << anwser << endl;
			}
			else
			{
				selected = false;
			}
		}
		else
		{
			//string val = "***";
			anwserWrite = fileMapWrite("***");
			//anwserRead = fileMapRead();
			//cout << " aswserWrite: " << anwserWrite << " anwserRead: " << anwserRead << endl;
		}
		if (selected == true) { track_acquired = "YES"; }
		else { track_acquired = "NO"; }
		cursor_size = to_string(cursor);
		putText(imgHud, "TRACKING TYPE: " + track_type, Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
		putText(imgHud, "TRACK ACQUIRED: " + track_acquired, Point(20, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
		putText(imgHud, "SELECTOR SIZE: " + cursor_size, Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
		putText(imgHud, "CHANGE SELECTOR SIZE WITH KEYS 1 AND 2, EXIT WITH ESC ", Point(20, 640), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 2);
		imshow("Display window", imgHud);
		key = waitKey(10); // you can change wait time 
		keyCommand(key);
	}
}
void MouseCallBackFunc(int event, int cx, int cy, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN) { lx = cx; ly = cy; leftClick = true; selected = true; }
	else if (event == EVENT_RBUTTONDOWN) { rx = cx; ry = cy; rightClick = true; selected = false; first = true; }
	else if (event == EVENT_MBUTTONDOWN)
	{
		mx = cx; my = cy;
		if (middleClick == false) { middleClick = true; /*rx = cx; ry = cy; selected = false; first = true;*/ }
		else { middleClick = false; }
	}
	else if (event == EVENT_MOUSEMOVE) { x = cx; y = cy; cursorMove = true; /*cout << "X: " << cx << ", Y: " << cy << endl;*/ }
}
void keyCommand(int key) // 1=49 2=50 3=51 4=52 
{
	if (key == 49)
	{
		if (cursor < imgHud.rows && cursor < imgHud.cols)
		{
			cursor = cursor + 5;
		}
	}
	else if (key == 50)
	{
		if (cursor > 5)
		{
			cursor = cursor - 5;
		}
	}
	else if (key == 51)
	{
		blankScr = !blankScr;
	}
}
Mat GetScreenShot(void)
{
	HDC hdc = GetDC(NULL); // get the desktop device context
	HDC hDest = CreateCompatibleDC(hdc); // create a device context to use yourself

	// get the height and width of the screen
	//int heightScr = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	//int widthScr = GetSystemMetrics(SM_CXVIRTUALSCREEN); 

	Mat mat(heightScr, widthScr, CV_8UC4);//create corresponding Mat
	Mat gaus(heightScr, widthScr, CV_8UC4);//create corresponding Mat

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
	//resize(mat, mat, Size(mat.cols * 0.75, mat.rows * 0.75), 2, 2);
	//Canny(mat, mat, 50, 200);
	//resize(mat, mat, Size(mat.cols * 0.75, mat.rows * 0.75), 2, 2);
	mat = mat(Rect(200, 200, mat.cols - 400, mat.rows - 400)).clone();
	if (blankScr == true) { mat = Mat::zeros(mat.rows, mat.cols, CV_8UC4); }
	//cvtColor(mat, mat, COLOR_BGR2GRAY);
	//mat = mat(Rect(100, 100, mat.cols - 200, mat.rows - 200)).clone();
	//gaus = mat.clone();
	//GaussianBlur(gaus, gaus, Size(5, 5), 0);
	//GaussianBlur(mat, mat, Size(9, 9), 0);
	//bitwise_not(gaus, gaus);
	//bitwise_not(mat, mat);
	//imshow("gaus", gaus);
	return mat;
}
int* seeker(Mat img, Mat temp, double treshold, int match_method)
{
	int found = 0;
	static int return_arr[10] = { 0,0,0,0,0,0,0,0,0,0 };
	/// Create the result matrix
	int result_cols = img.cols - temp.cols + 1;
	int result_rows = img.rows - temp.rows + 1;

	double firstMatchValTemp = 0.0;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(img, temp, result, match_method);
	//normalize(result, result, 0, 1, NORM_MINMAX);

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; double matchVal; Point minLoc; Point maxLoc;	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better 
	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
		if (matchVal <= double(1 - treshold)) { found = 1; }
		else { found = 0; }
		if (firstMatch == true) { firstMatchValTemp = matchVal; }
		//cout << "given treshold value: " << treshold << " matchVal: " << matchVal << " treshold value: " << double(1 - treshold) << " firstMatchValTemp: " << firstMatchValTemp << endl;

	}
	else if (match_method == TM_CCOEFF || match_method == TM_CCOEFF_NORMED || match_method == TM_CCORR || match_method == TM_CCORR_NORMED)
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
		if (matchVal >= double(1 - (1 - treshold))) { found = 1; }
		else { found = 0; }
	}
	if (found == 1)
	{
		return_arr[0] = found;
		return_arr[1] = minVal;
		return_arr[2] = maxVal;
		return_arr[3] = matchVal;
		return_arr[4] = minLoc.x;
		return_arr[5] = minLoc.y;
		return_arr[6] = maxLoc.x;
		return_arr[7] = maxLoc.y;
		return_arr[8] = matchLoc.x;
		return_arr[9] = matchLoc.y;
	}
	else
	{
		return_arr[0] = found;
		return_arr[1] = 0;
		return_arr[2] = 0;
		return_arr[3] = 0;
		return_arr[4] = 0;
		return_arr[5] = 0;
		return_arr[6] = 0;
		return_arr[7] = 0;
		return_arr[8] = 0;
		return_arr[9] = 0;
	}
	//imshow("result", result);
	//cout << " ------ " << "found: " << return_arr[0] << " minVal: " << minVal << " maxVal: " << maxVal << " matchVal: " << matchVal << " minLoc: " << minLoc << " maxLoc: " << maxLoc << " matchLoc: " << matchLoc << endl;
	return return_arr;
}
string fileMapWrite(string msg)
{
	//cout << "\t\t....FILEMAPPING SERVER or PARENT...." << endl;
	//Local Variable Definitions
	HANDLE hFileMap;
	BOOL bResult;
	PCHAR lpBuffer = NULL;
	char Buffer[1024];
	strcpy_s(Buffer, msg.c_str());
	size_t szBuffer = sizeof(Buffer);
	//cout << "Buffer: " << Buffer << " szBuffer: " << szBuffer << endl;
	hFileMap = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		256,
		L"Local\\MyFileMapServer");
	if (hFileMap == FALSE)
	{
		cout << "CreateFileMapping err: " << GetLastError() << endl;
	}
	//cout << "CreatingFileMapping succesful" << endl;

	lpBuffer = (PCHAR)MapViewOfFile(
		hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);
	if (lpBuffer == NULL)
	{
		cout << "MapViewOfFile err: " << GetLastError << endl;
	}
	//cout << "MapViewOfFile succesful" << endl;

	CopyMemory(lpBuffer, Buffer, szBuffer);

	bResult = UnmapViewOfFile(lpBuffer);
	if (bResult == FALSE)
	{
		cout << "UnmapViewOfFile err: " << GetLastError() << endl;
	}
	//cout << "UnmapViewOfFile succesful" << endl;
	return "SUCCESS";
}
string fileMapRead()
{
	HANDLE hFileMap;
	BOOL bResult;
	PCHAR lpBuffer = NULL;
	string dataServer = "";

	hFileMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"Local\\MyFileMapClient");
	if (hFileMap == NULL)
	{
		string res = "  OpenFileMap err: " + to_string(GetLastError());
		return res;
		//cout << "OpenFileMap err: " << GetLastError() << endl;
	}
	//cout << "OpenFileMap succesful" << endl;

	lpBuffer = (PCHAR)MapViewOfFile(
		hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);
	if (lpBuffer == NULL)
	{
		string res = "  MapViewOfFile err: " + to_string(GetLastError());
		return res;
		//cout << "MapViewOfFile err: " << GetLastError << endl;
	}
	//cout << "MapViewOfFile succesful" << endl;

	//cout << "DATA FROM FILE SERVER: " << lpBuffer << endl;
	dataServer = lpBuffer;
	bResult = UnmapViewOfFile(lpBuffer);
	if (bResult == FALSE)
	{
		string res = "  UnmapViewOfFile err: " + to_string(GetLastError());
		return res;
		//cout << "UnmapViewOfFile err: " << GetLastError() << endl;
	}
	//cout << "UnmapViewOfFile succesful" << endl;

	CloseHandle(hFileMap);
	return dataServer;
}