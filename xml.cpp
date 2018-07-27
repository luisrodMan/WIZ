#include "WIZ/xml.h"

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cctype>

#define IDENTIFIER  1
#define EQUAL 		2
#define STRING 		3
#define SLASH   	4
#define L_ARROW 	5
#define R_ARROW 	6

XmlElement *XmlDocument::getRoot() {
	return rootElement;
}

bool XmlDocument::isSpace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\n' || c=='\r';
}

void XmlDocument::consumeSpace(FILE *file) {
	if (isSpace((char)currentValue))
	while ((currentValue = getc(file)) != EOF && isSpace((char)currentValue));
}

bool XmlDocument::accept(int token) {
	if (currentToken == token) {
		nextToken();
		return true;
	}
	return false;
}

bool XmlDocument::expected(int token) {
	if (!accept(token))
		throw std::runtime_error("Unexpected token: " + (currentToken==IDENTIFIER? "IDENTIFIER" : (currentToken==STRING?"STRING": std::to_string(currentToken))) + " expected: " + (token==IDENTIFIER? "IDENTIFIER" : (token==STRING?"STRING": std::to_string(token))));
	return true;
}


int XmlDocument::nextToken() {
	lastValue = currentValueString;
	currentToken = nextTokenX;
	if (currentValue != EOF && noConsumeNext == 0)
		currentValue = getc(file);	
	consumeSpace(file);
	if (currentValue == EOF) {
		nextTokenX = -1;
		return currentToken;
	}
	noConsumeNext = 0;
	
	std::string ll = nextValue;
	
	if (currentToken == IDENTIFIER || currentToken == STRING)
		currentValueString  = nextValue;


	char c = (char) currentValue;

	switch(c) {
		case '<':
			nextTokenX = L_ARROW;
			break;
		case '>':
			nextTokenX = R_ARROW;
			break;
		case '/':
			nextTokenX = SLASH;
			break;
		case '=':
			nextTokenX = EQUAL;
			break;
		case '"':
			nextTokenX = STRING;
			nextValue = "";
			while ((currentValue = getc(file)) != EOF && (char)currentValue != '"')
				nextValue += (char) currentValue;
			//noConsumeNext = 1;
			break;
		default:
			nextTokenX = IDENTIFIER;
			if (isalpha((char) currentValue)) {
				nextValue = (char) currentValue;
				while ((currentValue = getc(file)) != EOF && isalpha(currentValue))
					nextValue += (char) currentValue;
				noConsumeNext = 1;
			} else
				throw std::runtime_error("Invalid character as IDENTIFIER start!: " + std::to_string(currentValue));

	}

	// if (nextTokenX == IDENTIFIER || nextTokenX == STRING)
	// 	lastValue = ll;

	std::cout << "value: " <<  (char) currentValue << " token: " << nextTokenX << std::endl;
	if (noConsumeNext == 1 || nextTokenX == STRING)
		std::cout << "\t" << nextValue << std::endl;
	return currentToken;
}



XmlElement *XmlDocument::createElement() {

	if (expected(L_ARROW) && expected(IDENTIFIER)) {
		std::string tag = lastValue;
		std::cout << "child -> " << tag << std::endl;
		XmlElement *element = new XmlElement(tag);
		while (accept(IDENTIFIER)) {
			std::string attrib = lastValue;
			if (expected(EQUAL) && expected(STRING))
				element->addAttribute(attrib, lastValue);
		}
		if (accept(SLASH)) {	
			if (expected(R_ARROW))
				return element;
		} else if (expected(R_ARROW)) {
			std::cout << "to childs1" << std::endl;

			if (currentToken != L_ARROW) {
				std::string text;
				char c ='<';
				int v = EOF;
				while ((v = getc(file)) != EOF && (c = (char)v) != '<')
					text += v;
				currentValue = v;
				noConsumeNext = 1;
				if (text != "")
					element->setText(text);
			}

			while (currentToken == L_ARROW && nextTokenX != SLASH) {
				std::cout << "to childs2" << std::endl;
				XmlElement *child = createElement();
				if (child == nullptr)
					return element;
				element->addChild(child);
			}
			std::cout << "fin1: " << tag <<  std::endl;
			if (expected(L_ARROW) && expected(SLASH) && expected(IDENTIFIER) && tag == lastValue) {
				std::cout << "fin2 " << std::endl;
				if (expected(R_ARROW))
					return element;
			} else 
				return nullptr; // no tag match!
		}
	}
	return nullptr;
}


void XmlDocument::load(std::string path) {
	std::cout << "hereeeee path" << std::endl;
	file = fopen(path.c_str(), "r");
	std::cout << "File content" << std::endl;
	nextToken();
	nextToken();
	if (currentToken == L_ARROW)
		rootElement = createElement();
	std::cout << "End OF FILE IS: " << " " << EOF << std::endl;
	fclose(file);
}

std::string XmlDocument::print() {
	return "";
}

std::string XmlDocument::printBeauty() {
	return "";
}






XmlElement::XmlElement(std::string tag) {
	this->tag = new std::string(tag);
}
	
XmlElement::~XmlElement() {
	delete tag;
	if (children != nullptr) {
		for (int i = 0; i < getChildrenCount(); i++)
			delete *(children->begin() + i);
	}
	children->clear();
	delete children;

	if (attributes != nullptr) {
		delete attributes;
	}
}

XmlElement *XmlElement::addChild(XmlElement *child) {
	if (child->parent != nullptr)
		throw std::runtime_error("child " + *(child->tag) + " aready has a parent!");
	if (children == nullptr)
		children = new std::vector<XmlElement*>();
	children->push_back(child);
	return child;
}

XmlElement *XmlElement::removeChild(XmlElement *child) {
	if (children != nullptr) {
		auto it = children->begin();
		while (it != children->end()) {
			if (*it == child) {
				children->erase(it);
				break;
			}
		}
	}
	return child;
}

int XmlElement::getChildrenCount() {
	return children != nullptr ? (int)children->size() : 0;
}

std::vector<XmlElement*> *XmlElement::getChildren() {
	return children;
}


void XmlElement::addAttribute(std::string name, int value) {
	addAttribute(name, std::to_string(value));
}

void XmlElement::addAttribute(std::string name, float value) {
	addAttribute(name, std::to_string(value));
}

void XmlElement::addAttribute(std::string name, double value) {
	addAttribute(name, std::to_string(value));
}

void XmlElement::addAttribute(std::string name, std::string value) {
	if (attributes == nullptr) attributes = new std::map<std::string, std::string>();
	attributes->insert(std::map<std::string, std::string>::value_type(name, value));
}

void XmlElement::setText(std::string text) {
	this->text = new std::string(text);
}

std::string XmlElement::getTag() {
	return *tag;
}

bool XmlElement::hasAttribute(std::string name) {
	if (attributes == nullptr) return false;
	auto it = attributes->find(name);
	return it != attributes->end();
}

bool XmlElement::hasText() {
	return text != nullptr;
}

std::string XmlElement::getString(std::string name) {
	if (hasAttribute(name))
		return attributes->find(name)->second;
	else
		throw std::runtime_error("This element " + *tag + " has no attribute named " + name);
}

int XmlElement::getInt(std::string name) {
	return std::stoi(getString(name));
}

float XmlElement::getFloat(std::string name) {
	return std::stof(getString(name));
}

double XmlElement::getDouble(std::string name) {
	return std::stof(getString(name));
}