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
	Q_PROPERTY(int maxNumberOfObstacles READ maxNumberOfObstacles WRITE setMaxNumberOfObstacles NOTIFY maxNumberOfObstaclesChanged FINAL)
	Q_PROPERTY(int windowRightmost READ windowRightmost WRITE setWindowRightmost NOTIFY windowRightmostChanged FINAL)
	Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged FINAL)
	Q_PROPERTY(int gapHeight READ gapHeight WRITE setGapHeight NOTIFY gapHeightChanged FINAL) // obstacle gap height

public:
	explicit Driver(QObject *parent = nullptr);
	~Driver();

	bool started() { return m_started; }
	void setStarted(bool s);

	int windowRightmost() { return m_windowRightmost; }
	void setWindowRightmost(int rightMostPoint);

	int windowHeight() { return m_windowHeight; }
	void setWindowHeight(int newWindowHeight);

	int gapHeight() { return m_gapHeight; }
	void setGapHeight(int newGapHeight);

	Model* model() { return m_model; }

	int maxNumberOfObstacles() { return m_maxNumberOfObstacles; }
	void setMaxNumberOfObstacles(int newMaxNumObstacles);

signals:
	void startedChanged(bool s);
	void windowRightmostChanged(int rightMostPoint);
	void modelChanged();
	void windowHeightChanged(int newWindowHeight);
	void gapHeightChanged(int newGapHeight);
	void maxNumberOfObstaclesChanged(int newMaxNumObstacles);

private:
	QTimer *m_timer;

	Model *m_model;

	bool m_started = false;

	int m_maxNumberOfObstacles;
	int secondsBetweenObstablesGeneration = 2;

	int timeoutInMS = 10;

	int m_windowRightmost;
	int m_windowHeight;
	int m_gapHeight;
};

#endif // DRIVER_H
