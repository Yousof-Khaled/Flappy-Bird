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

	Q_PROPERTY(int birdX MEMBER m_birdX WRITE setBirdX NOTIFY birdXChanged FINAL)
	Q_PROPERTY(int birdY MEMBER m_birdY WRITE setBirdY NOTIFY birdYChanged FINAL)
	Q_PROPERTY(int birdWidth MEMBER m_birdWidth WRITE setBirdWidth NOTIFY birdWidthChanged FINAL)
	Q_PROPERTY(int birdHeight MEMBER m_birdHeight WRITE setBirdHeight NOTIFY birdHeightChanged FINAL)
	Q_PROPERTY(int obstableWidth MEMBER m_obstacleWidth WRITE setObstableWidth NOTIFY obstableWidthChanged FINAL)

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

	void setObstableWidth(int newObstacleWidth);

	Model* model() { return m_model; }

	int maxNumberOfObstacles() { return m_maxNumberOfObstacles; }
	void setMaxNumberOfObstacles(int newMaxNumObstacles);

	void setBirdX(int newX);
	void setBirdY(int newY);
	void setBirdWidth(int newWidth);
	void setBirdHeight(int newHeight);

	Q_INVOKABLE void resetGame() {
		m_model->eraseObstacles();
	}

signals:
	void startedChanged(bool s);
	void windowRightmostChanged(int rightMostPoint);
	void modelChanged();
	void windowHeightChanged(int newWindowHeight);
	void gapHeightChanged(int newGapHeight);
	void obstableWidthChanged(int newObstacleWidth);
	void maxNumberOfObstaclesChanged(int newMaxNumObstacles);
	void birdXChanged(int x);
	void birdYChanged(int y);
	void birdWidthChanged(int newWidth);
	void birdHeightChanged(int newHeight);
	void baseMoveLeft();

	void gameOver();

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

	int m_obstacleWidth;

	int m_birdX;
	int m_birdY;
	int m_birdWidth;
	int m_birdHeight;
};

#endif // DRIVER_H
