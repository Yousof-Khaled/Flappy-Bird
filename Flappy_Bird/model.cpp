#include "model.h"

Model::Model(QObject *parent)
	: QAbstractListModel(parent)
{

}

int Model::rowCount(const QModelIndex &parent) const
{
	// For list models only the root node (an invalid parent) should return the list's size. For all
	// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
	if (parent.isValid())
		return 0;

	// FIXME: Implement me!
	return obstacles.size();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	// FIXME: Implement me!

	switch(role) {
	case XPositionRole:
		return obstacles[index.row()].x;
	case gapYRole:
		return obstacles[index.row()].gapY;
	default:
		break;
	}

	return QVariant();
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (data(index, role) != value) {
		// FIXME: Implement me!

		switch (role) {
		case XPositionRole:
			obstacles[index.row()].x = value.toInt();
			break;
		default:
			break;
		}

		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

void Model::trace()
{
	for (auto &o : obstacles) {
		qInfo() << o.x << ", gapY: " << o.gapY;
	}
}

void Model::moveLeft()
{
	for (auto& o: obstacles) {
		o.x -= 1;
	}
	// trace();
	emit dataChanged(createIndex(0, 0), createIndex(int(obstacles.size()) - 1, 0), {XPositionRole});
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

void Model::removeFirst()
{
	beginRemoveRows(QModelIndex(), 0, 0);
	obstacles.pop_front();
	endRemoveRows();
}

void Model::addLast()
{
	if (obstacles.size() == m_maxNumberOfObstacles) {
		removeFirst();
	}
	beginInsertRows(QModelIndex(), obstacles.size(), obstacles.size());
	obstacles.emplace_back(m_windowRightmost, randomGenerator->bounded(10, m_windowHeight - m_gapHeight - 50));
	endInsertRows();
}

void Model::setWindowRightMost(int rightMostPoint)
{
	if (m_windowRightmost != rightMostPoint) {
		m_windowRightmost = rightMostPoint;
	}
}

void Model::setWindowHeight(int newWindowHeight)
{
	if (m_windowHeight != newWindowHeight) {
		m_windowHeight = newWindowHeight;
	}
}

void Model::setGapHeight(int newGapHeight)
{
	if (m_gapHeight != newGapHeight) {
		m_gapHeight = newGapHeight;
	}
}

void Model::setMaxNumberOfObstacles(int newNumObstacles)
{
	if (m_maxNumberOfObstacles != newNumObstacles) {
		m_maxNumberOfObstacles = newNumObstacles;
	}
}

QHash<int, QByteArray> Model::roleNames() const
{
	QHash<int, QByteArray> names;

	names[XPositionRole] = "X";
	names[gapYRole] = "gapY";

	return names;
}
