#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

	void init_bs_folder(const char* folder, const char* blkdev);

	void julea_bluestore_init(const char*);

	void julea_bluestore_mount(const char*);

	void julea_bluestore_umount(void);

	void julea_bluestore_create(const char*);

	void julea_bluestore_delete(const char*);

	int julea_bluestore_write(const char*, uint64_t, const char*, uint64_t);

	int julea_bluestore_read(const char*, uint64_t, char**, uint64_t);

	int julea_bluestore_status(const char*, struct stat*);

#ifdef __cplusplus
}
#endif
