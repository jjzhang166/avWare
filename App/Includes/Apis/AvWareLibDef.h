
#if 0
#if defined (WIN32)
#define AVWARE_API __declspec(dllexport)
#else
#define AVWARE_API __attribute__((visibility("default")))
#endif

#else
#ifdef AVWARE_API
#undef AVWARE_API
#endif

#ifdef AVWARE_SHARE_EXPORT
	#if defined (WIN32)
		#define AVWARE_API __declspec(dllexport)
	#else
		#define AVWARE_API __attribute__((visibility("default")))
	#endif
#elif defined (AVWARE_SHARE_IMPORT)
	#if defined(WIN32)
		#define AVWARE_API __declspec(dllimport)
	#else
		#define AVWARE_API
	#endif
#else
	#define AVWARE_API
#endif


#undef  AVWARE_SHARE_EXPORT
#undef  AVWARE_SHARE_IMPORT

#endif

