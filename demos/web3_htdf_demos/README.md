> 提示: 为了让你的Ðapp运行上HTDF,它通过RPC调用与节点通信。web3_htdf.js可以与任何暴露了RPC接口的HTDF节点连接。
> 

## 添加web3_htdf

### 首先你需要将htdf_web3引入，通过如下步骤：

```
$npm install htdf_web3

```

### 然后你需要创建htdf_web3的实例，通过如下步骤：

```
let HTDF = require("htdf_web3");

let htdf = new HTDF("http://htdf2020-test01.orientwalt.cn:1317","testchain");

let Web3 = require('htdf_web3/web3')

let web3 = new Web3()

成功引入后，你现在可以使用htdf_web3的相关[API](../demos/web3_htdf_demos/test.js)了。

```

### 使用Solidity编译.sol文件

```
$npm install solc

$solcjs --help

$solcjs --bin ./demo.sol

```

