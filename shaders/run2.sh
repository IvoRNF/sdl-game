#!/bin/bash
pkill main &
rm -r ./build/* &
outname=1
#for f in ./**/*.cpp ./*.cpp ../common/*.cpp; do 
# g++ -c ${f} -o "./build/$outname.o"
# outname=$((outname + 1)) &
#done;


g++ -c ./game/game.cpp -o "./build/game.o" &&

g++ -c ./game/Shader.cpp -o "./build/Shader.o" &&
g++ -c ./main.cpp -o "./build/main.o" &&
g++ -c ../common/Math.cpp -o "./build/Math.o" &&
g++ -c ../common/Texture.cpp -o "./build/Texture.o" &&
g++ -c ../common/VertexArray.cpp -o "./build/VertexArray.o" &&

prms=$(find ./build -type f -exec echo {} +)
cmd="g++ -o ./main $prms -lSOIL -lGL -lGLEW -lSDL2"
eval $cmd &
./main
# sudo apt-get install libglew-dev
# sudo apt-get install libsoil-dev
# sudo apt install libglm-dev
# sudo apt-get install rapidjson-dev


