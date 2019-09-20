@echo off

pushd build
cl -Zi ..\main.c user32.lib gdi32.lib

popd