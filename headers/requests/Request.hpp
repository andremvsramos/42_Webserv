/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:15:10 by andvieir          #+#    #+#             */
/*   Updated: 2024/03/05 15:15:10 by andvieir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# pragma once
# include "../webserv.hpp"
# include "../server/Server.hpp"

class Server;

class Request {

	private:		
		std::string _method;
		std::string _uri;
		std::string _httpVersion;
		std::string	_firstLineRequest;
		std::string _fullRequest;
		std::string _requestHeader;
		std::string _requestBody; //std::vector<char>
		std::string _contentType;
		std::string _contentValue;
		std::string _boundary;
		std::string _contentLength;
		std::string _filename;
		std::string _host;


	public:
		bool		_isChunked;
		bool		_isRequestComplete;
		Request();
		Request(const Request& original);
		Request& operator=(const Request& original);
		~Request();

		int		fillRequestHeader(int socket); //request loop
		int		fillRequestAtributes(const std::string& request);
		void	chunkedHandler(int fd);

		void	parseFullRequest();
		std::string clearValue(std::string toBeTrimmed);
		std::string parseFilename(std::string ClearDisposition);
		void parseContentType(std::string ContentType);
		std::string extractBody();

		std::string getHeaderValue(const std::string& headerName);
		bool validateRequestMethod(Server* server);
		std::string	getReqMethod() const;
		std::string	getReqUri() const;
		std::string	getReqHVersion() const;
		std::string	getReqContentLength() const;
		std::string	getReqContentType() const;
		std::string	getReqFilename() const;
		std::string	getReqbody() const;
		std::string	getReqHost() const;
		bool		isChunked()	const;
		bool		isRequestComplete() const;

		int		parseRequest(Server *server);
		void	unChunk();
		void	RequestLogger(std::string request);
		int		checkClientSize(Server* server);
		double		getContentLength();
		
		class RequestFileException : public std::exception {
			private:
				std::string _errorMsg;
			public:
				RequestFileException(const std::string& error);
				~RequestFileException() throw();
				virtual const char* what() const throw();
		};
};


#endif
