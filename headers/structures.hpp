/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 15:21:48 by andvieir          #+#    #+#             */
/*   Updated: 2024/02/06 15:21:48 by andvieir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_HPP
# define STRUCTURES_HPP

# include <iostream>
# include <string>

/* ===================== Location Structs ===================== */

/**
 * @brief Base class for location-related configurations.
 *
 * This class serves as the base class for location-related configurations.
 * It defines a virtual destructor to allow polymorphic behavior.
 */
struct LocationStruct {
	virtual ~LocationStruct() {}
};

/**
 * @brief Represents configuration for a file location.
 *
 * This struct represents configuration for a file location in the server block.
 * It inherits from LocationStruct and includes attributes such as the file name,
 * CGI pass information, and allowed HTTP methods.
 */
struct LocationFiles : LocationStruct {
	std::string					name;            /**< The name of the file location. */
	std::string					cgi_pass;        /**< The CGI pass information. */
	StringVector				allow_methods;   /**< The list of allowed HTTP methods. */
		LocationFiles() {}
		virtual ~LocationFiles() {
			allow_methods.clear();
		}
};

/**
 * @brief Represents configuration for a directory location.
 *
 * This struct represents configuration for a directory location in the server block.
 * It inherits from LocationStruct and includes attributes such as the directory name,
 * alias, redirection, root directory, allowed HTTP methods, index files, and a list
 * of nested files configurations.
 */
struct LocationDir : LocationStruct {
	std::string					name;            /**< The name of the directory location. */
	std::string					alias;           /**< The alias for the directory. */
	std::string					redirect;        /**< The redirection URL. */
	std::string					root;            /**< The root directory. */
	bool						autoindex;       /**< The autoindex setting. */
	StringVector				allow_methods;   /**< The list of allowed HTTP methods. */
	StringVector				index;           /**< The list of index files. */
	std::vector<LocationFiles*>	files;       	 /**< The list of nested files configurations. */
		LocationDir() : autoindex(false) {}           /* Constructor void */
		virtual ~LocationDir() {
			allow_methods.clear();
			index.clear();
			for (size_t i = 0; i < files.size(); i++)
				delete files[i];
		}                        /* Destructor */
};



/* ===================== Server Structs ===================== */

/**
 * @brief Represents configuration for a listen directive.
 *
 * This struct represents configuration for a listen directive in the server block.
 * It includes attributes such as the host and port to listen on.
 */
typedef struct s_listen {
	unsigned int	host;  /**< The host to listen on. */
	int				port;  /**< The port to listen on. */
} t_listen;

/**
 * @brief Represents the server configuration.
 *
 * This struct represents the server configuration containing various settings such
 * as server name, root directory, index files, allowed HTTP methods, error pages,
 * location configurations, redirection, autoindex setting, and maximum client
 * body size. It also includes the number of location structures configured.
 */
typedef struct s_server_conf {
	StringVector					server_name;            /**< The server names. */
	std::string						server_root;            /**< The server root directory. */
	StringVector					index;                  /**< The list of index files. */
	std::string						indexFile;
	bool							chunked_transfer_encoding; /*< Enabling processing chunked requests. */
	StringVector					allow_methods;          /**< The list of allowed HTTP methods. */
	std::map<int, std::string>		errorPages;             /**< The map of error pages. */
	unsigned int					client_max_body_size;   /**< The maximum client body size. */
	std::vector<LocationStruct*>	locationStruct;         /**< The list of location structures. */
		s_server_conf() : client_max_body_size(128) {}          /**< Constructor initializing numLocationStructs. */
		~s_server_conf() {
			server_name.clear();
			index.clear();
			allow_methods.clear();
			errorPages.clear();
			for (size_t i = 0; i < locationStruct.size(); i++)
				delete locationStruct[i];
		}
} t_server_conf;


typedef struct s_cgi_env {
	std::string auth_mode;
	std::string	content_length;
	std::string	content_type;
	std::string	gateway_interface;
	std::string	path_info;
	std::string	path_translated;
	std::string	query_string;
	std::string	remote_addr;
	std::string	remote_host;
	std::string	remote_ident;
	std::string	remote_user;
	std::string	request_method;
	std::string	script_name;
	std::string	server_name;
	std::string	server_port;
	std::string	server_protocol;
	std::string	server_software;
} t_cgi_env;

#endif
