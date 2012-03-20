//
//  main.cpp
//  IMM_Labo1
//
//  Created by Numa Trezzini on 14.02.12.
//  Copyright (c) 2012 Numa Trezzini. All rights reserved.
//

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

int main (int argc, const char * argv[])
{
    string titre = "IMM Labo1";
    int isColored = 1;
    Mat grey;
    VideoCapture vc(0);
    if (!vc.isOpened()) {
        printf("prout");
        return -1;
    }
    namedWindow("video", CV_WINDOW_NORMAL);
    Mat frame;
    while(1){
        if(waitKey(1) == 'c')
            isColored = !isColored;
        if (isColored) {
            printf("affichage de l'image en couleur");
            vc >> frame;
            imshow("video", frame); 
        }
        else{
            printf("affichage de l'image en niveaux de gris");
            vc >> frame;
            cvtColor(frame, grey, CV_BGR2GRAY);
            imshow("video", grey);
        }
    }
    return 0;
}

