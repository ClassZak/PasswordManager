# PasswordManager

Simple password manager for passwords storing.

## 🛠 Dependencies

1. inih https://github.com/benhoyt/inih
2. OpenSSL https://github.com/openssl/openssl

## 🖼️ Program appearance

<p align="center">
	<img align="center" src="https://github.com/ClassZak/PasswordManager/blob/master/screenshot1.png"/>
</p>
<p align="center">
	<img align="center" src="https://github.com/ClassZak/PasswordManager/blob/master/screenshot2.png"/>
</p>

## 📰 Config file

WARNING!!!
The actual expample of program configuration is stored in config\_example.ini

```ini
[app]
key_file_path=key.bin
iv_file_path=iv.bin
```

key\_file\_path is the path of chipher key
iv\_file\_path is the path of chipher iv

Both are required to start the proram

## 🏗 🚀 Building and launching

1. Clone and build

```bash
git clone https://github.com/ClassZak/PasswordManager
cd PasswordManager
cmake --preset=<chosen_by_you_preset>
cmake --build build/
```

Actual presets you can see in CMakePresets.txt

2. Create config file and chipher files

```bash
cp config_example.ini config.ini
touch key.bin
touch iv.bin
```

