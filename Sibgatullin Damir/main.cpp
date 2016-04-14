#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <json/json.h>

using namespace std;

// получение матрицы из картинки
vector<double> get_matrix(const char* filename)
{
	// получаем картинку
	IplImage* image = cvLoadImage(filename, 0);
	// клонируем картинку 
	IplImage* src = cvCloneImage(image);
	// меняем размер картинки
	IplImage* resized_image = cvCreateImage(cvSize(16, 20), image->depth, image->nChannels);
	cvResize(image, resized_image);

	cv::Mat matr = cv::cvarrToMat(resized_image);

	vector<double> vect;

	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			vect.push_back(((double)matr.at<uchar>(i, j)) / 255);
		}
	}

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&src);
	cvReleaseImage(&resized_image);

	return vect;
}

void fully_connected_layer(vector<double>& vect, string matrix, string bias)
{
	ifstream bias_file(bias);
	ifstream matrix_file(matrix);

	double d_b;
	char c;

	vector<double> result;

	while (bias_file >> d_b)
	{
		double sum = 0;
		double d_m;
		int j = 0;
		c = ',';
		while (c == ',' && matrix_file >> d_m)
		{
			sum += d_m * vect[j];
			++j;
			matrix_file >> c;
		}
		bias_file >> c;
		result.push_back(sum + d_b);
	}

	vect.clear();
	vect = result;
}

void neuron_layer(vector<double>& vect, string function)
{
	if (function.compare("tanh") == 0)
	{
		for (int i = 0; i < vect.size(); ++i)
		{
			vect[i] = tanh(vect[i]);
		}
	}
	else if (function.compare("softmax") == 0)
	{
		double sum = 0;
		for (int i = 0; i < vect.size(); ++i)
		{
			vect[i] = exp(vect[i]);
			sum += vect[i];
		}

		for (int i = 0; i < vect.size(); ++i)
		{
			vect[i] = vect[i] / sum;
		}
	}
}

vector<double> recognize(const vector<double>& vect, string& output_pins)
{
	ifstream recognizer_file("recognizer.json");
	Json::Value root;
	recognizer_file >> root;
	int number_of_layers = root["recognizer"].get("number_of_layers", 0).asInt();

	const Json::Value layers = root["recognizer"];

	vector<double> result(vect);

	for (Json::Value::const_iterator itr = layers.begin(); itr != layers.end(); ++itr)
	{
		if (!(itr->isObject()))
		{
			break;
		}

		string type = itr->get("type", "").asString();

		if (type.compare("fully_connected") == 0)
		{
			fully_connected_layer(result, itr->get("matrix", "").asString(), itr->get("bias", "").asString());
		}
		else if (type.compare("neuron") == 0)
		{
			neuron_layer(result, itr->get("function", "").asString());
		}
	}

	output_pins = root.get("output_pins", "").asString();

	return result;
}

int main()
{
	const char* filename = "6_2_03_06_aabmh.jpg.jpg";
	cout << filename << endl << endl;

	vector<double> vect = get_matrix(filename);

	string output_pins;
	vector<double> result = recognize(vect, output_pins);

	for (int i = 0; i < result.size(); ++i)
	{
		cout << output_pins[i] << ": " << result[i] << endl;
	}

	cout << endl;
	
	return 0;
}