#include "mt9v03x_project.h"

#include <math.h>


#define AT                  AT_IMAGE
#define AT_CLIP(img, x, y)  AT_IMAGE((img), clip((x), 0, (img)->width-1), clip((y), 0, (img)->height-1));
#define PI_1           		    3.14159265358979f

uint8 point_addtion = 0;

extern int clip(int x, int low, int up);



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     高斯平滑空间滤波
// 参数说明     void
// 返回参数     void
// 使用示例     generateGaussianTemplate();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void generateGaussianTemplate(double window[][11], int ksize, double sigma)
{
    static const double pi = 3.1415926;
    int center = ksize / 2; // 模板的中心位置，也就是坐标的原点
    double x2, y2;
    for (int i = 0; i < ksize; i++)
    {
        x2 = pow(i - center, 2);
        for (int j = 0; j < ksize; j++)
        {
            y2 = pow(j - center, 2);
            double g = exp(-(x2 + y2) / (2 * sigma * sigma));
            g /= 2 * pi * sigma;
            window[i][j] = g;
        }
    }
    double k = 1 / window[0][0]; // 将左上角的系数归一化为1
    for (int i = 0; i < ksize; i++)
    {
        for (int j = 0; j < ksize; j++)
        {
            window[i][j] *= k;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     高斯平滑空间滤波
// 参数说明     uint8
// 返回参数     return_state
// 使用示例     lineate_uart_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

void generateGaussianTemplate2(double window[][11], int ksize, double sigma)
{
    static const double pi = 3.1415926;
    int center = ksize / 2; // 模板的中心位置，也就是坐标的原点
    double x2, y2;
    double sum = 0;
    for (int i = 0; i < ksize; i++)
    {
        x2 = pow(i - center, 2);
        for (int j = 0; j < ksize; j++)
        {
            y2 = pow(j - center, 2);
            double g = exp(-(x2 + y2) / (2 * sigma * sigma));
            g /= 2 * pi * sigma;
            sum += g;
            window[i][j] = g;
        }
    }
    //double k = 1 / window[0][0]; // 将左上角的系数归一化为1
    for (int i = 0; i < ksize; i++)
    {
        for (int j = 0; j < ksize; j++)
        {
            window[i][j] /= sum;
        }
    }
}




AT_ITCM_SECTION_INIT(void clone_image(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    if (img0->width == img0->step && img1->width == img1->step) {
        memcpy(img1->data, img0->data, img0->width * img0->height);
    } else {
        for (int y = 0; y < img0->height; y++) {
            memcpy(&AT(img1, 0, y), &AT(img0, 0, y), img0->width);
        }
    }
}

AT_ITCM_SECTION_INIT(void clear_image(image_t *img)) {
    assert(img && img->data);
    if (img->width == img->step) {
        memset(img->data, 0, img->width * img->height);
    } else {
        for (int y = 0; y < img->height; y++) {
            memset(&AT(img, 0, y), 0, img->width);
        }
    }
}

// 固定阈值二值化
AT_ITCM_SECTION_INIT(void threshold(image_t *img0, image_t *img1, uint8_t thres, uint8_t low_value, uint8_t high_value)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);

    // 先遍历y后遍历x比较cache-friendly
    for (int y = 0; y < img0->height; y++) {
        for (int x = 0; x < img0->width; x++) {
                    AT(img1, x, y) = AT(img0, x, y) < thres ? low_value : high_value;
        }
    }
}

// 自适应阈值二值化
AT_ITCM_SECTION_INIT(void adaptive_threshold(image_t *img0, image_t *img1, int block_size, int down_value, uint8_t low_value, uint8_t high_value)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->data != img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(block_size > 1 && block_size % 2 == 1);

    int half = block_size / 2;
    // 先遍历y后遍历x比较cache-friendly
    for (int y = 0; y < img0->height; y++) {
        for (int x = 0; x < img0->width; x++) {
            int thres_value = 0;
            for (int dy = -half; dy <= half; dy++) {
                for (int dx = -half; dx <= half; dx++) {
                    thres_value += AT_CLIP(img0, x + dx, y + dy);
                }
            }
            thres_value /= block_size * block_size;
            thres_value -= down_value;
                    AT(img1, x, y) = AT(img0, x, y) < thres_value ? low_value : high_value;
        }
    }
}

// 图像逻辑与
AT_ITCM_SECTION_INIT(void image_and(image_t *img0, image_t *img1, image_t *img2)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img2 && img2->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0->width == img2->width && img0->height == img2->height);

    // 先遍历y后遍历x比较cache-friendly
    for (int y = 0; y < img0->height; y++) {
        for (int x = 0; x < img0->width; x++) {
                    AT(img2, x, y) = (AT(img0, x, y) == 0 || AT(img1, x, y) == 0) ? 0 : 255;
        }
    }
}

// 图像逻辑或
AT_ITCM_SECTION_INIT(void image_or(image_t *img0, image_t *img1, image_t *img2)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img2 && img2->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0->width == img2->width && img0->height == img2->height);

    // 先遍历y后遍历x比较cache-friendly
    for (int y = 0; y < img0->height; y++) {
        for (int x = 0; x < img0->width; x++) {
                    AT(img2, x, y) = (AT(img0, x, y) == 0 && AT(img1, x, y) == 0) ? 0 : 255;
        }
    }
}

// 2x2最小池化(赛道边界是黑色，最小池化可以较好保留赛道边界)
AT_ITCM_SECTION_INIT(void minpool2(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width / 2 == img1->width && img0->height / 2 == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    uint8_t min_value;
    // 先遍历y后遍历x比较cache-friendly
    for (int y = 1; y < img0->height; y += 2) {
        for (int x = 1; x < img0->width; x += 2) {
            min_value = 255;
            if (AT(img0, x, y) < min_value) min_value = AT(img0, x, y);
            if (AT(img0, x - 1, y) < min_value) min_value = AT(img0, x - 1, y);
            if (AT(img0, x, y - 1) < min_value) min_value = AT(img0, x, y - 1);
            if (AT(img0, x - 1, y - 1) < min_value) min_value = AT(img0, x - 1, y - 1);
                    AT(img1, x / 2, y / 2) = min_value;
        }
    }
}

// 图像滤波降噪
AT_ITCM_SECTION_INIT(void blur(image_t *img0, image_t *img1, uint32_t kernel)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    // 先遍历y后遍历x比较cache-friendly
    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
                    AT(img1, x, y) = (1 * AT(img0, x - 1, y - 1) + 2 * AT(img0, x, y - 1) + 1 * AT(img0, x + 1, y - 1) +
                                      2 * AT(img0, x - 1, y) + 4 * AT(img0, x, y) + 2 * AT(img0, x + 1, y) +
                                      1 * AT(img0, x - 1, y + 1) + 2 * AT(img0, x, y + 1) + 1 * AT(img0, x + 1, y + 1)) / 16;
        }
    }
}

