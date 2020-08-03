#include "Trie.h"

#include <iostream>

TrieNode::TrieNode(char value, CommandType type, std::string command)
	: _value{value}, _type{type}, _command{command}, _frequency{1}
{

}

TrieNodeRef TrieNode::at(char child)
{
	if (this->_isChild(child))
		return this->_children[child];
	else
		return nullptr;
}

void TrieNode::_addChild(char child, CommandType type, std::string command)
{
	if (!this->_isChild(child))
		this->_children[child] = std::make_shared<TrieNode>(child,
															type,
															command);
}

bool TrieNode::_isChild(char child)
{
	return this->_children.find(child) != this->_children.end();
}

// Trie Implementation

Trie::Trie()
	: _root{ std::make_unique<TrieNode>('\0',
	                                    CommandType::UNDEFINED_COMMAND,"") }
{

}

Trie::~Trie()
{

}

void Trie::push(const std::string& value, CommandType command)
{
	TrieNodeRef node = this->_root;
	for (unsigned int i = 0; i < value.length(); i++) {
		while (node->_isChild(value.at(i))) {
			node = node->at(value.at(i++));
			node->_frequency++;
		}

		node->_addChild(value.at(i), command, value);
		node = node->at(value.at(i));
	}

	node->_isEndOfWord = true;
}

Command Trie::findShortestPrefix(const std::string& value) {
	TrieNodeRef node = this->_root;
	unsigned int i = 0;

	Command matched;

	while (i < value.size() && node->_isChild(value.at(i))) {
		node = node->at(value.at(i++));

		if (node->_frequency == 1) {
			// found unique path
			matched = Command(node->_command, node->_type);
		}
	}

	// check to see if the rest of the command is valid
	if (matched.type != CommandType::UNDEFINED_COMMAND
	    && matched.message.rfind(value, 0) == 0)
		return matched;

	return Command(); // default command
}

bool Trie::search(const std::string& value) {
	TrieNodeRef node = this->_root;

	unsigned int i = 0;
	while (i < value.size()) {
		if (!node->_isChild(value.at(i)))
			return false;
		node = node->at(value.at(i++));
	}

	return node->_isEndOfWord;
}
