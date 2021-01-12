/*
 * author: yqq
 * date: 2020-10-09
 * descriptions: test case
 */

package com.orientwalt.demos;

import org.bouncycastle.util.encoders.Hex;
import org.junit.Test;

import java.math.BigInteger;
import java.nio.charset.StandardCharsets;

import static com.orientwalt.demos.HtdfAddressGenerate.*;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class HtdfAddressGenerateTest {


    private  static final String  HELLO = "hello";


    @Test
    public void testSha256() {
        String sha256Hello = getSHA256( HELLO);
        final String SHA256_RESULT = "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824";
        assertTrue( sha256Hello.compareTo(SHA256_RESULT)  == 0);
    }


    @Test
    public void testRipemd160() throws Exception {
        final  String RIPEMD160_RESULT = "108f07b8382412612c048d07d13f814118445acd";
        String rp160 =  Hex.toHexString( encodeRipeMD160(HELLO.getBytes()) ) ;
        assertTrue( rp160.compareTo(RIPEMD160_RESULT) == 0);
    }


    @Test
    public void testBech32() throws Exception {

        /*
        python3  :  pip3 install bech32

        >>> from bech32 import bech32_encode
        >>> bech32_encode("hello", [14, 15, 3, 31, 13])
        'hello1w0rld80pk3y'
        */


        final String  BECH32_RESULT = "hello1w0rld80pk3y";
        Bech32Util bech32Inst = Bech32Util.getInstance();
        byte[] tstBytes = new byte[] {14, 15, 3, 31, 13};
        String bech32Result =  bech32Inst.bech32Encode( HELLO.getBytes(StandardCharsets.UTF_8), tstBytes ) ;
        assertTrue(bech32Result.compareTo(BECH32_RESULT) == 0);
    }




    @Test
    public void generatePrivateKey() throws Exception {


        byte[] privkey  = HtdfAddressGenerate.generatePrivateKey();
        assertEquals(privkey.length,  32  );
        BigInteger bigintPrivakey  =  new BigInteger(1, privkey);
        assertTrue( bigintPrivakey.compareTo(BigInteger.ZERO) != 0 && bigintPrivakey.compareTo(MAX_P) < 1   );
    }

    @Test
    public void generatePublicKey() {

        //private key: 13044bd3f558609827bb94320710ef8bcee393f8a6ca10abc881cbc3ff20c1a0
        //public key:  03c8bd1f8009e9b8228b92f9f4f8e22c15f7caf2e47207a3ab3e30eeb95c6af3b7
        //address: htdf1qmgs5gzd685sxd7x3tkjtfur4rz5tga06g5nj9

        byte[] privkey  = Hex.decode("13044bd3f558609827bb94320710ef8bcee393f8a6ca10abc881cbc3ff20c1a0");
        byte[] pubkey = HtdfAddressGenerate.generatePublicKey(privkey);
        assertEquals(pubkey.length, 33 );
        assertEquals(pubkey[0], 0x03);

        String strPubKey =  Hex.toHexString(pubkey);
        assertTrue( 0 == strPubKey.compareTo( "03c8bd1f8009e9b8228b92f9f4f8e22c15f7caf2e47207a3ab3e30eeb95c6af3b7" )  );

    }

    @Test
    public void publicKeyToAddress() throws Exception{

        //private key: 13044bd3f558609827bb94320710ef8bcee393f8a6ca10abc881cbc3ff20c1a0
        //public key:  03c8bd1f8009e9b8228b92f9f4f8e22c15f7caf2e47207a3ab3e30eeb95c6af3b7
        //address: htdf1qmgs5gzd685sxd7x3tkjtfur4rz5tga06g5nj9

        byte[] pubkey = Hex.decode("03c8bd1f8009e9b8228b92f9f4f8e22c15f7caf2e47207a3ab3e30eeb95c6af3b7");
        String address = HtdfAddressGenerate.publicKeyToAddress(pubkey);
        assertTrue( 0 == address.compareTo( "htdf1qmgs5gzd685sxd7x3tkjtfur4rz5tga06g5nj9" )  );

    }
}