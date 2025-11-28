# PasswordManager

Simple password manager for storing encrypted password by OpenSSL library using cipher with key and iv.

This program uses AES-256-CBC cipher.

**⚠️ ATTENTION!!!**

Check the sizes of the key file and iv file to ensure you are using the correct cipher.

## 🛠 Dependencies

1. inih https://github.com/benhoyt/inih
2. OpenSSL https://github.com/openssl/openssl

## ⚙️ Options

Run `PasswordManager -h` or `PasswordManager --help` for show help
Run `PasswordManager -v` or `PasswordManager --version` for show version

## 🖼️ Program appearance

<p align="center">
	<img align="center" src="https://github.com/ClassZak/PasswordManager/blob/master/screenshot1.png"/>
</p>
<p align="center">
	<img align="center" src="https://github.com/ClassZak/PasswordManager/blob/master/screenshot2.png"/>
</p>

## 📰 Config file

**⚠️ ATTENTION!!!**

The actual expample of program configuration is stored in config\_example.ini

```ini
[app]
key_file_path=key.bin
iv_file_path=iv.bin
```

- key\_file\_path is the path of chipher key
- iv\_file\_path is the path of chipher iv

Both are required to start the proram

## 🏗 🚀 Building and launching

1. Copy the necessary files to the deps/ directory. All necessary information is located in deps/deps.txt

2. Clone and build

```bash
git clone https://github.com/ClassZak/PasswordManager
cd PasswordManager
cmake --preset=<chosen_by_you_preset>
cmake --build build/
```

Actual presets you can see in CMakePresets.txt

3. Create config file and chipher files

```bash
cp config_example.ini config.ini
touch key.bin
touch iv.bin
```

4. Launch

```bash
./build/PasswordManager
```

