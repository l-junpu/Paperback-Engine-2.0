/**********************************************************************************
*\file         ParticleEmitter.h
*\brief        ParticleEmitter.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "glm/inc/gtc/random.hpp"

enum class ParticleSpawnPosition : int
{
	DEFAULT = 0				// Spawn In Center of BB
,	TOP						// Spawn at BB.Max.y
,	BOTTOM					// Spawn at BB.Min.y
};

struct Generate_Lifetime
{
	PPB_INLINE
	float Rand( void ) const noexcept
	{
		return glm::linearRand( m_Min, m_Max );
	}

	float m_Min = 1.0f;
	float m_Max = 5.0f;
};

struct Generate_Position
{
	PPB_INLINE
	paperback::Vector3f Rand( const transform&             EmitterTransform
						    , const boundingbox&           EmitterBB
							, const ParticleSpawnPosition& Status ) const noexcept
	{
		boundingbox Boundaries = EmitterBB.Extend( EmitterTransform.m_Position + EmitterTransform.m_Offset );

		if ( Status == ParticleSpawnPosition::DEFAULT )
			return paperback::Vector3f{ glm::linearRand( Boundaries.Min.x, Boundaries.Max.x )
									  , glm::linearRand( Boundaries.Min.y, Boundaries.Max.y )
									  , glm::linearRand( Boundaries.Min.z, Boundaries.Max.z ) };
		if ( Status == ParticleSpawnPosition::TOP )
			return paperback::Vector3f{ glm::linearRand( Boundaries.Min.x, Boundaries.Max.x )
									  , Boundaries.Max.y
									  , glm::linearRand( Boundaries.Min.z, Boundaries.Max.z ) };
		if ( Status == ParticleSpawnPosition::BOTTOM )
			return paperback::Vector3f{ glm::linearRand( Boundaries.Min.x, Boundaries.Max.x )
									  , Boundaries.Min.y
									  , glm::linearRand( Boundaries.Min.z, Boundaries.Max.z ) };
	}
};

struct Generate_Velocity
{
	PPB_INLINE
	paperback::Vector3f Rand( void ) const noexcept
	{
		auto x = glm::linearRand( m_Min.x, m_Max.x );
		auto y = glm::linearRand( m_Min.y, m_Max.y );
		auto z = glm::linearRand( m_Min.z, m_Max.z );

		if ( x < 0.0f && x > -min_force_v )
            x = -min_force_v;
        else if ( x > 0.0f && x < min_force_v )
            x = min_force_v;

        if ( y < 0.0f && y > -min_force_v )
            y = -min_force_v;
        else if ( y > 0.0f && y < min_force_v )
            y = min_force_v;

        if ( z < 0.0f && z > -min_force_v )
            z = -min_force_v;
        else if ( z > 0.0f && z < min_force_v )
            z = min_force_v;

		return paperback::Vector3f{ x
								  , y
								  , z };
	}

	static constexpr auto min_force_v = 0.52f;
	paperback::Vector3f m_Min{ 0.0f, 1.0f, 0.0f };
	paperback::Vector3f m_Max{ 0.0f, 2.0f, 0.0f };
};

struct Generate_Rotation
{
	PPB_INLINE
	paperback::Vector3f Rand( void ) const noexcept
	{
		return paperback::Vector3f{ glm::linearRand( m_Min.x, m_Max.x )
								  , glm::linearRand( m_Min.y, m_Max.y )
								  , glm::linearRand( m_Min.z, m_Max.z ) };
	}

	paperback::Vector3f m_Min{ 0.0f, 0.0f, 0.0f };
	paperback::Vector3f m_Max{ 0.0f, 0.0f, 0.0f };
};

struct Generate_Opacity
{
	PPB_INLINE
	float Rand( void ) const noexcept
	{
		return glm::linearRand( m_Min, m_Max );
	}

	float m_Min = 0.5f;
	float m_Max = 1.0f;
};

struct Update_Velocity
{
	PPB_INLINE
	paperback::Vector3f Rand( const transform& ParticleTransform ) const noexcept
	{
		auto NormalizedDir = ( m_Destination - ParticleTransform.m_Position ).Normalized( );

		return ( NormalizedDir * glm::linearRand( m_Min, m_Max ) );
	}

	paperback::Vector3f m_Destination{ 2.0f, 2.0f, 2.0f };
	float               m_Min = 1.0f;
	float               m_Max = 5.0f;
};

struct Generate_Scale
{
	PPB_INLINE
	paperback::Vector3f Rand( void ) const noexcept
	{
		if ( m_bUniformScale )
		{
			float Scale = glm::linearRand( m_Min.x, m_Max.x );

			return paperback::Vector3f{ Scale, Scale, Scale };
		}
		else
		{
			return paperback::Vector3f{ glm::linearRand( m_Min.x, m_Max.x )
								      , glm::linearRand( m_Min.y, m_Max.y )
								      , glm::linearRand( m_Min.z, m_Max.z ) };
		}
	}

	paperback::Vector3f m_Min{ 0.001f, 0.001f, 0.001f };
	paperback::Vector3f m_Max{ 0.003f, 0.003f, 0.003f };
	bool                m_bUniformScale = true;
};




//------------------------------------------------------------
// 
// 
//                     Emitter Component
// 
// 
//------------------------------------------------------------
BEGIN_CREATE_DATA_COMPONENT( particle_emitter, Particle Emitter )
	
	using ParticleList = std::vector<paperback::u32>;

	PPB_INLINE
	void UpdateParticleList( std::span<paperback::u32> List ) noexcept
	{
		// Merge List w/ m_ActiveParticles List
		for ( const auto ID : List )
		{
			m_ActiveParticles.push_back( ID );
		}
	}

	PPB_INLINE
	int UpdateEmitter( float Dt ) noexcept
	{
		m_Lifetime -= Dt;
		m_CurrentTime -= Dt;

		if ( m_CurrentTime <= 0.0f && m_Lifetime > 0.0f )
		{
			m_CurrentTime = m_EmissionInterval;
			int QuantityAvailable = m_EmissionCap - static_cast<int>( m_ActiveParticles.size() );

			PPB_ASSERT( QuantityAvailable < 0 );

			return ( QuantityAvailable > m_EmissionRate ) ? m_EmissionRate
													      : QuantityAvailable;
		}

		return 0;
	}

	PPB_INLINE
	void ReleaseParticle( paperback::u32 ParticleGID ) noexcept
	{
		m_ActiveParticles.erase( std::remove(m_ActiveParticles.begin(), m_ActiveParticles.end(), ParticleGID), m_ActiveParticles.end() );
	}

	PPB_INLINE
	bool IsAlive( void ) const noexcept
	{
		return m_Lifetime > 0.0f;
	}

	//-----------------------------------
	//          Emitter Data
	//-----------------------------------
	float              m_Lifetime         = 0.0f;				// Lifetime of Spawner
	float              m_CurrentTime      = 0.0f;				// Current Timer
	float              m_EmissionInterval = 1.0f;				// Rate Of Particle Emission
	int                m_EmissionRate     = 1;					// Particles Spawned Per Emission Interval
	int                m_EmissionCap      = 100;				// Max Particles Owned By Emitter
	ParticleList       m_ActiveParticles;						// List Of Active Particles
	std::string        m_TextureName;							// Texture Name
	bool			   m_AffectedByGravity = false;				// Affected by Gravity

	//-----------------------------------
	//       Particle Generation
	//-----------------------------------
	Generate_Lifetime  m_GenerateLifetime;
	Generate_Position  m_GeneratePosition;
	Generate_Velocity  m_GenerateVelocity;
	Generate_Rotation  m_GenerateRotation;
	Generate_Opacity   m_GenerateOpacity;
	Update_Velocity    m_UpdateVelocity;
	Generate_Scale     m_GenerateScale;

	//-----------------------------------
	//          Emitter Status
	//-----------------------------------
	bool               m_bHasDestination = false;               // If True - Compute Normalized Direction & Multiply By Randomized Velocity Magnitude
	bool               m_bPrewarm = false;						// If True - Simulate Particles Being Already Emitted for ___ Emission Interval Loops
	int                m_SpawnLocation = 0;						// Default Spawn Location - In Center of BB

END_CREATE_DATA_COMPONENT


// Need to register all "Rands" too


namespace RR_ParticleEmitter
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_< particle_emitter >( particle_emitter::typedef_v.m_pName )
			.constructor()(rttr::policy::ctor::as_object)
			.property( "Emitter Lifetime",				 &particle_emitter::m_Lifetime         )( rttr::policy::prop::as_reference_wrapper )
			.property( "Emitter Timer",					 &particle_emitter::m_CurrentTime      )( rttr::policy::prop::as_reference_wrapper )
			.property( "Emission Interval",				 &particle_emitter::m_EmissionInterval )( rttr::policy::prop::as_reference_wrapper )
			.property( "Emission Rate",					 &particle_emitter::m_EmissionRate     )( rttr::policy::prop::as_reference_wrapper )
			.property( "Emission Cap",					 &particle_emitter::m_EmissionCap      )( rttr::policy::prop::as_reference_wrapper )
			.property( "Particle Texture Name",			 &particle_emitter::m_TextureName      )( rttr::policy::prop::as_reference_wrapper )
			.property( "Generate Particle Lifetime",	 &particle_emitter::m_GenerateLifetime )( rttr::policy::prop::as_reference_wrapper )
			.property( "Generate Particle Position",	 &particle_emitter::m_GeneratePosition )( rttr::policy::prop::as_reference_wrapper )
			.property( "Generate Particle Velocity",	 &particle_emitter::m_GenerateVelocity )( rttr::policy::prop::as_reference_wrapper )
			.property( "Generate Particle Rotation",	 &particle_emitter::m_GenerateRotation )( rttr::policy::prop::as_reference_wrapper )
			.property( "Generate Particle Opacity",		 &particle_emitter::m_GenerateOpacity  )( rttr::policy::prop::as_reference_wrapper )
			.property( "Update Particle Velocity",		 &particle_emitter::m_UpdateVelocity   )( rttr::policy::prop::as_reference_wrapper )
			.property( "Generate Particle Scale",		 &particle_emitter::m_GenerateScale    )( rttr::policy::prop::as_reference_wrapper )
			.property( "Particle Affected by Gravity?",	 &particle_emitter::m_AffectedByGravity)( rttr::policy::prop::as_reference_wrapper )
			.property( "Particle Has Destination?",		 &particle_emitter::m_bHasDestination  )( rttr::policy::prop::as_reference_wrapper )
			.property( "Enable Emitter Prewarm?",		 &particle_emitter::m_bPrewarm         )( rttr::policy::prop::as_reference_wrapper )
			.property( "Particle Spawn Location",		 &particle_emitter::m_SpawnLocation    )( rttr::policy::prop::as_reference_wrapper );
	}
}