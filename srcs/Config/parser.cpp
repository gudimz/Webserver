#include "parser.hpp"

ft::ParserException::ParserException(std::string message) throw() : _message(message) {}

const char* ft::ParserException::what(void) const throw() {
	return _message.c_str();
}

ft::ParserException::~ParserException() throw() {}

void ft::split(v_string& content, const std::string& line, char c) {
	size_t size = line.size();
	size_t start = 0;
	(void)c;
	size_t end;
	while (start < size) {
		// skip whitespace and find the beginning of a word
		while (start < size && isspace(line[start])) {
			++start;
		}
		if (start == size) {
			break;
		}
		// find the end a word
		end = start + 1;
		while (end < size && !isspace(line[end])) {
			++end;
		}
		content.push_back(line.substr(start, end - start));
		start = end;
	}
}

void ft::lineJoin(std::string& line) {
	int pos = line.find('#');
	if (pos != -1) {
		line.erase(pos);
	}
	for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
		if (*it == '\n') {
			line.replace(it, it + 1, " ");
		} else if (*it == '\t') {
			line.replace(it, it + 1, " ");
		} else if (*it == '\r') {
			line.replace(it, it + 1, " ");
		}
	}
	pos = line.find(';');
	if (pos != -1) {
		line.insert(pos, 1, ' ');
	}
}

int ft::readFile(v_vec_string& content, char* path) {
	std::string line;
	std::string filename;
	v_string words;
    if (!path) {
        filename = "srcs/Config/file/default.conf";
    } else {
        filename = path;
    }
	std::ifstream file(filename.c_str());
	if (!file) {
		return 1;
	}
	while(std::getline(file, line)) {
		ft::lineJoin(line);
		if (!line.empty()) {
			ft::split(words, line, ' ');
			if (!words.empty()) {
				content.push_back(words);
				words.clear();
			}
		}
	}
	file.close();
	return 0;

}

void ft::checkValidPort(std::string& port) {
	// if there is no ',' at the end of the port
	if (port[port.length() - 1] != ',') {
		throw ft::ParserException(RED "Parser Error:" REST " port in config file is incorrect");
	}
	// all ports, except last
	port = port.substr(0, port.length() - 1);
}

std::vector<std::string> ft::checkMultiplePort(v_iter_v_string& it, const v_vec_string& content) {
	std::string value;
	std::vector<std::string> ret;
	for (v_iter_v_string iter = it; iter != content.end() && iter->front() != "}"; ++iter) {
		if (iter->front() == "listen") {
			// Next after the listen, i.e. port numbers
			v_iter_string ports = iter->begin() + 1;
			while (ports != iter->end() - 1) {
				ret.push_back(*ports);
				++ports;
			}
			break;
		}
	}
	if (ret.size() > 1) {
		for (v_iter_string it = ret.begin(); it != ret.end(); ++it) {
			// Because the last port number has no ','
			if (it != (ret.end() - 1)) {
				checkValidPort(*it);
			}
			for (std::string::iterator iter = (*it).begin(); iter != (*it).end(); ++iter) {
				if (!std::isdigit(*iter)) {
					throw ft::ParserException(RED "Parser Error:" REST " port in config file is incorrect");
				}
			}
		}
		// Check ports is not duplicated
		std::sort(ret.begin(), ret.end());
		v_iter_string iter = std::adjacent_find(ret.begin(), ret.end());
		if (iter != ret.end()) {
			throw ft::ParserException(RED "Parser Error:" REST " port in config file duplicated");
		}
	}
	return ret;
}

int ft::checkCountBrackets(const v_vec_string& content) {
	size_t left_br = 0;
	size_t right_br = 0;
	for (v_const_iter_v_string it = content.begin(); it != content.end(); ++it) {
		for (v_const_iter_string iter = it->begin(); iter != it->end(); ++iter) {
			if (*iter == "}") {
				++right_br;
			}
			if (*iter == "{") {
				++left_br;
			}
		}
	}
	if (right_br != left_br) {
		return 1;
	}
	return 0;
}

