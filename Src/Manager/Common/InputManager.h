#pragma once
#include <map>
#include <functional>
#include <vector>
#include "../../Template/Singleton.h"
#include "../../Common/Vector2.h"
#include "Input.h"

class InputManager : public Singleton<InputManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<InputManager>;

public:

	/// <summary>
	/// 入力状況の種類
	/// </summary>
	enum class TYPE
	{
		NONE,

		PLAYER_MOVE_RIGHT,		// 移動右
		PLAYER_MOVE_LEFT,		// 移動左
		PLAYER_MOVE_UP,			// 移動上
		PLAYER_MOVE_DOWN,		// 移動下
		PLAYER_JUMP,			// ジャンプ
		PLAYER_DASH,			// ダッシュ

		CAMERA_MOVE_RIGHT,		// カメラ右移動
		CAMERA_MOVE_LEFT,		// カメラ左移動
		CAMERA_MOVE_UP,			// カメラ上移動
		CAMERA_MOVE_DOWN,		// カメラ下移動

		SELECT_RIGHT,			// 選択右
		SELECT_LEFT,			// 選択左
		SELECT_DOWN,			// 選択下
		SELECT_UP,				// 選択上
		SELECT_DECISION,		// 選択決定
		SELECT_CANCEL,			// 選択キャンセル

		PAUSE,					// ポーズ(開閉)

		INPUT_EXPLANTION_OPEN,	// 操作説明を開く

		ANOMARY_REPORT,			// 異変の報告

		CAMERA_MODE_CHANGE,		// カメラモード切替

		LIGHT_SWITCH,			// ライトのON/OFF

		EXPLANTION_SKIP,		// 説明のスキップ

		DEBUG_SCENE_CHANGE,		// デバッグシーン遷移
		DEBUG_CAMERA_CHANGE,	// デバッグカメラビュー変更
		CREATE_POSITION,		// 位置リストの生成
		OPEN_FILE,				// ファイルを開く
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// リソースの破棄
	/// </summary>
	void Release();

	/// <summary>
	/// 指定した状況での各入力機器の押下判定
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合入力中,falseの場合未入力</returns>
	bool IsNew(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::PAD1);

	/// <summary>
	/// 指定した状況での各入力機器の押下判定(最初のみ)
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合入力中,falseの場合未入力</returns>
	bool IsTrgDown(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::PAD1);

	/// <summary>
	/// 指定した状況での各入力機器の離した判定
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合離したタイミング,falseの場合まだ押下中</returns>
	bool IsTrgUp(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::PAD1);

	/// <summary>
	/// マウス位置を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetMousePos(const Vector2& pos);

	/// <summary>
	/// マウス座標の取得
	/// </summary>
	/// <returns>マウス座標の取得</returns>
	Vector2 GetMousePos() const;

	/// <summary>
	/// マウスの移動量を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>マウスの移動量</returns>
	Vector2 GetMouseMove() const;

	/// <summary>
	/// 左スティックの倒れた度合いを取得
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <returns>倒れ具合を渡す</returns>
	Vector2 GetKnockLStickSize(Input::JOYPAD_NO no = Input::JOYPAD_NO::PAD1) const;
	
	/// <summary>
	/// 右スティックの倒れた度合いを取得
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <returns>倒れた具合を渡す</returns>
	Vector2 GetKnockRStickSize(Input::JOYPAD_NO no = Input::JOYPAD_NO::PAD1) const;

private:

	// 入力トリガーの情報
	struct TriggerInfo
	{
		std::vector<int> keys;
		std::vector<Input::JOYPAD_BTN> padButtons;
		Input::JOYPAD_STICK padStick = Input::JOYPAD_STICK::MAX;
		Input::MOUSE mouse = Input::MOUSE::MAX;
	};

	// 入力判定クラス
	std::unique_ptr<Input> input_;

	// 入力状況別のトリガーを管理するマップ
	std::map<TYPE, TriggerInfo> triggerMap_;

	// 入力状況に応じた押下処理を管理するマップ
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcNewMap_;
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcTrgDownMap_;
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcTrgUpMap_;

	// 入力状況に応じたトリガーを登録
	void RegisterTrigger(
		const TYPE type, 
		const std::vector<int> keys,
		const std::vector<Input::JOYPAD_BTN> padButtons,
		const Input::JOYPAD_STICK padStick = Input::JOYPAD_STICK::MAX,
		const Input::MOUSE mouse = Input::MOUSE::MAX);

	// 処理の登録
	void RegisterTriggerFunction(
		const InputManager::TYPE type, 
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> newFuncs,
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgDownFuncs,
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgUpFuncs);

	// キー関係の入力判定
	bool IsNewKey(const TYPE type);
	bool IsTrgDownKey(const TYPE type);
	bool IsTrgUpKey(const TYPE type);

	// パッドボタンの入力判定
	bool IsNewPadButton(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgDownPadButton(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgUpPadButton(const TYPE type, const Input::JOYPAD_NO padNo);

	// パッドスティックの入力判定
	bool IsNewPadStick(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgDownPadStick(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgUpPadStick(const TYPE type, const Input::JOYPAD_NO padNo);

	// マウスの入力判定
	bool IsNewMouse(const TYPE type);
	bool IsTrgDownMouse(const TYPE type);
	bool IsTrgUpMouse(const TYPE type);

	// コンストラクタ
	InputManager();

	// デストラクタ
	~InputManager();
};