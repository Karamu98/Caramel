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

echo xcopy "%1.\resources\"*.* %2 /s /y /d /r 
xcopy "%~1.\resources\"*.* %2 /s /y /d /r 

@echo off
