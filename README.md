
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

# Build from sources
```shell
mkdir build && cd build
cmake ..
make
```
