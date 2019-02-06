@echo off
goto :CopyResources
==========================================================================================
CopyResources
  Usage:
    This script sets copies the required files for the project into the Directory specified
	by the first parameter passed into the script

  Notes:
   

==========================================================================================
:CopyResources

xcopy "%~dp0..\resources" "%~dp0..\bin" /S /Y
xcopy "%dp0..\lib\assimp-vc140-mt.dll" "%~dp0..\bin" /Y