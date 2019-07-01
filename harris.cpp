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
    int max_thresh = 255;
	FILE* fichier = NULL;
	Mat entree_n,entree_c,sortie,dst_norm,dst_norm_scaled,src;
	int n=200;
   /*------------------------------------*/
	Mat image= cv::imread(argv[1],1);
	src = Mat::zeros( image.size(), image.type() );
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
				src.at<Vec3b>(y,x)[c] =
				saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
            }
		}
    }
    cv::imwrite("changement.jpg", src); 
	/*------------------------------------*/
	entree_c = src.clone();
	cvtColor(entree_c, entree_n, cv::COLOR_RGB2GRAY);
	/*------------------------------------*/
    fichier = fopen("/root/sharefolder/filtre.txt", "r");
    fscanf(fichier,"%d",&n);
    fclose(fichier);
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    double t = (double)getTickCount();
      /// Detecting corners
    cornerHarris( entree_n, sortie, blockSize, apertureSize, k, BORDER_DEFAULT );

      /// Normalizing
    normalize( sortie, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    /// Drawing a circle around corners
    for( int j = 0; j < dst_norm.rows ; j++ )
    { 
        for( int i = 0; i < dst_norm.cols; i++ )
        {
            if( (int) dst_norm.at<float>(j,i) > n )
            {
                circle( entree_c, Point( i, j ), 5,  Scalar(0,0,255), 2, 8, 0 );
            }
        }
    }
    t = ((double)getTickCount() - t)/getTickFrequency();
    fichier = fopen("/root/sharefolder/time.txt", "w");
    fprintf(fichier,"%lf",t);
    fclose(fichier);
    imwrite("algo_resultat.jpg", entree_c);
    return 0;
}
