#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;

class WatershedSegmenter{
private:
    cv::Mat markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(markers, CV_32S);
    }

    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, markers);
        markers.convertTo(markers,CV_8U);
        return markers;
    }
};


int main(int argc, char* argv[])
{
    int n;
	float alpha; /**< Simple contrast control */
	int beta;  /**< Simple brightness control */
	Mat image1= cv::imread(argv[1],1);
	Mat src = Mat::zeros( image1.size(), image1.type() );
	FILE* fichier = NULL;
	fichier = fopen("/root/sharefolder/contrast.txt", "r");
    fscanf(fichier,"%f",&alpha);
    fclose(fichier);
	fichier = fopen("/root/sharefolder/brightness.txt", "r");
    fscanf(fichier,"%d",&beta);
    fclose(fichier);
	for( int y = 0; y < image1.rows; y++ )
    { 
		for( int x = 0; x < image1.cols; x++ )
        { 
			for( int c = 0; c < 3; c++ )
            {
				src.at<Vec3b>(y,x)[c] =
				saturate_cast<uchar>( alpha*( image1.at<Vec3b>(y,x)[c] ) + beta );
            }
		}
    }
    cv::imwrite("changement.jpg", src); 
	
	
	
	
    cv::Mat image = src.clone();;;
    cv::Mat binary;// = cv::imread(argv[2], 0);
    double t = (double)getTickCount();
    cv::cvtColor(image, binary, CV_BGR2GRAY);
    cv::threshold(binary, binary, 100, 255, THRESH_BINARY);

    // Eliminate noise and smaller objects
    cv::Mat fg;
    cv::erode(binary,fg,cv::Mat(),cv::Point(-1,-1),2);

    // Identify image pixels without objects
    
    fichier = fopen("/root/sharefolder/filtre.txt", "r");
    fscanf(fichier,"%d",&n);
    fclose(fichier);
    cv::Mat bg;
    cv::dilate(binary,bg,cv::Mat(),cv::Point(-1,-1),3);
    cv::threshold(bg,bg,n, 128,cv::THRESH_BINARY_INV);


    // Create markers image
    cv::Mat markers(binary.size(),CV_8U,cv::Scalar(0));
    markers= fg+bg;

    // Create watershed segmentation object
    WatershedSegmenter segmenter;
    segmenter.setMarkers(markers);

    cv::Mat result = segmenter.process(image);
    result.convertTo(result,CV_8U);
    t = ((double)getTickCount() - t)/getTickFrequency();
    fichier = fopen("/root/sharefolder/time.txt", "w");
    fprintf(fichier,"%lf",t);
    fclose(fichier);
    imwrite("algo_resultat.jpg", result);
    //cv::waitKey(0);

    return 0;
}