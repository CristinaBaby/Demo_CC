#!/usr/bin/python
# xcode_project_rename.py
# Copyright (c) tsm
# Author: tsm

# define global variables

import sys
import os
import shutil

context = {
"project_path" : "undefined",
"project_old_name": "undefined",
"project_new_name" : "undefined"
}

fileNames = {
  "rename": [
    "PROJECT_NAME.xcodeproj"
  ],
  "remove": [
    "PROJECT_NAME.xcodeproj/project.xcworkspace",
    "PROJECT_NAME.xcodeproj/xcuserdata"
  ],
  "replace_package_name": [
    "Info.plist"
  ],
  "replace_project_name": [
    "Info.plist",
    "PROJECT_NAME.xcodeproj/project.pbxproj"
  ]
}

def checkParams(context):
    # generate our internal params
    if(len(sys.argv) < 3):
        sys.exit()
    # find our params
    context["project_path"] = sys.argv[1]
    if(sys.argv[1][-1] != '/'):
        context["project_path"] = context["project_path"] + "/"
    context["project_old_name"] = sys.argv[2]
    context["project_new_name"] = sys.argv[3]
                                 
# end of checkParams(context) function

def replaceString(filepath, src_string, dst_string):
    content = ""
    f1 = open(filepath, "rb")
    for line in f1:
        if src_string in line:
            content += line.replace(src_string, dst_string)
        else:
            content += line
    f1.close()
    f2 = open(filepath, "wb")
    f2.write(content)
    f2.close()
# end of replaceString


def processPlatformProjects(platform):
    # determine proj_path
    proj_path = context["project_path"]
    java_package_path = ""

    # rename files and folders
    for i in range(0, len(fileNames["rename"])):
        tmp = fileNames["rename"][i].replace("PACKAGE_PATH", java_package_path)
        src = tmp.replace("PROJECT_NAME", context["project_old_name"])
        dst = tmp.replace("PROJECT_NAME", context["project_new_name"])
        print "rename %s to %s" %(proj_path + src , proj_path + dst)
        if (os.path.exists(proj_path + src) == True):
            print "rename %s to %s" % (proj_path + src,proj_path + dst)
            os.rename(proj_path + src, proj_path + dst)

    # remove useless files and folders
    for i in range(0, len(fileNames["remove"])):
        dst = fileNames["remove"][i].replace("PROJECT_NAME", context["project_new_name"])
        if (os.path.exists(proj_path + dst) == True):
            shutil.rmtree(proj_path + dst)
    
    # rename package_name. This should be replaced at first. Don't change this sequence
    #for i in range(0, len(data["replace_package_name"])):
    #    tmp = data["replace_package_name"][i].replace("PACKAGE_PATH", java_package_path)
    #    dst = tmp.replace("PROJECT_NAME", context["dst_project_name"])
    #    if (os.path.exists(proj_path + dst) == True):
    #        replaceString(proj_path + dst, context["src_package_name"], context["dst_package_name"])
    
    # rename project_name
    for i in range(0, len(fileNames["replace_project_name"])):
        tmp = fileNames["replace_project_name"][i].replace("PACKAGE_PATH", java_package_path)
        dst = tmp.replace("PROJECT_NAME", context["project_new_name"])
        if (os.path.exists(proj_path + dst) == True):
            replaceString(proj_path + dst, context["project_old_name"], context["project_new_name"])
                  
    # done!
    print "proj.%s\t\t: Done!" % platform
# end of processPlatformProjects


checkParams(context)
processPlatformProjects("ios")
