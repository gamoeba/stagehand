@echo off
set DIR=%~dp0
set PATH=%DIR%bin;%PATH%
set QT_PLUGIN_PATH=%DIR%plugins
bin\stagehand
