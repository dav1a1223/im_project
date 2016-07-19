#include <iostream>
#include "opencv2/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#define _PICPATH "E:\\noideal-eredeti.jpg"

using namespace std;
using namespace cv;
void LUTtest(Mat& image);
void showIMG(Mat& src);

//define the raw HSL color code type
struct HSLDataType
{
	int *Hraw;
	int *Sraw;
	int *Lraw;
	int length;
};

//this class will handle all decoding process correctly
class decodeData
{
public:
	decodeData(Mat& src,int n);
	decodeData(int n);
	decodeData(Mat& src);
	void showResult();
	HSLDataType getResult();
	int arraySize();
	int sampleSize();
	void showH();
	void showS();
	void showL();
private:
	HSLDataType HSLData;
	int sampleSizeN;
};
int decodeData::arraySize()
{
	//for HSL color scope, we retrun array size that the whole map is
	return sampleSizeN*sampleSizeN*3;
}
int decodeData::sampleSize()
{
	return sampleSizeN;
}

//this Constructor take a IMG Mat which should be in openCV default BGR Mat and a interger that present what resolution we want for the analysis
decodeData::decodeData(Mat& src,int n)
{
	sampleSizeN=n;
	HSLData.length=arraySize();
	HSLData.Hraw=new int[HSLData.length];
	HSLData.Sraw=new int[HSLData.length];
	HSLData.Lraw=new int[HSLData.length];
	//since we only want the avarage color, we resize it to get the matrix
	resize(src,src,Size(sampleSizeN,sampleSizeN));
	//and for the most important thing, please, turn ALL Data in the right color scope
	cvtColor(src,src,COLOR_BGR2HLS);
	LUTtest(src);
	for(int i=0;i<sampleSizeN;i++)
		for(int j=0;j<sampleSizeN;j++)
			HSLData.Hraw[i*sampleSizeN+j]=src.at<Vec3b>(i,j)[0];
	for(int i=0;i<sampleSizeN;i++)
		for(int j=0;j<sampleSizeN;j++)
			HSLData.Lraw[i*sampleSizeN+j]=src.at<Vec3b>(i,j)[1];
	for(int i=0;i<sampleSizeN;i++)
		for(int j=0;j<sampleSizeN;j++)
			HSLData.Sraw[i*sampleSizeN+j]=src.at<Vec3b>(i,j)[2];
	showResult();

}
HSLDataType decodeData::getResult()
{
	return HSLData;
}
void decodeData::showResult()
{
	for(int i=0;i<sampleSizeN;i++)
	{
		for(int j=0;j<sampleSizeN;j++)
		{
			cout<<HSLData.Hraw[i*sampleSizeN+j]<<'\t';
		}
		cout<<endl;
	}
	cout<<endl;
	for(int i=0;i<sampleSizeN;i++)
	{
		for(int j=0;j<sampleSizeN;j++)
		{
			cout<<HSLData.Sraw[i*sampleSizeN+j]<<'\t';
		}
		cout<<endl;
	}
	cout<<endl;
	for(int i=0;i<sampleSizeN;i++)
	{
		for(int j=0;j<sampleSizeN;j++)
		{
			cout<<HSLData.Lraw[i*sampleSizeN+j]<<'\t';
		}
		cout<<endl;
	}
}
void decodeData::showH()
{
	Mat temp(3,3,CV_8UC3,Scalar(128,128,256));

	for(int i=0;i<sampleSizeN;i++)
		for(int j=0;j<sampleSizeN;j++)
			temp.at<Vec3b>(i,j)[0]=HSLData.Hraw[i*sampleSizeN+j]*26+13;

	cvtColor(temp,temp,COLOR_HLS2BGR);
	resize(temp,temp,Size(540,540),0,0,INTER_NEAREST);
	showIMG(temp);
}
void decodeData::showL()
{
	Mat temp(3,3,CV_8UC3,Scalar(128,128,0));

	for(int i=0;i<sampleSizeN;i++)
		for(int j=0;j<sampleSizeN;j++)
			temp.at<Vec3b>(i,j)[1]=HSLData.Lraw[i*sampleSizeN+j]*26+13;

	cvtColor(temp,temp,COLOR_HLS2BGR);
	resize(temp,temp,Size(540,540),0,0,INTER_NEAREST);
	showIMG(temp);
}
void decodeData::showS()
{
	Mat temp(3,3,CV_8UC3,Scalar(0,128,256));

	for(int i=0;i<sampleSizeN;i++)
		for(int j=0;j<sampleSizeN;j++)
			temp.at<Vec3b>(i,j)[2]=HSLData.Sraw[i*sampleSizeN+j]*26+13;

	cvtColor(temp,temp,COLOR_HLS2BGR);
	resize(temp,temp,Size(540,540),0,0,INTER_NEAREST);
	showIMG(temp);
}
void showIMG(Mat& src)
{
	namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", src );
    waitKey(0);
}
void LUTtest(Mat& image)
{
	//this is the main function the decode data
	//which now divide color in roughly 10 color
	//we use Look-Up-Table to speed up the task
	//But for now, it's work awful and even get the process slower.
	Mat lut_mat(1,256,CV_8UC3);
	int j=0;
	for(int i=0;i<256;i++)
	{
		lut_mat.at<Vec3b>(0,i)[0]=(i/26);
		lut_mat.at<Vec3b>(0,i)[1]=(i/26);
		lut_mat.at<Vec3b>(0,i)[2]=(i/26);
	}
//	showIMG(lut_mat);
	LUT(image,lut_mat,image);
	//showIMG(image);
}
int main()
{
	std::cout << "OpenCV Version: " << CV_VERSION << std::endl;
    cout << "Hello world!" << endl;

    Mat srcPic=imread(_PICPATH,CV_LOAD_IMAGE_COLOR);
	if(! srcPic.data )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
	showIMG(srcPic);
	resize(srcPic,srcPic,Size(3,3));

	resize(srcPic,srcPic,Size(540,540),0,0,INTER_NEAREST);
	showIMG(srcPic);
	decodeData testA(srcPic,3);
	testA.showH();
	testA.showS();
	testA.showL();
//	cvtColor(srcPic,srcPic,COLOR_BGR2GRAY);


//	double th1(50),th2(100),th3(150),th4(200);
//
//	threshold(grayImg,grayImg1,th1,255,THRESH_BINARY);
//	threshold(grayImg,grayImg2,th2,255,THRESH_BINARY);
//	threshold(grayImg,grayImg3,th3,255,THRESH_BINARY);
//	threshold(grayImg,grayImg4,th4,255,THRESH_BINARY);
//
//	showIMG(grayImg1);
//	showIMG(grayImg2);
//	showIMG(grayImg3);
//	showIMG(grayImg4);
//	addWeighted(grayImg1,0.8,grayImg2,0.2,0.0,finalImg);
//	addWeighted(finalImg,0.8,grayImg3,0.2,0.0,finalImg);
//	addWeighted(finalImg,0.8,grayImg4,0.2,0.0,finalImg);
//	showIMG(finalImg);

	//LUTtest(srcPic);
    return 0;
}
