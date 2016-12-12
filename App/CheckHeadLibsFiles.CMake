include (CheckIncludeFiles)

check_include_files(pthread.h CHECK_PTHREAD_H)

#include (CheckLibraryExists)
#check_library_exists(Debug AvBsp "" check_have_lab_bsp)
#message("^^^${check_have_lab_bsp}")
