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

int main(int argc, char** argv)
{
	float alpha; /**< Simple contrast control */
	int beta;  /**< Simple brightness control */
	FILE* fichier = NULL;
	int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
	Mat src, src_gray, dst,abs_dst,src1;
    /*------------------------------------*/
	Mat image= cv::imread(argv[1],1);
	src1 = Mat::zeros( image.size(), image.type() );
	fichier = fopen("/root/sharefolder/contrast.txt", "r");
    fscanf(fichier,"%f",&alpha);
    fclose(fichier);
	fichier = fopen("/root/sharefolder/brightness.txt", "r");
    fscanf(fichier,"%d",&beta);
    fclose(fichier);
	/*------------------------------------*/
	for( int y = 0; y < image.rows; y++ )
    { 
		for( int x = 0; x < image.cols; x++ )
        { 
			for( int c = 0; c < 3; c++ )
            {
				src1.at<Vec3b>(y,x)[c] =
				saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
            }
		}
    }
    cv::imwrite("changement.jpg", src1); 
	/*------------------------------------*/
	
	src = src1.clone();;
    
    fichier = fopen("/root/sharefolder/filtre.txt", "r");
    fscanf(fichier,"%d",&kernel_size);
    fclose(fichier);
    double t = (double)getTickCount();
    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
    cvtColor( src, src_gray, CV_BGR2GRAY );
    Laplacian( src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( dst, abs_dst );
    t = ((double)getTickCount() - t)/getTickFrequency();
    fichier = fopen("/root/sharefolder/time.txt", "w");
    fprintf(fichier,"%lf",t);
    fclose(fichier);
    imwrite("algo_resultat.jpg", abs_dst);
    return 0;
}
