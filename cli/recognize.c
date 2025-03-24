#include <num_recognizer.h>
#include <stdio.h>

//! @brief 主函数
//! @param argc 命令行参数个数
//! @param argv 命令行参数值数组
//! @return 返回码，0表示正常退出
int main(int argc, const char **argv){
    if(argc != 3){
        printf("Usage: recognize mnist.onnx 3.png\n");
        return -1;
    }
    int ret = 0;
    int result = -1;
    Recognizer *recog;
    num_recognizer_init();
    num_recognizer_create(argv[1], &recog);
    if(ret = num_recognizer_recognize_png(recog, argv[2], &result)){
        printf("Failed to recognize\n");
    } else {
        printf("result:%d\n", result);
    }
    num_recognizer_delete(recog);
    return ret;
}