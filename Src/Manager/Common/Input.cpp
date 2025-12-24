#include <DxLib.h>
#include "Input.h"

Input::Input(void)
{
	mouseInput_ = -1;	
	
	//キーの登録
	Add(KEY_INPUT_SPACE);
	Add(KEY_INPUT_N);
	Add(KEY_INPUT_Z);
	Add(KEY_INPUT_W);
	Add(KEY_INPUT_A);
	Add(KEY_INPUT_S);
	Add(KEY_INPUT_D);
	Add(KEY_INPUT_LSHIFT);
	Add(KEY_INPUT_RSHIFT);
	Add(KEY_INPUT_E);
	Add(KEY_INPUT_R);
	Add(KEY_INPUT_Q);
	Add(KEY_INPUT_P);
	Add(KEY_INPUT_DOWN);
	Add(KEY_INPUT_UP);
	Add(KEY_INPUT_RIGHT);
	Add(KEY_INPUT_LEFT);
	Add(KEY_INPUT_RETURN);
	Add(KEY_INPUT_TAB);
	Add(KEY_INPUT_C);
	Add(KEY_INPUT_V);
	Add(KEY_INPUT_BACK);
}

Input::~Input(void)
{
}

void Input::Init(void)
{
	Input::MouseInfo info;

	// マウス
	info = Input::MouseInfo();
	for (int i = 0; i < static_cast<int>(MOUSE::MAX); i++)
	{
		mouseInfos_.emplace(static_cast<MOUSE>(i), info);
	}

	//スティック
	Input::StickInfo stickInfo;
	stickInfo = Input::StickInfo();
	for (int padNo = 0; padNo <= static_cast<int>(JOYPAD_NO::PAD4); padNo++)
	{
		std::vector<StickInfo>sticks;
		for (int i = 0; i < static_cast<int>(JOYPAD_STICK::MAX); i++)
		{
			stickInfo.key = static_cast<JOYPAD_STICK>(i);
			sticks.push_back(stickInfo);
		}
		stickInfos_.emplace(static_cast<JOYPAD_NO>(padNo), sticks);
	}
}

void Input::Update(void)
{

	// キーボード検知
	for (auto& p : keyInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = CheckHitKey(p.second.key);
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// マウス検知
	mouseInput_ = GetMouseInput();
	mousePrePos_ = mousePos_;
	GetMousePoint(&mousePos_.x, &mousePos_.y);
	wheelRot_ = GetMouseWheelRotVol();

	for (auto& p : mouseInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		switch (p.first)
		{
		case MOUSE::CLICK_RIGHT:
			p.second.keyNew = (mouseInput_ & MOUSE_INPUT_RIGHT) != 0;
			break;
		case MOUSE::CLICK_LEFT:
			p.second.keyNew = (mouseInput_ & MOUSE_INPUT_LEFT) != 0;
			break;
		case MOUSE::MOVE_LEFT:
			p.second.keyNew = mousePos_.x < mousePrePos_.x;
			break;
		case MOUSE::MOVE_RIGHT:
			p.second.keyNew = mousePos_.x > mousePrePos_.x;
			break;
		case MOUSE::MOVE_UP:
			p.second.keyNew = mousePos_.y < mousePrePos_.y;
			break;
		case MOUSE::MOVE_DOWN:
			p.second.keyNew = mousePos_.y > mousePrePos_.y;
			break;
		case MOUSE::WHEEL_FRONT:
			p.second.keyNew = (wheelRot_ > 0);
			break;
		case MOUSE::WHEEL_BACK:
			p.second.keyNew = (wheelRot_ < 0);
			break;
		case MOUSE::MAX:
			break;
		default:
			break;
		}
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// パッド情報
	SetJPadInState(JOYPAD_NO::KEY_PAD1);
	SetJPadInState(JOYPAD_NO::PAD1);
	SetJPadInState(JOYPAD_NO::PAD2);
	SetJPadInState(JOYPAD_NO::PAD3);
	SetJPadInState(JOYPAD_NO::PAD4);

	// スティック検知
	for (auto& stickInfo : stickInfos_)
	{
		for (auto& stick : stickInfo.second)
		{
			int overSize = PadStickOverSize(stickInfo.first, stick.key);
			stick.keyOld = stick.keyNew;
			stick.keyNew = overSize > STICK_THRESHOLD;
			stick.keyTrgDown = !stick.keyOld && stick.keyNew;
			stick.keyTrgUp = stick.keyOld && !stick.keyNew;
		}
	}
}

void Input::Release(void)
{
	keyInfos_.clear();
	mouseInfos_.clear();
	stickInfos_.clear();
}

void Input::Add(int key)
{
	Input::Info info = Input::Info();
	info.key = key;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	keyInfos_.emplace(key, info);
}

void Input::Clear(void)
{
	keyInfos_.clear();
}

bool Input::IsNew(int key) const
{
	return Find(key).keyNew;
}

bool Input::IsTrgDown(int key) const
{
	return Find(key).keyTrgDown;
}

bool Input::IsTrgUp(int key) const
{
	return Find(key).keyTrgUp;
}

void Input::SetMousePos(const Vector2& pos)
{
	mousePos_ = pos;
	mousePrePos_ = pos;
	SetMousePoint(static_cast<int>(mousePos_.x), static_cast<int>(mousePos_.y));
}

Vector2 Input::GetMousePos(void) const
{
	return mousePos_;
}

Vector2 Input::GetMousePosDistance(void) const
{
	return { mousePos_.x - mousePrePos_.x ,mousePos_.y - mousePrePos_.y };
}

int Input::GetMouse(void) const
{
	return mouseInput_;
}

bool Input::IsClickMouseLeft(void) const
{
	return mouseInput_ == MOUSE_INPUT_LEFT;
}

bool Input::IsClickMouseRight(void) const
{
	return mouseInput_ == MOUSE_INPUT_RIGHT;
}

bool Input::IsMouseNew(MOUSE mouse) const
{
	return FindMouse(mouse).keyNew;
}

bool Input::IsMouseTrgUp(MOUSE mouse) const
{
	return FindMouse(mouse).keyTrgUp;
}

bool Input::IsMouseTrgDown(MOUSE mouse) const
{
	return FindMouse(mouse).keyTrgDown;
}

const Input::Info& Input::Find(int key) const
{

	auto it = keyInfos_.find(key);
	if (it != keyInfos_.end())
	{
		return it->second;
	}

	return infoEmpty_;

}

const Input::MouseInfo& Input::FindMouse(MOUSE key) const
{
	auto it = mouseInfos_.find(key);
	if (it != mouseInfos_.end())
	{
		return it->second;
	}

	return mouseInfoEmpty_;
}

Input::JOYPAD_TYPE Input::GetJPadType(JOYPAD_NO no)
{
	return static_cast<Input::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no)));
}

