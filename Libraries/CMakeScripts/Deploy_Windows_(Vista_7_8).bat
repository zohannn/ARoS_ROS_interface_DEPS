@echo off

REM ---------------------------------------------------------------------------
REM test the existence of environment variable MAINPROJECT
if "%MAINPROJECT%" EQU "" goto ERROR_MISSING_MAINPROJECT

REM ---------------------------------------------------------------------------
REM Proceed with creation of links

if "%CLEye_ROOT%" NEQ "" mklink "%CLEye_ROOT%"\CLEyeConfig.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\CLEyeConfig.cmake

if "%CMU1394_ROOT%" NEQ "" mklink "%CMU1394_ROOT%"\CMU1394Config.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\CMU1394Config.cmake

if "%faceAPI_ROOT%" NEQ "" mklink "%faceAPI_ROOT%"\faceAPIConfig.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\faceAPIConfig.cmake

if "%IPP_ROOT%" NEQ "" mklink "%IPP_ROOT%"\IPPConfig.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\IPPConfig.cmake

if "%NEWTONGD_ROOT%" NEQ "" mklink "%NEWTONGD_ROOT%"\NEWTONGDConfig.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\NEWTONGDConfig.cmake

if "%OPENGLUT_ROOT%" NEQ "" mklink "%OPENGLUT_ROOT%"\OPENGLUTConfig.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\OPENGLUTConfig.cmake

if "%SVS_ROOT%" NEQ "" mklink "%SVS_ROOT%"\SVSConfig.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\SVSConfig.cmake

if "%MFCChartControl_ROOT%" NEQ "" mklink "%MFCChartControl_ROOT%"\MFCChartControlConfig.cmake "%MAINPROJECT%"\Libraries\CMakeScripts\MFCChartControlConfig.cmake

goto end

REM ---------------------------------------------------------------------------
REM Report error
:ERROR_MISSING_MAINPROJECT
echo "MAINPROJECT variable is missing"
goto end

REM ---------------------------------------------------------------------------
REM the end
:end
