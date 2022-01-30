#include "Config.hpp"

// Default Constructor
ft::Config::Config() : _host(), _port(), _server_name(), _root(), _index(), _error_pages(), _locations() {}

ft::Config::Config(v_iter_v_string& it, v_vec_string& content) : _host(), _port(), _server_name(),
																		_root(), _index(), _error_pages(), _locations() {
	initErrPages();
	for (; it != content.end() - 1; ++it) {
		if (it->front() == "server") {
			--it;
			break;
		}
		if (it->front() == "listen") {
			if (!_port.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " host is duplicated");
			}
			setPort(*it);
		}
		if (it->front() == "host") {
			if (!_host.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " port is duplicated");
			}
			setHost(*it);
		}
		if (it->front() == "server_name") {
			if (!_server_name.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " server_name is duplicated");
			}
			setServName(*it);
		}
		if (it->front() == "index") {
			if (!_index.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST " index in service is duplicated");
			}
			setIndex(*it);
		}
		if (it->front() == "root")  {
			if (!_root.empty()) {
				throw ft::ParserException(RED "Parser Error:" REST ": root in service is duplicated");
			}
			setRoot(*it);
		}
		if (it->front() == "error_page") {
			setErrPages(*it);
		}
		if (it->front() == "location") {
			v_iterator_locationov iter = _locations.find(*(it->begin() + 1));
			if (iter != _locations.end()) {
				throw ft::ParserException(RED "Parser Error:" REST ": location is duplicated");
			}
			setLocation(it, content, _root);
		}
	}
}

// Copy Constructor
ft::Config::Config(const ft::Config& other) {
	*this = other;
}

// Destructor
ft::Config::~Config() {}

// Assign
ft::Config& ft::Config::operator=(const ft::Config& other) {
	if (this != &other) {
		_host = other._host;
		_port = other._port;
		_server_name = other._server_name;
		_root = other._root;
		_error_pages = other._error_pages;
		_locations = other._locations;
		_index = other._index;
	}
	return *this;
}

std::string const ft::Config::getHost(void) const {
	return this->_host;
}

std::string const ft::Config::getPort(void) const {
	return this->_port;
}

std::string const ft::Config::getServName(void) const {
	return this->_server_name;
}

std::string const ft::Config::getRoot(void) const {
	return this->_root;
}

std::vector<std::string> const ft::Config::getIndex(void) const {
	return this->_index;
}

std::string const ft::Config::getErrPages(int key) const {
	std::map<int, std::string>::const_iterator it;
	it = _error_pages.find(key);
	if (it == _error_pages.end()) {
		throw ft::ParserException("Error: bad config file(error_page)");
	}
	return (*it).second;
}

std::map<std::string, ft::Location> ft::Config::getLocation(void) const {
	return this->_locations;
}

std::map<std::string, ft::Location>::iterator ft::Config::findKeyLocation(std::string key) {
	std::map<std::string, ft::Location>::iterator it = _locations.find(key);
	if (it == _locations.end()) {
		throw ft::ParserException("Error: path to location not found");
	}
	return it;
}

void ft::Config::setHost(const v_string& line) {
	std::string value = line[1];
	_host = value;
	if (_host != "localhost") {
		size_t count = 0;
		// example .0.0.1  ||  127.0.0.
		if (*_host.begin() == '.' || *(_host.end() - 1) == '.')  {
			throw ft::ParserException(RED "Parser Error:" REST " host in config file is incorrect");
		}
		for (std::string::iterator it = _host.begin(); it != _host.end(); ++it) {
			if (*it == '.') {
				++count;
				if (!std::isdigit(*(it - 1))) {
					throw ft::ParserException(RED "Parser Error:" REST " host in config file is incorrect");
				}
			}
			if (!std::isdigit(*it) && *it != '.') {
				throw ft::ParserException(RED "Parser Error:" REST " host in config file is incorrect");
			}
		}
		if (count != 3) {
			throw ft::ParserException(RED "Parser Error:" REST " host in config file is incorrect");
		}
	}
}

void ft::Config::setPort(const std::string& port) {
	this->_port = port;
}

void ft::Config::setPort(const v_string& line) {
	size_t count = line.size() - 2;
	std::string value = line[1];
	if (count != 1) {
		// if there is no ',' at the end of the port
		if (value[value.length() - 1] != ',') {
			throw ft::ParserException(RED "Parser Error:" REST " port in config file is incorrect");
		}
		// all ports, except last
		value = value.substr(0, value.length() - 1);
	}
	for (std::string::iterator iter = value.begin(); iter != value.end(); ++iter) {
		if (!std::isdigit(*iter)) {
			throw ft::ParserException(RED "Parser Error:" REST " port in config file is incorrect");
		}
	}
	_port = value;
}

void ft::Config::setServName(const v_string& line) {
	_server_name = line[1];
}

void ft::Config::setRoot(const v_string& line) {
	char dir[1024];
	getcwd(dir, 1024);

	std::string value = line[1];
	_root = dir + value;
	checkPath(_root, " incorrect root in service, path: ");
}

void ft::Config::setIndex(const v_string& line) {
	v_const_iter_string it = line.begin();
	++it;
	std::string value;
	while (it != (line.end() - 1)) {
		_index.push_back(_root + *it);
		++it;
	}
}

void ft::Config::initErrPages(void) { // ЕРОРЫ
	char dir[1024];
	getcwd(dir, 1024);
	std::string root = dir;

	_error_pages[400] = root + "/srcs/www/default_pages/400.html";
	_error_pages[403] = root + "/srcs/www/default_pages/403.html";
	_error_pages[404] = root + "/srcs/www/default_pages/404.html";
	_error_pages[405] = root + "/srcs/www/default_pages/405.html";
	_error_pages[500] = root + "/srcs/www/default_pages/500.html";
	_error_pages[505] = root + "/srcs/www/default_pages/505.html";
	_error_pages[408] = root + "/srcs/www/default_pages/408.html";
	_error_pages[413] = root + "/srcs/www/default_pages/413.html";
}

void ft::Config::setErrPages(const v_string& line) {
	std::string key = line[1];
	std::string value = _root + line[2];
	checkPath(value, " incorrect error page file, path: ");
	map_iter_int_string it = _error_pages.find(atoi(key.c_str()));
	if (it != _error_pages.end()) {
		_error_pages[std::atoi(key.c_str())] = value;
	}
}

void ft::Config::setLocation(v_iter_v_string& it, v_vec_string& content, std::string server_root) {
	std::string key = *(it->begin() + 1);
	ft::Location newLocation(it, content, server_root);
	_locations.insert(std::make_pair(key, newLocation));
}

ft::Config::v_iterator_locationov ft::Config::getBeginLocation()
{
	return this->_locations.begin();
}

ft::Config::v_iterator_locationov ft::Config::getEndLocation()
{
	return this->_locations.end();
}