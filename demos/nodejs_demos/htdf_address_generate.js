/**
 * author: yqq
 * date: 2020-09-28
 * descriptions: HTDF address generate tool , js
 */

const { randomBytes }  = require('crypto')
const secp256k1 = require('secp256k1')
const sha256 = require('js-sha256')
const ripemd160 = require('ripemd160')
const bech32 = require('bech32')
const EC = require('elliptic').ec
const ec = new EC('secp256k1')

function generate_private_key() {
    let privKey
    do {
        privKey = randomBytes(32)
    } while (!secp256k1.privateKeyVerify(privKey))
    return privKey
}

function privkey_to_compress_pubkey(privateKey) {
    const point = ec.keyFromPrivate(privateKey).getPublic()
    return Buffer.from(point.encode(null, true))
}


function pubkey_to_address(pubKey, hrp="htdf") {
    let rp160 = new ripemd160()
    let sh256 = new sha256.create()
    let sh256data = sh256.update(pubKey).digest()
    let rp160data =  rp160.update(Buffer.from( sh256data )).digest()
    let address = bech32.encode(hrp, bech32.toWords( Buffer.from(rp160data) ) )
    return address
}

function generate_htdf_address() {

    let private_key = generate_private_key()
    let public_key = privkey_to_compress_pubkey(private_key)
    let address = pubkey_to_address(public_key)

    console.log( "private key: ", Buffer.from(private_key).toString('hex') )
    console.log( "public key: ", public_key.toString('hex') )
    console.log("address : ",  address)

}

generate_htdf_address()


