#ifndef _TIME_COUNT_BASIC_H_
#define _TIME_COUNT_BASIC_H_

#pragma once

/// <summary>A time counting class that acts as a wrapper for
/// different ways of counting time using different libraries.
/// <para>It has internal members to store time in different moments and 
/// accessors to make it easy to work with.</para>
/// <para>Each new implementation using this class must only implement
/// <see cref="CTimeCount::TimeNow()"/>.</para>
/// <para>This class is similar to <see cref="CTimeCount"/>, but this has
/// no mutex protection in its members, so it's not thread safe.</para></summary>
class CTimeCountBasic
{
public:
	/// <summary>Default constructor.</summary>
	CTimeCountBasic( void );

protected:
	double
		/// <summary>Member variable to store the moment that the time started counting.</summary>
		dTimeStart,
		/// <summary>Member variable to store the moment that the time stopped counting.</summary>
		dTimeFinish;

public:
	/// <summary>Accessor that allows the start time to be read.</summary>
	/// <returns>The start time.</returns>
	double get_Time_Start( void );

	/// <summary>Accessor that allows the finish time to be read.</summary>
	/// <returns>The finish time.</returns>
	double get_Time_Finish( void );
	
	/// <summary>Mark this moment as the start time.</summary>
	void set_Time_Start( void );

	/// <summary>Mark this moment as the finish time.</summary>
	void set_Time_Finish( void );

	/// <summary>Compute the difference time, in seconds, between the call of 
	/// <see cref="CTimeCountBasic::set_Time_Finish()/> and <see cref="CTimeCountBasic::set_Time_Start()/> functions.</summary>
	/// <returns>The time that passed between start and finish.</returns>
	double HowLong( void );

	/// <summary>Compute the difference time, in seconds, between now and 
	/// the call of <see cref="CTimeCountBasic::set_Time_Start"/>.</summary>
	/// <returns>The time that passed between start and now.</returns>
	double HowLong_untilNow(void);

	/// <summary>Returns the current time at this moment.</summary>
	/// <returns>The time now.</returns>
	virtual double Time_Now( void ) = 0;
};

#endif //_TIME_COUNT_BASIC_H_

