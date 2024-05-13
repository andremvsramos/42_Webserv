#include "../../headers/responses/ResponseCode.hpp"

/**
 * @brief Returns the corresponding status message for an HTTP status code.
 *
 * This function takes an HTTP status code as input and returns the corresponding
 * status message according to the HTTP specification. It covers a wide range
 * of status codes from informational, success, redirection, client error, to server error.
 * If the input status code doesn't match any known codes, it defaults to "OK".
 *
 * @param code The HTTP status code for which to retrieve the status message.
 * @return The corresponding status message for the given status code.
 */
std::string responseCode(int code)
{
	std::string msg;

	switch (code)
	{
		case 100:
			msg = "Continue";
			break;
		case 101:
			msg = "Switching Protocols";
			break;
		case 102:
			msg = "Processing";
			break;
		case 103:
			msg = "Early Hints";
			break;
		case 201:
			msg = "Created";
			break;
		case 202:
			msg = "Accepted";
			break;
		case 203:
			msg = "Non-Authoritative Information";
			break;
		case 204:
			msg = "No Content";
			break;
		case 205:
			msg = "Reset Content";
			break;
		case 206:
			msg = "Partial Content";
			break;
		case 207:
			msg = "Multi-Status";
			break;
		case 208:
			msg = "Already Reported";
			break;
		case 226:
			msg = "IM Used";
			break;
		case 300:
			msg = "Multiple Choices";
			break;
		case 301:
			msg = "Moved Permanently";
			break;
		case 302:
			msg = "Found";
			break;
		case 303:
			msg = "See Other";
			break;
		case 304:
			msg = "Not Modified";
			break;
		case 305:
			msg = "Use Proxy";
			break;
		case 306:
			msg = "unused";
			break;
		case 307:
			msg = "Temporary Redirect";
			break;
		case 308:
			msg = "Permanent Redirect";
			break;
		case 400:
			msg = "Bad Request";
			break;
		case 401:
			msg = "Unauthorized";
			break;
		case 402:
			msg = "Payment Required";
			break;
		case 403:
			msg = "Forbidden";
			break;
		case 404:
			msg = "Not Found";
			break;
		case 405:
			msg = "Method Not Allowed";
			break;
		case 406:
			msg = "Not Acceptable";
			break;
		case 407:
			msg = "Proxy Authentication Required";
			break;
		case 408:
			msg = "Request Timeout";
			break;
		case 409:
			msg = "Conflict";
			break;
		case 410:
			msg = "Gone";
			break;
		case 411:
			msg = "Length Required";
			break;
		case 412:
			msg = "Precondition Failed";
			break;
		case 413:
			msg = "Payload Too Large";
			break;
		case 414:
			msg = "URI Too Long";
			break;
		case 415:
			msg = "Unsupported Media Type";
			break;
		case 416:
			msg = "Range Not Satisfiable";
			break;
		case 417:
			msg = "Expectation Failed";
			break;
		case 418:
			msg = "I'm a teapot";
			break;
		case 421:
			msg = "Misdirected Request";
			break;
		case 422:
			msg = "Unprocessable Content";
			break;
		case 423:
			msg = "Locked";
			break;
		case 424:
			msg = "Failed Dependency";
			break;
		case 425:
			msg = "Too Early";
			break;
		case 426:
			msg = "Upgrade Required";
			break;
		case 428:
			msg = "Precondition Required";
			break;
		case 429:
			msg = "Too Many Requests";
			break;
		case 431:
			msg = "Request Header Fields Too Large";
			break;
		case 451:
			msg = "Unavailable For Legal Reasons";
			break;
		case 500:
			msg = "Internal Server Error";
			break;
		case 501:
			msg = "Not Implemented";
			break;
		case 502:
			msg = "Bad Gateway";
			break;
		case 503:
			msg = "Service Unavailable";
			break;
		case 504:
			msg = "Gateway Timeout";
			break;
		case 505:
			msg = "HTTP Version Not Supported";
			break;
		case 506:
			msg = "Variant Also Negotiates";
			break;
		case 507:
			msg = "Insufficient Storage";
			break;
		case 508:
			msg = "Loop Detected";
			break;
		case 510:
			msg = "Not Extended";
			break;
		case 511:
			msg = "Network Authentication Required";
			break;
		default:
			msg = "OK";
			break;
	}

	return msg;
}
