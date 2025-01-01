set(ParticleSysSources
	AParticleShape.cpp
	BakedParticleShape.cpp 
	LogicalParticleShape.cpp 
	ParticleSystem.cpp 
	ParticleSystemManager.cpp 
)
set(ParticleSysHeaders
	AParticleShape.h 
	BakedParticleShape.h 
	LogicalParticleShape.h 
	Particle.cpp Particle.h 
	ParticleSystem.h 
	ParticleSystemManager.h
)

target_sources(Asteroids PRIVATE ${ParticleSysSources} ${ParticleSysHeaders})
#target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source/ParticleSys)