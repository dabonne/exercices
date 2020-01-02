//
// Created by liris on 03/11/2019.
//
#include "SmallImage.h"
#include "opencv2/imgproc.hpp"
#include <math.h>

using namespace cv;
using namespace std;


//!
//! \param image
SmallImage::SmallImage(cv::Mat image): m_image(image), m_ratio(0) {
}

SmallImage::SmallImage() {
    cv::Mat image;
    m_image = image;
    m_ratio = 0;
}

//!
//! \param ratio
void SmallImage::setRatio() {
    int nz;
    nz = countNonZero(m_image);
    double taille(m_image.cols*m_image.rows);
    int zonePoly = abs(taille-nz);
    m_ratio = zonePoly/taille;
}
//!
//! \return
Mat SmallImage::getImage() const {
    return m_image;
}
double SmallImage::getRatio() const {
    return m_ratio;
}
//!

SmallImage::~SmallImage() {

}

