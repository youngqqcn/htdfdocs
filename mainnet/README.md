# 搭建本地观察节点

## 环境要求


操作系统
  - Ubuntu 16.04/18.04/20.04 amd64  (**推荐**)
  - Centos 7  amd64
  - Window 7 / Window 10 x64
  - MacOS amd64

|内容|建议配置|最低配置|
|:-----:|:-----:|:-----:|
|CPU|4核|2核|
|内存|8G|4G|
|磁盘|60G(可扩展)|60G(可扩展)|
|带宽|2~4M| 1M |


## 获取最新应用和配置


### 下载(使用浏览器进行下载)
- 下载相应操作系统(Linux/Windows/MacOS)的可执行文件: 
https://gitee.com/orientwalt/htdf/releases/v1.3.1

- 下载配置文件和创世文件:
 https://gitee.com/orientwalt/docs/attach_files/593332/download/config.tar.gz


## 解压和部署
这里以linux部署为例

```

tar xzf hsd_hscli_linux_amd64-v1.3.1.tar.gz

cd hsd_hscli_linux_amd64-v1.3.1

cp hsd /usr/local/bin/

# 如果不需要热钱包则使用Release版本(推荐)
cp hscli_Release/hscli /usr/local/bin/

# 如果需要使用热钱包使用Debug版本
cp hscli_Debug/hscli /usr/local/bin/


mkdir -p ~/.hsd/config

tar xzf config.tar.gz

cp config/genesis.json .hsd/config

cp config/config.toml  .hsd/config
```

> 问: hscli的Relase和Debug有什么不同?
> 
> 答: Debug版`hscli`包含 `/accounts/newaccount`,`send`, `create`和`sign`等几个热钱包RPC接口, 而Release版`hscli`则不包含这些接口.


## 启动hsd

```
$nohup hsd start >> ${HOME}/.hsd/app.log  2>&1  &

```

hsd日志的观察；能成功出块，说明hsd部署成功

```
$tail -f ~/.hsd/app.log


I[2019-09-11|14:37:29.882] Executed block                               module=state height=867122 validTxs=0 invalidTxs=0
I[2019-09-11|14:37:29.887] Committed state                              module=state height=867122 txs=0 appHash=0502F8AC65336C3D359EC29CF49D903E542AFD46CCA8759351A6A356433F98FB


I[2019-09-11|14:37:35.271] Executed block                               module=state height=867123 validTxs=0 invalidTxs=0
I[2019-09-11|14:37:35.276] Committed state                              module=state height=867123 txs=0 appHash=2647D3A0A1CE1B924C24CE158AD2B44EFB6CC40516FAFD9164AF8911BDC230D9

```

## 启动 REST 服务(RPC 接口)

启动REST服务，其中`chain-id`，主网(生产环境)是`mainchain`

```
$nohup hscli rest-server --chain-id=mainchain --trust-node=true  --laddr=tcp://0.0.0.0:1317  >> ${HOME}/.hsd/restServer.log  2>&1  &
```

REST 服务的观察
```
$tail -f ~/.hsd/restServer.log
```


## 尝试访问 REST 服务(RPC接口)

```
$curl http://localhost:1317/blocks/latest

```

