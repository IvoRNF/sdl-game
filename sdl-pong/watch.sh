inotifywait -r -m -e modify ./ | ./run.sh
pkill main
./watch.sh