/**
date: 2020/9/25
author : yqq
descriptions: rewrite  HTDF transaction demo in golang
reference : https://gitee.com/orientwalt/apidoc_2020/blob/master/demo/python/htdf_transfer.py
core signature algorithm : secp256k1   (same as bitcoin , ethereum )
*/

package main

import (
	"bytes"
	"crypto/sha256"
	"encoding/base64"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"math"
	"net/http"
	"strconv"
	"strings"
	"time"

	secp256k1 "github.com/btcsuite/btcd/btcec"
)

// this is a testchain  address that has 100HTDF for test, you can run this demo directly , 2020-09-25
var gChainId = "testchain" // mainchain or testchain
var gFromAddress = "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml"
var gPrivateKey = "279bdcd8dccec91f9e079894da33d6888c0f9ef466c0b200921a1bf1ea7d86e8"
var gNodeIpPort = "htdf2020-test01.orientwalt.cn:1317"
var gGasLimit = 30000
var gGasPrice = 100

// mainchain parameters
//var gChainId = "mainchain"                                     //TODO:  mainchain or testchain
//var gFromAddress = "YOUR_HTDF_ADDRESS"                         //TODO
//var gPrivateKey = "YOUR_HTDF_ADDRESS_PRIVATE_KEY"              //TODO
//var gNodeIpPort = "YOUR_HTDF_FULL_NODE_IP_ADDRESS:1317" 		 //TODO
//var gGasLimit = 30000
//var gGasPrice =  100

var gUnsignedStr = `{
	"account_number": "%d",
	"chain_id": "%s",
	"fee": {
		"gas_price": "%d",
		"gas_wanted": "%d"
	},
	"memo": "%s",
	"msgs": [{
		"Amount": [{
		"amount": "%d",
		"denom": "satoshi"
	}],
		"Data": "",
		"From": "%s",
		"GasPrice": %d,
		"GasWanted": %d,
		"To": "%s"
	}],
	"sequence": "%d"
	}`

var gBroadcastStr = `{
	"type": "auth/StdTx",
		"value":{
		"msg": [{
			"type": "htdfservice/send",
			"value":{
				"From": "%s",
				"To": "%s",
				"Amount": [{
					"denom": "satoshi",
					"amount": "%d"
				}],
				"Data": "",
				"GasPrice": "%d",
				"GasWanted": "%d"
			}
		}],
	"fee": {
		"gas_wanted": "%d",
		"gas_price": "%d"
	},
	"signatures": [{
		"pub_key": {
		"type": "tendermint/PubKeySecp256k1",
		"value": "%s"
	},
	"signature": "%s"
	}],
	"memo": "%s"
	}
	}`

func HtdfToSatoshi(amount float64) int64 {
	// 1 HTDF  = 10^8 satoshi
	return int64(amount * math.Pow(10, 8))
}

func Sign(privateKey []byte, msg []byte) (string, error) {
	pk, _ := secp256k1.PrivKeyFromBytes(secp256k1.S256(), privateKey[:])

	sig, err := pk.Sign(msg)
	if err != nil {
		return "", err
	}
	sigRS := hex.EncodeToString(sig.R.Bytes()) + hex.EncodeToString(sig.S.Bytes())
	fmt.Printf("rs: %s\n", sigRS)
	return sigRS, nil
}

func HttpGet(url string) (string, error) {
	client := &http.Client{Timeout: 10 * time.Second}
	resp, err := client.Get(url)
	if err != nil {
		return "", err
	}
	if resp == nil {
		return "", fmt.Errorf("empty response ")
	}
	defer resp.Body.Close()

	if 200 != resp.StatusCode {
		// HTTP 204  if an account has no transaction
		if 204 == resp.StatusCode {
			return "", fmt.Errorf("not found any information")
		}
		return "", fmt.Errorf("http code: %v", resp.StatusCode)
	}

	var buffer [1024]byte
	result := bytes.NewBuffer(nil)
	for {
		n, err := resp.Body.Read(buffer[0:])
		result.Write(buffer[0:n])
		if err != nil && err == io.EOF {
			break
		} else if err != nil {
			return "", err
		}
	}

	return result.String(), nil
}

func HttpPost(url string, data interface{}, contentType string) (string, error) {
	client := &http.Client{Timeout: 10 * time.Second}
	jsonStr, _ := json.Marshal(data)
	resp, err := client.Post(url, contentType, bytes.NewBuffer(jsonStr))
	if err != nil {
		return "", err
	}
	if resp == nil {
		return "", fmt.Errorf("empty response ")
	}
	defer resp.Body.Close()

	result, err := ioutil.ReadAll(resp.Body)
	return string(result), err
}

