#include "Location.hpp"

typedef std::vector<std::string>::iterator v_iter_string;

// Default Constructor
ft::Location::Location() : _root(), _index(), _allowed_methods(), _cgi_extension(),
							_cgi_path(), _max_body(), _autoindex(false) {}

ft::Location::Location(v_iter_v_string& it, v_vec_string& content, std::string server_root) : _root(), _index(),
																			_allowed_methods(), _cgi_extension(), _cgi_path(),
																			 _max_body(), _autoindex(false) {
	// flag = checkk duplicate autoindex
	bool flag = false;
	_root = server_root;
	for (; it != content.end() && it->front() != "}"; ++it) {
		if (it->front() == "root") {
			if (_root != server_root) {
				throw ft::ParserException(RED "Parser Error:" REST " root in location is duplicated");
			}
			setRoot(*it);
		}
		if (it->front() == "index") {
			if (!_index.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " index in location is duplicated");
			}
			setIndex(*it);
		}
		if (it->front() == "allow_method") {
			if (!_allowed_methods.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " allow methods in location is duplicated");
			}
			setMethods(*it);
		}
		if (it->front() == "cgi_extension") {
			if (!_cgi_extension.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " cgi extension in location is duplicated");
			}
			setCgiExtension(*it);
		}
		if (it->front() == "cgi_path") {
			if (!_cgi_path.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " cgi path in location is duplicated");
			}
			setCgiPath(*it);
		}
		if (it->front() == "autoindex") {
			if (flag) {
				throw ft::ParserException(RED "Parser Error:" REST " autoindex in location is duplicated");
			}
			setAutoindex(*it);
			flag = true;
		}
		if (it->front() == "max_body_size") {
			if (!_max_body.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " max body size in location is duplicated");
			}
			setMaxBody(*it);
		}
	}
}

// Copy Constructor
ft::Location::Location(const ft::Location& other) {
	*this = other;
}

// Destructor
ft::Location::~Location() {}

// Assign
ft::Location& ft::Location::operator=(const ft::Location& other) {
	if (this != &other) {
		_root = other._root;
		_index = other._index;
		_allowed_methods = other._allowed_methods;
		_cgi_extension = other._cgi_extension;
		_cgi_path = other._cgi_path;
		_max_body = other._max_body;
		_autoindex = other._autoindex;
	}
	return *this;
}

std::string const ft::Location::getRoot(void) const {
	return this->_root;
}

std::vector<std::string> const ft::Location::getIndex(void) const {
	return this->_index;
}

std::vector<std::string> const ft::Location::getMethods(void) const {
	return this->_allowed_methods;
}

std::vector<std::string> const ft::Location::getCgiExtension(void) const {
	return this->_cgi_extension;
}
std::vector<std::string> const ft::Location::getCgiPath(void) const {
	return this->_cgi_path;
}

std::string const ft::Location::getMaxBody(void) const {
	return this->_max_body;
}

bool ft::Location::getAutoindex(void) const {
	return this->_autoindex;
}

void ft::Location::setRoot(const v_string& line) {
	_root += line[1];
	checkPath(_root, " incorrect root in location, path: ");
}

void ft::Location::setIndex(const v_string& line) {
	v_const_iter_string it = line.begin();
	++it;
	while (it != (line.end() - 1)) {
		_index.push_back(_root + *it);
		++it;
	}
}

void ft::Location::setMethods(const v_string& line) {
	v_const_iter_string it = line.begin();
	++it;
	while (it != (line.end() - 1)) {
		if (*it != "POST" && *it != "GET" && *it != "DELETE") {
			throw ft::ParserException(RED "Parser Error:" REST " allowed method " YEL + *it + REST " incorrect");
		}
			_allowed_methods.push_back(*it);
		++it;
	}
}

void ft::Location::setCgiExtension(const v_string& line) {
	v_const_iter_string it = line.begin();
	++it;
	while (it != (line.end() - 1)) {
		_cgi_extension.push_back(*it);
		++it;
	}
}

void ft::Location::setCgiPath(const v_string& line) {
	v_const_iter_string it = line.begin();
	++it;
	while (it != (line.end() - 1)) {
		_cgi_path.push_back(*it);
		++it;
	}
}

void ft::Location::setMaxBody(const v_string& line) {
	std::string value = line[1];
	size_t i = 0;
	while (i < value.length() - 1) {
		if (!isdigit(value[i])) {
			throw ft::ParserException(RED "Parser Error:" REST " max body size " YEL + value + REST " incorrect");
		}
		++i;
	}
	if (value[i] != 'm' && value[i] != 'b') {
		throw ft::ParserException(RED "Parser Error:" REST " max body size " YEL + value + REST " incorrect");
	}
	if (value[i] == 'm') {
		value.erase(value.length() - 1);
		value += "000";
	} else if (value[i] == 'b') {
		value.erase(value.length() - 1);
	}
	_max_body = value;
}

void ft::Location::setAutoindex(const v_string& line) {
	std::string value = line[1];
	if (value != "on" && value != "off") {
		throw ft::ParserException(RED "Parser Error:" REST " autoindex incorrect");
	}
	if (value == "on") {
		_autoindex = true;
	}
}