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
int dMin = 1;
int dMax = 25;
int tau = 4;
int main()
{
	Mat* rightImage = new Mat();
	Mat* leftImage = new Mat();
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
	Mat* Cost = new Mat(numOfRows, numOfColumns, CV_64F);
	Mat* Layer = new Mat(numOfRows, numOfColumns, CV_64F);

	CalMu(rightImage, IntegralRightImage, MuRightImage);
	CalMu(leftImage, IntegralLeftImage, MuLeftImage);
	SqrIntegralImage(rightImage, SqrIntegralRightImage);
	SqrIntegralImage(leftImage, SqrIntegralLeftImage);
	Sigam(MuRightImage, SqrIntegralRightImage, SigmaRightImage);
	Sigam(MuLeftImage, SqrIntegralLeftImage, SigmaLeftImage);
	


	int v = numOfRows - rho - 2;
	int u_r = 0;
	for (int u_l = rho + dMax + 1; u_l < numOfColumns - rho - dMax - 2; u_l = u_l + 1) {
		double tempCost = 0;
		Cost->at<double >(v, u_l) = 0;
		Layer->at<double>(v, u_l) = dMax;
		//cout << "u_l is " << u_l << "and layer is "<< (int)Layer->at<uchar>(v, u_l) <<endl;
		for (int d = dMin; d <= dMax; d = d + 1) {
			u_r = u_l - d;
			double SigmaLeftRight = SigmaLeftImage->at<double>(v, u_l)*SigmaRightImage->at<double>(v, u_r);
			if (SigmaLeftRight != 0) {
				double Sumation = 0;
				for (int i = -rho; i <= rho; i = i + 1) {
					for (int j = -rho; j <= rho; j = j + 1) {
						Sumation = Sumation + rightImage->at<double>(v + j, u_r + i)*leftImage->at<double>(v + j, u_l + i) - MuRightImage->at<double>(v, u_r)*MuLeftImage->at<double>(v, u_l)*n;
					}
				}
				tempCost = Sumation / (n*SigmaLeftRight);
				if (tempCost > Cost->at<double >(v, u_l)) {
					Cost->at<double >(v, u_l) = tempCost;
					Layer->at<double >(v, u_l) = d;
				}
			}
		}
	}

	std::vector<int> SRPorpation;
	SRPorpation.assign(3, 0);
	for (v = numOfRows - rho - 3; v >= rho + 1; v = v - 1) {
		int u_r = 0;
		for (int u_l = rho + dMax + 1; u_l <= numOfRows - rho - dMax - 2; u_l = u_l + 1) {
			double tempCost = 0;
			Cost->at<double >(v, u_l) = 0;
			Layer->at<double >(v, u_l) = dMax;
			SRPorpation[0] =(int)Layer->at<double >(v + 1, u_l - 1);
			SRPorpation[1] = (int)Layer->at<double >(v + 1, u_l);
			SRPorpation[2] = (int)Layer->at<double >(v + 1, u_l + 1);
			int _dMin = SRPorpation[0];
			int _dMax = SRPorpation[0];
			for (int k = 1; k <= 2; k = k + 1) {
				//cout << "k is " << k << endl;
				if (_dMin > SRPorpation[k])
					_dMin = SRPorpation[k];
				if (_dMax < SRPorpation[k])
					_dMax = SRPorpation[k];
			}
			_dMin = _dMin - tau;
			_dMax = _dMax + tau;
			if (_dMin <= 0) {
				//cout << "you may have error hear!!!!" << "_dmin is "<<_dMin<<endl;
				_dMin = 0;
			}
			if (_dMax <= 0) {
				//cout << "you may have error hear!!!! dMax is Negetive or zero" << endl;
				return -1;
			}
			for (int d = _dMin; d <= _dMax; d = d + 1) {
				u_r = u_l - d;
				double SigmaLeftRight = SigmaLeftImage->at<double>(v, u_l)*SigmaRightImage->at<double>(v, u_r);
				
				if (SigmaLeftRight != 0) {
					double Sumation = 0;
					for (int i = -rho; i <= rho; i = i + 1) {
						for (int j = -rho; j <= rho; j = j + 1) {
							Sumation = Sumation + rightImage->at<double>(v + j, u_r + i)*leftImage->at<double>(v + j, u_l + i) - MuRightImage->at<double>(v, u_r)*MuLeftImage->at<double>(v, u_l)*n;
						}
					}
					tempCost = Sumation / (n*SigmaLeftRight);
					if (tempCost > Cost->at<double >(v, u_l)) {
						Cost->at<double >(v, u_l) = tempCost;
						Layer->at<double >(v, u_l) = d;
					}
				}


			}
		}
	}

	for (int i = 30; i < numOfColumns-40;i++) {
		cout << Layer->at<double >(100, i) << endl;

	}
	cout << "hello" << endl;
	///namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", *Layer); 
	//int x;
	//cin >> x;// Show our image inside it.
	waitKey(0);                                          // Wait for a keystroke in the window
	return 0;
}



