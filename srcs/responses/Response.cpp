/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:56:19 by andvieir          #+#    #+#             */
/*   Updated: 2024/03/13 15:56:19 by andvieir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/server/Server.hpp"

/* ===================== Orthodox Canonical Form ===================== */

Response::Response() : _isAlias(false), _HasRedirect(false) {}

Response::Response(const Response& original) {
	_httpResponse = original._httpResponse;
}

Response& Response::operator=(const Response& original) {
	if (this != &original)
		_httpResponse = original._httpResponse;
	return *this;
}

Response::~Response() {}

/* ===================== Setter Functions ===================== */

void	Response::setHTTPResponse(std::string str) {
	_httpResponse = str;
}

void	Response::initFlags() {
	_isAlias = false;
	_HasRedirect = false;
}

/* ===================== Getter Functions ===================== */

bool	Response::getRedirectFlag() {
	return _HasRedirect;
}

std::string	Response::getHTTPResponse() const {
	return _httpResponse;
}

size_t	Response::getIndexSize() const {
	return _indexSize;
}

StringVector Response::getIndexes() const {
	return _indexes;
}

std::string	Response::getRootPath() const {
	return _rootPath;
}

std::string Response::generateCodeMsg(int code) {
	std::string msg = responseCode(code);
	return msg;
}

/* ===================== Attribute Functions ===================== */

/**
 * @brief Retrieves the error page associated with the given error code from the server configuration.
 *
 * This function searches for the error page corresponding to the provided error code in the server configuration.
 * If the error page is found, it returns the path to the error page file.
 * If the error page is not found, it returns the path to the default error page file.
 *
 * @param errorCode The HTTP error code.
 * @param serverConf The server configuration containing error pages mapping.
 * @return The path to the error page file.
 */
const std::string Response::getErrorPage(int errorCode, const t_server_conf &serverConf) {
	std::map<int, std::string>::const_iterator it = serverConf.errorPages.find(errorCode);
	if (it != serverConf.errorPages.end()) {
		std::string errorPageDir(it->second);
		size_t pos = errorPageDir.find("0x.html");
		size_t pos2 = errorPageDir.find("xx.html");
		std::string errorCodeStr = intToStr(errorCode);

		if (pos2 != std::string::npos) {
			errorCodeStr.erase(0, 1);
			errorPageDir.replace(pos2, 2, errorCodeStr);
		}
		else if (pos != std::string::npos)
			errorPageDir.replace(pos - 1, 3, errorCodeStr);

		std::string errorPage = serverConf.server_root + "errors" + errorPageDir;
		std::ifstream errorFile(errorPage.c_str());

		if (errorFile.is_open())
			return errorPage;
		else {
			std::cout << "Error: The error page (" << errorPage  << ") wasn't present in the server files.\nUsing DefaultErrorPage instead." << std::endl;
			return serverConf.server_root + "errors" + "/DefaultErrorPage.html";
		}
	}
	else {
		std::cout << "Error: The error code (" << errorCode << ") wasn't present in the config file.\nUsing DefaultErrorPage instead." << std::endl;
		return serverConf.server_root + "errors" + "/DefaultErrorPage.html";
	}
}

/**
 * @brief Finds the root directory associated with the requested URI.
 *
 * This function searches for the root directory associated with the requested URI in the server configuration.
 * It iterates through the location blocks to find the appropriate root directory.
 * If the requested URI matches a location block, it returns the corresponding root directory.
 * If no matching location block is found, it returns "404".
 *
 * @param server Pointer to the Server object containing the server configuration.
 * @param uri The requested URI.
 * @return The root directory associated with the requested URI.
 */