// 3x3sobel边缘提取
AT_ITCM_SECTION_INIT(void sobel3(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    int gx, gy;
    // 先遍历y后遍历x比较cache-friendly    
    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
            gx = (-1 * AT(img0, x - 1, y - 1) + 1 * AT(img0, x + 1, y - 1) +
                  -2 * AT(img0, x - 1, y) + 2 * AT(img0, x + 1, y) +
                  -1 * AT(img0, x - 1, y + 1) + 1 * AT(img0, x + 1, y + 1)) / 4;
            gy = (1 * AT(img0, x - 1, y - 1) + 2 * AT(img0, x, y - 1) + 1 * AT(img0, x + 1, y - 1) +
                  -1 * AT(img0, x - 1, y + 1) - 2 * AT(img0, x, y + 1) - 1 * AT(img0, x + 1, y + 1)) / 4;
                    AT(img1, x, y) = (abs(gx) + abs(gy)) / 2;
        }
    }
}

// 3x3腐蚀
AT_ITCM_SECTION_INIT(void erode3(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    int min_value;
    // 先遍历y后遍历x比较cache-friendly    
    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
            min_value = 255;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (AT(img0, x + dx, y + dy) < min_value) min_value = AT(img0, x + dx, y + dy);
                }
            }
                    AT(img1, x, y) = min_value;
        }
    }
}

// 3x3膨胀
AT_ITCM_SECTION_INIT(void dilate3(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    int max_value;
    // 先遍历y后遍历x比较cache-friendly    
    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
            max_value = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (AT(img0, x + dx, y + dy) > max_value) max_value = AT(img0, x + dx, y + dy);
                }
            }
                    AT(img1, x, y) = max_value;
        }
    }
}

//
AT_ITCM_SECTION_INIT(void remap(image_t *img0, image_t *img1, fimage_t *mapx, fimage_t *mapy)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(mapx && mapx->data);
    assert(mapy && mapy->data);
    assert(img0 != img1 && img0->data != img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(mapx->width == mapy->width && mapx->height == mapy->height);
    assert(img0->width == mapx->width && img0->height == mapx->height);

    // 先遍历y后遍历x比较cache-friendly    
    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
                    AT(img1, x, y) = AT(img0, (int) (AT(mapx, x, y) + 0.5), (int) (AT(mapy, x, y) + 0.5));
        }
    }
}

/* 前进方向定义：
 *   0
 * 3   1
 *   2
 */
AT_DTCM_SECTION_ALIGN_INIT(const int dir_front[4][2], 8) = {{0,  -1},
                                                            {1,  0},
                                                            {0,  1},
                                                            {-1, 0}};
AT_DTCM_SECTION_ALIGN_INIT(const int dir_frontleft[4][2], 8) = {{-1, -1},
                                                                {1,  -1},
                                                                {1,  1},
                                                                {-1, 1}};
AT_DTCM_SECTION_ALIGN_INIT(const int dir_frontright[4][2], 8) = {{1,  -1},
                                                                 {1,  1},
                                                                 {-1, 1},
                                                                 {-1, -1}};

