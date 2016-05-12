//
// Created by Оскар on 06.04.16.
//

#ifndef COGNITIVECV_MNIST_H
#define COGNITIVECV_MNIST_H

#include <string>
#include <fstream>
#include <iostream>

class Mnist {

private:
    std::ifstream labelFile, dataFile;
    int rows, cols, numberOfImages, currentImageId;
    char currentLabel;

    int ReverseInt (int i);
    void getCurrentLabel(unsigned char* label);
    void getCurrentImage(unsigned char* image);

public:
    int getCurrentImageId() const {
        return currentImageId;
    }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

    int getNumberOfImages() const {
        return numberOfImages;
    }

    Mnist(std::string labelFilePath, std::string dataFilePath);

    void getNext(unsigned char *image, unsigned char *label);
};


#endif //COGNITIVECV_MNIST_H
