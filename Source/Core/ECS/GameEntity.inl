#pragma once

#include "ObjectManager.h"

// Component Template definitions
template<typename T>
std::weak_ptr<T> GameEntity::AddComponent()
{
    std::weak_ptr<T> WeakCmp = System::GetInstance()->GetObjectMgr()->CreateComponent<T>(this);

    components_.push_back(WeakCmp);
    return WeakCmp;
}

template<typename T> std::weak_ptr<T> GameEntity::GetComponentOfType()
{
    static_assert((std::is_base_of<Component, T>::value && "Type is not a component type"));

    for (auto& Component : components_)
    {
        if (auto shared = Component.lock())
        {
            if (std::shared_ptr<T> CompPtr = std::dynamic_pointer_cast<T>(shared))
            {
                return std::weak_ptr<T>(CompPtr);
            }
        }
    }
    return std::weak_ptr<T>();
}

template<typename T>
void GameEntity::GetComponentsOfType(std::vector<std::weak_ptr<T>>& outResult)
{
    static_assert((std::is_base_of<Component, T>::value && "Type is not a component type"));

    for (auto& comp : components_)
    {
        if (auto shared = comp.lock())
        {
            if (std::shared_ptr<T> compPtr = std::dynamic_pointer_cast<T>(shared))
            {
                outResult.push_back(compPtr);
            }
        }
    }
}

template<typename T>
inline void GameEntity::DestroyComponentOfType()
{
    DestroyComponentsOfTypeInternal_<T>(true);
}

template<typename T>
inline void GameEntity::DestroyAllComponentsOfType()
{
    if (std::is_same<T, Collider>::value)
    {
        bHasColliders_ = false;
    }
    DestroyComponentsOfTypeInternal_<T>(false);
}

template<typename T>
inline void GameEntity::DestroyComponentsOfTypeInternal_(const bool onlyOne)
{
    static_assert((std::is_base_of<Component, T>::value && "Type mismatch while destroying object"));

    int i = 0;
    for (auto& comp : components_)
    {
        auto component = comp.lock();
        if (std::dynamic_pointer_cast<T>(component))
        {
            System::GetInstance()->GetObjectMgr()->DestroyObject(component.get());
            components_.erase(components_.begin() + i);
            if (onlyOne)
            {
                if (static_cast<int>(components_.size()) == 1 && std::is_same<T, Collider>::value)
                {
                    bHasColliders_ = false;
                }
                return;
            }
        }
        i++;
    }
}

template<typename T, typename ... Types> std::weak_ptr<T> GameEntity::CreateDrawable(Types... args)
{
    static_assert((std::is_base_of<sf::Drawable, T>::value && "Type is not a drawable type"));
    std::shared_ptr<T> Drawable = std::make_shared<T>(args...);
    drawables_.push_back(Drawable);
    return Drawable;
}

template<typename T>
std::weak_ptr<T> GameEntity::GetDrawableOfType()
{
    static_assert((std::is_base_of<sf::Drawable, T>::value && "Type is not a drawable type"));

    for (auto& Drawable : drawables_)
    {
        //if (auto shared = Component)
        {
            if (std::shared_ptr<T> DrawPtr = std::dynamic_pointer_cast<T>(Drawable))
            {
                return std::weak_ptr<T>(DrawPtr);
            }
        }
    }
    return std::weak_ptr<T>();
}
//End Component Template definitions
