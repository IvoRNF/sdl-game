#!/bin/bash
outname=1
for f in ./**/*.cpp ./*.cpp; do 
 g++ -c ${f} -o "./build/$outname.o" -lSDL2
 outname=$((outname + 1))
done;

prms=$(find ./build -type f -exec echo {} +)
cmd="g++ -o ./main $prms -lSDL2"
eval $cmd
./main & echo "runned"

