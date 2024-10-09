#!/bin/bash
pkill main
#rm -r ./build/* &

#for fullFileName in ./**/*.cpp  ./*.cpp ../common/*.cpp ; do

 
#done;
echo "compiling..."
for fullFileName in "$@"
do 
    fname=${fullFileName##*/} #remove base path from file name
    fname=${fname%.*} #remove extension from file name
    echo "g++ -c ${fullFileName} -o "./build/$fname.o"" 
    g++ -c ${fullFileName} -o "./build/$fname.o"
done
echo "compile end."



prms=$(find ./build -type f -exec echo {} +)
cmd="g++ -o ./main $prms -lassimp -lSOIL -lGL -lGLEW -lSDL2 "
echo $cmd
eval $cmd
./main & echo "runned"
# sudo apt-get install libglew-dev
# sudo apt-get install libsoil-dev
# sudo apt install libglm-dev
# sudo apt-get install rapidjson-dev
# sudo apt-get install libassimp-dev





