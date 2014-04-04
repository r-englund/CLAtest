from __future__ import print_function

import os
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
parser.add_argument("--no-header", action="store_true", dest="no_header", help="Don't write header file")
parser.add_argument("--no-source", action="store_true", dest="no_source", help="Don't write source file")

args = parser.parse_args()

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

def findCMakeList(pathlist):
	for i in range(len(pathlist),0,-1):
		if os.path.exists(os.sep.join(pathlist[:i] + ["CMakeLists.txt"])):
			return pathlist[:i] + ["CMakeLists.txt"]
	return []
	
def updateCMakeLists(cmakefile, hfile, cfile):
	with open(cmakefile, "r") as cm:
		m1 = re.compile(r"\s*set\(HEADER_FILES\s*")
		m2 = re.compile(r"\s*set\(SOURCE_FILES\s*")
		m0 = re.compile(r"\s*\)\s*")
		
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
		for line in cm:
			if hfile != "" and m1.match(line):
				lines.append(line)
				lines.extend(sortAndInsertLine(cm, hfile))						
			elif cfile != "" and m2.match(line):
				lines.append(line)
				lines.extend(sortAndInsertLine(cm, cfile))
			else:
				lines.append(line)
	return lines
	
print("Adding files to inwivo")
	
for name in args.names:
	(path, file)  = os.path.split(name)
	abspath = os.path.abspath(path).split(os.sep)

	if re.compile(r".*/[Ii]nviwo/include/inviwo/.*").match("/".join(abspath)):
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
		
		
	elif re.compile(r".*/[Ii]nviwo/modules/.*").match("/".join(abspath)):
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
		api = "IVW_MODULE_"+mod.upper()+"_API"
	
	else:
		print("ERROR")
	
	hfilename = os.sep.join(hpath + [file.lower() + ".h"])
	cfilename = os.sep.join(cpath + [file.lower() + ".cpp"])
	cmakefile = os.sep.join(findCMakeList(cpath))
		
	print("Classname: " + file)
	print("... h-file: " + hfilename)
	print("... c-file: " + cfilename)
	print("... incfile: " + incfile)
	print("... cmake: " + cmakefile)
	 
	
	if args.no_header:
		cmhfile = ""
	if args.no_source:
		cmcfile = ""
	lines = updateCMakeLists(cmakefile, cmhfile, cmcfile)	
	
	if(args.dummy):
		hfilename = file.lower() + ".h"
		cfilename = file.lower() + ".cpp"
		cmakefile = file.lower() + ".cmake"
	
	if os.path.exists(hfilename) and not args.no_header:
		print("... ERROR file already exists: " + hfilename)
	elif os.path.exists(cfilename) and not args.no_source:
		print("... ERROR file already exists: " + cfilename)
	else:
		if not args.no_header:
			with open(hfilename, "w") as f:
				print("... Writing h-file: " + hfilename)
				f.write(makeHeader(file, moddef, api))
		if not args.no_source:
			with open(cfilename, "w") as f:
				print("... Writing c-file: " + cfilename)
				f.write(makeSource(incfile))		
		with open(cmakefile, "w") as f:
			print("... Updating cmakelists: " + cmakefile)
			for l in lines:
				f.write(l)
	
		if(args.svn):
			print("... Adding to svn...")
			# Do an svn add...
			if not args.no_header:
				mess = subprocess.Popen(SVN + " add " + hfilename, stdout=subprocess.PIPE, universal_newlines=True).stdout.read()
				for i in mess.splitlines():
					print("... " + i)
				mess = subprocess.Popen(SVN + " propset svn:eol-style native " + hfilename, stdout=subprocess.PIPE, universal_newlines=True).stdout.read()
				for i in mess.splitlines():
					print("... " + i)
				
			if not args.no_source:
				mess = subprocess.Popen(SVN + " add " + cfilename, stdout=subprocess.PIPE, universal_newlines=True).stdout.read()
				for i in mess.splitlines():
					print("... " + i)
				mess = subprocess.Popen(SVN + " propset svn:eol-style native " + cfilename, stdout=subprocess.PIPE, universal_newlines=True).stdout.read()
				for i in mess.splitlines():
					print("... " + i)
	
if(args.builddir != ""):
	print("... run cmake...")
	mess = subprocess.Popen(CMAKE + " " + str(args.builddir[0]), stdout=subprocess.PIPE, universal_newlines=True).stdout.read()
	for i in mess.splitlines():
		print("... " + i)








