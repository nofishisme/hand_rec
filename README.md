# hand_rec
手写数字识别项目

手写识别模型下载网址

```
https://github.com/onnx/models/tree/main/validated/vision/classification/mnist
```

使用

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```

测试用例

```shell
#进入build文件夹
./recognize ../models/model/model.onnx ../picture/number_2.png
```

