xcopy "%~dp0..\Game\resources" "%~dp0..\bin\Release-windows-x86_64\Game\resources" /s /y /i /c
xcopy "%~dp0..\Caramel\deps\assimp\bin\Release\assimp-vc141-mt.dll" "%~dp0..\bin\Release-windows-x86_64\Game" /s /y /i /c
xcopy "%~dp0..\Game\imgui.ini" "%~dp0..\bin\Release-windows-x86_64\Game" /s /y /i /c