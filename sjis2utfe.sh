#/bin/sh

for file in `find src_sjis -name "*.c" -print`
do
    res=`expr $file : "src_sjis\/\(.*\)"`
    echo "src/$res"
    nkf -Sw $file > src/$res
done
for file in `find src_sjis -name "*.cpp" -print`
do
    res=`expr $file : "src_sjis\/\(.*\)"`
    echo "src/$res"
    nkf -Sw $file > src/$res
done


for file in `find src_sjis -name "*.h" -print`
do
    res=`expr $file : "src_sjis\/\(.*\)"`
    echo "src/$res"
    nkf -Sw $file > src/$res
done
for file in `find src_sjis -name "*.hpp" -print`
do
    res=`expr $file : "src_sjis\/\(.*\)"`
    echo "src/$res"
    nkf -Sw $file > src/$res
done


for file in `find src_sjis -name "*.txt" -print`
do
    res=`expr $file : "src_sjis\/\(.*\)"`
    echo "src/$res"
    nkf -Sw $file > src/$res
done
