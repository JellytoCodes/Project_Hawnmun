@echo off
set "OutputFile=ProjectH_AllCode.txt"

:: 기존에 만들어둔 병합 파일이 있다면 삭제
if exist "%OutputFile%" del "%OutputFile%"

echo 코드를 병합하는 중입니다. 잠시만 기다려주세요...

:: 현재 폴더와 모든 하위 폴더를 돌면서 .h 와 .cpp 파일을 찾아 병합
for /r %%i in (*.h *.cpp) do (
    echo. >> "%OutputFile%"
    echo // ======================================================= >> "%OutputFile%"
    echo // File: %%~nxi >> "%OutputFile%"
    echo // ======================================================= >> "%OutputFile%"
    echo. >> "%OutputFile%"
    type "%%i" >> "%OutputFile%"
)

echo 코드 병합이 완료되었습니다! (생성된 파일: %OutputFile%)
pause