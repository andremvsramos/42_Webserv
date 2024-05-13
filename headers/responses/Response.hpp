/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:53:30 by andvieir          #+#    #+#             */
/*   Updated: 2024/03/13 15:53:30 by andvieir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../server/Server.hpp"

class Server;
class Request;

class Response {
	private:
		std::string _httpResponse;
		size_t	_indexSize;
		StringVector	_indexes;
		std::string		_rootPath;
		bool		_isAlias;
		bool		_HasRedirect;


	public:
		Response();
		Response(const Response& original);
		Response& operator=(const Response& original);
		~Response();

		bool	getRedirectFlag();
		std::string	getHTTPResponse() const;
		void	setHTTPResponse(std::string str);
		void	initFlags();

		size_t getIndexSize() const;
		StringVector getIndexes() const;
		std::string	getRootPath() const;
		void	redirectToURL(const std::string& url);
		std::string generateCodeMsg(int code);

		const std::string getErrorPage(int errorCode, const t_server_conf &serverConf);
		const std::string findRequestRoot(Server* server, const std::string& uri);
		LocationDir*	getDirectory(Server* server, const std::string& name);

		std::string	selectIndexFile(Server* server, int fd, const StringVector indexes, size_t size, const std::string& root, const std::string& uri, bool autoindex, const std::string& possibleIndex);
		void		sendResponse(Server* server, int fd, std::string file, int code);
		int			generateListingFile(Server* server, int fd, std::string location);
		
		void	sendResponseCGI(int read_fd, int write_fd, int clientSocket);
		
		void		reset();

		class ResponseException : public std::exception {
			private:
				std::string _errorMsg;
			public:
				ResponseException(const std::string& error);
				~ResponseException() throw();
				virtual const char* what() const throw();
		};
};

#endif
