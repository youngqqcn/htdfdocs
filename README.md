

# HTDF 2020版文档

## 内容简介

- bip44 : HTDF支持`BIP44`规范, 包含了HTDF `BIP44`序号
- demo: 包含了不同编程语言的示例代码
  - c++_demos : HTDF交易签名示例, C++实现
  - golang_bech32:  TDF地址判断示例, Golang实现
  - golang_demos: 包含 HTDF私钥和地址生成 和 HTDF交易签名示例代码, Golang实现
  - java_demos: 包含 HTDF私钥和地址生成 和 HTDF交易签名示例代码, Java实现
  - nodejs_demos/htdf_demos : 包含 HTDF私钥和地址生成 和 HTDF交易签名示例代码, Nodejs实现
  - python_demos : python的示例
    - htdf_address_generate.py :  HTDF 私钥地址生存 
    - htdf_transfer.py : HTDF 交易签名示例
  - hrc20_demos_python : HRC20智能合约示例
  
- 区块链浏览器: 区块链浏览器网址
- 接口文档
  - 常用RPC接口.md :  包含了对接 HTDF所需的接口说明
  - HRC-20常用接口.md : 包含了对接HRC20代币所需接口的说明 
  - 常见问题及其处理.md

- 搭建本地观察节点: 包含了搭建节点相关的文档和可执行程序

- 搭建公网观察节点

  

## 对接常见问题汇总(FAQ)



### RPC接口文档?

[常用RPC接口.md](./apidoc/常用RPC接口.md)



### 如何创建账户?

- 方式1: 自行生成私钥和地址 (推荐)
  - [Python3_HTDF私钥和地址生成](./demo/python_demos/htdf_address_generate.py)
  - [Golang_HTDF私钥和地址生成](./demo/golang_demos/htdf_address_generate.go)
  - [Nodejs_HTDF私钥和地址生成](./demo/nodejs_demos/htdf_demos/htdf_address_generate.js)
  - [Java_HTDF私钥和地址生成](./demo/java_demos/demos/src/main/java/com/orientwalt/demos/HtdfAddressGenerate.java)

- 方式2: 用 *Debug*版的`hscli`可执行程序, 其中包含`/accounts/newaccount` 接口, 可以方便地实现 *节点热钱包*   ( :warning: 此方式私钥存放在节点中, 不安全, 不推荐) 

### 如何获取测试链(testchain)的测试币?

转账demo中的地址 `htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml` 有一定数量的测试链(testchain)的测试币可直接使用. 

### 如何进行转账交易?

- 方式1: 自行实现签名 , 可实现*冷钱包* (推荐)
  - [Python3_离线签名交易demo](./demo/python_demos/htdf_transfer.py) 
  - [Golang_离线签名交易demo](./demo/golang_demos/htdf_transfer.go)
  - [Nodejs_离线签名交易demo](./demo/nodejs_demos/htdf_demos/htdf_transfer.js)
  - [Java_离线签名交易demo](./demo/java_demos/demos/src/main/java/com/orientwalt/demos/HtdfTransfer.java)
  - [C++_离线签名交易demo](./demo/c++_demos/htdf/)

- 方式2: 使用 *Debug*版的`hscli`可执行程序, 其中包含`/send` 接口, 可以方便地实现 *节点热钱包*  ( :warning: 此方式私钥存放在节点中, 不安全, 不推荐)

### `hscli`的Release版和Debug版有什么区别?

 :warning:  **警告: Debug版`hscli`执行程序仅用于开发测试环境, 不推荐用于生产环境** 

- Debug版`hscli` 包含 `/accounts/newaccount`、`/send`等几个RPC接口,  而Release版则不包含这些接口.
- 路径:
  - Release版`hscli`路径: `./搭建本地观察节点/release/hscli_v最新版本号_linux_amd64.tar.gz`
  - Debug版`hscli`路径: `./搭建本地观察节点/release/hscli_v最新版本号_DEBUG_MODE_linux_amd64.tar.gz`



