echo "LAUNCHING PROCESS"
set /A NUMSLIDES=%1
set IMGNAME=%2
cd C:\Users\matth\SuperStitch\output\
Rem md "%IMGNAME%"
cd C:\Users\matth\SuperStitch\src\camera\
Rem .\runCam.exe %NUMSLIDES%
Rem cd ..
Rem matlab -nodisplay -r "cd('C:\Users\matth\SuperStitch\src\matlab\'); SuperStitch('C:\Users\matth\SuperStitch\src\camera\','C:\Users\matth\SuperStitch\src\camera\');exit"
