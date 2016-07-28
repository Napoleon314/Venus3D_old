CORE_LIB(true "Venus3D" "fcontext;yaml" "_shared" "Shared")
SHELL_LIB(true "Framework" "Venus3D_shared" "_shared" "Shared")
TEST_PROJ(true false "PowerTest" "Venus3D_shared" "_shared" "Shared/Test")
TEST_PROJ(true true "RenderTest" "Framework_shared" "_shared" "Shared/Test")
