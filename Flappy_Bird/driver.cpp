#include "driver.h"

Driver::Driver(QObject *parent)
	: QObject{parent}
{
	qInfo() << "consructed the driver";
	m_timer = new QTimer(this);


	m_model = new Model(maxNumberOfObstacles, this);

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
