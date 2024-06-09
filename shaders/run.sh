#!/bin/bash
outname=1
for f in ./**/*.cpp ./*.cpp ../common/*.cpp; do 
 g++ -c ${f} -o "./build/$outname.o"
 outname=$((outname + 1))
done;

prms=$(find ./build -type f -exec echo {} +)
cmd="g++ -o ./main $prms -lSOIL -lGL -lGLEW -lSDL2"
eval $cmd
./main & echo "runned"
# sudo apt-get install libglew-dev
# sudo apt-get install libsoil-dev