### 如何查询交易内容?

使用 `/txs/{txhash}` 接口查询接口 ，`txhash` 是交易hash. 

> 注意: `/transaction/{hash} `是旧版（HTDF 2019）的接口, 不建议使用.

详见 [常用RPC接口.md](./接口文档/常用RPC接口.md)



### 区块链浏览器网址?

- [HTDF 主网区块链浏览器 https://www.htdfscan.com/](https://www.htdfscan.com/)

- [HTDF 测试公网(testnet) 区块链浏览器 http://test.htdfscan.com/](http://test.htdfscan.com/)



### 搭建全节点所需的服务器配置?

|内容|建议配置|最低配置|
|:-----:|:-----:|:-----:|
|CPU|4核|2核|
|内存|8G|4G|
|磁盘|40G|最好是可扩展|



### 如何搭建节点?

- [搭建本地节点(主网)](./搭建本地观察节点/搭建本地观察节点.md)
- [这里有公共测试网节点地址列表, 可直接使用](./测试公网观察节点/htdf.md)



### 最小区块确认数是多少?

HTDF公链不存在分叉，所以交易只需1个区块确认.



### 如何检验地址有效性?

- 算法描述

  ```
  (1)检查地址长度是否等于43 并且 地址是否全部为小写字符, 如果满足条件则进入(2), 否则地址非法;
  (2)使用bech32_decode对地址进行解码, 检查地址是否符合bech32编码规则, 如果能正常解码则进入(3),否则地址非法;
  (3)检查bech32_decode返回值, 检查前缀是否为"htdf" 并且 字节数组长度是否为32, 如果符合条件则地址有效; 否则地址非法.
  ```

- Python3实现

  ```python
  def is_valid_address(address: str) -> bool:
      # (1)检查地址长度是否等于43 并且 地址是否全部为小写字符, 如果满足条件则进入(2), 否则地址非法;
      if not (len(address) == 43 and address.islower()):
          return False
      # (2)使用bech32_decode对地址进行解码, 检查地址是否符合bech32编码规则, 如果能正常解码则进入(3),否则地址非法;
      prefix, data = bech32_decode(address)
      if prefix is None or data is None:
          return False
      # (3)检查bech32_decode返回值, 检查前缀是否为"htdf" 并且 字节数组长度是否为32, 如果符合条件则地址有效; 否则地址非法.
      if prefix == 'htdf' and len(data) == 32:
          return True
      return False
  ```
  
  
  
- Javascript实现, 见[JavaScript_离线签名交易demo](./demo/nodejs_demos/htdf_demos/htdf_transfer.js)

 - [golang源码例子](./demo/golang_bech32)

 - 其他语言的可以参考`demo`目录下不同语言示例的`bech32`解码函数自行实现



### token数量单位

- 我们的数量单位，完全参考BTC
- 两个数量单位 HTDF、satoshi（聪）
- 最小单位是 satoshi（聪）

```
1 HTDF =  100000000 satoshi（聪）
1 satoshi（聪） = 0.00000001 HTDF
```




### HTDF公链黑洞地址
黑洞地址一般用于做币销毁相关业务；

HTDF公链（建议的）黑洞地址如下：

|地址|说明|
|:-----:|:-----:|
|htdf1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq0d4n7t|参考ETH最出名的黑洞地址全零地址`0x0000000000000000000000000000000000000000`，转换为对应HTDF地址为：`htdf1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq0d4n7t`， 转换过程为 `hscli bech32 h2b 0000000000000000000000000000000000000000`，详见[Bech32地址和HEX-20地址的转换](https://gitee.com/orientwalt/apidoc_2020/blob/master/%E6%8E%A5%E5%8F%A3%E6%96%87%E6%A1%A3/HRC-20%E5%B8%B8%E7%94%A8%E6%8E%A5%E5%8F%A3.md) |
|htdf100000000000000000000000000000000zpjht5|"全零"地址，除了末尾的"zpjht5"校验和|
