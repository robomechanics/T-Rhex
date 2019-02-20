cd DynamixelSDK/
git submodule init
git submodule update

sudo apt-get install gcc-5
sudo apt-get install build-essential
sudo apt-get install gcc-multilib g++-multilib

cd ./c++/build/linux64/
make clean
make
sudo make install