DINPUT_JOYSTATE Input::GetJPadDInputState(JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
	return joyDInState_;
}

XINPUT_STATE Input::GetJPadXInputState(JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
	return joyXInState_;
}

void Input::SetJPadInState(JOYPAD_NO jpNo)
{

	int no = static_cast<int>(jpNo);
	auto stateNew = GetJPadInputState(jpNo);
	auto& stateNow = padInfos_[no];

	int max = static_cast<int>(JOYPAD_BTN::MAX);
	for (int i = 0; i < max; i++)
	{

		stateNow.ButtonsOld[i] = stateNow.ButtonsNew[i];
		stateNow.ButtonsNew[i] = stateNew.ButtonsNew[i];

		stateNow.IsOld[i] = stateNow.IsNew[i];
		//stateNow.IsNew[i] = stateNow.ButtonsNew[i] == 128 || stateNow.ButtonsNew[i] == 255;
		stateNow.IsNew[i] = stateNow.ButtonsNew[i] > 0;

		stateNow.IsTrgDown[i] = stateNow.IsNew[i] && !stateNow.IsOld[i];
		stateNow.IsTrgUp[i] = !stateNow.IsNew[i] && stateNow.IsOld[i];


		stateNow.AKeyLX = stateNew.AKeyLX;
		stateNow.AKeyLY = stateNew.AKeyLY;
		stateNow.AKeyRX = stateNew.AKeyRX;
		stateNow.AKeyRY = stateNew.AKeyRY;

	}

}

