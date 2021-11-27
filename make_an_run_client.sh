mkdir ./target
make clean
make
rm ./snake-game
cp ./target/snake-game ./snake-game
./snake-game --remote 127.0.0.1