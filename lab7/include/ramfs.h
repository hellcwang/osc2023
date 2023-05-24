#ifndef RAMFS_H
#define RAMFS_H

#include "vfs.h"
#include "uart.h"
#include "mem.h"
#include "str.h"

typedef struct{
	char* name;
	int type;
	int size;
	void *dirs;
	void *data;
}FsAttr;

int ramfs_init(struct filesystem* fs, struct mount* m);

struct filesystem *getRamFs();

// This function is to initial from cpio archive
int ramfs_initFsCpio(struct vnode*);

int ramfs_lookup(struct vnode* dir_node, struct vnode** trget, const char*);
int ramfs_create(struct vnode* dir_node, struct vnode** trget, const char*);
int ramfs_mkdir(struct vnode* dir_node, struct vnode** trget, const char*);

int ramfs_read(struct file* f, void* buf, size_t len);
int ramfs_write(struct file* f, const void* buf, size_t len);
int ramfs_open(struct vnode*, struct file** target);
int ramfs_close(struct file* f);

void data_init(FsAttr* fs);

#endif
