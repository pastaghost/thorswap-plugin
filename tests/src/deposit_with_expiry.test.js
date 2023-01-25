import { processTest } from "./test.fixture";

const contractName = "THORChain Router v4"
// From : https://etherscan.io/tx/0xdeea9126dd842d39d217de2d79e1c0dfb36d021e67388d3ea45fdca89d19d7f7
const rawTxHex =
  "0x44bc937b000000000000000000000000a9ce88d2a853836e9da4886e6ac569c02b59d00300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005bd5440154d9600000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000063d0f1390000000000000000000000000000000000000000000000000000000000000047535741503a54484f522e52554e453a74686f72317432706673637571336374677466356833783770367a726a643765306a637675737a797674353a35393934373435333539383600000000000000000000000000000000000000000000000000";
const testLabel = "Deposit with Expiry"; // <= Name of the test
const testDirSuffix = "deposit_with_expiry"; // <= directory to compare device snapshots to
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

devices.forEach((device) =>
  processTest(device, contractName, testLabel, testDirSuffix, rawTxHex, signedPlugin,"",testNetwork)
);


