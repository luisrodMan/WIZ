#ifndef __WIZ_NODES2D__
#define __WIZ_NODES2D__

#include "WIZ/math.h"
#include "graphics.h"

#include <vector>
#include <string>

class Node2d {
 
public:
	static const int ACTIVE_STATE = 1;
	static const int VISIBLE_STATE = 2;
	static const int DESTROY_STATE = 3;

public:
	Vector2 position, origin, scale=Vector2(1,1);
	float rotation=0;
	Color color = Color(1, 1, 1, 1);

private:
	Node2d *parent = nullptr;
	int zorder = 0;
	std::vector<Node2d*> *children = nullptr;
	Matrix transform;
	int internalState = ACTIVE_STATE | VISIBLE_STATE;
	std::string name;
	int tag = 0;

	inline bool hasStates(int states) {
		return (internalState & states) == states; 
	}

public:

	virtual ~Node2d();

	Node2d *getParent();

	Node2d *addChild(Node2d *child, int z = 0);

	Node2d *removeChild(Node2d *child);

	Node2d *getChild(int index);

	std::vector<Node2d*> *getChildren();


	int getChildrenCount();

	bool hasChildren();

	// STATES
	int getInternalState();

	bool isActive();

	bool isVisible();

	void setInternalState(int states);

	int getTag();

	std::string getNodeName();

	void setNodeName(std::string name);


	virtual void updateNode(float delta);

	virtual void drawNode(SpriteBatch &g);

	virtual void update(float delta);

	virtual void draw(SpriteBatch &g);

};




class Sprite : public Node2d {

private:
	Texture *texture;

public:

	bool flipx=false, flipy=false;

	Sprite(Texture *t);

	void draw(SpriteBatch &g);

	int getWidth();
	int getHeight();

};


#endif