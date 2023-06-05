@echo off
mkdir ..\build
pushd ..\build
cl ..\src\main.cpp /I ..\src\SDL2-2.26.5\include ..\src\SDL2-2.26.5\lib\x64\SDL2.lib ..\src\SDL2-2.26.5\lib\x64\SDL2main.lib shell32.lib opengl32.lib /link /MACHINE:X64 /SUBSYSTEM:CONSOLE
popd