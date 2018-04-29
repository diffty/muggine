#include "or_transition_screen.hpp"
#include "rsc_manager.hpp"
#include "system.hpp"
#include "or_game_manager.hpp"


ORTransitionScreen::ORTransitionScreen() {
	m_fAnimCoef = 0;
	m_fAnimActive = false;
	m_pStripesSprSht = RscManager::get()->getSprShtRsc(14);
}


ORTransitionScreen::~ORTransitionScreen() {

}


void ORTransitionScreen::doTransition(ETransitionAnimType eTransitionType) {
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

bool ORTransitionScreen::isActive() {
	return m_fAnimActive;
}

void ORTransitionScreen::update() {
	if (m_fAnimActive) {
		switch (m_eAnimType) {
		case TRANSITION_IN:
			m_fAnimCoef += ((1. - m_fAnimCoef) / 2.) * System::get()->getDeltaTime() * 20.;

			if (m_fAnimCoef >= 0.99) {
				m_fAnimCoef = 1;
				m_fAnimActive = false;
				ORGameManager::get()->onEndTransition(m_eAnimType);
			}
			break;

		case TRANSITION_OUT:
			m_fAnimCoef -= ((m_fAnimCoef) / 2.) * System::get()->getDeltaTime() * 20.;

			if (m_fAnimCoef <= 0.01) {
				m_fAnimCoef = 0;
				m_fAnimActive = false;
				ORGameManager::get()->onEndTransition(m_eAnimType);
			}
			break;
		}
	}
}

void ORTransitionScreen::draw(uint8* pBuffer) {
	if (m_fAnimActive) {
		int iStripeWSize = m_pStripesSprSht->getFrameSize().w;
		int iStripeYPos = m_fAnimCoef * 240;

		for (int i = 0; i < 5; i++) {
			if (i % 2 == 0)
				m_pStripesSprSht->draw(pBuffer, 0, i * iStripeWSize, 240 - iStripeYPos, false, false);
			else
				m_pStripesSprSht->draw(pBuffer, 1, i * iStripeWSize, - 240 + iStripeYPos, false, false);
		}
	}
}
