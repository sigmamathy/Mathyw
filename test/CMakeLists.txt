add_executable("test" "main.cpp")

set_property(TARGET "test" PROPERTY CXX_STANDARD 20)
target_include_directories("test" PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries("test" ${PROJECT_NAME})