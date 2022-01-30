#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Server.hpp"
#include "Config.hpp"
#include "parser.hpp"
#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <functional>

namespace ft
{
	class Location {
	private:
		std::string							_root;
		std::vector<std::string>			_index;
		std::vector<std::string>			_allowed_methods;
		std::vector<std::string>			_cgi_extension;
		std::vector<std::string>			_cgi_path;
		std::string							_max_body;
		bool								_autoindex;
		typedef std::vector<std::string>::iterator 						v_iter_string;
		typedef std::vector<std::string>::const_iterator 				v_const_iter_string;
		typedef std::vector<std::vector<std::string> >::iterator 		v_iter_v_string;
		typedef std::vector<std::vector<std::string> >::const_iterator 	v_const_iter_v_string;
		typedef std::vector<std::string> 								v_string;
		typedef std::vector<std::vector<std::string> > 					v_vec_string;
	public:
		Location(void);
		Location(v_iter_v_string& it, v_vec_string& content, std::string server_root);
		Location(const Location& other);
		~Location(void);
		Location& operator=(const Location& other);

		std::string const getRoot(void) const;
		std::vector<std::string> const getIndex(void) const;
		std::vector<std::string> const getMethods(void) const;
		std::vector<std::string> const getCgiExtension(void) const;
		std::vector<std::string> const getCgiPath(void) const;
		std::string const getMaxBody(void) const;
		bool getAutoindex(void) const;

		void setRoot(const v_string& line);
        void setIndex(const v_string& line);
		void setMethods(const v_string& line);
		void setCgiExtension(const v_string& line);
		void setCgiPath(const v_string& line);
		void setMaxBody(const v_string& line);
		void setAutoindex(const v_string& line);

	};
}



#endif