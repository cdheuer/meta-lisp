#include "elf.h"

typedef uint32_t SECTION_TYPE;
#define NULL_SECTION_TYPE 0x0
#define PROGRAM_DATA_SECTION_TYPE 0x1
#define SYMBOL_TABLE_SECTION_TYPE 0x2
#define STRING_TABLE_SECTION_TYPE 0x3
#define RELOCATION_WITH_ADDENDS_SECTION_TYPE 0x4
#define SYMBOL_HASH_TABLE_SECTION_TYPE 0x5
#define DYNAMIC_SYMBOL_TABLE_SECTION_TYPE 0x6
#define NOTES_SECTION_TYPE 0x7
#define NO_BSS_SECTION_TYPE 0x8
#define RELOCATION_WITH_NO_ADDENDS_SECTION_TYPE 0x9
#define CONSTRUCTOR_ARRAY_SECTION_TYPE 

struct elf_section {
	uint32_t name;
	SECTION_TYPE type;
};

// size_t is assumed to be 8 bytes for 64bit and 4 bytes for 32bit
struct elf_headers {
	struct {
		enum {
			ADDRESS_32BIT,
			ADDRESS_64BIT
		} address_size;

		enum {
			LITTLE_ENDIAN,
			BIG_ENDIAN
		} endianness;

		bool original_version;

		enum {
			LINUX_ABI,
			SYSTEM_V_ABI,
			NETBSD_ABI,
			FREEBSD_ABI
		} abi;

		int abi_version;

		enum {
			UNKNOWN_OBJECT_TYPE,
			RELOCATABLE_OBJECT_TYPE,
			EXECUTABLE_OBJECT_TYPE,
			SHARED_OBJECT_TYPE,
			CORE_OBJECT_TYPE
		} object_type;

		int object_type_flags;

		enum {
			X86_ISA,
			AMD64_ISA,
			AARCH64_ISA,
			ARM_ISA
		} isa;

		void *entry_point;

		void *program_header;

		void *section_header;

		int isa_flags;

		size_t file_header_size;

		size_t program_header_size;

		int nsection_header_entries;

		void *section_header_names;
	} file;

	struct {
		enum {
			UNUSED_PROGRAM_TYPE,
			LOADABLE_PROGRAM_TYPE,
			DYNAMIC_PROGRAM_TYPE,
			INTERPRETER_PROGRAM_TYPE,
			AUXILLARY_PROGRAM_TYPE,
			RESERVED_PROGRAM_TYPE,
			PROGRAM_HEADER_PROGRAM_TYPE,
			TLS_PROGRAM_TYPE,
			LOW_OS_PROGRAM_TYPE = 0x60000000,
			HIGH_OS_PROGRAM_TYPE = 0x6FFFFFFF,
			LOW_PROCESSOR_PROGRAM_TYPE = 0x70000000,
			HIGH_PROCESSOR_PROGRAM_TYPE = 0x7FFFFFFF
		} type;

		int flags_64bit;

		void *file_image;

		void *virtual_address;

		void *physical_address;

		size_t file_image_size;

		size_t program_header_size;

		int flags_32bit;

		size_t alignment;
	} program;
;


