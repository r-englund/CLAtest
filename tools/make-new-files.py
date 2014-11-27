from __future__ import print_function

import os
import sys
import argparse
import re
import subprocess


if os.name == 'posix':
	SVN='/usr/bin/svn'
	CMAKE='cmake'
else:
	SVN='svn.exe'
	CMAKE='cmake.exe'


parser = argparse.ArgumentParser(description='Add new files to Inviwo.\n typical usage: \n python.exe ./make-new-files.py --svn --cmake ../build ../include/inviwo/path/to/h-file/MyNewClass', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('names', type=str, nargs='+', action="store", help='Classes to add, form: path/to/h-file/NewClassName')
parser.add_argument("--dummy", action="store_true", dest="dummy", help="Write local testfiles instead")
parser.add_argument("--svn", action="store_true", dest="svn", help="Add files to svn, and set file ending to native")
parser.add_argument("--cmake", type=str, nargs=1, action="store", dest="builddir", help="Rerun cmake in the specified build directory", default="")
parser.add_argument("--no-header", action="store_false", default=True, dest="header", help="Don't write header file")
parser.add_argument("--no-source", action="store_false", default=True, dest="source", help="Don't write source file")
parser.add_argument("--frag", action="store_true", dest="frag", help="Add fragment shader")
parser.add_argument("--vert", action="store_true", dest="vert", help="Add vertex shader")
parser.add_argument("--processor", action="store_true", dest="processor", default=False, help="Make a skeleton inviwo processor")


args = parser.parse_args()

def find_inv_path():
	path = os.path.abspath(sys.argv[0])
	print(path)
	folders=[]
	while 1:
		path, folder = os.path.split(path)
		if folder != "":
			folders.append(folder)
		else:
			if path != "":
				folders.append(path)
			break

	folders.reverse()
	
	basepath = ""
	for i in range(len(folders), 0 ,-1):
		if (os.path.exists(os.sep.join(folders[:i] + ['modules', 'base'])) 
		and os.path.exists(os.sep.join(folders[:i] + ['include', 'inviwo']))
		and os.path.exists(os.sep.join(folders[:i] + ['tools', 'templates']))):
			basepath = os.sep.join(folders[:i])
			break

	return basepath


def makeTemplate(ivwpath, file, name, define, api, incfile):
	lines = []
	with open(os.sep.join([ivwpath, 'tools', 'templates', file]),'r') as f:
		for line in f:
			line = line.replace("<name>", name)
			line = line.replace("<lname>", name.lower())
			line = line.replace("<uname>", name.upper())
			line = line.replace("<api>", api)
			line = line.replace("<define>", define)
			line = line.replace("<incfile>", incfile)
			lines.append(line)
		return "".join(lines)


def makeHeader(name, define, api):
	str = ""
	str+= "#ifndef IVW_" + name.upper() + "_H" + "\n"
	str+= "#define IVW_" + name.upper() + "_H" + "\n"
	str+= "\n"
	str+= "#include "+ define + "\n"
	str+= "#include <inviwo/core/common/inviwo.h>" + "\n"
	str+= "\n"
	str+= "namespace inviwo {" + "\n"
	str+= "\n"
	str+= "class " + api + " " + name + " { " + "\n"
	str+= "public:" + "\n"
	str+= "    " + name + "();" + "\n"
	str+= "    virtual ~" + name + "(){}" + "\n"
	str+= "};" + "\n"	
	str+= "\n"	
	str+= "} // namespace" + "\n"
	str+= "\n"
	str+= "#endif // IVW_" + name.upper() + "_H" + "\n"
	str+= "\n"
	return str
	
def makeSource(incfile):
	str = ""
	str+= "#include " + incfile + "\n"
	str+= "\n"
	str+= "namespace inviwo {" + "\n"
	str+= "\n"
	str+= "\n"	
	str+= "} // namespace" + "\n"
	str+= "\n"
	return str

def makeFrag():
	str = ""
	str += "void main(void) { \n"
	str += "    \n"
	str += "}\n"
	str += "\n"
	return str
	
def makeVert():
	str = ""
	str += "void main(void) { \n"
	str += "    \n"
	str += "}\n"
	str += "\n"
	return str
	
def findCMakeList(pathlist):
	for i in range(len(pathlist),0,-1):
		if os.path.exists(os.sep.join(pathlist[:i] + ["CMakeLists.txt"])):
			return pathlist[:i] + ["CMakeLists.txt"]
	return []

def addFileToCMakeLists(cmlines, group, file):
	m1 = re.compile(r"\s*set\(" + group + "\s*")
	m0 = re.compile(r"\s*\)\s*")
	it = iter(cmlines)
	
	def sortAndInsertLine(f, line):
		lines = []
		for l in f:
			if m0.match(l):
				lines.append(line)
				lines.sort()
				lines.append(l)
				break
			elif l.strip() != "":	
				lines.append(l.replace("\t","    "))
				
		return lines
		
	lines = []
	for line in it:
		if m1.match(line):
			lines.append(line)
			lines.extend(sortAndInsertLine(it, file))						
		else:
			lines.append(line)
			
	return lines

def addFileToSvn(file):
	mess = subprocess.Popen([SVN, "add", file], 
				stdout=subprocess.PIPE, 
				universal_newlines=True).stdout.read()
	for i in mess.splitlines():
		print("... " + i)
	mess = subprocess.Popen([SVN, "propset", "svn:eol-style", "native", file], 
				stdout=subprocess.PIPE, 
				universal_newlines=True).stdout.read()
	for i in mess.splitlines():
		print("... " + i)

	
print("Adding files to inwivo")

ivwpath = find_inv_path()
	
for name in args.names:
	(path, file)  = os.path.split(name)
	abspath = os.path.abspath(path).split(os.sep)

	if re.compile(r".*/include/inviwo/.*").match("/".join(abspath)):
		hpath = abspath
		newpath = []
		incfilepath = []
		for i in range(len(abspath)):
			if abspath[i] == "include" and abspath[i+1] == "inviwo":
				newpath.append("src")
				incfilepath = abspath[i+1:]
				newpath.extend(abspath[i+2:])
				break
			else:
				newpath.append(abspath[i])
			
		incfile = "<" + "/".join(incfilepath + [file.lower()+".h"] ) + ">"
		moddef = "<inviwo/core/common/inviwocoredefine.h>"
		cpath = newpath
		cmhfile = "    " + "/".join(["${IVW_INCLUDE_DIR}"] + incfilepath + [file.lower() +".h"])  + "\n"
		cmcfile = "    " + "/".join(incfilepath[2:] + [file.lower() +".cpp"]) + "\n"
		api = "IVW_CORE_API"
		
		
	elif re.compile(r".*/modules/.*").match("/".join(abspath)):
		mod="ERROR"
		incfilepath=[]
		for i in range(len(abspath)):
			if (abspath[i] == "Inviwo" or abspath[i] == "inviwo") and abspath[i+1] == "modules":
				mod = abspath[i+2]
				incfilepath = abspath[i+2:]
				break
				
		hpath = abspath
		cpath = abspath
		incfile = "\"" + file.lower()+".h" + "\""
		moddef = "<modules/" + mod + "/" + mod + "moduledefine.h>"
		cmhfile = "    " + "/".join(["${IVW_MODULE_DIR}"] + incfilepath + [file.lower() + ".h"]) + "\n"
		cmcfile = "    " + "/".join(["${IVW_MODULE_DIR}"] + incfilepath + [file.lower() + ".cpp"]) + "\n"
		cmvertfile = "    " + "/".join(["${IVW_MODULE_DIR}"] + incfilepath + [file.lower() + ".vert"]) + "\n"
		cmfragfile = "    " + "/".join(["${IVW_MODULE_DIR}"] + incfilepath + [file.lower() + ".frag"]) + "\n"
		api = "IVW_MODULE_"+mod.upper()+"_API"
	
	else:
		print("ERROR")
	
	hfilename = os.sep.join(hpath + [file.lower() + ".h"])
	cfilename = os.sep.join(cpath + [file.lower() + ".cpp"])
	vertfilename = os.sep.join(cpath + [file.lower() + ".vert"])
	fragfilename = os.sep.join(cpath + [file.lower() + ".frag"])
	
	cmakefile = os.sep.join(findCMakeList(cpath))
	
	if args.frag or args.vert:
		args.header = False
		args.source = False
	
	if args.frag or args.vert:
		if args.frag:
			print("... vert-file: " + hfilename)
		if args.vert:
			print("... frag-file: " + cfilename)
	else:
		print("Classname: " + file)
		print("... h-file: " + hfilename)
		print("... c-file: " + cfilename)
		print("... incfile: " + incfile)
	
	print("... cmake: " + cmakefile)
	 
	
	lines = []
	with open(cmakefile, "r") as f:
		for l in f:
			lines.append(l)
	
	
	if args.header:
		lines = addFileToCMakeLists(lines, "HEADER_FILES", cmhfile)

	if args.source:
		lines = addFileToCMakeLists(lines, "SOURCE_FILES", cmcfile)

	if args.frag:
		lines = addFileToCMakeLists(lines, "SHADER_FILES", cmfragfile)
	
	if args.vert:
		lines = addFileToCMakeLists(lines, "SHADER_FILES", cmvertfile)
	
	if(args.dummy):
		hfilename = file.lower() + ".h"
		cfilename = file.lower() + ".cpp"
		cmakefile = file.lower() + ".cmake"
		fragfilename = file.lower() + ".frag"
		vertfilename = file.lower() + ".vert"
	
	if os.path.exists(hfilename) and args.header:
		print("... ERROR file already exists: " + hfilename)
	elif os.path.exists(cfilename) and args.source:
		print("... ERROR file already exists: " + cfilename)
	else:
		if args.header:
			with open(hfilename, "w") as f:
				print("... Writing h-file: " + hfilename)
				if args.processor:
					f.write(makeTemplate(ivwpath, "processor.h", file, moddef, api, incfile))
				else:
					f.write(makeTemplate(ivwpath, "file.h", file, moddef, api, incfile))

		if args.source:
			with open(cfilename, "w") as f:
				print("... Writing c-file: " + cfilename)
				if args.processor:
					f.write(makeTemplate(ivwpath, "processor.cpp", file, moddef, api, incfile))
				else:
					f.write(makeTemplate(ivwpath, "file.cpp", file, moddef, api, incfile))

		if args.frag:
			with open(fragfilename, "w") as f:
				print("... Writing frag-file: " + fragfilename)
				f.write(makeFrag())

		if args.vert:
			with open(vertfilename, "w") as f:
				print("... Writing vert-file: " + vertfilename)
				f.write(makeVert())	
		
		with open(cmakefile, "w") as f:
			print("... Updating cmakelists: " + cmakefile)
			for l in lines:
				f.write(l)
	
		if args.svn:
			print("... Adding to svn...")
			# Do an svn add...
			if args.header:
				addFileToSvn(hfilename)
				
			if args.source:
				addFileToSvn(cfilename)
				
			if args.frag:
				addFileToSvn(fragfilename)
				
			if args.vert:
				addFileToSvn(vertfilename)
	
if(args.builddir != ""):
	print("... run cmake...")
	mess = subprocess.Popen(CMAKE + " " + str(args.builddir[0]), stdout=subprocess.PIPE, universal_newlines=True).stdout.read()
	for i in mess.splitlines():
		print("... " + i)








