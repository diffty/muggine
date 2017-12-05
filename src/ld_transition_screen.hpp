#ifndef LD_TRANSITION_SCREEN
#define LD_TRANSITION_SCREEN

#include "common_types.hpp"
#include "spritesheet.hpp"


enum ETransitionAnimType {
	TRANSITION_IN,
	TRANSITION_OUT,
};

class LDTransitionScreen
{
private:
	float m_fAnimCoef = 0;
	bool m_fAnimActive = false;
	ETransitionAnimType m_eAnimType;

	SpriteSheet* m_pStripesSprSht;

public:
	LDTransitionScreen();
	~LDTransitionScreen();

	void doTransition(ETransitionAnimType eTransitionType);

	bool isActive();

	void update();
	void draw(uint8* pBuffer);
};


#endif