@echo off

REM ---------------------------------------------------------------------------
REM test the existance of environment variable MAINPROJECT
if "%MAINPROJECT%" EQU "" goto ERROR_MISSING_MAINPROJECT

REM ---------------------------------------------------------------------------
REM Proceed with creation of links

if "%BOOST_ROOT%" NEQ "" copy "%MAINPROJECT%"\Libraries\CMakeScripts\BOOSTConfig.cmake "%BOOST_ROOT%"

if "%CLEye_ROOT%" NEQ "" copy "%MAINPROJECT%"\Libraries\CMakeScripts\CLEyeConfig.cmake "%CLEye_ROOT%"

if "%CMU1394_ROOT%" NEQ "" "%MAINPROJECT%"\Libraries\CMakeScripts\CMU1394Config.cmake "%CMU1394_ROOT%"

if "%faceAPI_ROOT%" NEQ "" "%MAINPROJECT%"\Libraries\CMakeScripts\faceAPIConfig.cmake "%faceAPI_ROOT%"

if "%IPP_ROOT%" NEQ "" "%MAINPROJECT%"\Libraries\CMakeScripts\IPPConfig.cmake "%IPP_ROOT%"

if "%NEWTONGD_ROOT%" NEQ "" "%MAINPROJECT%"\Libraries\CMakeScripts\NEWTONGD230Config.cmake "%NEWTONGD_ROOT%"

if "%OPENGLUT_ROOT%" NEQ "" "%MAINPROJECT%"\Libraries\CMakeScripts\OPENGLUTConfig.cmake "%OPENGLUT_ROOT%"

if "%SVS_ROOT%" NEQ "" "%MAINPROJECT%"\Libraries\CMakeScripts\SVSConfig.cmake "%SVS_ROOT%"

if "%MFCChartControl_ROOT%" NEQ "" "%MAINPROJECT%"\Libraries\CMakeScripts\MFCChartControlConfig.cmake "%MFCChartControl_ROOT%"

goto end

REM ---------------------------------------------------------------------------
REM Report error
:ERROR_MISSING_MAINPROJECT
echo "MAINPROJECT variable is missing"
goto end

REM ---------------------------------------------------------------------------
REM the end
:end
