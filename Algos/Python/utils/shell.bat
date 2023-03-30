@echo off
set pythonExe=%1
set inputScript=%2
set parameter1=%3
set parameter2=%4
set parameter3=%5
set workingDirectory="%cd%"
set projectName=script batch shell

title %projectName%.
echo %projectName%.
echo ---------Input Paths------------
echo Working Directory:  %workingDirectory%
echo Python directory:   %pythonExe%
echo Parser script:      %inputScript%
echo parameter1:         %parameter1%
echo parameter2:         %parameter2%
echo parameter3:         %parameter3%
echo --------------------------------

GOTO PreExecuteProcedure

:PreExecuteProcedure
    if not exist %pythonExe%  (
        echo python .exe not Found: %pythonExe%
        echo looking for system python installation...
        python --version || GOTO FileError
        set pythonExe=python
    )
    if not exist %inputScript% GOTO FileError
GOTO ExecutePyScript

:FileError
    echo -----------Error!---------------
    echo Error occured!
    if not exist %pythonExe%   echo Python .exe file not found!: %pythonExe%
    if not exist %inputScript% echo Python parser script file not found!: %inputScript%
    IF %ERRORLEVEL% NEQ 0 echo ErrorLevel Is not Equal to zero!
    echo ErrorLevel Code: %ERRORLEVEL%
    echo --
    pause
GOTO EndOfScript

:ExecutePyScript
    set ExeCommand="%projectName%" /D %workingDirectory% /B /W %pythonExe% %inputScript% %parameter1% %parameter2% %parameter3%
    echo Executing... 'start %ExeCommand%'
    echo ----e-x-e-c-u-t-i-o-n-----------
    start %ExeCommand%
    echo ----s-c-r-i-p-t--d-o-n-e--------
    IF %ERRORLEVEL% NEQ 0 GOTO FileError
GOTO EndOfScript


:EndOfScript
    echo ---------Script End-------------
    exit
