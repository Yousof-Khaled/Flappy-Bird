#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <deque>
#include <QRandomGenerator>

struct Obstacle {
	static int width;

	int x;
	int gapY;

	Obstacle(int x, int gapY): x(x), gapY(gapY)
	{

	}
};
// int Obstacle::gapHeight = 0;

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

public slots:
	void setWindowRightMost(int rightMostPoint);
	void setWindowHeight(int newWindowHeight);
	void setGapHeight(int newGapHeight);
	void setMaxNumberOfObstacles(int newNumObstacles);

private:
	virtual QHash<int, QByteArray> roleNames() const override;

	std::deque<Obstacle> obstacles;

	int m_windowRightmost;
	int m_windowHeight;
	int m_maxNumberOfObstacles;
	int m_gapHeight;

	QRandomGenerator *randomGenerator = QRandomGenerator::global();
};

#endif // MODEL_H
