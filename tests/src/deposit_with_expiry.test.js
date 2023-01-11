import "core-js/stable";
import "regenerator-runtime/runtime";
import { genericTx, nano_models } from './test.fixture';
import { ethers } from "ethers";
import { parseEther, parseUnits} from "ethers/lib/utils";


const contractName = "THORSwap Router V2"
const contractAddr = "0xC145990E84155416144C532E31f89B840Ca8c2cE";
const pluginName = "thorswap-router-v2";
const abi_path = `../networks/${testNetwork}/${pluginName}/abis/` + contractAddr + '.json';
const abi = require(abi_path);

const contract = new ethers.Contract(contractAddr, abi);


const vault ="0xa58dca25971fa9e0c809e12995f65b0a32156fb5";
const asset_ethereum ="0x0000000000000000000000000000000000000000";
const amount = parseUnits("43073650000000000", 'wei');
const memo_valid_standard = "=:AVAX.AVAX:0x681B29a3f3230Cb9Ad1247922BAA8E6a983466Eb:0:t:30"
const expiration = 1658875582

const {data} = (async () => {await contract.populateTransaction.depositWithExpiry(vault, asset_ethereum, amount, memo_valid_standard, expiration);})();


let unsignedTx = genericTx;
unsignedTx.to = contractAddr;
unsignedTx.data = data;
unsignedTx.value = parseEther("0.1");

// Create serializedTx and remove the "0x" prefix
const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);


const testLabel = "DepositWithExpiry(Valid Standard)"; // <= Name of the test
const testDirSuffix = "deposit_with_expiry_valid_standard"; // <= directory to compare device snapshots to
const testNetwork = "ethereum";
const signedPlugin = false;

const devices = [
  {
    name: "nanos",
    label: "Nano S",
    steps: 9 // <= Define the number of steps for this test case and this device
  },
  {
    name: "nanox",
    label: "Nano X",
    steps: 5 // <= Define the number of steps for this test case and this device
  }
];



nano_models.forEach((device) => processTest(device, contractName, testLabel, testDirSuffix, "", signedPlugin, serializedTx, testNetwork))

