#include <DxLib.h>
#include "InputManager.h"

void InputManager::Init()
{
	// 入力判定処理の生成と初期化
	input_ = std::make_unique<Input>();
	input_->Init();

	// 入力状況に応じたトリガーの登録
	using BTN = Input::JOYPAD_BTN;
	using STICK = Input::JOYPAD_STICK;
	using MOUSE = Input::MOUSE;

	// プレイヤー操作
	RegisterTrigger(TYPE::PLAYER_MOVE_RIGHT, { KEY_INPUT_D }, { }, STICK::L_STICK_RIGHT);
	RegisterTrigger(TYPE::PLAYER_MOVE_LEFT, { KEY_INPUT_A }, { }, STICK::L_STICK_LEFT);
	RegisterTrigger(TYPE::PLAYER_MOVE_UP, { KEY_INPUT_W }, { }, STICK::L_STICK_UP);
	RegisterTrigger(TYPE::PLAYER_MOVE_DOWN, { KEY_INPUT_S }, { }, STICK::L_STICK_DOWN);
	RegisterTrigger(TYPE::PLAYER_JUMP, { KEY_INPUT_SPACE }, { BTN::RB_RIGHT });
	RegisterTrigger(TYPE::PLAYER_DASH, { KEY_INPUT_LSHIFT }, { BTN::RB_DOWN });

	RegisterTrigger(TYPE::CAMERA_MOVE_RIGHT, { KEY_INPUT_RIGHT }, { }, STICK::R_STICK_RIGHT, MOUSE::MOVE_RIGHT);
	RegisterTrigger(TYPE::CAMERA_MOVE_LEFT, { KEY_INPUT_LEFT }, { }, STICK::R_STICK_LEFT, MOUSE::MOVE_LEFT);
	RegisterTrigger(TYPE::CAMERA_MOVE_UP, { KEY_INPUT_UP }, { }, STICK::R_STICK_UP, MOUSE::MOVE_UP);
	RegisterTrigger(TYPE::CAMERA_MOVE_DOWN, { KEY_INPUT_DOWN }, { }, STICK::R_STICK_DOWN, MOUSE::MOVE_DOWN);

	// メニュー操作
	RegisterTrigger(TYPE::SELECT_RIGHT, { KEY_INPUT_D }, { }, STICK::L_STICK_RIGHT);
	RegisterTrigger(TYPE::SELECT_LEFT, { KEY_INPUT_A }, { }, STICK::L_STICK_LEFT);
	RegisterTrigger(TYPE::SELECT_UP, { KEY_INPUT_W }, { }, STICK::L_STICK_UP);
	RegisterTrigger(TYPE::SELECT_DOWN, { KEY_INPUT_S }, { }, STICK::L_STICK_DOWN);
	RegisterTrigger(TYPE::SELECT_DECISION, { KEY_INPUT_SPACE }, { BTN::RB_RIGHT }, STICK::MAX, MOUSE::CLICK_LEFT);
	RegisterTrigger(TYPE::SELECT_CANCEL, { KEY_INPUT_BACK }, { BTN::RB_DOWN });
	RegisterTrigger(TYPE::PAUSE, { KEY_INPUT_BACK }, { BTN::SELECT });

	// タイトル説明
	RegisterTrigger(TYPE::EXPLANTION_SKIP, { KEY_INPUT_RETURN }, { BTN::RB_TOP });

	// 報告処理
	RegisterTrigger(TYPE::ANOMARY_REPORT, { KEY_INPUT_R }, { BTN::R_BUTTON, BTN::R_TRIGGER }, STICK::MAX, MOUSE::CLICK_LEFT);
	RegisterTrigger(TYPE::CAMERA_MODE_CHANGE, { KEY_INPUT_E }, { BTN::RB_TOP });

	// ライトの電源切り替え処理
	RegisterTrigger(TYPE::LIGHT_SWITCH, { KEY_INPUT_Q }, { BTN::RB_LEFT });

	// 操作説明を開く
	RegisterTrigger(TYPE::INPUT_EXPLANTION_OPEN, { KEY_INPUT_TAB }, { BTN::LB_DOWN });

	// デバッグ操作
	RegisterTrigger(TYPE::DEBUG_SCENE_CHANGE, { KEY_INPUT_RSHIFT }, { });
	RegisterTrigger(TYPE::DEBUG_CAMERA_CHANGE, { KEY_INPUT_TAB }, { });
	RegisterTrigger(TYPE::CREATE_POSITION, { KEY_INPUT_C }, { });
	RegisterTrigger(TYPE::OPEN_FILE, { KEY_INPUT_V }, { });
}

void InputManager::Update()
{
	input_->Update();
}

void InputManager::Release()
{
	triggerMap_.clear();
	funcNewMap_.clear();
	funcTrgDownMap_.clear();
	funcTrgUpMap_.clear();
	input_->Release();
}

