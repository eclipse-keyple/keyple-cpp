
set(CMAKE_SYSTEM_NAME Linux)
set(UNIX 1)
set(APPLE 0)

if(${UNIX})
  message("GENERATION LINUX")

  #set(CMAKE_CXX_FLAGS              ${CMAKE_CXX_FLAGS} "-std=c++14 -pthread")
  #set(CMAKE_C_FLAGS                ${CMAKE_C_FLAGS} "-pthread")

else()
  if(${APPLE})
    message("GENERATION MAC")

    #set(CMAKE_SYSROOT /usr/include )    
    #set(CMAKE_C_FLAGS "   -pthread -march=armv7-a -marm -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 ")
    #set(CMAKE_CXX_FLAGS "   -std=c++14 -pthread -march=armv7-a -marm -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 ")
    #link_directories( /usr/lib )

  else()
    message("GENERATION WINDOWS")

    #set(CMAKE_SYSROOT /usr/include )    
    #set(CMAKE_C_FLAGS "   -pthread -march=armv7-a -marm -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 ")
    #set(CMAKE_CXX_FLAGS "   -std=c++14 -pthread -march=armv7-a -marm -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 ")
    #link_directories( /usr/lib )

  endif()
endif()
