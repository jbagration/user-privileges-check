@echo off
REM Check if Node.js is installed
where node >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (
    echo Node.js is not installed or not in your PATH.
    echo Please install Node.js from https://nodejs.org/
    exit /b 1
)

REM Navigate to the project directory
cd /d "%~dp0"

REM Install dependencies if not already installed
if not exist node_modules (
    echo Installing dependencies...
    npm install
)

REM Start the server
echo Starting the server...
node index.js

pause
