
//Create object
template <typename T>
std::weak_ptr<T> ObjectManager::CreateObject() {
	assert((CanCreateObject() && "Pool full, can't manage any more objects"));
	static_assert((std::is_base_of<Object, T>::value && "Type mismatch while creating object"));

	auto newObject = std::make_shared<T>();

	AddToObjectList(newObject);

	return newObject;
}

template<typename T>
std::weak_ptr<T> ObjectManager::CreateGameEntity(const sf::Vector2f& Pos) {
	static_assert((std::is_base_of<GameEntity, T>::value && "Type mismatch while creating GameEntity"));

	std::weak_ptr<T> NewEntity = CreateObject<T>();
	const std::shared_ptr<GameEntity> Casted = std::static_pointer_cast<GameEntity>(NewEntity.lock());

	Casted->SetPosition(Pos);

	//add to update & render priority lists
	UpdateEntities.insert(NewEntity);
	RenderEntities.insert(NewEntity);

	System::GetInstance()->Event_EntityCreated.Invoke(NewEntity);

	return std::weak_ptr<T>(NewEntity);
}

template<typename T>
std::weak_ptr<T> ObjectManager::CreateComponent(GameEntity* OwningEntity) {
	static_assert((std::is_base_of<Component, T>::value && "Type mismatch while creating Component"));

	std::weak_ptr<T> WeakCmp = CreateObject<T>();

	auto OwningObject = GetObjectByIndex(OwningEntity->GetObjectIndex()).lock();
	auto entity = std::dynamic_pointer_cast<GameEntity>(OwningObject);

	std::shared_ptr<Component> casted = std::static_pointer_cast<Component>(WeakCmp.lock());
	casted->SetOwningEntity(entity);

	if (auto scene = std::dynamic_pointer_cast<SceneComponent>(casted)) {
		scene->SetWorldPosition(entity->GetPosition());
	}
	return WeakCmp;
}