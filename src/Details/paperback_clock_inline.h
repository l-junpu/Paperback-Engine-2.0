/**********************************************************************************
*\file         paperback_clock_inline.h
*\brief        paperback_clock_inline.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
namespace tools
{
	//-----------------------------------
	//            Default
	//-----------------------------------
	clock::clock(float TimeScale) noexcept :
		m_FixedDT{ 1 / 60.0f },
		m_TimeScale{ TimeScale },
		m_DT{ },
		m_Start{ Now() }
	{ }

	uint32_t clock::FPS() noexcept
	{
		return static_cast<uint32_t>( 1.0f / m_DT.count() );
	}

	void clock::Tick() noexcept
	{
		m_DT = HRClock::now() - m_Start;
		m_Start = HRClock::now();

		m_TimeElapsed += m_DT.count();
	}

	float clock::DeltaTime() const noexcept
	{
		return m_DT.count();
	}

	auto clock::Now() noexcept -> decltype( HRClock::now() )
	{
		return HRClock::now();
	}

	float clock::ComputeTime( TimePoint StartingTime ) noexcept
	{
		return static_cast<Duration>( HRClock::now() - StartingTime ).count();
	}

	void clock::TimeScale( const float TimeScale ) noexcept
	{
		m_TimeScale = TimeScale;
	}

	float clock::TimeScale() const noexcept
	{
		return m_TimeScale;
	}
}