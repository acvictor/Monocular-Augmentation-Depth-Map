#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/boundingBox.h"

using namespace std;
using namespace cv;

void BoundingBox::computeBoundingBoxes(int argc, char** argv, float depth[][768]) {   
    int count = 0;

    if(argc != 2) {
        cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
        exit(0);
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   

    if(!image.data ) {
        cout <<  "Could not open or find the image" << endl;
        exit(0);
    }                 

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int r, g, b;
            r = (int)image.at<Vec3b>(i, j)[2];
            g = (int)image.at<Vec3b>(i, j)[1];
            b = (int)image.at<Vec3b>(i, j)[0];
            bool found = false;
            for (int k = 0; k < colors.size(); k++) {
                if(r == colors[k].r && g == colors[k].g && b == colors[k].b) {
                    found = true;
                    break;
                }
            }
            if(found == false) {
                ImColor c(r, g, b);
                colors.push_back(c);
            }
        }
    }
    cout << colors.size() << endl;
    cout << image.rows << "x" << image.cols << endl;
    for (int k = 0; k < colors.size(); k++) {
        int minx = 2000, miny = 2000, maxx = -1, maxy = -1;
        float minz = 1000, maxz = -1000;
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                int r, g, b;
                r = (int)image.at<Vec3b>(i, j)[2];
                g = (int)image.at<Vec3b>(i, j)[1];
                b = (int)image.at<Vec3b>(i, j)[0];
                if(r == colors[k].r && g == colors[k].g && b == colors[k].b) {
                    minx = min(minx, j);
                    maxx = max(maxx, j);
                    miny = min(miny, i);
                    maxy = max(maxy, i);
                    minz = min(minz, depth[i][j]);
                    maxz = max(maxz, depth[i][j]);
                }
            }
        }
        
        rectangle(image,Point(minx, miny),Point(maxx,maxy),Scalar(0,255,0),1);
        Box b(minx, maxx, miny, maxy, minz, maxz);
        boxes.push_back(b);
    }
    /*
     * uncomment to view segmentation
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );    
    waitKey(0); 
     */
}
