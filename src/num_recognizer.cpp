#include <array>
#include <onnxruntime_cxx_api.h>
#include <algorithm>
#include <png.h>
#include "num_recognizer.h"







#ifdef __WIN32
    //在Windows操作系统中，我们需要使⽤WindowsAPI来帮助完成constchar*到const
    //wchar_t*的编码转换。因此需要引⽤Windows.h。
    #include <Windows.h>
#endif


static const char* INPUT_NAMES[] = {"Input3"};//模型输入参数名
static const char* OUTPUT_NAMES[] = {"Plus214_Output_0"};//模型输出参数名
static constexpr int64_t INPUT_WIDTH = 28;//模型输入图片宽度
static constexpr int64_t INPUT_HEIGHT = 28;//模型输入图片高度

static const std::array<int64_t, 4> input_shape{
    1, 1, INPUT_WIDTH, INPUT_HEIGHT};//输入图片的形状
static const std::array<int64_t, 2> output_shape{
    1, 10};//输出数据的形状
static Ort::Env env{nullptr};
static Ort::MemoryInfo memory_info{nullptr};//onnxruntime内存信息

//! @brief 手写数字识别类
struct Recognizer{
    //! @brief onnxruntime会话
    Ort::Session session;
};

//! @brief 将byte类型的颜⾊值转换为模型接受的⼆值化后的float类型数值
//! @param b byte类型的颜⾊值
//! @return 模型接受的⼆值化后的float类型值，0代表⽩⾊，1代表⿊⾊。
static float byte2float(png_byte b){return b < 128 ? 1:0;}
//! @brief 获取PNG图⽚指定像素的⼆值化后的float类型颜⾊值
//! @param x 像素横坐标
//! @param y 像素纵坐标
//! @param png_width 图⽚宽度
//! @param png_height 图⽚⾼度
//! @param color_type 图⽚颜⾊类型
//! @param png_data 图⽚数据
//! @return 对应像素的⼆值化后的float类型颜⾊值
static float get_float_color_in_png(uint32_t x, uint32_t y, png_uint_32 png_width, png_uint_32 png_height, png_byte color_type, png_bytepp png_data){
    if(x >= png_width || y >= png_height)
        return 0.0f;
    switch (color_type)
    {
    case PNG_COLOR_TYPE_RGB:{
        auto p = png_data[y] + x * 3;
        return byte2float((p[0] + p[1] + p[2]) / 3);
    }
        break;
    case PNG_COLOR_TYPE_RGBA:{
        auto p = png_data[y] + x * 4 ;
        return byte2float((p[0] + p[1] + p[2]) / 3);
    }
        break;
    default:
        break;
    }
    return 0.0f;
}

extern "C"{
    void num_recognizer_init(){
        env = Ort::Env{static_cast<const OrtThreadingOptions *>(nullptr)};
        memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    }

    void num_recognizer_create(const char* model_path,
        Recognizer** out_recognizer){
        Ort::Session session{nullptr};
    #if _WIN32
        //Windows中，onnxruntime的Session接受模型⽂件路径时需使⽤const
    //wchar_t*，即宽字符串。因此在这⾥做⼀下转换。
        wchar_t wpath[256];
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, model_path, -1, wpath, 256);
        session = Ort::Session(env, wpath, Ort::SessionOptions(nullptr));
    #else
        session = Ort::Session(env, model_path, Ort::SessionOptions(nullptr));
    #endif 
        *out_recognizer = new Recognizer{std::move(session)};
    }

    void num_recognizer_delete(Recognizer* recognizer) {
        delete recognizer;
    }

    int num_recognizer_recognize(Recognizer* recognizer, float* input_image, int* result){
        std::array<float, 10> results{};
        auto input_tensor = Ort::Value::CreateTensor<float>(memory_info, input_image, INPUT_WIDTH * INPUT_HEIGHT,input_shape.data(), input_shape.size());
        auto output_tensor = Ort::Value::CreateTensor<float>(memory_info, results.data(), results.size(), output_shape.data(), output_shape.size());
        recognizer->session.Run(Ort::RunOptions{nullptr}, INPUT_NAMES, &input_tensor, 1, OUTPUT_NAMES, &output_tensor, 1);
        *result = static_cast<int>(std::distance(results.begin(), std::max_element(results.begin(), results.end())));
        for(int i = 0; i < results.size(); ++i){
            printf("result[%d]:%.3f\n", i, results[i]);
        }
        return 0;
    }

    int num_recognizer_recognize_png(Recognizer *recognizer, const char* png_path, int *result){
        int ret = 0;
        std::array<float, INPUT_WIDTH * INPUT_HEIGHT> input_image;
        FILE* fp;
        u_int8_t header[8];
        png_structp png_ptr;
        png_infop info_ptr;
        png_uint_32 png_width, png_height;
        png_byte color_type;
        png_bytep* png_data;
        //打开png图片
        fp = fopen(png_path, "rb");
        if(!fp){
            ret = -2;
            goto exit3;
        }
        //读取PNG图片文件头
        fread(header, 1, 8, fp);
        if(png_sig_cmp(reinterpret_cast<u_int8_t*>(header), 0, 8)){
            ret = -3;
            goto exit2;
        }
        //创建png指针数据结构
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if(!png_ptr){
            ret = -4;
            goto exit2;
        }
        //创建PNG信息指针数据结构
        info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr){
            ret = -5;
            goto exit2;
        }
        //设置跳转处理异常
        if(setjmp(png_jmpbuf(png_ptr))){
            ret = -6;
            goto exit2;
        }
        //初始化png文件
        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);
        //读取PNG信息
        png_read_info(png_ptr, info_ptr);
        png_width = png_get_image_width(png_ptr, info_ptr);
        png_height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        //设置跳转处理异常
        if(setjmp(png_jmpbuf(png_ptr))){
            ret = -7;
            goto exit2;
        }
        //读取PNG的数据
        png_data = (png_bytep*)malloc(sizeof(png_bytep)* png_height);
        for(uint32_t y = 0; y < png_height; ++y){
            png_data[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
        png_read_image(png_ptr, png_data);
        for(int32_t y = 0; y < INPUT_HEIGHT; ++y){
            for(int32_t x = 0; x < INPUT_WIDTH; ++x){
                float res = 0;
                int n = 0;
                for(uint32_t png_y = y * png_height / INPUT_HEIGHT; png_y < (y + 1) * png_height / INPUT_HEIGHT; ++png_y){
                    for(uint32_t png_x = x * png_width / INPUT_WIDTH; png_x < (x + 1) * png_width / INPUT_WIDTH; ++png_x){
                        res += get_float_color_in_png(png_x, png_y, png_width, png_height, color_type, png_data);
                        ++n;
                    }
                }
                input_image[y * INPUT_HEIGHT + x] = res / n;
                printf("%.3f ", res / n);
            }
            printf("\n");
        }
        //识别图片数据中的手写数字
        ret = num_recognizer_recognize(recognizer, input_image.data(), result);
        exit1:
            for(uint32_t y = 0; y < png_height; ++y){
                free(png_data[y]);
            }
            free(png_data);
        exit2:
            //关闭文件
            fclose(fp);
        exit3:
            return ret;
    }
}