# 常见问题及其处理

# gas相关问题

- HTDF公链采用类似ETH的机制
  > gas_price 等同于ETH 的gasPrice

  > gas_wanted 等同于ETH 的 gasLimit

- 普通交易的fee（手续费）
  > 固定为 0.03 HTDF token
    >> gas_price 为100  （satoshi）

    >> gas_wanted 为 30000

  ```
  curl --location --request POST 'http://htdf2020-test01.orientwalt.cn:1317/hs/send' \
  --header 'Content-Type: application/json' \
  --data-raw '    { "base_req": 
      { "from": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy", 
        "memo": "Sent via Cosmos Voyager ",
        "password": "12345678", 
        "chain_id": "testchain", 
        "account_number": "0", 
        "sequence": "0", 
        "gas_wanted": "30000",
        "gas_price": "100", 
         "simulate": false
      },          
      "amount": [ 
              { "denom": "htdf", 
                "amount": "0.01" } ],
      "to": "htdf1rgsfxav0af8a79cmtq6rnjtjkqngl9qcj8k9l7",
    "data": ""
  }'

  ```



# 节点搭建

- 节点搭建
  > [本地观察节点](https://gitee.com/orientwalt/apidoc_2020/tree/master/%E6%90%AD%E5%BB%BA%E6%9C%AC%E5%9C%B0%E8%A7%82%E5%AF%9F%E8%8A%82%E7%82%B9) 

  > [测试公网观察节点](https://gitee.com/orientwalt/apidoc_2020/tree/master/测试公网观察节点)

- 正常现象
  > 同步速度较快，网络基本没有什么波动

- 可能出现的错误
  >  同步区块速度过慢---正常情况主网110万块大约时间4H
  >>  1.cpu占用率过高 --关掉其他的服务

  >>  2.内存过小      --加内存

  >>  3.网络速度      --如果是云服务器加带宽    

# 发送交易
## /send 转账接口
```
curl --location --request POST 'http://htdf2020-test01.orientwalt.cn:1317/hs/send' \
--header 'Content-Type: application/json' \
--data-raw '    { "base_req": 
      { "from": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy", 
        "memo": "Sent via Cosmos Voyager ",
        "password": "12345678", 
        "chain_id": "testchain", 
        "account_number": "0", 
        "sequence": "0", 
        "gas_wanted": "30000",
        "gas_price": "100", 
         "simulate": false
      },          
      "amount": [ 
              { "denom": "htdf", 
                "amount": "0.06" } ],
      "to": "htdf1rgsfxav0af8a79cmtq6rnjtjkqngl9qcj8k9l7",
      "data": ""
    }
    
    '
```
## 返回
```
// 1.首先返回
{
  "height": "0",
  "txhash": "30B2D681459610E7BDA731B1009FAFDF5D3080D9E94D15AE245C3A71A7039075"
}
//至此交易不一定正确，需要继续查询交易
~$ curl http://localhost:1317/txs/30B2D681459610E7BDA731B1009FAFDF5D3080D9E94D15AE245C3A717039075
{
  "height": "4680",
  "txhash": "30B2D681459610E7BDA731B1009FAFDF5D3080D9E94D15AE245C3A71A7039075",
  "log": [
    {
      "msg_index": "0",
      "success": true,
      "log": ""
    }
  ],
  "gas_wanted": "30000",
  "gas_used": "32970",
  "tags": [
    {
      "key": "action",
      "value": "sendfrom"
    },
    {
      "key": "sender",
      "value": "htdf1d8walt2k5824v4zxz6wp6n0mk7z6ml7f2mdagn"
    },
    {
      "key": "recipient",
      "value": "htdf1g9jlhyu07jjm4gfus52l28r29y4m063gmtxcx0"
    }
  ],
  "tx": {
    "msg": [
      {
        "From": "htdf1d8walt2k5824v4zxz6wp6n0mk7z6ml7f2mdagn",
        "To": "htdf1g9jlhyu07jjm4gfus52l28r29y4m063gmtxcx0",
        "Amount": [
          {
            "denom": "htdf",
            "amount": "10"
          }
        ],
        "Hash": "",
        "Memo": ""
      }
    ],
    "fee": {
      "amount": [
        {
          "denom": "htdf",
          "amount": "20"
        }
      ],
      "gas": "0.002"
    },
    "signatures": [
      {
        "pub_key": {
          "type": "tendermint/PubKeySecp256k1",
          "value": "A0BzWlukxj28cCB0MhN8ioZzPUoF/zkTGBHzb1jDkX1d"
        },
        "signature": "9k3UY5yfSxIVfyPrLoh8C6rrCs/tDHd89kmNwiIEJEhRIbmlTkbnvCN62PuXwEQbNyaTzxGBmPe7U/9nHMteHA=="
      }
    ],
    "memo": "Sent via Cosmos Voyager"
  }
}
```
## 正确标志

1. 首先send发送交易，返回height和txhash
2. 然后查询txhash，查看返回的log字段
3. log字段"success": true,则交易成功

## 可能出现的错误

### a.余额不足
```
  TxHash: 638CB8A0C88E6874DF063EAD8BD76935B161D7EB46958B5A09736612DC9FF314
  Code: 10
  Raw Log: [{"msg_index":"0","success":false,"log":"{\"codespace\":\"sdk\",\"code\":10,\"message\":\"insufficient account funds; 1999990000satoshi \u003c 2000000000satoshi\"}"}]
  Logs: [{"msg_index":0,"success":false,"log":"{\"codespace\":\"sdk\",\"code\":10,\"message\":\"insufficient account funds; 1999990000satoshi \u003c 2000000000satoshi\"}"}]
  GasWanted: 200000
  GasUsed: 15203

```
解决方案
```
转账金额to = from资金 - fee;
自己先算好资金，在进行转账
```
### b. signation error

>> 如果是在搭建的节点进行转账，出现单纯的签名错误且地址不是本地生成的，可能是.hscli下keystore下面没有key文件

解决方案
>> 将key文件，拷贝到对应目录

### c. 转账交易的特殊性
>> 同一个转出地址，同一个区块内，memo需要确保唯一； 否则会报交易已存在错误

```
{"error":"broadcast_tx_sync: Response error: RPC error -32603 - Internal error: Tx already exists in cache"}
```
解决方案


- 方案1（简单方案）：等待区块打包
  > 发完一笔交易后，sleep 8~10秒（即等待上一笔打包），再发下一笔交易


- 方案2（完整的通用方案）：改变seq和memo
  > 参考一：详见下方伪代码
  
  > 参考二：[离线签名demo](https://gitee.com/orientwalt/apidoc/tree/master/demo) 


```

## 伪代码，关于 sequence 和 memo 字段
>> 指定的转出地址， 每笔交易的sequence 和 memo ，必须有唯一性=
>> sequence 依次递增， memo 建议也依次递增；


//应用启动

//查询 seq
currSeq = GetSeqFromRPC()

while(1)
{
    //获取转出地址currToAddress 、转出金额 currAmount
    currToAddress = getToAddressFromParam();
    currAmount = getAmountFromParam();
    
    while(1)
    {

        //交易数据格式
        currPostData= {
                Sequence : currSeq,
                Memo :  "memo_currSeq",
                ToAddress: currToAddress, 
                Amount: currAmount
             }

        //发送交易
        send( currPostData) ;

        //交易发送成功，则 seq加1
         if 发送交易成功
         {
             currSeq++;
             break;
         }

        //睡眠和重试  
         sleep(1);

    }
     
}

```


### d.chainID问题

请注意交易所在链--主网mainchain，测试公网testchain

### e.关于普通交易gas相关字段

普通交易fee固定为 0.03 HTDF

即 gas_wanted 固定为 30000, gas_price 固定为 gas_price 

如下

```
curl --location --request POST 'http://htdf2020-test01.orientwalt.cn:1317/hs/send' \
--header 'Content-Type: application/json' \
--data-raw '    { "base_req": 
      { "from": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy", 
        "memo": "Sent via Cosmos Voyager ",
        "password": "12345678", 
        "chain_id": "testchain", 
        "account_number": "0", 
        "sequence": "0", 
        "gas_wanted": "30000",
        "gas_price": "100", 
         "simulate": false
      },          
      "amount": [ 
              { "denom": "htdf", 
                "amount": "0.06" } ],
      "to": "htdf1rgsfxav0af8a79cmtq6rnjtjkqngl9qcj8k9l7",
      "data": ""
    }
    
    '
```



## /broadcast 广播接口
- 成功应答

```
{
  "height":...,
  "txhash":..., 
}
```
- 错误应答
> 注意这个时候多了 `code`和 `raw_log` 两个字段； 而且 `txHash` 是有值的；
```

{
  "height":...,
  "txhash":...,
  "code":...,
  "raw_log":...}
}
```




