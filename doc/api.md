# HTDF RPC接口

目录
- [余额查询]( #余额查询 )
- [查询账户信息]( #查询账户信息 )
- [创建账户]( #创建账户) 
- [导出账户私钥]( #导出账户私钥 )
- [查账户列表]( #查账户列表 )
- [查询最新区块]( #查询最新区块 )
- [查询区块内容]( #查询区块内容 )
- [查询节点信息]( #查询节点信息 )
- [查询验证节点信息]( #查询验证节点信息 )
- [普通转账或创建并发布智能合约]( #普通转账或创建并发布智能合约 )
- [查询交易详情]( #查询交易详情 )
- [创建原始交易或智能合约交易]( #创建原始交易或智能合约交易 )
- [在线签名]( #在线签名 )
- [广播已签名的交易]( #广播已签名的交易 )
- [查询账户在某区块高度范围内的交易]( #查询账户在某区块高度范围内的交易)
- [查询交易池中未确认的所有交易详情](#查询交易池中未确认的所有交易详情)
- [查询交易池中未确认的单笔交易详情](#查询交易池中未确认的单笔交易详情)
- [查询交易池中未确认交易的总数量](#查询交易池中未确认交易的总数量)
- [查询验证者]( #查询验证者 )
- [导出共识状态]( #导出共识状态 )
- [查询未确认交易数量]( #查询未确认交易数量 )
- [查询未确认交易详情]( #查询未确认交易详情 )



## 余额查询

- HTTP请求方法：   `GET`

- endpoint:   `/bank/balances/{address}`

- 说明：   {address} 为账户地址. 

- 请求： 

  ```
  curl http://localhost:1317/bank/balances/htdf10nw7hujgmnl0hqfj7x3lv2vf40vaay64hkwnpm
  ```

- 响应 :

  ```json
  [
    {
      "denom": "htdf",
      "amount": "1000000000"
    }
  ]
  ```



##  查询账户信息
- HTTP请求方法：   `GET`    
- endpoint:   `/auth/accounts/{address}`
- 说明：   `{address}` 为账户地址 , 如果账户没有任何交易记录, `HTTP`状态码为`204`

- 请求： 
  ```
  curl http://localhost:1317/auth/accounts/htdf10nw7hujgmnl0hqfj7x3lv2vf40vaay64hkwnpm
  ```

- 响应 :
  ```json
    {
      "type": "auth/Account",
      "value": {
        "address": "htdf10nw7hujgmnl0hqfj7x3lv2vf40vaay64hkwnpm",
        "coins": [
          {
            "denom": "htdf",
            "amount": "1000000000"
          }
        ],
        "public_key": null,
        "account_number": "1",
        "sequence": "0"
      }
    }
  ```


## 创建账户

:warning: *警告: 仅限于Debug 版`hscli`, 用于热钱包(简单方便但不安全, 不推荐使用) , 推荐自行参考`demo`生成地址*

- HTTP请求方法：   `POST` 

- endpoint:    `/accounts/newaccount`
- 标注：   仅限于Debug 版`hscli`, 用于热钱包(简单方便但不安全, 不推荐使用) , 推荐自行参考`demo`生成地址
- 说明：   参数password是为新账户设置的密码

- 请求： 

  ```shell
  curl -X POST "http://localhost:1317/accounts/newaccount" -H "accept: application/json" -d "{\"password\": \"12345678\"}"
  ```

- 响应 :
  ```json
  {
    "address": "htdf1g9jlhyu07jjm4gfus52l28r29y4m063gmtxcx0"
  }
  ```


##  导出账户私钥

 :warning: *警告: 仅限于Debug 版`hscli`, 用于热钱包(简单方便但不安全, 不推荐使用) , 推荐自行参考`demo`生成私钥*

- HTTP请求方法：  `POST`

- endpoint:    `/accounts/export`
- 标注：   仅限于Debug 版`hscli`, 用于热钱包(简单方便但不安全, 不推荐使用) , 推荐自行参考`demo`生成私钥
- 说明：   `password`  是使用 ` /accounts/newaccount` 接口创建账户时设置的钱包密码

- 请求： 

  ```shell
  curl --location --request POST 'http://localhost:1317/accounts/export' \
  --header 'Content-Type: application/json' \
  --data-raw '{
    "address": "htdf10v3ewq09lq5m0mgd36x0g9stuhhgwffx0lmf3x",
    "password": "12345678"
  }'
  ```

- 响应 :
  
  ```json
  {
    "private_key": "8b24610a3cbfb4491f3ed88325a28254cdcea89f5cf507df62ab2c06b211edbc"
  }
  ```


##  查账户列表
- HTTP请求方法：    `GET` 
- endpoint:   `/auth/accounts/list`
- 请求： 

  ```shell
  curl http://localhost:1317/accounts/list
  ```

- 响应 :

  ```
  Account #0: {htdf1gy5lwx2zmfhk22amuudqu5etlflrdx93r0dmeh}
  Account #1: {htdf1d8walt2k5824v4zxz6wp6n0mk7z6ml7f2mdagn}
  Account #2: {htdf14ev6dr4y5ms46h8jw8rwmrpnt8wl9clme29am6}
  Account #3: {htdf1jduyhyensy5helsezcdpnd8qja5xhgs5cmq5ts}
  Account #4: {htdf1xm9g53mc0m0dyj807h2rtxw3y39zq4z0cs0nde}
  Account #5: {htdf1g9jlhyu07jjm4gfus52l28r29y4m063gmtxcx0}
  ```


##  查询最新区块
- HTTP请求方法：    `GET` 
- endpoint:  `/blocks/latest`
- 请求： 
  ```shell
  curl http://localhost:1317/blocks/latest
  ```

- 响应 :

  ```json
  {
    "block_meta": {
      "block_id": {
        "hash": "9AA37C8011DD1E2046951E36CC41B9FB8273AF79C198C4F4EC1CE97C731A15D4",
        "parts": {
          "total": "1",
          "hash": "0EAD2228A6408EFEEC90780227AFFE4A4867B1AFBA0596E9BFA730C0A8D42803"
        }
      },
      "header": {
        "version": {
          "block": "10",
          "app": "0"
        },
        "chain_id": "testchain",
        "height": "3361",
        "time": "2019-09-20T07:22:19.206881171Z",
        "num_txs": "0",
        "total_txs": "9",
        "last_block_id": {
          "hash": "FE0AB858A88142C5D21929B30ABCB3307225971F4961743D4ABA9AF11765662C",
          "parts": {
            "total": "1",
            "hash": "9DD6A664DCF9E527DF720576FC7D2B4249F0EC9E34E54957DFCC487027B947EE"
          }
        },
        "last_commit_hash": "D15156E590D6C89A8782E32A5EA13889D3724B437F385D2CA5A9EBDFBA9DC462",
        "data_hash": "",
        "validators_hash": "ED2CBAD61C7D2DFD336FF8A49892D9E73F9FE6539B704427C33B4CF1041F72C7",
        "next_validators_hash": "ED2CBAD61C7D2DFD336FF8A49892D9E73F9FE6539B704427C33B4CF1041F72C7",
        "consensus_hash": "048091BC7DDC283F77BFBF91D73C44DA58C3DF8A9CBC867405D8B7F3DAADA22F",
        "app_hash": "5AC57573161FFF216B79A4805BF8454C969DCA8A2DCC1498BD44E343C238B306",
        "last_results_hash": "",
        "evidence_hash": "",
        "proposer_address": "390949686A41E0B186AE52A47732F33B5DE88D3A"
      }
    },
    "block": {
      "header": {
        "version": {
          "block": "10",
          "app": "0"
        },
        "chain_id": "testchain",
        "height": "3361",
        "time": "2019-09-20T07:22:19.206881171Z",
        "num_txs": "0",
        "total_txs": "9",
        "last_block_id": {
          "hash": "FE0AB858A88142C5D21929B30ABCB3307225971F4961743D4ABA9AF11765662C",
          "parts": {
            "total": "1",
            "hash": "9DD6A664DCF9E527DF720576FC7D2B4249F0EC9E34E54957DFCC487027B947EE"
          }
        },
        "last_commit_hash": "D15156E590D6C89A8782E32A5EA13889D3724B437F385D2CA5A9EBDFBA9DC462",
        "data_hash": "",
        "validators_hash": "ED2CBAD61C7D2DFD336FF8A49892D9E73F9FE6539B704427C33B4CF1041F72C7",
        "next_validators_hash": "ED2CBAD61C7D2DFD336FF8A49892D9E73F9FE6539B704427C33B4CF1041F72C7",
        "consensus_hash": "048091BC7DDC283F77BFBF91D73C44DA58C3DF8A9CBC867405D8B7F3DAADA22F",
        "app_hash": "5AC57573161FFF216B79A4805BF8454C969DCA8A2DCC1498BD44E343C238B306",
        "last_results_hash": "",
        "evidence_hash": "",
        "proposer_address": "390949686A41E0B186AE52A47732F33B5DE88D3A"
      },
      "data": {
        "txs": null
      },
      "evidence": {
        "evidence": null
      },
      "last_commit": {
        "block_id": {
          "hash": "FE0AB858A88142C5D21929B30ABCB3307225971F4961743D4ABA9AF11765662C",
          "parts": {
            "total": "1",
            "hash": "9DD6A664DCF9E527DF720576FC7D2B4249F0EC9E34E54957DFCC487027B947EE"
          }
        },
        "precommits": [
          {
            "type": 2,
            "height": "3360",
            "round": "0",
            "block_id": {
              "hash": "FE0AB858A88142C5D21929B30ABCB3307225971F4961743D4ABA9AF11765662C",
              "parts": {
                "total": "1",
                "hash": "9DD6A664DCF9E527DF720576FC7D2B4249F0EC9E34E54957DFCC487027B947EE"
              }
            },
            "timestamp": "2019-09-20T07:22:19.206881171Z",
            "validator_address": "390949686A41E0B186AE52A47732F33B5DE88D3A",
            "validator_index": "0",
            "signature": "sk1EPNmmGGd32N8ubhGSgcavd1PbT9dPNONLeaFiLtOkyBBFjp/giN8e0MdLUz1/ciADDRc4Q5e9GZG58XulBw=="
          }
        ]
      }
    }
  }

  ```



##  查询区块内容

- HTTP请求方法：    `GET` 
- endpoint:   `/block_detail/{height}`
- 说明：   `height`：要查询的区块高度

- 请求： 

  ```shell
  curl http://localhost:1317/block_detail/100
  ```

- 响应 :

  ```json
  {
    "block_meta": {
      "block_id": {
        "hash": "14933AC1EFA3BA1D96E5F7194931A8C078D3498ADFEAD3A6322C17E054B16439",
        "parts": {
          "total": "1",
          "hash": "CBD69AF783CB9286715B08E3616FFD5BB2D1456451E12DDC993F79551B64F9F7"
        }
      },
      "header": {
        "version": {
          "block": "10",
          "app": "0"
        },
        "chain_id": "testchain",
        "height": "100",
        "time": "2019-08-07T06:22:29.796678433Z",
        "num_txs": "0",
        "total_txs": "0",
        "last_block_id": {
          "hash": "EDF8605805CD96438A8C221AD0C712AB4528279F192618B4BCFB55D454BB3121",
          "parts": {
            "total": "1",
            "hash": "70384FBE32273A549703359411E132135BCEB0BAB7EBC3B9BEB8BB80A4114BB0"
          }
        },
        "last_commit_hash": "18F91AA62E1400C2CEB2531B06A9D9051F835F9972FDED8AC792ECE221B3C920",
        "data_hash": "",
        "validators_hash": "CF1761357F59F515E321AC59F5A7C868EE16DA76BE4D34781966621272C7549C",
        "next_validators_hash": "CF1761357F59F515E321AC59F5A7C868EE16DA76BE4D34781966621272C7549C",
        "consensus_hash": "048091BC7DDC283F77BFBF91D73C44DA58C3DF8A9CBC867405D8B7F3DAADA22F",
        "app_hash": "0BADBA2437F499D3ECF77276CEB5782645A7308AE3D5954A9F6230D1BFEEA7D8",
        "last_results_hash": "",
        "evidence_hash": "",
        "proposer_address": "960C6246D5CFB6C2E5BD40E45831765CCCFB5E64"
      }
    },
    "block": {
      "txs": null,
      "evidence": {
        "evidence": null
      },
      "last_commit": {
        "block_id": {
          "hash": "EDF8605805CD96438A8C221AD0C712AB4528279F192618B4BCFB55D454BB3121",
          "parts": {
            "total": "1",
            "hash": "70384FBE32273A549703359411E132135BCEB0BAB7EBC3B9BEB8BB80A4114BB0"
          }
        },
        "precommits": [
          {
            "type": 2,
            "height": "99",
            "round": "0",
            "block_id": {
              "hash": "EDF8605805CD96438A8C221AD0C712AB4528279F192618B4BCFB55D454BB3121",
              "parts": {
                "total": "1",
                "hash": "70384FBE32273A549703359411E132135BCEB0BAB7EBC3B9BEB8BB80A4114BB0"
              }
            },
            "timestamp": "2019-08-07T06:22:29.796678433Z",
            "validator_address": "960C6246D5CFB6C2E5BD40E45831765CCCFB5E64",
            "validator_index": "0",
            "signature": "owJLjORDfrbgSWj8zGsSQXOEaC5uB+M+jTsJsK4G9PEFPA9k8xHoCSW2WCMdPZYtGbc49mF4nZMm2NRkrAiYBA=="
          }
        ]
      }
    },
    "time": "2019-08-07 14:22:29"
  }
  ```



##  查询节点信息
- HTTP请求方法：    `GET` 
- endpoint:   `/node_info`

- 请求： 

  ```
  curl http://localhost:1317/node_info
  ```

- 响应 :

  ```json
  {
    "protocol_version": {
      "p2p": "7",
      "block": "10",
      "app": "0"
    },
    "id": "007e8043c33e216643ce96988eaa945b0bd6abb8",
    "listen_addr": "tcp://0.0.0.0:26656",
    "network": "testchain",
    "version": "0.31.5",
    "channels": "4020212223303800",
    "moniker": "htdf_dev_net",
    "other": {
      "tx_index": "on",
      "rpc_address": "tcp://0.0.0.0:26657"
    }
  }
  ```


##  查询验证节点信息

- HTTP请求方法：    `GET` 
- endpoint:    `/validatorsets/latest`

- 请求： 

  ```shell
  curl http://localhost:1317/validatorsets/latest
  ```

- 响应 :

  ```json
  {
    "block_height": "4494",
    "validators": [
      {
        "address": "htdfvalcons1jcxxy3k4e7mv9edagrj9svtktnx0khnyqgpc4v",
        "pub_key": "htdfvalconspub1zcjduepqrnpf2jkfrn20rwvwavxut9va36rjxrxrnxz6e3teagnumvzqr0qqrakqmf",
        "proposer_priority": "0",
        "voting_power": "100"
      }
    ]
  }
  ```

##  普通转账或创建并发布智能合约

 :warning: *警告: 仅限于Debug 版`hscli`, 用于热钱包(简单方便但不安全, 不推荐使用) , 推荐自行参考`demo`使用自主签名交易并广播*

- HTTP请求方法：    `POST`   

- endpoint:     `/hs/send`
- 标注：   仅限于Debug 版`hscli`;  智能合约后续版本实现；
- 说明：  
  -  `from`: 发送者地址，
  - `memo`：备注， 
  - `password` ：密码
  - `chain_id`：链id , `testchain` :测试网 , `mainchain`:主网
  -  `account_number`：账户编号, 可通过 [查询账户信息接口](#查询账户信息)获取
  - `sequence`：序列号(账户的交易序号) , 可通过 [查询账户信息接口](#查询账户信息)获取
  - `amount`：金额
  -  `to`：接收者地址
  -  `data`: 普通HTDF转账, 此字段填空字符串即可

**例1: HTDF 转账**

  - 请求

  ```shell
curl  'http://localhost:1317/hs/send' -X POST \
-H 'Content-Type: application/json' \
-d '{ "base_req": 
      { 
      	"from": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy", 
        "memo": "Sent via Cosmos Voyager ",
        "password": "12345678", 
        "chain_id": "testchain", 
        "account_number": "0", 
        "sequence": "0", 
        "gas_wanted": "200000",
        "gas_price": "100", 
        "simulate": false
      },          
      "amount": [ 
          {
          "denom": "htdf", 
          "amount": "0.06" 
          }
       ],
      "to": "htdf1rgsfxav0af8a79cmtq6rnjtjkqngl9qcj8k9l7",
      "data": ""
    }
'
  ```

- 响应 :

  ```json
  {
    "height":"0",
    "txhash":"0B1B5197C0FAE0C7E0FE12C8767EF4501576E65E2D24B5D813DBDD829FA442E8"
  }
  ```

> 提示: 如此接口遇到问题可以参考:  [常见问题及其处理.md](./常见问题及其处理.md)

**例子 2 智能合约**

- 请求

  ```shell
  curl http://127.0.0.1:1317/hs/send \
      -H 'Content-Type: application/json' \
      -X POST \
      --data '{
                  "base_req": {
                      "from": "htdf10nw7hujgmnl0hqfj7x3lv2vf40vaay64hkwnpm",
                      "memo": "",
                      "password": "12345678",
                      "chain_id": "testchain",
                      "account_number": "0",
                      "sequence": "0",
                      "gas_wanted": "500000",
                      "gas_price": "100",
                      "simulate": false
                  },
                  "amount": [{
                      "denom": "htdf",
                      "amount": "0"
                  }],
                  "to": "",
                  "data": "6060604052341561000f57600080fd5b336000806101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555061042d8061005e6000396000f300606060405260043610610062576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff168063075461721461006757806327e235e3146100bc57806340c10f1914610109578063d0679d341461014b575b600080fd5b341561007257600080fd5b61007a61018d565b604051808273ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200191505060405180910390f35b34156100c757600080fd5b6100f3600480803573ffffffffffffffffffffffffffffffffffffffff169060200190919050506101b2565b6040518082815260200191505060405180910390f35b341561011457600080fd5b610149600480803573ffffffffffffffffffffffffffffffffffffffff169060200190919080359060200190919050506101ca565b005b341561015657600080fd5b61018b600480803573ffffffffffffffffffffffffffffffffffffffff16906020019091908035906020019091905050610277565b005b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1681565b60016020528060005260406000206000915090505481565b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff163373ffffffffffffffffffffffffffffffffffffffff1614151561022557610273565b80600160008473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020600082825401925050819055505b5050565b80600160003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000205410156102c3576103fd565b80600160003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000828254039250508190555080600160008473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020600082825401925050819055507f3990db2d31862302a685e8086b5755072a6e2b5b780af1ee81ece35ee3cd3345338383604051808473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020018373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001828152602001935050505060405180910390a15b50505600a165627a7a72305820f3c54d8cf0c62d5295ef69e3fc795fa1886b4de4d3d58f50f83c70ed26b99d890029"
              }'
  ```


- 响应 :
  ```json
  {
    "height": "0",
    "txhash": "2F7B060935C70BFB474C4A4DDD1692B019CF3DED9A412C2362ABD7E76F31EB59"
  }
  ```


##  查询交易详情

- HTTP请求方法：    `GET` 
- endpoint:     `/txs/{hash}`
- 说明：   {hash}:要查询的交易hash 


|交易类型|说明|
|:-----:|:----:|
|普通交易 |type字段为 `htdfservice/send` 、Data为空字符串 ""|
|智能合约交易 |type字段为 `htdfservice/send` 、Data不为空|
|扩展型交易|type字段不为 `htdfservice/send`，而是具体类型的类， 如`htdf/MsgWithdrawDelegationReward`、`MsgDelegate`等等 |


- 请求： 

  ```
  curl http://localhost:1317/txs/30B2D681459610E7BDA731B1009FAFDF5D3080D9E94D15AE245C3A717039075
  ```


- 1.普通交易的响应

  ```json
  {
    "height": "310",
    "txhash": "775587233120A3D813D37A6B07FE2F7E1B46E5B258E07844BCB373992D1C1485",
    "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"{\\\"code\\\":0,\\\"message\\\":\\\"ok\\\",\\\"contract_address\\\":\\\"\\\",\\\"evm_output\\\":\\\"\\\"}\"}]",
    "logs": [
      {
        "msg_index": "0",
        "success": true,
        "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"\",\"evm_output\":\"\"}"
      }
    ],
    "gas_wanted": "200000",
    "gas_used": "30000",
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
              "From": "htdf10nw7hujgmnl0hqfj7x3lv2vf40vaay64hkwnpm",
              "To": "htdf13reg33ddkczds8cpk9rzgpa39d90hck8znmef2",
              "Amount": [
                {
                  "denom": "satoshi",
                  "amount": "1000000000000"
                }
              ],
              "Data": "",
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
              "value": "AlFXfXWDvlZfhnB7BVulzNAhNebbsssBtEgCVx2Jx3k3"
            },
            "signature": "2xyJTpNDhTxShHCAuRYGlsaY6gNOPWkqcpss3A30kLVBYqSyfUUiS9VuFrMPgXVxzijBAAeyeQ5bIj/uPNVCHQ=="
          }
        ],
        "memo": "Sent via Cosmos Voyager"
      }
    },
    "timestamp": "2019-12-23T02:51:17Z"
  }
  ```


- 2.智能合约交易的响应

  ```json
  {
      "height": "362",
      "txhash": "12E458B3E3C9ECC4D0E3628E29F5BE534D619DE8F68C1F1FBCA592989A8EB99F",
      "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"{\\\"code\\\":0,\\\"message\\\":\\\"ok\\\",\\\"contract_address\\\":\\\"htdf1zuvnu4hlwlpnh0mwj4w5htcx3mgvx3t4zfzaut\\\",\\\"evm_output\\\":\\\"\\\"}\"}]",
      "logs": [
          {
              "msg_index": "0",
              "success": true,
              "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"htdf1zuvnu4hlwlpnh0mwj4w5htcx3mgvx3t4zfzaut\",\"evm_output\":\"\"}"
          }
      ],
      "gas_wanted": "5000000",
      "gas_used": "1025316",
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
                          "From": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy",
                          "To": "",
                          "Amount": [
                              {
                                  "denom": "satoshi",
                                  "amount": "0"
                              }
                          ],
                          "Data": "60606040526000600260006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff160217905550341561005157600080fd5b6aa49be39dc14cb8270000006003819055506003546000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002081905550610d61806100b76000396000f3006060604052600436106100af576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff16806306fdde03146100b4578063095ea7b31461014257806318160ddd1461019c57806323b872dd146101c5578063313ce5671461023e5780634d853ee51461026d57806370a08231146102c257806393c32e061461030f57806395d89b4114610348578063a9059cbb146103d6578063dd62ed3e14610430575b600080fd5b34156100bf57600080fd5b6100c761049c565b6040518080602001828103825283818151815260200191508051906020019080838360005b838110156101075780820151818401526020810190506100ec565b50505050905090810190601f1680156101345780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b341561014d57600080fd5b610182600480803573ffffffffffffffffffffffffffffffffffffffff169060200190919080359060200190919050506104d5565b604051808215151515815260200191505060405180910390f35b34156101a757600080fd5b6101af61065c565b6040518082815260200191505060405180910390f35b34156101d057600080fd5b610224600480803573ffffffffffffffffffffffffffffffffffffffff1690602001909190803573ffffffffffffffffffffffffffffffffffffffff16906020019091908035906020019091905050610662565b604051808215151515815260200191505060405180910390f35b341561024957600080fd5b610251610959565b604051808260ff1660ff16815260200191505060405180910390f35b341561027857600080fd5b61028061095e565b604051808273ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200191505060405180910390f35b34156102cd57600080fd5b6102f9600480803573ffffffffffffffffffffffffffffffffffffffff16906020019091905050610984565b6040518082815260200191505060405180910390f35b341561031a57600080fd5b610346600480803573ffffffffffffffffffffffffffffffffffffffff169060200190919050506109cc565b005b341561035357600080fd5b61035b610a6c565b6040518080602001828103825283818151815260200191508051906020019080838360005b8381101561039b578082015181840152602081019050610380565b50505050905090810190601f1680156103c85780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b34156103e157600080fd5b610416600480803573ffffffffffffffffffffffffffffffffffffffff16906020019091908035906020019091905050610aa5565b604051808215151515815260200191505060405180910390f35b341561043b57600080fd5b610486600480803573ffffffffffffffffffffffffffffffffffffffff1690602001909190803573ffffffffffffffffffffffffffffffffffffffff16906020019091905050610c77565b6040518082815260200191505060405180910390f35b6040805190810160405280600981526020017f414a4320636861696e000000000000000000000000000000000000000000000081525081565b60008082148061056157506000600160003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002054145b151561056c57600080fd5b81600160003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020819055508273ffffffffffffffffffffffffffffffffffffffff163373ffffffffffffffffffffffffffffffffffffffff167f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925846040518082815260200191505060405180910390a36001905092915050565b60035481565b600080600160008673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020549050600073ffffffffffffffffffffffffffffffffffffffff168473ffffffffffffffffffffffffffffffffffffffff161415151561072057600080fd5b80831115151561072f57600080fd5b610780836000808873ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002054610cfe90919063ffffffff16565b6000808773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002081905550610813836000808773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002054610d1790919063ffffffff16565b6000808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020819055506108688382610cfe90919063ffffffff16565b600160008773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060003373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020819055508373ffffffffffffffffffffffffffffffffffffffff168573ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef856040518082815260200191505060405180910390a360019150509392505050565b601281565b600260009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1681565b60008060008373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020549050919050565b600260009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff163373ffffffffffffffffffffffffffffffffffffffff16141515610a2857600080fd5b80600260006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555050565b6040805190810160405280600381526020017f414a43000000000000000000000000000000000000000000000000000000000081525081565b60008073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1614151515610ae257600080fd5b610b33826000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002054610cfe90919063ffffffff16565b6000803373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002081905550610bc6826000808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002054610d1790919063ffffffff16565b6000808573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020819055508273ffffffffffffffffffffffffffffffffffffffff163373ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef846040518082815260200191505060405180910390a36001905092915050565b6000600160008473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060008373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002054905092915050565b6000828211151515610d0c57fe5b818303905092915050565b6000808284019050838110151515610d2b57fe5b80915050929150505600a165627a7a7230582043a3cd97586e182885676a8c6e6413be040c6f728b9763d794ecdbfff9a4b7c90029",
                          "GasPrice": "100",
                          "GasWanted": "5000000"
                      }
                  }
              ],
              "fee": {
                  "gas_wanted": "5000000",
                  "gas_price": "100"
              },
              "signatures": [
                  {
                      "pub_key": {
                          "type": "tendermint/PubKeySecp256k1",
                          "value": "A2t2ZhUf8W2rq3TEDCmYJCJ7Qrermowx4TR5MzDC4nOE"
                      },
                      "signature": "DhhCSLCFABO+ZfNFR/qoZJn5v1JXsBk+9IUFdD98qJ1YvxfgCeZpYEvUtenV6ojU17u8QnEMAV2P7gqBthxMpg=="
                  }
              ],
              "memo": "contract test"
          }
      },
      "timestamp": "2020-04-06T10:17:02Z"
  }

  ```

- 3.扩展型交易的响应

  ```json
  {
      "height": "98649",
      "txhash": "106C4697D29F21A7A7515AEF493D3FEE54F9F773185A0EB328B875A81AEB6DD5",
      "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"\"}]",
      "logs": [
          {
              "msg_index": "0",
              "success": true,
              "log": ""
          }
      ],
      "gas_wanted": "200000",
      "gas_used": "30000",
      "tags": [
          {
              "key": "action",
              "value": "withdraw_delegator_reward"
          },
          {
              "key": "rewards",
              "value": "1109652768satoshi"
          },
          {
              "key": "delegator",
              "value": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy"
          },
          {
              "key": "source-validator",
              "value": "htdfvaloper1hv3hgjq9qadlnsf38qsrmnjwua92gm80ql45g7"
          }
      ],
      "tx": {
          "type": "auth/StdTx",
          "value": {
              "msg": [
                  {
                      "type": "htdf/MsgWithdrawDelegationReward",
                      "value": {
                          "delegator_address": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy",
                          "validator_address": "htdfvaloper1hv3hgjq9qadlnsf38qsrmnjwua92gm80ql45g7"
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
                          "value": "A2t2ZhUf8W2rq3TEDCmYJCJ7Qrermowx4TR5MzDC4nOE"
                      },
                      "signature": "oEm0GjdEtgUzFccudIdAtBdP2Kym9MrQe+GO8XNkH1cAnwFObkjWy8y0kpaKBM9GG7bRNz+B0BvEkFiErn4hOA=="
                  }
              ],
              "memo": ""
          }
      },
      "timestamp": "2020-04-13T06:58:53Z"
  }

  ```





## 创建原始交易或智能合约交易

- HTTP请求方法： `POST`

- endpoint:    `/hs/create`

- 标注：   仅限于Debug 版`hscli`;   智能合约后续版本实现；

- 说明：    返回值：16进制的字符串,交易的十六进制编码 (可以用于sign接口)

  -  `from`: 发送者地址，
  -  `memo`：备注， 
  -  `password` ：密码
  -  `chain_id`：链id , `testchain` :测试网 , `mainchain`:主网
  -  `account_number`：账户编号, 可通过 [查询账户信息接口](#查询账户信息)获取
  -  `sequence`：序列号(账户的交易序号) , 可通过 [查询账户信息接口](#查询账户信息)获取
  -  `amount`：金额
  -  `to`：接收者地址
  -  `data`: 普通HTDF转账, 此字段填空字符串即可
  -  `gas_wanted`: 普通转账设置为`30000`即可
  -  `gas_price`: 默认`100`即可

- 关联接口 :

  - `/hs/create` : 创建未签名交易
  - `/hs/sign`: 对未签名交易(unsigned tx)进行签名
  - `/hs/broadcast`: 广播已签名交易(signed tx)

- 请求 : 原始交易（raw）： 

  ```shell
  curl --location --request POST 'http://htdf2020-test01.orientwalt.cn:1317/hs/create' \
  --header 'Content-Type: application/json' \
  --data '    { "base_req": 
        { "from": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy", 
          "memo": "test",
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
        "data": "",
        "encode": true
      }
      '

  ```

- 响应 :

  ```
  7b2274797065223a22617574682f5374645478222c2276616c7565223a7b226d7367223a5b7b2274797065223a2268746466736572766963652f73656e64222c2276616c7565223a7b2246726f6d223a226874646631736838643368306e6e387434653833637263716c383077756137753378746c666a3564656a33222c22546f223a2268746466317267736678617630616638613739636d747136726e6a746a6b716e676c3971636a386b396c37222c22416d6f756e74223a5b7b2264656e6f6d223a227361746f736869222c22616d6f756e74223a2236303030303030227d5d2c2244617461223a22222c224761735072696365223a22313030222c2247617357616e746564223a223330303030227d7d5d2c22666565223a7b226761735f77616e746564223a223330303030222c226761735f7072696365223a22313030227d2c227369676e617475726573223a6e756c6c2c226d656d6f223a2253656e742076696120436f736d6f7320566f7961676572227d7d
  ```
  
  


## 在线签名

 :warning: *警告: 仅限于Debug 版`hscli`, 用于热钱包(简单方便但不安全, 不推荐使用) , 推荐自行参考`demo`进行签名*

- HTTP请求方法： `POST` 
- endpoint:   `/hs/sign`
- 标注：  仅限于Debug 版`hscli`
- 返回：十六进制编码的已签名的交易(signed_tx_hex)可以用于广播接口(`/hs/broadcast`)
- 请求参数说明： 
  - `tx`：由创建原始交易返回的十六进制编码的未交易(unsigned_tx_hex)
  - `passphrase`：账户密码
  - `offline`:  是否是离线模式
    - `fasle`: 在线模式, 此模式下会检查tx中的account_number和sequence是否为链上最新状态. 
    - `true`: 离线模式, 此模式下不会检查tx中的account_number和sequence.
  - `encode`: 是否将响应结果进行十六进制格式编码(hex_encode)
    - `true`: 进行编码
    - `false`: 已签名交易以json格式返回(主要用于调试)
- 关联接口 :
  - `/hs/create` : 创建未签名交易
  - `/hs/sign`: 对未签名交易(unsigned tx)进行签名
  - `/hs/broadcast`: 广播已签名交易(signed tx)

请求：

```
curl -X POST "http://localhost:1317/hs/sign" -H "accept: application/json" -H "Content-Type: application/json" -d "{\"tx\":\"7b2274797065223a22617574682f5374645478222c2276616c7565223a7b226d7367223a5b7b2274797065223a2268746466736572766963652f73656e64222c2276616c7565223a7b2246726f6d223a226874646631306e773768756a676d6e6c306871666a3778336c763276663430766161793634686b776e706d222c22546f223a22687464663133726567333364646b637a64733863706b39727a677061333964393068636b387a6e6d656632222c22416d6f756e74223a5b7b2264656e6f6d223a227361746f736869222c22616d6f756e74223a223130303030303030227d5d2c2244617461223a22222c224761735072696365223a22313030222c2247617357616e746564223a223330303030227d7d5d2c22666565223a7b226761735f77616e746564223a223330303030222c226761735f7072696365223a22313030227d2c227369676e617475726573223a6e756c6c2c226d656d6f223a2253656e742076696120436f736d6f7320566f7961676572227d7d\", \"passphrase\":\"12345678\",\"offline\":false,\"encode\":true}"
```

响应 :

```
7b0a20202274797065223a2022617574682f5374645478222c0a20202276616c7565223a207b0a20202020226d7367223a205b0a2020202020207b0a20202020202020202274797065223a202268746466736572766963652f73656e64222c0a20202020202020202276616c7565223a207b0a202020202020202020202246726f6d223a2022687464663165787078706b336b307737637065303630376b75686a6e6d78786c78347174633866637a3775222c0a2020202020202020202022546f223a20226874646631787774713775326d6b68793935677576786d3473326879306561307061726c796a74756c6736222c0a2020202020202020202022416d6f756e74223a205b0a2020202020202020202020207b0a20202020202020202020202020202264656e6f6d223a20227361746f736869222c0a202020202020202020202020202022616d6f756e74223a202231303030303030303030220a2020202020202020202020207d0a202020202020202020205d0a20202020202020207d0a2020202020207d0a202020205d2c0a2020202022666565223a207b0a20202020202022616d6f756e74223a205b0a20202020202020207b0a202020202020202020202264656e6f6d223a20227361746f736869222c0a2020202020202020202022616d6f756e74223a202232303030303030303030220a20202020202020207d0a2020202020205d2c0a20202020202022676173223a20223230303030303030303030303030220a202020207d2c0a20202020227369676e617475726573223a205b0a2020202020207b0a2020202020202020227075625f6b6579223a207b0a202020202020202020202274797065223a202274656e6465726d696e742f5075624b6579536563703235366b31222c0a202020202020202020202276616c7565223a2022417774656833714255664630304855796450722b78326f354c79676c4f5a2f4e36372b414c66685a7a4e7672220a20202020202020207d2c0a2020202020202020227369676e6174757265223a20224c38684b56454f467435586a4c754770496939503533496b4f4d66593064516f4e786532656d397945534e70334a476a747767365a61733261576954716f556b4a763659446d33524e4a4d592f2b37424c76346c56413d3d220a2020202020207d0a202020205d2c0a20202020226d656d6f223a202253656e742076696120436f736d6f7320566f7961676572220a20207d0a7d
```


## 广播已签名的交易

- HTTP请求方法：`POST`

- endpoint: `/hs/broadcast`

- 说明： 
  
- `tx`：十六进制格式编码的已签名交易(signed tx encoded in hex format)
  
- 关联接口 :

  - 冷钱包: 使用代码自行构造交易并签名, 使用此接口进行交易广播
  - 热钱包转账方式1:
    - `/hs/send` : 转账
  - 热钱包转账方式2:
    - `/hs/create` : 创建未签名交易
    - `/hs/sign`: 对未签名交易(unsigned tx)进行签名
    - `/hs/broadcast`: 广播已签名交易(signed tx)

- 请求： 

  ```shell
  curl -X POST "http://localhost:1317/hs/broadcast" -H "accept: aplication/json" -H "Content-Type: application/json" -d "{\"tx\":\"7b0a20202274797065223a2022617574682f5374645478222c0a20202276616c7565223a207b0a20202020226d7367223a205b0a2020202020207b0a20202020202020202274797065223a202268746466736572766963652f73656e64222c0a20202020202020202276616c7565223a207b0a202020202020202020202246726f6d223a20226874646631306e773768756a676d6e6c306871666a3778336c763276663430766161793634686b776e706d222c0a2020202020202020202022546f223a2022687464663133726567333364646b637a64733863706b39727a677061333964393068636b387a6e6d656632222c0a2020202020202020202022416d6f756e74223a205b0a2020202020202020202020207b0a20202020202020202020202020202264656e6f6d223a20227361746f736869222c0a202020202020202020202020202022616d6f756e74223a20223130303030303030220a2020202020202020202020207d0a202020202020202020205d2c0a202020202020202020202244617461223a2022222c0a20202020202020202020224761735072696365223a2022313030222c0a202020202020202020202247617357616e746564223a20223330303030220a20202020202020207d0a2020202020207d0a202020205d2c0a2020202022666565223a207b0a202020202020226761735f77616e746564223a20223330303030222c0a202020202020226761735f7072696365223a2022313030220a202020207d2c0a20202020227369676e617475726573223a205b0a2020202020207b0a2020202020202020227075625f6b6579223a207b0a202020202020202020202274797065223a202274656e6465726d696e742f5075624b6579536563703235366b31222c0a202020202020202020202276616c7565223a2022416c465866585744766c5a66686e42374256756c7a4e41684e65626273737342744567435678324a78336b33220a20202020202020207d2c0a2020202020202020227369676e6174757265223a202274557375642b696f7853594e7438635461716b394f7879624c396d3652515845415a6e7a507544504d793067462b3671316f726878354374517870444b4b76467157724a4c57437a6e6e5843677a557a646e30306e413d3d220a2020202020207d0a202020205d2c0a20202020226d656d6f223a202253656e742076696120436f736d6f7320566f7961676572220a20207d0a7d\"}"
  ```

- 响应 :

  ```json
  {
    "height": "0",
    "txhash": "9538E311F3CACD94749D60721A3B5460E35C86FB3AEBB66778A3A980E3DD1925"
  }
  ```

##  查询账户在某区块高度范围内的交易 

- HTTP请求方法：`POST`
- endpoint:`/accounts/transactions`
- 说明： 
  - `address`：指定的账户地址
  - `fromHeight` ：起始高度
  - `endHeight`：结束高度
    
    >  若`fromHeight`, `endHeight` 全为0, 则查询指定账户地址近1000个块范围内相关交易

- 请求： 

  ```shell
  curl -X POST "http://localhost:1317/accounts/transactions" -H "accept: application/json" -d "{\"address\": \"htdf10nw7hujgmnl0hqfj7x3lv2vf40vaay64hkwnpm\",\"fromHeight\":0,\"endHeight\":0,\"Flag\":0}"
  ```

- 响应 :
  ```json
  {
    "ChainHeight": "5745",
    "FromHeight": "100",
    "EndHeight": "1000",
    "ArrTx": null
  }
  ```

## 查询交易池中未确认的所有交易详情

👉 注意: hscli版本v1.3.0及以上. 此接口仅用于特定场景. 普通交易的转账和查询操作不需要用到此接口(因为, 正常情况下交易会在5秒内打包进区块).

- HTTP请求方法：`GET`

- endpoint:`/mempool/txs`

- 说明:

  - "所有" 最多能查询交易池中前`101`笔交易.即如果交易池中的交易多于`101`, 则只返回前`101`笔交易
  - 因交易池中的交易尚未被打包进区块, 所以, `height`为`0`

- 请求:

  ```shell
  curl http://localhost:1317/mempool/txs
  ```

- 响应:

  ```json
  {
  	"mempool_txs_count": "4992",
  	"mempool_total_bytes": "2437032",
  	"txs_count": "101",
  	"txs": [{
  			"height": "0",
  			"txhash": "b64fd647e735a2ebf1ac1464589dac00b611be94334e4f32721f1da84a556f0a",
  			"tx": {
  				"type": "auth/StdTx",
  				"value": {
  					"msg": [{
  						"type": "htdfservice/send",
  						"value": {
  							"From": "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2",
  							"To": "",
  							"Amount": [{
  								"denom": "satoshi",
  								"amount": "0"
  							}],
  							"Data": "ffffffffffffffffffffffff",
  							"GasPrice": "100",
  							"GasWanted": "7500000"
  						}
  					}],
  					"fee": {
  						"gas_wanted": "7500000",
  						"gas_price": "100"
  					},
  					"signatures": [{
  						"pub_key": {
  							"type": "tendermint/PubKeySecp256k1",
  							"value": "AynNwicLJpg9SSyojXQ04vGcd2DYe4663ynQFrk5Mp6d"
  						},
  						"signature": "EhouYmnC+NN5GG4hxl+jYhVFdxxPMr3yldd/GslTKhAyn/NWe+0a77kTtXg8C07JfOMXxuKZKP5osNGwJGwIAA=="
  					}],
  					"memo": "yqq1"
  				}
  			}
  		},
  		{
  			"height": "0",
  			"txhash": "d4ff48c6e9062561944672437e248c70b2d7fef2e3199182c2b23b20c92ec7c4",
  			"tx": {
  				"type": "auth/StdTx",
  				"value": {
  					"msg": [{
  						"type": "htdfservice/send",
  						"value": {
  							"From": "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2",
  							"To": "",
  							"Amount": [{
  								"denom": "satoshi",
  								"amount": "0"
  							}],
  							"Data": "ffffffffffffffffffffffff",
  							"GasPrice": "100",
  							"GasWanted": "7500000"
  						}
  					}],
  					"fee": {
  						"gas_wanted": "7500000",
  						"gas_price": "100"
  					},
  					"signatures": [{
  						"pub_key": {
  							"type": "tendermint/PubKeySecp256k1",
  							"value": "AynNwicLJpg9SSyojXQ04vGcd2DYe4663ynQFrk5Mp6d"
  						},
  						"signature": "wd2n7pq0fxKT25ZM17ixLGQ1QxbDnYOseAPCBPU5YwcWLzsYLcNKqR13b/yZgMWBvWKUB1our0WrFVzi5Zl9Sw=="
  					}],
  					"memo": "yqq1"
  				}
  			}
  		}
          //其他省略.....
  	]
  }
  ```



## 查询交易池中未确认的单笔交易详情

👉 注意: hscli版本需在v1.3.0及以上. 此接口仅用于特定场景. 普通交易的转账和查询操作不需要用到此接口(因为, 正常情况下交易会在5秒内打包进区块).

- HTTP请求方法：`GET`

- endpoint:`/mempool/txs/{tx_hash}`

- 说明: 因交易池中的交易尚未被打包进区块, 所以, `height`为`0`

- 请求:

  ```shell
  curl http://localhost:1317/mempool/txs/18961cda3e65a7a10bafc6a0c621b60c6af6111e754c9ddcc08b7883816d9276
  ```

- 响应:

  ```json
  {
      "height": "0",
      "txhash": "18961cda3e65a7a10bafc6a0c621b60c6af6111e754c9ddcc08b7883816d9276",
      "tx": {
          "type": "auth/StdTx",
          "value": {
              "msg": [
                  {
                      "type": "htdfservice/send",
                      "value": {
                          "From": "htdf1k6euyac0lsdzzz75geer62verf7gud5wgckvxy",
                          "To": "htdf1xa655qdecmey75y8tcgfjlew34sk6llxmflaxf",
                          "Amount": [
                              {
                                  "denom": "satoshi",
                                  "amount": "10"
                              }
                          ],
                          "Data": "",
                          "GasPrice": "100",
                          "GasWanted": "7500000"
                      }
                  }
              ],
              "fee": {
                  "gas_wanted": "7500000",
                  "gas_price": "100"
              },
              "signatures": [
                  {
                      "pub_key": {
                          "type": "tendermint/PubKeySecp256k1",
                          "value": "A1G/IzwJR/tKz4Ogwx+vlc0zWbN91KlkxZCvEsIzalcT"
                      },
                      "signature": "1Zq5VK5af8x8LCKUK19SBgA0I69a/N/l2ArRBZRujgIUMWb95JZYESy/02IVAu/9TCGDBx9Dndhc/2sqEFQ8Uw=="
                  }
              ],
              "memo": "yqq1"
          }
      }
  }
  ```



## 查询交易池中未确认交易的总数量

👉 注意: hscli版本需在v1.3.0及以上. 此接口仅用于特定场景. 普通交易的转账和查询操作不需要用到此接口(因为, 正常情况下交易会在5秒内打包进区块).

- HTTP请求方法：`GET`

- endpoint:`/mempool/txscount`

- 说明: 

- 请求:

  ```shell
  curl http://localhost:1317/mempool/txscount
  ```

- 响应:

  ```json
  {
      "mempool_txs_count": "3432",
      "mempool_total_bytes": "689832"
  }
  ```

  




***

*注意以下是Tendermint的RPC接口,  注意RPC端口的不同*

## 查询验证者

- HTTP请求方法：    `GET` 
- endpoint:     `/validators`
- 请求： 

  ```shell
  curl http://localhost:26657/validators
  ```


- 响应 :
  ```json
  {
    "jsonrpc": "2.0",
    "id": "",
    "result": {
      "block_height": "5070",
      "validators": [
        {
          "address": "960C6246D5CFB6C2E5BD40E45831765CCCFB5E64",
          "pub_key": {
            "type": "tendermint/PubKeyEd25519",
            "value": "HMKVSskc1PG5jusNxZWdjocjDMOZhazFeeonzbBAG8A="
          },
          "voting_power": "100",
          "proposer_priority": "0"
        }
      ]
    }
  }
  ```


## 导出共识状态

- HTTP请求方法：    `GET` 
- endpoint:    `/dump_consensus_state`

- 请求： 

  ```shell
  curl http://localhost:26657/dump_consensus_state
  ```

- 响应 :

  ```json
  {
    "jsonrpc": "2.0",
    "id": "",
    "result": {
      "round_state": {
        "height": "5113",
        "round": "0",
        "step": 1,
        "start_time": "2019-08-08T10:03:12.422187231Z",
        "commit_time": "2019-08-08T10:03:07.422187231Z",
        "validators": {
          "validators": [
            {
              "address": "960C6246D5CFB6C2E5BD40E45831765CCCFB5E64",
              "pub_key": {
                "type": "tendermint/PubKeyEd25519",
                "value": "HMKVSskc1PG5jusNxZWdjocjDMOZhazFeeonzbBAG8A="
              },
              "voting_power": "100",
              "proposer_priority": "0"
            }
          ],
          "proposer": {
            "address": "960C6246D5CFB6C2E5BD40E45831765CCCFB5E64",
            "pub_key": {
              "type": "tendermint/PubKeyEd25519",
              "value": "HMKVSskc1PG5jusNxZWdjocjDMOZhazFeeonzbBAG8A="
            },
            "voting_power": "100",
            "proposer_priority": "0"
          }
        },
        "proposal": null,
        "proposal_block": null,
        "proposal_block_parts": null,
        "locked_round": "-1",
        "locked_block": null,
        "locked_block_parts": null,
        "valid_round": "-1",
        "valid_block": null,
        "valid_block_parts": null,
        "votes": [
          {
            "round": "0",
            "prevotes": [
              "nil-Vote"
            ],
            "prevotes_bit_array": "BA{1:_} 0/100 = 0.00",
            "precommits": [
              "nil-Vote"
            ],
            "precommits_bit_array": "BA{1:_} 0/100 = 0.00"
          }
        ],
        "commit_round": "-1",
        "last_commit": {
          "votes": [
            "Vote{0:960C6246D5CF 5112/00/2(Precommit) E54D048BC8D6 00A8376F0B9B @ 2019-08-08T10:03:07.419583284Z}"
          ],
          "votes_bit_array": "BA{1:x} 100/100 = 1.00",
          "peer_maj_23s": {}
        },
        "last_validators": {
          "validators": [
            {
              "address": "960C6246D5CFB6C2E5BD40E45831765CCCFB5E64",
              "pub_key": {
                "type": "tendermint/PubKeyEd25519",
                "value": "HMKVSskc1PG5jusNxZWdjocjDMOZhazFeeonzbBAG8A="
              },
              "voting_power": "100",
              "proposer_priority": "0"
            }
          ],
          "proposer": {
            "address": "960C6246D5CFB6C2E5BD40E45831765CCCFB5E64",
            "pub_key": {
              "type": "tendermint/PubKeyEd25519",
              "value": "HMKVSskc1PG5jusNxZWdjocjDMOZhazFeeonzbBAG8A="
            },
            "voting_power": "100",
            "proposer_priority": "0"
          }
        },
        "triggered_timeout_precommit": false
      },
      "peers": []
    }
  }
  ```


##  查询未确认交易数量

- HTTP请求方法：    `GET` 
- endpoint:     `/num_unconfirmed_txs`
- 请求： 

  ```shell
  curl http://localhost:26657/num_unconfirmed_txs
  ```


- 响应 :

  ```json
  {
    "jsonrpc": "2.0",
    "id": "",
    "result": {
      "n_txs": "0",
      "total": "0",
      "total_bytes": "0",
      "txs": null
    }
  }
  ```

##  查询未确认交易详情

- endpoint:    `/unconfirmed_txs`
- 说明：    若没有未验证的tx 则 `txs` 字段为空
- HTTP请求方法：    `GET` 
- 请求： 

  ```shell
  curl http://localhost:26657/unconfirmed_txs
  ```

- 响应 :

  ```json
  {
    "jsonrpc": "2.0",
    "id": "",
    "result": {
      "n_txs": "0",
      "total": "0",
      "total_bytes": "0",
      "txs": []
    }
  }
  ```


(结束)
