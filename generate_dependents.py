import os, sys, multiprocessing, subprocess
from build_util import *

def download_dependents(bi):
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

def generate_vulkan():
	curdir = os.path.abspath(os.curdir)
	os.chdir(curdir + "/" + build_cfg.dependent_path + "/vulkan_loader")
	cmake_cmd = batch_command(bi.host_platform)
	cmake_cmd.add_command("python vk_gen.py " + curdir + "/" + build_cfg.source_path + "/Venus3D/Vulkan")
	if cmake_cmd.execute() != 0:
		log_error("Config %s failed." % build_cfg.project_name)
	os.chdir(curdir)

def generate_dependents(bi):
	download_dependents(bi)
	generate_vulkan()

if __name__ == "__main__":
	cfg = cfg_from_argv(sys.argv)
	bi = build_info(cfg.compiler, cfg.archs, cfg.cfg)
	generate_dependents(bi)
