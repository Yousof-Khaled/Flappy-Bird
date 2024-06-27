#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include <deque>

struct Obstacle {
	static int gapHeigh;
	static int width;

	int x;

	Obstacle(int x): x(x)
	{

	}
};

class Model : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit Model(int maxNumObstacles, QObject *parent = nullptr);

	enum {
		XPositionRole = Qt::UserRole
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

private:
	virtual QHash<int, QByteArray> roleNames() const override;

	std::deque<Obstacle> obstacles;

	int m_windowRightmost;
	int m_maxNumberOfObstacles;
};

#endif // MODEL_H
