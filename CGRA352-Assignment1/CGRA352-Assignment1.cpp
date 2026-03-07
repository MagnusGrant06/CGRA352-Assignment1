// CGRA352-Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

void part1() {

    std::string img_path = "Flower.jpg";
    cv::Mat BGR_flower = cv::imread(img_path);

    cv::Mat HSV_flower = cv::imread(img_path);

    cv::cvtColor(BGR_flower, HSV_flower, cv::COLOR_BGR2HSV);

    cv::Mat combined;
    cv::hconcat(BGR_flower, HSV_flower, combined);
    cv::imshow("BGR and HSV", combined);

    cv::waitKey(0);
    cv::destroyAllWindows();
    //initialize grayscale images
    cv::Mat BGR_B = cv::imread(img_path);
    cv::cvtColor(BGR_flower, BGR_B, cv::COLOR_BGR2GRAY);
    
    cv::Mat BGR_G = BGR_B.clone();
    cv::Mat BGR_R = BGR_B.clone();

    cv::Mat HSV_H = BGR_B.clone();
    cv::Mat HSV_S = BGR_B.clone();
    cv::Mat HSV_V = BGR_B.clone();

    for (int i = 0; i < BGR_flower.rows; i++) {
        for (int j = 0; j < BGR_flower.cols; j++) {
            cv::Vec3b current_BGR_pixel = BGR_flower.at<cv::Vec3b>(i, j);
            BGR_B.at<uchar>(i, j) = current_BGR_pixel.val[0];
            BGR_G.at<uchar>(i, j) = current_BGR_pixel.val[1];
            BGR_R.at<uchar>(i, j) = current_BGR_pixel.val[2];

            cv::Vec3b current_HSV_pixel = HSV_flower.at<cv::Vec3b>(i, j);
            HSV_H.at<uchar>(i, j) = current_HSV_pixel.val[0];
            HSV_S.at<uchar>(i, j) = current_HSV_pixel.val[1];
            HSV_V.at<uchar>(i, j) = current_HSV_pixel.val[2];
        }
    }

    cv::Mat HSV_row ,BGR_row, final_combined;
    cv::hconcat(BGR_B, BGR_G, BGR_row);
    cv::hconcat( BGR_row, BGR_R, BGR_row);

    cv::hconcat(HSV_H, HSV_S, HSV_row);
    cv::hconcat(HSV_row, HSV_V, HSV_row);

    cv::vconcat(BGR_row, HSV_row, final_combined);
    cv::imshow("Part 1",final_combined);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

void part2() {

    std::string img_path = "Flower.jpg";
    cv::Mat BGR_flower = cv::imread(img_path);

    cv::Mat HSV_flower = cv::imread(img_path);

    cv::cvtColor(BGR_flower, HSV_flower, cv::COLOR_BGR2HSV);


    cv::Mat combined;
    for (float intensity = 0.0; intensity < 1; intensity += 0.2) {
        cv::Mat vert_combined;
        for (int i = 0; i < 3; i++) {

            cv::Mat temp_channels[3];
            cv::split(HSV_flower, temp_channels);

            temp_channels[i] = temp_channels[i] * intensity;

            cv::Mat result;
            cv::merge(temp_channels, 3, result);

            cv::Mat result_BGR;
            cv::cvtColor(result, result_BGR, cv::COLOR_HSV2BGR);


            if (vert_combined.empty()) {
                vert_combined = result_BGR.clone();
            }
            else {
                cv::vconcat(vert_combined, result_BGR, vert_combined);
            }
        }

        if (combined.empty()) {
            combined = vert_combined.clone();
        }
        else {
            cv::hconcat(combined, vert_combined, combined);
        }

    }

    cv::imshow("test", combined);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

void part3() {

    std::string img_path = "Flower.jpg";
    cv::Mat BGR_flower = cv::imread(img_path);
    
    cv::Vec3b base_pixel = BGR_flower.at<cv::Vec3b>(80, 80);

    for (int i = 0; i < BGR_flower.rows; i++) {
        for (int j = 0; j < BGR_flower.cols; j++) {
            cv::Vec3b& target_pixel = BGR_flower.at<cv::Vec3b>(i, j);

            if (cv::norm(base_pixel, target_pixel) < 100.0) {
                target_pixel = cv::Vec3b(255, 255, 255);
            }
            else {
                target_pixel = cv::Vec3b(0, 0, 0);
            }
        }
    }

    cv::imshow("test",BGR_flower);

    cv::waitKey(0);
    cv::destroyAllWindows();
    
}


//completion

void completion() {
    std::string img_path = "Flower.jpg";
    cv::Mat BGR_flower = cv::imread(img_path);
    cv::Mat gray_flower = cv::imread(img_path);

    cv::cvtColor(BGR_flower, gray_flower, cv::COLOR_BGR2GRAY);

    cv::Mat laplacian_img = gray_flower.clone();
    cv::Mat sobel_x_img = gray_flower.clone();
    cv::Mat sobel_y_img = gray_flower.clone();

    for (int i = 1; i < gray_flower.rows - 1; i++) {             //start 1 down and 1 across as to not go out of bounds
        for (int j = 1; j < gray_flower.cols - 1; j++) {         //same thing for stopping 1 up and 1 across

            //Laplacian edge detection using the matrix
            //     0  1 0
            //     1 -4 1
            //     0  1 0

            int laplacian_sum = (int)(gray_flower.at<uchar>(i, j)*-4) + (int)gray_flower.at<uchar>(i + 1, j) +
                (int)gray_flower.at<uchar>(i - 1, j) + (int)gray_flower.at<uchar>(i, j + 1) +
                (int)gray_flower.at<uchar>(i, j - 1);
            //used 30 as the threshold as it provided good, balanced results
            if (laplacian_sum <= -30) {
                laplacian_img.at<uchar>(i, j) = 100;
            }
            else if (laplacian_sum >= 30) {
                laplacian_img.at<uchar>(i, j) = 154;
            }
            else {
                laplacian_img.at<uchar>(i, j) = 127;
            }

            //Sobel x-direction edge detection using the matrix
            //     -1 0 1
            //     -2 0 2
            //     -1 0 1

            int sobel_x_sum = (int)(gray_flower.at<uchar>(i - 1, j - 1) * -1) + (int)(gray_flower.at<uchar>(i, j - 1) * -2) +
                (int)(gray_flower.at<uchar>(i + 1, j - 1) * -1) + (int)(gray_flower.at<uchar>(i - 1, j + 1)) +
                    (int)(gray_flower.at<uchar>(i, j+1) * 2) + (int)(gray_flower.at<uchar>(i + 1, j + 1));

            if (sobel_x_sum <= -40) {
                sobel_x_img.at<uchar>(i, j) = 100;
            }
            else if (sobel_x_sum >= 40) {
                sobel_x_img.at<uchar>(i, j) = 154;
            }
            else {
                sobel_x_img.at<uchar>(i, j) = 127;
            }

            //Sobel y-direction edge detection using the matrix
            //     -1 -2 -1
            //      0  0  0
            //      1  2  1

            int sobel_y_sum = (int)(gray_flower.at<uchar>(i - 1, j - 1) * -1) + (int)(gray_flower.at<uchar>(i - 1, j) * -2) +
                (int)(gray_flower.at<uchar>(i - 1, j + 1) * -1) + (int)(gray_flower.at<uchar>(i + 1, j - 1)) +
                    (int)(gray_flower.at<uchar>(i + 1, j ) * 2) + (int)(gray_flower.at<uchar>(i + 1, j + 1));

            if (sobel_y_sum <= -40) {
                sobel_y_img.at<uchar>(i, j) = 114;
            }
            else if (sobel_y_sum >= 40) {
                sobel_y_img.at<uchar>(i, j) = 140;
            }
            else {
                sobel_y_img.at<uchar>(i, j) = 127;
            }
        }
    }

    cv::imshow("Laplacian", laplacian_img);
    cv::imshow("Sobel X", sobel_x_img);
    cv::imshow("Sobel Y", sobel_y_img);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main()
{
    part1();
    part2();
    part3();
    completion();

    return 0;
}