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
		Hough()
		{
			sinCache = std::vector<double>(180);
			cosCache = std::vector<double>(180);			
			for (int t = 0; t < 180; t++) 
			{ 
				
				double theta = (double)t* DEG2RAD;
	            sinCache[t] = std::sin(theta); 
	            cosCache[t] = std::cos(theta); 
	        } 
		}

		uint8_t* houghlines_shift(uint8_t* data, std::vector<double> shifts, int w, int h) 
		{
			int max_dim = h;
			if (w > max_dim)
			{	
				max_dim = w;
			}
			
			
 			int houghHeight = std::sqrt((double)(w*w+h*h))/2;
        	int doubleHeight = 2 * houghHeight; 
 			int houghWidth = 180;
        	
        	int* houghArray = (int*)calloc(doubleHeight*houghWidth, sizeof(int)); 
		
			double center_x = w/2;
			double center_y = h/2;
			for (int x = 0; x < w; x++) 
			{ 
	            for (int y = 0; y < h; y++) 
				{ 
	            
	                if (data[y*w+x] > 0) 
					{ 
	                    for (int t = 0; t < houghWidth; t++) 
						{ 
            				double r = (((double)x - center_x) * cosCache[t]) + (((double)y - center_y) * sinCache[t]); 
 
				            r += houghHeight; 
 
            				houghArray[(int)(round(r)*180) + t] += data[y*w+x]; 

        				} 
	                } 
	            } 
	        } 
	
			int max_val = -10000;
			for (int r = 0; r < doubleHeight; r++)
			{
				for (int t = 0; t < houghWidth; t++) 
				{
					if (houghArray[r*houghWidth + t] > max_val)
					{
						max_val = houghArray[r*houghWidth + t];
					}
				}

			}
			
			cout << max_val << "\n";
			uint8_t* hough_uint = (uint8_t*)calloc(doubleHeight*houghWidth, sizeof(uint8_t)); 
			for (int r = 0; r < doubleHeight; r++)
			{
				for (int t = 0; t < houghWidth; t++) 
				{
					hough_uint[r*houghWidth + t] = (uint8_t)(houghArray[r*houghWidth + t] * 255.0f/ max_val);
				
				}

			}
			
			return hough_uint;
		}

	private:
		std::vector<double> sinCache;
		std::vector<double> cosCache;
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
	uint8_t* new_pic = hough.houghlines_shift(src.data, std::vector<double>(), w, h);
	
	int max_d = h;
	if (w > max_d)
	{	
		max_d = w;
	}
	double hh = std::sqrt((double)(w*w+h*h))/2;
	int acc_h = 2*hh;
	int acc_w = 180;
	Mat new_p = Mat(acc_h, acc_w, CV_8U, new_pic);
	
	imshow("source", src);

	imshow("new", new_p);
    waitKey();

    return 0;
}
