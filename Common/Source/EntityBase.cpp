#include "EntityBase.h"

EntityBase::EntityBase() 
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, vel(0.0f, 0.0f, 0.0f)
	, isDone(false)
	, m_bCollider(false)
	, health(100)
	, damage(0)
	, isPlayerCreated(false)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::Update(double _dt)
{
	position += vel * static_cast<float>(_dt);
	if (health <= 0)
	{

		SetIsDone(true);
	}
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}

// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool _value)
{
	m_bCollider = _value;
}

void EntityBase::CollisionResponse(EntityBase* other)
{
	//SetIsDone(true);
	vel += other->vel * ((float)other->damage / (float)100);

}