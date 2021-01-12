/*
 * author: yqq
 * date: 2020-10-09
 * descriptions: htdf transfer demo  ( java )
 *
 * reference: ../demo/python/htdf_transfer.py
 *
 */

package com.orientwalt.demos;


import com.alibaba.fastjson.JSONObject;
import com.google.common.primitives.Bytes;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;
import org.bouncycastle.jce.ECNamedCurveTable;
import org.bouncycastle.jce.spec.ECNamedCurveParameterSpec;
import org.bouncycastle.math.ec.ECPoint;
import org.bouncycastle.util.encoders.Hex;
import org.web3j.crypto.ECKeyPair;
import org.web3j.crypto.Sign;

import java.io.IOException;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.Base64;



public class HtdfTransfer {

    private static final String gChainId = "testchain";        // mainchain or testchain
    private static final String gFromAddress = "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml";
    private static final String gPrivateKey = "279bdcd8dccec91f9e079894da33d6888c0f9ef466c0b200921a1bf1ea7d86e8";
    private static final String gNodeIpPort = "htdf2020-test01.orientwalt.cn:1317";
    private static final int gGasLimit = 30000;
    private static final int gGasPrice =  100;


    // mainchain parameters
//    private static final String gChainId = "mainchain";                                     // mainchain or testchain
//    private static final String  gFromAddress = "YOUR_HTDF_ADDRESS"                         //TODO
//    private static final String  gPrivateKey = "YOUR_HTDF_ADDRESS_PRIVATE_KEY"              //TODO
//    private static final String  gNodeIpPort = "YOUR_HTDF_FULL_NODE_IP_ADDRESS:1317" 		  //TODO
//    private static final int gGasLimit = 30000;
//    private static final int gGasPrice =  100;


    private static final String  gUnsignedStr = "{\n" +
            "    \"account_number\": \"%d\",\n" +
            "    \"chain_id\": \"%s\",\n" +
            "    \"fee\": {\n" +
            "        \"gas_price\": \"%d\",\n" +
            "        \"gas_wanted\": \"%d\"\n" +
            "    },\n" +
            "    \"memo\": \"%s\",\n" +
            "    \"msgs\": [{\n" +
            "        \"Amount\": [{\n" +
            "        \"amount\": \"%d\",\n" +
            "        \"denom\": \"satoshi\"\n" +
            "    }],\n" +
            "        \"Data\": \"\",\n" +
            "        \"From\": \"%s\",\n" +
            "        \"GasPrice\": %d,\n" +
            "        \"GasWanted\": %d,\n" +
            "        \"To\": \"%s\"\n" +
            "    }],\n" +
            "    \"sequence\": \"%d\"\n" +
            "}";

    private  static final  String gBroadcastStr = "{\n" +
            "    \"type\": \"auth/StdTx\",\n" +
            "        \"value\":{\n" +
            "        \"msg\": [{\n" +
            "            \"type\": \"htdfservice/send\",\n" +
            "            \"value\":{\n" +
            "                \"From\": \"%s\",\n" +
            "                \"To\": \"%s\",\n" +
            "                \"Amount\": [{\n" +
            "                    \"denom\": \"satoshi\",\n" +
            "                    \"amount\": \"%d\"\n" +
            "                }],\n" +
            "                \"Data\": \"\",\n" +
            "                \"GasPrice\": \"%d\",\n" +
            "                \"GasWanted\": \"%d\"\n" +
            "            }\n" +
            "        }],\n" +
            "    \"fee\": {\n" +
            "        \"gas_wanted\": \"%d\",\n" +
            "        \"gas_price\": \"%d\"\n" +
            "    },\n" +
            "    \"signatures\": [{\n" +
            "        \"pub_key\": {\n" +
            "        \"type\": \"tendermint/PubKeySecp256k1\",\n" +
            "        \"value\": \"%s\"\n" +
            "    },\n" +
            "    \"signature\": \"%s\"\n" +
            "    }],\n" +
            "    \"memo\": \"%s\"\n" +
            "    }\n" +
            "}";

