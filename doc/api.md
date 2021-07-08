# HTDF RPC接口


**HTDF2.0中有小改动的RPC接口, 如下:**
- [查询最新区块]( #查询最新区块 )
- [查询区块内容]( #查询区块内容 )
- [查询交易详情]( #查询交易详情 )

其他RPC接口基本上保持不变, 原则上尽量做到不影响应用层(钱包,交易所等等). 但是出于稳妥起见, 应用方应该进行相关的测试.

---

目录
- [创建账户]( #创建账户)
- [余额查询]( #余额查询 )
- [查询账户信息]( #查询账户信息 )
- [send转账或创建并发布智能合约]( #普通转账或创建并发布智能合约 )
- [查账户列表]( #查账户列表 )
- [查询最新区块]( #查询最新区块 )
- [查询区块内容]( #查询区块内容 )
- [查询节点信息]( #查询节点信息 )
- [查询验证节点信息]( #查询验证节点信息 )
- [查询交易详情]( #查询交易详情 )
- [查询交易收据(HTDF2.0新增)]( #查询交易收据 )
- [创建原始交易或智能合约交易]( #创建原始交易或智能合约交易 )
- [在线签名]( #在线签名 )
- [导出账户私钥]( #导出账户私钥 )
- [广播已签名的交易]( #广播已签名的交易 )
- [查询账户在某区块高度范围内的交易]( #查询账户在某区块高度范围内的交易)
- [查询交易池中未确认的所有交易详情](#查询交易池中未确认的所有交易详情)
- [查询交易池中未确认的单笔交易详情](#查询交易池中未确认的单笔交易详情)
- [查询交易池中未确认交易的总数量](#查询交易池中未确认交易的总数量)
- [查询验证者]( #查询验证者 )
- [导出共识状态]( #导出共识状态 )
- [查询未确认交易数量]( #查询未确认交易数量 )
- [查询未确认交易详情]( #查询未确认交易详情 )


---

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

---


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

---

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


---

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

---

##  查询最新区块
- HTTP请求方法：    `GET`
- endpoint:  `/blocks/latest`
- 请求：
  ```shell
  curl http://localhost:1317/blocks/latest
  ```

- 响应 :

  HTDF1.x响应
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

  HTDF2.0响应:

  - 增加了`events`字段: 用于委托和收益提取操作的记录
  ```json
    {
        "block_meta": {
            "block_id": {
                "hash": "952092DB8C384C39CC6C02F4C32A4852B09214D1A30221BD4BBB66F5E0F44B63",
                "parts": {
                    "total": "1",
                    "hash": "4D4A5F7A1D4BF6A51F40319B2E202A029B7D1467685F25B8018B4530B14D0FEB"
                }
            },
            "header": {
                "version": {
                    "block": "10",
                    "app": "0"
                },
                "chain_id": "testchain",
                "height": "12378",
                "time": "2021-07-08T06:24:20.529242313Z",
                "last_block_id": {
                    "hash": "24644194E43A7D4E1246B028A25B66969815413AAE99D79C3B047516862CF02B",
                    "parts": {
                        "total": "1",
                        "hash": "BBB8D39E39884CB133C44AF5E7353D5D432084A985F2AB5E503EF070E592F330"
                    }
                },
                "last_commit_hash": "3ED4A41FF965EF7CEFDF9E0E8E4A18D2A7493318EAA67A18CF9F2BCD6FE79FB8",
                "data_hash": "",
                "validators_hash": "B9523061DF5D36ADE95B20516F32DDBC060EFC36EA478B3A1235364EC79D9E20",
                "next_validators_hash": "B9523061DF5D36ADE95B20516F32DDBC060EFC36EA478B3A1235364EC79D9E20",
                "consensus_hash": "BB4E36307426C4131EECAACD63BAB5802FF378CAD3CA8FF913275147E8F2AE2E",
                "app_hash": "3A45D8BEFBE554AFAAF10260483A6D2FC31A4E64BF59B0856AE5249B96D9C0E8",
                "last_results_hash": "",
                "evidence_hash": "",
                "proposer_address": "5C446BE6267A70E329B52C814097E10197B45BAA"
            }
        },
        "block": {
            "header": {
                "version": {
                    "block": "10",
                    "app": "0"
                },
                "chain_id": "testchain",
                "height": "12378",
                "time": "2021-07-08T06:24:20.529242313Z",
                "last_block_id": {
                    "hash": "24644194E43A7D4E1246B028A25B66969815413AAE99D79C3B047516862CF02B",
                    "parts": {
                        "total": "1",
                        "hash": "BBB8D39E39884CB133C44AF5E7353D5D432084A985F2AB5E503EF070E592F330"
                    }
                },
                "last_commit_hash": "3ED4A41FF965EF7CEFDF9E0E8E4A18D2A7493318EAA67A18CF9F2BCD6FE79FB8",
                "data_hash": "",
                "validators_hash": "B9523061DF5D36ADE95B20516F32DDBC060EFC36EA478B3A1235364EC79D9E20",
                "next_validators_hash": "B9523061DF5D36ADE95B20516F32DDBC060EFC36EA478B3A1235364EC79D9E20",
                "consensus_hash": "BB4E36307426C4131EECAACD63BAB5802FF378CAD3CA8FF913275147E8F2AE2E",
                "app_hash": "3A45D8BEFBE554AFAAF10260483A6D2FC31A4E64BF59B0856AE5249B96D9C0E8",
                "last_results_hash": "",
                "evidence_hash": "",
                "proposer_address": "5C446BE6267A70E329B52C814097E10197B45BAA"
            },
            "data": {
                "txs": null
            },
            "evidence": {
                "evidence": null
            },
            "last_commit": {
                "height": "12377",
                "round": "0",
                "block_id": {
                    "hash": "24644194E43A7D4E1246B028A25B66969815413AAE99D79C3B047516862CF02B",
                    "parts": {
                        "total": "1",
                        "hash": "BBB8D39E39884CB133C44AF5E7353D5D432084A985F2AB5E503EF070E592F330"
                    }
                },
                "signatures": [
                    {
                        "block_id_flag": 2,
                        "validator_address": "5C446BE6267A70E329B52C814097E10197B45BAA",
                        "timestamp": "2021-07-08T06:24:20.529242313Z",
                        "signature": "rkrpRwnQdChzm4fZPDHfRfgCvPNfEgslRUQraEW3Dih870jm1qghpOsYUZCVnHarDVPwEljie3q/J1Z5GyJDCQ=="
                    }
                ]
            }
        },
        "events": []
    }
  ```



---

##  查询区块内容

- HTTP请求方法：    `GET`
- endpoint:   `/block_detail/{height}`
- 说明：   `height`：要查询的区块高度

- 请求：

  ```shell
  curl http://localhost:1317/block_detail/1222934
  ```

- 响应 :

  HTDF1.x响应
  ```json
  {
      "block_meta": {
          "block_id": {
              "hash": "75FC52FD893C4BAF8DFABBA8DC566F4EB3A61A6BC3C02B75A031FC9050F63930",
              "parts": {
                  "total": "1",
                  "hash": "03DE6083B4B3EA5E4E3E6FDF0BB0290A2096BC1B0722225464DE18AE628793EB"
              }
          },
          "header": {
              "version": {
                  "block": "10",
                  "app": "0"
              },
              "chain_id": "testchain",
              "height": "1222934",
              "time": "2021-03-25T02:19:26.853951515Z",
              "num_txs": "1",
              "total_txs": "10085",
              "last_block_id": {
                  "hash": "1E79E68AECEC120EDD76B9F317C524C7926E8C174C27E05FD590A966AFEAAF6B",
                  "parts": {
                      "total": "1",
                      "hash": "8797FF6207259C345DCEB9F0205D46A607CD477FD1DAD12694892C106A94FFC7"
                  }
              },
              "last_commit_hash": "0B923330481D3D1049F39A891417EA5E631932FEF9F2BD5FECFD07FF87AC67CD",
              "data_hash": "9C3982BD296788E5E7C0742A792946C6648D7D93D561E7B91C88CB58814F792A",
              "validators_hash": "9D0CEB3248C9C6CF007081F964C7FD037C119BAE15957D638EC1D842969ABB7F",
              "next_validators_hash": "9D0CEB3248C9C6CF007081F964C7FD037C119BAE15957D638EC1D842969ABB7F",
              "consensus_hash": "4A3999F2F89E2E3AB1198825F730FCAE48D21406F7B9E36813A76B9A9B5ED509",
              "app_hash": "DC9AB6A17AE7190A98E1FD4D0184CFC47A98E79412E9731F76AA896D7329B37A",
              "last_results_hash": "",
              "evidence_hash": "",
              "proposer_address": "8C60F45F7FAA84A398AE90AE7C4D043943C65DDA"
          }
      },
      "block": {
          "txs": [
              {
                  "From": "htdf1t5hev9y9kyjnprkxaarzvyyrxwsd32cs99mwk2",
                  "To": "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2",
                  "Amount": [
                      {
                          "denom": "htdf",
                          "amount": "20"
                      }
                  ],
                  "Hash": "eeb7219fca92fc0816b3d28aa1a6a4717b07f886071ea97d51713485d1533f90",
                  "Memo": "",
                  "Data": "",
                  "TxClassify": 0,
                  "TypeName": "send"
              }
          ],
          "evidence": {
              "evidence": null
          },
          "last_commit": {
              "block_id": {
                  "hash": "1E79E68AECEC120EDD76B9F317C524C7926E8C174C27E05FD590A966AFEAAF6B",
                  "parts": {
                      "total": "1",
                      "hash": "8797FF6207259C345DCEB9F0205D46A607CD477FD1DAD12694892C106A94FFC7"
                  }
              },
              "precommits": [
                  {
                      "type": 2,
                      "height": "1222933",
                      "round": "0",
                      "block_id": {
                          "hash": "1E79E68AECEC120EDD76B9F317C524C7926E8C174C27E05FD590A966AFEAAF6B",
                          "parts": {
                              "total": "1",
                              "hash": "8797FF6207259C345DCEB9F0205D46A607CD477FD1DAD12694892C106A94FFC7"
                          }
                      },
                      "timestamp": "2021-03-25T02:19:26.850156636Z",
                      "validator_address": "8C60F45F7FAA84A398AE90AE7C4D043943C65DDA",
                      "validator_index": "0",
                      "signature": "dUeamxYPUhpeWiY5X4nQFXUX04AcbmxtPkWgAlv/qWndgUMFxDF0geh/jx8LNIJX8i98vhS7K4Qnd8VOVKQ7BQ=="
                  },
                  {
                      "type": 2,
                      "height": "1222933",
                      "round": "0",
                      "block_id": {
                          "hash": "1E79E68AECEC120EDD76B9F317C524C7926E8C174C27E05FD590A966AFEAAF6B",
                          "parts": {
                              "total": "1",
                              "hash": "8797FF6207259C345DCEB9F0205D46A607CD477FD1DAD12694892C106A94FFC7"
                          }
                      },
                      "timestamp": "2021-03-25T02:19:26.854725651Z",
                      "validator_address": "9410E67D719CCC9D73699CA5D7E1F0E938D15A03",
                      "validator_index": "1",
                      "signature": "oDRRoSHXVxY/H3PH83Ge3n75JSFkQDNV0oNjeA0OupzKpC/PC+EhmaxekPci75ktSmYbcvJTaZKEKCA4eTd2Bg=="
                  },
                  {
                      "type": 2,
                      "height": "1222933",
                      "round": "0",
                      "block_id": {
                          "hash": "1E79E68AECEC120EDD76B9F317C524C7926E8C174C27E05FD590A966AFEAAF6B",
                          "parts": {
                              "total": "1",
                              "hash": "8797FF6207259C345DCEB9F0205D46A607CD477FD1DAD12694892C106A94FFC7"
                          }
                      },
                      "timestamp": "2021-03-25T02:19:26.853951515Z",
                      "validator_address": "BA9BF4CBA3A4E9DCC3A33C64414C9B40C446934A",
                      "validator_index": "2",
                      "signature": "ZH5DrpOyfnhhkflz+F5PkA/A3n1nCK/rFHm6QTIdbLTf3BgGfMscplEfSXzQaDJSeKYJm8DezlubHbqgAzKYBw=="
                  }
              ]
          }
      },
      "time": "2021-03-25 10:19:26"
  }
  ```

   HTDF2.0响应:

   - 交易中增加了`Success`字段来表示交易状态, `true`:交易成功, `false`: 交易失败
   - 增加了`events` 用于委托和收益提取相关操作的记录

  ```json
    {
        "block_meta": {
            "block_id": {
                "hash": "4C6A106E47626BE209FB17076A86FAFC5BB268FC69D0CCABA6031439C1AAC505",
                "parts": {
                    "total": "1",
                    "hash": "41054DA02EFB3947C33279CF29488C67E1F783B9D865B7903EB47834C6323D29"
                }
            },
            "block_size": "731",
            "header": {
                "version": {
                    "block": "10",
                    "app": "0"
                },
                "chain_id": "testchain",
                "height": "12248",
                "time": "2021-07-08T03:47:58.646247583Z",
                "last_block_id": {
                    "hash": "B1B23D13E88598A5B4FEFBE9581F4FAF5E6B1D03C62D3FD40C151D1138A4620D",
                    "parts": {
                        "total": "1",
                        "hash": "31D76FF1A6326E44F3FCD362FBE20D9627E22CE73910313961D7A3F600747460"
                    }
                },
                "last_commit_hash": "825ADCE294060FF41B4AE49E3467097191200E236B9C6A71EEC547D9A46DCADD",
                "data_hash": "0E79E4127948137D94A4DB6D842245780B8A9CB24DC44F588C5FE8F398543501",
                "validators_hash": "B9523061DF5D36ADE95B20516F32DDBC060EFC36EA478B3A1235364EC79D9E20",
                "next_validators_hash": "B9523061DF5D36ADE95B20516F32DDBC060EFC36EA478B3A1235364EC79D9E20",
                "consensus_hash": "BB4E36307426C4131EECAACD63BAB5802FF378CAD3CA8FF913275147E8F2AE2E",
                "app_hash": "2EA7C07BBDFB409BC53E369F88D0776824EF8402C08D347EA304C54951252CCE",
                "last_results_hash": "",
                "evidence_hash": "",
                "proposer_address": "5C446BE6267A70E329B52C814097E10197B45BAA"
            },
            "num_txs": "1"
        },
        "block": {
            "txs": [
                {
                    "From": "htdf1heyvvns7fxyxcnu72du5267jxeglnvvlhhqqvs",
                    "To": "htdf1rgsfxav0af8a79cmtq6rnjtjkqngl9qcj8k9l7",
                    "Amount": [
                        {
                            "denom": "htdf",
                            "amount": "0.06"
                        }
                    ],
                    "Hash": "032d8b3f7bf1dd68d68948be4f3ca4e0cdfce2ce671d571750b218e97ffb8c35",
                    "Memo": "test",
                    "Data": "",
                    "TxClassify": 0,
                    "TypeName": "ethereum",
                    "Success": true
                }
            ],
            "evidence": {
                "evidence": null
            },
            "last_commit": {
                "height": "12247",
                "round": "0",
                "block_id": {
                    "hash": "B1B23D13E88598A5B4FEFBE9581F4FAF5E6B1D03C62D3FD40C151D1138A4620D",
                    "parts": {
                        "total": "1",
                        "hash": "31D76FF1A6326E44F3FCD362FBE20D9627E22CE73910313961D7A3F600747460"
                    }
                },
                "signatures": [
                    {
                        "block_id_flag": 2,
                        "validator_address": "5C446BE6267A70E329B52C814097E10197B45BAA",
                        "timestamp": "2021-07-08T03:47:58.646247583Z",
                        "signature": "O3Q2duGl43ilbdz0a366Iu9Z0qVLoyJin6aMjii+hRjyuEQ17cFNu58Ii+qT7LRwIOvTteeUHs7LJoG1EsV1Cw=="
                    }
                ]
            }
        },
        "events": [],
        "time": "2021-07-08 11:47:58"
    }
  ```


---

##  普通转账或创建并发布智能合约

 :warning: *警告: 仅限于Debug 版`hscli`, 用于热钱包(简单方便但不安全, 不推荐使用) , 推荐自行参考`demo`使用自主签名交易并广播*

- HTTP请求方法：    `POST`

- endpoint:     `/hs/send`
- 标注：   仅限于Debug 版`hscli`;  智能合约后续版本实现；
- 说明：
  - `from`: 发送方地址，
  - `memo`：转账备注，
  - `password` ：密码
  - `chain_id`：链类型:`mainchain`: 主网 ;  `testchain` : 测试网
  - `account_number`：账户编号, 可通过 [查询账户信息接口](#查询账户信息)获取
  - `sequence`：序列号(账户的交易序号) , 可通过 [查询账户信息接口](#查询账户信息)获取
  - `amount`：金额
  - `to`：普通HTDF转账`to`为接收方地址; 若为创建智能合约`to`为空; 若为调用智能合约`to`为智能合约地址
  - `data`: 普通HTDF转账此字段填`""`串即可;
  - `gas_price`: 普通交易和智能合约, 此字段填`"100"`即可;
  - `gas_wanted`: 普通HTDF转账此字段填`"30000"`即可; 智能合约交易根据实际情况而定.

> 提示: 如此`/hs/send`接口遇到问题可以参考:  [send_questions.md](./send_questions.md)



**例1: 普通 HTDF 转账**

  - 请求

    ```shell
    curl  'http://localhost:1317/hs/send' -X POST \
    -H 'Content-Type: application/json' \
    -d '{ "base_req":
        {
            "from": "htdf13fe966ffsqc3evc853ej2s8gprpzqv4yfsnfuy",
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

> 提示: 如此`/hs/send`接口遇到问题可以参考:  [send_questions.md](./send_questions.md)



**例子 2 创建智能合约**

注意事项:
  1. 创建智能合约时`to`设置为空; 调用智能合约时`to`填写智能合约的地址

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
                  "data": "6060604052341561000f57600080fd====> 此处省略几千字符 <=====f50f83c70ed26b99d890029"
              }'
  ```


- 响应 :
  ```json
  {
    "height": "0",
    "txhash": "cce1f3234a816a7e023c3730a48a53d78da381a59faa82c5af488bd74d04e44c"
  }
  ```

- 如果时新建智能合约, 获取刚刚创建的智能合约的地址:

    ```
    curl localhost:1317/txs/cce1f3234a816a7e023c3730a48a53d78da381a59faa82c5af488bd74d04e44c

    {
        "height": "2234819",
        "txhash": "CCE1F3234A816A7E023C3730A48A53D78DA381A59FAA82C5AF488BD74D04E44C",
        "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"{\\\"code\\\":0,\\\"message\\\":\\\"ok\\\",\\\"contract_address\\\":\\\"htdf18xq0p7h8rwxykfyrfytylhmrtusmzfwst4s6jx\\\",\\\"evm_output\\\":\\\"\\\"}\"}]",
        "logs": [
            {
                "msg_index": "0",
                "success": true,
                "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"htdf18xq0p7h8rwxykfyrfytylhmrtusmzfwst4s6jx\",\"evm_output\":\"\"}"
            }
        ],

        // 其他字段 略
    }
    ```
    其中`logs`字段中的 `"contract_address"`就是新建的合约地址


**例子 3 调用智能合约**

- 见[HRC20.md](./HRC20.md)


> 提示: 如此`/hs/send`接口遇到问题可以参考:  [send_questions.md](./send_questions.md)



---


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


- 2.创建智能合约交易的响应

    ```json
    {
        "height": "2234819",
        "txhash": "CCE1F3234A816A7E023C3730A48A53D78DA381A59FAA82C5AF488BD74D04E44C",
        "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"{\\\"code\\\":0,\\\"message\\\":\\\"ok\\\",\\\"contract_address\\\":\\\"htdf18xq0p7h8rwxykfyrfytylhmrtusmzfwst4s6jx\\\",\\\"evm_output\\\":\\\"\\\"}\"}]",
        "logs": [
            {
                "msg_index": "0",
                "success": true,
                "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"htdf18xq0p7h8rwxykfyrfytylhmrtusmzfwst4s6jx\",\"evm_output\":\"\"}"
            }
        ],
        "gas_wanted": "12345678",
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
                            "From": "htdf1uc5t3x8a7gxhnard7mwu6n4sx5jfh54znzxj8x",
                            "To": "",
                            "Amount": [
                                {
                                    "denom": "satoshi",
                                    "amount": "0"
                                }
                            ],
                            "Data": "606060405260006002600====> 此处省略几千个字符<===0929e4f365d480e83063f02298b7dd20029",
                            "GasPrice": "100",
                            "GasWanted": "1200000"
                        }
                    }
                ],
                "fee": {
                    "gas_wanted": "12345678",
                    "gas_price": "100"
                },
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "AonkvtOFmofOjpRThnmaTLr2MqYW5zEbTLqN5nUwy+xe"
                        },
                        "signature": "VvuEMuzEuLxO38IxHaihHjBdjEmzA7l/pbSBsdyWyyhTkXPxcI4GrwttDHMYljU9UU0MKF8ZliF/63Rw1wFLcg=="
                    }
                ],
                "memo": ""
            }
        },
        "timestamp": "2020-08-03T16:01:16Z"
    }
    ```

- 3.调用智能合约
这里以HRC20的转账为例:

    ```json
        {
            "height": "4780206",
            "txhash": "09197DE04389777C02B683894F3294D624A1F9BD64EEAC5C8A890B83F1F14A30",
            "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"{\\\"code\\\":0,\\\"message\\\":\\\"ok\\\",\\\"contract_address\\\":\\\"\\\",\\\"evm_output\\\":\\\"0000000000000000000000000000000000000000000000000000000000000001\\\"}\"}]",
            "logs": [
                {
                    "msg_index": "0",
                    "success": true,
                    "log": "{\"code\":0,\"message\":\"ok\",\"contract_address\":\"\",\"evm_output\":\"0000000000000000000000000000000000000000000000000000000000000001\"}"
                }
            ],
            "gas_wanted": "200000",
            "gas_used": "90604",
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
                                "From": "htdf1eq9n000kaveckx00g72usncm5qtdu8fgtq0uja",
                                "To": "htdf18xq0p7h8rwxykfyrfytylhmrtusmzfwst4s6jx",
                                "Amount": [
                                    {
                                        "denom": "satoshi",
                                        "amount": "0"
                                    }
                                ],
                                "Data": "a9059cbb0000000000000000000000003f63d0a97bdeb940dfed3a40d7d0ca67d742e35c0000000000000000000000000000000000000000000004b828c5a63d76000000",
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
                                "value": "AvxW40vNFF/4rfBj53httsdEl9FeBJPZX3QnSS0PdsZ2"
                            },
                            "signature": "IFEIVunhDdp8KoFSUYpTBpHOnUygy3hTSMEjU88lx/VqqgxvLFT6Kefni2uqTrYtHa13EJrKgJpG2udVMTTqhw=="
                        }
                    ],
                    "memo": "糖果"
                }
            },
            "timestamp": "2021-01-19T07:54:03Z"
        }
    ```

- 4.扩展型交易的响应

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


   HTDF2.0 响应:
   - `raw_log` 和 `logs`中`log`字段改成了字符串
   - 删除了原来的`tags`字段, 改成了`events`字段
   - 增加了`data`字段

    ```json
    {
        "height": "635",
        "txhash": "63EEE084F9E8D39EBDE5668018508DB6555C7C732CA6CAC5D072372AC60E1C39",
        "data": "BD020A140000000000000000000000000000000000000000128002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002001322063EEE084F9E8D39EBDE5668018508DB6555C7C732CA6CAC5D072372AC60E1C39",
        "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"executed EVM state transition; sender address: 0xBE48c64e1e49886C4f9E5379456BD23651F9B19f, recipient address: 0x338300688033c44d2064413D08969956Aba7211c, output: \"}]",
        "logs": [
            {
                "msg_index": "0",
                "success": true,
                "log": "executed EVM state transition; sender address: 0xBE48c64e1e49886C4f9E5379456BD23651F9B19f, recipient address: 0x338300688033c44d2064413D08969956Aba7211c, output: "
            }
        ],
        "gas_wanted": "200000",
        "gas_used": "30000",
        "events": [
            {
                "type": "ethereum",
                "attributes": [
                    {
                        "key": "amount",
                        "value": "1000000000000000satoshi"
                    },
                    {
                        "key": "recipient",
                        "value": "0x338300688033c44d2064413D08969956Aba7211c"
                    }
                ]
            },
            {
                "type": "message",
                "attributes": [
                    {
                        "key": "action",
                        "value": "ethereum"
                    },
                    {
                        "key": "module",
                        "value": "evm"
                    },
                    {
                        "key": "sender",
                        "value": "0xBE48c64e1e49886C4f9E5379456BD23651F9B19f"
                    }
                ]
            }
        ],
        "tx": {
            "type": "auth/StdTx",
            "value": {
                "msg": [
                    {
                        "type": "htdfservice/send",
                        "value": {
                            "From": "htdf1heyvvns7fxyxcnu72du5267jxeglnvvlhhqqvs",
                            "To": "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml",
                            "Amount": [
                                {
                                    "denom": "satoshi",
                                    "amount": "1000000000000000"
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
                            "value": "AhJmmPJp8URY6ZAvEWWN+HxC0eG+p9a0Ja9A9ONEwDCz"
                        },
                        "signature": "cux0QH7ZxNOwgZoC/bfOdWqsVUVcxQgGKi9DPPetkzRSw+Oe3P2yyHkDYNH80ig1NtNKz4fyRdMigpVRPHC2XA=="
                    }
                ],
                "memo": ""
            }
        },
        "timestamp": "2021-07-07T10:56:51Z"
    }
    ```

    HTDF2.0创建智能合约交易详情:

    - 同上
    - 新创建的合约地址在logs中 ` "log": "executed EVM state transition; sender address: 0x338300688033c44d2064413D08969956Aba7211c, contract address: 0x2B8b609ECf499291fB3e8de6289291dbA76BCDeB, `
    - 也可以通过 `/v2/tx-receipt/{txhash}` 接口查询交易收据, 获取新创建的合约地址

    - 可以通过以下命令将hex-string地址转为htdf地址
        ```
        $ hscli bech32 h2b 2B8b609ECf499291fB3e8de6289291dbA76BCDeB
        hexAddr=2B8b609ECf499291fB3e8de6289291dbA76BCDeB|accountPrefixAddress=htdf19w9kp8k0fxffr7e73hnz3y53mwnkhn0tkh2cpz
        ```

    ```json
        {
        "height": "11332",
        "txhash": "E1DCB311F5FC1A1164DD149DFD454014C4AA66D68BF47E46476F3CD072301F7C",
        "data": "D20E0A142B8B609ECF499291FB3E8DE6289291DBA76BCDEB1280020000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020012A920C608060405234801561001057600080FD5B50600436106100835760003560E01C806306FDDE0314610088578063095EA7B3146100A657806318160DDD146100C657806323B872DD146100DB578063313CE567146100EE57806370A082311461010357806395D89B4114610116578063A9059CBB1461011E578063DD62ED3E14610131575B600080FD5B610090610144565B60405161009D919061052D565B60405180910390F35B6100B96100B43660046104F9565B610169565B60405161009D9190610522565B6100CE61020D565B60405161009D9190610580565B6100B96100E93660046104BE565B610213565B6100F661033A565B60405161009D9190610589565B6100CE61011136600461046B565B61033F565B61009061035E565B6100B961012C3660046104F9565B61037D565B6100CE61013F36600461048C565B610429565B6040518060400160405280600981526020016820A5219031B430B4B760B91B81525081565B600081158061019957503360009081526001602090815260408083206001600160A01B0387168452909152902054155B6101A257600080FD5B3360008181526001602090815260408083206001600160A01B03881680855292529182902085905590519091907F8C5BE1E5EBEC7D5BD14F71427D1E84F3DD0314C0F7B2291E5B200AC8C7C3B925906101FC908690610580565B60405180910390A350600192915050565B60025481565B6001600160A01B0380841660009081526001602090815260408083203384529091528120549091841661024557600080FD5B8083111561025257600080FD5B6001600160A01B0385166000908152602081905260409020546102769084906105AF565B6001600160A01B0380871660009081526020819052604080822093909355908616815220546102A6908490610597565B6001600160A01B0385166000908152602081905260409020556102C983826105AF565B6001600160A01B0380871660008181526001602090815260408083203384529091529081902093909355915190861691907FDDF252AD1BE2C89B69C2B068FC378DAA952BA7F163C4A11628F55A4DF523B3EF90610327908790610580565B60405180910390A3506001949350505050565B601281565B6001600160A01B0381166000908152602081905260409020545B919050565B60405180604001604052806003815260200162414A4360E81B81525081565B60006001600160A01B03831661039257600080FD5B336000908152602081905260409020546103AD9083906105AF565B33600090815260208190526040808220929092556001600160A01B038516815220546103DA908390610597565B6001600160A01B0384166000818152602081905260409081902092909255905133907FDDF252AD1BE2C89B69C2B068FC378DAA952BA7F163C4A11628F55A4DF523B3EF906101FC908690610580565B6001600160A01B03918216600090815260016020908152604080832093909416825291909152205490565B80356001600160A01B038116811461035957600080FD5B60006020828403121561047C578081FD5B61048582610454565B9392505050565B6000806040838503121561049E578081FD5B6104A783610454565B91506104B560208401610454565B90509250929050565B6000806000606084860312156104D2578081FD5B6104DB84610454565B92506104E960208501610454565B9150604084013590509250925092565B6000806040838503121561050B578182FD5B61051483610454565B946020939093013593505050565B901515815260200190565B6000602080835283518082850152825B818110156105595785810183015185820160400152820161053D565B8181111561056A5783604083870101525B50601F01601F1916929092016040019392505050565B90815260200190565B60FF91909116815260200190565B600082198211156105AA576105AA6105C6565B500190565B6000828210156105C1576105C16105C6565B500390565B634E487B7160E01B600052601160045260246000FDFEA2646970667358221220BE548E6823759B97209CB1C708173812496C1ABFC64F372FFAEB4BC7CDB2BC1364736F6C634300080000333220E1DCB311F5FC1A1164DD149DFD454014C4AA66D68BF47E46476F3CD072301F7C",
        "raw_log": "[{\"msg_index\":\"0\",\"success\":true,\"log\":\"executed EVM state transition; sender address: 0x338300688033c44d2064413D08969956Aba7211c, contract address: 0x2B8b609ECf499291fB3e8de6289291dbA76BCDeB, output: 608060405234801561001057600080fd5b50600436106100835760003560e01c806306fdde0314610088578063095ea7b3146100a657806318160ddd146100c657806323b872dd146100db578063313ce567146100ee57806370a082311461010357806395d89b4114610116578063a9059cbb1461011e578063dd62ed3e14610131575b600080fd5b610090610144565b60405161009d919061052d565b60405180910390f35b6100b96100b43660046104f9565b610169565b60405161009d9190610522565b6100ce61020d565b60405161009d9190610580565b6100b96100e93660046104be565b610213565b6100f661033a565b60405161009d9190610589565b6100ce61011136600461046b565b61033f565b61009061035e565b6100b961012c3660046104f9565b61037d565b6100ce61013f36600461048c565b610429565b6040518060400160405280600981526020016820a5219031b430b4b760b91b81525081565b600081158061019957503360009081526001602090815260408083206001600160a01b0387168452909152902054155b6101a257600080fd5b3360008181526001602090815260408083206001600160a01b03881680855292529182902085905590519091907f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925906101fc908690610580565b60405180910390a350600192915050565b60025481565b6001600160a01b0380841660009081526001602090815260408083203384529091528120549091841661024557600080fd5b8083111561025257600080fd5b6001600160a01b0385166000908152602081905260409020546102769084906105af565b6001600160a01b0380871660009081526020819052604080822093909355908616815220546102a6908490610597565b6001600160a01b0385166000908152602081905260409020556102c983826105af565b6001600160a01b0380871660008181526001602090815260408083203384529091529081902093909355915190861691907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef90610327908790610580565b60405180910390a3506001949350505050565b601281565b6001600160a01b0381166000908152602081905260409020545b919050565b60405180604001604052806003815260200162414a4360e81b81525081565b60006001600160a01b03831661039257600080fd5b336000908152602081905260409020546103ad9083906105af565b33600090815260208190526040808220929092556001600160a01b038516815220546103da908390610597565b6001600160a01b0384166000818152602081905260409081902092909255905133907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef906101fc908690610580565b6001600160a01b03918216600090815260016020908152604080832093909416825291909152205490565b80356001600160a01b038116811461035957600080fd5b60006020828403121561047c578081fd5b61048582610454565b9392505050565b6000806040838503121561049e578081fd5b6104a783610454565b91506104b560208401610454565b90509250929050565b6000806000606084860312156104d2578081fd5b6104db84610454565b92506104e960208501610454565b9150604084013590509250925092565b6000806040838503121561050b578182fd5b61051483610454565b946020939093013593505050565b901515815260200190565b6000602080835283518082850152825b818110156105595785810183015185820160400152820161053d565b8181111561056a5783604083870101525b50601f01601f1916929092016040019392505050565b90815260200190565b60ff91909116815260200190565b600082198211156105aa576105aa6105c6565b500190565b6000828210156105c1576105c16105c6565b500390565b634e487b7160e01b600052601160045260246000fdfea2646970667358221220be548e6823759b97209cb1c708173812496c1abfc64f372ffaeb4bc7cdb2bc1364736f6c63430008000033\"}]",
        "logs": [
            {
                "msg_index": "0",
                "success": true,
                "log": "executed EVM state transition; sender address: 0x338300688033c44d2064413D08969956Aba7211c, contract address: 0x2B8b609ECf499291fB3e8de6289291dbA76BCDeB, output: 608060405234801561001057600080fd5b50600436106100835760003560e01c806306fdde0314610088578063095ea7b3146100a657806318160ddd146100c657806323b872dd146100db578063313ce567146100ee57806370a082311461010357806395d89b4114610116578063a9059cbb1461011e578063dd62ed3e14610131575b600080fd5b610090610144565b60405161009d919061052d565b60405180910390f35b6100b96100b43660046104f9565b610169565b60405161009d9190610522565b6100ce61020d565b60405161009d9190610580565b6100b96100e93660046104be565b610213565b6100f661033a565b60405161009d9190610589565b6100ce61011136600461046b565b61033f565b61009061035e565b6100b961012c3660046104f9565b61037d565b6100ce61013f36600461048c565b610429565b6040518060400160405280600981526020016820a5219031b430b4b760b91b81525081565b600081158061019957503360009081526001602090815260408083206001600160a01b0387168452909152902054155b6101a257600080fd5b3360008181526001602090815260408083206001600160a01b03881680855292529182902085905590519091907f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925906101fc908690610580565b60405180910390a350600192915050565b60025481565b6001600160a01b0380841660009081526001602090815260408083203384529091528120549091841661024557600080fd5b8083111561025257600080fd5b6001600160a01b0385166000908152602081905260409020546102769084906105af565b6001600160a01b0380871660009081526020819052604080822093909355908616815220546102a6908490610597565b6001600160a01b0385166000908152602081905260409020556102c983826105af565b6001600160a01b0380871660008181526001602090815260408083203384529091529081902093909355915190861691907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef90610327908790610580565b60405180910390a3506001949350505050565b601281565b6001600160a01b0381166000908152602081905260409020545b919050565b60405180604001604052806003815260200162414a4360e81b81525081565b60006001600160a01b03831661039257600080fd5b336000908152602081905260409020546103ad9083906105af565b33600090815260208190526040808220929092556001600160a01b038516815220546103da908390610597565b6001600160a01b0384166000818152602081905260409081902092909255905133907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef906101fc908690610580565b6001600160a01b03918216600090815260016020908152604080832093909416825291909152205490565b80356001600160a01b038116811461035957600080fd5b60006020828403121561047c578081fd5b61048582610454565b9392505050565b6000806040838503121561049e578081fd5b6104a783610454565b91506104b560208401610454565b90509250929050565b6000806000606084860312156104d2578081fd5b6104db84610454565b92506104e960208501610454565b9150604084013590509250925092565b6000806040838503121561050b578182fd5b61051483610454565b946020939093013593505050565b901515815260200190565b6000602080835283518082850152825b818110156105595785810183015185820160400152820161053d565b8181111561056a5783604083870101525b50601f01601f1916929092016040019392505050565b90815260200190565b60ff91909116815260200190565b600082198211156105aa576105aa6105c6565b500190565b6000828210156105c1576105c16105c6565b500390565b634e487b7160e01b600052601160045260246000fdfea2646970667358221220be548e6823759b97209cb1c708173812496c1abfc64f372ffaeb4bc7cdb2bc1364736f6c63430008000033"
            }
        ],
        "gas_wanted": "2000000",
        "gas_used": "436347",
        "events": [
            {
                "type": "ethereum",
                "attributes": [
                    {
                        "key": "amount",
                        "value": "0satoshi"
                    }
                ]
            },
            {
                "type": "message",
                "attributes": [
                    {
                        "key": "action",
                        "value": "ethereum"
                    },
                    {
                        "key": "module",
                        "value": "evm"
                    },
                    {
                        "key": "sender",
                        "value": "0x338300688033c44d2064413D08969956Aba7211c"
                    }
                ]
            }
        ],
        "tx": {
            "type": "auth/StdTx",
            "value": {
                "msg": [
                    {
                        "type": "htdfservice/send",
                        "value": {
                            "From": "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml",
                            "To": "",
                            "Amount": [
                                {
                                    "denom": "satoshi",
                                    "amount": "0"
                                }
                            ],
                            "Data": "608060405234801561001057600080fd5b506aa49be39dc14cb827000000600281905533600090815260208190526040902055610612806100416000396000f3fe608060405234801561001057600080fd5b50600436106100835760003560e01c806306fdde0314610088578063095ea7b3146100a657806318160ddd146100c657806323b872dd146100db578063313ce567146100ee57806370a082311461010357806395d89b4114610116578063a9059cbb1461011e578063dd62ed3e14610131575b600080fd5b610090610144565b60405161009d919061052d565b60405180910390f35b6100b96100b43660046104f9565b610169565b60405161009d9190610522565b6100ce61020d565b60405161009d9190610580565b6100b96100e93660046104be565b610213565b6100f661033a565b60405161009d9190610589565b6100ce61011136600461046b565b61033f565b61009061035e565b6100b961012c3660046104f9565b61037d565b6100ce61013f36600461048c565b610429565b6040518060400160405280600981526020016820a5219031b430b4b760b91b81525081565b600081158061019957503360009081526001602090815260408083206001600160a01b0387168452909152902054155b6101a257600080fd5b3360008181526001602090815260408083206001600160a01b03881680855292529182902085905590519091907f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925906101fc908690610580565b60405180910390a350600192915050565b60025481565b6001600160a01b0380841660009081526001602090815260408083203384529091528120549091841661024557600080fd5b8083111561025257600080fd5b6001600160a01b0385166000908152602081905260409020546102769084906105af565b6001600160a01b0380871660009081526020819052604080822093909355908616815220546102a6908490610597565b6001600160a01b0385166000908152602081905260409020556102c983826105af565b6001600160a01b0380871660008181526001602090815260408083203384529091529081902093909355915190861691907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef90610327908790610580565b60405180910390a3506001949350505050565b601281565b6001600160a01b0381166000908152602081905260409020545b919050565b60405180604001604052806003815260200162414a4360e81b81525081565b60006001600160a01b03831661039257600080fd5b336000908152602081905260409020546103ad9083906105af565b33600090815260208190526040808220929092556001600160a01b038516815220546103da908390610597565b6001600160a01b0384166000818152602081905260409081902092909255905133907fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef906101fc908690610580565b6001600160a01b03918216600090815260016020908152604080832093909416825291909152205490565b80356001600160a01b038116811461035957600080fd5b60006020828403121561047c578081fd5b61048582610454565b9392505050565b6000806040838503121561049e578081fd5b6104a783610454565b91506104b560208401610454565b90509250929050565b6000806000606084860312156104d2578081fd5b6104db84610454565b92506104e960208501610454565b9150604084013590509250925092565b6000806040838503121561050b578182fd5b61051483610454565b946020939093013593505050565b901515815260200190565b6000602080835283518082850152825b818110156105595785810183015185820160400152820161053d565b8181111561056a5783604083870101525b50601f01601f1916929092016040019392505050565b90815260200190565b60ff91909116815260200190565b600082198211156105aa576105aa6105c6565b500190565b6000828210156105c1576105c16105c6565b500390565b634e487b7160e01b600052601160045260246000fdfea2646970667358221220be548e6823759b97209cb1c708173812496c1abfc64f372ffaeb4bc7cdb2bc1364736f6c63430008000033",
                            "GasPrice": "100",
                            "GasWanted": "2000000"
                        }
                    }
                ],
                "fee": {
                    "gas_wanted": "2000000",
                    "gas_price": "100"
                },
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "A/tHuncwUGZHLxKdevqasdS+DCcW8fbt0Sv8sRpezqWy"
                        },
                        "signature": "ErTJ3uiZOM8WnsmhhHrYpFXAznuSDtu3wtrS0OuvBbV/38I0NBryOO0pV4OmwDobojjQFv8tiaSIFoUzH9nOdg=="
                    }
                ],
                "memo": "test_create_hrc20_token_contract"
            }
        },
        "timestamp": "2021-07-08T01:53:39Z"
    }
    ```


---

## 查询交易收据

> 提示: HTDF2.0新增的API, 请使用v2.0.0及以上版本的 hscli

- HTTP请求方法： `GET`

- endpoint:    `/v2/tx-receipt/{txhash}`

- 说明: 获取交易收据(包括了智能合约执行的Logs), 此API功能类似以太坊的`getTransactionReceipt`

- 关联接口:
  - 查询交易详情: `/txs/{hash}`

- 请求:

  ```shell
  curl http://localhost:1317/v2/tx-receipt/F6902DE7EE034D1535031BE2C11DF7DC5DC29DA23B3270293F379AF817BD3472

  ```

- 响应 :

   ```json

    {
        "height": "13615",
        "txhash": "F6902DE7EE034D1535031BE2C11DF7DC5DC29DA23B3270293F379AF817BD3472",
        "results": {
            "contractAddress": "",
            "logsBloom": "0x00000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000840000000000000000000000008000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000800000000000000000000000000400000000000040000000000000000002000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000200000000000000000000000040000000",
            "logs": [
                {
                    "address": "0x7b539be07a96762ffc1b01b956192dbf72996817",
                    "topics": [
                        "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
                        "0x000000000000000000000000338300688033c44d2064413d08969956aba7211c",
                        "0x00000000000000000000000048e09fccb999150a290c0d336dee2118503dd30f"
                    ],
                    "data": "0x00000000000000000000000000000000000000000000021e27c1806e59a40000",
                    "blockNumber": "0x352f",
                    "transactionHash": "0xf6902de7ee034d1535031be2c11df7dc5dc29da23b3270293f379af817bd3472",
                    "transactionIndex": "0x1",
                    "blockHash": "0x0000000000000000000000000000000000000000000000000000000000000000",
                    "logIndex": "0x0",
                    "removed": false
                }
            ],
            "status": "0x1",
            "transactionHash": "0xf6902de7ee034d1535031be2c11df7dc5dc29da23b3270293f379af817bd3472"
        },
        "gas_wanted": "200000",
        "gas_used": "60170",
        "timestamp": "2021-05-18T02:12:32Z",
        "from": "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml",
        "to": "htdf10dfehcr6jemzllqmqxu4vxfdhaefj6qhqux4q8"
    }
   ```


---

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


---

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

---

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

---

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


---

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

---

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

---

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