const std::string Response::findRequestRoot(Server* server, const std::string& uri) {
	std::vector<LocationStruct*>::const_iterator it;

	// Set default as 404
	std::string result = "404";

	// If uri is / it means we're at the server root
	if (uri == "/")
		result = "/";

	// Check if uri is present in any other location, even / , because locations have precedence
	for (it = server->getConf().locationStruct.begin(); it != server->getConf().locationStruct.end(); ++it) {
		LocationDir* dir = dynamic_cast<LocationDir*>(*it);
		if (dir && dir->name == uri) {

			// Check if alias is defined in config file and we've not setted result with the alias previously
			if (!dir->alias.empty() && !_isAlias) {
				result = dir->alias;
				_isAlias = true;
				break ;
			}

			// If alias is empty or we checked it previously, now we search if root is defined. If it is, disable alias
			if (!dir->root.empty()) {
				result = dir->root;
				_isAlias = false;
				break ;
			} // If root is empty then we check redirect
			else if (!dir->redirect.empty()) {
				result = dir->redirect;
				_HasRedirect = true;

				// Update _httpResponse object for redirect
				redirectToURL(result);
				if (_httpResponse == "400")
					return _HasRedirect = false, "400"; // Turn flag off so we don't trigger a redirect send in the sendResponse function
				break ;
			}
			else
				return "404";
		}
	}

	// After receiving the correct path, we manipulate it to have the correct syntax to add to our path
	if (result.find(".") == 0) {
		result.erase(0, 1);
		if (result.find("/") == 0)
			result.erase(0, 1);
	} else if (result.find("/") == 0)
		result.erase(0, 1);

	return result;
}

/**
 * @brief Retrieves the LocationDir object associated with the given name.
 *
 * This function searches for the LocationDir object with the specified name in the server configuration.
 * It iterates through the location blocks to find the LocationDir object with the matching name.
 * If found, it returns a pointer to the LocationDir object; otherwise, it returns NULL.
 *
 * @param server Pointer to the Server object containing the server configuration.
 * @param name The name of the LocationDir object to retrieve.
 * @return Pointer to the LocationDir object if found; otherwise, NULL.
 */
LocationDir*	Response::getDirectory(Server *server, const std::string& name) {
	std::vector<LocationStruct*>::const_iterator it;
	std::string result;
	for (it = server->getConf().locationStruct.begin(); it != server->getConf().locationStruct.end(); ++it) {
		LocationDir* dir = dynamic_cast<LocationDir*>(*it);
		if (dir && dir->name == name)
			return dir;
	}
	return NULL;
}

/**
 * @brief Selects the appropriate index file for the requested URI.
 *
 * This function selects the appropriate index file for the requested URI based on the server configuration.
 * It checks if directory listing is enabled and if the requested URI corresponds to a directory.
 * If directory listing is enabled and there are no index files defined, it returns "LIST" indicating directory listing.
 * If the requested URI corresponds to a file directly, it checks for the existence of the file.
 * If the file is found, it returns the file path.
 * If the file is not found, it recursively searches for index files in the directory's root path.
 *
 * @param server Pointer to the Server object containing the server configuration.
 * @param fd File descriptor.
 * @param indexes Vector of index files defined in the server configuration.
 * @param size Size of the indexes vector.
 * @param root Root directory path.
 * @param uri Requested URI.
 * @param autoindex Flag indicating if directory listing is enabled.
 * @param possibleIndex Possible index file.
 * @return The path to the selected index file or directory listing indicator.
 */
