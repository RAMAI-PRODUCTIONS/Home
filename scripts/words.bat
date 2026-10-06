@echo off
rem Refuses to build while any authored file reaches the 300 word ceiling.
call "%~dp0env.bat" || exit /b 1
"%PY%" "%ROOT%\tools\check_word_limits.py" || exit /b 1
exit /b 0
