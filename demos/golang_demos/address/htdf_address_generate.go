/**
date: 2020/9/27
author : yqq
descriptions:  HTDF  address generation demo  in golang

reference : https://gitee.com/orientwalt/apidoc_2020/blob/master/demo/python/htdf_address_generate.py
*/
package main

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"

	secp256k1 "github.com/btcsuite/btcd/btcec"
	"github.com/btcsuite/btcutil/bech32"
	"golang.org/x/crypto/ripemd160"
)

func GeneratePrivateKey() (*secp256k1.PrivateKey, error) {
	privateKey, err := secp256k1.NewPrivateKey(secp256k1.S256())
	if err != nil {
		return nil, err
	}
	return privateKey, nil
}

func PrivateKeyToCompressPubkey(privateKey *secp256k1.PrivateKey) []byte {
	pubKey := privateKey.PubKey().SerializeCompressed()
	return pubKey
}

func PubkeyToAddress(pubKey []byte) (string, error) {
	h256 := sha256.New()
	h256.Write(pubKey)

	rp160 := ripemd160.New()
	h256Pubkey := h256.Sum(nil)
	rp160.Write(h256Pubkey)
	data := rp160.Sum(nil)
	convertData, _ := bech32.ConvertBits(data, 8, 5, false)
	address, _ := bech32.Encode("htdf", convertData)
	return address, nil
}

func main() {
	privateKey, _ := GeneratePrivateKey()
	fmt.Printf("private key: %v\n", hex.EncodeToString(privateKey.Serialize()))

	pubKey := PrivateKeyToCompressPubkey(privateKey)
	fmt.Printf("public key: %v\n", hex.EncodeToString(pubKey))

	address, _ := PubkeyToAddress(pubKey)
	fmt.Printf("address: %v\n", address)
}
