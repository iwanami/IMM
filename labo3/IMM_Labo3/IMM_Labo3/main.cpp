//
//  main.cpp
//  IMM_Labo3
//
//  Created by Numa Trezzini on 06.03.12.
//

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
#define IMG_PATH "/Users/numa/NUMA/Dossier Ecole/HEIG/Semestre 6/IMM/labos/labo2/IMM_Labo2/IMM_Labo1/MainCouleur.jpg"
#define TITRE_IMG "IMM_Labo3"
#define TITRE_HIST "histogramme"


int main (int argc, const char * argv[])
{
    //touche saisie par l'utilisateur
    char saisie;
    //image sur laquelle travailelr
    IplImage* img = cvLoadImage(IMG_PATH);
    if(!img)
        return EXIT_FAILURE;
    Mat img_mat(img);
    
    
    //separation des composantes couleurs de l'image de base
    vector<Mat> layers;
    split(img, layers);
    
    //set des parametres des histogrammes
    int histSize = 255;
    float range[] = {0, 255};
    const float* histRange = {range};
    const int zero = 0;
    Mat hist_red, hist_green, hist_blue, hist;
    
    //calcul des histogrammes en fonction d'une composante couleur en particulier
    //extraction de la composante rouge
    calcHist( &layers[2], 1, &zero, Mat(), hist_red, 1, &histSize, &histRange);
    Mat img_mat_red(layers[2]);
    
    //extraction de la composante verte
    calcHist( &layers[1], 1, &zero, Mat(), hist_green, 1, &histSize, &histRange);
    Mat img_mat_green(layers[1]);
    
    //extraction de la composante bleue
    calcHist( &layers[0], 1, &zero, Mat(), hist_blue, 1, &histSize, &histRange);
    Mat img_mat_blue(layers[0]);
    
    //initialisation de l'hisogramme de l'image de base
    calcHist( &img_mat, 1, &zero, Mat(), hist, 1, &histSize, &histRange);
    
    //parametres de l'image representant l'histogramme
    int hist_w = 600; int hist_h = 600;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    //initialisation des images representant les histogrammes
    Mat hist_r( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
    Mat hist_g( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
    Mat hist_b( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
    Mat hist_img( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
    
    //normalisation des histogrammes
    normalize(hist_red, hist_red, 0, hist_img.rows, NORM_MINMAX, -1, Mat() );
    normalize(hist_green, hist_green, 0, hist_img.rows, NORM_MINMAX, -1, Mat() );
    normalize(hist_blue, hist_blue, 0, hist_img.rows, NORM_MINMAX, -1, Mat() );
    
    //dessin des histogrammes dans les images 
    for( int i = 1; i < histSize; i++ )
    {
        //dessin dans l'histogramme rouge
        line( hist_r,
             Point( bin_w*(i-1), hist_h - cvRound(hist_red.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist_red.at<float>(i)) ),
             Scalar( 0, 0, 255), 2, 8, 0  );
        
        //dessin dans l'histogramme vert
        line( hist_g,
             Point( bin_w*(i-1), hist_h - cvRound(hist_green.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist_green.at<float>(i)) ),
             Scalar( 0, 255, 0), 2, 8, 0  );
        
        //dessin dans l'histogramme bleu
        line( hist_b,
             Point( bin_w*(i-1), hist_h - cvRound(hist_blue.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist_blue.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
        
        //dessin de l'histogramme dans l'image de base, pour les 3 couleurs
        line( hist_img, Point( bin_w*(i-1), hist_h - cvRound(hist_red.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist_red.at<float>(i)) ),
             Scalar( 0, 0, 255), 2, 8, 0  );
        line( hist_img, Point( bin_w*(i-1), hist_h - cvRound(hist_green.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist_green.at<float>(i)) ),
             Scalar( 0, 255, 0), 2, 8, 0  );
        line( hist_img, Point( bin_w*(i-1), hist_h - cvRound(hist_blue.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(hist_blue.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
    }
    
    //initialisation des fenetres
    namedWindow(TITRE_IMG, CV_WINDOW_AUTOSIZE);
    namedWindow(TITRE_HIST, CV_WINDOW_AUTOSIZE);
    
    //affichage de l'image de base et son histogramme
    imshow(TITRE_IMG, img_mat);
    imshow(TITRE_HIST, hist_img);
    printf("pour afficher les composantes de chaque couleur (RGB), appuyer sur R pour rouge, B pour bleu, G pour vert, N pour l'image originale ou toute autre touche pour quitter ");
    //boucle d'interaction avec l'utilisateur
    while (true) {
        saisie = waitKey();
        switch (saisie) {
            case 'r':
                imshow(TITRE_IMG, img_mat_red);
                imshow(TITRE_HIST, hist_r);
                break;
            case 'g':
                imshow(TITRE_IMG, img_mat_green);
                imshow(TITRE_HIST, hist_g);
                break;
            case 'b':
                imshow(TITRE_IMG, img_mat_blue);
                imshow(TITRE_HIST, hist_b);
                break;
            case 'n':
                imshow(TITRE_IMG, img_mat);
                imshow(TITRE_HIST, hist_img);
                break;
            default:
                return EXIT_SUCCESS;
        }
    }
    
}

