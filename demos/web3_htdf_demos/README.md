> 提示: 为了让你的Dapp运行上HTDF,它通过RPC调用与节点通信。web3_htdf.js可以与任何暴露了RPC接口的HTDF节点连接。
> 

## 安装htdf_web3

首先你需要将htdf_web3引入，通过如下步骤：

```
$npm install htdf_web3
```

然后你需要创建htdf_web3的实例进行测试，通过如下步骤：

```js
let HTDF = require("htdf_web3");

let htdf = new HTDF("http://htdf2020-test01.orientwalt.cn:1317","testchain");

let Web3 = require('htdf_web3/web3')

let web3 = new Web3()

```

成功引入后，你现在可以使用htdf_web3了. 

- 普通HTDF转账交易[htdf.js](./htdf.js)
- 智能合约相关, 这里以HRC20合约为例[hrc20.js](./hrc20.js)

