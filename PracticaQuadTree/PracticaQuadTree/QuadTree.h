#pragma once
#include "circle.h"
#include <list>
#include <memory>
#include <functional>


#ifndef MAX_ELEMENTS
#define MAX_ELEMENTS 6
#endif 

#undef MIN_QUAD_SIZE
#define MIN_QUAD_SIZE (int)(CIRCLES_RADIUS * 0.7f)

class QuadTree
{
private:
	Point _position;
	Point _length;
	Point _halfLength;

	list<Circle*> _elements;
    unique_ptr<QuadTree[]> _children;
	//QuadTree* _children;

	void _AddToChildren(Circle*);
	void _CreateChildren();
	int _PointToChildIdx(const Point&) const;

	QuadTree(int, int, const Point&);

public:
    explicit QuadTree();
    QuadTree(int, int);
    ~QuadTree() = default;

    inline Point position() const {
        return _position;
    }
    inline Point length() const {
        return _length;
    }

    QuadTree& operator=(const QuadTree& that) {
        _position = that._position;
        _length = that._length;
        _halfLength = that._halfLength;
        _elements = that._elements;

        return *this;
    }
    
	inline bool IsLeaf() const;

	void AddCircle(Circle*);
	Circle* RayCast(const Point&) const;

    void ForEachCircle(function<void(Circle*)>);
    void ForEachQuad(function<void(QuadTree*)>);
};

