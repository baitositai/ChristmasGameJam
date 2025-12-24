#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "ControllerTextAnimation.h"

ControllerTextAnimation::ControllerTextAnimation(CharacterString& text, const float charPerFrame) :
	text_(text),
	charPerFrame_(charPerFrame),
	scnMng_(SceneManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
	textData_ = text_.string;
	step_ = 0.0f;
	charNum_ = 0;
	length_ = static_cast<int>(textData_.length());
}

ControllerTextAnimation::~ControllerTextAnimation()
{
}

void ControllerTextAnimation::Init()
{
	step_ = 0.0f;
	charNum_ = 0;
}

void ControllerTextAnimation::Update()
{
	// 文字数を達していた場合
	if (charPerFrame_ >= length_)
	{
		// 終了
		return;
	}

	// ステップの更新
	step_ += scnMng_.GetDeltaTime();

	// 目標のフレーム数に達していた場合
	if (step_ > charPerFrame_)
	{
		// 一文字増やす
		charNum_++;

		// ステップを初期化
		step_ = 0.0f;

	}
}

void ControllerTextAnimation::Draw()
{
	// 現在の表示すべき文字列を取得
	text_.string = textData_.substr(0, charNum_);

	// 空の場合終了
	if (text_.string.empty())
	{
		return;
	}

	// 中央ぞろえで描画
	text_.DrawCenter();
}

bool ControllerTextAnimation::IsEnd()
{
	return charNum_ >= length_;
}

void ControllerTextAnimation::Skip()
{
	charNum_ = length_;
	step_ = 0.0f;
}

void ControllerTextAnimation::SetCharacterString(CharacterString& text)
{
	text_ = text;
	textData_ = text_.string;
	length_ = static_cast<int>(textData_.length());
	Init();
}

void ControllerTextAnimation::SetCharaPerFrame(const float charPerFrame)
{
	charPerFrame_ = charPerFrame;
}