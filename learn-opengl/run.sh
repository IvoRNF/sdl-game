#!/bin/bash
pkill model-loader
rm -r ./build/* &
outname=1
for f in ./*.cpp /usr/include/glad/src/*.c ; do 
 echo "outputing"
 echo "./build/$outname.o from $f"
 g++ -c ${f} -o "./build/$outname.o" 
 outname=$((outname + 1))
done;
prms=$(find ./build -type f -exec echo {} +)
cmd="g++ -o model-loader $prms -lassimp -lGL -lglfw -lGLEW -lSDL2"
echo $cmd
eval $cmd
./model-loader & echo "runned"

