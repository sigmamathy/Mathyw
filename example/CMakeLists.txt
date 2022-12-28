list(APPEND MATHYW_EXAMPLES "create_window")

foreach(src IN ZIP_LISTS MATHYW_EXAMPLES)
	add_executable(src "${src}.cpp")
	set_property(TARGET src PROPERTY CXX_STANDARD 20)
	target_include_directories(src PUBLIC "${PROJECT_SOURCE_DIR}/include")
	target_link_libraries(src ${PROJECT_NAME})
endforeach()
