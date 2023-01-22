
# RISC-V simulator written in C++

# Prerequisites
## Building googletest
To succesfully run test, build googletest submodule.
```shell
cd third-party/googletest/
mkdir build
cd build
cmake ..
make
sudo make install
```


## GPG key setup
You can skip following step if you already have gpg key on your machine.

Create gpg key locally (follow the CLI instructions)
```shell
gpg --full-generate-key
```
Get public key in following command output
```shell
gpg --list-keys
```
Run `gpg --armor --export <KEY>`
Add it to GitHub (User/Settings/SSH and GPG keys)
Try to commit: `git commit -S -m "Test message"

Enable commits autosign
```shell
git config commit.gpgsign true
```

# Cloning repo
Project includes submodules, so you need to clone them as well.
```shell
git clone --recursive git@github.com:K3S-prod/risc-v-simulator.git
cd risc-v-simulator
git submodule update --init --recursive
```
# Build
```shell
mkdir build && cd build
cmake ..
make
```

# Run
Running tests:
```shell
cd build
./bin/tests
```

Running simulator:
```shell
cd build
./bin/simulator <elf_filename>

# Example output:
$ cd build
$ ./bin/simulator ./bin/fibbonacci_riscv
(rvsim) c
[RUNTIME] Program exited with code: 0
[RUNTIME] Exec time: 0.202988
[RUNTIME] Insts executed: 4253
```
