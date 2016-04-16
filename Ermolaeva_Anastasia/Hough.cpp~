#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>
#include <cmath>

#define DEG2RAD 0.017453293f
#define OUT -10.0

using namespace cv;
using namespace std;

double shifts_function(double d)
{
    return std::sin(d);
}

class Hough 
{  
    private:

    static int myround(double d) {
        return d > 0.0 ? (int)(d + 0.5) : (int)(d - 0.5);
    }

    double check_x(int x, double r, int t, int h)
    {
        int y = (int)((r - (double)(x)*cosCache[t])/sinCache[t]);
        if ((y >= 0) && (y <= h))
        {
            return y;
        }
        return OUT;
    }

    double check_y(int y, double r, int t, int w)
    {
        int x = (int)((r - (double)(y)*sinCache[t])/cosCache[t]);
        if ((x >= 0) && (x <= w))
        {
            return x;
        }
        return OUT;
    }

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

        uint8_t* houghlines_shift(uint8_t* data, double(*func)(double), int w, int h) 
        {
            
            int houghHeight = std::sqrt((double)(w*w+h*h))/2;
            int doubleHeight = 2 * houghHeight; 
            int houghWidth = 180;
            
            int* houghArray = (int*)calloc(doubleHeight*houghWidth, sizeof(int)); 
        
            double center_x = w/2.0;
            double center_y = h/2.0;
            for (int x = 1; x < w; x++) 
            { 
                for (int y = 1; y < h; y++) 
                { 
                    
                    if (data[y*w + x] > 0) 
                    { 
                        for (int t = 0; t < houghWidth; t++) 
                        {                      
                            int r = myround(((double)x - center_x) * cosCache[t]) + myround( ((double)y - center_y) * sinCache[t]);
                            r += houghHeight; 

                            // координаты крайних точек отрезка(чтобы найти середину)
                             int x1, y1, x2, y2;

                            //вертикальная линия
                            if (t == 0)
                            {    
                                x1 = x;
                                x2 = x;
                                y1 = 0;
                                y2 = h;
                            }
                            else 
                            {
                                //горизонтальная линия                                
                                if (t == 90) 
                                {
                                    x1 = 0;
                                    x2 = w;
                                    y1 = y;
                                    y2 = y;
                                }
                                else
                                {
                                    y1 = check_x(0, r, t, h);
                                    x1 = 0;
                                    if (y1 == OUT)
                                    {
                                        y1 = check_x(w, r, t, h);
                                        x1 = w;
                                    }
                                     
                                    x2 = check_y(0, r, t, w);
                                    y2 = 0;
                                    if (x2 == OUT)
                                    {
                                        x2 = check_y(h, r, t, w);
                                        y2 = h;
                                    }
                                    
                                }
                            }
                            double mid_segment_x = (x1 + x2)/2.0f;
                    
                            double mid_segment_y = (y1 + y2)/2.0f;
                            
                            double d = std::sqrt(std::pow((mid_segment_x - x), 2) + std::pow(mid_segment_y - y, 2));
                            double shift = func(d); 
                            
                            double shift_x = (double)x + shift*cosCache[t];
                            double shift_y = (double)y + shift*sinCache[t];
                            if ((shift_x >= 0) && (shift_x <= w) && (shift_y >= 0) && (shift_y <= h))
                            {
                                 houghArray[r*180 + t] += data[(int)(shift_y*w + shift_x)]; 
                            }
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
    uint8_t* new_pic = hough.houghlines_shift(src.data, shifts_function, w, h);
    
    double hh = std::sqrt((double)(w*w+h*h))/2;
    int acc_h = 2*hh;
    int acc_w = 180;
    Mat new_p = Mat(acc_h, acc_w, CV_8U, new_pic);
    
    imshow("source", src);

    imshow("new", new_p);
    waitKey();

    return 0;
}
