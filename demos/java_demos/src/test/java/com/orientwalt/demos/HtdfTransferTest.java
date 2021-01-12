/*
 * author: yqq
 * date: 2020-10-09
 * descriptions: test case
 */

package com.orientwalt.demos;

import org.bouncycastle.util.encoders.Hex;
import org.junit.Test;

import java.math.BigDecimal;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;

import static com.orientwalt.demos.HtdfTransfer.HtdfToSatoshi;
import static org.junit.Assert.assertTrue;

public class HtdfTransferTest {

    @Test
    public void sign() throws Exception {

        String hexPrivateKey = "25E9F2EDAAF9464E9FA0EFDB896835741EBE0F5E34F97CFB88457818B6681C32";
        byte [] privateKey = Hex.decode(hexPrivateKey);

        MessageDigest sh256Digest = MessageDigest.getInstance("SHA-256");
        sh256Digest.update("hello".getBytes(StandardCharsets.UTF_8));
        byte[] sha256Bytes =  sh256Digest.digest();

        byte[] sigRS = HtdfTransfer.sign(sha256Bytes, privateKey);

        // test vector
        //r:5a74e18c921f8a1d0e4086cbb3806942829613fba638c1cb75b5da35324c8ab6
        //s:6e97dba80c7050b2bf2afbaa2a6a5ee93646e36218c9a9bbda56d377df15989a
        final String RIGHT_RS = "5a74e18c921f8a1d0e4086cbb3806942829613fba638c1cb75b5da35324c8ab6" +
                        "6e97dba80c7050b2bf2afbaa2a6a5ee93646e36218c9a9bbda56d377df15989a";

        System.out.println( Hex.toHexString(sigRS) );

        assertTrue( RIGHT_RS.compareTo( Hex.toHexString(sigRS) ) == 0 );
    }


    @Test
    public void htdfToSatoshi1() {
        assertTrue(0 == HtdfToSatoshi(BigDecimal.valueOf(0.1234)).compareTo(BigInteger.valueOf(12340000)));
    }

    @Test
    public void doHttpGet() throws Exception {
        String ret = HtdfTransfer.doHttpGet("http://www.htdfscan.com");
        assertTrue(ret.length() > 0);
    }

    @Test
    public void doHttpPost() {
        /*
        JSONObject reqData = new JSONObject();
        reqData.put("tx", "24234829482394829342389482394823948234");
        String url = String.format("http://%s/hs/broadcast" , gNodeIpPort);
        String  ret = HtdfTransfer.doHttpPost(url, reqData).toString();
        assertTrue(ret.length() > 0);
        */
    }



    @Test
    public void transfer() throws Exception {
        String dstAddress = "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2";
        String memo = "htdf transaction demo in java";
        BigInteger amountSatoshi = HtdfToSatoshi( BigDecimal.valueOf(0.12345678));
        HtdfTransfer.transfer( dstAddress, amountSatoshi , memo );
    }
}