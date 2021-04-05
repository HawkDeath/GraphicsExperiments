cmake_minimum_required(VERSION 3.10)
project(App)

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3party/Leprechaun/3party>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3party/Leprechaun/3party/glad/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3party/Leprechaun/LeprechaunLib/include>)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(${PROJECT_NAME} PRIVATE LeprechaunLib glfw glad imgui stb_image spdlog glm)