import subprocess
import os, sys

BASE_DIR = os.path.dirname(os.path.dirname(__file__))

def common_clear(submodule_directory, version):
	common_shell = ['cd '+ submodule_directory,
	'git checkout ' + version,
	'cd ..',
	'git add '+submodule_directory,
	'git commit -m "moved ' + submodule_directory + ' to ' + version + '"',
	'git push']
	dr = submodule_directory
	os.chdir(dr)
	os.system("git checkout " + version)
	os.chdir("..")
	os.system('git add '+submodule_directory)
	os.system('git commit -m "moved ' + submodule_directory + ' to ' + version + '"')
	os.system('git push')

if __name__ == '__main__':
	""" change commands and add shell"""
	commands = [["git submodule add ssh://git@stash.stm.com:7999/cml/audioutil.git audio", 'v1.0'],
	["git submodule add ssh://git@stash.stm.com:7999/cml/datamanager.git datamanager", 'null'],
	["git submodule add ssh://git@stash.stm.com:7999/cml/layerext.git layerext", 'v1.0_v3.8.1_framework'],
    ["git submodule add git@github.stm.com:3rd-party/cross_platform_api-public.git platform", 'V1.3.5'],
    ["git submodule add ssh://git@stash.stm.com:7999/cml/scenemanager.git scenemanager",'null'],
    ["git submodule add ssh://git@stash.stm.com:7999/cml/screenadapter.git screenadapter", 'v1.0'],
    ["git submodule add ssh://git@stash.stm.com:7999/cml/scribble.git scribble", 'v2.2.3.19'],
    ["git submodule add ssh://git@stash.stm.com:7999/cml/cocosxutils.git utils", 'v1.0']]

	for cmd in commands:
		p = subprocess.Popen(cmd[0], stdout=subprocess.PIPE, env=os.environ, shell=True)
		while True:
			line = p.stdout.readline()
			if not line:
				break
			print line
		err = p.wait()
		if err != 0:
			print "error shell: ", cmd, "git submodule failed"
		common_clear(cmd[0].split(' ')[-1], cmd[1])