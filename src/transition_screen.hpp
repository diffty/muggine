#ifndef LD_TRANSITION_SCREEN
#define LD_TRANSITION_SCREEN

#include "common_types.hpp"
#include "spritesheet.hpp"


enum ETransitionAnimType {
	TRANSITION_IN,
	TRANSITION_OUT,
};

class TransitionScreen
{
private:
	float m_fAnimCoef;
	bool m_fAnimActive;

	ETransitionAnimType m_eAnimType;

public:
	TransitionScreen();
	~TransitionScreen();

	void doTransition(ETransitionAnimType eTransitionType);

	bool isActive();

	void update();
	void draw(uint8* pBuffer);
};


#endif
