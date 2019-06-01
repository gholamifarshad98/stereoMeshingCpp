#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
cv::Mat IntegraImage(cv::Mat);
int main()
{

	Mat image;
	image = imread("farshad.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	cv::Mat output(image.cols,image.rows, CV_64F);
	output = IntegraImage(image);
	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.
	
	waitKey(0);                                          // Wait for a keystroke in the window
	return 0;
}



cv::Mat IntegraImage(cv::Mat input) {
	int numOfColumns = input.cols;
	int numOfRows = input.rows;
	cv::Mat output(numOfColumns, numOfRows, CV_64F);
	output.at<double>(0, 0) = input.at<uchar>(0, 0);
	for (int u = 1; u < numOfColumns; u=u+1) {
		output.at<double>(u,0) = output.at<double>(u - 1, 0) + input.at<uchar>(u, 0);
		cout << output.at<double>(u, 0) << "  ";
	}
	for (int v = 1; v < numOfRows; v++) {
		output.at<double>(0, v) = output.at<double>(0,v-1) + input.at<uchar>(0,v);
	}

	for (int u = 1; u < numOfColumns; u++) {
		for (int v = 1; v < numOfRows; v++) {
			output.at<double>(u, v) = output.at<double>(u - 1, v) + output.at<double>(u, v - 1) + input.at<uchar>(u, v) - output.at<double>(u - 1, v - 1);
		}
	}
	return output;
}

cv::Mat SumImage(cv::Mat input){
	cv::Mat integralImage=IntegraImage(input);
	int numOfColumns = input.cols;
	int numOfRows = input.rows;
	cv::Mat output(numOfColumns, numOfRows, CV_64F);
	for (int u = 0; u < numOfColumns; u++) {
		for (int v = 0; v < numOfRows; v++) {

		}
	}


} 