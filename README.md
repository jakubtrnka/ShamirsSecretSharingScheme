# Shamir's secret sharing scheme for HD-bitcoin wallets
Standardized way of making distributed backups of secret message using Shamir's secret sharing technique. Current version operates on enthropy used for generating Bitcoin [HD-wallet](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki) seed which is currently mostly encapsulated in mnemonic phrases given by standard [BIP0039](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki).

The method follows standard given by standard [SLIP0039](https://github.com/satoshilabs/slips/blob/master/slip-0039.md).
Program reads BIP0039 mnemonic phrase, restores encoded enthropy and splits it into given number of shamir's shares with a given reconstruction threshold. Each share is encapsulated into array described in SLIP0039 and encoded using word-list.
Existing Shamir's shares can be reconstructed into BIP0039 mnemonic phrase. Program recognizes reconstruction threshold from first share and asks for next share unless enough shares are supplied.
The simple schematics of how this was implemented is given in the following picture:<br>
<div align="center">
<a href="docs/long-message-shamir.svg">
<img src="docs/long-message-shamir.svg" alt="Shamir's key split" style="width: 400px;"/></a>
</div>

## Compilation
Program follows standard C++11 and can be compiled using command
```
$ make
```
in project directory.

## Basic usage
Program must be launched either with `-d` of `-m` argument, which determines whether secret will be distributed (`-d`) into shamir's shares or restored (merged, `-m`) from existing shamir's shares. When secret option `-d` is applied, also reconsturction threshold (`-t`) and total share count (`-n`) needs to be specified together with their values.
Threshold must be number in range 1 .. 32, total share count must be greater or equal to the threshold but at most 32.
```
$ ./shamir -d -t 3 -n 5
$ ./shamir -m
```
