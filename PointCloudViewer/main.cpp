#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>

using namespace cv;
using namespace std;

#define MAX_NUM 3
#define DELTA_TRANSLATION_X 0.005

int main()
{

	float f = 364, cx = 256.4, cy = 202;

	viz::Viz3d myWindow("Coordinate Frame");
	//myWindow.showWidget("Coordinate Widget", viz::WCoordinateSystem());

	for (int i = 1; i < MAX_NUM + 1; i++)
	{
		string src_name = "./data/depth";
		if (i < 10)
		{
			src_name += "0";
		}
		src_name += to_string(i) + ".png";

		Mat src, dst;
		src = imread(src_name, IMREAD_UNCHANGED);
		src.convertTo(dst, CV_32FC1, 1 / 5000.0f);

		Mat cloud(1, dst.rows * dst.cols, CV_32FC3);
		Point3f* data = cloud.ptr<Point3f>();
		for (int row = 0; row < dst.rows; row++)
		{
			for (int col = 0; col < dst.cols; col++)
			{
				float depth = dst.at<float>(row, col);

				int idx = row * dst.cols + col;
				data[idx].x = (col - cx) / f * depth;
				data[idx].y = (row - cy) / f * depth;
				data[idx].z = depth;
			}
		}

		Vec3f cam_pos(0.0f + DELTA_TRANSLATION_X * (i-1), 0.0f, 0.0f), cam_focal_point(0.0f + DELTA_TRANSLATION_X * (i-1), 0.0f, 0.01f), cam_y_dir(0.0f, -1.0f, 0.0f);
		Affine3f cam_pose = viz::makeCameraPose(cam_pos, cam_focal_point, cam_y_dir);
		Affine3f cam_transform_matrix = viz::makeTransformToGlobal(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f), cam_pos);
		Affine3f cloud_pose_camera = Affine3f().translate(cam_pos);
		Affine3f cloud_pose_global = cam_transform_matrix * cloud_pose_camera;

		viz::WCloud cloud_widget(cloud, viz::Color::white());
		myWindow.showWidget("cloud" + to_string(i), cloud_widget, cloud_pose_global);

		viz::WCameraPosition cpw(0.1); // Coordinate axes
		viz::WCameraPosition cpw_frustum(Vec2f(0.8, 0.5), 0.1); // Camera frustum
		myWindow.showWidget("CPW" + to_string(i), cpw, cam_pose);
		myWindow.showWidget("CPW_FRUSTUM" + to_string(i), cpw_frustum, cam_pose);
	}
	

	myWindow.spin();

	return 0;
}