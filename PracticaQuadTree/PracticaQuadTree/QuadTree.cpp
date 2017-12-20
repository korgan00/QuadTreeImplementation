#include "QuadTree.h"


QuadTree::QuadTree() :
    _length(0, 0), _halfLength(0, 0),
    _elements(), _children() {
}
QuadTree::QuadTree(int tamX, int tamY) :
	_length(tamX, tamY), _halfLength(tamX / 2, tamY / 2),
	_elements(), _children() {}


//private
QuadTree::QuadTree(int tamX, int tamY, const Point& position) : QuadTree(tamX, tamY) {
	_position = position;
}


bool QuadTree::IsLeaf() const {
	return !_children;
}

void QuadTree::AddCircle(Circle* circle) {
	if (!IsLeaf()) {
		_AddToChildren(circle);
		return;
	}

	_elements.push_front(circle);

	// if sounds good, branch the tree
	if (_elements.size() >= MAX_ELEMENTS && 
		_length.x() > MIN_QUAD_SIZE && _length.y() > MIN_QUAD_SIZE) {

		_CreateChildren();
	}
}

int QuadTree::_PointToChildIdx(const Point& p) const {
	Point pRelative = (p - _position) - _halfLength;

	return (pRelative.x() >= 0 ? 1 : 0) + (pRelative.y() >= 0 ? 2 : 0);
}

Circle* QuadTree::RayCast(const Point& ray) const {
	if (!IsLeaf()) {
		int childIdx = _PointToChildIdx(ray);

		return _children[childIdx].RayCast(ray);
	} 

	// ELSE
	for (auto it = _elements.begin(); it != _elements.end(); ++it) {
		if ((*it)->IsColliding(ray)) {
			return *it;
		}
	}

    return nullptr;
}


void QuadTree::_AddToChildren(Circle* circle) {
	Point relativeP = (circle->center() - _position) - _halfLength;
	Point absRelP(abs(relativeP.x()), abs(relativeP.y()));
	bool quadsSelected[4] = { false, false, false, false };


    int childIdxX = (relativeP.x() >= 0 ? 1 : 0);
    int childIdxY = (relativeP.y() >= 0 ? 2 : 0);
    int childOppIdxX = (relativeP.x() >= 0 ? 0 : 1);
    int childOppIdxY = (relativeP.y() >= 0 ? 0 : 2);

	quadsSelected[childIdxY + childIdxX] = true;

    if (absRelP.x() < CIRCLES_RADIUS) {
        quadsSelected[childOppIdxX + childIdxY] = true;
    }
	if (absRelP.y() < CIRCLES_RADIUS) {
		quadsSelected[childIdxX + childOppIdxY] = true;
	}
    if (relativeP.sqrDist(Point(0, 0)) < SQUARED_RADIUS) {
        quadsSelected[childOppIdxX + childOppIdxY] = true;
    }

	for (int i = 0; i < 4; ++i) {
		if (quadsSelected[i]) _children[i].AddCircle(circle);
	}
}

void QuadTree::_CreateChildren() {
	_children = make_unique<QuadTree[]>(4);
	_children[0] = std::move(QuadTree(_halfLength.x(), _halfLength.y(), _position/*+ Point(0,0)*/));
	_children[1] = std::move(QuadTree(_halfLength.x(), _halfLength.y(), _position + Point(_halfLength.x(), 0)));
	_children[2] = std::move(QuadTree(_halfLength.x(), _halfLength.y(), _position + Point(0, _halfLength.y())));
	_children[3] = std::move(QuadTree(_halfLength.x(), _halfLength.y(), _position + Point(_halfLength.x(), _halfLength.y())));

	for (auto it = _elements.begin(); it != _elements.end(); ++it) {
		_AddToChildren(*it);
	}

	_elements.clear();
}


void QuadTree::ForEachCircle(function<void(Circle*)> f) {
    if (!IsLeaf()) {
        for (int i = 0; i < 4; ++i) {
            _children[i].ForEachCircle(f);
        }
    } else {
        for (auto it = _elements.begin(); it != _elements.end(); ++it) {
            f(*it);
        }
    }

}
void QuadTree::ForEachQuad(function<void(QuadTree*)> f) {
    if (!IsLeaf()) {
        for (int i = 0; i < 4; ++i) {
            _children[i].ForEachQuad(f);
        }
    }

    f(this);
}