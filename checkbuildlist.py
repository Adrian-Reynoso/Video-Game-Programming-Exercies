import os, subprocess,sys

files = subprocess.check_output(["git", "diff-tree", "--no-commit-id", "--name-only", "-r", "HEAD"])
#print(files)
files = files.decode("utf-8")
fileSplit = files.split('\n')
filtered = list(filter(lambda file: "Lab" in file, fileSplit))
#print(filtered)

build_file = open("Build.txt", "r")
build_contents = build_file.readlines()
#print(build_contents)
build_list = []
for build_temp in build_contents:
	build_list.append(build_temp.strip())
#print(build_list)

for file_filter in filtered:
	file_filter_splits = file_filter.split('/')
	if file_filter_splits[0] not in build_list:
		print('::warning::' + file_filter_splits[0] + ' modified but not in Build.txt. Adding to build. Please update Build.txt to include all projects you want to build.')
		build_list.append(file_filter_splits[0])
		
build_list.sort()
#print(build_list)
out_file = open("BuildActual.txt", "w", newline='\n')
for build_temp in build_list:
	out_file.write(build_temp + '\n')
