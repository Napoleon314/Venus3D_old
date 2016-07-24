CORE_LIB(true "Venus3D" "fcontext" "_shared" "Shared")
SHELL_LIB(true "Framework" "Venus3D_shared" "_shared" "Shared")
TEST_PROJ(true false "PowerTest" "Venus3D_shared" "_shared" "Shared/Test")
TEST_PROJ(true true "RenderTest" "Framework_shared" "_shared" "Shared/Test")


#TARGET_LINK_LIBRARIES(RenderTest_shared SDL2)
#TARGET_INCLUDE_DIRECTORIES(RenderTest_shared PRIVATE ${EXTERNAL_PATH}/SDL2/include)
