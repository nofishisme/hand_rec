# hand_rec
手写数字识别项目

手写识别模型下载网址

```
https://github.com/onnx/models/tree/main/validated/vision/classification/mnist
```

**依赖：zlib、png库**

安装zlib（一般linux预装了zlib，可以先安装png库，看看是否能成功）

```shell
https://github.com/madler/zlib

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cmake --install . #需要管理员权限
```

安装png

```shell
https://github.com/pnggroup/libpng/releases/tag/v1.6.40

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cmake --install . #需要管理员权限
```

**使用**

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```

**测试用例**

```shell
#进入build文件夹
./recognize ../models/model/model.onnx ../picture/number_2.png
```

