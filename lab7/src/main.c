#include "dtb.h"
#include "exception.h"
#include "heap.h"
#include "initrd.h"
#include "interrupt.h"
#include "loader.h"
#include "mem.h"
#include "ramfs.h"
#include "syscall.h"
#include "terminal.h"
#include "thread.h"
#include "uart.h"
#include "vfs.h"
#include <stdint.h>

extern void set_exception_vector_table(void);
extern struct vnode *fsRoot;
extern struct mount *fsRootMount;

int main(void *dtb_location) {
  uart_setup();
  heap_init();
  pmalloc_init();
  set_exception_vector_table();
  enable_int();
  thread_init();
  preserve(0, 0x5000000);
  preserve(0x8200000, 0x2000000); // For user program
  smalloc_init();
  fdt_find_do(dtb_location, "linux,initrd-start", initrd_fdt_callback);
  uart_puts("test_thread\n");
  // Initial Fs
  fsRootMount = (struct mount *)malloc(sizeof(struct mount));
  fsRootMount->root = (struct vnode *)malloc(sizeof(struct vnode));
  fsRoot = fsRootMount->root;
  fsRoot->parent = NULL;
  fsRoot->mount = fsRootMount;
  struct filesystem *fs = getRamFs();
  register_filesystem(fs);
  fs->setup_mount(fs, fsRootMount);
  ramfs_initFsCpio(fsRoot);
  uart_puts("\nDump\n");
  struct vnode *test;
  // ramfs_mkdir(fsRoot, &test, "mkdir");
  vfs_mkdir("mkdir", NULL);
  vfs_lookup("mkdir", &test, NULL);
  // vfs_create(test, &test, "vfs_creast");
  struct file *f = NULL, *f2 = NULL;
  vfs_open("mkdir/vfs", O_CREAT, &f, NULL);
  vfs_open("mkdir/vfs", O_CREAT, &f2, NULL);
  char ttt[20] = {0};
  uart_puts("test R/W\n");
  f->f_ops->write(f, "written data\n", 14);
  f2->f_ops->read(f2, ttt, 14);
  uart_puts(ttt);
  // ramfs_dump(f->vnode, 6);
  ramfs_dump(fsRoot, 0);
  // ramfs_dump(test, 6);

  //
  core_timer_enable();
  terminal_run_thread();
  // idle();
  // terminal_run();

  return 0;
}
