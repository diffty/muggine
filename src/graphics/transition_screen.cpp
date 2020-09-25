#include "transition_screen.hpp"

#include "../manager/rsc_manager.hpp"
#include "../core/system.hpp"
#include "../manager/game_manager.hpp"


TransitionScreen::TransitionScreen() {
	m_fAnimCoef = 0;
	m_fAnimActive = false;
}


TransitionScreen::~TransitionScreen() {

}


void TransitionScreen::doTransition(ETransitionAnimType eTransitionType) {
	m_fAnimActive = true;
	m_eAnimType = eTransitionType;

	switch (m_eAnimType) {
	case TRANSITION_IN:
		m_fAnimCoef = 0.;
		break;

	case TRANSITION_OUT:
		m_fAnimCoef = 1.;
		break;
	}
}

bool TransitionScreen::isActive() {
	return m_fAnimActive;
}

void TransitionScreen::update() {
	if (m_fAnimActive) {
		switch (m_eAnimType) {
		case TRANSITION_IN:
			m_fAnimCoef += ((1. - m_fAnimCoef) / 2.) * System::get()->getDeltaTime() * 20.;

			if (m_fAnimCoef >= 0.99) {
				m_fAnimCoef = 1;
				m_fAnimActive = false;
				GameManager::get()->onEndTransition(m_eAnimType);
			}
			break;

		case TRANSITION_OUT:
			m_fAnimCoef -= ((m_fAnimCoef) / 2.) * System::get()->getDeltaTime() * 20.;

			if (m_fAnimCoef <= 0.01) {
				m_fAnimCoef = 0;
				m_fAnimActive = false;
				GameManager::get()->onEndTransition(m_eAnimType);
			}
			break;
		}
	}
}

void TransitionScreen::draw(drawbuffer* pBuffer) {
	if (m_fAnimActive) {
        // Use m_fAnimCoef to know where we are in the animation
	}
}