////////////////////////////////////////////////////////////////////
/// In this part we can load two Images.
////////////////////////////////////////////////////////////////////
void ReadBothImages(cv::Mat* leftImage, cv::Mat* rightImage) {

	try {
		*rightImage = imread("view1.png", CV_LOAD_IMAGE_GRAYSCALE);   // Read the right image
		rightImage->convertTo(*rightImage, CV_64F);
		*rightImage = *rightImage / 255;
		*leftImage = imread("view0.png", CV_LOAD_IMAGE_GRAYSCALE);   // Read the left image
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
		output->at<double>(0,u) = output->at<double>(0, u - 1) + input->at<double>(0 ,u);
		//cout << output.at<double>(u, 0) << "  ";
	}

	for (int v = 1; v < numOfRows; v++) {
		output->at<double>(v, 0) = output->at<double>(v-1,0) + input->at<double>(v,0);
	}
	for (int u = 1; u < numOfColumns; u++) {
		for (int v = 1; v < numOfRows; v++) {
			output->at<double>(v,u) = (output->at<double>(v, u - 1) + output->at<double>(v - 1, u) + input->at<double>(v,u) - output->at<double>(v - 1, u- 1))/n;
		}
	}
}

////////////////////////////////////////////////////////////////////
/// In this part we can calculate sqr integral Image.
////////////////////////////////////////////////////////////////////
void SqrIntegralImage(cv::Mat* input , cv::Mat* output) {
	for (int u = 0; u < numOfColumns; u = u + 1) {
		for (int v = 0; v < numOfRows; v = v + 1) {
			input->at<double>(v, u) = input->at<double>(v, u)*input->at<double>(v, u);
		}
	}
	IntegralImage(input, output);
}

///////////////////////////////////////////////////////////////////////
/// in this function we can calculate average of intensits in very moving Windows
///////////////////////////////////////////////////////////////////////
void CalMu(cv::Mat* input , cv::Mat* integralImage, cv::Mat* Mu){
	IntegralImage(input, integralImage);
	for (int u = 0+rho+1; u < numOfColumns - rho - 1; u++) {
		for (int v = 0 + rho + 1; v < numOfRows - rho - 1; v++) {
			double r1 = integralImage->at<double>(v + rho, u + rho);
			double r2 = integralImage->at<double>(v - rho - 1,u - rho - 1);
			double r3 = integralImage->at<double>(v + rho,u - rho - 1);
			double r4 = integralImage->at<double>(v - rho - 1, u + rho);
			Mu->at<double>(v, u) = (r1 + r2 - r3 - r4)/n;
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
					Sumation= Sumation+sqrIntegralImage->at<double>(v + j, u + i) / n - MuImage->at<double>(v,u)*MuImage->at<double>(v, u);
				}
			}
			output->at<double>(v,u) = sqrt(Sumation);
		}
	}
}


