#/bin/sh

for file in `find src -name "*.c" -print`
do
    res=`expr $file : "src\/\(.*\)"`
    sufix=""
    resc=$res$sufix
    echo "src_sjis/$resc"
    nkf -s $file > src_sjis/$resc
done
for file in `find src -name "*.cpp" -print`
do
    res=`expr $file : "src\/\(.*\)"`
    sufix=""
    resc=$res$sufix
    echo "src_sjis/$resc"
    nkf -s $file > src_sjis/$resc
done



for file in `find src -name "*.h" -print`
do
    res=`expr $file : "src\/\(.*\)"`
    echo "src_sjis/$res"
    nkf -s $file > src_sjis/$res
done
for file in `find src -name "*.hpp" -print`
do
    res=`expr $file : "src\/\(.*\)"`
    echo "src_sjis/$res"
    nkf -s $file > src_sjis/$res
done


for file in `find src -name "*.txt" -print`
do
    res=`expr $file : "src\/\(.*\)"`
    sufix=""
    restxt=$res$sufix
    echo "src_sjis/$restxt"
    nkf -s $file > src_sjis/$restxt
done
