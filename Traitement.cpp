#include <opencv2/core/mat.hpp>
//
// Created by liris on 30/10/2019.
//
#include "Traitement.h"
#include "SmallImage.h"
using namespace std;

///
/// \param imageTrans est l'image à transformer
/// \return nous rétournons le résultat de la transformation de l'image
void arrierePlanBlanc(Mat &imageTrans){

    for ( int i = 0; i < imageTrans.rows; i++ ) {
        for ( int j = 0; j < imageTrans.cols; j++ ) {
            if ( imageTrans.at<Vec3b>(i, j) == Vec3b(0,0,0) )
            {
                imageTrans.at<Vec3b>(i, j)[0] = 255;
                imageTrans.at<Vec3b>(i, j)[1] = 255;
                imageTrans.at<Vec3b>(i, j)[2] = 255;
            }
        }
    }
    imwrite("./Image1background.png", imageTrans);
}
//!
//! \param img
//! \return
vector <SmallImage>imageCrop(Mat &img){
    //int colones(img.cols),lignes(img.rows);
    Mat imR;
    resize(img, imR, Size(21568, 21632));
    vector <SmallImage> tableImg;
    const unsigned int height(64), width(64);
    for(int x(0); x+width <= imR.cols; x += width)
    {
        for(int y(0); y+height <= imR.rows; y += height)
        {
            Rect imgRect = Rect(x, y, width, height);
            if(0 <= imgRect.x && 0 <= imgRect.width && imgRect.x + imgRect.width < img.cols &&
               0 <= imgRect.y && 0 <= imgRect.height && imgRect.y + imgRect.height < img.rows)
            {

                Mat new_image = img(imgRect);
                SmallImage small_image(new_image);
                small_image.rect_from_bigimage = imgRect;
                tableImg.push_back(small_image);
                cout << x << "xx" << y << endl ;
            }
        }
    }
    imwrite("./imResize.png", imR);
    return tableImg;
}

vector <SmallImage> patchClassification(vector <SmallImage> &tabImg)
{
    SmallImage tmp;
    int des(1);
    while(des) {
        des = 0;// On suppose que le tableau est ordonné
        for (int i(0); i< tabImg.size()-1; i++) {
            if (tabImg.at(i).SmallImage::getRatio() > tabImg.at(i+1).SmallImage::getRatio()) {
                tmp = tabImg.at(i+1);
                tabImg.at(i+1) = tabImg.at(i);
                tabImg.at(i) = tmp;
                // On constate qu'il ne l'est pas
                des = 1;
            }
        }
    }
    return tabImg;
}

void WriteOnDisk(vector <SmallImage> &tableImag)
{
    for (int i(0); i < tableImag.size(); i++) {
        imwrite("./recopieDecoupage/img"+to_string(i)+".png",tableImag.at(i).SmallImage::getImage());
    }
}

//Reconstitution de l'Image


void imageReconstruction(vector <SmallImage> &tableImg, Mat &img){
    cv::Size output_size = Size(21568, 21632);

    Mat contours_reconstructed = Mat(output_size, CV_64F);
    Mat carte_chaleur = Mat(output_size, CV_64F);

    if(tableImg.empty())
        cout << "Pas de vecteur d'image";

    for(SmallImage element: tableImg)
    {
        // if elem.rect....
        // Reconstruction
        element.getImage().copyTo(contours_reconstructed(element.rect_from_bigimage));
        // Ratio to mat mapping
        cv::Mat ones(element.getImage().size(), CV_64FC1, Scalar(1));
        ones *= element.getRatio();
        ones.copyTo(carte_chaleur(element.rect_from_bigimage));
    }
    // Save contours
    imwrite("./carteC.png", contours_reconstructed);

    // Normalize carte_chaleur and save it
    cv::normalize(carte_chaleur, carte_chaleur, 255, 0, NORM_MINMAX);
    carte_chaleur.convertTo(carte_chaleur, CV_8UC1);
    imwrite("./carte_chaleur.png",carte_chaleur);

    // Apply color map on carte chaleur and save it
    Mat carte_chaleur_color = Mat(output_size, CV_8UC3);
    cv::applyColorMap (carte_chaleur, carte_chaleur_color ,  COLORMAP_HSV );
    imwrite("./carte_chaleur_colorhsv.png", carte_chaleur_color);

}

cv::Mat imagePreprocessing(Mat &image){
    if(image.empty())
    {
        std::cout << "Pas d'image disponible";
    }
    double thresh = 55;
    double maxValue = 255;
    cv::cvtColor(image, image, COLOR_BGR2GRAY);
    threshold(image, image, thresh, maxValue, THRESH_BINARY);
    return image;
}

void updateRatio(std::vector <SmallImage> &tableImages){
    if(tableImages.empty())
    {
        cout << "Aucune Image";
    }
    for(SmallImage element: tableImages)
    {
        element.setRatio();
    }
}