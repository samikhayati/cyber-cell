#include <iostream>
#include <windows.h>
#include <tchar.h> 
#include<stdio.h>
#include <math.h>
#include <ctime>
#include <cstdio>
#include <map>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <ratio>
#pragma comment(lib, "Gdi32.lib")



using namespace std;
using namespace cv;
using namespace std::chrono;


int GlobalWHeight, GlobalWWidth;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    std::vector<std::string>& windowTitles = *reinterpret_cast<std::vector<std::string>*>(lParam);
    char buffer[256];
    if (GetWindowTextA(hwnd, buffer, sizeof(buffer))) {
        windowTitles.emplace_back(buffer);
    }
    return TRUE;
}

void getcontour(Mat img1, Mat img2) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    drawContours(img2, contours, -1, Scalar(255, 0, 255), 3);
}

void contourtest(Mat img1, Mat img2) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    int id, id2, id3;
    id3 = 0;
    id2 = 0;
    id = 0;
    int d = 9000000, d2 = 9000000, d3 = 9000000;
    int di;
    for (int i = 0; i < contours.size(); i++) {
        if ((contours[i][0].y < (img2.rows - 60)) && (contours[i][0].y > (img2.rows - 960))) {
            drawContours(img2, contours, i, Scalar(255, 0, 255), 3);
            di = pow(contours[i][0].y - img2.rows / 2, 2) + pow(contours[i][0].x - img2.cols / 2, 2);
            if (di < d) {
                d2 = d;
                id2 = id;
                d = di;
                id = i;
            }
            else {
                if (di < d2) {
                    d2 = di;
                    id2 = i;
                }

            }
        }
    }
    Point P = contours[id2][0];

    int x = P.x;
    int y = P.y;

    drawContours(img2, contours, id, Scalar(0, 255, 0), 3);
    drawContours(img2, contours, id2, Scalar(0, 0, 0), 3);
    line(img2, Point(img2.cols / 2, img2.rows / 2), Point(contours[id2][0].x, contours[id2][0].y), Scalar(0, 0, 255), 2);
    img2.at<Vec3b>(Point(contours[id2][0].x, contours[id2][0].y))[0] = 100; // Blue component
    img2.at<Vec3b>(Point(contours[id2][0].x, contours[id2][0].y))[1] = 100; // Green component
    img2.at<Vec3b>(Point(contours[id2][0].x, contours[id2][0].y))[2] = 100; // Red component
    for (int i = 0; i < contours[id2].size(); i++) {
        Point point = contours[id2][i];

        img2.at<Vec3b>(point.y, point.x)[0] = 0; // Blue component
        img2.at<Vec3b>(point.y, point.x)[1] = 0; // Green component
        img2.at<Vec3b>(point.y, point.x)[2] = 0; // Red component
    }
    SetCursorPos(contours[id2][0].x, contours[id2][0].y + 33);






}


void preparation(Mat img) {
    img.at<Vec3b>(img.rows / 2, img.cols / 2)[0] = 0;  //draws point in the middle
    img.at<Vec3b>(img.rows / 2, img.cols / 2)[1] = 255;
    img.at<Vec3b>(img.rows / 2, img.cols / 2)[2] = 0;
    line(img, Point(0, img.rows - 60), Point(img.cols - 1, img.rows - 60), Scalar(0, 0, 255), 2);
    line(img, Point(0, img.rows - 960), Point(img.cols - 1, img.rows - 960), Scalar(0, 0, 255), 2);
}

