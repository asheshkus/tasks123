#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>
#include <cmath>

#define DEG2RAD 0.017453293f

using namespace cv;
using namespace std;


class Hough 
{
	public:
		double* houghlines_shift(unsigned char* data, std::vector<double> shifts, int w, int h) 
		{
			img_w = w;
			img_h = h;
			float diag = sqrt((double)(w*w+ h*h));
			double center_x = w/2;
			double center_y = h/2;
			int acc_h = round(diag);
			int acc_w = 180;
			double* acc = (double*)calloc(acc_h * acc_w, sizeof(double));
			for (int y=0; y < h; y++)
			{
				for (int x=0; x < w; x++)
				{
						if (data[y*w+x] > 0) 
						{
							for (int t = 0; t < acc_w; t++)
							{
								double r = ( ((double)(x - center_x)) * cos((double)t * DEG2RAD)) + (((double)(y-center_y)) * sin((double)t * DEG2RAD));
								
								acc[(int)((r + diag/2.0f) * acc_w + t)] += data[y*w+x];
							}
						}
				}
			}

			/*double max_val = -10000;
			for (int r = 0; r < acc_h; r++)
			{
				for (int t = 0; t < acc_w; t++) 
				{
					if (acc[r*acc_w+t] > max_val)
					{
						max_val = acc[r*acc_w+t];
					}
				}

			}
			
			*/
			for (int r = 0; r < acc_h; r++)
			{
				for (int t = 0; t < acc_w; t++) 
				{
					acc[r*acc_w+acc_h] = int(acc[r*acc_w+acc_h])%255;
				}

			}
			
			return acc;
		}

	private:
		int img_w;
		int img_h;
};

int main(int argc, char** argv) 
{
	const char* filename = argc >= 2 ? argv[1] : "pic1.png";
	Mat src;
	
    src = imread(filename, 0);
    if (src.empty())
    {
        cout << "Can't open " << filename << "\n";
        return -1;
    }

	Hough hough;
	int w = src.cols;  
	int h = src.rows;
	double* new_pic = hough.houghlines_shift(src.data, std::vector<double>(), w, h);

	int acc_h = round(sqrt((double)(w*w+h*h)));
	
	int acc_w = 180;
	Mat new_p = Mat(acc_h, acc_w, CV_16U, new_pic);
	cout << new_p.rows << "\n";
	cout << new_p.cols << "\n";
	
	
	imshow("source", src);

	imshow("new", new_p);
    

    waitKey();

    return 0;
}
