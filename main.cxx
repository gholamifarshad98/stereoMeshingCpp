#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<math.h>
#include<vector>
using namespace cv;
using namespace std;
void ReadBothImages(cv::Mat *, cv::Mat *);
void IntegralImage(cv::Mat* , cv::Mat*);
void SqrIntegralImage(cv::Mat* , cv::Mat*);
void CalMu(cv::Mat* , cv::Mat* , cv::Mat*);
void Sigam(cv::Mat*, cv::Mat*, cv::Mat*);
int numOfColumns;
int numOfRows;
int rho = 3;
int n = (rho + 1) ^ 2;
int numOfLyr = 25;
int main()
{
	Mat* rightImage=new Mat();
	Mat* leftImage=new Mat();
	ReadBothImages(leftImage, rightImage);
	numOfColumns = leftImage->cols;
	numOfRows = leftImage->rows;
	Mat* IntegralRightImage = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* IntegralLeftImage = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* MuRightImage = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* MuLeftImage = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* SqrIntegralRightImage = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* SqrIntegralLeftImage = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* SigmaRightImage = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* SigmaLeftImage = new Mat(numOfRows, numOfColumns, CV_64F);
	CalMu(rightImage, IntegralRightImage, MuRightImage);
	CalMu(leftImage, IntegralLeftImage, MuLeftImage);
	SqrIntegralImage(rightImage, SqrIntegralRightImage);
	SqrIntegralImage(leftImage, SqrIntegralLeftImage);
	Sigam(MuRightImage, SqrIntegralRightImage, SigmaRightImage);
	Sigam(MuLeftImage, SqrIntegralLeftImage, SigmaLeftImage);





	
	///namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", *rightImage);                   // Show our image inside it.
	
	waitKey(0);                                          // Wait for a keystroke in the window
	return 0;
}


////////////////////////////////////////////////////////////////////
/// In this part we can load two Images.
////////////////////////////////////////////////////////////////////
void ReadBothImages(cv::Mat* leftImage, cv::Mat* rightImage) {

	try {
		*rightImage = imread("farshad.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the right image
		rightImage->convertTo(*rightImage, CV_64F);
		*rightImage = *rightImage / 255;
		*leftImage = imread("farshad.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the left image
		leftImage->convertTo(*leftImage, CV_64F);
		*leftImage = *leftImage / 255;
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
	}
}

////////////////////////////////////////////////////////////////////
/// In this part we can calculate integral Image.
////////////////////////////////////////////////////////////////////
void IntegralImage(cv::Mat* input, cv::Mat* output){
	output->at<double>(0, 0) = input->at<double>(0, 0);
	for (int u = 1; u < numOfColumns; u=u+1) {
		output->at<double>(u,0) = output->at<double>(u - 1, 0) + input->at<double>(u, 0);
		//cout << output.at<double>(u, 0) << "  ";
	}
	for (int v = 1; v < numOfRows; v++) {
		output->at<double>(0, v) = output->at<double>(0,v-1) + input->at<double>(0,v);
	}
	for (int u = 1; u < numOfColumns; u++) {
		for (int v = 1; v < numOfRows; v++) {
			output->at<double>(u, v) = (output->at<double>(u - 1, v) + output->at<double>(u, v - 1) + input->at<double>(u, v) - output->at<double>(u - 1, v - 1))/n;
		}
	}
}

////////////////////////////////////////////////////////////////////
/// In this part we can calculate sqr integral Image.
////////////////////////////////////////////////////////////////////
void SqrIntegralImage(cv::Mat* input , cv::Mat* output) {
	for (int u = 0; u < numOfColumns; u = u + 1) {
		for (int v = 0; v < numOfRows; v = v + 1) {
			input->at<double>(u, v) = input->at<double>(u, v)*input->at<double>(u, v);
		}
	}
	IntegralImage(input, output);
}

///////////////////////////////////////////////////////////////////////
/// in this function we can calculate average of intensits in very moving Windows
///////////////////////////////////////////////////////////////////////
void CalMu(cv::Mat* input , cv::Mat* integralImage, cv::Mat* Mu){
	int numOfColumns = input->cols;
	int numOfRows = input->rows;
	IntegralImage(input, integralImage);
	for (int u = 0+rho+1; u < numOfColumns - rho - 1; u++) {
		for (int v = 0 + rho + 1; v < numOfRows - rho - 1; v++) {
			double r1 = integralImage->at<double>(u + rho, v + rho);
			double r2 = integralImage->at<double>(u - rho - 1, v - rho -1);
			double r3 = integralImage->at<double>(u - rho - 1, v + rho);
			double r4 = integralImage->at<double>(u + rho, v - rho - 1);
			Mu->at<double>(u, v) = (r1 + r2 - r3 - r4)/n;
		}
	}
} 

/////////////////////////////////////////////////////////////////////////////////////
/// In this function we can calculate Sigma for left image. Note that Sigma is standard devistion.
/////////////////////////////////////////////////////////////////////////////////////
void Sigam(cv::Mat* MuImage, cv::Mat* sqrIntegralImage, cv::Mat* output) {
	cv::Mat tempImage(numOfColumns, numOfRows, CV_64F);
	for (int u = 0 + rho ; u < numOfColumns - rho - 2; u++) {
		for (int v = 0 + rho ; v < numOfRows - rho - 2; v++) {
			double Sumation = 0.0;
			for (int i = -rho; i <= rho; i = i + 1) {
				for (int j = -rho; j <= rho; j = j + 1) {
					Sumation= Sumation+sqrIntegralImage->at<double>(u + i, v + j) / n - MuImage->at<double>(u, v)*MuImage->at<double>(u, v);
				}
			}
			output->at<double>(u, v) = sqrt(Sumation);
		}
	}
}


