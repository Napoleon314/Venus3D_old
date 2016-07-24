CORE_LIB(false "Venus3D" "fcontext" "_static" "Static")
SHELL_LIB(false "Framework" "Venus3D_static" "_static" "Static")
TEST_PROJ(false false "PowerTest" "Venus3D_static" "_static" "Static/Test")
TEST_PROJ(false true "RenderTest" "Framework_static" "_static" "Static/Test")


#TARGET_LINK_LIBRARIES(RenderTest_static SDL2)
#TARGET_INCLUDE_DIRECTORIES(RenderTest_static PRIVATE ${EXTERNAL_PATH}/SDL2/include)
