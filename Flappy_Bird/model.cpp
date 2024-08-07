#include "model.h"

Model::Model(QObject *parent)
	: QAbstractListModel(parent)
{
	QObject::connect(this, &Model::shouldCheckForCollision, this, &Model::checkForCollision);
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
		o.x -= m_obstacleStep;
		if ((o.x + m_obstacleWidth < m_birdX) && !o.passed) {
			o.passed = true;
			// qInfo() << "bird x : " << m_birdX;
			// qInfo() << "x : " << o.x << ", width : " << m_obstacleWidth << ", x	+ width : " << o.x + m_obstacleWidth;
			// qInfo() << "passed an obstacle";
		}
	}
	// trace();
	emit shouldCheckForCollision();
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

double Model::euclidianDistance(int bird_midPoint_x, int bird_midPoint_y, int pointX, int pointY)
{
	int dx = abs(pointX - bird_midPoint_x);
	int dy = abs(pointY - bird_midPoint_y);

	return std::sqrt(dx * dx + dy * dy);
}

/*
........|##########|............
.  (2) .|## wall ##|..	 (2) ...
........|##########|............
--------|----------|------------
.. (3)..|.. (1) ...|..   (3)....
........|..........|............
--------|----------|------------
........|##########|............
.. (2)..|## wall ##|.... (2) ...
........|##########|............

*/



bool Model::isCollided(int bird_x1, int bird_x2, int bird_y1, int bird_y2, int model_x1, int model_gap_y1) {
	int model_x2 = model_x1 + m_obstacleWidth;
	int model_gap_y2 = model_gap_y1 + m_gapHeight;

	int bird_midPoint_X = (bird_x1 + bird_x2) / 2;
	int bird_midPoint_Y = (bird_y1 + bird_y2) / 2;
	int collision_radius = (bird_x2 - bird_x1) / 2;

	// allow colliding with a part of the image instead of colliding
	// with air as the image is not circular
	collision_radius -= collision_radius / 3;

	if (model_x1 <= bird_midPoint_X && bird_midPoint_X <= model_x2 &&
			model_gap_y1 <= bird_midPoint_Y && bird_midPoint_Y <= model_gap_y2) { // (1)

		// qInfo() << "here 1";

		return bird_midPoint_Y - collision_radius <= model_gap_y1
				|| bird_midPoint_Y + collision_radius >= model_gap_y2;
	}
	else if (model_gap_y1 <= bird_midPoint_Y && bird_midPoint_Y <= model_gap_y2) { // (3)
		int wall_1_corner_1_x = model_x1, wall_1_corner_1_y = model_gap_y1;
		int wall_1_corner_2_x = model_x2, wall_1_corner_2_y = model_gap_y1;

		int wall_2_corner_1_x = model_x1, wall_2_corner_1_y = model_gap_y2;
		int wall_2_corner_2_x = model_x2, wall_2_corner_2_y = model_gap_y2;

		double dist = std::min({euclidianDistance(bird_midPoint_X, bird_midPoint_Y, wall_1_corner_1_x, wall_1_corner_1_y),
							   euclidianDistance(bird_midPoint_X, bird_midPoint_Y, wall_1_corner_2_x, wall_1_corner_2_y),
							   euclidianDistance(bird_midPoint_X, bird_midPoint_Y, wall_2_corner_1_x, wall_2_corner_1_y),
							   euclidianDistance(bird_midPoint_X, bird_midPoint_Y, wall_2_corner_2_x, wall_2_corner_2_y)});

		// qInfo() << "here 3";

		return dist < double(collision_radius);
	}
	else { // (2)
		if (bird_midPoint_X <= model_x1) {
			// qInfo() << "here 2";

			return model_x1 - bird_midPoint_X < collision_radius;
		}
		else if (model_x2 <= bird_midPoint_X){
			return bird_midPoint_X - model_x2 < collision_radius;
		}
	}

	qInfo() << "here 4";

	return false;
}

void Model::eraseObstacles()
{
	if (obstacles.empty())
		return;

	beginRemoveRows(QModelIndex(), 0, (int)obstacles.size() - 1);
	obstacles.clear();
	endRemoveRows();
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

void Model::setObstableWidth(int newObstacleWidth)
{
	if (m_obstacleWidth != newObstacleWidth) {
		m_obstacleWidth = newObstacleWidth;
	}
}

void Model::setMaxNumberOfObstacles(int newNumObstacles)
{
	if (m_maxNumberOfObstacles != newNumObstacles) {
		m_maxNumberOfObstacles = newNumObstacles;
	}
}

void Model::setBirdX(int newX)
{
	if (m_birdX != newX) {
		m_birdX = newX;
	}
}

void Model::setBirdY(int newY)
{
	if (m_birdY != newY) {
		m_birdY = newY;
		emit shouldCheckForCollision();
	}
}

void Model::setBirdWidth(int newWidth)
{
	if (m_birdWidth != newWidth) {
		m_birdWidth = newWidth;
	}
}

void Model::setBirdHeight(int newHeight)
{
	if (m_birdHeight != newHeight) {
		m_birdHeight = newHeight;
	}
}

void Model::checkForCollision()
{
	static int collisions = 0;
	for (auto& obstacle : obstacles) {
		if (obstacle.passed) continue;

		if (isCollided(m_birdX,
					   m_birdX + m_birdWidth,
					   m_birdY,
					   m_birdY + m_birdHeight,
					   obstacle.x,
					   obstacle.gapY)) {
			qInfo() << "collision detected : " << collisions++;
			emit detectedCollisions();
		}
		break;
	}
}

QHash<int, QByteArray> Model::roleNames() const
{
	QHash<int, QByteArray> names;

	names[XPositionRole] = "X";
	names[gapYRole] = "gapY";

	return names;
}
