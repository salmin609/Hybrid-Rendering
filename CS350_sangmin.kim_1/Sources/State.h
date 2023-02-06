#pragma once
class State
{
public:
	virtual ~State() = default;
	virtual void Load() = 0;
	virtual void Update(float dt) = 0;
	virtual void UnLoad() = 0;
	virtual void SetNextPrevState(State* prev, State* next)
	{
		nextState = next;
		prevState = prev;
	}
	virtual State* GetNextState()
	{
		return nextState;
	}
	virtual State* GetPrevState()
	{
		return prevState;
	}
private:
protected:
	State* nextState;
	State* prevState;
};
