cls

:: change directory to that of this file (in case this bat file was called from another path )
:: so the pushd and popd calls later refer to the root tac dir
pushd "%~dp0"

:: loop label for goto
:loop

:: change directory from tac/run to tac/ before running cmake
pushd ..
:: cmake                   -G "Visual Studio 17 2022" -A x64 -B build_vs2022

:: generate project files
:: -WDev enables developer warnings
cmake --log-level=VERBOSE -G "Visual Studio 17 2022" -A x64 -B build_vs2022 -Wdev

:: build the project
:: cmake --build build_vs2022

popd

:: user input triggers each loop iteration
pause

:: cls at end (instead of at beginning) to show vcvars
cls
goto :loop
