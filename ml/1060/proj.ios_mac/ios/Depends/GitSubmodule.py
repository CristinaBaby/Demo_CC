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
	commands = [
	["git submodule add ssh://git@stash.stm.com:7999/cmc/about-page-ios.git adboutPage", 'V2.2.3'],
    ["git submodule add ssh://git@stash.stm.com:7999/cmc/studio-resources.git adboutPageResources", 'develop'],
	["git submodule add ssh://git@stash.stm.com:7999/cmc/iapurchase-ios.git iap", 'V1.0.5'],
	["git submodule add ssh://git@stash.stm.com:7999/cmc/newsblast-ios.git newsblast", 'V2.3.2'],
	["git submodule add ssh://git@stash.stm.com:7999/cmc/adssdk-ios.git adsdk", 'production-Mopub-v1.2.1'],
    ["git submodule add ssh://git@stash.stm.com:7999/cmc/analyticssdk-ios.git analytics", 'test-V1.0.2'],
    ["git submodule add ssh://git@stash.stm.com:7999/cmf/cocos2d-x-static-library-ios.git cocos2dxStatic", 'lib-ios-v3.8.1-m-1.0.1']]
	

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