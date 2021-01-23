let HTDF = require("htdf_web3");

let htdf = new HTDF("http://htdf2020-test01.orientwalt.cn:1317","testchain");

let Web3 = require('htdf_web3/web3')

let web3 = new Web3()

let abi = '[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"founder","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"newFounder","type":"address"}],"name":"changeFounder","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"inputs":[],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"}]'
    abi = JSON.parse(abi)

let contract_address = htdf.address.isValidHtdfAddress('htdf1vtdajv6jd6a8ymdxmjea9f95yq85ucmlavps6c')

let from_address = htdf.address.isValidHtdfAddress('htdf1pj9vwp226l3gsq5fzk0a58adyad9lg7hw2xjlz') 
let from_private = '7411b3815230a3b266222a6a7797c39e06972893e60b22e0c7cd668c4b3545ab'

let to_address = htdf.address.isValidHtdfAddress('htdf16pz2k07lnd63q4dlft23jqsm87w3m660fgx4t6')
let to_address_private = 'cfd95e93e64c1c50a72e7071ccda5669a3cfc26c4205d00220acd732ada37991'

let new_to_address = htdf.address.isValidHtdfAddress('htdf1nuxf4amphaajuwg0ph3se6kmsda9cs6v0sja7r')
let new_to_address_private = '82467e3de6c1499866eda81f493aa502f1a65bd6d7f7e669df63139e9fe37adb'

var MyContract = new web3.eth.Contract(abi,contract_address);

function Contract_name(){
    MyContract.methods.name().call(function(err,result){
        console.log(result)
    });
}

// data
function Contract_name_data(){
   MyContract.methods.name().call({
     'whether':1   
   },function(err,result){
      console.log(result)
   });
}

function Contract_balanceOf(){
   MyContract.methods.balanceOf(new_to_address).call(function(err,result){
      console.log(result)
   });
}

// data
function Contract_approve_data(){
   MyContract.methods.approve(to_address,100).call({
     'whether':1
   },function(err,result){
      var data = result['params'][0].data
      console.log(data)
   });
}

// txhash:4082A0310788746AB06A0E101B7E3C7F9B988E2130BD42C6703B35954C4BDED2
function Contract_approve(){
   MyContract.methods.approve(new_to_address,1)
   .send({
      from: from_address,
      to:contract_address, 
      gas:100,
      gasPrice:200000,
      remark:1,
      gPrivateKey:from_private,
   },function(err,result){
      console.log(result)
   });
}

// txhash:00E96A9CB2CC957B20B39D82A8C3E3E2E80F3B19ACCFF2E1AC89A70F9ABC9390
function Contract_transferFrom(){
   MyContract.methods.transferFrom(from_address,new_to_address,1)
  .send({
      from:new_to_address,
      to:contract_address, 
      gas:100,
      gasPrice:200000,
      remark:1,
      gPrivateKey:new_to_address_private,
  },function(err,result){
     console.log(result)
  });
}

// txhash:F4696222112D8CB8E08A9071DAD40A208DC6CA487D3861DEB9F38D1D5503A687
function Contract_transfer(){
   MyContract.methods.transfer(new_to_address,10)
   .send({
      from: from_address,
      to:new_to_address,
      gas:100,
      gasPrice:200000,
      remark:1,
      gPrivateKey:from_private,
   },function(err,result){
      console.log(result)
   });
}


