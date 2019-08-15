#include "A10.h"

void A10::Primary()
{
	GameObject* bullet = GOManager::GetInstance()->fetchGO();
	bullet->type = GameObject::PLAYER_PROJECTILE_MACHINE;
	bullet->vel = vel;
	bullet->pos = pos;
	bullet->scale.Set(1, 2, 1);
	bullet->hasGravity = true;
	bullet->wrapMode = SW_CLEAR;
	for (int i = 0; i < MAX_TEXTURES; ++i)
		bullet->color[i] = color[i];
}

A10::A10()
{
}

A10::~A10()
{
}