Input::JOYPAD_IN_STATE Input::GetJPadInputState(JOYPAD_NO no)
{

	JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

	auto type = GetJPadType(no);

	switch (type)
	{
	case Input::JOYPAD_TYPE::OTHER:
		break;
	case Input::JOYPAD_TYPE::XBOX_360:
	{
	}
	break;
	case Input::JOYPAD_TYPE::XBOX_ONE:
	{

		auto d = GetJPadDInputState(no);
		auto x = GetJPadXInputState(no);

		int idx;

		//   Y
		// X   B
		//   A



		idx = static_cast<int>(JOYPAD_BTN::RB_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// Y

		idx = static_cast<int>(JOYPAD_BTN::RB_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[2];// X

		idx = static_cast<int>(JOYPAD_BTN::RB_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[1];// B

		idx = static_cast<int>(JOYPAD_BTN::RB_DOWN);
		ret.ButtonsNew[idx] = d.Buttons[0];// A

		idx = static_cast<int>(JOYPAD_BTN::R_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[5];// R_BUTTON

		idx = static_cast<int>(JOYPAD_BTN::L_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[4]; // L_BUTTON

		idx = static_cast<int>(JOYPAD_BTN::START);
		ret.ButtonsNew[idx] = d.Buttons[7];// START

		idx = static_cast<int>(JOYPAD_BTN::SELECT);
		ret.ButtonsNew[idx] = d.Buttons[6]; // SELECT

		idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
		ret.ButtonsNew[idx] = x.RightTrigger;// R_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
		ret.ButtonsNew[idx] = x.LeftTrigger; // L_TRIGGER	

		idx = static_cast<int>(JOYPAD_BTN::LB_RIGHT);
		ret.ButtonsNew[idx] = x.Buttons[3];// →

		idx = static_cast<int>(JOYPAD_BTN::LB_LEFT);
		ret.ButtonsNew[idx] = x.Buttons[2];// ←

		idx = static_cast<int>(JOYPAD_BTN::LB_DOWN);
		ret.ButtonsNew[idx] = x.Buttons[1];// ↓

		idx = static_cast<int>(JOYPAD_BTN::LB_TOP);
		ret.ButtonsNew[idx] = x.Buttons[0];// ↑

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;

		// 右スティック
		ret.AKeyRX = d.Rx;
		ret.AKeyRY = d.Ry;

	}
	break;
	case Input::JOYPAD_TYPE::DUAL_SHOCK_4:
		break;
	case Input::JOYPAD_TYPE::DUAL_SENSE:
	{

		auto d = GetJPadDInputState(no);
		int idx;

		//   △
		// □  〇
		//   ×

		idx = static_cast<int>(JOYPAD_BTN::RB_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// Y

		idx = static_cast<int>(JOYPAD_BTN::RB_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[2];// X

		idx = static_cast<int>(JOYPAD_BTN::RB_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[1];// B

		idx = static_cast<int>(JOYPAD_BTN::RB_DOWN);
		ret.ButtonsNew[idx] = d.Buttons[0];// A

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;

		// 右スティック
		ret.AKeyRX = d.Z;
		ret.AKeyRY = d.Rz;

	}
	break;
	case Input::JOYPAD_TYPE::SWITCH_JOY_CON_L:
		break;
	case Input::JOYPAD_TYPE::SWITCH_JOY_CON_R:
		break;
	case Input::JOYPAD_TYPE::SWITCH_PRO_CTRL:
		break;
	case Input::JOYPAD_TYPE::MAX:
		break;
	}

	return ret;

}

int Input::PadStickOverSize(const JOYPAD_NO no, const JOYPAD_STICK stick)
{
	int ret = 0;
	auto padInfo = GetJPadInputState(no);
	switch (stick)
	{
	case JOYPAD_STICK::L_STICK_UP:
		ret = padInfo.AKeyLY < 0 ? padInfo.AKeyLY : 0;
		break;
	case JOYPAD_STICK::L_STICK_DOWN:
		ret = padInfo.AKeyLY > 0 ? padInfo.AKeyLY : 0;
		break;
	case JOYPAD_STICK::L_STICK_LEFT:
		ret = padInfo.AKeyLX < 0 ? padInfo.AKeyLX : 0;
		break;
	case JOYPAD_STICK::L_STICK_RIGHT:
		ret = padInfo.AKeyLX > 0 ? padInfo.AKeyLX : 0;
		break;
	case JOYPAD_STICK::R_STICK_UP:
		ret = padInfo.AKeyRY < 0 ? padInfo.AKeyRY : 0;
		break;
	case JOYPAD_STICK::R_STICK_DOWN:
		ret = padInfo.AKeyRY > 0 ? padInfo.AKeyRY : 0;
		break;
	case JOYPAD_STICK::R_STICK_LEFT:
		ret = padInfo.AKeyRX < 0 ? padInfo.AKeyRX : 0;
		break;
	case JOYPAD_STICK::R_STICK_RIGHT:
		ret = padInfo.AKeyRX > 0 ? padInfo.AKeyRX : 0;
		break;
	case JOYPAD_STICK::MAX:
		break;
	default:
		break;
	}
	return abs(ret);
}

bool Input::IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsNew[static_cast<int>(btn)];
}

bool Input::IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)];
}

bool Input::IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgUp[static_cast<int>(btn)];
}

bool Input::IsStickNew(JOYPAD_NO no, JOYPAD_STICK stick) const
{
	for (auto& stickInfo : stickInfos_)
	{
		if (stickInfo.first != no)
		{
			continue;
		}
		for (auto& stickI : stickInfo.second)
		{
			if (stickI.key != stick)
			{
				continue;
			}
			return stickI.keyNew;
		}
	}
	return false;
}

bool Input::IsStickDown(JOYPAD_NO no, JOYPAD_STICK stick) const
{
	for (auto& stickInfo : stickInfos_)
	{
		if (stickInfo.first != no)
		{
			continue;
		}
		for (auto& stickI : stickInfo.second)
		{
			if (stickI.key != stick)
			{
				continue;
			}
			return stickI.keyTrgDown;
		}
	}
	return false;
}

bool Input::IsStickUp(JOYPAD_NO no, JOYPAD_STICK stick) const
{
	for (auto& stickInfo : stickInfos_)
	{
		if (stickInfo.first != no)
		{
			continue;
		}
		for (auto& stickI : stickInfo.second)
		{
			if (stickI.key != stick)
			{
				continue;
			}
			return stickI.keyTrgUp;
		}
	}
	return false;
}