# linux/macOS build Android
#>>> export ANDROID_NDK=/Users/jmpews/Library/Android/sdk/ndk-bundle
## arm64
#>>> cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI=arm64-v8a -DZPLATFORM=Android -DZARCH=arm64
#>>> make
## armv7
#>>> cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI=armeabi-v7a -DZPLATFORM=Android -DZARCH=armv7
#>>> make

# windows build Android
#>>> set path=%path%;xxx\cmake\3.6.4111459\bin
#>>> set ANDROID_NDK=D:\TechnicalProgramFiles\Android-SDK\ndk-bundle
## arm64
#>>> cmake .. -G "Android Gradle - Ninja" -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake -DAN DROID_NDK=%ANDROID_NDK% -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI=arm64-v8a -DZPLATFORM=Android -DZARCH=arm64
#>>> ninja
## armv7
#>>> cmake .. -G "Android Gradle - Ninja" -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake -DAN DROID_NDK=%ANDROID_NDK% -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI=armeabi-v7a -DZPLATFORM=Android -DZARCH=armv7
#>>> ninja

# macOS build iOS
## arm64
#>>> cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/ios.toolchain.cmake -DIOS_PLATFORM=OS -DIOS_ARCH=arm64 -DENABLE_ARC=FALSE -DZPLATFORM=iOS -DZARCH=arm64
#>>> make
## armv7
#>>> cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/ios.toolchain.cmake -DIOS_PLATFORM=OS -DIOS_ARCH=armv7 -DENABLE_ARC=FALSE -DZPLATFORM=iOS -DZARCH=armv7
#>>> make

cmake_minimum_required(VERSION 3.10)
project(HookZz)

include(cmake/Util.cmake)

set(CMAKE_CXX_STANDARD 11)
enable_language(ASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu11")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")

set(GLOBAL.SOURCE_FILE)
set(GLOBAL.SOURCE_DIR)
set(GLOBAL.HEADER_FILE)
set(GLOBAL.HEADER_DIR)

set(HookZz.Path .)

set(GLOBAL.SOURCE_DIR
        ${HookZz.Path}/srcxx
        ${HookZz.Path}/srcxx/CommonClass
        ${HookZz.Path}/srcxx/CommonClass/DesignPattern
        ${HookZz.Path}/srcxx/Platforms
        ${HookZz.Path}/srcxx/Platforms/arch-arm64
        ${HookZz.Path}/srcxx/Platforms/backend-darwin
        ${HookZz.Path}/srcxx/Platforms/backend-posix
        )

set(GLOBAL.HEADER_DIR
        ${HookZz.Path}/include
        )



# *.cpp
search_suffix_files("cpp" GLOBAL.SOURCE_DIR HookZz.SOURCE_CPP)

# *.h
search_suffix_files("h" GLOBAL.SOURCE_DIR HookZz.HEADER_H)

set(GLOBAL.SOURCE_FILE ${GLOBAL.SOURCE_FILE} ${HookZz.SOURCE_CPP})
set(GLOBAL.HEADER_FILE ${GLOBAL.HEADER_FILE} ${HookZz.HEADER_H})

include_directories(${GLOBAL.HEADER_DIR} ${GLOBAL.SOURCE_DIR})
add_library(HookZz STATIC ${GLOBAL.SOURCE_FILE} ${GLOBAL.HEADER_FILE})
