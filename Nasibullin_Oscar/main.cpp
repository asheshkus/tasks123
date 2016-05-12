#include <cv.h>
#include <highgui.h>
#include <opencv2/core/types_c.h>
#include "Mnist.h"
#include <time.h>
#define ESC char(0x1B)

using namespace std;
using namespace cv;

// возвращает число из отрезка [-0.9 , 0.9]
float randRate(void)
{
	return ((float)pow((-1),rand()%2)) * ((float)(rand()%10)) * 0.1f;
}

// возвращает строку по двоичному коду числа, единица - есть буква, ноль - нет буквы.
string getIndexedString(int id)
{
	char* alphabet = "abcdefghijklmnopqrstuvwxyz";
	string rezult("_");

	if (id==0)
		return "_a";

	for(int i=0; id!=0; i++)
	{
		if( id%2 )
		{
			rezult.push_back(alphabet[i]);
		}
		id /= 2 ;
	}

	return rezult;
}


int main( int argc, char** argv )
{
	if( argc < 4)
	{
		//   пример входных данных
		/// /Users/Oskar/Desktop/Phystech/C++/Cognitive/MNIST/train-images-idx3-ubyte
		/// /Users/Oskar/Desktop/Phystech/C++/Cognitive/MNIST/train-labels-idx1-ubyte
		/// /Users/Oskar/Desktop/Images/

		///Users/Oskar/Desktop/Phystech/C++/Cognitive/MNIST/train-images-idx3-ubyte /Users/Oskar/Desktop/Phystech/C++/Cognitive/MNIST/train-labels-idx1-ubyte /Users/Oskar/Desktop/Images/ 100


		cout << "no args, usage:  <dataFile_path>  <LabelFile_path>  <output_path>  <optional: images_amount>";
		return -1;
	}

	string dataPath(argv[1]);
	string labelPath(argv[2]);
	string outPath(argv[3]);
	Mnist mnist(labelPath, dataPath);

	int rows = mnist.getRows();
	int cols = mnist.getCols();
	int amountImg = 20;
	int warpsAmount = 20; //количество генерируемых искажений для каждой картинки
	int warpRadius = 3;   // радиус в пикселях, при 5 искажения бывают слишком большими, 3 - оптимальный вариант
	int statusUpdateTime = 5; //время между выводом статуса работы

	if(argc > 4)
	{
		int inputAmount = stoi(string(argv[4]));
		if(inputAmount == 0)
		{
			amountImg = mnist.getNumberOfImages();
		}
		else
		{
			amountImg = inputAmount;
		}
	}

	int fullAmount = amountImg * warpsAmount + amountImg;
	time_t lastTime = time(0) - 15 ;
	time_t startTime = time(0) ;

	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(3);

	int savedImageCount = 0;

	cout << "Generation status" << endl;

	for(int i = 0; i < amountImg; i++)
	{
		unsigned char* imageData = (unsigned char*)calloc(rows * cols ,sizeof(char));
		unsigned char label;

		mnist.getNext(imageData, &label);

		Mat img(Size(cols,rows),CV_8U);
		img.data = imageData;

		imwrite(outPath + to_string(((int)label)) + "_" + getIndexedString(savedImageCount) + ".png", img, compression_params);
		savedImageCount++;

		// задаём точки
		std::vector<cv::Point2f> srcPoints;
		srcPoints.push_back(cv::Point2f(rows/4, cols/4));           //src Top left square center
		srcPoints.push_back(cv::Point2f(3 * rows/4, cols/4));       //src Top right square center
		srcPoints.push_back(cv::Point2f(rows/4, 3 * cols/4));       //src Bottom left square center
		srcPoints.push_back(cv::Point2f(3 * rows/4, 3 * cols/4));   //src Bot right square center

		for (int j = 0; j < warpsAmount; j++)
		{
			Mat warpedImg(Size(cols,rows),CV_8U);
			std::vector<cv::Point2f> dstPoints;

			for(int i = 0; i < 4; i++) {
				float x = srcPoints[i].x + warpRadius * randRate();
				float y = srcPoints[i].y + warpRadius * randRate();
				dstPoints.push_back(cv::Point2f(x, y));
			}

			CvMat* warp_matrix = cvCreateMat(3,3,CV_32FC1);

			// получаем матрицу преобразования
			cv::Mat H = cv::findHomography(srcPoints, dstPoints, CV_RANSAC);
			// преобразование перспективы
			cv::warpPerspective(img,
				warpedImg,
				H,
				cv::Size(28,28),
				cv::INTER_NEAREST | CV_WARP_FILL_OUTLIERS
			);

			string name = to_string(((int)label)) + "_" + getIndexedString(savedImageCount) + ".png";
			savedImageCount++;

			/*
			// для вывода изображений во время процесса раскомментируйте этот код
			string window("Perspective Warping Tool");
			namedWindow(window, CV_WINDOW_AUTOSIZE );
			imshow(window, warpedImg );
			waitKey(1);
			cvDestroyWindow(window.c_str());
			*/

			imwrite(outPath + name , warpedImg, compression_params);
		}

		// вывод текущего статуса работы
		time_t delta = time(0) - lastTime ;
		if(delta > statusUpdateTime) {
			int progress = (int) (((float) (savedImageCount) / (float) (fullAmount) * 100));

			// времени затрачено
			time_t timeFromStartInSec = time(0) - startTime ;
			tm sfromStart = *gmtime(&timeFromStartInSec);

			// скорость
			float speed = (float)savedImageCount / timeFromStartInSec;

			// времени осталось
			time_t timeLeft = (time_t)((fullAmount - savedImageCount) / speed);
			tm sleft = *gmtime(&timeLeft) ;

			string spent = to_string(sfromStart.tm_hour) + "h "+ to_string(sfromStart.tm_min) + "m " + to_string(sfromStart.tm_sec) + "s";
			string left = to_string(sleft.tm_hour) + "h "+ to_string(sleft.tm_min) + "m " + to_string(sleft.tm_sec) + "s";

			// очистка строки и вывод данных
			cout << ESC << "[2K" << ESC << "[200D" << "\r\tprogress: " << progress << "%  [" << savedImageCount << "/" << fullAmount
				<< "]     speed: " << (int)speed << " img/sec     time spent: "
				<< spent <<"     time left: "<< left;

			cout.flush();

			lastTime = time(0);
		}
	}
	cout << ESC << "[2K" << ESC << "[200D" << "\r\tGeneration complete! " << savedImageCount <<" images saved to " << outPath << endl;
}