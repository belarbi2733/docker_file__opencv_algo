#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;


bool compare_response(cv::KeyPoint first, cv::KeyPoint second)
{
	if (first.response > second.response) return true;
	else return false;
}

int main(int argc, char *argv[])
{
	FILE* fichier1 = NULL;
	FILE* fichier2 = NULL;
	DIR* rep;
	string name;
	const char* filename1,*filename2;
	cv::Mat img;
	cv::SiftFeatureDetector detector1;
	int minHessian = 400;
	cv::SurfFeatureDetector detector2(minHessian);
	cv::SiftDescriptorExtractor extractor1;
	cv::SurfDescriptorExtractor extractor2;
	cv::Mat descriptors;
	cv::vector<cv::KeyPoint> keypoints_1;
	vector<cv::KeyPoint> keypoints_2;
	struct dirent* lecture = NULL;
	int j=1;
	char extension[5]=".jpg";
	//strcpy(extension,argv[3]);
	cout << argv[1] << endl;
	if(!strcmp(argv[1],"SIFT"))
	{
		mkdir ("/root/SIFT",S_IRWXU);
		rep=opendir("/root/wang");
		cout << "entrer" <<endl;
		while (lecture = readdir(rep))
		{
			cout << " nom de l'image : " << lecture->d_name << endl;
			if(strstr(lecture->d_name, extension) != NULL)
			{
				string h=lecture->d_name;
				string ext="/root/SIFT/"+h.substr(0,h.find("."));
				string name=ext+".xml";
				filename2=name.c_str();
				string  ab="/root/wang/";
				string  ac= lecture->d_name;
				string a=ab+ac;
				img = cv::imread(a.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
				detector1.detect(img, keypoints_1);
				sort(keypoints_1.begin(), keypoints_1.end(), compare_response);
				for (int i = 0; i < (keypoints_1.size())/4; i++)
				{
					keypoints_2.push_back(keypoints_1[i]);
				}
				extractor1.compute(img, keypoints_2, descriptors);
				cv::FileStorage fs(filename2, cv::FileStorage::WRITE);
				cout << " nom de l'index : " << filename2 << endl;
				fs << "INDEX" << descriptors;
				fs.release();
				img.release();
				descriptors.release();
				keypoints_1.clear();
				keypoints_2.clear();
				printf(" compteur :%d \n",j++);
			}
		}
		printf("\n---------------------------------------------------------------------------\n");
		closedir(rep);
	}
	else if(!strcmp(argv[1],"SURF"))
	{
		mkdir ("/root/SURF",S_IRWXU);
		rep=opendir("/root/wang");
		j=1;
		cout << "entrer" <<endl;
		while (lecture = readdir(rep))
		{
			cout << " nom de l'image : " << lecture->d_name << endl;
			if(strstr(lecture->d_name, ".jpg") != NULL)
            {
				string h=lecture->d_name;
				string ext="/root/SURF/"+h.substr(0,h.find("."));
				string name=ext+".xml";
				filename2=name.c_str();
				string  ab="/root/wang/";
				string  ac= lecture->d_name;
				string a=ab+ac;
				img = cv::imread(a.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
				detector2.detect(img, keypoints_1);
				sort(keypoints_1.begin(), keypoints_1.end(), compare_response);
				for (int i = 0; i <(keypoints_1.size()) / 4; i++)
				{
					keypoints_2.push_back(keypoints_1[i]);
				}
				extractor2.compute(img, keypoints_2, descriptors);
				cv::FileStorage fs(filename2, cv::FileStorage::WRITE);
				cout << " nom de l'index : " << filename2 << endl;
				fs << "INDEX" << descriptors;
				fs.release();
				img.release();
				descriptors.release();
				keypoints_1.clear();
				keypoints_2.clear();
				printf("compteur :%d \n",j++);
			}
		}
		printf("\n---------------------------------------------------------------------------\n");
		closedir(rep);
	}
	else if(!strcmp(argv[1],"HISTO"))
	{
		mkdir ("/root/HISTO",S_IRWXU);
		rep=opendir("/root/wang");
		j=1;
		cout << "entrer" <<endl;
		while (lecture = readdir(rep))
		{
			cout << " nom de l'image : " << lecture->d_name << endl;
			if(strstr(lecture->d_name, ".jpg") != NULL)
            {
				string h=lecture->d_name;
				string ext="/root/HISTO/"+h.substr(0,h.find("."));
				string name=ext+".xml";
				filename2=name.c_str();
				string  ab="/root/wang/";
				string  ac= lecture->d_name;
				string a=ab+ac;
				img = cv::imread(a.c_str(), 1);
				vector<Mat> bgr_planes;
				split( img, bgr_planes );
				int histSize = 256;
				float range[] = { 0, 256 } ;
				const float* histRange = { range };
				int channels[] = { 0, 1 };
				Mat b_hist, g_hist, r_hist;
				calcHist( &bgr_planes[0], 1, channels, Mat(), b_hist, 1,&histSize, &histRange, true, false );
				calcHist( &bgr_planes[1], 1, channels, Mat(), g_hist, 1,&histSize, &histRange, true, false );
				calcHist( &bgr_planes[2], 1, channels, Mat(), r_hist, 1,&histSize, &histRange, true, false );
				MatND  hist_base,la;
				hconcat(b_hist,g_hist,la);
				hconcat(la,r_hist,hist_base);
				cv::FileStorage fs(filename2, cv::FileStorage::WRITE);
				cout << " nom de l'index : " << filename2 << endl;
				fs << "INDEX" << hist_base;
				fs.release();
				img.release();
				printf("compteur :%d \n",j++);
			}
		}
		printf("\n---------------------------------------------------------------------------\n");
		closedir(rep);
	}
		return 0;
}
