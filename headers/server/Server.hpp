/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:36:38 by andvieir          #+#    #+#             */
/*   Updated: 2024/02/05 14:36:38 by andvieir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# pragma once
# include "../webserv.hpp"
# include "../structures.hpp"
# include "Connection.hpp"
# include "../requests/Request.hpp"

class Connection;
class Request;
class Response;

/**
 * @brief Class that manages a server for handling connections.
 *
 * The Server class encapsulates the functionality for setting up and managing
 * a server that handles connections. It provides methods for retrieving and
 * setting the socket descriptor and sockaddr_in structure.
 *
 * @note This class follows the Singleton pattern, ensuring only one instance
 * of the server is created.
 *
 * @param _socketfd
 *   The socket descriptor used to identify the server's endpoint for incoming
 *   connections.
 *
 * @param _sockaddr
 *   The sockaddr_in structure, which contains basic structures for all syscalls
 *   and functions that deal with internet addresses. It represents the address
 *   and port information of the server.
 */
class Server {

	private:
		Server();
		Server(const Server& original);
		Server& operator=(const Server& original);
		long						_socketfd;
		sockaddr_in					_sockaddr;
		t_listen					_listen;
		t_server_conf				_svConf;
		std::vector<std::string>	_body;
		bool						_isServerOn;
		bool						_GETAllowed;
		bool						_POSTAllowed;
		bool						_DELETEAllowed;
		std::vector<Connection>		_connections;
		t_cgi_env					_envp;
		bool						_isCGI;

	public:
		Server(const t_listen& listen);
		virtual ~Server();

		long	getFD();
		// sockaddr_in getAddr();
		std::vector<Connection>&	getOpenConnections();
		Connection&		getConnection(int toFind);
		std::vector<Connection>&	getConnectionVector();
		const std::vector<std::string>& getBody() const;
		std::vector<std::string>& getMutableBody();
		const t_server_conf&	getConf() const;
		t_server_conf&	getMutableConf() ;
		const t_listen&	getListen() const;
		t_listen&	getMutableListen() ;
		//Request&	getConnectionRequest(int fd);
		sockaddr_in*	getSockaddr();
		bool	getServerStatus() const;
		bool	isGETAllowed() const;
		bool	isPOSTAllowed() const;
		bool	isDELETEAllowed() const;

		void	setFD(long fd);
		void	setAddr();
		//void	setAddr(struct hostent* serverHost);
		void	setConnection(int connection);
		void	setNonBlock(int socket);

		void	setup();
		int		closer(int fd, int epoll_fd, struct epoll_event* event_buffer, std::map<int, Server*>& ServerMap, std::map<int, time_t>& TimeMap);
		//int		accept(Server* server, std::vector<struct pollfd>& _pollfds, std::map<int, Server*>& _fdToServerMap, size_t& addrlen);
		int		sender(int socket);
		int		testCGI(const std::string& uri, int fd, Request& req, Response& resp, int& reqCode);
		void	testCGI_DELETE(const std::string& uri, int fd, Request& req, Response& resp);
		int		testCGI_POST(const std::string& uri, int fd, Request& req, Response& resp, int& reqCode);
		int		testCGI_GET(const std::string& uri, int fd, Request& req, Response& resp);
		
		void    executePost(Request& req);
		void	executeDeleteFile();

		void	executeDeleteCGIScript(const std::string& scriptPath, Request& req, int fd, Response &resp);
		void	executeDeleteCGIScriptByGet(const std::string& scriptPath, int fd, Response &resp, std::string fileToDel);
		void	executeUploadCGIScript(const std::string& scriptPath, Request& req, int fd, Response &resp);
		void	executeCGIScript(const std::string& scriptPath, Request& req, int fd, Response &resp);

		int		curlyBracketsCheck();
		int		fillBody(std::istringstream& iss);

		class ServerException : public std::exception {
			private:
				std::string _errorMsg;
			public:
				ServerException(const std::string& error);
				~ServerException() throw();
				virtual const char* what() const throw();
		};
};

std::ostream& operator<<(std::ostream& os, const Server& server);

void    fillCGIEnvPOST(const t_server_conf& svConf, const std::string& uri, t_cgi_env& envp, Request& req);

#endif
