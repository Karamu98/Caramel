xcopy "%~dp0..\resources" "%~dp0..\bin" /s /y /i /c
xcopy "%~dp0..\lib\assimp-vc140-mt.dll" "%~dp0..\bin" /y /c

start /d "%~dp0..\bin" CaramelEngine.exe

pause