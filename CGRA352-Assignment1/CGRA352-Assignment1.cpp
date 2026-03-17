// CGRA352-Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

void part1() {

    //initial testing for loading, displaying and concatenating images
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
            //change each grayscale images pixel to corresponding B, G, or R value
            BGR_B.at<uchar>(i, j) = current_BGR_pixel.val[0];
            BGR_G.at<uchar>(i, j) = current_BGR_pixel.val[1];
            BGR_R.at<uchar>(i, j) = current_BGR_pixel.val[2];

            //same thing but for HSV
            cv::Vec3b current_HSV_pixel = HSV_flower.at<cv::Vec3b>(i, j);
            HSV_H.at<uchar>(i, j) = current_HSV_pixel.val[0];
            HSV_S.at<uchar>(i, j) = current_HSV_pixel.val[1];
            HSV_V.at<uchar>(i, j) = current_HSV_pixel.val[2];
        }
    }

    //combine all different grayscale images
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
    for (float intensity = 0.0; intensity < 1; intensity += 0.2) {       //loop through different intensities to apply
        cv::Mat vert_combined;
        for (int i = 0; i < 3; i++) {

            //split the three H,S,V channels into an array
            cv::Mat temp_channels[3];
            cv::split(HSV_flower, temp_channels);

            //multiply current channel by inetnsity
            temp_channels[i] = temp_channels[i] * intensity;

            //merge changed channel with unchanged channels
            cv::Mat result;
            cv::merge(temp_channels, 3, result);

            //convert back to BGR for displaying
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

    cv::imshow("Part 2", combined);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

void part3() {

    std::string img_path = "Flower.jpg";
    cv::Mat BGR_flower = cv::imread(img_path);
    
    //initialize base point
    cv::Vec3b base_pixel = BGR_flower.at<cv::Vec3b>(80, 80);

    for (int i = 0; i < BGR_flower.rows; i++) {
        for (int j = 0; j < BGR_flower.cols; j++) {
            cv::Vec3b& target_pixel = BGR_flower.at<cv::Vec3b>(i, j);

            //use cv::norm to calculate distance then change to white if below distance and black if above
            if (cv::norm(base_pixel, target_pixel) < 100.0) {
                target_pixel = cv::Vec3b(255, 255, 255);
            }
            else {
                target_pixel = cv::Vec3b(0, 0, 0);
            }
        }
    }

    cv::imshow("Part 3",BGR_flower);

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

            int laplacian_output = 127 + laplacian_sum*0.5;
            laplacian_output = std::max(0, std::min(255, laplacian_output));
  
            laplacian_img.at<uchar>(i, j) = (uchar)laplacian_output;

            //Sobel x-direction edge detection using the matrix
            //     -1 0 1
            //     -2 0 2
            //     -1 0 1

            int sobel_x_sum = (int)(gray_flower.at<uchar>(i - 1, j - 1) * -1) + (int)(gray_flower.at<uchar>(i, j - 1) * -2) +
                (int)(gray_flower.at<uchar>(i + 1, j - 1) * -1) + (int)(gray_flower.at<uchar>(i - 1, j + 1)) +
                    (int)(gray_flower.at<uchar>(i, j+1) * 2) + (int)(gray_flower.at<uchar>(i + 1, j + 1));

            int sobel_x_output = 127 + sobel_x_sum*0.3;
            sobel_x_output = std::max(0, std::min(255, sobel_x_output));
            sobel_x_img.at<uchar>(i, j) = (uchar)sobel_x_output;


            //Sobel y-direction edge detection using the matrix
            //     -1 -2 -1
            //      0  0  0
            //      1  2  1

            int sobel_y_sum = (int)(gray_flower.at<uchar>(i - 1, j - 1) * -1) + (int)(gray_flower.at<uchar>(i - 1, j) * -2) +
                (int)(gray_flower.at<uchar>(i - 1, j + 1) * -1) + (int)(gray_flower.at<uchar>(i + 1, j - 1)) +
                    (int)(gray_flower.at<uchar>(i + 1, j ) * 2) + (int)(gray_flower.at<uchar>(i + 1, j + 1));

            int sobel_y_output = 127 + sobel_y_sum*0.3;
            sobel_y_output = std::max(0, std::min(255, sobel_y_output));
            sobel_y_img.at<uchar>(i, j) = (uchar)sobel_y_output;
        }
    }

    cv::imshow("Laplacian", laplacian_img);
    cv::imshow("Sobel X", sobel_x_img);
    cv::imshow("Sobel Y", sobel_y_img);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

void challenge() {

    //read image as gray as to hold correct data
    std::string img_path = "Building.jpg";
    cv::Mat building_img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);

    cv::Mat working_img = building_img.clone();
    int pixel_counter[256] = { 0 };

    //create frequency table for each intensity of gray
    for (int i = 0; i < building_img.rows; i++) {
        for (int j = 0; j < building_img.cols; j++) {
            int current_pixel = building_img.at<uchar>(i, j);
            pixel_counter[current_pixel]++;
        }
    }
    int total_pixels = building_img.rows * building_img.cols;

    float pixel_probs[256] = { 0.0 };

    //calculate probabilty for each intensity
    for (int i = 0; i < 256; i++) {
        pixel_probs[i] = (float)((float)pixel_counter[i] /(float) total_pixels);
    }
    
    float cumulative_probs[256] = { 0.0 };

    //create table of increasing cumulative probability 
    for (int i = 0; i < 256; i++) {
        if (i == 0) {
            cumulative_probs[i] = pixel_probs[i];
            continue;
        }
        cumulative_probs[i] = cumulative_probs[i - 1] + pixel_probs[i];
    }

    uchar lookup_table[256] = { 0 };

    //create a lookup table for each intesity for use in final image
    for (int i = 0; i < 256; i++) {
        lookup_table[i] = floor(255 * cumulative_probs[i]);
    }

    for (int i = 0; i < working_img.rows; i++) {
        for (int j = 0; j < working_img.cols; j++) {
            working_img.at<uchar>(i, j) = lookup_table[working_img.at<uchar>(i, j)];
        }
    }
    cv::imshow("Normal", building_img);
    cv::imshow("Equalization", working_img);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main()
{
    part1();
    part2();
    part3();
    completion();
    challenge();

    return 0;
}