/*
This header is "gcc -include"d for all compilations of firmware files when building as simulat0r.
The following symbols were found to be defined within glibc.
Use different names within simulat0r to keep the firmware and simulat0r-host universes collision-free.
 */
#define buf __r0ket_buf
#define deselect __r0ket_deselect
#define ECIES_embedded_public_key_validation __r0ket_ECIES_embedded_public_key_validation
#define ECIES_generate_key_pair __r0ket_ECIES_generate_key_pair
#define ECIES_public_key_validation __r0ket_ECIES_public_key_validation
#define f_sync __r0ket_f_sync
#define getInput __r0ket_getInput
#define isalnum __r0ket_isalnum
#define isalpha __r0ket_isalpha
#define isascii __r0ket_isascii
#define isblank __r0ket_isblank
#define iscntrl __r0ket_iscntrl
#define isdigit __r0ket_isdigit
#define isgraph __r0ket_isgraph
#define islower __r0ket_islower
#define isprint __r0ket_isprint
#define ispunct __r0ket_ispunct
#define isspace __r0ket_isspace
#define isupper __r0ket_isupper
#define isxdigit __r0ket_isxdigit
#define key __r0ket_key
#define memcmp __r0ket_memcmp
#define memcpy __r0ket_memcpy
#define memmove __r0ket_memmove
#define memset __r0ket_memset
#define printf __r0ket_printf
#define select __r0ket_select
#define snprintf __r0ket_snprintf
#define sprintf __r0ket_sprintf
#define strchr __r0ket_strchr
#define strcmp __r0ket_strcmp
#define strcpy __r0ket_strcpy
#define strlen __r0ket_strlen
#define strncmp __r0ket_strncmp
#define strncpy __r0ket_strncpy
#define strrchr __r0ket_strrchr
#define strtok __r0ket_strtok
#define strtok_r __r0ket_strtok_r
#define sync __r0ket_sync
#define tolower __r0ket_tolower
#define toupper __r0ket_toupper
#define vprintf __r0ket_vprintf
#define vsnprintf __r0ket_vsnprintf
#define vsprintf __r0ket_vsprintf
#define XTEA_init_key __r0ket_XTEA_init_key
