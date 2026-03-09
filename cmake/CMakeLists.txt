cmake_minimum_required(VERSION 3.19)

project(7segmant C CXX ASM)

add_executable(${PROJECT_NAME} src/dio.c src/main.c)

target_include_directories(calculator PUBLIC inc/ PRIVATE incPriv/)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)
# include_directories(${PROJECT_NAME} inc/)