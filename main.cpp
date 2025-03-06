#include "tgaimage.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    for (float t=0.; t<1.; t+=.1) {
        int x = x0*(1.-t) + x1*t;
        int y = y0*(1.-t) + y1*t;
        printf("(%d, %d)\n", x, y);
        image.set(x, y, color);
    }
}
int main(int arge, char *argv[]){
    TGAImage image(100, 100, TGAImage::RGB);
    // line(13, 20, 80, 40, image, white);
    //image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    // image.set(13, 20, red); 
    // image.set(19, 21, red);
    // image.set(26, 23, red);
    // image.set(33, 25, red);
    // image.set(39, 27, red);
    // image.set(46, 30, red);
    // image.set(53, 31, red);
    // image.set(59, 34, red);
    // image.set(66, 36, red);
    // TGAColor promo11 = image.get(66, 36);
    // printf("(r1 =%d, g1 = %d, b1 = %d, a1 = %d)\n\n\n", promo11.r, promo11.g, promo11.b, promo11.a);
    // image.set(72, 38, white); //2222
    // image.set(74, 38, white);
    for (int i = 0; i < 80; i++)
    {
        image.set(i, 37, white); 
        image.set(i, 38, white);    //单独画前38行的前80个像素不知道为什么会画不出来，但是画第39行前80个像素就可以画出来
    }

    bool res = image.write_tga_file("LearnTinyRender/output.tga");
    if (res)
    std::cout<<"success"<<std::endl;
    else
    std::cout<<"fail"<<std::endl;
    
    return 0;
}