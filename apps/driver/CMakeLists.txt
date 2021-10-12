add_executable(sassmrun
  main.cpp
)

target_compile_features(sassmrun PRIVATE cxx_std_11)

target_include_directories(sassmrun PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(sassmrun PRIVATE sassm)

install(TARGETS sassmrun DESTINATION bin)
