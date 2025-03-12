#include "tgaimage.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

/*
 线性插值，lerp
*/
void line3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    for (float t=0.; t<1.; t+=.1) {
        int x = x0*(1.-t) + x1*t;
        int y = y0*(1.-t) + y1*t;
        image.set(x, y, color);
    }
}

/*
 逐像素插值，line2
*/
void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    for (int x=x0; x<=x1; x++) {
        float t = (float)(x-x0)/(x1-x0);
        int y = y0*(1.-t) + y1*t; 
        image.set(x, y, color); 
    }
}
/*  
    逐行逐像素去画线，如果有斜率非常陡峭的线，像素绘制时会不连续
*/
void line4(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;  // 陡峭线段检测
    if (std::abs(x0-x1)<std::abs(y0-y1)) { // 当垂直变化量（Δy）大于水平变化量（Δx）时，判定为陡峭线段（斜率绝对值>1
        std::swap(x0, y0); 
        std::swap(x1, y1);  // 将坐标对调（例如原坐标(2,5)变为(5,2)，本质操作：将坐标系旋转90度，使陡峭线段转换为平缓线段处理
        steep = true; 
    } 
    if (x0>x1) { // make it left−to−right 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    for (int x=x0; x<=x1; x++) { 
        float t = (x-x0)/(float)(x1-x0); 
        int y = y0*(1.-t) + y1*t; 
        if (steep) { 
            image.set(y, x, color); // if transposed, de−transpose 还原操作
        } else { 
            image.set(x, y, color); 
        } 
    } 
}

/*  
     Bresenham算法，把除法从循环中剥离出来，减少除法次数，提高效率
*/
void line5(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    float derror = std::abs(dy/float(dx)); 
    float error = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error += derror; 
        if (error>.5) { 
            y += (y1>y0?1:-1); 
            error -= 1.; 
        } 
    } 
} 
/**
 *  Bresenham算法的整数优化
 * 
 */
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
} 

int main(int arge, char *argv[]){
    TGAImage image(100, 100, TGAImage::RGB);
    line(13, 20, 80, 40, image, white); 
    line(20, 13, 40, 80, image, red); 
    line(80, 40, 13, 20, image, red);
    image.flip_vertically(); // 左下为原点
    image.write_tga_file("LearnTinyRender/output.tga");
    
    return 0;
}