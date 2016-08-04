project_name		= "Venus3D"

binary_path			= "Binary"
build_path			= "Build"
dependent_path		= "Dependent"
document_path		= "Document"
external_path		= "External"
include_path		= "Include"
source_path			= "Source"
test_path			= "Test"
install_path		= "SDK"

dependent_list		= [("maker", "https://github.com/Napoleon314/maker"),("vulkan_loader", "https://github.com/Napoleon314/vulkan_loader")]

# Compiler name.
#   On Windows desktop, could be "vc140", "vc120", "vc110", "mingw", "auto".
#   On Windows store, could be "vc140", "vc120", "vc110", "auto".
#   On Windows phone, could be "vc140", "vc120", "vc110", "auto".
#   On Android, could be "gcc", "auto".
#   On Linux, could be "gcc", "auto".
#   On MacOSX, could be "clang", "auto".
#   On iOS, could be "clang", "auto".
compiler			= "auto"

# Toolset name.
#   On Windows desktop, could be "v140", "auto".
#   On Windows store, could be "auto".
#   On Windows phone, could be "auto".
#   On Android, could be "4.6", "4.8", "4.9", "auto".
#   On Linux, could be "auto".
#   On MacOSX, could be "auto".
#   On iOS, could be "auto".
toolset				= "auto"

# Target CPU architecture.
#   On Windows desktop, could be "x86", "x64".
#   On Windows store, could be "arm", "x86", "x64".
#   On Windows phone, could be "arm", "x86".
#   On Android, cound be "armeabi", "armeabi-v7a", "arm64-v8a", "x86", "x86_64".
#   On Linux, could be "x86", "x64".
#   On MacOSX, could be "x64".
#   On iOS, could be "arm", "x86".
arch				= ("x86", "x64")

# Configuration. Could be "Debug", "Release", "MinSizeRel", "RelWithDebInfo".
config				= ("Debug", "Release", "MinSizeRel", "RelWithDebInfo")

# Target platform for cross compiling. Could be "android" plus version number, "win_store", "win_phone" plus version number, "ios", or "auto".
target				= "auto"

intrinsics_level	= 1