Mat getmat(HWND hGameWindow) { //160-200 FPS

    HDC dc1 = GetDC(hGameWindow);

    HDC dc2;
    dc2 = CreateCompatibleDC(dc1);

    RECT windo;
    GetClientRect(hGameWindow, &windo);
    int height, width;  
    height = windo.bottom;
    width = windo.right;

    HBITMAP hbitmp = CreateCompatibleBitmap(dc1, width, height);


    SelectObject(dc2, hbitmp);

    cout << height;
    BitBlt(dc2, 0, 0, width, height * 0.813, dc1, 0, height * 0.1, SRCCOPY); //to remove tabs and ad boxes
    int newheight = height * 0.813;   
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -newheight;  
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0; //because no compression
    bi.biXPelsPerMeter = 1; 
    bi.biYPelsPerMeter = 2; 
    bi.biClrUsed = 3; 
    bi.biClrImportant = 4; 

    Mat mt(newheight, width, CV_8UC4);   

    GetDIBits(dc2, hbitmp, 0, newheight, mt.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  
    DeleteObject(hbitmp);
    DeleteDC(dc2);
    ReleaseDC(hGameWindow, dc1);

    return mt;

    //quick explanation, GetDIBits captures screeen and save in mt
    //GetDIBits:0 & newheight represent first scan line & number of scan lines
    //160-200 FPS,further optimisations are possible by removing matrix recreation,and other stuff

}



void playtest(Mat img) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    int id, id2;
    id2 = 0;
    id = 0;
    int d = 9000000, d2 = 9000000;
    int di;
    for (int i = 0; i < contours.size(); i++) {
        if ((contours[i][0].y < (img.rows - 60)) && (contours[i][0].y > (img.rows - 960))) {

            di = pow(contours[i][0].y - img.rows / 2, 2) + pow(contours[i][0].x - img.cols / 2, 2);
            if (di < d) {
                d2 = d;
                id2 = id;
                d = di;
                id = i;
            }
            else {
                if (di < d2) {
                    d2 = di;
                    id2 = i;
                }

            }
        }
    }
    SetCursorPos(contours[id2][0].x, contours[id2][0].y + 33);



}
void contourtest2(Mat img1, Mat img2) {   //playtest/contourtest 2 added enemy detection 
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int id, id2, id3;
    id3 = 0;
    id2 = 0;
    id = 0;
    int d = 9000000, d2 = 9000000, d3 = 900000;
    int di;
    double darea, di2area;
    for (int i = 0; i < contours.size(); i++) {

        drawContours(img2, contours, i, Scalar(255, 0, 255), 3);
        di = pow(contours[i][0].y - img2.rows / 2, 2) + pow(contours[i][0].x - img2.cols / 2, 2);


        if (di < d) {

            d = di;
            id = i;

        }

    }

    darea = arcLength(contours[id], true);
    putText(img2, to_string(int(darea)), contours[id][0], FONT_HERSHEY_DUPLEX, 2, Scalar(255, 255, 0));
    darea = darea * 1.1;
    for (int i = 0; i < contours.size(); i++) {


        di = pow(contours[i][0].y - img2.rows / 2, 2) + pow(contours[i][0].x - img2.cols / 2, 2);
        di2area = arcLength(contours[i], true);

        if (di2area < darea) {
            if ((di < d2) && (di > d)) {
                id2 = i;
                d2 = di;
            }

        }
        else {
            if (di < d3) {
                id3 = i;
                d3 = di;
            }

            drawContours(img2, contours, i, Scalar(255, 0, 0), 3);
            putText(img2, to_string(int(di2area)), contours[i][0], FONT_HERSHEY_DUPLEX, 2, Scalar(255, 255, 0));

        }



    }

    drawContours(img2, contours, id, Scalar(0, 255, 0), 3);
    drawContours(img2, contours, id2, Scalar(0, 0, 0), 3);

    if ((d3 < 250000) || (d3 < d2)) {
        line(img2, Point(img2.cols / 2, img2.rows / 2), Point(img2.cols - contours[id3][0].x, img2.rows - (contours[id3][0].y + 33)), Scalar(0, 0, 255), 2);
    }
    else {

        line(img2, Point(img2.cols / 2, img2.rows / 2), Point(contours[id2][0].x, contours[id2][0].y), Scalar(0, 0, 255), 2);

    }






}

void contourtest3(Mat img1, Mat img2) {   //playtest/contourtest 2 added enemy detection 
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int id, id2, id3;
    id3 = 0;
    id2 = 0;
    id = 0;
    int d = 9000000, d2 = 9000000, d3 = 900000;
    int di;
    double darea, di2area;
    vector<vector<Point> > approxShape(contours.size());

    for (int i = 0; i < contours.size(); i++) {
        if ((contours[i][0].y < (img2.rows - 60)) && (contours[i][0].y > (img2.rows - 960))) {
            //approxPolyDP(contours[i], approxShape[i], 1, true);
            for (int j = 0; j < contours[i].size(); j++) {
                circle(img2, Point(contours[i][j].x, contours[i][j].y), 0, Scalar(j % 255, j % 255, j % 255), 4);

            }

        }
    }
    darea = arcLength(contours[id], true);
    putText(img2, to_string(int(darea)), contours[id][0], FONT_HERSHEY_DUPLEX, 2, Scalar(255, 255, 0));
    darea = darea * 1.1;

}
void playtest2(Mat img) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    int id, id2, id3;
    id3 = 0;
    id2 = 0;
    id = 0;
    int d = 9000000, d2 = 9000000, d3 = 9000000;
    int di;
    double darea, di2area;
    for (int i = 0; i < contours.size(); i++) {  //this loop determines your cell contour


        di = pow(contours[i][0].y - img.rows / 2, 2) + pow(contours[i][0].x - img.cols / 2, 2); 
        //distance between the middle point and the first point of the contour

        if (di < d) {

            d = di;
            id = i;

        }

    }

    darea = arcLength(contours[id], true) * 1.1;
    for (int i = 0; i < contours.size(); i++) {


        di = pow(contours[i][0].y - img.rows / 2, 2) + pow(contours[i][0].x - img.cols / 2, 2);
        di2area = arcLength(contours[i], true);
        if ((di < d2) && (di > d)) { //for detecting closest food
            if (di2area < darea) {
                id2 = i;
                d2 = di;
            }

        }
        if (di2area > darea) {   //for detecting closest threat
            if (di < d3) {
                id3 = i;
                d3 = di;
            }
        }




    } 
    if ((d3 < 490000) || (d3 < d2)) {  
        SetCursorPos((img.cols - contours[id3][0].x) * 10, GlobalWHeight * 0.1 + (img.rows - contours[id3][0].y) * 10);
        //opposite direction of threat
    }
    else {

        SetCursorPos(contours[id2][0].x * 10, GlobalWHeight * 0.1 + contours[id2][0].y * 10);
        //food direction
    }







}

