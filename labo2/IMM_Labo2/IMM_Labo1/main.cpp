/*
 * Nom: IMM Labo1
 * Auteur: Numa Trezzini
 * Date: 14.02.2012
 * But: premiere application avec openCV. L'application recupère l'image de la webcam
 *      et l'affiche. l'utilisateur peut changer l'affichage de l'image entre couleur
 *      et niveaux de gris
 * Fonctionnement: lance le thread pthSaisie: attend que l'utilisateur appuie sur une touche du clavier
 *                        si la touche est 'c', la couleur de l'affichage change, sinon
 *                        le programme est interrompu. corps du thread dans la methode "saisie"
 *						  la recuperation du flux de la camera se fait dans la fonction main
 * Remarques: Il semblerait qu'il ne soit pas possible d'utiliser des fonctions OpenCV dans un thread
 *			  autre que celui ou la fenetre a ete creee (celle ouverte par "namedWindow"). Ainsi, il
 *		 	  n'est pas possible d'utiliser "waitKey" dans la tache de saisie.
 */

#include <iostream>
#include <pthread.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

//code ASCII de la touche 'esc'
#define ESC 0x1B

#define IMG_PATH "/Users/numa/NUMA/Dossier Ecole/HEIG/Semestre 6/IMM/labos/labo2/IMM_Labo2/IMM_Labo1/MainCouleur.jpg"

//titre de la fenetre
char* titre = "IMM Labo2";

//flag indiquant si l'image doit etre en couleur
int isColored = 1;

//flag de fin de programme
int isOver = 0;

//thread de saisie
pthread_t pthSaisie;

/*
 * Nom: saisie
 * But: attend une saisie clavier de l'utilisateur. Si la touche pressee est 'c',
 *      l'affichage est modifie, sinon, le thread se termine
 */
void* saisie(void* arg){
    char i;
    //on attend une saisie de l'utilisateur
    printf("presser c pour changer le mode d'affichage ou sur esc pour quitter: ");
    //si l'utilisateur a presse 'c'
    while (1)
    {
        
        scanf("%c", &i);
		//absorbe le retour a la ligne
        getchar();
        
        //la couleur est changee
        if (i=='c') {
            isColored = !isColored;
            cout << "affichage en " << (isColored?"couleur":"niveaux de gris") << endl;
        }
        //on quitte quand l'utilisateur appuie sur 'esc'
        else if(i == ESC)
            break;
        
    }
    //indique la fin du programme a l'autre thread
    isOver = 1;
    return NULL;
}

/*
 * Nom: onMouseClick
 * But: Fonction de callback de la souris. lorsque le bouton gauche est presse (LBUTTONDOWN),
 *      la position courante de la souris est affichee, ainsi que les composantes RGB du pixel
 * Remarques: les axes d'une image openCV sont disposes ainsi:
 * 0-----> x
 * |
 * |
 * v
 * y
 * la valeur des lignes est donc donnee par y et la valeur des colonnes par x
 */
void onMouseClick(int event, int x, int y, int flags, void* param){
    if(event == CV_EVENT_LBUTTONDOWN){
        IplImage* img = (IplImage*) param;
        CvScalar sc = cvGet2D(img, y, x);
        cout << "coordonnees (ligne; colonne): (" << y << "; " << x << "), couleur (R; G; B): (" << sc.val[0] << "; " << sc.val[1] << "; " << sc.val[2] << ")" << endl;
    }
}

/*
 * Nom: main
 * But: lance les threads d'affichage et de saisie puis attend leurs terminaisons
 */
int main (int argc, const char * argv[])
{
    printf("afficher la webcam ou interagir avec l'image [0 -> webcam; 1 -> image]? ");
    int choix;
    scanf("%d", &choix);
    //ouverture de la fenetre (taille de l'image affichee par defaut)
    namedWindow(titre, CV_WINDOW_AUTOSIZE);
    //l'utilisateur choisit l'image
    if(choix){
        printf("pour quitter, presser une touche du clavier");
        //chargement de l'image permettant l'acces aux pixels
        IplImage* img = cvLoadImage(IMG_PATH);
        //creation de l'image permettant l'affichage
        Mat imageAffiche = Mat(img);
        //initialisation du controlleur de souris
        cvSetMouseCallback(titre, onMouseClick, (void*)img);
        //affichage de l'image
        imshow(titre, imageAffiche);
        //attente de la terminaison par l'utilisateur
        waitKey();
    }
    //l'utilisateur choisit la webcam
    else{
        //initialisation du thread de saisie
        if(pthread_create(&pthSaisie , NULL, saisie, NULL))
            return EXIT_FAILURE;
        //connexion a la webcam
        VideoCapture vc(0);
        //verification de la connexion
        if (!vc.isOpened()) {
            printf("webcam non connectee");
            return EXIT_FAILURE;
        }
        //Mat contenant l'image de la webcam
        Mat frame;
        //Mat contenant l'image en niveaux de gris
        Mat grey;
        
        //boucle d'affichage
        while(!isOver){
            //recuperation de l'image de la webcam
            vc >> frame;
            //si l'image est en couleurs
            if (isColored) {
                //on affiche l'image recuperee
                //printf("affichage de l'image en couleur");
                imshow(titre, frame); 
            }
            else{
                //sinon, l'image doit etre mise en niveaux de gris
                //printf("affichage de l'image en niveaux de gris");
                //transformation
                cvtColor(frame, grey, CV_BGR2GRAY);
                //affichage
                imshow(titre, grey);
            }
        }
    }
    
    return EXIT_SUCCESS;
}

