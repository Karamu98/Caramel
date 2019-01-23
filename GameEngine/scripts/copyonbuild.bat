:: This script will copy the lib files to the working directory, call from prebuild

mkdir "%~dp0..\bin"

xcopy /Y "%~dp0..\lib\framework.lib" "%~dp0..\bin"
xcopy /Y "%~dp0..\lib\FBX_Loader.lib" "%~dp0..\bin"
xcopy /Y "%~dp0..\lib\assimp-vc140-mt.lib" "%~dp0..\bin"

pause