# THORSwap Plugin

Ledger app plugin for the [THORChain Router v2 Contract](https://etherscan.io/address/0xD37BbE5744D730a1d98d8DC97c42F0Ca46aD7146#code)

## Ethereum SDK

Ethereum plugins need the [Ethereum SDK](https://github.com/LedgerHQ/ethereum-plugin-sdk).
You can use the `ETHEREUM_PLUGIN_SDK` variable to point to the directory where you cloned
this repository. By default, the `Makefile` expects it to be at the root directory of this
plugin repository, by the `ethereum-plugin-sdk` name.

This repository is deliberately **not** a submodule. You can see that the CI workflows
clone and checkout either the latest `master` or on `develop` references. This ensures
the code is compiled and tested on the latest version of the SDK.

## Formatting

The C source code is expected to be formatted with `clang-format` 11.0.0 or higher.
