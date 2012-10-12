
@echo off

rem =========================================

set ROOT_DIR=%~dp0
set BUILD_DIR=build
set CURRENT_DIR=%CD%

rem =========================================

cd %ROOT_DIR% || goto End

if not exist %BUILD_DIR% ( 
  mkdir %BUILD_DIR% || goto End
)

cd %BUILD_DIR% || goto End
cmake -G "NMake Makefiles" .. || goto End
nmake || goto End

rem =========================================

:End

if %ERRORLEVEL% GTR 0 (
  echo Build failure.
  goto Exit
)

echo.
echo Successfully build
echo.
goto Exit

rem =========================================

:Exit

cd %CURRENT_DIR%

if %ERRORLEVEL% GTR 0 (
  exit /b 1 
) else (
  exit /b 0
)

