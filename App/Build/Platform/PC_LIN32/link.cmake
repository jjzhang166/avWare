#target_link_libraries(${target} avdevice)
#target_link_libraries(${target} avfilter)
#target_link_libraries(${target} avformat)
#target_link_libraries(${target} avcodec)
#target_link_libraries(${target} swresample)
#target_link_libraries(${target} swscale)
#target_link_libraries(${target} avutil)
#target_link_libraries(${target} postproc)

target_link_libraries(${target} AvCedrShared)
target_link_libraries(${target} Xv)
target_link_libraries(${target} Xext)
target_link_libraries(${target} xcb)
target_link_libraries(${target} xcb-shm)
target_link_libraries(${target} xcb)
target_link_libraries(${target} xcb-xfixes)
target_link_libraries(${target} xcb-render)
target_link_libraries(${target} xcb-shape)
target_link_libraries(${target} xcb)
target_link_libraries(${target} X11)
target_link_libraries(${target} asound)
target_link_libraries(${target} SDL)
target_link_libraries(${target} pthread)
target_link_libraries(${target} m)
target_link_libraries(${target} lzma)


#target_link_libraries(${target} bz2)
#target_link_libraries(${target} z)
#target_link_libraries(${target} x264)




## -lavformat  -lavdevice -lavcodec  -lavutil -pthread  -ldl -lswscale -lSDL -lbz2  -lasound -lz -lm  
##lzma_stream_decoder
##

#Name: libavdevice
#Requires.private: libavfilter >= 5.2.103, libswscale >= 3.1.101, libavformat >= 56.15.102, libavcodec >= 56.13.100, libswresample >= 1.1.100, libavutil >= 54.15.100
#Libs.private: -lXv -lX11 -lXext -lxcb -lxcb-shm -lxcb -lxcb-xfixes -lxcb-render -lxcb-shape -lxcb -lxcb-shape -lxcb -lX11 -lasound -lSDL -lpthread -lm -llzma -lbz2 -lz -pthread

#Name: libavfilter
#Requires.private: libswscale >= 3.1.101, libavformat >= 56.15.102, libavcodec >= 56.13.100, libswresample >= 1.1.100, libavutil >= 54.15.100
#Libs.private: -lXv -lX11 -lXext -lxcb -lxcb-shm -lxcb -lxcb-xfixes -lxcb-render -lxcb-shape -lxcb -lxcb-shape -lxcb -lX11 -lasound -lSDL -lpthread -lm -llzma -lbz2 -lz -pthread

#Name: libavformat
#Requires.private: libavcodec >= 56.13.100, libswresample >= 1.1.100, libavutil >= 54.15.100
#Libs.private: -lXv -lX11 -lXext -lxcb -lxcb-shm -lxcb -lxcb-xfixes -lxcb-render -lxcb-shape -lxcb -lxcb-shape -lxcb -lX11 -lasound -lSDL -lpthread -lm -llzma -lbz2 -lz -pthread


#Name: libavcodec
#Requires.private: libswresample >= 1.1.100, libavutil >= 54.15.100
#Libs.private: -lXv -lX11 -lXext -lxcb -lxcb-shm -lxcb -lxcb-xfixes -lxcb-render -lxcb-shape -lxcb -lxcb-shape -lxcb -lX11 -lasound -lSDL -lpthread -lm -llzma -lbz2 -lz -pthread


#Name: libswresample
#Requires.private: libavutil >= 54.15.100
#Libs.private: -lm

#Name: libswscale
#Requires.private: libavutil >= 54.15.100
#Libs.private: -lm



#Name: libavutil
#Libs.private: -lm