// 左手迷宫巡线
AT_ITCM_SECTION_INIT(void findline_lefthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num)) {
    assert(img && img->data);
    assert(num && *num >= 0);
    assert(block_size > 1 && block_size % 2 == 1);
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;
    while (step < *num && half < x && x < img->width - half - 1 && half < y && y < img->height - half - 1 && turn < 4) {
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT(img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;

        int current_value = AT(img, x, y);
        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontleft_value = AT(img, x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 1) % 4;
            turn++;
        } else if (frontleft_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {
            x += dir_frontleft[dir][0];
            y += dir_frontleft[dir][1];
            dir = (dir + 3) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}

// 右手迷宫巡线
AT_ITCM_SECTION_INIT(void findline_righthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num)) {
    assert(img && img->data);
    assert(num && *num >= 0);
    assert(block_size > 1 && block_size % 2 == 1);
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;
    while (step < *num && 0 < x && x < img->width - 1 && 0 < y && y < img->height - 1 && turn < 4) {
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT(img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;

        int current_value = AT(img, x, y);
        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontright_value = AT(img, x + dir_frontright[dir][0], y + dir_frontright[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 3) % 4;
            turn++;
        } else if (frontright_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {
            x += dir_frontright[dir][0];
            y += dir_frontright[dir][1];
            dir = (dir + 1) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}

// 折线段近似
AT_ITCM_SECTION_INIT(void approx_lines(int pts[][2], int pts_num, float epsilon, int lines[][2], int *lines_num)) {
    assert(pts);
    assert(epsilon > 0);

    int dx = pts[pts_num - 1][0] - pts[0][0];
    int dy = pts[pts_num - 1][1] - pts[0][1];
    float nx = -dy / sqrtf(dx * dx + dy * dy);
    float ny = dx / sqrtf(dx * dx + dy * dy);
    float max_dist = 0, dist;
    int idx = -1;
    for (int i = 1; i < pts_num - 1; i++) {
        dist = fabs((pts[i][0] - pts[0][0]) * nx + (pts[i][1] - pts[0][1]) * ny);
        if (dist > max_dist) {
            max_dist = dist;
            idx = i;
        }
    }
    if (max_dist >= epsilon) {
        int num1 = *lines_num;
        approx_lines(pts, idx + 1, epsilon, lines, &num1);
        int num2 = *lines_num - num1 - 1;
        approx_lines(pts + idx, pts_num - idx, epsilon, lines + num1 - 1, &num2);
        *lines_num = num1 + num2 - 1;
    } else {
        lines[0][0] = pts[0][0];
        lines[0][1] = pts[0][1];
        lines[1][0] = pts[pts_num - 1][0];
        lines[1][1] = pts[pts_num - 1][1];
        *lines_num = 2;
    }
}

// float类型的折线段近似
AT_ITCM_SECTION_INIT(void approx_lines_f(float pts[][2], int pts_num, float epsilon, float lines[][2], int *lines_num)) {
    assert(pts);
    assert(epsilon > 0);

    int dx = pts[pts_num - 1][0] - pts[0][0];
    int dy = pts[pts_num - 1][1] - pts[0][1];
    float nx = -dy / sqrtf(dx * dx + dy * dy);
    float ny = dx / sqrtf(dx * dx + dy * dy);
    float max_dist = 0, dist;
    int idx = -1;
    for (int i = 1; i < pts_num - 1; i++) {
        dist = fabs((pts[i][0] - pts[0][0]) * nx + (pts[i][1] - pts[0][1]) * ny);
        if (dist > max_dist) {
            max_dist = dist;
            idx = i;
        }
    }
    if (max_dist >= epsilon && *lines_num > 2) {
        int num1 = *lines_num;
        approx_lines_f(pts, idx + 1, epsilon, lines, &num1);
        int num2 = *lines_num - num1 - 1;
        approx_lines_f(pts + idx, pts_num - idx, epsilon, lines + num1 - 1, &num2);
        *lines_num = num1 + num2 - 1;
    } else {
        lines[0][0] = pts[0][0];
        lines[0][1] = pts[0][1];
        lines[1][0] = pts[pts_num - 1][0];
        lines[1][1] = pts[pts_num - 1][1];
        *lines_num = 2;
    }
}

AT_ITCM_SECTION_INIT(void draw_line(image_t *img, int pt0[2], int pt1[2], uint8_t value)) {
    int dx = pt1[0] - pt0[0];
    int dy = pt1[1] - pt0[1];
    if (abs(dx) > abs(dy)) {
        for (int x = pt0[0]; x != pt1[0]; x += (dx > 0 ? 1 : -1)) {
            int y = pt0[1] + (x - pt0[0]) * dy / dx;
                    AT(img, clip(x, 0, img->width - 1), clip(y, 0, img->height - 1)) = value;
        }
    } else {
        for (int y = pt0[1]; y != pt1[1]; y += (dy > 0 ? 1 : -1)) {
            int x = pt0[0] + (y - pt0[1]) * dx / dy;
                    AT(img, clip(x, 0, img->width - 1), clip(y, 0, img->height - 1)) = value;
        }
    }
}

// 计算大津阈值
AT_ITCM_SECTION_INIT(uint16_t getOSTUThreshold(image_t *img, uint8_t MinThreshold, uint8_t MaxThreshold)) {
    uint8_t Histogram[256];
    uint16_t OUSTThreshold = 0;
    uint32_t PixelAmount = 0, Value_Sum = 0;
    uint64_t sigma = 0, maxSigma = 0;
    float w1 = 0, w2 = 0;
    int32_t u1 = 0, u2 = 0;
    uint8_t MinValue = 0, MaxValue = 255;

    //各像素点个数
    uint8_t *ptr = img->data;
    uint8_t *ptrEnd = img->data + img->width * img->height;
    while (ptr != ptrEnd) {
        ++Histogram[*ptr++];
    }

    for (uint8_t m = 0; m < 100; m++) {

        Histogram[m] = 0;
    }

    for (MinValue = 0; Histogram[MinValue] == 0 && MinValue < 255; ++MinValue);
    for (MaxValue = 255; Histogram[MaxValue] == 0 && MaxValue > 0; --MaxValue);

    if (MaxValue == MinValue) return MaxValue;         // 只有一个颜色
    if (MinValue + 1 == MaxValue) return MinValue;        // 只有二个颜色

    if (MinValue < MinThreshold) {
        MinValue = MinThreshold;
    }
    if (MaxValue > MaxThreshold) {
        MaxValue = MaxThreshold;
    }

    uint32_t Pixel_Integral[256] = {0};   //像素积分 
    uint32_t Value_Integral[256] = {0};    //灰度积分
    for (uint8_t i = MinValue; i <= MaxValue; ++i) {
        PixelAmount += Histogram[i];      //像素总数
        Value_Sum += Histogram[i] * i;     //灰度总和
        Pixel_Integral[i] = PixelAmount;
        Value_Integral[i] = Value_Sum;
    }
    for (uint8_t i = MinValue; i < MaxValue + 1; ++i) {
        w1 = (float) Pixel_Integral[i] / PixelAmount;  //前景像素点比例
        w2 = 1 - w1;                               //背景比例
        u1 = (int32_t) (Value_Integral[i] / w1);                   //前景平均灰度
        u2 = (int32_t) ((Value_Sum - Value_Integral[i]) / w2);      //背景平均灰度
        sigma = (uint64_t) (w1 * w2 * (u1 - u2) * (u1 - u2));
        if (sigma >= maxSigma) {
            maxSigma = sigma;
            OUSTThreshold = i;
        } else {
            break;
        }
    }
    return OUSTThreshold;
}

// 点集三角滤波
AT_ITCM_SECTION_INIT(void blur_points(float pts_in[][2], int num, float pts_out[][2], int kernel)) {
    assert(kernel % 2 == 1);
    int half = kernel / 2;
    for (int i = 0; i < num; i++) {
        pts_out[i][0] = pts_out[i][1] = 0;
        for (int j = -half; j <= half; j++) {
            pts_out[i][0] += pts_in[clip(i + j, 0, num - 1)][0] * (half + 1 - abs(j));
            pts_out[i][1] += pts_in[clip(i + j, 0, num - 1)][1] * (half + 1 - abs(j));
        }
        pts_out[i][0] /= (2 * half + 2) * (half + 1) / 2;
        pts_out[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
}

// 点集等距采样  使走过的采样前折线段的距离为`dist`
AT_ITCM_SECTION_INIT(void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist)){
    int remain = 0, len = 0;
    for(int i=0; i<num1-1 && len < *num2; i++){
        float x0 = pts_in[i][0];
        float y0 = pts_in[i][1];
        float dx = pts_in[i+1][0] - x0;
        float dy = pts_in[i+1][1] - y0;
        float dn = sqrt(dx*dx+dy*dy);
        dx /= dn;
        dy /= dn;

        while(remain < dn && len < *num2){
            x0 += dx * remain;
            pts_out[len][0] = x0;
            y0 += dy * remain;
            pts_out[len][1] = y0;
            
            len++;
            dn -= remain;
            remain = dist;
        }
        remain -= dn;
    }
    *num2 = len;
}

// 点集等距采样2  使采样后点与点的距离为`dist`
// TODO: fix bug
AT_ITCM_SECTION_INIT(void resample_points2(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist)) {
    if (num1 < 0) {
        *num2 = 0;
        return;
    }
    pts_out[0][0] = pts_in[0][0];
    pts_out[0][1] = pts_in[0][1];
    int len = 1;
    for (int i = 0; i < num1 - 1 && len < *num2; i++) {
        float x0 = pts_in[i][0];
        float y0 = pts_in[i][1];
        float x1 = pts_in[i + 1][0];
        float y1 = pts_in[i + 1][1];

        do {
            float x = pts_out[len - 1][0];
            float y = pts_out[len - 1][1];

            float dx0 = x0 - x;
            float dy0 = y0 - y;
            float dx1 = x1 - x;
            float dy1 = y1 - y;

            float dist0 = sqrt(dx0 * dx0 + dy0 * dy0);
            float dist1 = sqrt(dx1 * dx1 + dy1 * dy1);

            float r0 = (dist1 - dist) / (dist1 - dist0);
            float r1 = 1 - r0;

            if (r0 < 0 || r1 < 0) break;
            x0 = x0 * r0 + x1 * r1;
            y0 = y0 * r0 + y1 * r1;
            pts_out[len][0] = x0;
            pts_out[len][1] = y0;
            len++;
        } while (len < *num2);

    }
    *num2 = len;
}

// 点集局部角度变化率
AT_ITCM_SECTION_INIT(void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist)) {
    for (int i = 0; i < num; i++) {
        if (i <= 0 || i >= num - 1) {
            angle_out[i] = 0;
            continue;
        }
        float dx1 = pts_in[i][0] - pts_in[clip(i - dist, 0, num - 1)][0];
        float dy1 = pts_in[i][1] - pts_in[clip(i - dist, 0, num - 1)][1];
        float dn1 = sqrtf(dx1 * dx1 + dy1 * dy1);
        float dx2 = pts_in[clip(i + dist, 0, num - 1)][0] - pts_in[i][0];
        float dy2 = pts_in[clip(i + dist, 0, num - 1)][1] - pts_in[i][1];
        float dn2 = sqrtf(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        angle_out[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
}

// 角度变化率非极大抑制
AT_ITCM_SECTION_INIT(void nms_angle(float angle_in[], int num, float angle_out[], int kernel)) {
    assert(kernel % 2 == 1);
    int half = kernel / 2;
    for (int i = 0; i < num; i++) {
        angle_out[i] = angle_in[i];
        for (int j = -half; j <= half; j++) {
            if (fabs(angle_in[clip(i + j, 0, num - 1)]) > fabs(angle_out[i])) {
                angle_out[i] = 0;
                break;
            }
        }
    }
}

// 左边线跟踪中线
AT_ITCM_SECTION_INIT(void track_leftline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist)) {
    for (int i = 0; i < num; i++) {
        float dx = pts_in[clip(i + approx_num, 0, num - 1)][0] - pts_in[clip(i - approx_num, 0, num - 1)][0];
        float dy = pts_in[clip(i + approx_num, 0, num - 1)][1] - pts_in[clip(i - approx_num, 0, num - 1)][1];
        float dn = sqrt(dx * dx + dy * dy);
        dx /= dn;
        dy /= dn;
        pts_out[i][0] = pts_in[i][0] - dy * dist;
        pts_out[i][1] = pts_in[i][1] + dx * dist;
    }
}

// 右边线跟踪中线
AT_ITCM_SECTION_INIT(void track_rightline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist)) {
    for (int i = 0; i < num; i++) {
        float dx = pts_in[clip(i + approx_num, 0, num - 1)][0] - pts_in[clip(i - approx_num, 0, num - 1)][0];
        float dy = pts_in[clip(i + approx_num, 0, num - 1)][1] - pts_in[clip(i - approx_num, 0, num - 1)][1];
        float dn = sqrt(dx * dx + dy * dy);
        dx /= dn;
        dy /= dn;
        pts_out[i][0] = pts_in[i][0] + dy * dist;
        pts_out[i][1] = pts_in[i][1] - dx * dist;
    }
}

void draw_x(image_t *img, int x, int y, int len, uint8_t value) {
    for (int i = -len; i <= len; i++) {
                AT(img, clip(x + i, 0, img->width - 1), clip(y + i, 0, img->height - 1)) = value;
                AT(img, clip(x - i, 0, img->width - 1), clip(y + i, 0, img->height - 1)) = value;
    }
}

void draw_o(image_t *img, int x, int y, int radius, uint8_t value) {
    for (float i = -PI; i <= PI; i += PI / 10) {
                AT(img, clip(x + radius * cosf(i), 0, img->width - 1), clip(y + radius * sinf(i), 0, img->height - 1)) = value;
    }
}


//大津法获得阈值，取自韬子的函数
unsigned char original_image[image_h][image_w];
unsigned char image_thereshold;//图像分割阈值
unsigned char otsuThreshold(unsigned char *image, unsigned short col, unsigned short row)
{
    #define GrayScale 256
    unsigned short width = col;
    unsigned short height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height;
    unsigned char threshold = 0;
    unsigned char* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素在整幅图像中的比例
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else   //前景部分
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = (unsigned char)i;
        }
    }

    return threshold;
}



unsigned char bin_image[image_h_bin][image_w_bin];//图像数组
void turn_to_bin(void)
{
  unsigned char i,j;
 image_thereshold = otsuThreshold(original_image[0], image_w, image_h) + 80;
//	while(1){tft180_show_int(0,0,image_thereshold,3);}
  for(i = 0;i<image_h_bin;i++)
  {
      for(j = 0;j<image_w_bin;j++)
      {
          if(original_image[i][j]>image_thereshold)bin_image[i][j] = white_pixel;
          else bin_image[i][j] = black_pixel;
      }
  }
}




void Get_image(unsigned char(*mt9v03x_image)[image_w])
{
#define use_num		2	//1就是不压缩，2就是压缩一倍	
	unsigned char i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)          //
    {
        for (j = 0; j <image_w; j += use_num)     //
        {
            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写你的摄像头采集到的图像
			line++;
        }
        line = 0;
        row++;
    }
}



/*
函数名称：void search_l_r(unsigned short break_flag, unsigned char(*image)[image_w],unsigned short *l_stastic, unsigned short *r_stastic,
							unsigned char l_start_x, unsigned char l_start_y, unsigned char r_start_x, unsigned char r_start_y,unsigned char*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]		：需要进行找点的图像数组，必须是二值图,填入数组名称即可
					   特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic				：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic				：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x				：左边起点横坐标
l_start_y				：左边起点纵坐标
r_start_x				：右边起点横坐标
r_start_y				：右边起点纵坐标
hightest				：循环结束所得到的最高高度
函数返回：无
修改时间：2022年9月25日
备    注：
example：
	search_l_r((unsigned short)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
				start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
// */
#define USE_num	image_h*3	//定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点

 //存放点的x，y坐标
unsigned short points_l[(unsigned short)USE_num][2] = { {  0 } };//左线
unsigned short points_r[(unsigned short)USE_num][2] = { {  0 } };//右线
unsigned short dir_r[(unsigned short)USE_num] = { 0 };//用来存储右边生长方向
unsigned short dir_l[(unsigned short)USE_num] = { 0 };//用来存储左边生长方向
unsigned short data_stastics_l = 0;//统计左边找到点的个数
unsigned short data_stastics_r = 0;//统计右边找到点的个数
unsigned char hightest = 0;//最高点
void search_l_r(unsigned short break_flag, unsigned char(*image)[image_w], unsigned short *l_stastic, unsigned short *r_stastic, unsigned char l_start_x, unsigned char l_start_y, unsigned char r_start_x, unsigned char r_start_y, unsigned char*hightest)
{

	unsigned char i = 0, j = 0;

	//左边变量
	unsigned char search_filds_l[8][2] = { {  0 } };
	unsigned char index_l = 0;
	unsigned char temp_l[8][2] = { {  0 } };
	unsigned char center_point_l[2] = {  0 };
	unsigned short l_data_statics;//统计左边
	//定义八个邻域
	static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是顺时针

	//右边变量
	unsigned char search_filds_r[8][2] = { {  0 } };
	unsigned char center_point_r[2] = { 0 };//中心坐标点
	unsigned char index_r = 0;//索引下标
	unsigned char temp_r[8][2] = { {  0 } };
	unsigned short r_data_statics;//统计右边
	//定义八个邻域
	static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是逆时针

	l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
	r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

	//第一次更新坐标点  将找到的起点值传进来
	center_point_l[0] = l_start_x;//x
	center_point_l[1] = l_start_y;//y
	center_point_r[0] = r_start_x;//x
	center_point_r[1] = r_start_y;//y

		//开启邻域循环
	while (break_flag--)
	{

		//左边
		for (i = 0; i < 8; i++)//传递8F坐标
		{
			search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
			search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
		}
		//中心坐标点填充到已经找到的点内
		points_l[l_data_statics][0] = center_point_l[0];//x
		points_l[l_data_statics][1] = center_point_l[1];//y
		l_data_statics++;//索引加一

		//右边
		for (i = 0; i < 8; i++)//传递8F坐标
		{
			search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
			search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
		}
		//中心坐标点填充到已经找到的点内
		points_r[r_data_statics][0] = center_point_r[0];//x
		points_r[r_data_statics][1] = center_point_r[1];//y

		index_l = 0;//先清零，后使用
		for (i = 0; i < 8; i++)
		{
			temp_l[i][0] = 0;//先清零，后使用
			temp_l[i][1] = 0;//先清零，后使用
		}

		//左边判断
		for (i = 0; i < 8; i++)
		{
			if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
				&& image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
			{
				temp_l[index_l][0] = search_filds_l[(i)][0];
				temp_l[index_l][1] = search_filds_l[(i)][1];
				index_l++;
				dir_l[l_data_statics - 1] = (i);//记录生长方向
			}

			if (index_l)
			{
				//更新坐标点
				center_point_l[0] = temp_l[0][0];//x
				center_point_l[1] = temp_l[0][1];//y
				for (j = 0; j < index_l; j++)
				{
					if (center_point_l[1] > temp_l[j][1])
					{
						center_point_l[0] = temp_l[j][0];//x
						center_point_l[1] = temp_l[j][1];//y
					}
				}
			}

		}
		if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
			&& points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
			||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
				&& points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
		{
			//printf("三次进入同一个点，退出\n");
			break;
		}
		if (abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
			&& abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
			)
		{
			//printf("\n左右相遇退出\n");	
			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
			//printf("\n在y=%d处退出\n",*hightest);
			break;
		}
		if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
		{
			// printf("\n如果左边比右边高了，左边等待右边\n");	
			continue;//如果左边比右边高了，左边等待右边
		}
		if (dir_l[l_data_statics - 1] == 7
			&& (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
		{
			//printf("\n左边开始向下了，等待右边，等待中... \n");
			center_point_l[0] = points_l[l_data_statics - 1][0];//x
			center_point_l[1] = points_l[l_data_statics - 1][1];//y
			l_data_statics--;
		}
		r_data_statics++;//索引加一

		index_r = 0;//先清零，后使用
		for (i = 0; i < 8; i++)
		{
			temp_r[i][0] = 0;//先清零，后使用
			temp_r[i][1] = 0;//先清零，后使用
		}

		//右边判断
		for (i = 0; i < 8; i++)
		{
			if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
				&& image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
			{
				temp_r[index_r][0] = search_filds_r[(i)][0];
				temp_r[index_r][1] = search_filds_r[(i)][1];
				index_r++;//索引加一
				dir_r[r_data_statics - 1] = (i);//记录生长方向	
				//printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
			}
			if (index_r)
			{

				//更新坐标点
				center_point_r[0] = temp_r[0][0];//x
				center_point_r[1] = temp_r[0][1];//y
				for (j = 0; j < index_r; j++)
				{
					if (center_point_r[1] > temp_r[j][1])
					{
						center_point_r[0] = temp_r[j][0];//x
						center_point_r[1] = temp_r[j][1];//y
					}
				}

			}
		}


	}


	//取出循环次数
	*l_stastic = l_data_statics;
	*r_stastic = r_data_statics;

}

/********************sobel算子检测边缘返回********************************/

//state:testing（try the best choose）

//instance:

/**************************************************************/
void sobel_deal(unsigned char(*target_image)[image_w])
{
	
	uint8 line_first_x, line_middle_x, line_end_x;
	uint8 line_first_y, line_middle_y, line_end_y;
	
	uint8 sobel_core_gx[3][3] = { -1, 0, 1,
															  -2, 0, 2,	
															  -1 ,0 ,1};  
	
	uint8 sobel_core_gy[3][3] = {-1,-2,-1,
															  0, 0, 0,	
															  1 ,2 ,1};  //定义3*3sobel卷积核
	
//池化原图像，扩展图像大小
char Pooling_image[image_h + 1][image_w + 1];
int16 deal_image_x[image_h][image_w];
int16 deal_image_y[image_h][image_w];						//定义卷积后的x梯度图像与y梯度图像												
													
//计算x方向梯度，计算y方向梯度
	for(int pool_y = 0; pool_y < image_w + 1;pool_y++)
	{
		for(int pool_x = 0;pool_x < image_h + 1;pool_x++)
		{
			line_first_x   =     Pooling_image[pool_y][pool_x + 2]*sobel_core_gx[0][2] - Pooling_image[pool_y][pool_x]*sobel_core_gx[0][0];
			line_middle_x  =     Pooling_image[pool_y+1][pool_x + 2]*sobel_core_gx[0][2] - Pooling_image[pool_y+1][pool_x]*sobel_core_gx[0][0];
			line_end_x     =     Pooling_image[pool_y+2][pool_x + 2]*sobel_core_gx[0][2] - Pooling_image[pool_y+2][pool_x]*sobel_core_gx[0][0];
			
			
			line_first_y   =     Pooling_image[pool_y][pool_x + 2]*sobel_core_gy[0][2] - Pooling_image[pool_y][pool_x]*sobel_core_gy[0][0];
			line_middle_y  =     Pooling_image[pool_y+1][pool_x + 2]*sobel_core_gy[0][2] - Pooling_image[pool_y+1][pool_x]*sobel_core_gy[0][0];
			line_end_y     =     Pooling_image[pool_y+2][pool_x + 2]*sobel_core_gy[0][2] - Pooling_image[pool_y+2][pool_x]*sobel_core_gy[0][0];
			
		if(pool_x < image_h && pool_y < image_w){	
			deal_image_x[pool_y][pool_x] = line_first_x + line_middle_x + line_end_x;
			
			deal_image_y[pool_y][pool_x] = line_first_y + line_middle_y + line_end_y;
			
			
				if(deal_image_x[pool_y][pool_x] > 255)
				{
					deal_image_x[pool_y][pool_x] = deal_image_x[pool_y][pool_x] - 255;
					
				}else if(deal_image_x[pool_y][pool_x] < 0)
				{
					deal_image_x[pool_y][pool_x] = - deal_image_x[pool_y][pool_x];                 //x梯度图像越界影像处理
				}else{
					deal_image_x[pool_y][pool_x] = 0;   	
				}
				
				if(deal_image_y[pool_y][pool_x] > 255)
				{
					deal_image_y[pool_y][pool_x] = deal_image_y[pool_y][pool_x] - 255;
					
				}else if(deal_image_y[pool_y][pool_x] < 0)
				{
					deal_image_y[pool_y][pool_x] = - deal_image_y[pool_y][pool_x];                 //y梯度图像越界影像处理
				}else{
					deal_image_y[pool_y][pool_x] = 0;   	
				}
			}
		}
	}															 																																				 
														 
  //计算总梯度													 
//	g = abs(gx) + abs(gy);//简化总梯度用法（精确算法：g = sqrt((gx^2) + sqrt(gy^2)）
	for(int pool_y = 0; pool_y < image_w ;pool_y++)
	{
		for(int pool_x = 0;pool_x < image_h ;pool_x++)
		{
			assert(deal_image_x[pool_y][pool_x] ==255&&deal_image_y[pool_y][pool_x] == 255);                            //判断是否存在x，y梯度共同极大的情况
			Pooling_image[pool_y][pool_x] = sqrt(deal_image_x[pool_y][pool_x]^2 + deal_image_y[pool_y][pool_x]^2);
			
			
		} 
	}
}

/********************FAST非极大值抑制找角点********************************/

//state:testing（try the best choose）

//instance:

/************************************************************************/
uint8 concern_point_x[300] = {0};
uint8 concern_point_y[300] = {0};
//uint8 point_addtion = 0;

void fast_find_angular_point(unsigned char(*target_image)[image_w_bin])
{

	uint8 count = 0;
	

	uint8 point_filter[image_h_bin][image_w_bin] = {0};
	uint8 preliminary_sign_point[image_h_bin][image_w_bin] = {0};
	
	uint8  max_point = 0;
	uint8 last_max_x = 0,last_max_y = 0;
	for(int y = 0;y <image_h_bin;y++)
	{
		for(int x = 0;x < image_w_bin; x++)
		{
			
			if(target_image[y][x] == 255)
			{
				
					count = 0;
			
					(target_image[y+3][x] == 0) ?(count++) :(count = count);
					(target_image[y][x+3] == 0) ?(count++) :(count = count);
					(target_image[y-3][x] == 0) ?(count++) :(count = count);
					(target_image[y][x-3] == 0) ?(count++) :(count = count);
					
					if(count >=3)
					{
						preliminary_sign_point[y][x] = 1 ;
						
					}
				}
			
				
		}
	}                                                                       //以左上到右下的顺序遍历全部图像数组，预筛选角点位置，将位置标志在preliminary_sign_point数组中
	
	for(int y = 0;y <image_h_bin;y++)
	{
		for(int x = 0;x < image_w_bin; x++)
		{
			if(preliminary_sign_point[y][x] == 1)                              
			{
					count = 0;
				
					(target_image[y+3][x] == 0) ?(count++) :(count = count);
					(target_image[y][x+3] == 0) ?(count++) :(count = count);
					(target_image[y-3][x] == 0) ?(count++) :(count = count);
					(target_image[y][x-3] == 0) ?(count++) :(count = count);
					
					(target_image[y+3][x+1] == 0) ?(count++) :(count = count);
					(target_image[y+1][x+3] == 0) ?(count++) :(count = count);
					(target_image[y-3][x-1] == 0) ?(count++) :(count = count);
					(target_image[y-1][x-3] == 0) ?(count++) :(count = count);
					
					(target_image[y+3][x-1] == 0) ?(count++) :(count = count);
					(target_image[y-1][x+3] == 0) ?(count++) :(count = count);
					(target_image[y-3][x+1] == 0) ?(count++) :(count = count);
					(target_image[y+1][x-3] == 0) ?(count++) :(count = count);
					
					(target_image[y+2][x+2] == 0) ?(count++) :(count = count);
					(target_image[y-2][x+2] == 0) ?(count++) :(count = count);
					(target_image[y-2][x-2] == 0) ?(count++) :(count = count);
					(target_image[y+2][x-2] == 0) ?(count++) :(count = count);
				
				if(count >=9)
					{
						preliminary_sign_point[y][x] = 1 ;
						point_filter[y][x] = count;
					}else{
						preliminary_sign_point[y][x] = 0;
					}
			}
		}
	}                                                                    //同样的顺序遍历预筛选角点数组，对预角点进行更多处理，读取预角点判断权值存放在point_filter数组中
	
	for(int y = 0;y <image_h_bin-4;y++)
	{
		for(int x = 0;x < image_w_bin-4; x++)
		{
			if(preliminary_sign_point[y][x] != 0)
			{
				for(int i = 0;i<3;i++)
				{
					for(int j = 0;j<3;j++)
					{
						if(point_filter[y+i][x+j] > max_point)
						{
							point_filter[last_max_y][last_max_x] = 0;
							preliminary_sign_point[last_max_y][last_max_x] = 0;
							
							last_max_x = x + j;
							last_max_y = y + i;
							max_point = point_filter[y+i][x+j];
							
							
						}else{
							point_filter[y+i][x+j] = 0;
							
						}
					}
					if(i == 2){
					concern_point_x[point_addtion] = last_max_x;
					concern_point_y[point_addtion] = last_max_y;
					point_addtion++;
					last_max_x = last_max_y = max_point = 0;
					}
				}
			}
		}
	}
}



void deal_image()
{
	uint8 x,y;
	Get_image(&mt9v03x_image[0]);

	turn_to_bin();
//	sobel_deal(&mt9v03x_image[0]);
//	for(int h=0;h<188;h++)
//	{
//		for(int w = 0;w<70;w++)
//		{
//			bin_image[w][h] = black_pixel;
//		}
//	}
	
		for(int h=0;h<94;h++)
	{
		for(int w = 0;w<60;w++)
		{
			if(w==0||h==0||w==60||h==94)
			bin_image[w][h] = black_pixel;
		}
	}
	fast_find_angular_point(&bin_image[0]);
	for(int i=0;i<point_addtion;i++)	
	{
		
		x = concern_point_x[point_addtion];
		y = concern_point_y[point_addtion];
//		if(x > 98)
//			x = 98;
//		if(y > 60)
//			y = 60;
		
		bin_image[y][x] = 155;
		bin_image[y+1][x] = 155;
		bin_image[y-1][x] = 155;
		bin_image[y][x+1] = 155;
		bin_image[y][x-1] = 155;
		bin_image[y+1][x+1] = 155;
		bin_image[y-1][x-1] = 155;
			  
//		tft180_show_int(80,16*6,x,3);
//		tft180_sh.ow_int(80,16*7,y,3);
////		tft180_draw_point(x,y,RGB565_RED);
		
	}
	
}