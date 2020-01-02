//
// Created by liris on 03/11/2019.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
#ifndef SEGMENTATIONPHOTOMETRIQUE_SMALLIMAGE_H
#define SEGMENTATIONPHOTOMETRIQUE_SMALLIMAGE_H


class SmallImage {
    public:
        SmallImage(Mat image);
        SmallImage();
        void setImage(Mat img);
        void setRatio();
        Mat getImage() const ;
        double getRatio() const ;
        ~SmallImage();
        cv::Rect rect_from_bigimage;

    private:
        Mat m_image;
        double m_ratio = 0;

};


#endif //SEGMENTATIONPHOTOMETRIQUE_SMALLIMAGE_H