    // this method exists a bug. when s[0] is greater than 0x80 , R is right but S is wrong.
    // I have not found solution yet, so I use web3j's sign to replace this method. 2020-10-09
    // public static byte[] sign(byte[] message, byte[] privateKey) {
    //     Security.addProvider(new BouncyCastleProvider());

    //     final String SECP256K1 = "secp256k1";
    //     ECNamedCurveParameterSpec spec = ECNamedCurveTable.getParameterSpec(SECP256K1);

    //     ECDSASigner ecdsaSigner = new ECDSASigner(new HMacDSAKCalculator(new SHA256Digest()));

    //     ECDomainParameters domain = new ECDomainParameters(spec.getCurve(),  spec.getG(),  spec.getN());

    //     ECPrivateKeyParameters privateKeyParams =
    //             new ECPrivateKeyParameters(new BigInteger(1, privateKey), domain);

    //     ecdsaSigner.init(true, privateKeyParams);

    //     BigInteger[] sig = ecdsaSigner.generateSignature(message);

    //     String  R = sig[0].toString(16);
    //     String  S = sig[1].toString(16);
    //     return Hex.decode(R + S);
    // }

    // use web3j
    public static byte[] sign(byte[] message, byte[] privateKey) {
        ECKeyPair eckKeyPair = ECKeyPair.create(privateKey);
        Sign.SignatureData sig = Sign.signMessage(message, eckKeyPair, false);
        return Bytes.concat(sig.getR(), sig.getS());
    }

    public static BigInteger HtdfToSatoshi(BigDecimal amount) {
        // 1 HTDF = 10 ^ 8 satoshi
        return amount.multiply(new BigDecimal(10).pow(8)).toBigInteger();
    }

