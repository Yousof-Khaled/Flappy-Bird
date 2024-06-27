#include "driver.h"

Driver::Driver(QObject *parent)
	: QObject{parent}
{
	qInfo() << "consructed the driver";
	m_timer = new QTimer(this);


	m_model = new Model(this);

	QObject::connect(m_timer, &QTimer::timeout, this, [this] {
		static int track = 0;

		m_model->moveLeft();

		track++;
		if (track % 200 == 0) {
			m_model->addLast();
		}

		qInfo() << "timeout occurred";
	});

	QObject::connect(this, &Driver::windowRightmostChanged, m_model, &Model::setWindowRightMost);
	QObject::connect(this, &Driver::windowHeightChanged, m_model, &Model::setWindowHeight);
	QObject::connect(this, &Driver::gapHeightChanged, m_model, &Model::setGapHeight);
	QObject::connect(this, &Driver::maxNumberOfObstaclesChanged, m_model, &Model::setMaxNumberOfObstacles);

	QObject::connect(this, &Driver::startedChanged, this, [this] {
		m_timer->start(timeoutInMS);
	});
}

Driver::~Driver()
{

}

void Driver::setStarted(bool s)
{
	if (m_started != s) {
		m_started = s;
		emit startedChanged(m_started);
	}
}

void Driver::setWindowRightmost(int rightMostPoint)
{
	if (m_windowRightmost != rightMostPoint) {
		m_windowRightmost = rightMostPoint;
		emit windowRightmostChanged(m_windowRightmost);
	}
}

void Driver::setWindowHeight(int newWindowHeight)
{
	if (m_windowHeight != newWindowHeight) {
		m_windowHeight = newWindowHeight;
		emit windowHeightChanged(m_windowHeight);
	}
}

void Driver::setGapHeight(int newGapHeight)
{
	if (m_gapHeight != newGapHeight) {
		m_gapHeight = newGapHeight;
		emit gapHeightChanged(m_gapHeight);
	}
}

void Driver::setMaxNumberOfObstacles(int newMaxNumObstacles)
{
	if (m_maxNumberOfObstacles != newMaxNumObstacles) {
		m_maxNumberOfObstacles = newMaxNumObstacles;
		emit maxNumberOfObstaclesChanged(m_maxNumberOfObstacles);
	}
}
