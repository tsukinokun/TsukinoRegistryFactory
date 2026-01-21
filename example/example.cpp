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
