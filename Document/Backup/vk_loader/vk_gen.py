import os, sys, shutil
from xml.dom import minidom

class vulkan_loader:
	def __init__(self, xml):
		try:
			self.xml_root = minidom.parse(xml).documentElement
		except:
			print("Parse xml spec error.")
			exit()

	def add_line_vulkan_h(self, line):
		self.vulkan_h_content += line + "\n"

	def add_comments(self):
		print("\tadd comments for vulkan.h")
		self.add_line_vulkan_h("////////////////////////////////////////////////////////////////////////////")
		for line in self.xml_root.getElementsByTagName("comment")[0].childNodes[0].data.split('\n'):
			self.add_line_vulkan_h("//  " + line)
		self.add_line_vulkan_h("////////////////////////////////////////////////////////////////////////////")
		self.add_line_vulkan_h("")

	def add_type(self, type_element):
		category = type_element.getAttribute("category")
		if category == "include":
			if type_element.getAttribute("name") == "vk_platform":
				self.add_line_vulkan_h("#define VK_VERSION_1_0 1")
				self.add_line_vulkan_h(type_element.firstChild.data)
				self.add_line_vulkan_h("")
		if category == "define" or category == "basetype" or category == "bitmask" \
			or category == "handle" or category == "funcpointer":
			line = ""
			for i in range(type_element.childNodes.length):
				node = type_element.childNodes[i]
				if node.nodeType == 3:
					line += node.data
				elif node.nodeType == 1:
					line += node.firstChild.data
			self.add_line_vulkan_h(line)
			self.add_line_vulkan_h("")
		elif category == "struct":
			self.add_line_vulkan_h("typedef struct {")
			for member in type_element.getElementsByTagName("member"):
				line = "\t"
				for i in range(member.childNodes.length):
					node = member.childNodes[i]
					if node.nodeType == 3:
						line += node.data
					elif node.nodeType == 1:
						line += node.firstChild.data
				self.add_line_vulkan_h(line + ";")

			self.add_line_vulkan_h("} " + type_element.getAttribute("name") + ";")
			self.add_line_vulkan_h("")

	def add_types(self):
		print("\tadd types for vulkan.h")
		types = self.xml_root.getElementsByTagName("types")[0].getElementsByTagName("type")
		for i in range(types.length):
			self.add_type(types[i])

	def add_enums(self):
		print("\tadd enums for vulkan.h")
		for enums in self.xml_root.getElementsByTagName("enums"):
			line = "/* " + enums.getAttribute("name");
			if enums.hasAttribute("comment"):
				line += ": " + enums.getAttribute("comment")
			line += " */"
			self.add_line_vulkan_h(line)
			for enum in enums.getElementsByTagName("enum"):
				if enums.hasAttribute("comment"):
					self.add_line_vulkan_h("/* " + enums.getAttribute("comment") + " */")
				self.add_line_vulkan_h("#define " + enum.getAttribute("name") + " " + enum.getAttribute("value"))
			self.add_line_vulkan_h("")

	def add_param(self, param):
		line = " * @param "
		for name in param.getElementsByTagName("name"):
			line += name.firstChild.data
			break
		for name in param.getElementsByTagName("type"):
			line += " type(" + name.firstChild.data + ")"
			break
		if param.hasAttribute("optional"):
			line += " optional(" + param.getAttribute("optional") + ")"
		if param.hasAttribute("externsync"):
			line += " externsync(" + param.getAttribute("externsync") + ")"
		if param.hasAttribute("externsync"):
			line += " len(" + param.getAttribute("externsync") + ")"
		self.add_line_vulkan_h(line)
		ret = ""
		for i in range(param.childNodes.length):
			node = param.childNodes[i]
			if node.nodeType == 3:
				ret += node.data
			elif node.nodeType == 1:
				ret += node.firstChild.data
		return ret

	def add_commands(self):
		print("\tadd commands for vulkan.h")
		for command in self.xml_root.getElementsByTagName("commands")[0].getElementsByTagName("command"):
			param_list = ""
			self.add_line_vulkan_h("/**")
			self.add_line_vulkan_h(" * Function: " + command.getElementsByTagName("proto")[0].getElementsByTagName("name")[0].firstChild.data)
			first = True;
			for param in command.getElementsByTagName("param"):
				if first:
					first = False
				else:
					param_list += ", "
				param_list += self.add_param(param)
			line = " * @return type(" + command.getElementsByTagName("proto")[0].getElementsByTagName("type")[0].firstChild.data + ")"
			if command.hasAttribute("successcodes"):
				line += " successcodes(" + command.getAttribute("successcodes") + ")"
			if command.hasAttribute("errorcodes"):
				line += " errorcodes(" + command.getAttribute("errorcodes") + ")"
			self.add_line_vulkan_h(line)
			for validity in command.getElementsByTagName("validity"):
				for usage in validity.getElementsByTagName("usage"):
					self.add_line_vulkan_h(" * @useage " + usage.firstChild.data)
			self.add_line_vulkan_h(" */")
			self.add_line_vulkan_h("")

			ret_type = command.getElementsByTagName("proto")[0].getElementsByTagName("type")[0].firstChild.data
			func_name = command.getElementsByTagName("proto")[0].getElementsByTagName("name")[0].firstChild.data

			self.add_line_vulkan_h("typedef " + ret_type + "(VKAPI_PTR *PFN_" + func_name + ")(" + param_list + ");")
			self.add_line_vulkan_h("")
			self.add_line_vulkan_h("#ifndef VK_NO_PROTOTYPES")
			self.add_line_vulkan_h("VKAPI_ATTR " + ret_type + " VKAPI_CALL " + func_name + "(" + param_list + ");")
			self.add_line_vulkan_h("#else")
			self.add_line_vulkan_h("extern PFN_" + func_name + " _" + func_name + ";")
			self.add_line_vulkan_h("#define " + func_name + " _" + func_name)
			self.add_line_vulkan_h("#endif")
			self.add_line_vulkan_h("")
			self.add_line_vulkan_h("")

	def add_extensions(self):
		print("\tadd extensions for vulkan.h")
		for ext in self.xml_root.getElementsByTagName("extensions")[0].getElementsByTagName("extension"):
			self.add_line_vulkan_h("/**")
			self.add_line_vulkan_h(" * " + ext.getAttribute("name"))
			for func in ext.getElementsByTagName("require")[0].getElementsByTagName("command"):
				self.add_line_vulkan_h(" * require: " + func.getAttribute("name"))
			self.add_line_vulkan_h(" */")
			self.add_line_vulkan_h("#define " + ext.getAttribute("name") + " " + ext.getAttribute("number"))

			for enum in ext.getElementsByTagName("require")[0].getElementsByTagName("enum"):
				#self.add_line_vulkan_h("#define " + enum.getAttribute("name") + " " + )
				print(enum.getAttribute("name"))

	def create_vulkan_h(self):
		print("generating vulkan.h...")
		self.vulkan_h_content = ""
		self.add_line_vulkan_h("#ifndef VULKAN_H_")
		self.add_line_vulkan_h("#define VULKAN_H_ 1")
		self.add_line_vulkan_h("")
		self.add_line_vulkan_h("#ifdef __cplusplus")
		self.add_line_vulkan_h("extern \"C\" {")
		self.add_line_vulkan_h("#endif")
		self.add_line_vulkan_h("")
		self.add_comments()
		self.add_types()
		self.add_enums()
		self.add_commands()
		self.add_extensions()
		self.add_line_vulkan_h("")
		self.add_line_vulkan_h("#ifdef __cplusplus")
		self.add_line_vulkan_h("}")
		self.add_line_vulkan_h("#endif")
		self.add_line_vulkan_h("")
		self.add_line_vulkan_h("#endif")
		print("vulkan.h has been generated.")

	def add_line_vulkan_loader_h(self, line):
		self.vulkan_loader_h_content += line + "\n"

	def add_commands_vulkan_loader(self):
		print("\tadd commands for vulkan_loader.h")
		for command in self.xml_root.getElementsByTagName("commands")[0].getElementsByTagName("command"):
			param_list = ""
			self.add_line_vulkan_loader_h("/**")
			self.add_line_vulkan_loader_h(" * Function: " + command.getElementsByTagName("proto")[0].getElementsByTagName("name")[0].firstChild.data)
			first = True;
			for param in command.getElementsByTagName("param"):
				if first:
					first = False
				else:
					param_list += ", "
				param_list += self.add_param(param)
			line = " * @return type(" + command.getElementsByTagName("proto")[0].getElementsByTagName("type")[0].firstChild.data + ")"
			if command.hasAttribute("successcodes"):
				line += " successcodes(" + command.getAttribute("successcodes") + ")"
			if command.hasAttribute("errorcodes"):
				line += " errorcodes(" + command.getAttribute("errorcodes") + ")"
			self.add_line_vulkan_loader_h(line)
			for validity in command.getElementsByTagName("validity"):
				for usage in validity.getElementsByTagName("usage"):
					self.add_line_vulkan_loader_h(" * @useage " + usage.firstChild.data)
			self.add_line_vulkan_loader_h(" */")
			self.add_line_vulkan_loader_h("")

			ret_type = command.getElementsByTagName("proto")[0].getElementsByTagName("type")[0].firstChild.data
			func_name = command.getElementsByTagName("proto")[0].getElementsByTagName("name")[0].firstChild.data

			self.add_line_vulkan_loader_h("typedef " + ret_type + "(VKAPI_PTR *PFN_" + func_name + ")(" + param_list + ");")
			self.add_line_vulkan_loader_h("")
			self.add_line_vulkan_loader_h("#ifndef VK_NO_PROTOTYPES")
			self.add_line_vulkan_loader_h("VKAPI_ATTR " + ret_type + " VKAPI_CALL " + func_name + "(" + param_list + ");")
			self.add_line_vulkan_loader_h("#else")
			self.add_line_vulkan_loader_h("extern PFN_" + func_name + " _" + func_name + ";")
			self.add_line_vulkan_loader_h("#define " + func_name + " _" + func_name)
			self.add_line_vulkan_loader_h("#endif")
			self.add_line_vulkan_loader_h("")
			self.add_line_vulkan_loader_h("")

	def create_vulkan_loader(self):
		print("generating vulkan_loader.h...")
		self.add_commands_vulkan_loader()

	def save_vulkan_loader(self, path):
		print("saving vulkan_loader.h...")
		vulkan_loader_h = open(path + "/vulkan_loader.h", "w")
		vulkan_loader_h.write(self.vulkan_loader_h_content)
		vulkan_loader_h.close()

	def save(self, path):
		if path == ".":
			target_path = "vulkan"
		else:
			target_path = path
		if not os.path.isdir(target_path):
			print("Making directory: " + target_path + " ...")
			os.makedirs(target_path)
		#self.create_vulkan_h()
		#vulkan_h = open(target_path+'/vulkan.h', 'w')
		#vulkan_h.write(self.vulkan_h_content)
		#vulkan_h.close()
		print("copying vk_platform.h to " + target_path)
		shutil.copyfile("vk_platform.h", target_path + "/vk_platform.h")
		print("copying vulkan.h to " + target_path)
		shutil.copyfile("vulkan.h", target_path + "/vulkan.h")
		self.create_vulkan_loader()

if __name__ == "__main__":
	vk = vulkan_loader("vk.xml")
	if len(sys.argv) > 1:
		vk.save(sys.argv[1])
	else:
		vk.save(".")
