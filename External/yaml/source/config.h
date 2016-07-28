#define YAML_VERSION_MAJOR 0
#define YAML_VERSION_MINOR 1
#define YAML_VERSION_PATCH 5
#define YAML_VERSION_STRING "0.1.5"

#if defined(_MSC_VER)
#	pragma warning(disable: 4996)
#	pragma warning(disable: 4224)
#	pragma warning(disable: 4245)
#	pragma warning(disable: 4244)
#	pragma warning(disable: 4706)
#	pragma warning(disable: 4100)
#	pragma warning(disable: 4456)
#	pragma warning(disable: 4267)
#	pragma warning(disable: 4457)
#	pragma warning(disable: 4701)
#	pragma warning(disable: 4702)
#elif defined(__clang__)
#	pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#	pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#	pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#	pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#	pragma GCC diagnostic ignored "-Wunused-parameter"
#	pragma GCC diagnostic ignored "-Wunused-value"
#endif
