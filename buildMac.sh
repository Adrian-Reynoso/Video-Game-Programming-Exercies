#!/bin/bash
# CMake into build directory
echo -e "***Running CMake..."
cmake -G Xcode -B build || { echo "::error::CMake failed to generate the project!"; exit 1; }
python3 checkbuildlist.py
cd build
build_failed=0
# Compile labs based on build file
while read p; do
	echo -e "***Building $p..."
	cmake --build . --target $p --config Debug -- -quiet | tee -a diagnostics.txt 
  	if [ "${PIPESTATUS[0]}" -ne "0" ] ; then
		echo "::error::Code for $p did not compile!"
		build_failed=1
	fi
done < ../BuildActual.txt

cd ..
./diagnostics-mac.py
if [[ "$build_failed" == 1 ]] ; then
	exit 1
fi
