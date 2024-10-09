#inotifywait -r -m -e modify ./ | ./run.sh
#pkill main
#./watch.sh

# ./**/*.cpp  ./*.cpp ../common/*.cpp
inotifywait -r -m -e modify "/home/ivo/Documents/c++/shaders/game/" "/home/ivo/Documents/c++/common" --include './**/*.cpp' | 
while read -r dir action file; do
    fname="$dir$file"
    echo "this file $fname has changed." 
   
    ./run.sh $fname 
     ./watch.sh
    break
done


