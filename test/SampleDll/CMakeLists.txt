project(ZxHook_SampleDll)

add_library("${PROJECT_NAME}" MODULE "SampleDll.cpp")
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_options("${PROJECT_NAME}" PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
target_link_libraries("${PROJECT_NAME}" PRIVATE Zut::ZxHook)