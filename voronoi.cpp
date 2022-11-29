#include <iostream>
#include <fstream>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;
uint32_t image_pixels[HEIGHT][WIDTH];
const int maxColor = 255; // 2**8 -1

struct Point{
    int x;
    int y;
};
const int NUMBER_OF_POINTS = 10;
Point points[NUMBER_OF_POINTS];

uint32_t const COLOR_WHITE = 0xFFFFFF;
uint32_t const COLOR_BLACK = 0x000000;

// Spectral color palete
uint32_t const SPECTRAL_01 = 0x42019e;
uint32_t const SPECTRAL_02 = 0x4fd53e;
uint32_t const SPECTRAL_03 = 0x436df4;
uint32_t const SPECTRAL_04 = 0x61aefd;
uint32_t const SPECTRAL_05 = 0x8be0fe;
uint32_t const SPECTRAL_06 = 0x98f5e6;
uint32_t const SPECTRAL_07 = 0xa4ddab;
uint32_t const SPECTRAL_08 = 0xa5c266;
uint32_t const SPECTRAL_09 = 0xbd8832;
uint32_t const SPECTRAL_10 = 0xa24f5e;
uint32_t colors[NUMBER_OF_POINTS] = {SPECTRAL_01, SPECTRAL_02, SPECTRAL_03, SPECTRAL_04, SPECTRAL_05, SPECTRAL_06, SPECTRAL_07, SPECTRAL_08, SPECTRAL_09, SPECTRAL_10};

int getDistanceSquared(Point p1, Point p2){
    return (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y);
}

void drawCircle(Point center, int radius, uint32_t color){
    Point p0 = {center.x - radius, center.y - radius};
    Point p1 = {center.x + radius, center.y + radius};

    for (int y = p0.y; y < p1.y ;y++){
        for (int x = p0.x; x < p1.x;x++){
            if ((x < WIDTH) && (y < HEIGHT)){
                Point p = {x,y};
                int distanceToCenterSquared = getDistanceSquared(center, p);
                if (distanceToCenterSquared < radius*radius){
                    image_pixels[y][x] = color;
                }
            }
        }
    }
}

int main(){
    std::srand(std::time(nullptr)); // use current time as seed for random generator

    // Generate random points
    for (int i = 0; i < NUMBER_OF_POINTS; i++){
        points[i].x = std::rand() % WIDTH;
        points[i].y = std::rand() % HEIGHT;
    }

    // Draw voronoi diagram
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            int minDistancePointIdx;
            int minDistance = INT_MAX;
            for (int i=0;i < NUMBER_OF_POINTS; i++){
                Point currentPixel = {x,y};
                int distance = getDistanceSquared(currentPixel, points[i]);
                if (distance < minDistance){
                    minDistance = distance;
                    minDistancePointIdx = i;
                }
            }
            image_pixels[y][x] = colors[minDistancePointIdx];
        }
    }

    // Draw points
    for (int i=0; i < NUMBER_OF_POINTS; i++){
        drawCircle(points[i], 5, COLOR_BLACK);
    }

    // Write image to file
    // - ASCII header
    std::ofstream ofs("voronoi.pbm", std::ofstream::out);
    ofs << "P6\n";
    ofs << WIDTH << " " << HEIGHT << " " << maxColor << " ";

    // - Pixels in Binary
    for(int j=0; j < HEIGHT;j++){
        for(int i=0; i < WIDTH;i++){
            ofs.write((char*)&image_pixels[j][i], 3); // Write 3 bytes (R G B)
        }
    }
    ofs.close();

}
