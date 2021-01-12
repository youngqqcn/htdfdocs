/**
author: yqq
date: 2020-10-20
decriptions: convert old version keystore( less than v0.1.0 ) to new keystore (v0.1.0 and above)
*/
package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path"

	"github.com/spf13/cobra"
	"github.com/tendermint/tendermint/crypto"
	"github.com/tendermint/tendermint/crypto/secp256k1"
	"github.com/tendermint/tendermint/libs/bech32"
)

type KeyOld struct {
	Address string   `json:"address"`
	PubKey  [33]byte `json:"pubkey"`
	PrivKey string   `json:"privkey.armor"`
}

type Key struct {
	Address string `json:"address"`
	PubKey  string `json:"pubkey"`
	PrivKey string `json:"privkey"`
}

func Bech32ifyAccPub(pub crypto.PubKey) (string, error) {
	bech32PrefixAccPub := "htdfpub"
	return bech32.ConvertAndEncode(bech32PrefixAccPub, pub.Bytes())
}

func TestBech32Encode() {
	testPubKey := "htdfpub1addwnpepqvpqthz3f83c6m0meztg30t68t5jhursx8hc8tap3hdy8a2yhscy6gptu2e"
	pubkey := secp256k1.PubKeySecp256k1{3, 2, 5, 220, 81, 73, 227, 141, 109, 251, 200, 150, 136, 189, 122, 58, 233, 43, 240, 112, 49, 239, 131, 175, 161, 141, 218, 67, 245, 68, 188, 48, 77}
	encPubKey, err := Bech32ifyAccPub(pubkey)
	if err != nil {
		panic(err)
	}
	//fmt.Printf("pub key: %s\n", encPubKey)

	if testPubKey != encPubKey {
		fmt.Printf("pubkey not matched: expected: %s , got %s", testPubKey, encPubKey)
		return
	}
	//fmt.Println(" test pass")
}

func CheckFileExist(fileName string) bool {
	_, err := os.Stat(fileName)
	if os.IsNotExist(err) {
		return false
	}
	return true
}

//  convert old version keystore( less than v0.1.0 ) to new keystore (v0.1.0 and above)
func Convert(oldKeystoreFilePath string, newKeyStoreFilePath string) error {
	if !CheckFileExist(oldKeystoreFilePath) {
		return fmt.Errorf("%s not exists \n", oldKeystoreFilePath)
	}

	// read keystore file
	f, err := os.Open(oldKeystoreFilePath)
	if err != nil {
		return err
	}
	defer f.Close()

	data, err := ioutil.ReadAll(f)
	if err != nil {
		return err
	}

	// unmarshal
	var keyOld KeyOld
	if err := json.Unmarshal(data, &keyOld); err != nil {
		return fmt.Errorf("file : %s , Unmarshal error: %v ", oldKeystoreFilePath, err)
	}

	// convert pubkey
	pubKey := secp256k1.PubKeySecp256k1(keyOld.PubKey)
	encPubkey, err := Bech32ifyAccPub(pubKey)
	//fmt.Println(encPubkey)

	var key Key
	key.PubKey = encPubkey
	key.Address = keyOld.Address
	key.PrivKey = keyOld.PrivKey

	outData, err := json.Marshal(key)
	if err != nil {
		return fmt.Errorf("file : %s , Marshal error: %v ", oldKeystoreFilePath, err)
	}

	// output
	err = ioutil.WriteFile(newKeyStoreFilePath, outData, 0666)
	if err != nil {
		return fmt.Errorf("WriteFile : %s , error: %v ", newKeyStoreFilePath, err)
	}

	return nil
}

func GetFileList(dirpath string) ([]string, error) {
	var fileList []string
	fInfos, err := ioutil.ReadDir(dirpath)
	if err != nil {
		return fileList, err
	}

	for i := range fInfos {
		if fInfos[i].IsDir() {
			continue
		}

		fileList = append(fileList, fInfos[i].Name())
	}

	return fileList, err
}

func IsDir(fileAddr string) bool {
	s, err := os.Stat(fileAddr)
	if err != nil {
		log.Println(err)
		return false
	}
	return s.IsDir()
}

func main() {
	TestBech32Encode()
	//err := Convert("./test_keystore.data", "new_test.txt")
	//if err != nil {
	//	panic(err)
	//}

	var converCmd = cobra.Command{
		Use:     "keystore_convert  --old old_keystores_absolute_dir  --new new_keystores_absolute_dir ",
		Example: "./keystore_convert --old /home/demo/.hscli/keystores/ --new /home/demo/new_newkeystores",
		Run: func(cmd *cobra.Command, args []string) {
			olddir, err := cmd.Flags().GetString("old")
			if err != nil {
				panic(err)
			}

			newdir, err := cmd.Flags().GetString("new")
			if err != nil {
				panic(err)
			}

			if len(olddir) == 0 || len(newdir) == 0 {
				cmd.Help()
				return
			}

			if !IsDir(olddir) {
				panic(fmt.Errorf("old dir : %v  not exists!\n", olddir))
			}
			if !IsDir(newdir) {
				panic(fmt.Errorf("new dir : %v  not exists!\n", newdir))
			}

			fileList, err := GetFileList(olddir)
			if err != nil {
				panic(err)
			}

			for _, filename := range fileList {
				oldpath := path.Join(olddir, filename)
				newpath := path.Join(newdir, filename)
				if err := Convert(oldpath, newpath); err != nil {
					panic(err)
				}
			}
			fmt.Printf("Successfully converted %d files!\n", len(fileList))
		},
	}
	converCmd.Flags().StringP("old", "o", "", "old keystores directory (absolute path)")
	converCmd.Flags().StringP("new", "n", "", "new keystores output dierectory(absolute path)")

	if err := converCmd.Execute(); err != nil {
		panic(err)
	}
}
