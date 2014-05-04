#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x8e452e5, "module_layout" },
	{ 0x61b7b126, "simple_strtoull" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x66e3f9e8, "kmem_cache_destroy" },
	{ 0xb1d63e0, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xc897c382, "sg_init_table" },
	{ 0x9b388444, "get_zeroed_page" },
	{ 0x280d5ffa, "kernel_sendmsg" },
	{ 0x51443c65, "mem_map" },
	{ 0x76ebea8, "pv_lock_ops" },
	{ 0x46c6cb85, "bio_alloc" },
	{ 0x15692c87, "param_ops_int" },
	{ 0xd0d8621b, "strlen" },
	{ 0x7b7122b, "page_address" },
	{ 0x64d49c2b, "filemap_write_and_wait_range" },
	{ 0x88290f62, "seq_open" },
	{ 0x3a013b7d, "remove_wait_queue" },
	{ 0xacf4d843, "match_strdup" },
	{ 0xc01cf848, "_raw_read_lock" },
	{ 0xa4eb4eff, "_raw_spin_lock_bh" },
	{ 0x1881c390, "sock_recvmsg" },
	{ 0x96c7e43b, "seq_printf" },
	{ 0x632f6752, "remove_proc_entry" },
	{ 0x6729d3df, "__get_user_4" },
	{ 0x44e9a829, "match_token" },
	{ 0x3cc2254f, "__register_chrdev" },
	{ 0x1d19d2d1, "filp_close" },
	{ 0xfb0e29f, "init_timer_key" },
	{ 0x97428091, "mutex_unlock" },
	{ 0x85df9b6c, "strsep" },
	{ 0x3f81ddad, "seq_read" },
	{ 0xa627b22c, "__alloc_pages_nodemask" },
	{ 0xc499ae1e, "kstrdup" },
	{ 0x568d43b5, "kthread_create_on_node" },
	{ 0x7d11c268, "jiffies" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xbc831b14, "fsync_bdev" },
	{ 0x5f568426, "sock_no_sendpage" },
	{ 0x68dfc59f, "__init_waitqueue_head" },
	{ 0xffd5a395, "default_wake_function" },
	{ 0x3fa58ef8, "wait_for_completion" },
	{ 0xfe0226a4, "vfs_read" },
	{ 0xd5f2172f, "del_timer_sync" },
	{ 0x53b5b61b, "netlink_kernel_create" },
	{ 0x2bc95bd4, "memset" },
	{ 0xf1424170, "proc_mkdir" },
	{ 0x11089ac7, "_ctype" },
	{ 0x692186bf, "current_task" },
	{ 0x6ae87002, "mutex_lock_interruptible" },
	{ 0xaae87824, "__mutex_init" },
	{ 0x50eedeb8, "printk" },
	{ 0xdf3d507e, "kthread_stop" },
	{ 0x547c0d48, "bio_add_page" },
	{ 0x966335f8, "netlink_kernel_release" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xb6ed1e53, "strncpy" },
	{ 0xb4390f9a, "mcount" },
	{ 0x41e7a3db, "blkdev_get_by_path" },
	{ 0x1d77c979, "kmem_cache_free" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xfe94349f, "mutex_lock" },
	{ 0x7f658e80, "_raw_write_lock" },
	{ 0x8834396c, "mod_timer" },
	{ 0xb23a82da, "netlink_unicast" },
	{ 0xc864fbbd, "skb_pull" },
	{ 0x253baa35, "init_net" },
	{ 0x5fd44d2d, "fput" },
	{ 0x8def339e, "contig_page_data" },
	{ 0x5d5b5a16, "radix_tree_delete" },
	{ 0x883461f3, "bio_put" },
	{ 0x38640ee3, "put_io_context" },
	{ 0x9848e6d9, "module_put" },
	{ 0x3678fa21, "submit_bio" },
	{ 0xdd1c65f6, "blk_finish_plug" },
	{ 0x9e1ab005, "kmem_cache_alloc" },
	{ 0x8ff4079b, "pv_irq_ops" },
	{ 0x3e79c41, "__free_pages" },
	{ 0x4e47fa42, "blkdev_put" },
	{ 0x2b711b03, "__alloc_skb" },
	{ 0x42c8de35, "ioremap_nocache" },
	{ 0x47b3f862, "radix_tree_lookup_slot" },
	{ 0x8bf826c, "_raw_spin_unlock_bh" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x4292364c, "schedule" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0xf1faac3a, "_raw_spin_lock_irq" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0x992f20a5, "get_task_io_context" },
	{ 0x63b02efb, "crypto_destroy_tfm" },
	{ 0xcd358c51, "create_proc_entry" },
	{ 0x2c5d3bcd, "wake_up_process" },
	{ 0x3d0f6d5b, "netlink_ack" },
	{ 0x4f31e693, "kmem_cache_alloc_trace" },
	{ 0x67f7403e, "_raw_spin_lock" },
	{ 0xa0f0c981, "vfs_writev" },
	{ 0xf9ce2d8f, "kmem_cache_create" },
	{ 0x4302d0eb, "free_pages" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0xd2965f6f, "kthread_should_stop" },
	{ 0xff1e9dd8, "seq_list_start" },
	{ 0x5c3edd59, "_raw_write_unlock_bh" },
	{ 0xd7bd3af2, "add_wait_queue" },
	{ 0x65a37537, "seq_lseek" },
	{ 0x37a0cba, "kfree" },
	{ 0x2e60bace, "memcpy" },
	{ 0xedc03953, "iounmap" },
	{ 0x9754ec10, "radix_tree_preload" },
	{ 0x481963d5, "fget" },
	{ 0x32eeaded, "_raw_write_lock_bh" },
	{ 0x90a1004a, "crypto_has_alg" },
	{ 0x19a9e62b, "complete" },
	{ 0xb81960ca, "snprintf" },
	{ 0x3134a249, "seq_release" },
	{ 0xe7d4daac, "seq_list_next" },
	{ 0x13abf850, "crypto_alloc_base" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xf202c5cb, "radix_tree_insert" },
	{ 0xe5d95985, "param_ops_ulong" },
	{ 0xddacc1a0, "__nlmsg_put" },
	{ 0x43a0458b, "blk_start_plug" },
	{ 0x3ee10641, "try_module_get" },
	{ 0x760a0f4f, "yield" },
	{ 0xec10c2e5, "vfs_write" },
	{ 0xc90adc63, "filp_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4ADA9C148ADBD227782E919");
