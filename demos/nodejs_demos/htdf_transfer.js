/**
 * author: yqq
 * date: 2020-09-28
 * descriptions: HTDF transaction demo
 * reference : https://gitee.com/orientwalt/apidoc_2020/blob/master/demo/python/htdf_transfer.py
 */

const secp256k1 = require('secp256k1')
const sha256 = require('js-sha256')
const {Base64} = require('js-base64');
const request = require('request')
const utils = require('util')
const bech32 = require('bech32');
const { time } = require('console');
const { chmod } = require('fs');

const gChainId = "testchain"    // mainchain or testchain
const gFromAddress = "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml"
const gPrivateKey = "279bdcd8dccec91f9e079894da33d6888c0f9ef466c0b200921a1bf1ea7d86e8"
const gNodeIpPort = "htdf2020-test01.orientwalt.cn:1317"
const gGasLimit = 30000
const gGasPrice =  100


// mainchain parameters
//const gChainId = "mainchain"                                     //TODO:  mainchain or testchain
//const gFromAddress = "YOUR_HTDF_ADDRESS"                         //TODO
//const gPrivateKey = "YOUR_HTDF_ADDRESS_PRIVATE_KEY"              //TODO
//const gNodeIpPort = "YOUR_HTDF_FULL_NODE_IP_ADDRESS:1317" 		 //TODO
//const gGasLimit = 30000
//const gGasPrice =  100




const gUnsignedStr = `{
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

const gBroadcastStr = `{
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



// check address validations
function isValidHtdfAddress(address) {
    if(address.length !== 43 && address.toLocaleLowerCase() === address) {
        return false
    }

    try{
        let {prefix , words} =  bech32.decode(address)
        if(prefix === "htdf" && words.length === 32 ){
            return true
        }
    }catch (e) {
    }
    return false
}



function  Transfer( amount , dstAddress , memo   ) {
    // let memo = "htdf_demo_in_golang"

    console.log("===========> step 0 : check parameters")
    if (amount < 1 || amount > 60_000_000 ) {
        throw new Error("nvalid amount " + amount)
    }

    if (!isValidHtdfAddress(dstAddress) ) {
        throw new Error("invalid dstAddress: " + dstAddress)
    }

    console.log("=========> step 1 : get account details")
    let url = utils.format("http://%s/auth/accounts/%s" , gNodeIpPort , gFromAddress )
    let options = {
        method : 'GET',
        headers: { 'Content-Type': 'application/json' }
    }

    request(url, options , function (error, response, body) {

        if (error || response.statusCode !== 200) {
            throw new Error(response.body)
        }

        let accInfo = JSON.parse( body )
        let accountNumber = Number( accInfo["value"]["account_number"] )
        let sequence = Number(accInfo["value"]["sequence"])
        let balance = Number(accInfo["value"]["coins"][0]["amount"])

        let fmtUnsignedStr = utils.format(gUnsignedStr, accountNumber, gChainId, gGasPrice,
            gGasLimit, memo, amount, gFromAddress, gGasPrice, gGasLimit,  dstAddress, sequence)

        fmtUnsignedStr = fmtUnsignedStr.replace(/ /g, '')
        fmtUnsignedStr = fmtUnsignedStr.replace(/\t/g, '')
        fmtUnsignedStr = fmtUnsignedStr.replace(/\n/g, '')
        fmtUnsignedStr = fmtUnsignedStr.replace(/\r/g, '')

        console.log("formatted unsigned string :", fmtUnsignedStr)

        console.log("===========> step 2 : make signature ")

        let sh256 = new sha256.create()
        let msgHash = sh256.update(fmtUnsignedStr).digest()
        console.log("msgHash: ", Buffer.from(msgHash).toString("hex") )

        let privKey = Buffer.from(gPrivateKey, "hex")
        let sig = secp256k1.ecdsaSign(Buffer.from( msgHash), privKey)
        let b64Sig = Base64.encode(sig.signature)
        console.log("base64 encoded signature : ", b64Sig)


        console.log("============> step 3 : make broadcast string ")
        let pubkey = secp256k1.publicKeyCreate(privKey)
        console.log( Buffer.from( pubkey).toString("hex") )

        let b64PubKey = Base64.encode( Buffer.from(pubkey) )

        let fmtBroadcastStr = utils.format(gBroadcastStr, gFromAddress, dstAddress, amount,
            gGasPrice, gGasLimit, gGasLimit, gGasPrice, b64PubKey, b64Sig , memo)

        fmtBroadcastStr = fmtBroadcastStr.replace(/ /g, '')
        fmtBroadcastStr = fmtBroadcastStr.replace(/\t/g, '')
        fmtBroadcastStr = fmtBroadcastStr.replace(/\n/g, '')
        fmtBroadcastStr = fmtBroadcastStr.replace(/\r/g, '')

        console.log("formatted broadcast string: ", fmtBroadcastStr)



        console.log("=========> step 4: broadcast  ")
        let hexBroadcastStr = Buffer.from(fmtBroadcastStr).toString('hex')
        console.log("hex hexBroadcastStr : ", hexBroadcastStr )

        let reqData = {"tx" : hexBroadcastStr}
        url = utils.format("http://%s/hs/broadcast", gNodeIpPort )
        options = {
            method : 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(reqData)
        }

        request(url,  options, function (error, response, body) {
            if (error || response.statusCode !== 200) {
                throw new Error(response.body)
            }

            let rsp = JSON.parse( body )

            if(rsp["code"] ) {
                throw new Error( rsp["raw_log"]  ) // error msg
            }

            console.log("=================")
            console.log("broadcast success : ", rsp)

            console.log("=================")
            tx_url = "http://htdf2020-test01.orientwalt.cn:1317/txs/"
            tx_url += rsp["txhash"]
            console.log( tx_url )
            
            //then, get transaction details on blockchain  as below ( testchain )
            // curl http://htdf2020-test01.orientwalt.cn:1317/txs/930CCBD108B085F4D40CDFE3BD4FF81E7F4B5DA9EC462325ADF5CFB6A4C4F12F
        })
    })
}

function HtdfToSatoshi(htdfamount) {
    // 1 HTDF  = 10^8 satoshi
    return parseInt(htdfamount * 1_0000_0000)
}


function test() {
    try{
        let dstAddress = "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2"
        let amount = HtdfToSatoshi(0.1234567)
        let memo = "htdf_transfer_demo_in_js"

        Transfer( amount, dstAddress, memo)
    }catch (e) {
        console.error(e)
    }
}


test()

