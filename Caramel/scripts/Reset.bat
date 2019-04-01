xcopy "%~dp0..\resources" "%~dp0..\bin" /s /y /i
xcopy "%~dp0..\lib\assimp-vc140-mt.dll" "%~dp0..\bin" /y

start /d "%~dp0..\bin" CaramelEngine.exe