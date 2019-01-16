:: Quick script to update the header and library of framework for this project if it were changed

xcopy /Y "%~dp0..\..\Framework\lib\framework.lib" "%~dp0..\lib"
xcopy /Y "%~dp0..\..\Framework\lib\FBX_Loader.lib" "%~dp0..\lib"

xcopy /Y "%~dp0..\..\Framework\framework\include\Application.h" "%~dp0..\include"
xcopy /Y "%~dp0..\..\Framework\framework\include\Error.h" "%~dp0..\include"
xcopy /Y "%~dp0..\..\Framework\framework\include\Gizmos.h" "%~dp0..\include"
xcopy /Y "%~dp0..\..\Framework\framework\include\Utilities.h" "%~dp0..\include"

pause