#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <memory>

#include "Command.h"

class TrieNode
{
	friend class Trie;

public:
	TrieNode(char value, CommandType type, std::string command);

private:
	// data
	char _value;
	CommandType _type;
	std::string _command;

	int _frequency;
	bool _isEndOfWord;

	std::shared_ptr<TrieNode> at(char child);
	void _addChild(char child, CommandType command, std::string s);
	bool _isChild(char child);

	std::unordered_map<char, std::shared_ptr<TrieNode>> _children;
};

typedef std::shared_ptr<TrieNode> TrieNodeRef;

class Trie
{
public:
	Trie();
	~Trie();

	void push(const std::string& value, CommandType command);
	Command findShortestPrefix(const std::string& value);
	bool search(const std::string& value);

private:
	TrieNodeRef _root;
};

#endif // TRIE_H
