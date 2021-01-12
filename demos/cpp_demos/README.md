# HTDF C++ demos

- boost
- cmake : `sudo apt install cmake`

### dependecy

- secp256k1 : https://github.com/bitcoin-core/secp256k1
- openssl : https://github.com/openssl/openssl
- boost

#### install boost

```shell
sudo apt-get install libboost-all-dev
```

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

#### install openssl:

```shell
sudo apt-get install openssl  libssl-dev
sudo ldconfig
```

### run the test cases

```shell
cd cpp_demos
mkdir build && cd build
cmake ../
make
./main
```