void edgeremover(Mat img) {  //currently usless as the edges grow as your cell grow ...
    int i, j;
    for (int x = 0; x < img.cols; x++) {
        if (((190 < img.at<uchar>(img.rows + 2 - 960, x)) && (img.at<uchar>(img.rows + 2 - 960, x) < 230)) && ((190 < img.at<uchar>(img.rows + 3 - 960, x)) && (img.at<uchar>(img.rows + 3 - 960, x) < 230))) {
            i = x;
            while (i < img.cols - 2) {
                for (int j = img.rows + -960; j < img.rows - 59; j++) {
                    if ((190 < img.at<uchar>(j, i)) && (img.at<uchar>(j, i) < 230)) {
                        img.at<uchar>(j, i) = 248;
                    }
                    if ((190 < img.at<uchar>(j, i + 1)) && (img.at<uchar>(j, i + 1) < 230)) {
                        img.at<uchar>(j, i + 1) = 248;
                    }
                    if ((190 < img.at<uchar>(j, i + 2)) && (img.at<uchar>(j, i + 2) < 230)) {
                        img.at<uchar>(j, i + 2) = 248;
                    }

                }
                i = i + 50;

            }
            break;
        }

    }


    for (int y = img.rows - 960; y < img.rows - 60; y++) {
        if (((190 < img.at<uchar>(y, 0)) && (img.at<uchar>(y, 0) < 230)) && ((190 < img.at<uchar>(y, 1)) && (img.at<uchar>(y, 1) < 230))) {
            j = y;
            while (j < img.rows - 2) {
                for (int i = 0; i < img.cols; i++) {
                    if ((190 < img.at<uchar>(j, i)) && (img.at<uchar>(j, i) < 230)) {
                        img.at<uchar>(j, i) = 248;
                    }
                    if ((190 < img.at<uchar>(j + 1, i)) && (img.at<uchar>(j + 1, i) < 230)) {
                        img.at<uchar>(j + 1, i) = 248;
                    }
                    if ((190 < img.at<uchar>(j + 2, i)) && (img.at<uchar>(j + 2, i) < 230)) {
                        img.at<uchar>(j + 2, i) = 248;
                    }
                }
                j = j + 50;
            }
            break;
        }
    }
}



int main()
{







    HWND hGameWindow = FindWindow(NULL, _T("Agar.io - Google Chrome")); //Diep.io - Google Chrome
    if (hGameWindow == NULL) {
        std::vector<std::string> windowTitles;
        cout << "failed to find window!";

    }
    else {
        Sleep(2000);
        //getting windows height/width
        HDC dc1 = GetDC(hGameWindow); RECT windo;
        GetClientRect(hGameWindow, &windo);
        GlobalWHeight = windo.bottom;
        GlobalWWidth = windo.right;
        ReleaseDC(hGameWindow, dc1);

        Mat scrn;
        Mat hsvImage;
        Mat resizedImg;
        Mat mask;
        Mat splitscrn[3];


        while (1) {


            auto start = high_resolution_clock::now();
            scrn = getmat(hGameWindow);
            resize(scrn, scrn, Size(scrn.cols / 10, scrn.rows / 10));
            cvtColor(scrn, hsvImage, COLOR_BGR2HSV);
            split(hsvImage, splitscrn);
            inRange(splitscrn[1], 0, 100, mask);
            bitwise_not(mask, mask);
            playtest2(mask);
            auto stop = high_resolution_clock::now();
            cout << "FPS:" << 1000 / duration_cast<milliseconds>(stop - start).count() << "\n";
            cout << duration_cast<milliseconds>(stop - start) << "\n";





            if (GetAsyncKeyState(VK_NUMPAD5)) {
                Sleep(5000);
            }

            if (GetAsyncKeyState(VK_NUMPAD0)) {
                return 0;
            }






        }



    }



}