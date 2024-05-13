#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# pragma once
# include "../webserv.hpp"
# include "../structures.hpp"
# include "../requests/Request.hpp"
# include "../responses/Response.hpp"
# include "../responses/ResponseCode.hpp"

class Request;
class Response;

class Connection {

    private:
        Connection();
        Request     _request;
        Response    _response;
        int         _fd;

    public:
        Connection(const Connection& original);
        Connection& operator=(const Connection& original);
        explicit Connection(int fd);
        ~Connection();

        int         getConnectionFD() const;
        Request&    getConnectionRequest();
        Response&   getConnectionResponse();
};

#endif
