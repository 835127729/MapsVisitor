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
typedef struct MapsVisitor MapsVisitor_t;

MapsVisitor_t *maps_visitor_create(pid_t pid);

bool maps_visitor_valid(MapsVisitor_t *visitor);

bool maps_visitor_reset(MapsVisitor_t *visitor);

bool maps_visitor_has_next(MapsVisitor_t *visitor);

MapItem* maps_visitor_next(MapsVisitor_t *visitor, MapItem *mapItem);

int maps_visitor_destroy(MapsVisitor_t *visitor);

__END_DECLS