void ft::printErrParser(const v_string& line,  std::string message) {
	std::string text = " in line: " YEL;
	for (v_const_iter_string iter = line.begin(); iter != line.end(); ++iter) {
		text += *iter;
		text += " ";
	}
	throw ft::ParserException(RED "Parser Error: " REST + message + text + REST);
}

void ft::checkSemicolon(const v_vec_string& content) {
	for (v_const_iter_v_string it = content.begin(); it != content.end(); ++it) {
		if (it->front() ==  "server") {
			if (it->back() != "{") {
				printErrParser(*it, "bad config file");
			}
		} else if (it->front() ==  "location") {
			if (it->back() != "{") {
				printErrParser(*it, "bad config file");
			}
		} else if (it->front() ==  "}") {
			if (it.base()->size() != 1) {
				printErrParser(*it, "bad config file");
			}
		} else if (it->front() != "}" && it->front() !=  "server" && it->front() != "location") {
			if (it->back() != ";") {
				printErrParser(*it, "expected '"RED ";"REST "'");
			}
		}
	}

}

void ft::checkPath(std::string path, std::string message) {
	std::ifstream in(path.c_str());
		if (!in) {
			in.close();
			throw ft::ParserException(RED "Parser Error: " REST + message + YEL + path + REST " does not exist");
		}
		in.close();
}

void ft::checkContent(const v_vec_string& content) {
	// check count brackets in all lines
	if (checkCountBrackets(content)) {
			throw ft::ParserException(RED "Parser Error:" REST " invalid number of brackets");
	}
	bool server = false;
	bool listen = false;
	for (size_t i = 0; i < content.size(); ++i) {
		if (content[i][0] == "server") {
			server = true;
		}
		if (content[i][0] == "server") {
			listen = true;
		}
	}
	if (!server) {
		throw ft::ParserException(RED "Parser Error:" REST " not a server");
	}
	if (!listen) {
		throw ft::ParserException(RED "Parser Error:" REST " not a listen");
	}
	if (content.front().front() != "server") {
		throw ft::ParserException(RED "Parser Error:" REST " bad config file");
	}
	// check symbol ';' in line and number of parametrs
	ft::checkSemicolon(content);
	for (v_const_iter_v_string it = content.begin(); it != content.end(); ++it) {
		if (it->front() ==  "server" && it->size() != 2) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "listen" && it->size() < 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "host" && it->size() != 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "server_name" && it->size() > 3) {
			printErrParser(*it, "bad config file, incorrect value");
		} else if (it->front() ==  "root" && it->size() != 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "index" && it->size() < 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "error_page" && it->size() < 4) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "location" && it->size() != 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "allow_method" && it->size() < 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "max_body_size" && it->size() != 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "cgi_extension" && it->size() < 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "cgi_path" && it->size() < 3) {
			printErrParser(*it, "bad config file, missing value");
		} else if (it->front() ==  "autoindex" && it->size() != 3) {
			printErrParser(*it, "bad config file, missing value");
		}
	}
}

std::vector<ft::Config> ft::parser(char* path) {
	std::vector<ft::Config>configs;
	v_vec_string content;
	if (ft::readFile(content, path)) {
		throw ft::ParserException(RED "Parser Error:" REST " could not open file");
	}
	ft::checkContent(content);
	for (v_iter_v_string it = content.begin(); it != content.end(); ++it) {
		if (it->front() == "server") {
			++it;
			std::vector<std::string> multiplePorts = ft::checkMultiplePort(it, content);
			configs.push_back(ft::Config(it, content));
			if (multiplePorts.size() > 1) {
				// because first port has already been added to the config above
				size_t i = 1;
				while (i < multiplePorts.size()) {
					ft::Config newConfig = configs.back();
					newConfig.setPort(multiplePorts[i]);
					configs.push_back(newConfig);
					++i;
				}
			}
		}
	}
	return configs;
}
