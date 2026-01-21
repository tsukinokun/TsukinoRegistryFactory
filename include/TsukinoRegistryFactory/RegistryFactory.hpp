//-------------------------------------------------------------
//! @file   RegistryFactory.hpp
//! @brief  自動登録型ファクトリのヘッダーオンリーライブラリ
//! @author 山﨑愛 ( Qiita:tsukino_   github:tsukino)
//-------------------------------------------------------------
#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <stdexcept>
//名前空間 TsukinoRegistryFactory
namespace TsukinoRegistryFactory
{
	//-------------------------------------------------------------
	//! @class     RegistryFactory
	//! @brief	   自動登録型ファクトリクラス
	//! @tparam    Base 基底クラス型
	//! @note	   登録と生成を静的メソッドで提供
	//! @attention Baseには仮想デストラクタが必要
	//! @attention 派生クラスにはデフォルトコンストラクタが必要
	//-------------------------------------------------------------
	template <typename Base>
	class RegistryFactory {
	public:
		using Creator = std::function<std::unique_ptr<Base>()>;				// 生成関数の型エイリアス

		//-------------------------------------------------------------
		//! @brief     クラス登録関数
		//! @param     name    クラス名
		//! @param     creator 生成関数
		//! @return    登録成功ならtrue
		//! @note	   登録マップにクラス名と生成関数を登録
		//! @attention 同じクラス名の重複登録は例外処理で検出
		//-------------------------------------------------------------
		static bool registerClass(const std::string& name, Creator&& creator) {
			auto& reg = registry();
			if (reg.count(name)) {
				// 重複登録エラー
				throw std::runtime_error("RegistryFactory: Duplicate registration for key: " + name);
			}
			// 登録
			reg.emplace(name, std::move(creator));
			// 登録成功
			return true;
		}


		//-------------------------------------------------------------
		//! @brief クラス生成関数
		//! @param name クラス名
		//! @return 生成されたクラスのポインタ、存在しない場合はnullptr
		//! @note 登録されているクラス名から対応するクラスを生成
		//-------------------------------------------------------------
		static std::unique_ptr<Base> create(const std::string& name)noexcept {
			auto& reg = registry();	  // 登録マップ取得
			auto it = reg.find(name); // クラス名で検索
			if (it != reg.end()) {	  // 見つかった場合
				return it->second();  // 生成関数を呼び出してインスタンス生成
			}
			return nullptr;
		}

	private:
		//-------------------------------------------------------------
		//! @brief  クラス登録用の静的マップ取得関数
		//! @return クラス登録マップの参照
		//! @note	マップはキーがクラス名、値が生成関数
		//-------------------------------------------------------------
		static std::unordered_map<std::string, Creator>& registry() noexcept {
			static std::unordered_map<std::string, Creator> instance;
			return instance;
		}
	};
}

//-------------------------------------------------------------
//! @brief クラス登録マクロ
//! @param KEY   クラス名（文字列）
//! @param TYPE  登録するクラス型
//! @param BASE  基底クラス型
//! @note このマクロをクラス実装ファイル内で使用することで、自動的にクラス登録が行われる
//-------------------------------------------------------------
#define TSUKINO_REGISTER_CLASS(KEY, TYPE, BASE) \
    namespace { \
        const bool registered_##TYPE = \
            TsukinoRegistryFactory::RegistryFactory<BASE>::registerClass(KEY, [](){ \
                return std::make_unique<TYPE>(); \
            }); \
    }