//
// Created by Оскар on 06.04.16.
//

#include "Mnist.h"

Mnist::Mnist(std::string labelFilePath, std::string dataFilePath) {
    labelFile = std::ifstream(labelFilePath, std::ios::binary);
    dataFile = std::ifstream(dataFilePath, std::ios::binary);

    if (!labelFile.is_open())
    {
        std::cout << "\nfailed to open label file";
        throw "failed to open label file";
    }
    if (!dataFile.is_open())
    {
        std::cout << "\nfailed to open data file";
        throw "failed to open data file";
    }
    int magic_number = 0;
    int numberOfLabels = 0;
    dataFile.read((char*)&magic_number,sizeof(magic_number));
    magic_number = ReverseInt(magic_number);
    labelFile.read((char*)&magic_number,sizeof(magic_number));
    magic_number = ReverseInt(magic_number);

    dataFile.read((char*)&numberOfImages,sizeof(numberOfImages));
    numberOfImages = ReverseInt(numberOfImages);
    labelFile.read((char*)&numberOfLabels,sizeof(numberOfLabels));
    numberOfLabels = ReverseInt(numberOfLabels);

    if(numberOfImages != numberOfLabels)
    {
        std::cout << "\nSomething wrong: amounts of images and labels are not the same!";
        std::cout << "\nimages: " << numberOfImages;
        std::cout << "\nlabels: " << numberOfLabels;
        throw "amounts of images and labels are not the same!";
    }

    dataFile.read((char*)&rows,sizeof(rows));
    rows = ReverseInt(rows);
    dataFile.read((char*)&cols,sizeof(cols));
    cols = ReverseInt(cols);

    currentImageId = 0;
}

void Mnist::getCurrentLabel(unsigned char* label) {
    labelFile.read((char*)label,sizeof(char));
}

void Mnist::getCurrentImage(unsigned char* image) {
    //

    for(int r=0; r < rows; ++r)
    {
        for(int c = 0; c < cols; ++c)
        {
            unsigned char temp=0;
            dataFile.read((char*)&temp,sizeof(temp));
            image[(rows * r)+c] = temp;
        }
    }
}

void Mnist::getNext(unsigned char *image, unsigned char *label) {
    if(currentImageId < numberOfImages - 1)
    {
        getCurrentImage(image);
        getCurrentLabel(label);
        currentImageId++;
    }
    else
    {
        std::cout << "\nThe end of the dataset has been reached";
        image = nullptr;
        label = nullptr;
        return;
    }
}

int Mnist::ReverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}
