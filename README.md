# Shamir's secret sharing scheme for HD-bitcoin wallets

[Shamir's secret sharing](https://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing) enables to split some secret between multiple parties in a secure way through the use of *shares*. In order to reconstruct original secret from shares, specific number of those shares is required (reconstruction threshold), while if not enough shares is available, no information about the secret is revealed.

There are many ways of implementing this technique. This program implements design proposed by document [SLIP39](https://github.com/satoshilabs/slips/blob/master/slip-0039.md).
It operates on enthropy used for generating Bitcoin [HD-wallet](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki) seed which is currently mostly encapsulated in mnemonic phrases given by standard [BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki).

**This is experimental implementation and may contain bugs - use it on your own responsibility.***

Program reads BIP39 mnemonic phrase, restores encoded enthropy and splits it into given number of Shamir's shares with a given reconstruction threshold. Each share contains metadata (share index and reconstruction threshold), the actual share, and checksum for random error detections.


Shares produced with this method can be used to restore original secret (enthropy) and outputs BIP39 mnemonic phrase. Program recognizes reconstruction threshold from the first share and keeps asking for another shares unless the reconstruction threshold is reached.

The simple schematics of how this was implemented is given in the following picture:<br>
<div align="center">
<a href="docs/long-message-shamir.png">
<img src="docs/long-message-shamir.svg" alt="Shamir's key split" style="width: 400px;"/></a>
</div>

## Compilation
Program follows standard C++11 and can be compiled using command
```
$ make
```
in project directory.
It does not require any additional non-standard libraries.

## Basic usage
Program must be launched either with `-d` of `-m` argument, which determines whether secret will be distributed (`-d`) into shamir's shares or restored (merged, `-m`) from existing shamir's shares. When secret option `-d` is applied, also reconsturction threshold (`-t`) and total share count (`-n`) needs to be specified together with their values.
Threshold must be number in range 1 .. 32, total share count must be greater or equal to the threshold but at most 32.
```
$ ./shamir -d -t 3 -n 5
$ ./shamir -m
```
