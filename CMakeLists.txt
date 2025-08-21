cmake_minimum_required(VERSION 3.10)
project(depthtest)

set(CMAKE_C_STANDARD 99)

# OpenGL ES ve GLFW paketlerini bul
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLES2 REQUIRED glesv2)
pkg_check_modules(GLFW REQUIRED glfw3)

# Executable oluştur
add_executable(depthtest src/main.c)

# Include dizinlerini ekle
target_include_directories(depthtest PRIVATE 
    ${GLES2_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

# Kütüphaneleri bağla
target_link_libraries(depthtest 
    ${GLES2_LIBRARIES}
    ${GLFW_LIBRARIES}
    m  # math library
)