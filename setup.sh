sudo apt-get update
sudo apt-get install libboost-all-dev -y
sudo apt-get install build-essential
sudo apt-get install cmake

mkdir build
mkdir files

source ./config.sh
./build.sh