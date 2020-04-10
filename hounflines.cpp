// hounflines.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void rollplay(Mat srcImage, double angle);//旋转无裁剪显示
void houghlines(int thresholdvalue, void* src);//霍夫直线检测

int main()
{
    Mat srcImage = imread("D:\\opencv\\myproject\\1picture\\metal-part-distorted-03.png");
    Mat dstImage;
    int thresholdvalue =100  ;
    double angle = 10.0;
    if (srcImage.empty())
    {
        cout << "can't' get the picture." << endl;
        return -1;
    }
    rollplay(srcImage,angle);
    createTrackbar("cannythresholdvalue", "srcImage",&thresholdvalue,255,houghlines);
    houghlines(thresholdvalue, &srcImage);
    return 0;
}

void rollplay(Mat srcImage,double angle)
{
    Mat desImage;
    Point2f center(srcImage.cols / 2, srcImage.rows);
    Mat rot = getRotationMatrix2D(center, angle,1);
    Rect bbox = RotatedRect(center, srcImage.size(), angle).boundingRect();
    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
    warpAffine(srcImage, desImage, rot, bbox.size());
    imshow("rollplay", desImage);
    waitKey(0);
}

void houghlines(int thresholdvalue,void*src)
{
    Mat srcImage = *(Mat*)src;
    Mat srcImage1 = *(Mat*)src;
    Mat srcGray,resultImage;
    vector<Vec4f> lines;
    cvtColor(srcImage, srcGray, CV_BGR2GRAY);
    threshold(srcGray, srcGray, 100, 255, THRESH_BINARY | THRESH_OTSU);
    Canny(srcGray, srcGray, thresholdvalue, thresholdvalue * 2, 3, false);
    imshow("srcGray", srcGray);

    int thresholdh = 40;
    double linemin = srcImage.cols / 10.0;
    HoughLines(srcGray, lines, 1,CV_PI/180.0, thresholdh,linemin,20);
    for (int i=0; i< lines.size();++i)
    {
        float rho = lines[0][0], theta = lines[0][1];
        Point pt1, pt2;
        double a = cos(theta);
        double b = sin(theta);
        int a1 = int(a * 1000);
        double x0 = a * rho;
        double y0 = b * rho;
        pt1.x = saturate_cast<int>(x0 + 1000 * (-b));
        pt1.y = saturate_cast<int>(y0 + a1);
        pt2.x = saturate_cast<int>(x0 - 1000 * (-b));
        pt2.y = saturate_cast<int>(y0 - 1000 * (-b));
        line(srcImage, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
    }
    imshow("srcImage", srcImage);
    
    HoughLinesP(srcGray, lines, 1, CV_PI / 180.0, thresholdh,linemin,20);
    Vec4f pts;
    for (int i = 0; i < lines.size();i++)
    {
        pts = lines[i];
       
            line(srcImage1, Point(pts[0], pts[1]), Point(pts[2], pts[3]), Scalar(0, 0, 255), 2, 8, 0);
    }
    imshow("srcImage1", srcImage1);
    waitKey(0);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
