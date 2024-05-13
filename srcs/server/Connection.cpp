#include "../../headers/server/Connection.hpp"

/* ===================== Orthodox Canonical Form ===================== */

Connection::Connection() : _fd() {}

Connection::Connection(const Connection& original) {
    _request = original._request;
    _response = original._response;
    _fd = original._fd;
}

Connection& Connection::operator=(const Connection& original) {
    if (this != &original) {
        _request = original._request;
        _response = original._response;
        _fd = original._fd;
    }
    return *this;
}

Connection::~Connection() {}

/* ===================== Constructors ===================== */

Connection::Connection(int fd) : _fd(fd) {
	_request = Request();
}

/* ===================== Getter Functions ===================== */

int Connection::getConnectionFD() const {
    return _fd;
}

Request&    Connection::getConnectionRequest() {
    return _request;
}

Response&   Connection::getConnectionResponse() {
    return _response;
}
