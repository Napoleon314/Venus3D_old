import os, sys, multiprocessing, subprocess
from build_util import *

if __name__ == "__main__":
	cfg = cfg_from_argv(sys.argv)
	bi = build_info(cfg.compiler, cfg.archs, cfg.cfg)
	print("Starting build project: " + build_cfg.project_name + " ...")
	print("Downloading dependents ...")

	if not os.path.isdir(build_cfg.dependent_path):
		print("Making directory: " + build_cfg.dependent_path + " ...")
		os.makedirs(build_cfg.dependent_path)

	curdir = os.path.abspath(os.curdir)

	for dependent in build_cfg.dependent_list:
		if os.path.isdir(build_cfg.dependent_path + "/" + dependent[0]):
			os.chdir(curdir + "/" + build_cfg.dependent_path + "/" + dependent[0])
			cmake_cmd = batch_command(bi.host_platform)
			cmake_cmd.add_command("git pull")
			if cmake_cmd.execute() != 0:
				log_error("Config %s failed." % build_cfg.project_name)
		else:
			os.chdir(curdir + "/" + build_cfg.dependent_path)
			print("Cloning " + dependent[0] + ":")
			cmake_cmd = batch_command(bi.host_platform)
			cmake_cmd.add_command("git clone " + dependent[1] + " " + dependent[0])
			if cmake_cmd.execute() != 0:
				log_error("Config %s failed." % build_cfg.project_name)
		os.chdir(curdir)

	os.chdir(curdir + "/" + build_cfg.dependent_path + "/vulkan_loader")
	cmake_cmd = batch_command(bi.host_platform)
	cmake_cmd.add_command("python vk_gen.py " + curdir + "/" + build_cfg.source_path + "/Venus3D/Vulkan")
	if cmake_cmd.execute() != 0:
		log_error("Config %s failed." % build_cfg.project_name)
	os.chdir(curdir)

	additional_options = "-DCFG_PROJECT_NAME:STRING=\"%s\"" % build_cfg.project_name
	additional_options += " -DCFG_BINARY_PATH:STRING=\"%s\"" % build_cfg.binary_path
	additional_options += " -DCFG_BUILD_PATH:STRING=\"%s\"" % build_cfg.build_path
	additional_options += " -DCFG_DEPENDENT_PATH:STRING=\"%s\"" % build_cfg.dependent_path
	additional_options += " -DCFG_DOCUMENT_PATH:STRING=\"%s\"" % build_cfg.document_path
	additional_options += " -DCFG_EXTERNAL_PATH:STRING=\"%s\"" % build_cfg.external_path
	additional_options += " -DCFG_INCLUDE_PATH:STRING=\"%s\"" % build_cfg.include_path
	additional_options += " -DCFG_SOURCE_PATH:STRING=\"%s\"" % build_cfg.source_path
	additional_options += " -DCFG_TEST_PATH:STRING=\"%s\"" % build_cfg.test_path
	additional_options += " -DCFG_INSTALL_PATH:STRING=\"%s\"" % build_cfg.install_path
	additional_options += " -DCFG_INTRINSICS_LEVEL:STRING=\"%d\"" % build_cfg.intrinsics_level

	print("Building %s..." % (build_cfg.project_name))
	for info in bi.compilers:
		build_project(build_cfg.project_name, build_cfg.build_path, bi, info, info.is_windows, additional_options)
