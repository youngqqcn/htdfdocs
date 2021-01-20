# send常见问题及其处理


## send接口及常见问题

- 使用[send 转账接口](./api.md#普通转账或创建并发布智能合约)接口发送交易后
- 返回结果:
    ```
    {
        "height": "0",
        "txhash": "0610DBF3B32308E4EF72C065E7E8A73FE9B019415365C45A8A0DA78593CCF60E"
    }

- 需继续使用接口[/txs/{txhash} 查询交易详情](./api.md#查询交易详情) , `logs[0]["success"]`为`true`, 则交易成功

    - 成功的交易:
        ```
        $ curl http://localhost:1317/txs/0610DBF3B32308E4EF72C065E7E8A73FE9B019415365C45A8A0DA78593CCF60E
        
        {
        "height": "4767567",
        "txhash": "0610DBF3B32308E4EF72C065E7E8A73FE9B019415365C45A8A0DA78593CCF60E",
        "raw_log": // 略,
        "logs": [
            {
                "msg_index": "0",
                "success": true,  // <==== 交易成功 ====
                "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"\",\"evm_output\":\"\"}"
            }
        ],
        // 略

        ```

    - 失败的交易:
        ```
        $ curl http://localhost:1317/txs/7F12916A9057380E60666E707C197F23E46FD3DB22DA7C9E4713E0C925915F69


        {
        "height": "4767953",
        "txhash": "7F12916A9057380E60666E707C197F23E46FD3DB22DA7C9E4713E0C925915F69",
        "code": 2,
        "raw_log": //略 ,
        "logs": [
            {
                "msg_index": "0",
                "success": false, // <=========== 失败
                "log": "{\"code\":2,\"message\":\"open contract error\",\"contract_address\":\"\",\"evm_output\":\"evm call error|err=invalid opcode 0xfe\\n\"}"
            }
        ],
        ```

---

可能出现的错误

### 1.余额不足
```
  TxHash: 638CB8A0C88E6874DF063EAD8BD76935B161D7EB46958B5A09736612DC9FF314
  Code: 10
  Raw Log: [{"msg_index":"0","success":false,"log":"{\"codespace\":\"sdk\",\"code\":10,\"message\":\"insufficient account funds; 1999990000satoshi \u003c 2000000000satoshi\"}"}]
  Logs: [{"msg_index":0,"success":false,"log":"{\"codespace\":\"sdk\",\"code\":10,\"message\":\"insufficient account funds; 1999990000satoshi \u003c 2000000000satoshi\"}"}]
  GasWanted: 200000
  GasUsed: 15203

```

解决方案:

自己先算好资金，再进行转账:

`可用余额 = 总余额 - 手续费 `


### 2. gas手续费相关问题

> HTDF公链采用类似ETH的机制: 
> `gas_price` 等同于ETH 的`gasPrice`, 
> `gas_wanted` 等同于ETH 的 `gasLimit`;

- 普通交易的fee（手续费）: `gas_price` 为`100`, `gas_wanted` 为 `30000`,  那么消耗手续费`0.03` HTDF, 即 `100 * 30000 / (10^8)` 


例如: 
- [各种语言的转账demo](../demos/)
- [Debug版hscli的send接口](./api.md#普通转账或创建并发布智能合约)



### 3. signation error

如果是在搭建的节点进行转账，出现单纯的签名错误且地址不是本地生成的，可能是.hscli下keystore下面没有key文件

解决方案: 将key文件，拷贝到对应目录

### 4. 如何批量转账

见[如何批量转账?](./FAQ.md##4.如何进行转账交易)



### 5.chainID问题

| 链类型       | ChainId | 
|-------------|---------|
| HTDF主网(链) | mainchain| 
| 测试网(链)   | testchain |


### 6.关于普通交易gas相关字段

普通交易fee固定为 `0.03` HTDF

即 `gas_wanted` 固定为 `30000`, `gas_price` 固定为 `100`


## broadcast广播接口

广播成功:
```
{
    "height":...,
    "txhash":..., 
}
```

广播错误:

响应多了 `code`和 `raw_log` 两个字段； 而且 `txHash` 是有值的；

```
{
  "height":...,
  "txhash":...,
  "code":...,
  "raw_log":...
}
```