std::string	Response::selectIndexFile(Server* server, int fd, const StringVector indexes, size_t size, const std::string& root, const std::string& uri, bool autoindex, const std::string& possibleIndex) {
	std::string newroot;
	std::string locationRoot;

	// Check if directory listing is off and we're trying to access via directory
	LocationDir* dir = getDirectory(server, uri);
	if(dir) {
		if (!autoindex && possibleIndex.empty() && dir->index.size() == 0)
			return "404";

		// If directory listing is on and there are no index files defined in the location block, list the files present within
		if (autoindex && possibleIndex.empty() && dir->index.size() == 0)
			return "LIST";
	}

	// In case we're trying to access the file directly, check if it exists, or is accessible
	if (!possibleIndex.empty()) {

		// If Alias is defined, searches there first
		std::ifstream htmlFile((root + possibleIndex).c_str());
		if (htmlFile.is_open()) {
			server->getMutableConf().indexFile = possibleIndex;
			htmlFile.close();
			return root;
		}
		// if not found in alias, try root
		else if (_isAlias) {
			locationRoot = findRequestRoot(server, uri);
			if (locationRoot == "404" || locationRoot == "400")
				return locationRoot;
			else if (_HasRedirect)
				return "REDIRECT"; // Useless return, but it's ok

			// Recursively search for the index file, now in root
			newroot = selectIndexFile(server, fd, indexes, size, (server->getConf().server_root + locationRoot), uri, autoindex, possibleIndex);
				return newroot;
		}

	// Try to access via directory when listing is ON
	} else {

		// Search for the first OK index file provided
		for (size_t i = 0; i < size; i++) {
			std::string filename = indexes[i];
			std::ifstream htmlFile((root + filename).c_str());
			if (htmlFile.is_open()) {
				server->getMutableConf().indexFile = filename;
				htmlFile.close();
				return root;
			}

			// If we didn't find the index in the alias path, then check the subdirectory's root path
			if (i + 1 >= size && _isAlias) {
				// This will fetch the root path since we already have tried alias once
				locationRoot = findRequestRoot(server, uri);
				// We add these checks here because root may also return an error if the file isn't present or we're dealing with a redirect
				if (locationRoot == "404" || locationRoot == "400")
					return locationRoot;
				else if (_HasRedirect)
					return "REDIRECT"; // Useless return, but it's ok
				// Recursively search for the index file, now in root
				newroot = selectIndexFile(server, fd, indexes, size, (server->getConf().server_root + locationRoot), uri, autoindex, possibleIndex);
			}
		}
	}

	// Final check if no root is found
	if (newroot.empty())
		return "404";

	// Will only reach this return if we're recursively search root after alias
	return newroot;
}

/* ===================== Directory Listing ===================== */

/**
 * @brief Generates a temporary HTML directory listing file and sends it as a response to the client.
 *
 * This function creates a temporary HTML directory listing file containing the contents of the specified directory.
 * It then sends the directory listing file as a response to the client with the provided HTTP status code.
 * After sending the response, it removes the temporary file.
 *
 * @param server Pointer to the Server object.
 * @param fd File descriptor of the client socket.
 * @param location Path of the directory to generate the listing for.
 * @return The number of files listed in the directory.
 */
int	Response::generateListingFile(Server* server, int fd, std::string location) {
	std::string		filename(".directorylist.html");
	std::string		path("var/www/html/" + filename);
	std::ofstream	tmp(path.c_str());

	// Create temporary HTML directory listing file and count how many files were found in the directory
	int nFiles = 0;
	nFiles = createListHTML(location, tmp);
	tmp.close();

	// Send the list
	sendResponse(server, fd, path, 200);

	// Remove the temp file
	if (std::remove(path.c_str()) != 0) {
        // Handle error if unable to delete file
        std::cerr << RED << "Error: Unable to delete file " << path << RESET << std::endl;
    }

	return nFiles;
}

/* ===================== Response Management Functions ===================== */

/**
 * @brief Sends an HTTP response to the client.
 *
 * This function sends an HTTP response to the client with the provided file content and HTTP status code.
 * If the response is a redirect, it sends the redirect response and closes the connection.
 * If the response is not a redirect, it reads the content from the specified file and sends it as the response.
 *
 * @param server Pointer to the Server object.
 * @param fd File descriptor of the client socket.
 * @param file Path of the file containing the response content.
 * @param code HTTP status code.
 */
