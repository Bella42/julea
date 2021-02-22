#include "julea_bluestore.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char** argv)
{
	void* store;
	void* coll;
	void* obj;
	store = julea_bluestore_init("/tmp/bluestore-test");
	if (access("/tmp/bluestore-test/mkfs_done", F_OK) == 0)
	{
		printf("mkfs skipped \n");

		int mtrt = julea_bluestore_mount(store);
		printf("mount returned %d \n", mtrt);
		assert(mtrt == 0);

		coll = julea_bluestore_open_collection(store);
	}
	else
	{
		int mkrt = julea_bluestore_mkfs(store);
		printf("mkfs returned %d \n", mkrt);
		assert(mkrt == 0);

		int mtrt = julea_bluestore_mount(store);
		printf("mount returned %d \n", mtrt);
		assert(mtrt == 0);

		coll = julea_bluestore_create_collection(store);
	}

	obj = julea_bluestore_create(store, coll, "test_object");
	int bw = julea_bluestore_write(store, coll, obj, 0, "Test Object Content", 19);
	printf("Bytes written: %d \n", bw);
	assert(bw == 19);

	char* readback = malloc(sizeof(char) * 20);
	int br = julea_bluestore_read(store, coll, obj, 0, &readback, 19);
	printf("Bytes read: %d \n", br);
	assert(br == 19);

	struct stat* buf;
	buf = malloc(sizeof(struct stat));
	julea_bluestore_status(store, coll, obj, buf);
	int size = buf->st_size;
	printf("Stat size: %d \n", size);
	assert(size == 19);

	julea_bluestore_delete(store, coll, obj);

	int umtrt = julea_bluestore_umount(store, coll);
	printf("umount returned %d \n", umtrt);
	assert(umtrt == 0);

	return 0;
}