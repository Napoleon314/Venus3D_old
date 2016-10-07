import os, sys, multiprocessing, subprocess
from build_util import *

if __name__ == "__main__":
	cfg = cfg_from_argv(sys.argv)
	bi = build_info(cfg.compiler, cfg.archs, cfg.cfg)
	print("Starting build project: " + build_cfg.project_name + " ...")

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

	print("Generating %s..." % (build_cfg.project_name))
	for info in bi.compilers:
		build_project(build_cfg.project_name, build_cfg.build_path, bi, "../cmake", info, False, False, additional_options)
