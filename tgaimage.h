#ifndef __IMAGE_H__
#define __IMAGE_H__

#include<fstream>

#pragma pack(push, 1)
/**
 * @struct TGA_Header
 * @brief TGA文件头结构体（紧凑内存布局）
 * @note 使用1字节对齐保证跨平台兼容性
 * 各字段含义参考TGA文件格式规范：
 * - idlength: 图像ID字段长度（0-255）
 * - colormaptype: 颜色表类型（0=无，1=调色板）
 * - datatypecode: 图像数据类型码（2=未压缩真彩色）
 * - colormaporigin: 颜色表起始索引
 * - colormaplength: 颜色表条目数
 * - colormapdepth: 颜色表位深度（15,16,24,32）
 * - x_origin/y_origin: 图像原点坐标（通常为0）
 * - width/height: 图像尺寸（像素）
 * - bitsperpixel: 像素位深度（24/32）
 * - imagedescriptor: 图像描述符（位3-0: alpha通道位数）
 */
struct TGA_Header
{
    unsigned char idlength;
    unsigned char colormaptype;
    unsigned char datatypecode;
    unsigned short colormaporigin;
    unsigned short colormaplength;
    unsigned char colormapdepth;
    unsigned short x_origin;
    unsigned short y_origin;
    unsigned short width;
    unsigned short height;
    unsigned char bitsperpixel;
    unsigned char imagedescriptor;
};
#pragma pack(pop)

/**
 * @struct TGAColor
 * @brief TGA颜色表示结构体
 * @note 使用联合体实现多种颜色访问方式：
 * - 结构体成员（b,g,r,a）：按BGRA顺序访问（TGA标准存储顺序）
 * - raw[4]：按字节数组访问
 * - val：整型值形式（注意小端字节序）
 * 
 * @var bytespp 每个像素的字节数（3=BGR，4=BGRA）
 */
struct TGAColor
{
    union
    {
        struct
        {
            unsigned char b, g, r, a;
        };
        unsigned char raw[4];
         /*
         * @note val字段解释依赖字节序：
         * - 小端系统：低位字节在前（例如0xFF0000FF表示RGBA(255,0,0,255)）
         * - 32位颜色值布局：0xAARRGGBB
         */
		unsigned int val; 
    };

    int bytespp;// 用int为了内存对齐

      /**
     * @brief 默认构造函数（创建单字节黑色）
     * @note 初始化值:
     *       - val: 0        (黑色颜色值)
     *       - bytespp: 1    (1字节/像素)
     */
    TGAColor() : val(0), bytespp(1) {
	}

      /**
     * @brief 四通道构造函数（RGBA格式）
     * @param R 红色分量 (0-255)
     * @param G 绿色分量 (0-255)
     * @param B 蓝色分量 (0-255)
     * @param A 透明度分量 (0-255)
     * @note 存储顺序为BGRA：
     *       - b(B), g(G), r(R), a(A)
     *       - bytespp固定为4
     */
	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {
	}

      /**
     * @brief 整型值构造函数
     * @param v 颜色整型值
     * @param bpp 每像素字节数 (1/2/3/4)
     * @warning 需注意字节序问题：
     *          - 小端系统低位在前
     *          - 32位值布局：0xAARRGGBB
     */
	TGAColor(int v, int bpp) : val(v), bytespp(bpp) {
	}

      /**
     * @brief 拷贝构造函数
     * @param c 源颜色对象
     * @note 执行浅拷贝：
     *       - 直接复制val和bytespp
     *       - 适用于联合体结构
     */
	TGAColor(const TGAColor &c) : val(c.val), bytespp(c.bytespp) {
	}

    /**
     * 构造函数：从字节流初始化TGAColor对象
     * 
     * @param p     指向原始颜色数据的指针（数据应按颜色通道顺序排列）
     * @param bpp   每个像素的字节数（Bytes Per Pixel），
     *              决定需要拷贝的字节数量（例如：RGB为3，RGBA为4）
     * 
     * 初始化流程：
     * - 初始化val成员变量为0（可能用于存储压缩后的颜色值）
     * - 初始化bytespp成员变量为输入的bpp值
     * - 将输入数据的每个字节拷贝到raw数组成员中
     */
    TGAColor(const unsigned char *p, int bpp) : val(0), bytespp(bpp) {
		for (int i=0; i<bpp; i++) {
			raw[i] = p[i];
		}
	}

      /**
     * @brief 赋值运算符重载
     * @param c 源颜色对象
     * @return TGAColor& 返回当前对象的引用
     * @note 执行深拷贝，避免自赋值
     */
	TGAColor & operator =(const TGAColor &c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
};

class TGAImage {
    protected:
        unsigned char* data;
        int width;
        int height;
        int bytespp;
    
        bool   load_rle_data(std::ifstream &in);
        bool unload_rle_data(std::ofstream &out);
    public:
        enum Format {
            GRAYSCALE=1, RGB=3, RGBA=4
        };
    
        TGAImage();
        TGAImage(int w, int h, int bpp);
        TGAImage(const TGAImage &img);
        bool read_tga_file(const char *filename);
        bool write_tga_file(const char *filename, bool rle=true);
        bool flip_horizontally();
        bool flip_vertically();
        bool scale(int w, int h);
        TGAColor get(int x, int y);
        bool set(int x, int y, TGAColor c);
        ~TGAImage();
        TGAImage & operator =(const TGAImage &img);
        int get_width();
        int get_height();
        int get_bytespp();
        unsigned char *buffer();
        void clear();
    };

#endif
