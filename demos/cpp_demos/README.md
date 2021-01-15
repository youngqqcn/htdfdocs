# HTDF C++ demos

- cmake : `sudo apt install cmake`

### dependencies

- secp256k1 : https://github.com/bitcoin-core/secp256k1

#### install secp256k1:

```shell
# git clone https://github.com/bitcoin-core/secp256k1
git clone https://gitee.com/youngqqcn/secp256k1
cd secp256k1
./autogen.sh
./configure --enable-module-recovery
make 
sudo make install
sudo ldconfig
```

### run the test cases

```shell
cd cpp_demos
mkdir build && cd build
cmake ../
make

# run all unit test cases
./test

# htdf address demo
./htdf_address_generate

# htdf normal transaction demo
./htdf_transfer

```

