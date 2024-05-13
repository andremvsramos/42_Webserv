/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-sous <lde-sous@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:36:38 by lde-sous          #+#    #+#             */
/*   Updated: 2024/02/05 16:36:38 by lde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# pragma once
# include "../webserv.hpp"
# include "../structures.hpp"
# include "../server/Server.hpp"
#include <filesystem>

class Server;

class Config {

	private:
		std::stack<std::string> _serverBlocks;
		std::vector<int>		_usedPorts;

	public:
		Config();
		Config(const std::string& filePath);
		Config(const Config& original);
		Config& operator=(const Config& original);
		~Config();

		std::stack<std::string>& getServerBlocks();
		void setServerBlocks(std::ifstream& file);

		void	parseSemicolon(StringVector& body);
		void	parseServerName(StringVector& body, t_server_conf& conf);
		void	parseListen(std::istringstream& iss, t_listen& listen);
		void	parseServerRoot(StringVector& body, t_server_conf& conf);
		void	parseErrorPage(StringVector& body, t_server_conf& conf);
		void	parseIndex(StringVector& body, t_server_conf& conf);
		void	parseMethods(StringVector& body, t_server_conf& conf);
		void	parseClientSize(StringVector &body, t_server_conf &conf);
		void	parseLocations(Server* server, StringVector& body, t_server_conf& conf);
		int		checkMandatoryKeywords(StringVector& body);
		int		setKeywordValue(std::string type, StringVector key, LocationStruct& strc);
		void	checkDoubles(std::vector<std::string>& body);
		void	printLogs(Server* server, t_server_conf& conf);


		class ConfigFileException : public std::exception {
			private:
				std::string _errorMsg;
			public:
				ConfigFileException(const std::string& error);
				~ConfigFileException() throw();
				virtual const char* what() const throw();
		};

};

#endif
