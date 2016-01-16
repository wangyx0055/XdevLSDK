SET(NVPACK_SOURCE_FOLDER "/media/Data/NVPACK")

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   ${NVPACK_SOURCE_FOLDER}/android-ndk-r10d/toolchains/aarch64-linux-android-4.9/gen_standalone/linux-x86_64/bin/aarch64-linux-android-gcc)
SET(CMAKE_CXX_COMPILER ${NVPACK_SOURCE_FOLDER}/android-ndk-r10d/toolchains/aarch64-linux-android-4.9/gen_standalone/linux-x86_64/bin/aarch64-linux-android-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH ${NVPACK_SOURCE_FOLDER}/android-ndk-r10d/toolchains/aarch64-linux-android-4.9/gen_standalone/linux-x86_64)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(PLATFORM_ANDROID TRUE)
#dd_definitions(-D_LOG -DXDEVL_PLATFORM_ANDROID=1)