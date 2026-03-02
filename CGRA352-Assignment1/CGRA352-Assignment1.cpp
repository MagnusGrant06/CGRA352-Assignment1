// CGRA352-Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
    std::string img_path = "c:/Users/magnu/Downloads/green_turtle_talisman_talisman_elden_ring_wiki_guide_200px.png";
    cv::Mat mat = cv::imread(img_path);
    
    cv::imshow("Image", mat);
    cv::waitKey(0);

    return 0;
}
