#pragma once

class timer {
public:
	timer();

	void Start(); //unpause or start timer
	void Stop(); //pause timers
	void Tick(); //call every frame
	void Reset(); //call before message loop

	float DeltaTime(); //get deltatime
	float TotalTime()const; //get totoal app time(not including paused time)

private:
	double mSecPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

