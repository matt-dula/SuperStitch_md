cd camera\
NUMSLIDES=$1
IMGNAME=$2
mkdir "C:\\Users\\matth\\SuperStitch\\output\\$2\\"
echo 'Launching Camera'
.\runCam.exe $NUMSLIDES
echo 'Launching Matlab'
#bash launch_matlab $IMGNAME
