




































# 调⽤libpng库⾃带的配置⽂件来查找软件包，其⾃带配置⽂件会创建两个导⼊库⽬标：
# 1. 动态库导⼊⽬标png_shared
# 2. 静态库导⼊⽬标png_static
find_package(libpng CONFIG CONFIGS libpng16.cmake)
# 若成功查找，为两个库⽬标补上缺失的头⽂件⽬录属性
if(libpng_FOUND)
    # 获取png动态库导⼊⽬标对应动态库⽂件的路径，⾸先尝试其IMPORTED_LOCATION属性
    get_target_property(libpng_LIBRARY png_shared IMPORTED_LOCATION)
    # 若未能获取动态库文件路径，再尝试其IMPORTED_LOCATION_RELEASE属性
    if(NOT libpng_LIBRARY)
        get_target_property(libpng_LIBRARY png_shared IMPORTED_LOCATION_RELEASE)
    endif()
    # 根据png动态库的路径，设置libpng的根⽬录
    set(_png_root "${libpng_LIBRARY}/../..")
    #查找png.h头文件所在目录的路径
    find_path(libpng_INCLUDE_DIR png.h
        HINTS ${_png_root}
        PATH_SUFFIXES include)
    # 为png_shared和png_static导⼊库⽬标设置头⽂件⽬录属性
    target_include_directories(png_shared INTERFACE ${libpng_INCLUDE_DIR})
    target_include_directories(png_static INTERFACE ${libpng_INCLUDE_DIR})
endif()
include(FindPackageHandleStandardArgs)
# 检查变量是否有效以及配置文件是否成功执行
find_package_handle_standard_args(libpng
    REQUIRED_VARS libpng_LIBRARY libpng_INCLUDE_DIR
    CONFIG_MODE
)
# 若一切成功， 设置结果变量
if(libpng_FOUND)
    set(libpng_INCLUDE_DIRS ${libpng_INCLUDE_DIR})
    set(libpng_LIBRARYS ${libpng_LIBRARY})
endif()