:: This script will copy the lib files to the working directory, call from prebuild

mkdir "%~dp0..\bin"

xcopy /Y "%~dp0..\bin\Debug-x64\Caramel\Caramel.dll" "%~dp0..\bin\Debug-x64\Sandbox"

pause