    public static String doHttpGet(String url) throws Exception {
        CloseableHttpClient httpClient = HttpClientBuilder.create().build();
        HttpGet httpGet = new HttpGet(url);

        String ret = "";
        try {
            CloseableHttpResponse response = httpClient.execute(httpGet);
            if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
                HttpEntity responseEntity = response.getEntity();

                if (responseEntity != null) {
                    ret = EntityUtils.toString(responseEntity);
                    response.close();
                }
            } else {
                throw new Exception(response.getStatusLine().getReasonPhrase());
            }

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            httpClient.close();
        }
        return ret;
    }

    public static JSONObject doHttpPost(String url, JSONObject jsonObject) throws RuntimeException {
        HttpClient client = HttpClientBuilder.create().build();
        HttpPost post = new HttpPost(url);
        JSONObject response = null;

        try {
            StringEntity s = new StringEntity(jsonObject.toString());
            s.setContentEncoding("UTF-8");
            s.setContentType("application/json");
            post.setEntity(s);
            HttpResponse res = client.execute(post);
            if (res.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
                HttpEntity entity = res.getEntity();
                String result = EntityUtils.toString(entity);
                response = JSONObject.parseObject(result);
            } else {
                throw new Exception(res.getStatusLine().getReasonPhrase());
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        return response;
    }

    public static byte[] getSha256(byte[] data) throws Exception {
        MessageDigest messageDigest = MessageDigest.getInstance("SHA-256");
        messageDigest.update(data);
        return messageDigest.digest();
    }

    public static byte[] generatePublicKey(byte[] privateKey) {
        ECNamedCurveParameterSpec spec = ECNamedCurveTable.getParameterSpec("secp256k1");
        ECPoint pointQ = spec.getG().multiply(new BigInteger(1, privateKey));
        return pointQ.getEncoded(true);
    }

    public static void transfer(String dstAddress, BigInteger amount, String memo) throws Exception {

        if (amount.compareTo(BigInteger.ONE) <= 0
                || amount.compareTo(HtdfToSatoshi(BigDecimal.valueOf(6000_0000))) >= 0) {
            throw new Exception("invalid amount");
        }

        String reqUrl = String.format("http://%s/auth/accounts/%s", gNodeIpPort, gFromAddress);
        String rsp = doHttpGet(reqUrl);
        JSONObject accInfo = JSONObject.parseObject(rsp);

        JSONObject value = accInfo.getJSONObject("value");

        double balance = Double.parseDouble(value.getJSONArray("coins").getJSONObject(0).getString("amount"));
        if (HtdfToSatoshi(BigDecimal.valueOf(balance)).compareTo(amount) < 0) {
            throw new Exception("balance is not enough for transfer");
        }

        int accountNumber = Integer.parseInt(value.getString("account_number"));
        int sequence = Integer.parseInt(value.getString("sequence"));

        String unsignedStr = gUnsignedStr.replaceAll("[\\t\\n\\r ]", "");
        String fmtUnsignedStr = String.format(unsignedStr, accountNumber, gChainId, gGasPrice, gGasLimit, memo, amount,
                gFromAddress, gGasPrice, gGasLimit, dstAddress, sequence);
        System.out.println("formatted raw transaction string: " + fmtUnsignedStr);

        byte[] sha256Bytes = getSha256(fmtUnsignedStr.getBytes(StandardCharsets.UTF_8));
        System.out.println("msgHash : " + Hex.toHexString(sha256Bytes));

        byte[] privKeyBytes = Hex.decode(gPrivateKey);
        byte[] sigRS = sign(sha256Bytes, privKeyBytes);
        System.out.println("signature : " + Hex.toHexString(sigRS));

        final Base64.Encoder base64 = Base64.getEncoder();
        String b64Sig = base64.encodeToString(sigRS);
        System.out.println("base64 encoded signature : " + b64Sig);

        byte[] pubKeyBytes = generatePublicKey(privKeyBytes);
        String b64PubKey = base64.encodeToString(pubKeyBytes);
        System.out.println("base64 encoded public key: " + b64PubKey);

        String broadcastStr = gBroadcastStr.replaceAll("[\\t\\n\\r ]", "");
        String fmtBroadcastStr = String.format(broadcastStr, gFromAddress, dstAddress, amount, gGasPrice, gGasLimit,
                gGasLimit, gGasPrice, b64PubKey, b64Sig, memo);

        System.out.println("formated broadcast string : " + fmtBroadcastStr);

        String hexBroadcastStr = Hex.toHexString(fmtBroadcastStr.getBytes());
        System.out.println("hex broadcast str: " + hexBroadcastStr);

        JSONObject reqObj = new JSONObject();
        reqObj.put("tx", hexBroadcastStr);

        String broadcastUrl = String.format("http://%s/hs/broadcast", gNodeIpPort);
        JSONObject bcastResponse = doHttpPost(broadcastUrl, reqObj);
        System.out.println("broadcast response : " + bcastResponse.toString());

        if (bcastResponse.containsKey("raw_log")) {
            throw new Exception(bcastResponse.getString("raw_log"));
        }

        System.out.println("================");
        String txhash = bcastResponse.getString("txhash");
        String tx_url = "http://htdf2020-test01.orientwalt.cn:1317/txs/" + txhash;
        System.out.println("tx_url: " + tx_url);

        int nSecondsToWait = 10;
        System.out.printf("Waiting %d seconds for tx ", nSecondsToWait);
        for (int i = nSecondsToWait; i > 0; i--) {
            System.out.printf(".");
            Thread.sleep(1000);
        }
        System.out.println();

        String strTx = doHttpGet(tx_url);
        // JSONObject.parseObject(strTx);
        System.out.println(strTx);

        // Congratulation! If everything is ok.
        // then, you could get transaction details on blockchain by http RPC (testchain)
        // curl http://htdf2020-test01.orientwalt.cn:1317/txs/{txhash}
        // example: curl
        // http://htdf2020-test01.orientwalt.cn:1317/txs/B85D5829299C63BC38BB2F215ED1405EEAB79982E14C3FEC4A748EC9A1604A57

    }

    public static void main(String[] args) throws Exception {

        String dstAddress = "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2";
        String memo = "htdf transaction demo in java";
        BigInteger amountSatoshi = HtdfToSatoshi(BigDecimal.valueOf(0.12345678));
        transfer(dstAddress, amountSatoshi, memo);

    }

}
