#!/bin/bash
# CMake into build directory
echo -e "\e[36m***Running CMake..."
cmake -G "Visual Studio 16 2019" -A x64 -B build || { echo "::error::CMake failed to generate the project!"; exit 1; }
python checkbuildlist.py
cd build
build_failed=0
# Compile labs based on build file
while read p; do
	echo -e "\e[36m***Building" $p
	cmake --build . --target $p --config Debug -- -v:q -clp:DisableConsoleColor | tee -a diagnostics.txt
	if [ "${PIPESTATUS[0]}" -ne "0" ] ; then
		echo "::error::Code for $p did not compile!"
		build_failed=1
	fi
done < ../BuildActual.txt

cd ..
./diagnostics-win.py
if [[ "$build_failed" == 1 ]] ; then
	exit 1
fi
