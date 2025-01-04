#include <jni.h>
#include <string>
#include "maps_visitor.h"
#include <android/log.h>
#include <fstream>
#include <sstream>

#define LOGV(TAG, ...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__))
#define LOGI(TAG, ...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGD(TAG, ...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define LOGW(TAG, ...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define LOGE(TAG, ...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#define TAG "testMapVisitor"

extern "C" JNIEXPORT jboolean JNICALL
Java_com_muye_mapsvisitor_MainActivity_testMapVisitor(
        JNIEnv* env,
        jobject /* this */) {
    MapVisitor_t* mapVisitor = map_visitor_create(0);
    std::ifstream file("/proc/self/maps");

    if (map_visitor_valid(mapVisitor) != file.is_open()) {
        map_visitor_destroy(mapVisitor);
        return JNI_FALSE;
    }

    std::stringstream ss;
    std::string line;
    MapItem mapItem{0};
    while(map_visitor_has_next(mapVisitor)) {
        MapItem* item = map_visitor_next(mapVisitor, &mapItem);
        getline(file, line);
        if(item == nullptr) {
            return JNI_FALSE;
        }
        LOGD(TAG,
             "start_address = %x, end_address = %x, permission = %s, offset = %x, major_dev = %x, minor_dev = %x, inode = %lx, path = %s",
             mapItem.start_address, mapItem.end_address, mapItem.permission,
             mapItem.offset, mapItem.major_dev, mapItem.minor_dev, mapItem.inode,
             mapItem.path);
        ss.clear();
        ss.str(std::string());
        std::stringstream ss1;
        /**
         * 重新拼接maps和真实maps逐行比较
         */
        ss1 << std::hex << mapItem.start_address << "-" << std::hex << mapItem.end_address << " "
            << mapItem.permission << " "
            << std::setfill('0') << std::setw(8) << std::right << std::hex << mapItem.offset << " "
            << std::setfill('0') << std::setw(2) << std::right << std::hex << mapItem.major_dev
            << ":"
            << std::setfill('0') << std::setw(2) << std::right << std::hex << mapItem.minor_dev
            << " " << std::dec << mapItem.inode;
        if (strlen(mapItem.path) > 0) {
            ss << std::setfill(' ');
#if defined(__LP64__)
            ss << std::setw(72);
#else
            ss << std::setw(48);
#endif
            ss << std::left;
        }
        ss << ss1.str() << " " << mapItem.path;
        if (ss.str() != line) {
            LOGE(TAG, "不相同:");
            LOGI(TAG, "%s", line.c_str());
            LOGE(TAG, "%s", ss.str().c_str());
            map_visitor_destroy(mapVisitor);
            return JNI_FALSE;
        }
        //810d9000-81250000 r-xp 00000000 00:04 51499      /dev/ashmem/dalvik-jit-code-cache (deleted)
    }

    if(!map_visitor_reset(mapVisitor)){
        return JNI_FALSE;
    }
    return JNI_TRUE;
}