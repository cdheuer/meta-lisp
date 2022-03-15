#ifndef IO_C
#include <assert.h>

enum io_privileges {
        IO_R_PRIVILEGES,
        IO_RW_PRIVILEGES,
        IO_RWE_PRIVILEGES
};

#ifdef POSIX
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct file_mapping {
        void *ptr;
        size_t size;
};

struct file {
	int fd;
};

int map_file(struct ml_continuation *cc, struct file_mapping *fmap, void *addr,
             const struct file *file, enum io_privileges privileges, bool private);

int file_size(struct ml_continuation *cc, const struct file *file, size_t *size);

int translate_mmap_io_privileges(enum io_privileges privileges);

#endif /* POSIX */

#endif

/* SOURCE ********************************************************************/
#define IO_C

#ifdef POSIX

int file_size(struct ml_continuation *cc, const struct file *file, size_t *size)
{
	int err;
	struct stat status;

	err = fstat(file->fd, &status);
	if (err)
		return -1;

	*size = status.st_size;
	return 0;
}

int translate_mmap_io_privileges(enum io_privileges privileges)
{
        switch (privileges)
        {
        case IO_R_PRIVILEGES:
                return PROT_READ;
        case IO_RW_PRIVILEGES:
                return PROT_READ | PROT_WRITE;
        case IO_RWE_PRIVILEGES:
                return PROT_READ | PROT_WRITE | PROT_EXEC;
        default:
                assert(!"Invalid enum");
                return -1;
        }
}

int map_file(struct ml_continuation *cc, struct file_mapping *fmap, void *addr,
             const struct file *file, enum io_privileges privileges, bool private)
{
        // TODO: Exceptions

        int prot, err, flags;
	size_t size;
        void *mem;

	err = file_size(cc, file, &size);
	if (err)
		return -1;

	flags = MAP_FILE;
	flags |= private ? MAP_PRIVATE : MAP_SHARED;

        prot = translate_mmap_io_privileges(privileges);
	mem = mmap(addr, size, prot, flags, file->fd, 0);

	// OSX man page says -1 is returned which should be the same as MAP_FAILED
	// which is what linux returns
	if (mem == MAP_FAILED)
		return -1;

	fmap->mem = mem;
	fmap->size = size;
        return 0;
}
#endif /* POSIX */


