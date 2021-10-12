add_library(sassm
  models.h
  tuning.h
  params.h
  rcp.c
  rcp64h.c
  sqrt.c
  rsqrt.c
  rsq64h.c
  sincos.c
  lg2.c
  ex2.c
)

add_subdirectory(common)
add_subdirectory(reduction)
add_subdirectory(tables)

target_compile_features(sassm PUBLIC c_std_11)
set_target_properties(sassm PROPERTIES C_EXTENSIONS OFF)

if(NOT MSVC)
  target_compile_options(sassm PRIVATE -mbmi2)
endif()

if(PTXM_USE_PIC)
  set_target_properties(sassm PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

install(TARGETS sassm DESTINATION lib)
install(FILES models.h DESTINATION include/sassm)
