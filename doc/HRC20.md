
> 提示: 如果你熟悉Python, 可以使用htdfsdk轻松对接HRC20
>
> https://github.com/youngqqcn/htdfsdk


目录
- [术语定义](#术语定义)
- [Bech32地址和HEX-20地址的转换](#Bech32地址和HEX-20地址的转换)
- [常用的HRC20函数签名对照表](#常用的HRC20函数签名对照表)
- [HRC20常用操作](#HRC20常用操作)
  - [查询地址余额(balanceOf)](#查询地址余额balanceOf)
  - [查询智能合约名称(name)](#查询智能合约名称name)
  - [查询代币总发行量(totalSupply)](#查询代币总发行量totalSupply)
  - [查询代币简称(symbol)](#查询代币简称symbol)
  - [查代币精度(decimals)](#查代币精度decimals)
- [案例1_HRC20转账](#案例1_HRC20转账)
- [案例2_HRC20批量转账](#案例2_HRC20批量转账)



# 术语定义
- **原生币/主币**: HTDF公链的基础token

- **HRC-20代币**: HTDF公链支持EVM虚拟机，可以使用solidiy智能合约发行的ERC-20代币，这些代币称为HRC-20代币；

- **HTDF公链交易类型**:
  - 普通交易；用于原生币的转账,即普通HTDF转账
  - 智能合约交易；用于创建、访问智能合约（转账、查询余额、铸币等）
  - 扩展型交易；用于委托、解除委托、创建验证节点等

- **Bech32地址**:

  [Bech32地址的定义](https://github.com/bitcoin/bips/blob/master/bip-0173.mediawiki#Bech32)

  HTDF公链地址采用Bech32地址格式，例如 
  
  ```
  htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y
  htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3
  htdf17qarupfh9gee0yvywhxfy2zv39fjttracvgapx
  ```

- **HEX-20地址**:

  ETH的原始地址是20字节的地址，用16进制字符串表示，就是类似于 
  
  ```
  9DAD8A9FE9C5C85576E8A3FB692A27AB2A44803B
  85ced8ddf399d75c9e381e01f3bddcefb9132fe9
  f03a3e05372a3397918475cc92284c895325ac7d
  ```

- **前补零HEX-20地址**:

  在智能合约原始数据（Data字段）中，使用前补零HEX-20地址

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

### [golang bech32代码例子](../demos/golang_demos/bech32)

```
bech32str=htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y
hex20ContractAddr=9dad8a9fe9c5c85576e8a3fb692a27ab2a44803b

bech32str=htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3
hex20ContractAddr=85ced8ddf399d75c9e381e01f3bddcefb9132fe9

bech32str=htdf17qarupfh9gee0yvywhxfy2zv39fjttracvgapx
hex20ContractAddr=f03a3e05372a3397918475cc92284c895325ac7d
```


# 常用的HRC20函数签名对照表

以下函数签名对于符合ERC20标准(HRC20)的智能合约是固定不变的,可直接使用

|函数签名|对应函数 | 说明|
|-------|--------|---------|
|06fdde03 | name()| 智能合约名称 |
|95d89b41 | symbol()| 代币简称(如: BEI)  |
|313ce567 | decimals()| 精度|
|18160ddd | totalSupply()| 代币总发行量|
|70a08231 | balanceOf(address)| 查询地址的代币余额,不需要发起交易 |
|a9059cbb | transfer(address,uint256)| 转账, 第1个参数是代币接收方的地址, 第2个参数是转账代币的金额 |



# HRC20免费查询接口



## 查询地址余额balanceOf

如上[常用的HRC20函数签名对照表](#常用的HRC20函数签名对照表), 使用`balanceOf`函数, 函数签名为:`70a08231`

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

## 查询智能合约名称name


如上[常用的HRC20函数签名对照表](#常用的HRC20函数签名对照表),, 使用`name`函数, 函数签名为:`06fdde03`

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


## 查询代币总发行量totalSupply


如上[常用的HRC20函数签名对照表](#常用的HRC20函数签名对照表),, 使用`totalSupply`函数, 函数签名为:`18160ddd`

#### RPC接口
http://htdf2020-test01.orientwalt.cn:1317/hs/contract/[Bech32合约地址]/18160ddd

```
$name=`curl http://htdf2020-test01.orientwalt.cn:1317/hs/contract/htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y/18160ddd`

python -c "print bytearray.fromhex($name)"
```

#### hscli
hscli query contract [Bech32合约地址]  18160ddd

```
$## 查询HRC-20代币名称
name=`hscli query contract htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y 18160ddd`

$python -c "print bytearray.fromhex('$name')"
```



## 查询代币简称symbol

如上[常用的HRC20函数签名对照表](#常用的HRC20函数签名对照表),, 使用`symbol`函数, 函数签名为:`95d89b41`

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

## 查代币精度decimals

如上[常用的HRC20函数签名对照表](#常用的HRC20函数签名对照表),, 使用`decimals`函数, 函数签名为:`313ce567`


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




# 案例1_HRC20转账

> 提示: 如果你熟悉Python, 可以使用htdfsdk轻松对接HRC20
>
> https://github.com/youngqqcn/htdfsdk

方便起见, 这里我们以`Debug`版`hscli`提供的`hs/send`接口进行演示,

在开始说明如何进行HRC20转账之前, 让我们先看看HTDF主网上已有的成功的HRC20转账交易:

```json
{
    "height": "4767616",
    "txhash": "8B29EA489434D2A103CC6AD83B78D36D0F0FFF12EC9D1D8A70561F7328AC4847",
    "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"{\\\"code\\\":0,\\\"message\\\":\\\"ok\\\",\\\"contract_address\\\":\\\"\\\",\\\"evm_output\\\":\\\"0000000000000000000000000000000000000000000000000000000000000001\\\"}\"}]",
    "logs": [
        {
            "msg_index": "0",
            "success": true,
            "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"\",\"evm_output\":\"0000000000000000000000000000000000000000000000000000000000000001\"}" //HRC20交易成功
        }
    ],
    "gas_wanted": "200000",
    "gas_used": "75604",
    "tags": [
        {
            "key": "action",
            "value": "send"
        }
    ],
    "tx": {
        "type": "auth/StdTx",
        "value": {
            "msg": [
                {
                    "type": "htdfservice/send",
                    "value": {
                        "From": "htdf1qnamql6cnlppfdmnam60kdfuepgh6rgy99gxp5", // token发送方
                        "To": "htdf1rcl9x2akjuy3tyqyxwk7sataldzmxy6jm06hq4", // HRC20-BEI 智能合约地址
                        "Amount": [
                            {
                                "denom": "satoshi",
                                "amount": "0" // 因为时HRC20转账, 此HTDF转账金额设置0即可
                            }
                        ],
                        "Data": "a9059cbb000000000000000000000000bcb8cbdace8ad411ca358b3490c655380a0bb021000000000000000000000000000000000000000000000000280445978f176400", // 这是调用HRC20智能合约数据, 下面会详细介绍如何组装此数据
                        "GasPrice": "100",
                        "GasWanted": "200000"
                    }
                }
            ],
            "fee": {
                "gas_wanted": "200000",
                "gas_price": "100"
            },
            "signatures": [
                {
                    "pub_key": {
                        "type": "tendermint/PubKeySecp256k1",
                        "value": "AgLJy7DgT6Psv8CMrBJdAQNpooJx2VyQMwXGZCmM0dmh"
                    },
                    "signature": "B2ZXDgs4ipbaA4EfOkyCPM6TKpvnT6M8jChCEQTgcZVqKID1SDgFq+Sm/x2yvs4i4IB0/pz4h95QTvmG4n40bw=="
                }
            ],
            "memo": "空投糖果"
        }
    },
    "timestamp": "2021-01-18T11:21:54Z"
}
```


接下来我们就以上面的这个成功的HRC20交易为例说明HRC20转账的细节:




0.HRC20-BEI智能合约地址为 

```
htdf1rcl9x2akjuy3tyqyxwk7sataldzmxy6jm06hq4
```

1.如上[常用的HRC20函数签名对照表](#常用的HRC20函数签名对照表), 转账使用`transfer`函数, 其签名为:

```
a9059cbb
```

2.代币接收方地址`htdf1hjuvhkkw3t2prj343v6fp3j48q9qhvppndjksk` 转十六进制格式(32字节)为:
```
000000000000000000000000bcb8cbdace8ad411ca358b3490c655380a0bb021
```


3.若转账金额为`2.883506178667013120`BEI, 精度为18, 则转为整数是, `2.883506178667013120 * 10^8 = 2883506178667013120` , 再转为十六进制格式(32字节)为:
```
000000000000000000000000000000000000000000000000280445978f176400
```

4.按照`[4字节函数签名][32字节地址][32字节金额]` 合并得到`data`如下:
```
a9059cbb000000000000000000000000bcb8cbdace8ad411ca358b3490c655380a0bb021000000000000000000000000000000000000000000000000280445978f176400
```

5.使用[send接口](./api.md/#普通转账或创建并发布智能合约), 将上一步合并到的`data`作为参数填入交易的`data`; `to`设置智能合约的地址; gas_wanted设置为200000即可, 自行获取account_number, sequence. 例如:

```shell
curl  'http://localhost:1317/hs/send' -X POST \
-H 'Content-Type: application/json' \
-d '{ "base_req": 
    { 
        "from": "htdf1qnamql6cnlppfdmnam60kdfuepgh6rgy99gxp5", 
        "memo": "HRC20 test",
        "password": "12345678", 
        "chain_id": "mainchain", 
        "account_number": "123638", 
        "sequence": "4082", 
        "gas_wanted": "200000",
        "gas_price": "100", 
        "simulate": false
    },          
    "amount": [ 
        {
        "denom": "htdf", 
        "amount": "0" 
        }
    ],
    "to": "htdf1rcl9x2akjuy3tyqyxwk7sataldzmxy6jm06hq4", 
    "data": "a9059cbb000000000000000000000000bcb8cbdace8ad411ca358b3490c655380a0bb021000000000000000000000000000000000000000000000000280445978f176400"
    }
'
```

6.[查询交易](./api.md#查询交易详情), 如果logs中的`evm_output`没有报错,即表示成功, 类似:

成功的HRC20转账例子:
真实交易的txhash: 8B29EA489434D2A103CC6AD83B78D36D0F0FFF12EC9D1D8A70561F7328AC4847

```json
// .... 其他字段, 略
"logs": [
    {
        "msg_index": "0",
        "success": true,
        "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"\",\"evm_output\":\"0000000000000000000000000000000000000000000000000000000000000001\"}"
    }
]
```

失败的HRC20转账的例子:

>温馨提示: 一般情况下, 导致HRC20转账错误的原因是from地址的HRC20代币余额不足. 当然,也有其他可能.

真实交易的txhash:: 961de5d5138c9d1985908be67060f20ee67a6396db193ca98925d69545ebfc08

```json
// .... 其他字段, 略
"logs": [
    {
        "msg_index": "0",
        "success": false,
        "log": "{\"code\":2,\"message\":\"open contract error\",\"contract_address\":\"\",\"evm_output\":\"evm call error|err=invalid opcode 0xfe\\n\"}"
    }
]
```


# 案例2_HRC20批量转账

可以参考这个示例代码[airdrop](../demos/python_demos/airdrop_tools/)


---
(结束)