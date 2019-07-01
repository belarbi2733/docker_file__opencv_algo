#!/bin/bash

target_file1="/root/sharefolder/sift.jpg"
target_file2="/root/sharefolder/surf.jpg"
target_file3="/root/sharefolder/color-histo.jpg"
target_file4="/root/sharefolder/hsv-histo.jpg"
target_file5="/root/sharefolder/blur.jpg"
target_file6="/root/sharefolder/median.jpg"
target_file7="/root/sharefolder/gaussian.jpg"
target_file8="/root/sharefolder/bilateral.jpg"
target_file9="/root/sharefolder/harris.jpg"
target_file10="/root/sharefolder/canny.jpg"
target_file11="/root/sharefolder/contours.jpg"
target_file12="/root/sharefolder/laplacien.jpg"
target_file13="/root/sharefolder/water.jpg"
target_file14="/root/sharefolder/mmr.jpg"
target_file15="/root/sharefolder/yolo.jpg"
target_file16="/root/sharefolder/nightmare.jpg"
resultat1="algo_resultat.jpg"
resultat2="/root/sharefolder/algo_resultat.jpg"
resultat3="changement.jpg"
resultat4="/root/sharefolder/changement.jpg"
resultat5="predictions.jpg"
resultat6="nightmare_vgg-conv_2_000000.jpg"


if [ -f $target_file1 ];
then
  /root/sift $target_file1 
  rm $target_file1;
  echo "sift";
fi

if [ -f $target_file2 ];
then
  /root/surf $target_file2 
  rm $target_file2;
  echo "surf";
fi

if [ -f $target_file3 ];
then
  /root/histo_cou $target_file3 
  rm $target_file3;
  echo "histo_cou";
fi

if [ -f $target_file4 ];
then
  /root/histo_hsv $target_file4 
  rm $target_file4;
  echo "histo_hsv";
fi

if [ -f $target_file5 ];
then
  /root/blur $target_file5 
  rm $target_file5;
  echo "blur";
fi

if [ -f $target_file6 ];
then
  /root/median $target_file6 
  rm $target_file6;
  echo "median";
fi

if [ -f $target_file7 ];
then
  /root/gaussian $target_file7 
  rm $target_file7;
  echo "gaussian";
fi

if [ -f $target_file8 ];
then
  /root/bilateral $target_file8 
  rm $target_file8;
  echo "bilateral";
fi

if [ -f $target_file9 ];
then
  /root/harris $target_file9 
  rm $target_file9;
  echo "harris";
fi

if [ -f $target_file10 ];
then
  /root/canny $target_file10 
  rm $target_file10;
  echo "canny";
fi

if [ -f $target_file11 ];
then
  /root/countours $target_file11 
  rm $target_file11;
  echo "countours";
fi

if [ -f $target_file12 ];
then
  /root/laplacien $target_file12 
  rm $target_file12;
  echo "laplacien";
fi

if [ -f $target_file13 ];
then
  /root/water $target_file13 
  rm $target_file13;
  echo "water";
fi

if [ -f $target_file14 ];
then
  /root/recherche $target_file14 
  rm $target_file14;
  echo "indexation";
fi

if [ -f $target_file15 ];
then
	cd /root/darknet
  ./darknet detector test cfg/coco.data cfg/yolo.cfg yolo.weights $target_file15 
  rm $target_file15;
  mv  $resultat5 $resultat2
  echo "yolo";
fi

if [ -f $target_file16 ];
then
        cd /root/darknet
  ./darknet nightmare cfg/vgg-conv.cfg vgg-conv.weights $target_file16 2 -iters 2 
  rm $target_file16;
  mv  $resultat6 $resultat2
  echo "nightmare";
fi


mv  $resultat1 $resultat2
mv  $resultat3 $resultat4
