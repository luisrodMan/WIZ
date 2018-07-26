#include "WIZ/nodes2d.h"

#include <stdexcept>

Node2d::~Node2d() {

}

Node2d *Node2d::addChild(Node2d *child, int z) {
	if (child->parent != nullptr)
		throw std::runtime_error("Already has a parent (addChild!) In Node2d");
	if (!hasChildren()) {
		if (children == nullptr)
			children = new std::vector<Node2d*>();
		children->push_back(child);
	} else if (children->back()->zorder <= z)
		children->push_back(child);
	else {
		// search correct place
		throw std::runtime_error("Not implemented Exception (addChild with z order!) In Node2d");
	}

	return child;	
}

Node2d *Node2d::removeChild(Node2d *child) {
	if (!hasChildren())
		throw std::runtime_error("Empty children (removeChild!) In Node2d");
	if (child->parent != this)
		throw std::runtime_error("This node is not parent of the child! (removeChild!) In Node2d");
	auto v = children->begin();
	while (v != children->end()) {
		if (*v == child) {
			child->parent = nullptr;
			children->erase(v);
			return child;
		}
	}
	throw std::runtime_error("Child not found! (removeChild!) In Node2d");
}

Node2d *Node2d::getChild(int index) {
	if (!hasChildren() || index < 0 || index >= getChildrenCount())
		throw std::runtime_error("Index out of range " + std::to_string(index) + " size " + std::to_string(getChildrenCount()) + " (getChild!) In Node2d");
	return *(children->begin() + index);
}

std::vector<Node2d*> *Node2d::getChildren() {
	if (!hasChildren())
		throw std::runtime_error("This node has no children (getChildren!) In Node2d");
	return children;
}


int Node2d::getChildrenCount() {
	return !hasChildren() ? 0 : children->size();
}

bool Node2d::hasChildren() {
	return children != nullptr && children->size() > 0;
}

// STATES
int Node2d::getInternalState() {
	return internalState;
}

bool Node2d::isActive() {
	return hasStates(ACTIVE_STATE);
}

bool Node2d::isVisible() {
	return hasStates(VISIBLE_STATE);
}

void Node2d::setInternalState(int states) {
	internalState |= states;
}

int Node2d::getTag() {
	return tag;	
}

std::string Node2d::getNodeName() {
	return name;
}

void Node2d::setNodeName(std::string name) {
	this->name = name;	
}


void Node2d::updateNode(float delta) {
	if (isActive()) {
		update(delta);
		if (hasChildren()) {
			for (int i = 0; i < getChildrenCount(); i++) {
				auto iterator = children->begin() + i;
				(*iterator)->updateNode(delta);
				if ((*iterator)->hasStates(DESTROY_STATE)) {
					Node2d *c = *iterator;
					children->erase(iterator);
					delete c;
					i--;
				}
			}
		}
	}
}

void Node2d::drawNode(SpriteBatch &g) {
	if (!isVisible())
		return;
	if (!hasChildren())
		draw(g);
	else {
		Matrix originalTransform;
		originalTransform.set(*g.getTransformMatrix());

		Matrix &finaltransform = *(g.getTransformMatrix());
		originalTransform.transform(transform, finaltransform);

		g.setTransformMatrix(finaltransform);

		int i = 0;
		// draw nodes behaind
		for (; i < getChildrenCount(); i++) {
			auto iterator = children->begin() + i;
			if ((*iterator)->zorder >= zorder)
				break;
			(*iterator)->drawNode(g);
		}

		// draw this
		g.setTransformMatrix(originalTransform);
		draw(g);

		// draw front
		for (; i < getChildrenCount(); i++) {
			(*(children->begin() + i))->drawNode(g);
		}

		g.setTransformMatrix(originalTransform);
	}
}

void Node2d::update(float delta) {
	
}

void Node2d::draw(SpriteBatch &g) {
	
}













Sprite::Sprite(Texture *t) {
	this->texture = t;
	origin.set(t->getWidth()*0.5f, t->getHeight() * 0.5f);
}

void Sprite::draw(SpriteBatch &g) {
	g.draw(*texture, 
		position.x, position.y, 
		0, 0, texture->getWidth(), texture->getHeight(),   
		origin.x, origin.y, rotation, scale.x, scale.y, flipx, flipy, color);
}

int Sprite::getWidth() {
	return texture->getWidth();
}

int Sprite::getHeight() {
	return texture->getHeight();
}