@echo off

set CommonCompilerFlags=-Od -MT -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DDEV_BUILD=1 -FC -Z7
set CommonLinkerFlags= -opt:ref user32.lib gdi32.lib winmm.lib

pushd build
:: cl /? for options. -FC will display full path to the file
:: TODO: Add compile flags, optimisation, etc.
cl %CommonCompilerFlags% ..\win32_platform.c /link %CommonLinkerFlags%

popd