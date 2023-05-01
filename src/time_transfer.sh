parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"
sshpass -p machinevision scp debian@192.168.7.2:/var/lib/cloud9/stageTranslation/timing.txt \\Users\\matth\\timing.txt
