#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat image = Mat::zeros(100, 100, CV_8UC3);
	imshow("", image);
	cout << "ok" << endl;
	waitKey();

	return 0;
}