# 更新 `~/.hsd/config/config.toml` 步骤:


0. 根据自身的实际情况, 自行决定是否需要停掉其他服务
1. 停掉hsd
2. 下载最新的config.toml ,  用浏览器下载: https://gitee.com/orientwalt/docs/blob/master/mainnet/config/config.toml (如果使用wget下载, 使用命令:  wget  https://gitee.com/orientwalt/docs/raw/master/mainnet/config/config.toml )
3. 使用最新的config.toml 替换 ~/.hsd/config/config.toml
4. 删掉旧的地址文件(备份)地址文件:   mv ~/.hsd/config/addrbook.json   ~/.hsd/config/addrbook.json.bak
5. 重新启动hsd,  观察日志, 是否能正常同步区块,  
6. 完成