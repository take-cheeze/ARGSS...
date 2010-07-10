#define argss_USE_NEDMALLOC 1

#if argss_USE_NEDMALLOC
	#include "nedmalloc.c"
	#define argss_malloc nedalloc::nedmalloc
	#define argss_free nedalloc::nedfree
#elif argss_USE_DLMALLOC
	#define USE_LOCKS 1
	#define USE_DL_PREFIX
	#include "malloc.c"
	#define argss_malloc dlmalloc
	#define argss_free dlfree
#endif

#include <cstddef>
#include <new>

void* operator new(std::size_t size) throw (std::bad_alloc)
{
	return argss_malloc(size);
}
void* operator new[](std::size_t size) throw (std::bad_alloc)
{
	return argss_malloc(size);
}
void operator delete(void* ptr) throw ()
{
	argss_free(ptr);
}
void operator delete[](void* ptr) throw ()
{
	argss_free(ptr);
}
