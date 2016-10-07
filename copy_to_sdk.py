import os, sys, shutil, glob
from build_util import *

def file_copy(src, dst, desc):
	try_create = False
	for fn in glob.glob(src + "/" + desc):
		if (not os.path.isdir(fn)) and fn.find("stdafx") == -1:
			if not try_create:
				if not os.path.isdir(dst):
					print("Making directory: " + dst + " ...")
					os.makedirs(dst)
				try_create = True
			shutil.copy(fn, dst)
	for fn in glob.glob(src + "/*"):
		if os.path.isdir(fn):
			sp = fn.split(src)
			nxt_dst = dst + sp[len(sp)-1]
			file_copy(fn, nxt_dst, desc)

def copy_to_sdk(bi):
	file_copy(build_cfg.external_path + "/fcontext/include", build_cfg.install_path + "/include", "*.h")
	file_copy(build_cfg.source_path, build_cfg.install_path + "/include", "*.h")
	file_copy(build_cfg.source_path, build_cfg.install_path + "/include", "*.inl")

	for info in bi.compilers:
		config_path = "%s%d_%s_%s" % (bi.compiler_name, bi.compiler_version, bi.target_platform, info.arch)
		lib_path = build_cfg.binary_path + "/" + config_path
		if os.path.isdir(lib_path):
			file_copy(lib_path, build_cfg.install_path + "/" + config_path, "*.lib")
			file_copy(lib_path, build_cfg.install_path + "/" + config_path, "*.dll")
			file_copy(lib_path, build_cfg.install_path + "/" + config_path, "*.pdb")
			file_copy(lib_path, build_cfg.install_path + "/" + config_path, "*.a")
			file_copy(lib_path, build_cfg.install_path + "/" + config_path, "*.so")
			file_copy(lib_path, build_cfg.install_path + "/" + config_path, "*.elf")

if __name__ == "__main__":
	cfg = cfg_from_argv(sys.argv)
	bi = build_info(cfg.compiler, cfg.archs, cfg.cfg)
	copy_to_sdk(bi)
