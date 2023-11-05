[![Build Status](https://github.com/penguin-teal/clhasher/actions/workflows/Build.yml/badge.svg)](https://github.com/penguin-teal/clhasher/actions/workflows/Build.yml)

# clhasher
CLHasher is a Linux command line utility to get non-cryptographic hashes
of strings or data.

```shell
clhasher <options> <algorithm flag> <input>
```

- [Installing](#installing)
- [Updating](#updating)
- [Uninstalling](#uninstalling)
- [Use](#use)
- [Algorithms](#algorithms)
- [Options](#options)
- [Escaping](#escaping)

## Installing

1. Clone the repository.
```shell
# Clone into directory 'clhasher'
git clone https://github.com/penguin-teal/clhasher.git clhasher
# Enter that directory
cd clhasher
# Go to specific release (v1.0.0 is latest)
git switch --detach v1.0.0
```

2. Build the project.
```shell
# Make a release build
make release
```

3. Install. To install there is a `make install` target and an optional `DESTDIR` variable.
```shell
# Install to /usr/local/bin: (ROOT is required)
sudo make install
# or to other dir:
# make install DESTDIR=/some/other/dir
```

## Updating

```shell
# Go to where you cloned the repository
cd clhasher
# Get the changes
git pull
# Switch to latest (v1.0.0 is latest)
git switch --detach v1.0.0
# Clean just to be safe then build release again
make clean release
# Install like you did before (ROOT is required)
sudo make install
```

## Uninstalling

To uninstall just `rm` the executable file:
```shell
# if you used make install: (ROOT is required)
rm /usr/local/bin/clhasher -i
# or if you installed using DESTDIR=*:
# rm /some/other/dir/clhasher -i
```

## Use

This creates a 64-bit hash using the FNV-1A algorithm:
```shell
clhasher --fnv1a-64 'My String'
```
Which prints:
```
9354639888599838980
```

You can also format the output better:
```shell
clhasher -ax --fnv1a-64 'My String'
```
Which prints:
```
u64 [0]:    0x81D25B6A69B17104
```

The `-a` option is for `--annotate` which produces a little more of a formatted
output, and `-x` is for `--hex` which prints the hash as a base-16 hexadecimal
number.

You can also split large integers into smaller ones:
```shell
clhasher -ax --fnv0-128 --split-64 'splitting'
```

```
u64 [0]:        0x2FE83015B8E8ABEC
u64 [8]:        0x12A4876D7700025E
```

It can also read from STDIN:
```shell
gcc --version | grep --color=never 'gcc' | clhasher -x --fnv1a-64
```

```
DBEF2942FEC1D09F
```

More examples:
```shell
# Hash entire file:
clhasher -x -i ./Makefile --fnv1-32
# 50C5D52

# Hash up to 20 chars from a file:
clhasher -x -i ./Makefile --fnv1a-32 -z 20
# 577B9B35

# Hash the NUL terminator:
clhasher -x0 --fnv1a-64 'TESTSTR'
# BEB7B33DE037BF70

# Use escape codes:
clhasher -xe --fnv1a-64 'line1\n\tline2'
# B2063D2905CD97E1
```

## Algorithms

Currently the following algorithms are supported:

- FNV-0: 32, 64, & 128 (`--fnv0-32`, `--fnv0-64`, `--fnv0-128`)
- FNV-1: 32, 64, & 128 (`--fnv1-32`, `--fnv1-64`, `--fnv1-128`)
- FNV-1A: 32, 64, & 128 (`--fnv1a-32`, `--fnv1a-64`, `--fnv1a-128`)

## Options

Here are all the options. You can also run `clhasher --help` for a similar list.

Usage:

```shell
clhasher <options> <algorithm option> <string>
```
Or omit `<string>` for STDIN:
```shell
echo 'hello' | clhasher <options> <algorithm option>
```

Miscellaneous:

- `-o <FILE>`/`--out=<FILE>` Print into a file. Defaults to `-` for STDOUT.
- `-i <FILE>`/`--in=<FILE>` Read from a file if no value argument is given. Defaults to `-` for STDIN.
- `-z <BYTES>`/`--length=<BYTES>` Number of bytes of input to hash. Defaults to `0` for automatic.
- `-0`/`--hash-nul` When length is automatic, includes the `\0` `NUL` to hash (by default does not hash the `NUL`).
- `-e`/`--escape` Allow escape sequences in the inputted string (see [Escaping](#escaping)).
- `-v`/`--verbose` Describe what is happening.
- `-?`/`--help` Print help list.
- `--usage` See usage message.
- `-V`/`--version` Print program version.

[Algorithms](#algorithms):

- `--fnv0-32` FNV-0 32-bit algorithm.
- `--fnv0-64` FNV-0 64-bit algorithm.
- `--fnv0-128` FNV-0 128-bit algorithm.
- `--fnv1-32` FNV-1 32-bit algorithm.
- `--fnv1-64` FNV-1 64-bit algorithm.
- `--fnv1-128` FNV-1 128-bit algorithm.
- `--fnv1a-32` FNV-1A 32-bit algorithm.
- `--fnv1a-64` FNV-1A 64-bit algorithm.
- `--fnv1a-128` FNV-1A 128-bit algorithm.

Output Formatting:

- `-a`/`--annotate` Print output with nicer formatting.
- `--hi-to-lo` When splitting the output, print the most significant part first (i.e. simulates big-endian on an integer scale).
- `-x`/`--hex` Print base-16 hexadecimal numbers.
- `--octal` Print base-8 octal numbers.
- `--base-10` Print base-10 "normal" numbers (default).

Splitting:

- `--split-32` Split hash output into one/multiple 32-bit numbers (e.g. 64-bit prints two 32-bit numbers).
- `--split-64` Split hash output into one/multiple 64-bit numbers (default).

## Escaping

You can enable escaping via `-e`. Enabling escaping means that the string _must_
end at a `NUL`, and therefore having a string containing `NUL`s like
`-z <SIZE>` allows will not work.

| Escape Code    | ASCII | Description |
|----------------|-------|-------------|
| `\n`           | `0A`  | New line    |
| `\r`           | `0D`  | Carriage return |
| `\t`           | `09`  | Tab         |
| `\\`           | `5C`  | Backslash   |
| `\0`           | `00`  | Nul*        |
| `\XXX`         |       | Octal Code* |
| `\oXXX`        |       | Octal Code* |
| `\xXX`         |       | Hex Code*   |

*If a `NUL` character exists the string will end there.