bool InputManager::IsNew(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto& func : funcNewMap_[type])
	{
		if (func(type, padNo))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgDown(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto& func : funcTrgDownMap_[type])
	{
		if (func(type, padNo))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgUp(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto& func : funcTrgUpMap_[type])
	{
		if (func(type, padNo))
		{
			return true;
		}
	}
	return false;
}

void InputManager::SetMousePos(const Vector2& pos)
{
	input_->SetMousePos(pos);
}

Vector2 InputManager::GetMousePos() const
{
	return input_->GetMousePos();
}

Vector2 InputManager::GetMouseMove() const
{
	return input_->GetMousePosDistance();
}

Vector2 InputManager::GetKnockLStickSize(Input::JOYPAD_NO no) const
{
	auto padInfo = input_->GetJPadInputState(no);
	return Vector2(padInfo.AKeyLX, padInfo.AKeyLY);
}

Vector2 InputManager::GetKnockRStickSize(Input::JOYPAD_NO no) const
{
	auto padInfo = input_->GetJPadInputState(no);
	return Vector2(padInfo.AKeyRX, padInfo.AKeyRY);
}

void InputManager::RegisterTrigger(const TYPE type, const std::vector<int> keys, const std::vector<Input::JOYPAD_BTN> padButtons, const Input::JOYPAD_STICK padStick, const Input::MOUSE mouse)
{
	// トリガーの情報を設定
	TriggerInfo info = { keys, padButtons, padStick,mouse };

	// 情報の格納
	triggerMap_[type] = info;

	// 各種処理の配列
	std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> newFuncs;
	std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> isTrgDownFuncs;
	std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> isTrgUpFuncs;

	// キーが登録されている場合
	if (!info.keys.empty())
	{
		newFuncs.push_back([this](TYPE t, Input::JOYPAD_NO) { return IsNewKey(t); });
		isTrgDownFuncs.push_back([this](TYPE t, Input::JOYPAD_NO) { return IsTrgDownKey(t); });
		isTrgUpFuncs.push_back([this](TYPE t, Input::JOYPAD_NO) { return IsTrgUpKey(t); });

	}
	// パッドのボタンが登録されている場合
	if (!info.padButtons.empty())
	{
		newFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsNewPadButton(t, padNo); });
		isTrgDownFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgDownPadButton(t, padNo); });
		isTrgUpFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgUpPadButton(t, padNo); });
	}
	// パッドのスティックが登録されている場合
	if (info.padStick != Input::JOYPAD_STICK::MAX)
	{
		newFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsNewPadStick(t, padNo); });
		isTrgDownFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgDownPadStick(t, padNo); });
		isTrgUpFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgUpPadStick(t, padNo); });
	}
	// マウスの登録がされている場合
	if (info.mouse != Input::MOUSE::MAX)
	{
		newFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsNewMouse(t); });
		isTrgDownFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgDownMouse(t); });
		isTrgUpFuncs.push_back([this](TYPE t, Input::JOYPAD_NO padNo) { return IsTrgUpMouse(t); });
	}

	// 処理の登録
	RegisterTriggerFunction(type, newFuncs, isTrgDownFuncs, isTrgUpFuncs);
}

void InputManager::RegisterTriggerFunction(const TYPE type, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> newFuncs, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgDownFuncs, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgUpFuncs)
{
	funcNewMap_.emplace(type, newFuncs);
	funcTrgDownMap_.emplace(type, trgDownFuncs);
	funcTrgUpMap_.emplace(type, trgUpFuncs);
}

bool InputManager::IsNewKey(const TYPE type)
{
	for (int key : triggerMap_[type].keys)
	{
		if (input_->IsNew(key))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgDownKey(const TYPE type)
{
	for (int key : triggerMap_[type].keys)
	{
		if (input_->IsTrgDown(key))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgUpKey(const TYPE type)
{
	for (int key : triggerMap_[type].keys)
	{
		if (input_->IsTrgUp(key))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsNewPadButton(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto button : triggerMap_[type].padButtons)
	{
		if (input_->IsPadBtnNew(padNo, button))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgDownPadButton(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto button : triggerMap_[type].padButtons)
	{
		if (input_->IsPadBtnTrgDown(padNo, button))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsTrgUpPadButton(const TYPE type, const Input::JOYPAD_NO padNo)
{
	for (auto button : triggerMap_[type].padButtons)
	{
		if (input_->IsPadBtnTrgUp(padNo, button))
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsNewPadStick(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsStickNew(padNo, triggerMap_[type].padStick);
}

bool InputManager::IsTrgDownPadStick(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsStickDown(padNo, triggerMap_[type].padStick);
}

bool InputManager::IsTrgUpPadStick(const TYPE type, const Input::JOYPAD_NO padNo)
{
	return input_->IsStickUp(padNo, triggerMap_[type].padStick);
}

bool InputManager::IsNewMouse(const TYPE type)
{
	return input_->IsMouseNew(triggerMap_[type].mouse);
}

bool InputManager::IsTrgDownMouse(const TYPE type)
{
	return input_->IsMouseTrgDown(triggerMap_[type].mouse);
}

bool InputManager::IsTrgUpMouse(const TYPE type)
{
	return input_->IsMouseTrgUp(triggerMap_[type].mouse);
}

InputManager::InputManager()
{
	input_ = nullptr;
	triggerMap_.clear();
	funcNewMap_.clear();
	funcTrgDownMap_.clear();
	funcTrgUpMap_.clear();
}

InputManager::~InputManager()
{

}