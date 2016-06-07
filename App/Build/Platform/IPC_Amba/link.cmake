
# share library
target_link_libraries(${target} amdev)
target_link_libraries(${target} amp)
target_link_libraries(${target} asound)
target_link_libraries(${target} freetype)
target_link_libraries(${target} bz2)
target_link_libraries(${target} z)
target_link_libraries(${target} dome)
target_link_libraries(${target} autofocus)


# static library
target_link_libraries(${target} img_algo_s2l)
target_link_libraries(${target} img_dsp_s2l_hf)
target_link_libraries(${target} textinsert)
target_link_libraries(${target} ProtoShare)

