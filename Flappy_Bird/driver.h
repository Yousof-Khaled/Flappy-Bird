#ifndef DRIVER_H
#define DRIVER_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>

#include "model.h"

class Driver : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(Model* model READ model NOTIFY modelChanged FINAL)
	Q_PROPERTY(bool started READ started WRITE setStarted NOTIFY startedChanged FINAL)
	Q_PROPERTY(int windowRightmost READ windowRightmost WRITE setWindowRightmost NOTIFY windowRightmostChanged FINAL)

public:
	explicit Driver(QObject *parent = nullptr);
	~Driver();

	bool started() { return m_started; }
	void setStarted(bool s);

	int windowRightmost() { return m_windowRightmost; }
	void setWindowRightmost(int rightMostPoint);

	Model* model() { return m_model; }

signals:
	void startedChanged(bool s);
	void windowRightmostChanged(int rightMostPoint);
	void modelChanged();

private:
	QTimer *m_timer;

	Model *m_model;

	bool m_started = false;

	int maxNumberOfObstacles = 5;
	int secondsBetweenObstablesGeneration = 2;

	int timeoutInMS = 10;

	int m_windowRightmost;

};

#endif // DRIVER_H
