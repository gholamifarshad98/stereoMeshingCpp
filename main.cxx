#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<math.h>
#include<vector>
using namespace cv;
using namespace std;
cv::Mat IntegraImage(cv::Mat);
int rho = 3;
int n = (rho + 1) ^ 2;
int numOfLyr = 25;
int main()
{

	Mat* rightImage;
	Mat* leftImage;
	ReadBothImages(leftImage, rightImage);


	//cv::Mat muLeft =
	cv::Mat* output;
	//IntegraImage(rightImage, output);
	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	//imshow("Display window", rightImage*);                   // Show our image inside it.
	
	waitKey(0);                                          // Wait for a keystroke in the window
	return 0;
}


void ReadBothImages(cv::Mat *leftImage, cv::Mat *rightImage){
try {
	*rightImage = imread("farshad.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the right image
	*leftImage = imread("farshaad.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the left image

	if (!rightImage->data)                             // Check for invalid input
	{
		throw "right";
	}
	if (!leftImage->data)                             // Check for invalid input
	{
		throw "left";
	}
}
catch (char* error) {
	cout << "can not load the " << error << " iamge" << endl;
	int x;
	cin >> x;
	return ;
}




////////////////////////////////////////////////////////////////////
/// In this part we can calculate integral Image.
////////////////////////////////////////////////////////////////////
void IntegraImage(cv::Mat* input, cv::Mat* output){
	int numOfColumns = input->cols;
	int numOfRows = input->rows;

	//cv::Mat output(numOfColumns, numOfRows, CV_64F);
	output->at<double>(0, 0) = input->at<uchar>(0, 0);
	for (int u = 1; u < numOfColumns; u=u+1) {
		output->at<double>(u,0) = output->at<double>(u - 1, 0) + input->at<uchar>(u, 0);
		//cout << output.at<double>(u, 0) << "  ";
	}
	for (int v = 1; v < numOfRows; v++) {
		output->at<double>(0, v) = output->at<double>(0,v-1) + input->at<uchar>(0,v);
	}
	for (int u = 1; u < numOfColumns; u++) {
		for (int v = 1; v < numOfRows; v++) {
			output->at<double>(u, v) = (output->at<double>(u - 1, v) + output->at<double>(u, v - 1) + input->at<uchar>(u, v) - output->at<double>(u - 1, v - 1))/n;
		}
	}
}


////////////////////////////////////////////////////////////////////
/// In this part we can calculate sqr integral Image.
////////////////////////////////////////////////////////////////////
cv::Mat SqrIntegraImage(cv::Mat input) {
	int numOfColumns = input.cols;
	int numOfRows = input.rows;
	cv::Mat output(numOfColumns, numOfRows, CV_64F);
	for (int u = 0; u < numOfColumns; u = u + 1) {
		for (int v = 0; v < numOfRows; v = v + 1) {
			input.at<double>(u, v) = input.at<double>(u, v)*input.at<double>(u, v);
		}
	}
	output.at<double>(0, 0) = input.at<uchar>(0, 0);
	for (int u = 1; u < numOfColumns; u = u + 1) {
		output.at<double>(u, 0) = output.at<double>(u - 1, 0) + input.at<uchar>(u, 0);
		cout << output.at<double>(u, 0) << "  ";
	}
	for (int v = 1; v < numOfRows; v++) {
		output.at<double>(0, v) = output.at<double>(0, v - 1) + input.at<uchar>(0, v);
	}

	for (int u = 1; u < numOfColumns; u++) {
		for (int v = 1; v < numOfRows; v++) {
			output.at<double>(u, v) = output.at<double>(u - 1, v) + output.at<double>(u, v - 1) + input.at<uchar>(u, v) - output.at<double>(u - 1, v - 1);
		}
	}
	return output;
}


///////////////////////////////////////////////////////////////////////
/// in this function we can calculate average of intensits in very moving Windows
///////////////////////////////////////////////////////////////////////
cv::Mat CalMu(cv::Mat input){
	cv::Mat integralImage=IntegraImage(input);
	int numOfColumns = input.cols;
	int numOfRows = input.rows;
	cv::Mat output(numOfColumns, numOfRows, CV_64F);
	for (int u = 0+rho+1; u < numOfColumns - rho - 1; u++) {
		for (int v = 0 + rho + 1; v < numOfRows - rho - 1; v++) {
			double r1 = integralImage.at<double>(u + rho, v + rho);
			double r2 = integralImage.at<double>(u - rho - 1, v - rho -1);
			double r3 = integralImage.at<double>(u - rho - 1, v + rho);
			double r4 = integralImage.at<double>(u + rho, v - rho - 1);
			output.at<double>(u, v) = (r1 + r2 - r3 - r4)/n;
		}
	}
	return output;
} 

/////////////////////////////////////////////////////////////////////////////////////
/// In this function we can calculate Sigma for left image. Note thst Sigma is standard devistion.
/////////////////////////////////////////////////////////////////////////////////////
cv::Mat SigamL(cv::Mat intagalImage, cv::Mat sqrIntegralImage) {
	int numOfColumns = intagalImage.cols;
	int numOfRows = intagalImage.rows;
	cv::Mat output(numOfColumns, numOfRows, CV_64F);
	for (int u = 0 + rho + 1; u < numOfColumns - rho - 1; u++) {
		for (int v = 0 + rho + 1; v < numOfRows - rho - 1; v++) {
			output.at<double>(u, v) = sqrt(sqrIntegralImage.at<double>(u,v)- intagalImage.at<double>(u, v)*intagalImage.at<double>(u, v));
		}
	}
	return output;
}




/////////////////////////////////////////////////////////////////////////////////////
/// In this function we can calculate Sigma for Right image. Note thst Sigma is standard devistion.
/////////////////////////////////////////////////////////////////////////////////////

vector<cv::Mat> SigamR(cv::Mat intagalImage, cv::Mat sqrIntegralImage) {
	int numOfColumns = intagalImage.cols;
	int numOfRows = intagalImage.rows;
	vector<cv::Mat> Outputs;
	for (int i = 1; i < numOfLyr; i = i + 1) {
		cv::Mat output(numOfColumns, numOfRows, CV_64F);
		for (int u = 0 + rho + 1 + i ; u < numOfColumns - rho - 1 ; u++) {
			for (int v = 0 + rho + 1; v < numOfRows - rho - 1; v++) {
				output.at<double>(u, v) = sqrt(sqrIntegralImage.at<double>(u-i, v) - intagalImage.at<double>(u, v)*intagalImage.at<double>(u, v));
			}
		}
		Outputs.push_back(output);
	}
	return Outputs;
}		



