#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <array>
#include <iomanip>

// получение матрицы из картинки
std::array<std::array<double, 16>, 20> get_matrix(const char* filename)
{
	// получаем картинку
	IplImage* image = cvLoadImage(filename, 0);
	// клонируем картинку 
	IplImage* src = cvCloneImage(image);
	// меняем размер картинки
	IplImage* resized_image = cvCreateImage(cvSize(16, 20), image->depth, image->nChannels);
	cvResize(image, resized_image);

	cv::Mat matr = cv::cvarrToMat(resized_image);

	std::array<std::array<double, 16>, 20> matrix;

	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			matrix[i][j] = ((double)matr.at<uchar>(i, j)) / 255;
		}
	}

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&src);
	cvReleaseImage(&resized_image);

	return matrix;
}

int main()
{
	const char* filename = "l_0_04_0_aaaaa.jpg.jpg";

	std::array<std::array<double, 16>, 20> matrix = get_matrix(filename);

	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			std::cout << std::fixed << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	
	return 0;
}