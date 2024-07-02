#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <deque>
#include <QRandomGenerator>

struct Obstacle {
	int x;
	int gapY;
	bool passed = false;

	Obstacle(int x, int gapY): x(x), gapY(gapY)
	{

	}
};

class Model : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit Model(QObject *parent = nullptr);

	enum {
		XPositionRole = Qt::UserRole,
		gapYRole
	};

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	// Editable:
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;

	void trace();

	void moveLeft();

	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void removeFirst();

	void addLast();

	bool isCollided(int bird_x1, int bird_x2, int bird_y1, int bird_y2, int model_x, int model_gap_y);

	void eraseObstacles();

public slots:
	void setWindowRightMost(int rightMostPoint);
	void setWindowHeight(int newWindowHeight);

	void setGapHeight(int newGapHeight);

	void setObstableWidth(int newObstacleWidth);
	void setMaxNumberOfObstacles(int newNumObstacles);

	void setBirdX(int newX);
	void setBirdY(int newY);
	void setBirdWidth(int newWidth);
	void setBirdHeight(int newHeight);

	void checkForCollision();

signals:
	void shouldCheckForCollision();
	void detectedCollisions();

private:
	virtual QHash<int, QByteArray> roleNames() const override;

	std::deque<Obstacle> obstacles;

	int m_windowRightmost;
	int m_windowHeight;
	int m_maxNumberOfObstacles;
	int m_gapHeight;
	int m_obstacleWidth;
	int m_obstacleStep = 1;

	int m_birdX;
	int m_birdY;
	int m_birdWidth;
	int m_birdHeight;

	QRandomGenerator *randomGenerator = QRandomGenerator::global();
};

#endif // MODEL_H
