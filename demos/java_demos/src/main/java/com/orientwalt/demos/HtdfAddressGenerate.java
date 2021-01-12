/*
 * author: yqq
 * date: 2020-10-09
 * descriptions: htdf address generation demo  (java)
 *
 * reference :   ../demo/python/htdf_address_generate.py
 *
 */

package com.orientwalt.demos;

import com.google.common.primitives.Bytes;
import org.bouncycastle.jce.ECNamedCurveTable;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.jce.spec.ECNamedCurveParameterSpec;
import org.bouncycastle.math.ec.ECPoint;
import org.bouncycastle.util.encoders.Hex;

import java.io.UnsupportedEncodingException;
import java.math.BigInteger;
import java.security.*;
import java.util.ArrayList;
import java.util.List;

public class HtdfAddressGenerate {

    // core algorithm
    // sha256
    // ripemd160
    // bech32
    // secp256k1

    public static final BigInteger MAX_P = new BigInteger(
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140", 16);

    public static byte[] generatePrivateKey() throws Exception {

        SecureRandom secureRandom = SecureRandom.getInstance("SHA1PRNG");

        BigInteger privateKeyCheck = BigInteger.ZERO;
        byte[] privateKeyAttempt = new byte[32];
        secureRandom.nextBytes(privateKeyAttempt);
        privateKeyCheck = new BigInteger(1, privateKeyAttempt);
        while (privateKeyCheck.compareTo(BigInteger.ZERO) == 0 || privateKeyCheck.compareTo(MAX_P) > 0) {
            secureRandom.nextBytes(privateKeyAttempt);
            privateKeyCheck = new BigInteger(1, privateKeyAttempt);
        }
        return privateKeyAttempt;
    }

    public static byte[] generatePublicKey(byte[] privateKey) {
        ECNamedCurveParameterSpec spec = ECNamedCurveTable.getParameterSpec("secp256k1");
        ECPoint pointQ = spec.getG().multiply(new BigInteger(1, privateKey));
        return pointQ.getEncoded(true);

    }

    public static String getSHA256(String str) {
        MessageDigest messageDigest;
        String encodestr = "";
        try {
            messageDigest = MessageDigest.getInstance("SHA-256");
            messageDigest.update(str.getBytes("UTF-8"));
            encodestr = byte2Hex(messageDigest.digest());
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return encodestr;
    }

    private static String byte2Hex(byte[] bytes) {
        return new String(Hex.encode(bytes));
    }

    public static byte[] encodeRipeMD160(byte[] data) throws Exception {
        Security.addProvider(new BouncyCastleProvider());
        MessageDigest md = MessageDigest.getInstance("RipeMD160");
        return md.digest(data);

    }

    public static String publicKeyToAddress(byte[] pubKey) throws Exception {

        MessageDigest sha256MsgDigst = MessageDigest.getInstance("SHA-256");
        sha256MsgDigst.update(pubKey);

        byte[] rp160Data = encodeRipeMD160(sha256MsgDigst.digest());

        Bech32Util bech32Inst = Bech32Util.getInstance();

        List<Byte> data = new ArrayList<>();
        data.addAll(Bytes.asList(rp160Data));

        byte[] convertData = Bech32Util.convertBits(data, 8, 5, false);
        String address = bech32Inst.bech32Encode("htdf".getBytes(), convertData);
        return address;
    }

    public static void main(String[] args) throws Exception {

        byte[] privateKey = generatePrivateKey();
        System.out.println("private key: " + byte2Hex(privateKey));

        // public key is not useful, only for debug
        byte[] publicKey = generatePublicKey(privateKey);
        System.out.println("public key: " + byte2Hex(publicKey));

        String address = publicKeyToAddress(publicKey);
        System.out.println("address: " + address);

    }

}
