set(PoolingSources
	APoolable.cpp
	WeakPointerObjectPool.cpp
)
set(PoolingHeaders
	APoolable.h
	WeakPointerObjectPool.h
)

target_sources(Asteroids PRIVATE ${PoolingSources} ${PoolingHeaders})
