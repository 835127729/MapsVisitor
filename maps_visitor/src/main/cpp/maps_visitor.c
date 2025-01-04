//
// Created by chenkaiyi on 2024/4/26.
//

#include <stdio.h>
#include <inttypes.h>
#include <android/log.h>
#include <malloc.h>
#include <string.h>
#include "include/maps_visitor.h"

struct MapVisitor {
    //进程id
    pid_t pid;
    //maps文件
    FILE *fd;
    //通用buffer
    char buffer[1024];
};

MapVisitor_t *map_visitor_create(pid_t pid) {
    MapVisitor_t *visitor = (MapVisitor_t *) calloc(1, sizeof(MapVisitor_t));
    if (visitor == NULL) {
        return NULL;
    }
    if (pid == 0) {
        snprintf(visitor->buffer, sizeof(visitor->buffer), "/proc/self/maps");
    } else {
        int n = snprintf(visitor->buffer, sizeof(visitor->buffer), "/proc/%d/maps", visitor->pid);
        if (n >= sizeof(visitor->buffer)) {
            free(visitor);
            return NULL;
        }
    }
    //打开文件
    FILE *mapFd = fopen(visitor->buffer, "r");
    if (mapFd == NULL) {
        free(visitor);
        return NULL;
    }
    visitor->fd = mapFd;
    return visitor;
}

int map_visitor_destroy(MapVisitor_t *visitor) {
    if (visitor == NULL) {
        return 0;
    }
    if (visitor->fd == NULL) {
        free(visitor);
        return 0;
    }
    if (fclose(visitor->fd) == 0) {
        free(visitor);
        return 0;
    }
    return -1;
}

bool map_visitor_valid(MapVisitor_t *visitor) {
    return visitor != NULL && visitor->fd != NULL;
}

bool map_visitor_reset(MapVisitor_t *visitor) {
    if (!map_visitor_valid(visitor)) {
        return false;
    }
    rewind(visitor->fd);
    return true;
}

bool map_visitor_has_next(MapVisitor_t *visitor) {
    if (!map_visitor_valid(visitor)) {
        return false;
    }
    //读取下一行
    return fgets(visitor->buffer, sizeof(visitor->buffer), visitor->fd) != NULL;
}

MapItem* map_visitor_next(MapVisitor_t *visitor, MapItem *mapItem) {
    if (!map_visitor_valid(visitor)) {
        return NULL;
    }
    int pathPosition;
    sscanf(visitor->buffer,
           "%" PRIxPTR"-%" PRIxPTR" %4s %" PRIxPTR" %" PRIxPTR":%" PRIxPTR" %" PRIuMAX" %n",
           &mapItem->start_address, &mapItem->end_address, mapItem->permission, &mapItem->offset,
           &mapItem->major_dev, &mapItem->minor_dev, &mapItem->inode, &pathPosition);
    char *path = visitor->buffer + pathPosition;
    size_t len = strlen(path);
    if (len && path[len - 1] == '\n') {
        path[len - 1] = '\0';
    }
    if (len == 0) {
        mapItem->path[0] = '\0';
    } else {
        strncpy(mapItem->path, path, len);
    }
    return mapItem;
}