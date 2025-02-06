# MapsVisitor

**MapsVisitor**是一个Android 读取并逐行遍历`/proc/[pid]/maps`文件，进而获得每行**格式化**后数据的库，支持armeabi-v7a, arm64-v8a。



## 一、特征

- 使用c语言实现。
- 使用迭代器模式实现。
- 使用 MIT 许可证授权。



## 二、实现原理
[Android Hook - 解析proc/pid/maps文件](https://juejin.cn/post/7456851920617373731)




## 三、快速开始

你可以参考[app](https://github.com/835127729/MapsVisitor/tree/main/app)中的示例。

### 1、在 build.gradle.kts 中增加依赖

```Kotlin
android{
	buildFeatures {        
    //1、声明可以进行原生依赖，具体参考https://developer.android.com/build/native-dependencies
    prefab = true
  }
}

dependencies {    
  	//2、依赖最新版本
    implementation("com.github.835127729:MapsVisitor:1.0.2")
}
```



### 2、在 CMakeLists.txt增加依赖

```Makefile
...
find_package(maps_visitor REQUIRED CONFIG)
  
...
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        maps_visitor::maps_visitor
)
```



### 3、增加打包选项

如果你是在一个 SDK 工程里使用 MapsVisitor，你可能需要避免把`libmaps_visitor.so` 打包到你的 AAR 里，以免 app 工程打包时遇到重复的 `libmaps_visitor.so` 文件。

```Kotlin
android {
    packagingOptions {
        excludes += listOf(
            "**/libmaps_visitor.so",
        )
    }
}
```

另一方面, 如果你是在一个 APP 工程里使用 MapsVisitor，你可以需要增加一些选项，用来处理重复的 `libmaps_visitor.so` 文件引起的冲突。

```Kotlin
android {
    packagingOptions {
        pickFirsts += listOf(
            "**/libmaps_visitor.so",
        )
    }
}
```



### 4、使用

```C
//1、传入pid打开对应的proc/pid/maps文件，传入0默认为当前进程
MapsVisitor_t *visitor = maps_visitor_create(0);
//2、申请内存，用于存储每行解析后的数据
MapItem mapItem = {0};

//3、迭代器，判断是否有下一行
while (maps_visitor_has_next(visitor)) {
  //4、读取当前行的数据到MapItem
  maps_visitor_next(visitor, &mapItem);
}

//5、读取完成，关闭文件
maps_visitor_destroy(visitor);
```

其中`MapItem`的结构为：

```C
typedef struct MapItem {
    uintptr_t start_address;
    uintptr_t end_address;
    char permission[4];
    uintptr_t offset;
    dev_t major_dev;
    dev_t minor_dev;
    ino_t inode;
    char path[1024];
} MapItem;
```

另外有两个常用方法：

```C++
//判断proc/pid/maps文件是否打开成功
bool maps_visitor_valid(MapsVisitor_t *visitor);

//重置文件流，从而可以从头开始读取proc/pid/maps文件
bool maps_visitor_reset(MapsVisitor_t *visitor);
```





## 四、许可证

MapsVisitor 使用 [MIT 许可证](https://github.com/bytedance/bhook/blob/main/LICENSE) 授权。