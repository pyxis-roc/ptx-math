add_subdirectory(driver)

if(SASSM_ENABLE_CUDA)
  add_subdirectory(validation)
endif()