void	Response::sendResponse(Server* server, int fd, std::string file, int code) {
	std::string response;
	(void)server;
	// If we're redirecting call _httpResponse from class, send the response and close it. It may be an exterior domain and we have no need to "control" those
	if (_HasRedirect && code == 200) {
		response = _httpResponse;
		send(fd, response.c_str(), response.size(), 0);
		close(fd);
	}
	else {

		// Check if we can open the file, we already did this in selectIndexFile, but it's a good practice
		std::ifstream htmlFile((file).c_str());
		if (htmlFile.is_open()) {

			// Read the html file and add it to our response headers with the received code and it's appropriate message
			std::stringstream ss;
			ss << htmlFile.rdbuf();
			response = ss.str();
			htmlFile.close();
			std::stringstream headers;
			headers <<	"HTTP/1.1 " << code << " " << generateCodeMsg(code) << "\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " << response.size() << "\r\n"
						"Cache-Control: no-cache, private \r\n"
						"\r\n";

			std::string headersStr = headers.str();
			response = headersStr + response;
			// Send the response
			send(fd, response.c_str(), response.size(), 0);
		}
		else {
			throw ResponseException("HTML file doesn't exist or is inaccessible.");
		}
	}
	gFullRequest.clear();
}

/**
 * @brief Sends an HTTP response containing the content of a CGI script to the client.
 *
 * This function sends an HTTP response to the client with the content received from a CGI script.
 * It reads the content from the specified file descriptor and sends it as the response body.
 *
 * @param read_fd File descriptor for reading from the CGI script.
 * @param write_fd File descriptor for writing to the CGI script.
 * @param clientSocket File descriptor of the client socket.
 */
void	Response::sendResponseCGI(int read_fd, int write_fd, int clientSocket) {

	// Define a buffer for reading
    const size_t bufferSize = 1024;
    char buffer[bufferSize];

    // Create an empty std::string to hold the content
    std::string content;

    // Read the contents of the file descriptor into the buffer
    ssize_t bytesRead;
    while ((bytesRead = read(read_fd, buffer, bufferSize)) > 0) {
        // Append the data read to the std::string object
        content.append(buffer, bytesRead);
		close(write_fd);
    }
	std::stringstream headers;
	headers <<	"HTTP/1.1 " << "200" << " " << generateCodeMsg(200) << "\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: " << content.size() << "\r\n\r\n";
	std::string headersStr = headers.str();
	content = headersStr + content;
	send(clientSocket, content.c_str(), content.size(), 0); // Send to client
}

/**
 * @brief Sets the redirect URL for the response.
 *
 * This function sets the redirect URL for the HTTP response.
 * If the provided URL is a relative path, it adds the server's root URL.
 * If the URL is invalid, it updates the HTTP response to generate a 400 Bad Request error.
 *
 * @param url The URL to redirect to.
 */
void	Response::redirectToURL(const std::string& url){
	std::string rdc;
	std::string url_tmp(url);
	// Checking if we're setting the redirect URL correctly

	// If we find localhost at the beginning of the redirect, then we say the redirect will be either root '/' or a location '/....'
	if (url_tmp.find("localhost") == 0) {
		if (url_tmp.find("/") != url_tmp.length() - 1)
			url_tmp.append("/");
		rdc = url_tmp.substr(url_tmp.find("/"), std::string::npos);
	} else if (url_tmp.find("http") == 0)	// If we are doing a redirect to another domain or website, then the url needs to be preceded by http or https
		rdc = url_tmp;
	else {
		_httpResponse = "400";	// If the previous rules aren't followed we update the response to generate a 400 Bad Request Error
		return ;
	};

	// If succesfull we update the response header with the correct redirect URL
	_httpResponse = "HTTP/1.1 302 Found\r\nLocation: " + rdc + "\r\n\r\n";
}


/* ===================== Reset Function ===================== */

/**
 * @brief Resets the Response object's member variables to their initial states.
 *
 * This function resets the member variables of the Response object to their initial states.
 * It is typically called after sending a response to prepare the object for processing the next request.
 */
void	Response::reset() {
	_indexSize = 0;
	_indexes.clear();
	_rootPath.clear();
	_httpResponse.clear();
}

/* ===================== Exceptions ===================== */

Response::ResponseException::ResponseException(const std::string& error) {
	std::ostringstream err;
	err << BOLD << RED << "Error: " << error << RESET;
	_errorMsg = err.str();
}

Response::ResponseException::~ResponseException() throw() {}

/**
 * @brief Returns the error message associated with this exception.
 *
 * @return The error message as a C-style string.
 */
const char *Response::ResponseException::what() const throw() {
	return _errorMsg.c_str();
}
