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
	int thresh = 100;
    int max_thresh = 255;
    RNG rng(12345);
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
	Mat entree_n,entree_c,sortie,detected_edges,src;
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
	
    sortie.create( entree_c.size(), entree_c.type() );
    
    fichier = fopen("/root/sharefolder/filtre.txt", "r");
    fscanf(fichier,"%d",&thresh);
    fclose(fichier);
    double t = (double)getTickCount();
    blur( entree_n, detected_edges, Size(3,3) );
    Canny( detected_edges, canny_output, thresh, thresh*2, 3 );
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    t = ((double)getTickCount() - t)/getTickFrequency();
    fichier = fopen("/root/sharefolder/time.txt", "w");
    fprintf(fichier,"%lf",t);
    fclose(fichier);
    imwrite("algo_resultat.jpg", drawing);
    return 0;
}
