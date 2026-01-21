![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Header-only](https://img.shields.io/badge/header--only-yes-brightgreen)
![Dependencies](https://img.shields.io/badge/dependencies-none-brightgreen)
![GitHub Repo stars](https://img.shields.io/github/stars/tsukinokun/TsukinoRegistryFactory)
# TsukinoRegistryFactory
TsukinoRegistryFactory は、**文字列キーから任意のクラスを自動生成できる軽量ヘッダーオンリーの C++ ライブラリ**です。
クラスをマクロで登録するだけで、静的初期化のタイミングで自動的にファクトリへ登録され、`RegistryFactory<Base>::create("キー名")` を呼び出すだけで対応する派生クラスのインスタンスを生成できます。
- if/switch の羅列を避けたい
- Factoryを簡潔に使いたい
といった場面で特に効果を発揮します。

本ライブラリは **依存なし・ヘッダーオンリー・C++17 対応** のため、 `include/` をインクルードパスに追加するだけで簡単に導入できます。

## Features

- 🔹 **Header-only**  
  `#include` するだけで使える軽量設計。

- 🔹 **Automatic class registration**  
  マクロを使うだけでクラスが自動登録され、追加が容易。

- 🔹 **String-based factory creation**  
  `"print"` のような文字列からクラスを生成可能。

- 🔹 **No dependencies**  
  標準ライブラリのみで動作。

- 🔹 **Extensible design**  
  プラグイン的な拡張やコマンドシステムに最適。

## Installation

TsukinoRegistryFactory はヘッダーオンリーのため、ビルドやインストール作業は不要です。  
本リポジトリを clone し、`include/` ディレクトリをインクルードパスに追加するだけで使用できます。

### g++ / clang++
```bash
g++ -std=c++17 -I path/to/TsukinoRegistryFactory/include your_code.cpp
```
追加の依存ライブラリはありません。

## Example
以下は、文字列コマンドからクラスを生成して実行する簡単な例です。
```cpp
//-------------------------------------------------------------
//! @file	example.cpp
//! @brief  TsukinoRegistryFactoryの使用例
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//-------------------------------------------------------------
#include <iostream>
#include "TsukinoRegistryFactory/RegistryFactory.hpp"
//-------------------------------------------------------------
//! @class     Command
//! @brief	   コマンド基底クラス
//! @note      execute()を派生クラスで実装する
//! @attention 仮想デストラクタを持つ
//-------------------------------------------------------------
struct Command {
	//-------------------------------------------------------------
	//! @brief    仮想デストラクタ
	//-------------------------------------------------------------
	virtual ~Command() = default;

	//-------------------------------------------------------------
	//! @brief    コマンド実行関数
	//! @note     派生クラスで実装する
	//-------------------------------------------------------------
	virtual void execute() const = 0;
};

//-------------------------------------------------------------
//! @struct  PrintCommand
//! @brief   PrintCommandクラス
//! @note    Commandの派生クラス
//-------------------------------------------------------------
struct PrintCommand : Command {
	//-------------------------------------------------------------
	//! @brief    コマンド実行関数
	//! @note     PrintCommandの実装
	//-------------------------------------------------------------
	void execute() const override {
		std::cout << "PrintCommand executed\n";
	}
};
// マクロでファクトリに自動登録
TSUKINO_REGISTER_CLASS("print", PrintCommand, Command);

//-------------------------------------------------------------
//! @struct  ExitCommand
//! @brief   ExitCommandクラス
//! @note    Commandの派生クラス
//-------------------------------------------------------------
struct ExitCommand : Command {
	//-------------------------------------------------------------
	//! @brief    コマンド実行関数
	//! @note     ExitCommandの実装
	//-------------------------------------------------------------
	void execute() const override {
		std::cout << "ExitCommand executed\n";
	}
};
// マクロでファクトリに自動登録
TSUKINO_REGISTER_CLASS("exit", ExitCommand, Command);

//-------------------------------------------------------------
//! @brief    メイン関数
//! @return   TsukinoRegistryFactoryの使い方サンプルコード
//! @note     コマンド入力に応じて対応するCommandを生成・実行する例
//-------------------------------------------------------------
int main() {

	while (true) {
		// ユーザからコマンド入力を受け取る
		std::cout << "Enter command (print / exit): ";
		std::string cmd;
		std::cin >> cmd;

		// コマンドに対応するCommandオブジェクトを生成
		auto cmdObj = TsukinoRegistryFactory::RegistryFactory<Command>::create(cmd);
		// コマンドが存在しない場合の処理
		if (!cmdObj) {
			std::cout << "Unknown command\n";
			continue;
		}

		// コマンドがあれば生成したコマンドを実行
		cmdObj->execute();

		// 終了コマンドならループを抜ける
		if (cmd == "exit") {
			break;
		}
	}
	// 終了
	return 0;
}
```

## Notes
- 基底クラス (`Base`) は **仮想デストラクタ** を持つ必要があります。
- 同じキー名を複数登録した場合、**例外がthrow**されます。
- 登録は静的初期化のタイミングで行われます。
- C++17 以降が必要です（`inline static` を使用）。

## Directory Structure
```
TsukinoRegistryFactory/
├── include/
│   └── TsukinoRegistryFactory/
│       └── RegistryFactory.hpp
├── example/
│   └── example.cpp
├── LICENSE
├── SECURITY.md
├── CMakeLists.txt
└── README.md
```