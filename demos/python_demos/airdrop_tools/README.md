
# airdrop_tools

基于[htdfsdk](https://github.com/youngqqcn/htdfsdk)的HRC20代币批量转账工具, 关于htdfsdk更多内容可以访问: https://github.com/youngqqcn/htdfsdk


## 运行环境

Python3.6及以上

## 配置步骤

- 安装依赖

    ```
    pip install -r requirements.txt
    ```

- 修改配置
    - 在`hrc20_tokens.py`添加对应的HRC20代币信息
    - 修改 `airdrop.py`中的`COMMON`中响应的配置以及`TOKEN_CFG`
    - 根据需要修改`address.txt` 地址信息
    - 根据你的需求修改其他内容

- 运行

    ```
    python3 airdrop.py
    ```

