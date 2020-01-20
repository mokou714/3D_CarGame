#include "timer.h"
#include <Windows.h>

timer::timer() : 
	mSecPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mPausedTime(0),
	mStopTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecPerCount = 1.0 / (double)countsPerSec;
}

void timer::Tick() {
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}

	//get current time
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	//get delta time in sec, curr - prev
	mDeltaTime = (mCurrTime - mPrevTime)*mSecPerCount;

	//update prev for next frame
	mPrevTime = mCurrTime;

	//force nonnegative, delta time could be negative in some cases
	if (mDeltaTime < 0.0) {
		mDeltaTime = 0.0;
	}

	return;
}

void timer::Reset() {
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mPausedTime = 0;
	mStopped = false;
}

void timer::Start() {
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	//accumulate pasuse time after restarting timer
	if (mStopped) {
		mPausedTime += (startTime - mStopped);
		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void timer::Stop() {
	if (!mStopped) {
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mStopTime = currTime; //save current time to stop time
		mStopped = true;
	}
}

float timer::DeltaTime() {
	return (float)mDeltaTime;
}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
float timer::TotalTime() const {
	if (mStopped) {
		//stop time - previous paused time and then - base time
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecPerCount);
	}
	else {
		//current time - previous paused time and them - base time
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecPerCount);
	}
}