func Transfer(amount int64, dstAddress string, memo string) error {

	fmt.Println("================ Step 0 : check parameters ")
	if amount < 1 || amount > (HtdfToSatoshi(60_000_000)) {
		return fmt.Errorf("invalid amount %v\n", amount)
	}

	// in this demo, `memo` DO NOT contains '\t' or '\r' or '\n'  or  space in memo
	if len(memo) > 256 || strings.ContainsAny(memo, "\t\r\n ") {
		return fmt.Errorf("memo is illegal")
	}

	//TODO: check dstAddress
	// reference: https://gitee.com/orientwalt/apidoc_2020/blob/master/demo/golang_bech32/bech32.go

	fmt.Println("==================== Step 1 : get account details by RPC")
	url := fmt.Sprintf("http://%s/auth/accounts/%s", gNodeIpPort, gFromAddress)
	data, err := HttpGet(url)
	if nil != err {
		return err
	}

	fmt.Printf("%s\n", data)
	var m map[string]interface{}
	if nil != json.Unmarshal([]byte(data), &m) {
		return err
	}
	mapValue := m["value"].(map[string]interface{})
	coins := mapValue["coins"].([]interface{})
	balance := coins[0].(map[string]interface{})["amount"].(string)
	accountNumber, err := strconv.Atoi(mapValue["account_number"].(string))
	if nil != err {
		return err
	}
	sequence, err := strconv.Atoi(mapValue["sequence"].(string))
	if nil != err {
		return err
	}
	fmt.Printf("account details:  accountNumber : %v, balance: %v, sequence: %v\n",
		accountNumber, balance, sequence)

	fmt.Println(" ===================== Step 2 : make raw transaction string")
	fmtUnsignedStr := fmt.Sprintf(gUnsignedStr, accountNumber, gChainId, gGasPrice,
		gGasLimit, memo, amount, gFromAddress, gGasPrice, gGasLimit, dstAddress, sequence)

	fmtUnsignedStr = strings.Replace(fmtUnsignedStr, " ", "", -1)
	fmtUnsignedStr = strings.Replace(fmtUnsignedStr, "\t", "", -1)
	fmtUnsignedStr = strings.Replace(fmtUnsignedStr, "\n", "", -1)
	fmtUnsignedStr = strings.Replace(fmtUnsignedStr, "\r", "", -1)

	fmt.Printf("formated raw transaction string: %v", fmtUnsignedStr)

	fmt.Printf("======================= Step 3 : make signature \n")
	hash := sha256.New()
	hash.Write([]byte(fmtUnsignedStr))
	msgHash := hash.Sum(nil)
	fmt.Printf("msgHash: %v\n", hex.EncodeToString(msgHash))

	pkBytes, err := hex.DecodeString(gPrivateKey)
	if nil != err {
		return err
	}

	sig, err := Sign(pkBytes, msgHash)
	if nil != err {
		return err
	}
	fmt.Printf("signature: %v\n", sig)

	fmt.Println("===================== Step 4 : make broadcast json string")
	sigBytes, err := hex.DecodeString(sig)
	if nil != err {
		return err
	}
	b64Sig := base64.StdEncoding.EncodeToString(sigBytes)
	fmt.Printf("base64 encoded signature : %v\n", b64Sig)

	_, pubKey := secp256k1.PrivKeyFromBytes(secp256k1.S256(), pkBytes[:])
	b64PubKey := base64.StdEncoding.EncodeToString(pubKey.SerializeCompressed())
	fmt.Printf("base64 encoded public key: %v", b64PubKey)

	fmtBroadcastStr := fmt.Sprintf(gBroadcastStr, gFromAddress, dstAddress, amount,
		gGasPrice, gGasLimit, gGasLimit, gGasPrice, b64PubKey, b64Sig, memo)

	fmtBroadcastStr = strings.Replace(fmtBroadcastStr, " ", "", -1)
	fmtBroadcastStr = strings.Replace(fmtBroadcastStr, "\t", "", -1)
	fmtBroadcastStr = strings.Replace(fmtBroadcastStr, "\n", "", -1)
	fmtBroadcastStr = strings.Replace(fmtBroadcastStr, "\r", "", -1)
	fmt.Printf("formated broadcast string : %v\n", fmtBroadcastStr)

	fmt.Println("=============== Step 5: hex-string encode and broadcast signed transaction data")
	hexBroadcastStr := hex.EncodeToString([]byte(fmtBroadcastStr))
	reqData := map[string]string{"tx": hexBroadcastStr}
	reqUrl := fmt.Sprintf("http://%s/hs/broadcast", gNodeIpPort)
	contentType := "application/json"
	rsp, err := HttpPost(reqUrl, reqData, contentType)
	if err != nil {
		return err
	}

	fmt.Println("=============  Step 6: check response ,  ")
	fmt.Println(rsp)

	return nil
}

func main() {

	amount := HtdfToSatoshi(0.12345678)
	tx_memo := "htdf_demo_in_golang"
	dstAddress := "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2"

	err := Transfer(amount, dstAddress, tx_memo)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println(" every thing is ok. ")
	}

	//then, get transaction details on blockchain  as below ( testchain )
	// curl -X GET  http://htdf2020-test01.orientwalt.cn:1317/txs/03FAD07F3B8B521FD4423BC8BC99A8E7F5CA76A8FD8253D8BD7FD2BA6261AA54

}
