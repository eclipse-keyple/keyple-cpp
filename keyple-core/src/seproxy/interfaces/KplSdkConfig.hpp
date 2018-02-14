
#ifndef KPL_SDK_CONFIG_H
#define KPL_SDK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* -------------------------------------------------------------------------- *
 * - Definition of the OS used ---------------------------------------------- *
 * -------------------------------------------------------------------------- */

#define	K_OS_KPL_PLATFORM_WIN32												1
#define	K_OS_KPL_PLATFORM_LINUX												2

/* -------------------------------------------------------------------------- * 
 * - Includes --------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

/* WIN32 -------------------------------------------------------------------- */

#if K_OS_KPL_PLATFORM == K_OS_KPL_PLATFORM_WIN32

// windows includes
// TODO reorganize header includes
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <stdint.h>

// Libraries exports
#define EXPORT __declspec(dllexport)

// Dynamic libraries management
#define handle_t HMODULE
#define OsOpenLibrary(LIB)			LoadLibrary(TEXT(#LIB ".dll"))
#define OsCloseLibrary(handle)		FreeLibrary(handle)
#define OsGetSymbol(handle, name)	GetProcAddress(handle, name)
#define OsGetLastError()			GetLastError()

#endif // K_OS_KPL_PLATFORM_WIN32

/* LINUX -------------------------------------------------------------------- */

#if K_OS_KPL_PLATFORM == K_OS_KPL_PLATFORM_LINUX

// linux includes
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <stdint.h>
#include <dlfcn.h>

// Libraries exports
#define EXPORT __attribute__((visibility("default")))

// Dynamic libraries management
#define handle_t void *
#define OsOpenLibrary(LIB)			dlopen("./lib" #LIB ".so", RTLD_LAZY|RTLD_LOCAL)
#define OsCloseLibrary(handle)		dlclose(handle)
#define OsGetSymbol(handle, name)	dlsym(handle, name)
#define OsGetLastError()			dlerror()

#endif // K_OS_KPL_PLATFORM_LINUX


/* -------------------------------------------------------------------------- *
 * - Defines ---------------------------------------------------------------- *
 * -------------------------------------------------------------------------- */

#if K_OS_KPL_PLATFORM == K_OS_KPL_PLATFORM_WIN32 ||  K_OS_KPL_PLATFORM == K_OS_KPL_PLATFORM_LINUX

#define K_OS_KPL_DEV_NAME_MAX_LENGTH										256

// Defines the file path buffer length. (Include de character '\0')
#define K_OS_KPL_FILE_PATH_MAX_SIZE											(MAX_PATH + 1)

#endif


/* ------------------------------------------------------------------------- *
* - Macros ----------------------------------------------------------------- *
* -------------------------------------------------------------------------- */
/* DEBUG MACROS */
#ifdef STANDARD_OUTPUT_LOG
#define DBG_TRACE_CALL()	do {	\
								std::cout << "# " << __FUNCTION__ << std::endl; \
							} while(0)

#define DBG_INFO_MSG(MSG)	do {	\
								std::cout << MSG << std::endl;	\
							} while(0)

#define DBG_ERROR_MSG(MSG)	do {	\
								std::cerr << MSG << std::endl;	\
							} while(0)

#define DBG_DUMP_HEX_VECTOR(MSG, VECT)	do { \
											std::cout << MSG; \
											for (auto const& element : VECT) { \
												std::cout << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << (int)element << " "; \
											} \
											std::cout << std::nouppercase << std::dec << std::endl; \
										} while(0)
#else
#define DBG_TRACE_CALL()
#define DBG_INFO_MSG(MSG)
#define DBG_ERROR_MSG(MSG)
#define DBG_DUMP_HEX_VECTOR(MSG, VECT)
#endif // STANDARD_OUTPUT_LOG


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //KPL_SDK_CONFIG_H