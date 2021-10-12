add_subdirectory(sassm)
add_subdirectory(tuning)

if(SASSM_ENABLE_CUDA)
  add_subdirectory(ptx)
  add_subdirectory(util)
endif()
