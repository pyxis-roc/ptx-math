add_executable(sassmvalidate
  main.cu
)

target_compile_features(sassmvalidate PRIVATE cxx_std_11)

target_include_directories(sassmvalidate PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(sassmvalidate PRIVATE ptx sassm util)

install(TARGETS sassmvalidate DESTINATION bin)
