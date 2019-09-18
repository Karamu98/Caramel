xcopy "%~dp0..\Game\resources" "%~dp0..\bin\Debug-windows-x86_64\Game\resources" /s /y /i /c
xcopy "%~dp0..\Game\imgui.ini" "%~dp0..\bin\Debug-windows-x86_64\Game" /s /y /i /c
xcopy "%~dp0..\Game\imgui.ini" "%~dp0" /s /y /i /c

call "%~dp0..\bin\Debug-windows-x86_64\Game\Game.exe