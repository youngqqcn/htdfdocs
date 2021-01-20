
# **HRC-20常用接口**

# 术语
- 原生币/主币
  > HTDF公链的基础token

- HRC-20代币
  > HTDF公链支持EVM虚拟机，可以使用solidiy智能合约发行的ERC-20代币，这些代币称为HRC-20代币；

- HTDF公链交易类型
  > 普通交易；用于原生币的转账

  > 智能合约交易；用于创建、访问智能合约（转账、查询余额、铸币等）

  > 扩展型交易；用于委托、解除委托、创建验证节点等

- Bech32地址
  > [Bech32地址的定义](https://github.com/bitcoin/bips/blob/master/bip-0173.mediawiki#Bech32)

  > HTDF公链地址采用Bech32地址格式，例如 
  
  ```
  htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y
  htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3
  htdf17qarupfh9gee0yvywhxfy2zv39fjttracvgapx
  ```

- HEX-20地址
  > ETH的原始地址是20字节的地址，用16进制字符串表示，就是类似于 
  
  ```
  9DAD8A9FE9C5C85576E8A3FB692A27AB2A44803B
  85ced8ddf399d75c9e381e01f3bddcefb9132fe9
  f03a3e05372a3397918475cc92284c895325ac7d
  ```

- 前补零HEX-20地址
  > 在智能合约原始数据（Data字段）中，使用前补零HEX-20地址

  ```
  0000000000000000000000009DAD8A9FE9C5C85576E8A3FB692A27AB2A44803B
  00000000000000000000000085ced8ddf399d75c9e381e01f3bddcefb9132fe9
  000000000000000000000000f03a3e05372a3397918475cc92284c895325ac7d

  ```

# Bech32地址和HEX-20地址的转换

### hscli工具

```
$hscli version
GitCommit=09dffd9|version=1.1.0|GitBranch=dev|DEBUGAPI=ON|ApiSecurityLevel=low

$hscli bech32 b2h htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3
bechAddress=htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3|hexAddress=85CED8DDF399D75C9E381E01F3BDDCEFB9132FE9

$hscli bech32 h2b 85CED8DDF399D75C9E381E01F3BDDCEFB9132FE9
hexAddr=85CED8DDF399D75C9E381E01F3BDDCEFB9132FE9|bech32Addr=htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3
```

### [golang bech32代码例子](https://gitee.com/orientwalt/apidoc_2020/tree/master/demo/golang_bech32)

```
bech32str=htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y
hex20ContractAddr=9dad8a9fe9c5c85576e8a3fb692a27ab2a44803b

bech32str=htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3
hex20ContractAddr=85ced8ddf399d75c9e381e01f3bddcefb9132fe9

bech32str=htdf17qarupfh9gee0yvywhxfy2zv39fjttracvgapx
hex20ContractAddr=f03a3e05372a3397918475cc92284c895325ac7d
```


# HRC-20常用操作

- 使用`智能合约交易`，创建智能合约、访问智能合约（转账、查询余额、铸币等）
  > 参考A，`gitee.com/orientwalt/apidoc_2020  接口文档/常用RPC接口.md `  有智能合约交易curl例子

  > 参考B，`gitee.com/orientwalt/apidoc_2020  demo/python `  有智能合约交易的程序例子（htdf_callcontract.py、htdf_contract.py）

- 使用`免费查询接口`
  > 以上智能合约交易，都是要收手续费（fee）的
  
  > HTDF公链内部另外提供了类似 eth.call 的机制，进而提供免费查询接口


# HRC-20免费查询接口

## 查询余额（操作码  70a08231 ）

#### RPC接口
http://htdf2020-test01.orientwalt.cn:1317/hs/contract/[Bech32合约地址]/70a08231+[前补零HEX-20地址]

```
$balanceOfHex=`curl http://htdf2020-test01.orientwalt.cn:1317/hs/contract/htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y/70a0823100000000000000000000000085ced8ddf399d75c9e381e01f3bddcefb9132fe9`

$echo $balanceOfHex                            ### (16进制)
$python -c "print int(${balanceOfHex}, 16)"  ###（10进制）

```

#### hscli
hscli query contract [Bech32合约地址]  70a08231+[前补零HEX-20地址]

```
## 查询余额
$balanceOfHex=`hscli query contract htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y  70a0823100000000000000000000000085ced8ddf399d75c9e381e01f3bddcefb9132fe9`  

$echo $balanceOfHex                            ### (16进制)
$python -c "print int('${balanceOfHex}', 16)"  ###（10进制）

```

## 查询代币名称（操作码  06fdde03 ）


#### RPC接口
http://htdf2020-test01.orientwalt.cn:1317/hs/contract/[Bech32合约地址]/06fdde03

```
$name=`curl http://htdf2020-test01.orientwalt.cn:1317/hs/contract/htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y/06fdde03`

python -c "print bytearray.fromhex($name)"
```

#### hscli
hscli query contract [Bech32合约地址]  06fdde03

```
$## 查询HRC-20代币名称
name=`hscli query contract htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y 06fdde03`

$python -c "print bytearray.fromhex('$name')"
```


## 查询代币单位符号（操作码  95d89b41 ）


#### RPC接口
http://htdf2020-test01.orientwalt.cn:1317/hs/contract/[Bech32合约地址]/95d89b41

```
$symbol=`curl http://htdf2020-test01.orientwalt.cn:1317/hs/contract/htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y/95d89b41`

$python -c "print bytearray.fromhex($symbol)"
```



#### hscli
hscli query contract [Bech32合约地址] 95d89b41

```
## 查询代币单位符号
$symbol=`hscli query contract htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y 95d89b41`

$python -c "print bytearray.fromhex('$symbol')"
```

## 查代币精度

#### RPC 接口
http://htdf2020-test01.orientwalt.cn:1317/hs/contract/[Bech32合约地址]/313ce567

```
$decimals=`curl http://htdf2020-test01.orientwalt.cn:1317/hs/contract/htdf18z3t3n85dn8lknah5pjlj7ks95s0zu5l5f097x/313ce567`
$python -c "print int($decimals,16)"
```

#### hscli 
hscli query contract [Bech32合约地址] 313ce567
```
decimals=`hscli query contract htdf18z3t3n85dn8lknah5pjlj7ks95s0zu5l5f097x 313ce567`
python -c "print int('$decimals',16)"
```




# 附录A 联调例子

#### 钱包转账

```
1、htdf地址转HEX-20地址
htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y->9dad8a9fe9c5c85576e8a3fb692a27ab2a44803b

(1)bech32.decode
(2)convertBits(adddata, 5, 8, true)
(3)bytetoHax ->  得到 9dad8a9fe9c5c85576e8a3fb692a27ab2a44803b 40位

2 、transfer
a9059cbb000000000000000000000000 32位

3、100000000
转16进制 00000000000000000000000000000000000000000000000000000000000f4240  64位

4、合并二一三步得到a9059cbb000000000000000000000000a0a23a8a2ba69a78111c5b7adf707fe583518a5900000000000000000000000000000000000000000000000000000000000f4240 得到data

```


---
(结束)