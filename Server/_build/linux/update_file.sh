
if [ "$1" = "100" ];then
	branch=0
elif [ "$1" = "101" ];then
	branch=1
else
	branch=2
fi
echo $branch

if [ ! -n "$2" ]; then
    	echo "please input file name!"
    	exit 0
fi
echo $2

# ����������浵�汾
cd ../../../
\cp -f ./Resource/config/* ./Bin/config/
cd Bin/tool/gcm/builder/
chmod 777 gcm_build
./gcm_build -t 2 -b $branch -f $2

cd ../../../../../../../
