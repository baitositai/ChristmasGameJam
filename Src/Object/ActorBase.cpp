#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "ActorBase.h"

ActorBase::ActorBase() :
	scnMng_(SceneManager::GetInstance()),
	sndMng_(SoundManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	inputMng_(InputManager::GetInstance())
{

}

ActorBase::~ActorBase()
{

}

void ActorBase::Init()
{

}

void ActorBase::Update()
{

}

void ActorBase::Draw()
{

}

const Transform& ActorBase::GetTransform() const
{
	return transform_;
}