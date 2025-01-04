//
// Created by chenkaiyi on 2024/4/26.
//

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/cdefs.h>

#pragma once
__BEGIN_DECLS

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

//前置声明，用于隐藏结构体细节
typedef struct MapVisitor MapVisitor_t;

MapVisitor_t *map_visitor_create(pid_t pid);

bool map_visitor_valid(MapVisitor_t *visitor);

bool map_visitor_reset(MapVisitor_t *visitor);

bool map_visitor_has_next(MapVisitor_t *visitor);

MapItem* map_visitor_next(MapVisitor_t *visitor, MapItem *mapItem);

int map_visitor_destroy(MapVisitor_t *visitor);

__END_DECLS
