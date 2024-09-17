#!/bin/bash
pkill main
rm -r ./build/* &
outname=1
for f in ./**/*.cpp ./*.cpp ../common/*.cpp; do 
 g++ -c ${f} -o "./build/$outname.o"
 outname=$((outname + 1))
done;

prms=$(find ./build -type f -exec echo {} +)
cmd="g++ -o ./main $prms -lassimp -lSOIL -lGL -lGLEW -lSDL2 "
eval $cmd
./main & echo "runned"
# sudo apt-get install libglew-dev
# sudo apt-get install libsoil-dev
# sudo apt install libglm-dev
# sudo apt-get install rapidjson-dev
# sudo apt-get install libassimp-dev
# git clone https://github.com/nothings/stb.git

