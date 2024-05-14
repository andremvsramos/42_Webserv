 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCluster.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:36:38 by andvieir          #+#    #+#             */
/*   Updated: 2024/02/05 14:36:38 by andvieir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCLUSTER_HPP
# define SERVERCLUSTER_HPP

# pragma once
# include "../webserv.hpp"
# include "../structures.hpp"
# include "../config/Config.hpp"
# include "Server.hpp"
# include "../requests/Request.hpp"

class Server;
class Config;

class	ServerCluster {

	private:
		ServerCluster();
		size_t	_nServ;
		std::vector<Server*> _servers;
		std::vector<pollfd> _pollfds;
		std::map<int, Server*> _fdToServerMap;
		std::map<int, time_t> _lastActivityTime;
		std::vector<int>	_serverSockets;
		Config	_config; // -> stack configs

	public:
		explicit ServerCluster(const std::string& filepath);
		ServerCluster(const ServerCluster& original);
		ServerCluster& operator=(const ServerCluster& original);
		~ServerCluster();

		void	setPollFD();

		Server* getServer();
		Server* getServer(int fd);
		std::vector<Server*>& getServers();

		void	SetupServers();
		void	ParseServer(Server* server);
		void	CreateNewServer(t_listen& listenStruct);
		void	SetupServerSockets();
		void	DisplayServerInfo();
		void	ClearServer();

		int	 	checkSocketActivity(int epoll_fd, struct epoll_event* event_buffer);

		void	verifySemiColon(Server* server);
		void	callConfigDoubleChecker(Server *server);
		void	fetchServerName(Server* server);
		void	fetchListen(std::string& wordStack, t_listen& listen);
		void	fetchServerRoot(Server* server);
		void	fetchErrorPage(Server* server);//percorrer com o iterador
		void	fetchIndex(Server* server);
		void	fetchMethods(Server* server);
		void	fetchClientSize(Server* server);
		void	fetchLocations(Server* server);
		void	config(std::string file_path);
		void	StartServers();
		void	connectionHandler(int fd, Server* server);

		class ServerClusterException : public std::exception {
			private:
				std::string _errorMsg;
			public:
				ServerClusterException(const std::string& error);
				~ServerClusterException() throw();
				virtual const char* what() const throw();
		};
};

#endif
