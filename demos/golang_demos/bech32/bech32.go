package main

import (
	"errors"
	"fmt"
	"os"

	"github.com/tendermint/tendermint/libs/bech32"
)

const (
	PREFIX = "htdf"
)

// GetFromBech32 decodes a bytestring from a Bech32 encoded string.
func GetFromBech32(bech32str, prefix string) ([]byte, error) {
	if len(bech32str) == 0 {
		return nil, errors.New("decoding Bech32 address failed: must provide an address")
	}

	hrp, bz, err := bech32.DecodeAndConvert(bech32str)
	if err != nil {
		return nil, err
	}

	if hrp != prefix {
		return nil, fmt.Errorf("invalid Bech32 prefix; expected %s, got %s", prefix, hrp)
	}

	return bz, nil
}

func validAddress(bech32str string) bool {
	_, error := GetFromBech32(bech32str, PREFIX)
	if error != nil {
		return false
	}

	return true
}

func demo(bech32str string) {

	//bech to hex20
	fmt.Printf("bech32str=%s\n", bech32str)
	hex20ContractAddr, err := GetFromBech32(bech32str, PREFIX)
	if err != nil {
		fmt.Printf("GetFromBech32 error|err=%s\n", err)
		os.Exit(1)
	}
	fmt.Printf("hex20ContractAddr=%x\n", hex20ContractAddr)

	//hex20 to bech32
	bech32Tmp, err := bech32.ConvertAndEncode(PREFIX, hex20ContractAddr)
	if err != nil {
		fmt.Printf("ConvertAndEncode error|err=%s\n", err)
		os.Exit(1)
	}

	if bech32str != bech32Tmp {
		fmt.Printf("ConvertAndEncode error|err=%s\n", err)
		os.Exit(1)
	}
}

func main() {

	//address convert
	bech32Addr := "htdf1nkkc48lfchy92ahg50akj2384v4yfqpm4hsq6y"
	demo(bech32Addr)

	bech32Addr = "htdf1sh8d3h0nn8t4e83crcql80wua7u3xtlfj5dej3"
	demo(bech32Addr)

	bech32Addr = "htdf17qarupfh9gee0yvywhxfy2zv39fjttracvgapx"
	demo(bech32Addr)

	//check valid address
	bech32Addr = "htdf17qarupfh9gee0yvywhxfy2zv39fjttracvgapx"
	bValid := validAddress(bech32Addr)
	fmt.Printf("address=%s|valid=%v\n", bech32Addr, bValid)

	bech32Addr = "htdf17qarupfh9gee0yvywhxfy2zv39fjttracvgapy"
	bValid = validAddress(bech32Addr)
	fmt.Printf("address=%s|valid=%v\n", bech32Addr, bValid)

}
