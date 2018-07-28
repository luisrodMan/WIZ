#include <string>
#include <map>
#include <vector>

class XmlElement {

private:
	std::string *tag = nullptr;
	std::string *text = nullptr;
	std::map<std::string, std::string> *attributes = nullptr;
	std::vector<XmlElement*> *children = nullptr;
	XmlElement *parent = nullptr;



public:

	XmlElement(std::string tag);
	
	~XmlElement();

	XmlElement *addChild(XmlElement *child);

	XmlElement *removeChild(XmlElement *child);

	int getChildrenCount();

	std::vector<XmlElement*> *getChildren();


	void addAttribute(std::string name, int value);

	void addAttribute(std::string name, float value);

	void addAttribute(std::string name, double value);

	void addAttribute(std::string name, std::string value);

	void setText(std::string text);

	std::string getTag();

	bool hasAttribute(std::string name);

	bool hasText();

	std::string getString(std::string name="");

	int getInt(std::string name="");

	float getFloat(std::string name="");

	double getDouble(std::string name="");

};

class XmlDocument {

private:
	XmlElement *rootElement = nullptr;
	int currentToken = -1;
	int nextTokenX = -1;
	int currentValue = (int)' ';
	int noConsumeNext = 0;
	std::string lastValue;
	std::string nextValue;
	std::string currentValueString;
	FILE *file;

public:

	XmlElement *getRoot();

	bool isSpace(char c);
	void consumeSpace(FILE *file);
	void load(std::string path);

	XmlElement *createElement();

	int nextToken();

	bool accept(int token);

	bool expected(int token);


	std::string print();

	std::string printBeauty();

};