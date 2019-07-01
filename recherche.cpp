#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
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
void tricroissant( double tab1[],string tab2[], int tab_size)
{
  int i=0;
  double tmp1=0;
  string tmp2;
  int j=0;

  for(i = 0; i < tab_size; i++)          //On veut remplir la case i du tableau
    {
      for(j = i+1; j < tab_size; j++)    //On vérifie s'il n'y a pas de nombre inférieur
        {                                //Dans les cases suivantes
          if(tab1[j] < tab1[i])
            {
              tmp1 = tab1[i];
              tmp2=tab2[i];
              tab1[i] = tab1[j];
              tab2[i] = tab2[j];
              tab1[j] = tmp1;
              tab2[j] = tmp2;
            }
        }
    }
}

int main(int argc, char *argv[])
{
	Mat img;
    cv::SiftFeatureDetector detector1;
    DIR* rep_image=opendir("/root/wang");
    DIR* rep_index;
    string img_req=argv[1];
    cv::SiftDescriptorExtractor extractor1;
    cv::Mat descriptors1,descriptors2;
    cv::vector<cv::KeyPoint> keypoints_1,keypoints_2;
    struct dirent* lecture = NULL;
    double distances_dis[1000];
    string imgs[1000];
    int compteur=0;
	FILE* fichier = NULL;
	int algo,dist_algo;
	string current_index;
	fichier = fopen("/root/sharefolder/algo.txt", "r");
	fscanf(fichier,"%d",&algo);
	fclose(fichier);
	fichier = fopen("/root/sharefolder/distance.txt", "r");
	fscanf(fichier,"%d",&dist_algo);
	fclose(fichier);
	if (algo==1)
	{
		rep_index=opendir("/root/SIFT");
		current_index="/root/SIFT/";
	}
	else if (algo==2)
	{
		rep_index=opendir("/root/SURF");
		current_index="/root/SURF/";
	}
	else
	{
		rep_index=opendir("/root/HISTO");
		current_index="/root/HISTO/";
	}
    if(algo==1 || algo==2)
    {
        img = cv::imread(img_req.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        detector1.detect(img, keypoints_1);
        sort(keypoints_1.begin(), keypoints_1.end(), compare_response);
        for (int i = 0; i < (keypoints_1.size())/4; i++)
        {
            keypoints_2.push_back(keypoints_1[i]);
        }
        extractor1.compute(img, keypoints_2, descriptors2);
        while (lecture = readdir(rep_index))
		{
			if(strstr(lecture->d_name, ".xml") != NULL)
			{
				string file_index=current_index+lecture->d_name;
				string name_file_index=lecture->d_name;
				cout << file_index << endl;
				cv::FileStorage file1(file_index.c_str(), cv::FileStorage::READ);
				file1["INDEX"] >> descriptors1;				
				vector< DMatch > matches;
				FlannBasedMatcher matcher1;
				//matcher1.match(descriptors1, descriptors2, matches);
				BFMatcher matcher2(NORM_L2);
				if(dist_algo==1)
				{
					matcher1.match(descriptors1, descriptors2, matches);
				}
				else 
				{	
					matcher2.match(descriptors1, descriptors2, matches);
				}
					
				double max_dist = 0.0;
				for (int i = 0; i < matches.size(); i++)
				{
					double dist = matches[i].distance;
					if (dist > max_dist) max_dist = dist;
				}
				if (max_dist != 0.0)
				{
					for (int i = 0; i < matches.size(); i++)
					{
						matches[i].distance = (matches[i].distance) / max_dist;
					}
				}
				double moy = 0.0;
				for (int i = 0; i < matches.size(); i++)
				{
					moy += matches[i].distance;
				}
				moy /= matches.size();
				std::vector< DMatch > good_matches;
				for (int i = 0; i < matches.size(); i++)
				{
					if (matches[i].distance <= moy)
					{
						good_matches.push_back(matches[i]);
					}
				}
				double good_moy = 0.0;
				for (int i = 0; i < good_matches.size(); i++)
				{
					good_moy += good_matches[i].distance;
				}
				good_moy /= good_matches.size();
				distances_dis[compteur]=good_moy;
				imgs[compteur++]=name_file_index.substr(0,name_file_index.find(".")); 
				cout << "Distance :" << good_moy << endl;
				cout << "Nom image :" << name_file_index.substr(0,name_file_index.find(".")) << endl;
			}        
		}
	}
	else
	{
		img = cv::imread(img_req.c_str(), 1);
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
		double dist_histo;
		while (lecture = readdir(rep_index))
		{
			if(strstr(lecture->d_name, ".xml") != NULL)
			{
				string file_index=current_index+lecture->d_name;
				string name_file_index=lecture->d_name;
				cout << file_index << endl;
				cv::FileStorage file1(file_index.c_str(), cv::FileStorage::READ);
				file1["INDEX"] >> descriptors1;	
				if(dist_algo==1)
				{
					dist_histo=1-compareHist(hist_base,descriptors1,CV_COMP_CORREL);
				}
				else if(dist_algo==2)
				{
					dist_histo=compareHist(hist_base,descriptors1,CV_COMP_CHISQR);
				}
				else if(dist_algo==3)
				{
					dist_histo=compareHist(hist_base,descriptors1,CV_COMP_INTERSECT);
				}
				else if(dist_algo==4)
				{
					dist_histo=compareHist(hist_base,descriptors1,CV_COMP_BHATTACHARYYA);
				}
				else if(dist_algo==5)
				{
					dist_histo=compareHist(hist_base,descriptors1,CV_COMP_HELLINGER);
				}
				distances_dis[compteur]=dist_histo;
				imgs[compteur++]=name_file_index.substr(0,name_file_index.find("."));
				
			}
		}
	}
        compteur--;
        tricroissant(distances_dis,imgs,compteur);
        string test1="/root/wang/";
	    string ext=".jpg";
		mkdir ("/root/sharefolder/Similaire",S_IRWXU);
		FILE* fichier_dist = NULL;
		fichier_dist=fopen("/root/sharefolder/taux.txt", "w");
		int distance_image;
        for(int j=0;j<20;j++)
        {
			distance_image=distances_dis[j]*100;
			fprintf(fichier_dist,"%d\n",distance_image);
			string filefrom=test1+imgs[j]+ext;
			Mat image1=cv::imread(filefrom.c_str(),1);
			std::stringstream sstm;
		    sstm << "/root/sharefolder/Similaire/image" << j<<ext;
			string fileto=sstm.str();
			imwrite( fileto.c_str(), image1);
			cout << "-------------------------------------------------------------" << endl;
			cout << "depuis fichier : " <<  filefrom << endl;
			cout << "vers fichier : " << fileto << endl;
        }
		fclose(fichier_dist);
		system("chown www-data:www-data -R /root/sharefolder/Similaire");  
}
