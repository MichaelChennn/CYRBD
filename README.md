# CYRBD
## Installation
### Linux:
- Clone the source code:

```bash
git clone https://github.com/MichaelChennn/CYRBD.git
cd CYRBD
```
- Install vcpkg as package manager:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
sudo apt update && sudo apt upgrade -y
sudo apt-get install pkg-config
# echo 'export PATH="pathtovcpkg:$PATH"' >> ~/.bashrc
# echo 'export PATH="pathtovcpkg:$PATH"' >> ~/.zshrc (optional for zsh)
# source ~/.bashrc
# source ~/.zshrc
./vcpkg integrate install
./vcpkg install igraph
cd ..
```

- Build the programm
```bash
mkdir build # if not exist
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```