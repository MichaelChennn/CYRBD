# CYRBD

## Installation

### Linux:

- Clone the source code:

```bash
git clone https://github.com/MichaelChennn/CYRBD.git
cd CYRBD
```

<!-- - Install vcpkg as package manager: -->

<!-- ```bash
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
``` -->

- Install the python libraries:
```
pip install -r requirements.txt
```

- change the CmakeLists.txt:
```
set(pybind11_DIR "/home/{your_username}/.local/lib/{your_python_version}/site-packages/pybind11/share/cmake/pybind11")
# this path can be seen with command:
pip show pybind11
```

- Build the programm

```bash
mkdir build # if not exist
cd build
cmake --build .
make
cd ..
```

- Run the Python benchmark_test.py file (not in the build directory)

```python
python benchmark_test.py
```
