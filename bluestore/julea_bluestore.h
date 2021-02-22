#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

	void* julea_bluestore_init(const char*);

	int julea_bluestore_mkfs(void*);

	int julea_bluestore_mount(void*);

	int julea_bluestore_umount(void*, void*);

	void* julea_bluestore_create_collection(void*);

	void* julea_bluestore_open_collection(void*);

	void julea_bluestore_fsync(void*);

	void* julea_bluestore_open(const char*);

	void* julea_bluestore_create(void*, void*, const char*);

	int julea_bluestore_delete(void*, void*, void*);

	int julea_bluestore_write(void*, void*, void*, uint64_t, const char*, uint64_t);

	int julea_bluestore_read(void*, void*, void*, uint64_t, char**, uint64_t);

	int julea_bluestore_status(void*, void*, void*, struct stat*);

#ifdef __cplusplus
}
#endif