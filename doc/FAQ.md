## 目录
- [01) API文档在哪?](#1.API文档在哪)
- [02) 如何创建账户?](#2.如何创建账户)
- [03) 如何获取测试链(testchain)的测试币?](#3.如何获取测试链(testchain)的测试币)
- [04) 如何进行转账交易?](#4.如何进行转账交易?)
- [05) hscli的Release版和Debug版有什么区别?](#5.`hscli`的Release版和Debug版有什么区)
- [06) 如何查询交易内容?](#6.如何查询交易内容)
- [07) 区块链浏览器网址?](#7.区块链浏览器网址)
- [08) 搭建全节点所需的服务器配置?](#8.搭建全节点所需的服务器配置)
- [09) 如何搭建节点?](#9.如何搭建节点)
- [10) 最小区块确认数是多少?](#10.最小区块确认数是多少)
- [11) 如何检验地址有效性?](#11.如何检验地址有效性)
- [12) token单位换算?](#12.token单位换算)
- [13) HTDF公链黑洞地址?](#13.HTDF公链黑洞地址)

---

# 对接常见问题汇总

## 1.API文档在哪

- [API文档: api.md](./api.md)

---

## 2.如何创建账户

- 方式1: 自行生成私钥和地址 (推荐)
  - [python demos](../demos/python_demos/)
  - [cpp demos](../demos/cpp_demos/)
  - [golang demos](../demos/golang_demos/)
  - [nodejs demos](../demos/nodejs_demos/)
  - [java_demos](../demos/java_demos/)

- 方式2: 用 *Debug*版的`hscli`可执行程序, 其中包含`/accounts/newaccount` 接口, 可以方便地实现 *节点热钱包*   ( :warning: 此方式私钥存放在节点中, 不安全, 不推荐) 

--- 

## 3.如何获取测试链(testchain)的测试币

转账demo中的地址 `htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml` 有一定数量的测试链(testchain)的测试币可直接使用. 

---

## 4.如何进行转账交易

- 方式1: 自行实现签名 , 可实现*冷钱包* (推荐)
  - [python demos](../demos/python_demos/)
  - [cpp demos](../demos/cpp_demos/)
  - [golang demos](../demos/golang_demos/)
  - [nodejs demos](../demos/nodejs_demos/)
  - [java_demos](../demos/java_demos/)

- 方式2: 使用 *Debug*版的`hscli`可执行程序, 其中包含`/send` 接口, 可以方便地实现 *节点热钱包*  ( :warning: 此方式私钥存放在节点中, 不安全, 不推荐)



问题延伸:

**如何进行批量转账?**

- 方案1： 等待区块打包
发完一笔交易后，等待8~10秒(等待交易被打包)，再发送下一笔交易

- 方案2： 改变交易的sequence, 具体实现可以参考[python_demos/airdrop_tools](../demos/python_demos/airdrop_tools/airdrop.py)代码,然后根据自身实际业务需求自行修改即可. 

这里以python(伪)代码, 进行算法描述:

```python
def batch_transfer(
        htdfrpc: HtdfRPC, 
        from_address:str, 
        from_private_key: str, 
        to_addresses : list):
    """
    batch_transfer 批量转账

    htdfrpc: 封装了rpc请求的对象  
    from_address: from地址
    from_private_key: from地址私钥
    to_addresses: to地址数组
    """

    account_info = htdfrpc.get_account_info(address=from_address)
    amount_satoshi = int(0.12345678 * 10**8) 

    current_sequence = account_info.sequence
    for to_address in to_addresses:
        
        # 构造交易并签名
        signed_tx = HtdfTxBuilder(
            from_address=from_address,
            to_address=to_address,
            amount_satoshi=amount_satoshi,
            sequence=current_sequence,
            account_number=account_info.account_number,
            chain_id=htdfrpc.chain_id,
            gas_price=100,
            gas_wanted=30000,
            data='',
            memo='test'
        ).build_and_sign(private_key=from_private_key)

        # 广播交易
        tx_hash = htdfrpc.broadcast_tx(tx_hex=signed_tx)
        print('tx_hash: {}'.format(tx_hash))

        # 改变sequence
        current_sequence += 1  
    pass
```

---

## 5.`hscli`的Release版和Debug版有什么区别

 :warning:  **警告: Debug版`hscli`执行程序仅用于开发测试环境, 不推荐用于生产环境** 

- Debug版`hscli` 包含 `/accounts/newaccount`、`/send`等几个RPC接口,  而Release版则不包含这些接口.


---

## 6.如何查询交易内容

使用 `/txs/{txhash}` 接口查询接口 ，`txhash` 是交易hash. 

> 注意: `/transaction/{hash} `是旧版（HTDF 2019）的接口, 不建议使用.

详见 [api.md](./api.md)

---


## 7.区块链浏览器网址

- [HTDF 主网区块链浏览器 https://www.htdfscan.com/](https://www.htdfscan.com/)

- [HTDF 测试公网(testnet) 区块链浏览器 http://test.htdfscan.com/](http://test.htdfscan.com/)

---

## 8.搭建全节点所需的服务器配置

|内容|建议配置|最低配置|
|:-----:|:-----:|:-----:|
|CPU|4核|2核|
|内存|8G|4G|
|磁盘|60G 可扩展|60G 可扩展|

---

## 9.如何搭建节点

- [搭建观察节点(主网)](../mainnet/README.md)

---

## 10.最小区块确认数是多少

HTDF公链不存在分叉，所以交易只需1个区块确认.

---

## 11.如何检验地址有效性

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

---

## 12.token单位换算

- 我们的数量单位，完全参考BTC
- 两个数量单位 HTDF、satoshi（聪）
- 最小单位是 satoshi（聪）

```
1 HTDF =  100000000 satoshi（聪）
1 satoshi（聪） = 0.00000001 HTDF
```


---

## 13.HTDF公链黑洞地址
黑洞地址一般用于做币销毁相关业务；

HTDF公链（建议的）黑洞地址如下：

|地址|说明|
|:-----:|:-----:|
|htdf1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq0d4n7t|参考ETH最出名的黑洞地址全零地址`0x0000000000000000000000000000000000000000`，转换为对应HTDF地址为：`htdf1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq0d4n7t`， 转换过程为 `hscli bech32 h2b 0000000000000000000000000000000000000000`，详见[Bech32地址和HEX-20地址的转换](./HRC20.md) |
|htdf100000000000000000000000000000000zpjht5|"全零"地址，除了末尾的"zpjht5"校验和|


---
(结束)